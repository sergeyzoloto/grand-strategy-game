#pragma once

#include <cstdint>
#include <type_traits>

#include "sim/ids.hpp"

namespace sim {

// Storage bounds of the per-character lists.
inline constexpr std::size_t NICKNAME_CAP = 4;
inline constexpr std::size_t PRACTISE_CAP = 64; // storage bound only, not a gameplay limit; never evict
inline constexpr std::size_t INVOLVEMENT_CAP = 8;
inline constexpr std::size_t SACRED_CAP = 8;

inline constexpr int INVOLVEMENT_WEIGHT_MAX = 255;

// Append-only; never renumber (persisted).
enum class SkillKind : std::uint8_t {
    Ability = 0,
    Education = 1,
    Language = 2,
};

// Append-only; never renumber (persisted).
enum class SacredSign : std::uint8_t {
    Plus = 0,
    Minus = 1,
};

// Result of every list mutation. Check order: Invalid (invalid id or enum value),
// then Duplicate / Conflict / NotFound against existing entries, then Full.
enum class ListResult : std::uint8_t {
    Ok = 0,
    Full = 1,      // a new entry was needed but the list is at capacity; state unchanged
    Duplicate = 2, // the entry already exists; state unchanged
    Conflict = 3,  // the entry exists with a contradicting value; state unchanged
    NotFound = 4,  // removal of an absent entry; state unchanged
    Invalid = 5,   // invalid id or enum value; state unchanged
};

// A skill the character has. Skills are capabilities: present or absent, no value.
// Sorted by (kind, skill) in Character.
struct PractiseEntry {
    SkillId skill;                   // u16 handle, always valid in a stored entry
    SkillKind kind = SkillKind::Ability; // u8 enum
    std::uint8_t reserved = 0;       // always 0; a mastery level may go here later without changing sizeof
};

// Weighted membership in a community. Sorted by community id in Character.
struct InvolvementEntry {
    CommunityId community;           // u32 handle, always valid in a stored entry
    std::uint8_t weight = 0;         // relative weight 1..255 (0 is never stored); share = weight / total
    std::uint8_t padding[3]{};       // explicit padding, always 0
};

// A sacred target and its sign. Sorted by target (raw order) in Character.
struct SacredEntry {
    TargetId target;                 // u32 kind+index, always valid in a stored entry
    SacredSign sign = SacredSign::Plus; // u8 enum
    std::uint8_t padding[3]{};       // explicit padding, always 0
};

static_assert(sizeof(PractiseEntry) == 4 && alignof(PractiseEntry) == 2);
static_assert(sizeof(InvolvementEntry) == 8 && alignof(InvolvementEntry) == 4);
static_assert(sizeof(SacredEntry) == 8 && alignof(SacredEntry) == 4);
// No implicit padding: value-initialized entries leave no indeterminate bytes.
static_assert(std::has_unique_object_representations_v<PractiseEntry>);
static_assert(std::has_unique_object_representations_v<InvolvementEntry>);
static_assert(std::has_unique_object_representations_v<SacredEntry>);

} // namespace sim
