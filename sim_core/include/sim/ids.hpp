#pragma once

#include <compare>
#include <cstdint>
#include <functional>

namespace sim {

// Strong, tag-typed 32-bit identifier. Value 0 is the invalid id.
// No implicit conversions to or from integers or between different id types.
template<class Tag>
struct Id {
    std::uint32_t value = 0; // opaque handle; 0 = invalid

    constexpr Id() noexcept = default;
    constexpr explicit Id(std::uint32_t v) noexcept : value(v) {}

    [[nodiscard]] constexpr bool valid() const noexcept { return value != 0; }

    friend constexpr bool operator==(Id, Id) noexcept = default;
    friend constexpr auto operator<=>(Id, Id) noexcept = default;
};

// Adding a new id type takes one line here.
using CharacterId = Id<struct CharacterIdTag>;
using NameId = Id<struct NameIdTag>;

} // namespace sim

template<class Tag>
struct std::hash<sim::Id<Tag>> {
    [[nodiscard]] std::size_t operator()(sim::Id<Tag> id) const noexcept {
        return std::hash<std::uint32_t>{}(id.value);
    }
};
