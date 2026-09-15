#pragma once

// Quantization of real-valued ranges into compact integers.
//
// Signed range -R..+R in a signed integer T:
//   raw in [-MAX, +MAX] (the type's minimum is never stored), value = raw * R / MAX.
// Unit range 0..1 in an unsigned integer T:
//   raw in [0, MAX], value = raw / MAX.
//
// Writes round to nearest (halves away from zero, so +x and -x mirror exactly)
// and saturate at the bounds; infinities saturate. NaN is a precondition
// violation for quantize_*; store_* and add_* leave the current raw unchanged.
//
// Resolution rule: an add_* whose delta is smaller than half a step is a no-op.
// int8 fields (step 1/127) are meant for rare, meaningful changes of at least one
// step. A trait that needs slow drift gets a wider field; no random rounding.
// Repeated small adds on uint16 fields accumulate a small rounding bias; accepted.

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

// ---- signed range -R..+R ---------------------------------------------------

template<std::signed_integral T>
[[nodiscard]] constexpr T quantize_signed(double value, double range) noexcept {
    assert(!detail::is_nan(value));
    assert(range > 0.0);
    const auto max = static_cast<double>(std::numeric_limits<T>::max());
    if (detail::is_nan(value)) {
        return T{0};
    }
    // Bound checks happen before any float-to-int conversion.
    const double scaled = value * max / range;
    if (scaled >= max) {
        return std::numeric_limits<T>::max();
    }
    if (scaled <= -max) {
        return static_cast<T>(-std::numeric_limits<T>::max());
    }
    return static_cast<T>(detail::round_half_away(scaled));
}

template<std::signed_integral T>
[[nodiscard]] constexpr float dequantize_signed(T raw, double range) noexcept {
    assert(raw != std::numeric_limits<T>::min());
    return static_cast<float>(static_cast<double>(raw) * range / static_cast<double>(std::numeric_limits<T>::max()));
}

// Quantizes `value`; NaN keeps `current`.
template<std::signed_integral T>
[[nodiscard]] constexpr T store_signed(T current, double value, double range) noexcept {
    return detail::is_nan(value) ? current : quantize_signed<T>(value, range);
}

// Adds `delta` in value space and requantizes; NaN keeps `current`.
template<std::signed_integral T>
[[nodiscard]] constexpr T add_signed(T current, double delta, double range) noexcept {
    if (detail::is_nan(delta)) {
        return current;
    }
    assert(current != std::numeric_limits<T>::min());
    // Uses the exact double quotient, not the float getter, so the step is not
    // perturbed by float rounding.
    const double value = static_cast<double>(current) * range / static_cast<double>(std::numeric_limits<T>::max());
    return quantize_signed<T>(value + delta, range);
}

// ---- unit range 0..1 -------------------------------------------------------

template<std::unsigned_integral T>
[[nodiscard]] constexpr T quantize_unit(double value) noexcept {
    assert(!detail::is_nan(value));
    const auto max = static_cast<double>(std::numeric_limits<T>::max());
    if (detail::is_nan(value)) {
        return T{0};
    }
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

template<std::unsigned_integral T>
[[nodiscard]] constexpr T store_unit(T current, double value) noexcept {
    return detail::is_nan(value) ? current : quantize_unit<T>(value);
}

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
