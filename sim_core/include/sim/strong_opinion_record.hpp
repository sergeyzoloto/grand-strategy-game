#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include "sim/date.hpp"
#include "sim/ids.hpp"

namespace sim {

// Strong opinion list bounds. A character's people limit depends on extraversion
// (see person_limit); the storage capacity is the maximum.
inline constexpr std::size_t PERSON_LIMIT_MIN = 8;
inline constexpr std::size_t PERSON_LIMIT_MAX = 40;
inline constexpr std::size_t TARGET_LIMIT = 16;

// Deviations are stored in hundredths: raw = value * 100, value -200.00..+200.00.
// The one exception to "raw == value" for bipolar scales.
inline constexpr std::int32_t STRONG_DEV_MAX_RAW = 20000;
// Event deltas are whole numbers clamped to -200..+200.
inline constexpr int EVENT_DELTA_MAX = 200;

// A remembered opinion, stored as a deviation from the weak opinion. Sorted by target.
// The deviation at time now is long_dev + (short_dev - long_dev) * retention^(now - t0).
struct StrongOpinion {
    std::uint32_t target = 0;   // CharacterId value (people list) or TargetId::raw() (target list); never 0
    std::int16_t short_dev = 0; // hundredths, -20000..+20000: deviation right after the last event
    std::int16_t long_dev = 0;  // hundredths, -20000..+20000: deviation the short one decays towards
    Date t0;                    // i32 weeks: date of the last change to short_dev
    CauseId cause;              // u16 cause of the last event; 0 = unspecified
    std::uint16_t reserved = 0; // always 0
};

static_assert(sizeof(StrongOpinion) == 16 && alignof(StrongOpinion) == 4);
static_assert(std::has_unique_object_representations_v<StrongOpinion>);

enum class OpinionEventOutcome : std::uint8_t {
    Updated = 0,             // an existing record was updated
    Created = 1,             // a new record was created
    CreatedWithEviction = 2, // a new record replaced the weakest one; `evicted` names it
    Dropped = 3,             // below the enter threshold, or not stronger than the weakest record at the limit
    Invalid = 4,             // id 0, a == b, or an invalid TargetId
    NotFound = 5,            // an unknown character
};

// Result of an opinion event. `evicted` is typed like the event target (CharacterId or
// TargetId) and invalid unless outcome is CreatedWithEviction.
template<class Target>
struct OpinionEventResult {
    OpinionEventOutcome outcome = OpinionEventOutcome::Invalid;
    Target evicted{};
};

struct StrongMaintainCounts {
    std::size_t removed_decayed = 0;    // records with |dev(now)| < exit_threshold
    std::size_t evicted_over_limit = 0; // weakest people records evicted to fit person_limit
};

} // namespace sim
