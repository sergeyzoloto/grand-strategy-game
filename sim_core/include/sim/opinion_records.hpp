#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "sim/bipolar.hpp"
#include "sim/ids.hpp"

namespace sim {

// Long-term opinion list bounds. A character's people limit depends on extraversion
// (see person_limit); the storage capacity is the maximum. Only long-term entries have
// limits and eviction.
inline constexpr std::size_t PERSON_LIMIT_MIN = 8;
inline constexpr std::size_t PERSON_LIMIT_MAX = 40;
inline constexpr std::size_t TARGET_LIMIT = 16;

// Modifier storage bound, not a gameplay rule: a full list returns Full, never evicts.
inline constexpr std::size_t MODIFIER_CAP = 32;
inline constexpr int MODIFIER_EFFECT_MAX = BIPOLAR_MAX;     // effects are -100..+100
inline constexpr int LONG_VALUE_MAX = 200;                  // long values are -200..+200
inline constexpr int LONG_DELTA_MAX = 2 * LONG_VALUE_MAX;   // one delta can cross the whole range

// A stored long-term personal opinion, independent of time. Sorted by target; a value
// of 0 is never stored.
struct LongOpinion {
    std::uint32_t target = 0;   // CharacterId value (people list) or TargetId::raw() (target list); never 0
    std::int16_t value = 0;     // -200..+200 units, stored directly, never 0
    std::uint16_t reserved = 0; // always 0
};

static_assert(sizeof(LongOpinion) == 8 && alignof(LongOpinion) == 4);
static_assert(std::has_unique_object_representations_v<LongOpinion>);

// Which id space a modifier's target belongs to, so a CharacterId and a TargetId with
// the same number never collide. Persisted: append-only.
enum class ModifierDomain : std::uint8_t {
    Person = 0, // target is a CharacterId value
    Target = 1, // target is a TargetId raw value (community or topic)
};

// An active circumstance shaping a character's short-term opinion of one target. The
// effect is fixed when the modifier is added and never changes. At most one modifier per
// (domain, target, modifier); sorted by that key.
struct OpinionModifier {
    std::uint32_t target = 0;                        // CharacterId value or TargetId raw, per domain; never 0
    ModifierId modifier;                             // u16 modifier kind, never 0 in a stored entry
    std::int8_t effect = 0;                          // -100..+100 units, stored directly; 0 allowed
    ModifierDomain domain = ModifierDomain::Person;  // u8 enum
};

static_assert(sizeof(OpinionModifier) == 8 && alignof(OpinionModifier) == 4);
static_assert(std::has_unique_object_representations_v<OpinionModifier>);

enum class LongOpinionOutcome : std::uint8_t {
    Updated = 0,             // an existing entry changed to another nonzero value
    Created = 1,             // a new entry was stored
    CreatedWithEviction = 2, // a new entry replaced the weakest one; `evicted` names it
    Removed = 3,             // an existing entry reached 0 and was removed
    Unchanged = 4,           // nothing written: a delta of 0, or a value already saturated in that direction
    Dropped = 5,             // the list is at its limit and the new entry is not stronger than the weakest
    Invalid = 6,             // id 0, a == b, or an invalid TargetId
    NotFound = 7,            // an unknown character
};

// Result of add_long_opinion. `evicted` is typed like the target (CharacterId or
// TargetId) and invalid unless outcome is CreatedWithEviction.
template<class Target>
struct LongOpinionResult {
    LongOpinionOutcome outcome = LongOpinionOutcome::Invalid;
    Target evicted{};
};

} // namespace sim
