#pragma once

#include <cassert>
#include <compare>
#include <cstdint>
#include <limits>

namespace sim {

// Calendar constant (mean Gregorian year in weeks), not a gameplay coefficient.
inline constexpr double WEEKS_PER_YEAR = 52.1775;

// A point in time: whole weeks since world start. Negative before world start.
struct Date {
    std::int32_t weeks = 0; // weeks since world start, signed, 1 unit = 1 week

    constexpr Date() noexcept = default;
    constexpr explicit Date(std::int32_t w) noexcept : weeks(w) {}

    friend constexpr bool operator==(Date, Date) noexcept = default;
    friend constexpr auto operator<=>(Date, Date) noexcept = default;
};

// Signed number of weeks from `from` to `to` (positive when `to` is later).
[[nodiscard]] constexpr std::int32_t weeks_between(Date from, Date to) noexcept {
    const std::int64_t diff = static_cast<std::int64_t>(to.weeks) - static_cast<std::int64_t>(from.weeks);
    assert(diff >= std::numeric_limits<std::int32_t>::min() && diff <= std::numeric_limits<std::int32_t>::max());
    return static_cast<std::int32_t>(diff);
}

[[nodiscard]] constexpr double weeks_to_years(std::int32_t weeks) noexcept {
    return static_cast<double>(weeks) / WEEKS_PER_YEAR;
}

} // namespace sim
