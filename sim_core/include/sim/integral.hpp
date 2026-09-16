#pragma once

#include <concepts>
#include <utility>

namespace sim {

// Integer argument types accepted wherever the model takes whole numbers: any
// integer width and signedness. Floating point is rejected so fractional values
// never truncate silently; the caller rounds explicitly. bool and character types
// are rejected as well (std::cmp_less/std::cmp_greater do not accept them).
template<class T>
concept StrictIntegral = std::integral<T>
                      && !std::same_as<T, bool>
                      && !std::same_as<T, char>
                      && !std::same_as<T, wchar_t>
                      && !std::same_as<T, char8_t>
                      && !std::same_as<T, char16_t>
                      && !std::same_as<T, char32_t>;

namespace detail {

// Clamps any integer to [lo, hi] using mixed-sign-safe comparisons.
[[nodiscard]] constexpr int clamp_integer(StrictIntegral auto v, int lo, int hi) noexcept {
    if (std::cmp_less(v, lo)) {
        return lo;
    }
    if (std::cmp_greater(v, hi)) {
        return hi;
    }
    return static_cast<int>(v);
}

} // namespace detail

} // namespace sim
