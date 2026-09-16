#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

#include "sim/edit_result.hpp"
#include "sim/fixed_vector.hpp"
#include "sim/ids.hpp"
#include "sim/relations.hpp"

namespace sim {

// Relations between characters, one sorted edge list per character. Owned by
// CharacterRegistry, which adds a node for every character it creates; all other
// access goes through the registry. Node ids are 1..node_count().
//
// Edits check everything before the first write and reserve room in every list they
// grow, so a failed call (including bad_alloc) leaves both sides unchanged.
// Spans and vectors obtained from the graph are invalidated by any edit or new node.
class RelationGraph {
public:
    // Minimum capacities passed to detail::reserve_one_more: the node vector and each edge
    // list grow by doubling from these.
    static constexpr std::size_t MIN_NODE_CAPACITY = 16;
    static constexpr std::size_t MIN_EDGE_CAPACITY = 4;

    // Prepares room for one more node; may throw bad_alloc. add_node() then cannot fail.
    void reserve_node();
    // Adds node node_count() + 1. Requires a preceding reserve_node().
    void add_node() noexcept;
    [[nodiscard]] std::size_t node_count() const noexcept { return edges_.size(); }

    // One-way types only; touch the edge a -> b.
    [[nodiscard]] EditResult set_relation(CharacterId a, RelationType type, CharacterId b);
    [[nodiscard]] EditResult clear_relation(CharacterId a, RelationType type, CharacterId b) noexcept;
    // Paired types only; set or clear `type` on a -> b and its complement on b -> a.
    [[nodiscard]] EditResult link(CharacterId a, RelationType type, CharacterId b);
    [[nodiscard]] EditResult unlink(CharacterId a, RelationType type, CharacterId b) noexcept;

    [[nodiscard]] bool has_relation(CharacterId a, RelationType type, CharacterId b) const noexcept;
    [[nodiscard]] std::span<const RelationEdge> relations(CharacterId a) const noexcept;
    [[nodiscard]] FixedVector<CharacterId, MAX_PARENTS> parents(CharacterId a) const noexcept;
    [[nodiscard]] std::vector<CharacterId> children(CharacterId a) const;
    [[nodiscard]] std::vector<CharacterId> siblings(CharacterId a) const;
    [[nodiscard]] int shared_parents(CharacterId a, CharacterId b) const noexcept;

    // Bytes held by the graph's vectors (capacity, not size), excluding allocator headers.
    [[nodiscard]] std::size_t allocated_bytes() const noexcept;

private:
    using EdgeList = std::vector<RelationEdge>;

    [[nodiscard]] bool exists(CharacterId id) const noexcept { return id.valid() && id.value <= edges_.size(); }
    [[nodiscard]] EdgeList& list(CharacterId id) noexcept { return edges_[id.value - 1]; }
    [[nodiscard]] const EdgeList& list(CharacterId id) const noexcept { return edges_[id.value - 1]; }
    [[nodiscard]] std::uint32_t mask(CharacterId a, CharacterId b) const noexcept;
    [[nodiscard]] std::size_t parent_count(CharacterId child) const noexcept;
    // Sets mask bits on a -> b, inserting the edge if absent. Requires room reserved.
    void add_bits(CharacterId a, CharacterId b, std::uint32_t bits) noexcept;
    // Clears mask bits on a -> b, removing the edge if its mask becomes 0.
    void remove_bits(CharacterId a, CharacterId b, std::uint32_t bits) noexcept;
    [[nodiscard]] bool pair_valid(CharacterId a, CharacterId b) const noexcept; // debug invariant check

    std::vector<EdgeList> edges_; // index = id - 1; each list sorted by other, masks never 0
};

} // namespace sim
