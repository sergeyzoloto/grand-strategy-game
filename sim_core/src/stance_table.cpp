#include "sim/stance_table.hpp"

#include <algorithm>
#include <array>
#include <cstdint>
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

void StanceTable::stances(std::span<const CommunityChain> sources, std::span<const TargetChain> targets,
                          std::span<int> out) const noexcept {
    assert(sources.size() <= STANCE_BATCH_MAX_SOURCES);
    assert(out.size() >= sources.size() * targets.size());
    if (sources.size() > STANCE_BATCH_MAX_SOURCES || out.size() < sources.size() * targets.size()) {
        std::fill(out.begin(), out.end(), 0); // release: a defined result (no stances) instead of stale values
        return;
    }

    // Blocks of entries for the distinct source communities, found once in this call.
    struct Block {
        CommunityId community;
        const StanceEntry* begin;
        const StanceEntry* end; // equal to begin for a community without entries
    };
    std::array<Block, STANCE_BATCH_MAX_SOURCES * MAX_COMMUNITY_DEPTH> blocks{};
    std::size_t block_count = 0;
    // Per source chain: indices of its non-empty blocks, nearest community first.
    std::array<std::array<std::uint8_t, MAX_COMMUNITY_DEPTH>, STANCE_BATCH_MAX_SOURCES> chain_blocks{};
    std::array<std::size_t, STANCE_BATCH_MAX_SOURCES> chain_block_count{};

    const StanceEntry* const first = stances_.data();
    const StanceEntry* const last = first + stances_.size();
    for (std::size_t i = 0; i < sources.size(); ++i) {
        for (const CommunityId s : sources[i]) {
            std::size_t index = 0;
            while (index < block_count && blocks[index].community != s) {
                ++index;
            }
            if (index == block_count) {
                const StanceEntry* lo = std::lower_bound(first, last, s,
                                                         [](const StanceEntry& e, CommunityId id) { return e.from < id; });
                const StanceEntry* hi = std::upper_bound(lo, last, s,
                                                         [](CommunityId id, const StanceEntry& e) { return id < e.from; });
                blocks[block_count++] = Block{s, lo, hi};
            }
            if (blocks[index].begin != blocks[index].end) { // skip empty blocks
                chain_blocks[i][chain_block_count[i]++] = static_cast<std::uint8_t>(index);
            }
        }
    }

    for (std::size_t i = 0; i < sources.size(); ++i) {
        for (std::size_t j = 0; j < targets.size(); ++j) {
            int value = 0;
            bool found = false;
            for (std::size_t b = 0; b < chain_block_count[i] && !found; ++b) {
                const Block& block = blocks[chain_blocks[i][b]];
                for (const TargetId u : targets[j]) {
                    const StanceEntry* it = std::lower_bound(block.begin, block.end, u,
                                                             [](const StanceEntry& e, TargetId t) { return e.to.raw() < t.raw(); });
                    if (it != block.end && it->to == u) {
                        value = it->value;
                        found = true;
                        break;
                    }
                }
            }
            out[i * targets.size() + j] = value;
        }
    }
}

int StanceTable::stance(const CommunityChain& from, const TargetChain& to) const noexcept {
    int value = 0;
    stances(std::span(&from, 1), std::span(&to, 1), std::span(&value, 1));
    return value;
}

int StanceTable::stance(CommunityId from, TargetId to) const noexcept {
    return stance(chain(from), target_chain(to));
}

} // namespace sim
