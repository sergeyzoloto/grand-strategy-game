#pragma once

#include <compare>
#include <concepts>
#include <cstdint>
#include <functional>
#include <optional>

#include "sim/integral.hpp"

namespace sim {

// Strong, tag-typed identifier stored as an unsigned integer Rep (u32 by default).
// Value 0 is the invalid id. No implicit conversions to or from integers or between
// different id types. Rep excludes bool and character types.
template<class Tag, class Rep = std::uint32_t>
    requires StrictIntegral<Rep> && std::unsigned_integral<Rep>
struct Id {
    Rep value = 0; // opaque handle; 0 = invalid

    constexpr Id() noexcept = default;
    constexpr explicit Id(Rep v) noexcept : value(v) {}

    [[nodiscard]] constexpr bool valid() const noexcept { return value != 0; }

    friend constexpr bool operator==(Id, Id) noexcept = default;
    friend constexpr auto operator<=>(Id, Id) noexcept = default;
};

// Adding a new id type takes one line here.
using CharacterId = Id<struct CharacterIdTag>;
using NameId = Id<struct NameIdTag>;
using CommunityId = Id<struct CommunityIdTag>;
using TopicId = Id<struct TopicIdTag>;
using SkillId = Id<struct SkillIdTag, std::uint16_t>;
using CauseId = Id<struct CauseIdTag, std::uint16_t>; // cause of an opinion event; 0 = unspecified

// Kind of entity a TargetId refers to. Stored in 2 bits: append-only, at most 4 kinds.
// Value 2 is intended for persons (a later step); 3 is free.
enum class TargetKind : std::uint8_t {
    Community = 0,
    Topic = 1,
};

// Reference to an entity of one of several kinds, packed into u32:
// 2 high bits of kind, 30 low bits of index (the source id's value).
// Valid iff index != 0; the default TargetId{} (raw 0) is invalid.
// Built only through from(), which rejects invalid ids and indices >= 2^30.
class TargetId {
public:
    static constexpr int INDEX_BITS = 30;
    static constexpr std::uint32_t INDEX_LIMIT = std::uint32_t{1} << INDEX_BITS; // indices are < 2^30

    constexpr TargetId() noexcept = default;

    [[nodiscard]] static constexpr std::optional<TargetId> from(CommunityId id) noexcept {
        return make(TargetKind::Community, id.value);
    }
    [[nodiscard]] static constexpr std::optional<TargetId> from(TopicId id) noexcept {
        return make(TargetKind::Topic, id.value);
    }

    [[nodiscard]] constexpr TargetKind kind() const noexcept {
        return static_cast<TargetKind>(value_ >> INDEX_BITS);
    }
    [[nodiscard]] constexpr std::uint32_t index() const noexcept { return value_ & (INDEX_LIMIT - 1); }
    [[nodiscard]] constexpr bool valid() const noexcept { return index() != 0; }
    [[nodiscard]] constexpr std::uint32_t raw() const noexcept { return value_; }

    [[nodiscard]] constexpr std::optional<CommunityId> as_community() const noexcept {
        if (!valid() || kind() != TargetKind::Community) {
            return std::nullopt;
        }
        return CommunityId{index()};
    }
    [[nodiscard]] constexpr std::optional<TopicId> as_topic() const noexcept {
        if (!valid() || kind() != TargetKind::Topic) {
            return std::nullopt;
        }
        return TopicId{index()};
    }

    // Raw order: grouped by kind, then by index.
    friend constexpr bool operator==(TargetId, TargetId) noexcept = default;
    friend constexpr auto operator<=>(TargetId, TargetId) noexcept = default;

private:
    [[nodiscard]] static constexpr std::optional<TargetId> make(TargetKind kind, std::uint32_t index) noexcept {
        if (index == 0 || index >= INDEX_LIMIT) {
            return std::nullopt;
        }
        TargetId id;
        id.value_ = (static_cast<std::uint32_t>(kind) << INDEX_BITS) | index;
        return id;
    }

    std::uint32_t value_ = 0; // 2 bits kind << 30 | 30 bits index; 0 = invalid
};

} // namespace sim

template<class Tag, class Rep>
    requires sim::StrictIntegral<Rep> && std::unsigned_integral<Rep>
struct std::hash<sim::Id<Tag, Rep>> {
    [[nodiscard]] std::size_t operator()(sim::Id<Tag, Rep> id) const noexcept {
        return std::hash<Rep>{}(id.value);
    }
};

template<>
struct std::hash<sim::TargetId> {
    [[nodiscard]] std::size_t operator()(sim::TargetId id) const noexcept {
        return std::hash<std::uint32_t>{}(id.raw());
    }
};
