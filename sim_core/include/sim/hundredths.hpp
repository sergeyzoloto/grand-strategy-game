#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdint>

namespace sim::detail {

// Values stored in hundredths: raw = value * 100.
inline constexpr double HUNDREDTHS_PER_UNIT = 100.0;

// Clamps to [lo, hi] and rounds to the nearest integer. Requires a non-NaN input.
// Clamping comes first: an out-of-range float-to-integer conversion is UB.
// std::round is half-away-from-zero regardless of the floating-point rounding mode.
//
// When a fractional delta is added to an integer, round the delta, not the sum:
// the same delta then always adds the same number of raw steps, while rounding the
// sum is not shift-invariant at halves (-3 + 0.5 rounds to -3, -3 + round(0.5) is -2).
[[nodiscard]] inline std::int64_t clamp_round(double scaled, std::int64_t lo, std::int64_t hi) noexcept {
    assert(!std::isnan(scaled));
    const double clamped = std::clamp(scaled, static_cast<double>(lo), static_cast<double>(hi));
    return static_cast<std::int64_t>(std::round(clamped));
}

} // namespace sim::detail
