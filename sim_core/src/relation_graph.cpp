#include "sim/relation_graph.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>

#include "sim/vector_growth.hpp"

namespace sim {

namespace {

const RelationEdge* find_edge(std::span<const RelationEdge> edges, CharacterId other) noexcept {
    const auto* it = std::lower_bound(edges.data(), edges.data() + edges.size(), other,
                                      [](const RelationEdge& e, CharacterId id) { return e.other < id; });
    return (it != edges.data() + edges.size() && it->other == other) ? it : nullptr;
}

// Relation info if `type` is a known type of the expected kind; empty otherwise (Invalid).
std::optional<RelationInfo> kind_checked(RelationType type, RelationKind expected) noexcept {
    const std::optional<RelationInfo> info = relation_info(type);
    if (!info || info->kind != expected) {
        return std::nullopt;
    }
    return info;
}

// Bits of all one-way types, and of all types that end at death.
constexpr std::uint32_t mask_where(bool (*predicate)(RelationType)) noexcept {
    std::uint32_t mask = 0;
    for (unsigned t = 0; t < 32; ++t) {
        const auto type = static_cast<RelationType>(t);
        if (relation_info(type) && predicate(type)) {
            mask |= relation_bit(type);
        }
    }
    return mask;
}

constexpr std::uint32_t ONE_WAY_MASK =
    mask_where([](RelationType t) { return relation_info(t)->kind == RelationKind::OneWay; });
constexpr std::uint32_t NON_SURVIVING_MASK = mask_where([](RelationType t) { return !survives_death(t); });

} // namespace

void RelationGraph::reserve_node() {
    detail::reserve_one_more(edges_, MIN_NODE_CAPACITY);
}

void RelationGraph::add_node() noexcept {
    assert(edges_.size() < edges_.capacity());
    edges_.emplace_back(); // an empty vector does not allocate
}

std::uint32_t RelationGraph::mask(CharacterId a, CharacterId b) const noexcept {
    const RelationEdge* e = find_edge(list(a), b);
    return e != nullptr ? e->mask : 0;
}

std::size_t RelationGraph::parent_count(CharacterId child) const noexcept {
    return static_cast<std::size_t>(std::count_if(list(child).begin(), list(child).end(), [](const RelationEdge& e) {
        return (e.mask & relation_bit(RelationType::Parent)) != 0;
    }));
}

void RelationGraph::add_bits(CharacterId a, CharacterId b, std::uint32_t bits) noexcept {
    EdgeList& edges = list(a);
    auto it = std::lower_bound(edges.begin(), edges.end(), b,
                               [](const RelationEdge& e, CharacterId id) { return e.other < id; });
    if (it != edges.end() && it->other == b) {
        it->mask |= bits;
        return;
    }
    assert(edges.size() < edges.capacity()); // room was reserved: insert cannot allocate
    edges.insert(it, RelationEdge{.other = b, .mask = bits});
}

void RelationGraph::remove_bits(CharacterId a, CharacterId b, std::uint32_t bits) noexcept {
    EdgeList& edges = list(a);
    auto it = std::lower_bound(edges.begin(), edges.end(), b,
                               [](const RelationEdge& e, CharacterId id) { return e.other < id; });
    assert(it != edges.end() && it->other == b);
    it->mask &= ~bits;
    if (it->mask == 0) {
        edges.erase(it); // erase does not allocate
    }
}

EditResult RelationGraph::set_relation(CharacterId a, RelationType type, CharacterId b) {
    if (!a.valid() || !b.valid() || a == b || !kind_checked(type, RelationKind::OneWay)) {
        return EditResult::Invalid;
    }
    if (!exists(a) || !exists(b)) {
        return EditResult::NotFound;
    }
    const std::uint32_t bit = relation_bit(type);
    const std::uint32_t current = mask(a, b);
    if ((current & bit) != 0) {
        return EditResult::Duplicate;
    }
    if (current == 0) {
        detail::reserve_one_more(list(a), MIN_EDGE_CAPACITY);
    }
    add_bits(a, b, bit);
    assert(pair_valid(a, b));
    return EditResult::Ok;
}

EditResult RelationGraph::clear_relation(CharacterId a, RelationType type, CharacterId b) noexcept {
    if (!a.valid() || !b.valid() || a == b || !kind_checked(type, RelationKind::OneWay)) {
        return EditResult::Invalid;
    }
    if (!exists(a) || !exists(b)) {
        return EditResult::NotFound;
    }
    const std::uint32_t bit = relation_bit(type);
    if ((mask(a, b) & bit) == 0) {
        return EditResult::NotFound;
    }
    remove_bits(a, b, bit);
    assert(pair_valid(a, b));
    return EditResult::Ok;
}

EditResult RelationGraph::link(CharacterId a, RelationType type, CharacterId b) {
    const std::optional<RelationInfo> info = kind_checked(type, RelationKind::Paired);
    if (!a.valid() || !b.valid() || a == b || !info) {
        return EditResult::Invalid;
    }
    if (!exists(a) || !exists(b)) {
        return EditResult::NotFound;
    }
    assert(info->complement.has_value());
    const RelationType complement = *info->complement;
    const std::uint32_t bit = relation_bit(type);
    const std::uint32_t complement_bit = relation_bit(complement);
    const std::uint32_t forward = mask(a, b);
    const std::uint32_t backward = mask(b, a);

    if ((forward & bit) != 0) {
        return EditResult::Duplicate;
    }
    // b would be both `type` and its complement for a (e.g. both parent and child).
    if (complement != type && ((forward & complement_bit) != 0 || (backward & bit) != 0)) {
        return EditResult::Conflict;
    }
    if ((type == RelationType::Child && parent_count(b) >= MAX_PARENTS)
        || (type == RelationType::Parent && parent_count(a) >= MAX_PARENTS)) {
        return EditResult::Full;
    }

    // Reserve in both lists before the first write, so bad_alloc cannot leave half a link.
    if (forward == 0) {
        detail::reserve_one_more(list(a), MIN_EDGE_CAPACITY);
    }
    if (backward == 0) {
        detail::reserve_one_more(list(b), MIN_EDGE_CAPACITY);
    }
    add_bits(a, b, bit);
    add_bits(b, a, complement_bit);
    assert(pair_valid(a, b));
    return EditResult::Ok;
}

EditResult RelationGraph::unlink(CharacterId a, RelationType type, CharacterId b) noexcept {
    const std::optional<RelationInfo> info = kind_checked(type, RelationKind::Paired);
    if (!a.valid() || !b.valid() || a == b || !info) {
        return EditResult::Invalid;
    }
    if (!exists(a) || !exists(b)) {
        return EditResult::NotFound;
    }
    const std::uint32_t bit = relation_bit(type);
    if ((mask(a, b) & bit) == 0) {
        return EditResult::NotFound;
    }
    const std::uint32_t complement_bit = relation_bit(*info->complement);
    assert((mask(b, a) & complement_bit) != 0);
    remove_bits(a, b, bit);
    remove_bits(b, a, complement_bit);
    assert(pair_valid(a, b));
    return EditResult::Ok;
}

bool RelationGraph::has_relation(CharacterId a, RelationType type, CharacterId b) const noexcept {
    if (!exists(a) || !exists(b) || a == b || !relation_info(type)) {
        return false;
    }
    return (mask(a, b) & relation_bit(type)) != 0;
}

std::span<const RelationEdge> RelationGraph::relations(CharacterId a) const noexcept {
    if (!exists(a)) {
        return {};
    }
    return list(a);
}

FixedVector<CharacterId, MAX_PARENTS> RelationGraph::parents(CharacterId a) const noexcept {
    FixedVector<CharacterId, MAX_PARENTS> result;
    if (!exists(a)) {
        return result;
    }
    for (const RelationEdge& e : list(a)) { // sorted by id
        if ((e.mask & relation_bit(RelationType::Parent)) != 0) {
            assert(!result.full());
            result.push_back(e.other);
        }
    }
    return result;
}

std::vector<CharacterId> RelationGraph::children(CharacterId a) const {
    std::vector<CharacterId> result;
    if (!exists(a)) {
        return result;
    }
    for (const RelationEdge& e : list(a)) { // sorted by id
        if ((e.mask & relation_bit(RelationType::Child)) != 0) {
            result.push_back(e.other);
        }
    }
    return result;
}

std::vector<CharacterId> RelationGraph::siblings(CharacterId a) const {
    std::vector<CharacterId> result;
    for (const CharacterId parent : parents(a)) {
        for (const RelationEdge& e : list(parent)) {
            if ((e.mask & relation_bit(RelationType::Child)) != 0 && e.other != a) {
                result.push_back(e.other);
            }
        }
    }
    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());
    return result;
}

int RelationGraph::shared_parents(CharacterId a, CharacterId b) const noexcept {
    if (!exists(a) || !exists(b) || a == b) {
        return 0;
    }
    int shared = 0;
    for (const CharacterId parent : parents(a)) {
        if ((mask(b, parent) & relation_bit(RelationType::Parent)) != 0) {
            ++shared;
        }
    }
    return shared;
}

void RelationGraph::remove_non_surviving(CharacterId dead) noexcept {
    assert(exists(dead));
    EdgeList& edges = list(dead);
    for (std::size_t i = edges.size(); i-- > 0;) { // backwards: remove_bits may erase edge i
        const CharacterId other = edges[i].other;
        const std::uint32_t ending = edges[i].mask & NON_SURVIVING_MASK;
        if (ending == 0) {
            continue;
        }
        std::uint32_t complements = 0;
        for (unsigned t = 0; t < 32; ++t) {
            const std::uint32_t bit = std::uint32_t{1} << t;
            if ((ending & bit) == 0) {
                continue;
            }
            const std::optional<RelationInfo> info = relation_info(static_cast<RelationType>(t));
            assert(info.has_value());
            if (info->kind == RelationKind::Paired) {
                complements |= relation_bit(*info->complement);
            }
        }
        if (complements != 0) {
            remove_bits(other, dead, complements);
        }
        remove_bits(dead, other, ending);
        assert(pair_valid(dead, other));
    }
}

void RelationGraph::clear_one_way(CharacterId from, CharacterId to) noexcept {
    const std::uint32_t bits = mask(from, to) & ONE_WAY_MASK;
    if (bits != 0) {
        remove_bits(from, to, bits);
    }
}

bool RelationGraph::has_one_way_edges(CharacterId a) const noexcept {
    return exists(a) && std::any_of(list(a).begin(), list(a).end(),
                                    [](const RelationEdge& e) { return (e.mask & ONE_WAY_MASK) != 0; });
}

std::size_t RelationGraph::allocated_bytes() const noexcept {
    std::size_t bytes = edges_.capacity() * sizeof(EdgeList);
    for (const EdgeList& edges : edges_) {
        bytes += edges.capacity() * sizeof(RelationEdge);
    }
    return bytes;
}

bool RelationGraph::pair_valid(CharacterId a, CharacterId b) const noexcept {
    for (const CharacterId id : {a, b}) {
        const EdgeList& edges = list(id);
        for (std::size_t i = 0; i < edges.size(); ++i) {
            if (edges[i].mask == 0 || !exists(edges[i].other) || edges[i].other == id
                || (i > 0 && !(edges[i - 1].other < edges[i].other))) {
                return false;
            }
        }
        if (parent_count(id) > MAX_PARENTS) {
            return false;
        }
    }
    // Paired bits are symmetric with their complement; one-way bits are unconstrained.
    const std::uint32_t forward = mask(a, b);
    const std::uint32_t backward = mask(b, a);
    for (unsigned t = 0; t < 32; ++t) {
        const auto type = static_cast<RelationType>(t);
        const std::optional<RelationInfo> info = relation_info(type);
        if (!info) {
            if (((forward | backward) & (std::uint32_t{1} << t)) != 0) {
                return false; // unknown bit
            }
            continue;
        }
        if (info->kind != RelationKind::Paired) {
            continue;
        }
        const bool has_forward = (forward & relation_bit(type)) != 0;
        const bool has_backward_complement = (backward & relation_bit(*info->complement)) != 0;
        if (has_forward != has_backward_complement) {
            return false;
        }
        if (*info->complement != type && has_forward && (forward & relation_bit(*info->complement)) != 0) {
            return false; // type and complement on the same edge
        }
    }
    return true;
}

} // namespace sim
