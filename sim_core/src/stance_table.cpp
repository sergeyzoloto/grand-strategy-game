#include "sim/stance_table.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <tuple>

#include "sim/vector_growth.hpp"

namespace sim {

namespace {

constexpr std::size_t MIN_STANCE_CAPACITY = 16;
constexpr std::size_t MIN_PARENT_CAPACITY = 16;

bool stance_key_less(const StanceEntry& e, CommunityId from, TargetId to) noexcept {
    return std::tuple(e.from.value, e.to.raw()) < std::tuple(from.value, to.raw());
}

} // namespace

EditResult StanceTable::set_stance_value(CommunityId from, TargetId to, std::int8_t value) {
    if (!from.valid() || !to.valid()) {
        return EditResult::Invalid;
    }
    auto it = std::lower_bound(stances_.begin(), stances_.end(), std::pair(from, to),
                               [](const StanceEntry& e, const auto& key) { return stance_key_less(e, key.first, key.second); });
    if (it != stances_.end() && it->from == from && it->to == to) {
        it->value = value;
        return EditResult::Ok;
    }
    const auto index = it - stances_.begin();
    detail::reserve_one_more(stances_, MIN_STANCE_CAPACITY);
    stances_.insert(stances_.begin() + index, StanceEntry{.from = from, .to = to, .value = value, .padding = {}});
    return EditResult::Ok;
}

EditResult StanceTable::clear_stance(CommunityId from, TargetId to) noexcept {
    if (!from.valid() || !to.valid()) {
        return EditResult::Invalid;
    }
    auto it = std::lower_bound(stances_.begin(), stances_.end(), std::pair(from, to),
                               [](const StanceEntry& e, const auto& key) { return stance_key_less(e, key.first, key.second); });
    if (it == stances_.end() || it->from != from || it->to != to) {
        return EditResult::NotFound;
    }
    stances_.erase(it);
    return EditResult::Ok;
}

std::optional<CommunityId> StanceTable::parent(CommunityId child) const noexcept {
    auto it = std::lower_bound(parents_.begin(), parents_.end(), child,
                               [](const ParentEntry& e, CommunityId id) { return e.child < id; });
    if (it == parents_.end() || it->child != child) {
        return std::nullopt;
    }
    return it->parent;
}

std::size_t StanceTable::subtree_height(CommunityId root) const noexcept {
    // Edit-time only: scans the parent entries for children, recursing at most
    // MAX_COMMUNITY_DEPTH levels (the hierarchy is acyclic and depth-limited).
    std::size_t below = 0;
    for (const ParentEntry& e : parents_) {
        if (e.parent == root) {
            below = std::max(below, subtree_height(e.child));
        }
    }
    return below + 1;
}

EditResult StanceTable::set_parent(CommunityId child, CommunityId new_parent) {
    if (!child.valid() || !new_parent.valid() || child == new_parent) {
        return EditResult::Invalid;
    }
    if (parent(child) == new_parent) {
        return EditResult::Ok;
    }
    const CommunityChain above = chain(new_parent);
    if (std::find(above.begin(), above.end(), child) != above.end()) {
        return EditResult::Conflict;
    }
    if (above.size() + subtree_height(child) > MAX_COMMUNITY_DEPTH) {
        return EditResult::Full;
    }

    auto it = std::lower_bound(parents_.begin(), parents_.end(), child,
                               [](const ParentEntry& e, CommunityId id) { return e.child < id; });
    if (it != parents_.end() && it->child == child) {
        it->parent = new_parent;
    } else {
        const auto index = it - parents_.begin();
        detail::reserve_one_more(parents_, MIN_PARENT_CAPACITY);
        parents_.insert(parents_.begin() + index, ParentEntry{.child = child, .parent = new_parent});
    }
    assert(chain(child).size() + subtree_height(child) - 1 <= MAX_COMMUNITY_DEPTH);
    return EditResult::Ok;
}

EditResult StanceTable::clear_parent(CommunityId child) noexcept {
    if (!child.valid()) {
        return EditResult::Invalid;
    }
    auto it = std::lower_bound(parents_.begin(), parents_.end(), child,
                               [](const ParentEntry& e, CommunityId id) { return e.child < id; });
    if (it == parents_.end() || it->child != child) {
        return EditResult::NotFound;
    }
    parents_.erase(it);
    return EditResult::Ok;
}

std::optional<int> StanceTable::explicit_stance(CommunityId from, TargetId to) const noexcept {
    if (!from.valid() || !to.valid()) {
        return std::nullopt;
    }
    auto it = std::lower_bound(stances_.begin(), stances_.end(), std::pair(from, to),
                               [](const StanceEntry& e, const auto& key) { return stance_key_less(e, key.first, key.second); });
    if (it == stances_.end() || it->from != from || it->to != to) {
        return std::nullopt;
    }
    return it->value;
}

CommunityChain StanceTable::chain(CommunityId community) const noexcept {
    CommunityChain result;
    for (std::optional<CommunityId> c = community.valid() ? std::optional(community) : std::nullopt; c;
         c = parent(*c)) {
        assert(!result.full()); // guaranteed by the depth limit
        if (result.full()) {
            break;
        }
        result.push_back(*c);
    }
    return result;
}

TargetChain StanceTable::target_chain(TargetId target) const noexcept {
    TargetChain result;
    if (!target.valid()) {
        return result;
    }
    if (const std::optional<CommunityId> community = target.as_community()) {
        for (const CommunityId c : chain(*community)) {
            if (const std::optional<TargetId> t = TargetId::from(c)) {
                result.push_back(*t);
            }
        }
    } else {
        result.push_back(target);
    }
    return result;
}

int StanceTable::stance(const CommunityChain& from, const TargetChain& to) const noexcept {
    for (const CommunityId s : from) {
        // All entries from s form one contiguous range.
        const auto lo = std::lower_bound(stances_.begin(), stances_.end(), s,
                                         [](const StanceEntry& e, CommunityId id) { return e.from < id; });
        if (lo == stances_.end() || lo->from != s) {
            continue;
        }
        const auto hi = std::upper_bound(lo, stances_.end(), s,
                                         [](CommunityId id, const StanceEntry& e) { return id < e.from; });
        for (const TargetId u : to) {
            const auto it = std::lower_bound(lo, hi, u,
                                             [](const StanceEntry& e, TargetId t) { return e.to.raw() < t.raw(); });
            if (it != hi && it->to == u) {
                return it->value;
            }
        }
    }
    return 0;
}

int StanceTable::stance(CommunityId from, TargetId to) const noexcept {
    return stance(chain(from), target_chain(to));
}

} // namespace sim
