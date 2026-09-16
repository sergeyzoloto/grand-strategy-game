#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "sim/bipolar.hpp"
#include "sim/character.hpp"
#include "sim/date.hpp"
#include "sim/dead_record.hpp"
#include "sim/edit_result.hpp"
#include "sim/fixed_vector.hpp"
#include "sim/ids.hpp"
#include "sim/integral.hpp"
#include "sim/noise.hpp"
#include "sim/opinion_config.hpp"
#include "sim/opinion_records.hpp"
#include "sim/relation_graph.hpp"
#include "sim/relations.hpp"
#include "sim/stance_table.hpp"

namespace sim {

// The only creator of characters, the owner of their relations, and the keeper of the dead.
//
// Ids start at 1, increase by one and are never reused. A living character's own values
// (condition, traits, lists) are edited through find(); relations and personal opinions
// only through the registry. kill() turns a living character into a DeadRecord.
//
// Storage: living characters in a vector sorted by id; dead records in a vector in order
// of death (append-only); one slot per id ever created, naming a living index or a dead
// index. find() and find_dead() are O(1). The relation graph keeps a node for every id.
//
// Lifetimes: any pointer, reference or span obtained from the registry or from a
// Character is valid only until the next registry mutation (create, kill, or any
// relation or personal opinion edit), because the registry may reallocate or relocate
// characters. Returned vectors are independent copies.
class CharacterRegistry {
public:
    // Creates a living character and returns its id. Returns CharacterId{} only if ids are
    // exhausted (2^31 - 1). May throw bad_alloc, in which case nothing is created.
    [[nodiscard]] CharacterId create(NameId name, Gender gender, Date birth, const CharacterInit& init);

    // Kills a living character at `death`:
    //  1. every living character's long-term entry about it is rebased by
    //     round(weak_before.total - weak_after.total), where weak_after uses the DeadRecord
    //     (an entry reaching 0 is removed), so opinions don't jump;
    //  2. paired relations that don't survive death are unlinked on both sides, one-way
    //     relations from and to it are cleared; family ties stay;
    //  3. its own long-term entries and modifiers are discarded; modifiers of others about
    //     it stay;
    //  4. the Character is replaced by a DeadRecord; later living slots are relocated.
    // Invalid: id 0, or death < birth. NotFound: an unknown id, or one already dead (kill
    // acts only on the living; other edits return Invalid for a dead participant).
    // Order: Invalid (id 0), NotFound, Invalid (death < birth). May throw bad_alloc before
    // any write, in which case nothing changes.
    [[nodiscard]] EditResult kill(CharacterId id, Date death, const StanceTable& stances, const OpinionConfig& config,
                                  WorldSeed seed);

    // Living characters only: nullptr for id 0, an unknown id or a dead one.
    [[nodiscard]] Character* find(CharacterId id) noexcept;
    [[nodiscard]] const Character* find(CharacterId id) const noexcept;
    // Dead characters only: nullptr for id 0, an unknown id or a living one.
    [[nodiscard]] const DeadRecord* find_dead(CharacterId id) const noexcept;
    // True for any id ever created, living or dead.
    [[nodiscard]] bool exists(CharacterId id) const noexcept { return id.valid() && id.value <= slots_.size(); }

    // Living characters in id order.
    [[nodiscard]] std::span<Character> characters() noexcept { return living_; }
    [[nodiscard]] std::span<const Character> characters() const noexcept { return living_; }
    [[nodiscard]] std::size_t size() const noexcept { return living_.size(); }
    [[nodiscard]] std::size_t dead_count() const noexcept { return dead_.size(); }

    // ---- relations: see RelationGraph and EditResult for the rules ----
    // Check order: Invalid (id 0, a == b, unknown type or wrong kind), NotFound (an id never
    // created), Invalid (a dead participant where not allowed), then the graph's own results.

    // One-way types only (Friend, Rival, Attraction); touch the edge a -> b only. A dead
    // participant is Invalid: the dead hold and receive no one-way relations.
    [[nodiscard]] EditResult set_relation(CharacterId a, RelationType type, CharacterId b);
    [[nodiscard]] EditResult clear_relation(CharacterId a, RelationType type, CharacterId b) noexcept;
    // Paired types only; both sides at once. link(a, Child, b) reads "b is a's child".
    // Conflict if an edge would carry a type and its complement; Full if the child
    // would get more than MAX_PARENTS parents (dead parents count). A dead participant is
    // allowed only for types that survive death (posthumous links, even between two dead).
    [[nodiscard]] EditResult link(CharacterId a, RelationType type, CharacterId b);
    [[nodiscard]] EditResult unlink(CharacterId a, RelationType type, CharacterId b) noexcept;

    [[nodiscard]] bool has_relation(CharacterId a, RelationType type, CharacterId b) const noexcept {
        return relations_.has_relation(a, type, b);
    }
    // Edges from a, sorted by other id; empty for an invalid or unknown id. Works for dead ids.
    [[nodiscard]] std::span<const RelationEdge> relations(CharacterId a) const noexcept {
        return relations_.relations(a);
    }
    // Queries below work for living and dead ids and may return dead ids: callers check
    // find() before treating one as living. In id order.
    [[nodiscard]] FixedVector<CharacterId, MAX_PARENTS> parents(CharacterId a) const noexcept {
        return relations_.parents(a);
    }
    [[nodiscard]] std::vector<CharacterId> children(CharacterId a) const { return relations_.children(a); }
    // Characters sharing at least one parent with a; excludes a, no duplicates, id order.
    [[nodiscard]] std::vector<CharacterId> siblings(CharacterId a) const { return relations_.siblings(a); }
    // 0, 1 or 2 (2: full siblings, 1: half siblings); 0 for invalid or unknown ids or a == b.
    [[nodiscard]] int shared_parents(CharacterId a, CharacterId b) const noexcept {
        return relations_.shared_parents(a, b);
    }

    // ---- personal opinions: the only way to change them ----
    // Check order: Invalid (id 0, a == b, an invalid TargetId or ModifierId 0), NotFound
    // (an id never created), Invalid (a dead holder), then the list's own results. A dead
    // target is allowed. Nothing depends on time.

    // Adds modifier `modifier` of a about b with a fixed effect, clamped to -100..+100.
    // Duplicate if a has it for that target already; Full at MODIFIER_CAP (never evicts).
    [[nodiscard]] EditResult add_modifier(CharacterId a, CharacterId b, ModifierId modifier,
                                          StrictIntegral auto effect) noexcept {
        return add_person_modifier(a, b, modifier, detail::clamp_bipolar(effect));
    }
    [[nodiscard]] EditResult add_modifier(CharacterId a, TargetId target, ModifierId modifier,
                                          StrictIntegral auto effect) noexcept {
        return add_target_modifier(a, target, modifier, detail::clamp_bipolar(effect));
    }
    // Ok, Invalid or NotFound (an unknown character or no such modifier).
    [[nodiscard]] EditResult remove_modifier(CharacterId a, CharacterId b, ModifierId modifier) noexcept;
    [[nodiscard]] EditResult remove_modifier(CharacterId a, TargetId target, ModifierId modifier) noexcept;

    // Adds `delta` (clamped to -400..+400) to a's long-term opinion of the target; the
    // result is clamped to -200..+200. See LongOpinionOutcome.
    [[nodiscard]] LongOpinionResult<CharacterId> add_long_opinion(CharacterId a, CharacterId b,
                                                                  StrictIntegral auto delta) noexcept {
        return add_person_long(a, b, detail::clamp_integer(delta, -LONG_DELTA_MAX, LONG_DELTA_MAX));
    }
    [[nodiscard]] LongOpinionResult<TargetId> add_long_opinion(CharacterId a, TargetId target,
                                                               StrictIntegral auto delta) noexcept {
        return add_target_long(a, target, detail::clamp_integer(delta, -LONG_DELTA_MAX, LONG_DELTA_MAX));
    }
    // One pass over living characters in id order: evicts the weakest long-term people
    // entries of lists above their limit (after extraversion dropped). Returns the number
    // evicted. Idempotent.
    [[nodiscard]] std::size_t maintain() noexcept;

    // Bytes held by the registry and its vectors (capacity, not size), excluding
    // allocator headers.
    [[nodiscard]] std::size_t allocated_bytes() const noexcept;
    // Bytes held by relation storage only (RelationGraph::allocated_bytes()).
    [[nodiscard]] std::size_t relation_bytes() const noexcept { return relations_.allocated_bytes(); }
    // Bytes held by dead records and by the per-id slots (capacity).
    [[nodiscard]] std::size_t dead_record_bytes() const noexcept { return dead_.capacity() * sizeof(DeadRecord); }
    [[nodiscard]] std::size_t slot_bytes() const noexcept { return slots_.capacity() * sizeof(std::uint32_t); }

private:
    // Slot values: a living index, or DEAD_FLAG | dead index.
    static constexpr std::uint32_t DEAD_FLAG = std::uint32_t{1} << 31;

    [[nodiscard]] bool is_dead(CharacterId id) const noexcept {
        assert(exists(id));
        return (slots_[id.value - 1] & DEAD_FLAG) != 0;
    }
    [[nodiscard]] EditResult check_relation_edit(CharacterId a, RelationType type, CharacterId b,
                                                 RelationKind kind) const noexcept;
    [[nodiscard]] EditResult add_person_modifier(CharacterId a, CharacterId b, ModifierId modifier,
                                                 int effect) noexcept;
    [[nodiscard]] EditResult add_target_modifier(CharacterId a, TargetId target, ModifierId modifier,
                                                 int effect) noexcept;
    [[nodiscard]] LongOpinionResult<CharacterId> add_person_long(CharacterId a, CharacterId b, int delta) noexcept;
    [[nodiscard]] LongOpinionResult<TargetId> add_target_long(CharacterId a, TargetId target, int delta) noexcept;

    std::vector<Character> living_;     // sorted by id
    std::vector<DeadRecord> dead_;      // in order of death, append-only
    std::vector<std::uint32_t> slots_;  // index = id - 1: living index, or DEAD_FLAG | dead index
    RelationGraph relations_;           // one node per id ever created, same indexing as slots_
};

} // namespace sim
