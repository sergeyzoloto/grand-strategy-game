#include <doctest.h>

#include <cmath>
#include <cstdint>

#include "sim/noise.hpp"

using namespace sim;

// Golden values pin the hash: changing it shifts the noise of the whole world.
static_assert(noise(WorldSeed{0}, CharacterId{0}, NoiseSubject::Person, 0) == -0x1.71f6290f1c0d2p-1);
static_assert(noise(WorldSeed{0}, CharacterId{1}, NoiseSubject::Person, 2) == -0x1.10839f03ab0e6p-2);
static_assert(noise(WorldSeed{12345}, CharacterId{7}, NoiseSubject::Community, 7) == 0x1.e3e6aeb8a04a4p-2);
static_assert(noise(WorldSeed{0xFFFF'FFFF'FFFF'FFFFu}, CharacterId{0xFFFF'FFFFu}, NoiseSubject::Topic, 0xFFFF'FFFFu)
              == -0x1.4ba39697f8f4ep-2);
static_assert(detail::splitmix64(0) != 0); // the increment keeps zero from mapping to zero

TEST_CASE("noise: golden values at run time") {
    // Same values as the static_asserts, computed by the runtime code path of this build.
    volatile std::uint64_t seed_zero = 0;
    CHECK(noise(WorldSeed{seed_zero}, CharacterId{0}, NoiseSubject::Person, 0) == -0x1.71f6290f1c0d2p-1);
    CHECK(noise(WorldSeed{seed_zero}, CharacterId{1}, NoiseSubject::Person, 2) == -0x1.10839f03ab0e6p-2);
    CHECK(noise(WorldSeed{seed_zero + 12345}, CharacterId{7}, NoiseSubject::Community, 7) == 0x1.e3e6aeb8a04a4p-2);
    CHECK(noise(WorldSeed{~seed_zero}, CharacterId{0xFFFF'FFFFu}, NoiseSubject::Topic, 0xFFFF'FFFFu)
          == -0x1.4ba39697f8f4ep-2);
}

TEST_CASE("noise: deterministic and in [-1, 1]") {
    for (std::uint32_t i = 0; i < 2000; ++i) {
        const double n = noise(WorldSeed{99}, CharacterId{i}, NoiseSubject::Topic, i * 7919u);
        if (!(n >= -1.0 && n <= 1.0)) {
            FAIL_CHECK("out of range at " << i);
        }
        if (n != noise(WorldSeed{99}, CharacterId{i}, NoiseSubject::Topic, i * 7919u)) {
            FAIL_CHECK("not deterministic at " << i);
        }
    }
}

TEST_CASE("noise: directed, seed-dependent, and person differs from community with the same number") {
    const WorldSeed seed{42};
    int directed_same = 0;
    int seed_same = 0;
    int subject_same = 0;
    for (std::uint32_t i = 1; i <= 1000; ++i) {
        const std::uint32_t j = i + 1;
        if (noise(seed, CharacterId{i}, NoiseSubject::Person, j) == noise(seed, CharacterId{j}, NoiseSubject::Person, i)) {
            ++directed_same;
        }
        if (noise(seed, CharacterId{i}, NoiseSubject::Person, j) == noise(WorldSeed{43}, CharacterId{i}, NoiseSubject::Person, j)) {
            ++seed_same;
        }
        if (noise(seed, CharacterId{i}, NoiseSubject::Person, j) == noise(seed, CharacterId{i}, NoiseSubject::Community, j)) {
            ++subject_same;
        }
    }
    CHECK(directed_same == 0);
    CHECK(seed_same == 0);
    CHECK(subject_same == 0);
}

TEST_CASE("noise: the mean over 100,000 pairs is within 0.01 of 0") {
    const WorldSeed seed{0xC0FFEEu};
    double sum = 0.0;
    double sum_abs = 0.0;
    constexpr std::uint32_t PAIRS = 100000;
    for (std::uint32_t i = 0; i < PAIRS; ++i) {
        const double n = noise(seed, CharacterId{i + 1}, NoiseSubject::Person, (i * 2654435761u) % 5000u + 1);
        sum += n;
        sum_abs += std::abs(n);
    }
    const double mean = sum / PAIRS;
    const double mean_abs = sum_abs / PAIRS;
    MESSAGE("noise mean = " << mean << ", mean |noise| = " << mean_abs);
    CHECK(std::abs(mean) <= 0.01);
    CHECK(std::abs(mean_abs - 0.5) <= 0.01); // uniform on [-1, 1]
}
