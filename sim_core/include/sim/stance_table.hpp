#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>
#include <vector>

#include "sim/bipolar.hpp"
#include "sim/edit_result.hpp"
#include "sim/fixed_vector.hpp"
#include "sim/ids.hpp"
#include "sim/integral.hpp"

namespace sim {

// A community plus at most 5 ancestors.
inline constexpr std::size_t MAX_COMMUNITY_DEPTH = 6;

// At most this many source chains per batched stance query (a character's involvement).
inline constexpr std::size_t STANCE_BATCH_MAX_SOURCES = 8;

// A community followed by its ancestors, nearest first.
using CommunityChain = FixedVector<CommunityId, MAX_COMMUNITY_DEPTH>;
// Targets to try for a stance, nearest first: a topic alone, or a community and its
// ancestors as TargetIds.
using TargetChain = FixedVector<TargetId, MAX_COMMUNITY_DEPTH>;

// An explicit stance of a community towards a target. Sorted by (from, to.raw()).
struct StanceEntry {
    CommunityId from;          // u32 handle, always valid
    TargetId to;               // u32 kind+index, always valid; may equal `from` as a community (in-group cohesion)
    std::int8_t value = 0;     // -100..+100 units, stored directly; an explicit 0 is a real value
    std::uint8_t padding[3]{}; // explicit padding, always 0
};

// Parent of a community in the hierarchy. Sorted by child.
struct ParentEntry {
    CommunityId child;  // u32 handle, always valid
    CommunityId parent; // u32 handle, always valid, never equal to child
};

static_assert(sizeof(StanceEntry) == 12 && std::has_unique_object_representations_v<StanceEntry>);
static_assert(sizeof(ParentEntry) == 8 && std::has_unique_object_representations_v<ParentEntry>);

// Explicit stances between communities and targets, plus the community hierarchy used
// for inheritance. Storage: two sorted vectors, binary-searched; lookups never allocate.
// Edits check first, reserve room geometrically, then write (a failure changes nothing).
//
// stance(from, to) returns the first explicit entry in this order, or 0:
//   for s in chain(from), nearest first:
//     for u in chain(to), nearest first (a topic: only the topic itself):
//       if an explicit entry (s, u) exists: return it
// The source side wins: a family's own feud overrides its tribe's attitude, and a
// tribe's stance towards another tribe reaches that tribe's families.
class StanceTable {
public:
    // Stores an explicit value clamped to -100..+100, overwriting any existing one.
    // Ok or Invalid (invalid from or to).
    [[nodiscard]] EditResult set_stance(CommunityId from, TargetId to, StrictIntegral auto value) {
        return set_stance_value(from, to, detail::clamp_bipolar(value));
    }
    // Ok, Invalid or NotFound. Removing an entry restores inheritance.
    [[nodiscard]] EditResult clear_stance(CommunityId from, TargetId to) noexcept;

    // Ok (also when parent is already child's parent), Invalid (id 0 or child == parent),
    // Conflict (would create a cycle) or Full (a chain in child's subtree would exceed
    // MAX_COMMUNITY_DEPTH).
    [[nodiscard]] EditResult set_parent(CommunityId child, CommunityId parent);
    // Ok, Invalid or NotFound.
    [[nodiscard]] EditResult clear_parent(CommunityId child) noexcept;

    [[nodiscard]] std::optional<CommunityId> parent(CommunityId child) const noexcept;
    [[nodiscard]] std::optional<int> explicit_stance(CommunityId from, TargetId to) const noexcept;

    // Inherited stance, 0 when no entry applies or an id is invalid.
    [[nodiscard]] int stance(CommunityId from, TargetId to) const noexcept;
    // Same resolution over prebuilt chains, so a caller can build each chain once.
    [[nodiscard]] int stance(const CommunityChain& from, const TargetChain& to) const noexcept;
    // Resolves every (source, target) pair in one call: out[i * targets.size() + j] is
    // stance(sources[i], targets[j]). Each distinct source community's block of entries
    // is found once inside the call; positions are never stored or returned.
    // Requires sources.size() <= STANCE_BATCH_MAX_SOURCES and a large enough `out`.
    void stances(std::span<const CommunityChain> sources, std::span<const TargetChain> targets,
                 std::span<int> out) const noexcept;

    // Empty for an invalid id.
    [[nodiscard]] CommunityChain chain(CommunityId community) const noexcept;
    // Empty for an invalid target. Ancestors with ids >= 2^30 cannot be targets and are skipped.
    [[nodiscard]] TargetChain target_chain(TargetId target) const noexcept;

    [[nodiscard]] std::size_t stance_count() const noexcept { return stances_.size(); }
    [[nodiscard]] std::size_t parent_count() const noexcept { return parents_.size(); }

private:
    [[nodiscard]] EditResult set_stance_value(CommunityId from, TargetId to, std::int8_t value);
    [[nodiscard]] std::size_t subtree_height(CommunityId root) const noexcept;

    std::vector<StanceEntry> stances_; // sorted by (from, to.raw()), unique keys
    std::vector<ParentEntry> parents_; // sorted by child, unique children, acyclic, depth <= 6
};

} // namespace sim
