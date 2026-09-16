#pragma once

#include <cstdint>

#include "sim/integral.hpp"

namespace sim {

// Every bipolar scale in the model (traits, reputation, stances, later opinions) is a
// whole number in -100..+100, stored directly.
inline constexpr int BIPOLAR_MIN = -100;
inline constexpr int BIPOLAR_MAX = 100;

namespace detail {

// Clamps any integer to -100..+100 using mixed-sign-safe comparisons.
[[nodiscard]] constexpr std::int8_t clamp_bipolar(StrictIntegral auto v) noexcept {
    return static_cast<std::int8_t>(clamp_integer(v, BIPOLAR_MIN, BIPOLAR_MAX));
}

} // namespace detail

} // namespace sim
