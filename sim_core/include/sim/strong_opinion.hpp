#pragma once

#include <cstdint>

#include "sim/character.hpp"
#include "sim/date.hpp"
#include "sim/ids.hpp"
#include "sim/opinion_config.hpp"
#include "sim/strong_opinion_record.hpp"

namespace sim {

// r^weeks by repeated squaring, using only multiplication, so stored values stay
// identical across platforms. 1 at 0 weeks. Returns 0 as soon as the running result,
// or a squared base still needed for a remaining exponent bit, drops below 1e-12:
// deterministic, and it keeps subnormal arithmetic out. Requires 0 < r <= 1 and
// weeks >= 0 (negative weeks count as 0).
[[nodiscard]] double retention_power(double r, std::int64_t weeks) noexcept;

// Linear in the character's current stability: retention_unstable at -100,
// retention_stable at +100. A stability change re-times the decay of every existing
// record over its whole elapsed time.
[[nodiscard]] double retention(const Character& a, const OpinionConfig& config) noexcept;
// Linear in stability: amplitude_unstable at -100, amplitude_stable at +100.
[[nodiscard]] double amplitude(const Character& a, const OpinionConfig& config) noexcept;
// PERSON_LIMIT_MIN + (PERSON_LIMIT_MAX - PERSON_LIMIT_MIN) * (extraversion + 100) / 200,
// rounded, in integer arithmetic: 8, 24 and 40 at -100, 0 and +100.
[[nodiscard]] std::size_t person_limit(const Character& a) noexcept;

// Deviation of a record at `now` in hundredths (unrounded). now < t0: debug assert,
// treated as 0 weeks.
[[nodiscard]] double strong_deviation_raw(const StrongOpinion& record, double retention_factor, Date now) noexcept;

// Deviation in value units (-200..+200) of a's record about the target at `now`; 0
// without a record. Reading never writes.
[[nodiscard]] double strong_deviation(const Character& a, CharacterId target, Date now,
                                      const OpinionConfig& config) noexcept;
[[nodiscard]] double strong_deviation(const Character& a, TargetId target, Date now,
                                      const OpinionConfig& config) noexcept;

} // namespace sim
