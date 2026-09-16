#pragma once

#include <cstdint>

#include "sim/ids.hpp"

namespace sim {

// Seed of all deterministic noise in a world. Passed explicitly; never global.
struct WorldSeed {
    std::uint64_t value = 0;
};

// Kind of the noise target. Part of the hash input: append-only, never renumber,
// or the noise of the whole world shifts. Independent of TargetKind, so a person and
// a community with the same number get independent noise.
enum class NoiseSubject : std::uint8_t {
    Person = 0,
    Community = 1,
    Topic = 2,
};

namespace detail {

// One full SplitMix64 step: add the golden-ratio increment, then the finalizer.
// The increment keeps a zero input from mapping to zero.
[[nodiscard]] constexpr std::uint64_t splitmix64(std::uint64_t x) noexcept {
    x += 0x9e3779b97f4a7c15u;
    x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9u;
    x = (x ^ (x >> 27)) * 0x94d049bb133111ebu;
    return x ^ (x >> 31);
}

} // namespace detail

// Deterministic, directed noise in [-1, 1] for (world seed, source character, target).
// Integer mixing only; the final mapping uses exact integer-to-double conversion and
// * / -, so the result is identical across platforms. Changing this function shifts
// the noise of the whole world: golden values in the tests pin it.
[[nodiscard]] constexpr double noise(WorldSeed seed, CharacterId source, NoiseSubject subject,
                                     std::uint32_t target) noexcept {
    std::uint64_t h = detail::splitmix64(seed.value);
    h = detail::splitmix64(h ^ source.value);
    h = detail::splitmix64(h ^ ((static_cast<std::uint64_t>(subject) << 32) | target));
    const std::uint64_t bits53 = h >> 11;                        // uniform in [0, 2^53 - 1]
    constexpr double MAX53 = static_cast<double>((std::uint64_t{1} << 53) - 1);
    return static_cast<double>(bits53) / MAX53 * 2.0 - 1.0;      // exact conversion, maps onto [-1, 1]
}

} // namespace sim
