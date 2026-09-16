#pragma once

#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

#include "sim/ids.hpp"

namespace sim {

// Relation between two characters. Append-only and explicitly numbered (persisted);
// the value is also the bit index in RelationEdge::mask, so at most 32 types.
enum class RelationType : std::uint8_t {
    // one-way
    Friend = 0,
    Rival = 1,
    Attraction = 2,
    // paired
    Parent = 3,
    Child = 4,
    Liege = 5,
    Vassal = 6,
    Employer = 7,
    Employee = 8,
    Spouse = 9,
};

enum class RelationKind : std::uint8_t {
    OneWay = 0, // set on the source edge only
    Paired = 1, // always set on both edges, with the complement on the reverse edge
};

struct RelationInfo {
    RelationKind kind;
    std::optional<RelationType> complement; // empty for one-way types; Spouse is its own complement
};

// Single source of kind and complement. Empty for values outside the enum.
// No default: appending a RelationType triggers -Wswitch until it is handled here.
[[nodiscard]] constexpr std::optional<RelationInfo> relation_info(RelationType type) noexcept {
    switch (type) {
    case RelationType::Friend:
    case RelationType::Rival:
    case RelationType::Attraction:
        return RelationInfo{RelationKind::OneWay, std::nullopt};
    case RelationType::Parent:
        return RelationInfo{RelationKind::Paired, RelationType::Child};
    case RelationType::Child:
        return RelationInfo{RelationKind::Paired, RelationType::Parent};
    case RelationType::Liege:
        return RelationInfo{RelationKind::Paired, RelationType::Vassal};
    case RelationType::Vassal:
        return RelationInfo{RelationKind::Paired, RelationType::Liege};
    case RelationType::Employer:
        return RelationInfo{RelationKind::Paired, RelationType::Employee};
    case RelationType::Employee:
        return RelationInfo{RelationKind::Paired, RelationType::Employer};
    case RelationType::Spouse:
        return RelationInfo{RelationKind::Paired, RelationType::Spouse};
    }
    return std::nullopt;
}

// Whether a relation stays when one of its participants dies. Family ties survive
// (parents(), children() and siblings() keep working through the dead); service and
// personal ties end. Values outside the enum: false.
// No default: appending a RelationType triggers -Wswitch until it is handled here.
[[nodiscard]] constexpr bool survives_death(RelationType type) noexcept {
    switch (type) {
    case RelationType::Parent:
    case RelationType::Child:
    case RelationType::Spouse:
        return true;
    case RelationType::Friend:
    case RelationType::Rival:
    case RelationType::Attraction:
    case RelationType::Liege:
    case RelationType::Vassal:
    case RelationType::Employer:
    case RelationType::Employee:
        return false;
    }
    return false;
}

// Mask bit of a relation type. Requires a valid type (see relation_info).
[[nodiscard]] constexpr std::uint32_t relation_bit(RelationType type) noexcept {
    return std::uint32_t{1} << static_cast<unsigned>(type);
}

// A character has at most this many parents. Siblings are derived from shared
// parents and never stored.
inline constexpr std::size_t MAX_PARENTS = 2;

// Relations from one character (the source) to `other`. One edge per ordered pair;
// a set bit on edge a -> b names b's role for a (Child: b is a's child).
struct RelationEdge {
    CharacterId other;        // u32 handle of the target character, always valid
    std::uint32_t mask = 0;   // bit relation_bit(t) set when b has role t for a; never 0 when stored
};

static_assert(sizeof(RelationEdge) == 8 && alignof(RelationEdge) == 4);
static_assert(std::has_unique_object_representations_v<RelationEdge>);

} // namespace sim
