#pragma once

// Quantization of the real range 0..1 into an unsigned integer T:
//   raw in [0, MAX], value = raw / MAX.
//
// Writes round to nearest and saturate at the bounds; infinities saturate.
// NaN is a precondition violation for quantize_unit; store_unit and add_unit
// leave the current raw unchanged.
//
// Repeated small adds accumulate a small rounding bias; accepted.
//
// Bipolar scales are not quantized floats: they are whole numbers -100..+100
// stored directly (see character.hpp).

#include <cassert>
#include <concepts>
#include <cstdint>
#include <limits>

namespace sim {

namespace detail {

[[nodiscard]] constexpr bool is_nan(double v) noexcept { return v != v; }

// Rounds to nearest, halves away from zero. Requires |v| well inside int64 range.
[[nodiscard]] constexpr std::int64_t round_half_away(double v) noexcept {
    auto t = static_cast<std::int64_t>(v); // truncates toward zero
    const double frac = v - static_cast<double>(t);
    if (frac >= 0.5) {
        ++t;
    } else if (frac <= -0.5) {
        --t;
    }
    return t;
}

} // namespace detail

template<std::unsigned_integral T>
[[nodiscard]] constexpr T quantize_unit(double value) noexcept {
    assert(!detail::is_nan(value));
    const auto max = static_cast<double>(std::numeric_limits<T>::max());
    if (detail::is_nan(value)) {
        return T{0};
    }
    // Bound checks happen before any float-to-int conversion.
    const double scaled = value * max;
    if (scaled >= max) {
        return std::numeric_limits<T>::max();
    }
    if (scaled <= 0.0) {
        return T{0};
    }
    return static_cast<T>(detail::round_half_away(scaled));
}

template<std::unsigned_integral T>
[[nodiscard]] constexpr float dequantize_unit(T raw) noexcept {
    return static_cast<float>(static_cast<double>(raw) / static_cast<double>(std::numeric_limits<T>::max()));
}

// Quantizes `value`; NaN keeps `current`.
template<std::unsigned_integral T>
[[nodiscard]] constexpr T store_unit(T current, double value) noexcept {
    return detail::is_nan(value) ? current : quantize_unit<T>(value);
}

// Adds `delta` in value space and requantizes; NaN keeps `current`.
template<std::unsigned_integral T>
[[nodiscard]] constexpr T add_unit(T current, double delta) noexcept {
    if (detail::is_nan(delta)) {
        return current;
    }
    // Uses the exact double quotient, not the float getter, so the step is not
    // perturbed by float rounding.
    const double value = static_cast<double>(current) / static_cast<double>(std::numeric_limits<T>::max());
    return quantize_unit<T>(value + delta);
}

} // namespace sim
