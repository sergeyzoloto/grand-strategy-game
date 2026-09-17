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
#include "sim/world_context.hpp"

namespace sim {

// Result of CharacterRegistry::kill. fame and legendary are meaningful only when result is Ok.
struct KillResult {
    EditResult result = EditResult::Invalid;
    std::uint16_t fame = 0;  // as stored in the DeadRecord
    bool legendary = false;  // fame >= LifecycleConfig::legendary_fame
};

// The only creator of characters, the owner of their relations, and the keeper of the dead.
//
// Ids start at 1, increase by one and are never reused. A living character's own values
// (condition, traits, lists) are edited through find(); relations and personal opinions
// only through the registry. kill() turns a living character into a DeadRecord, and a dead
// character is forgotten (its record deleted) once no living character remembers it.
//
// Memory: holders(X) counts the distinct living characters holding at least one reference
// to X: a long-term entry about X, a modifier with X as its person target, or an edge in
// their own edge list pointing at X. The dead hold no references. The count is kept
// incrementally by every registry edit, which compares "A references X" before and after;
// every mutation that can change a reference is registry-only or CharacterKey-gated, so the
// counts cannot go out of sync. A change that drops a dead character's holders to 0 forgets
// it at once (in id order when one change forgets several): its record is deleted, its
// remaining edges (all with other dead characters) are removed on both sides and its id
// behaves like one never created. Forgetting never cascades and never allocates.
//
// Storage: living characters in a vector sorted by id; dead records in a vector (deletion
// swaps in the last one); one slot and one holders count per id ever created. find(),
// find_dead() and holders() are O(1). The relation graph keeps a node for every id.
//
// Lifetimes: any pointer, reference or span obtained from the registry or from a
// Character is valid only until the next registry mutation (create, kill, maintain, or any
// relation or personal opinion edit), because the registry may reallocate or relocate
// characters or shift list entries. Returned vectors are independent copies.
class CharacterRegistry {
public:
    // Creates a living character and returns its id. Returns CharacterId{} only if ids are
    // exhausted (2^31 - 1). May throw bad_alloc, in which case nothing is created.
    [[nodiscard]] CharacterId create(NameId name, Gender gender, Date birth, const CharacterInit& init);

    // Kills a living character at `death` and returns its fame:
    //  0. fame = holders just before the kill + fame_per_reputation * |reputation|, saturating;
    //  1. every living character's long-term entry about it is rebased by
    //     round(weak_before.total - weak_after.total), where weak_after uses the DeadRecord
    //     (an entry reaching 0 is removed), so opinions don't jump;
    //  2. paired relations that don't survive death are unlinked on both sides, one-way
    //     relations from and to it are cleared; family ties stay;
    //  3. its own long-term entries and modifiers are discarded; modifiers of others about
    //     it stay;
    //  4. the Character is replaced by a DeadRecord; later living slots are relocated;
    //  5. the deceased's own references stop counting; then the deceased (if nobody holds
    //     it) and dead characters only it held are forgotten, in id order.
    // Invalid: id 0, or death < birth. NotFound: an unknown or forgotten id, or one already
    // dead (kill acts only on the living; other edits return Invalid for a dead participant).
    // Order: Invalid (id 0), NotFound, Invalid (death < birth). May throw bad_alloc before
    // any write, in which case nothing changes.
    [[nodiscard]] KillResult kill(CharacterId id, Date death, const WorldContext& context);

    // Living characters only: nullptr for id 0, an unknown id or a dead one.
    [[nodiscard]] Character* find(CharacterId id) noexcept;
    [[nodiscard]] const Character* find(CharacterId id) const noexcept;
    // Dead characters only: nullptr for id 0, an unknown id or a living one.
    [[nodiscard]] const DeadRecord* find_dead(CharacterId id) const noexcept;
    // True for a living or dead id; false for id 0, an id never created, or a forgotten one.
    [[nodiscard]] bool exists(CharacterId id) const noexcept {
        return id.valid() && id.value <= slots_.size() && slots_[id.value - 1] != FORGOTTEN;
    }
    // Distinct living characters holding a reference to id; 0 for unknown or forgotten ids.
    [[nodiscard]] std::uint32_t holders(CharacterId id) const noexcept {
        return exists(id) ? holders_[id.value - 1] : 0;
    }

    // Living characters in id order.
    [[nodiscard]] std::span<Character> characters() noexcept { return living_; }
    [[nodiscard]] std::span<const Character> characters() const noexcept { return living_; }
    [[nodiscard]] std::size_t size() const noexcept { return living_.size(); }
    [[nodiscard]] std::size_t dead_count() const noexcept { return dead_.size(); }

    // ---- relations: see RelationGraph and EditResult for the rules ----
    // Check order: Invalid (id 0, a == b, unknown type or wrong kind), NotFound (an id never
    // created or forgotten), Invalid (a dead participant where not allowed), then the graph's
    // own results.

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
    // entries of lists above their limit (after extraversion dropped), then forgets, in id
    // order, dead characters whose last holder was evicted. Returns the number evicted.
    // Idempotent. May throw bad_alloc before any write (scratch space), changing nothing.
    [[nodiscard]] std::size_t maintain();

    // Bytes held by the registry and its vectors (capacity, not size), excluding
    // allocator headers.
    [[nodiscard]] std::size_t allocated_bytes() const noexcept;
    // Bytes held by relation storage only (RelationGraph::allocated_bytes()).
    [[nodiscard]] std::size_t relation_bytes() const noexcept { return relations_.allocated_bytes(); }
    // Bytes held by dead records and by the per-id arrays (slots and holders; capacity).
    [[nodiscard]] std::size_t dead_record_bytes() const noexcept { return dead_.capacity() * sizeof(DeadRecord); }
    [[nodiscard]] std::size_t slot_bytes() const noexcept {
        return (slots_.capacity() + holders_.capacity()) * sizeof(std::uint32_t);
    }

private:
    // Slot values: a living index, DEAD_FLAG | dead index, or FORGOTTEN. Dead indices stay
    // below 2^31 - 1, so FORGOTTEN never names a record.
    static constexpr std::uint32_t DEAD_FLAG = std::uint32_t{1} << 31;
    static constexpr std::uint32_t FORGOTTEN = 0xFFFF'FFFFu;

    [[nodiscard]] bool is_dead(CharacterId id) const noexcept {
        assert(exists(id));
        return (slots_[id.value - 1] & DEAD_FLAG) != 0;
    }
    // The three kinds of reference a living holder can have to a character.
    enum class ReferenceKind : std::uint8_t { LongEntry, Modifier, Edge };
    // Whether living `holder` references x through a kind other than `except`.
    [[nodiscard]] bool references_except(const Character& holder, CharacterId x, ReferenceKind except) const noexcept;
    // Called only when an edit flipped the existence of one kind of reference from living
    // `holder` to x (an edge created or erased, the first or last person modifier on x, a long
    // entry created, removed or evicted); `now_present` is that kind's new state. Looks up the
    // other kinds, adjusts holders if "holder references x" flipped, and returns true if x is
    // dead and its holders just reached 0 (the caller forgets it, in id order).
    [[nodiscard]] bool reference_flipped(const Character& holder, CharacterId x, ReferenceKind kind,
                                         bool now_present) noexcept;
    // Forgets two candidates (either may be invalid) in id order.
    void forget_in_order(CharacterId first, CharacterId second) noexcept;
    // Deletes a dead character with no holders. Never allocates.
    void forget(CharacterId id) noexcept;
    // Ensures scratch_ can hold `count` ids without reallocating.
    void reserve_scratch(std::size_t count);
    [[nodiscard]] EditResult check_relation_edit(CharacterId a, RelationType type, CharacterId b,
                                                 RelationKind kind) const noexcept;
    [[nodiscard]] EditResult add_person_modifier(CharacterId a, CharacterId b, ModifierId modifier,
                                                 int effect) noexcept;
    [[nodiscard]] EditResult add_target_modifier(CharacterId a, TargetId target, ModifierId modifier,
                                                 int effect) noexcept;
    [[nodiscard]] LongOpinionResult<CharacterId> add_person_long(CharacterId a, CharacterId b, int delta) noexcept;
    [[nodiscard]] LongOpinionResult<TargetId> add_target_long(CharacterId a, TargetId target, int delta) noexcept;

    std::vector<Character> living_;     // sorted by id
    std::vector<DeadRecord> dead_;      // unordered; forgetting swaps the last record in
    std::vector<std::uint32_t> slots_;  // index = id - 1: living index, DEAD_FLAG | dead index, or FORGOTTEN
    std::vector<std::uint32_t> holders_; // index = id - 1: distinct living holders; 0 once forgotten
    RelationGraph relations_;           // one node per id ever created, same indexing as slots_
    std::vector<CharacterId> scratch_;  // working space for kill and maintain; not state
};

} // namespace sim
