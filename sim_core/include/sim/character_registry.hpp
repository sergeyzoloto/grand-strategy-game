#pragma once

#include <cstddef>
#include <span>
#include <vector>

#include "sim/bipolar.hpp"
#include "sim/character.hpp"
#include "sim/date.hpp"
#include "sim/edit_result.hpp"
#include "sim/fixed_vector.hpp"
#include "sim/ids.hpp"
#include "sim/integral.hpp"
#include "sim/opinion_records.hpp"
#include "sim/relation_graph.hpp"
#include "sim/relations.hpp"

namespace sim {

// The only creator of characters, and the owner of their relations.
//
// Ids start at 1, increase by one and are never reused; characters are never removed
// in this step. A character's own values (condition, traits, lists) are edited through
// find(); relations only through the registry.
//
// Lifetimes: any pointer, reference or span obtained from the registry or from a
// Character is valid only until the next registry mutation (create or any relation
// edit), because the registry may reallocate. Returned vectors are independent copies.
class CharacterRegistry {
public:
    // Creates a character and returns its id. Returns CharacterId{} only if ids are
    // exhausted. May throw bad_alloc, in which case nothing is created.
    [[nodiscard]] CharacterId create(NameId name, Gender gender, Date birth, const CharacterInit& init);

    // nullptr for id 0 or an unknown id.
    [[nodiscard]] Character* find(CharacterId id) noexcept;
    [[nodiscard]] const Character* find(CharacterId id) const noexcept;

    // All characters in creation order (index = id - 1).
    [[nodiscard]] std::span<Character> characters() noexcept { return characters_; }
    [[nodiscard]] std::span<const Character> characters() const noexcept { return characters_; }
    [[nodiscard]] std::size_t size() const noexcept { return characters_.size(); }

    // ---- relations: see RelationGraph and EditResult for the rules ----

    // One-way types only (Friend, Rival, Attraction); touch the edge a -> b only.
    [[nodiscard]] EditResult set_relation(CharacterId a, RelationType type, CharacterId b) {
        return relations_.set_relation(a, type, b);
    }
    [[nodiscard]] EditResult clear_relation(CharacterId a, RelationType type, CharacterId b) noexcept {
        return relations_.clear_relation(a, type, b);
    }
    // Paired types only; both sides at once. link(a, Child, b) reads "b is a's child".
    // Conflict if an edge would carry a type and its complement; Full if the child
    // would get more than MAX_PARENTS parents.
    [[nodiscard]] EditResult link(CharacterId a, RelationType type, CharacterId b) {
        return relations_.link(a, type, b);
    }
    [[nodiscard]] EditResult unlink(CharacterId a, RelationType type, CharacterId b) noexcept {
        return relations_.unlink(a, type, b);
    }

    [[nodiscard]] bool has_relation(CharacterId a, RelationType type, CharacterId b) const noexcept {
        return relations_.has_relation(a, type, b);
    }
    // Edges from a, sorted by other id; empty for an invalid or unknown id.
    [[nodiscard]] std::span<const RelationEdge> relations(CharacterId a) const noexcept {
        return relations_.relations(a);
    }
    // In id order.
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
    // (an unknown character), then the list's own results. Nothing depends on time.

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
    // One pass over all characters in id order: evicts the weakest long-term entries of
    // lists above their limit (after extraversion dropped). Returns the number evicted.
    // Idempotent.
    [[nodiscard]] std::size_t maintain() noexcept;

    // Bytes held by the registry and its vectors (capacity, not size), excluding
    // allocator headers.
    [[nodiscard]] std::size_t allocated_bytes() const noexcept;
    // Bytes held by relation storage only (RelationGraph::allocated_bytes()).
    [[nodiscard]] std::size_t relation_bytes() const noexcept { return relations_.allocated_bytes(); }

private:
    [[nodiscard]] EditResult add_person_modifier(CharacterId a, CharacterId b, ModifierId modifier,
                                                 int effect) noexcept;
    [[nodiscard]] EditResult add_target_modifier(CharacterId a, TargetId target, ModifierId modifier,
                                                 int effect) noexcept;
    [[nodiscard]] LongOpinionResult<CharacterId> add_person_long(CharacterId a, CharacterId b, int delta) noexcept;
    [[nodiscard]] LongOpinionResult<TargetId> add_target_long(CharacterId a, TargetId target, int delta) noexcept;

    std::vector<Character> characters_; // index = id - 1
    RelationGraph relations_;           // one node per character, same indexing
};

} // namespace sim
