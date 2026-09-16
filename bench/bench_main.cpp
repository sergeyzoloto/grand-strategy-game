// Manual benchmarks for sim_core. Build and run in Release:
//   cmake --build build/release --target sim_bench && ./build/release/bench/sim_bench
// Not part of the test run; timings are reported, never asserted.

#include <chrono>
#include <cstdint>
#include <cstdio>
#include <random>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/opinion.hpp"
#include "sim/stance_table.hpp"

using namespace sim;

namespace {

using Clock = std::chrono::steady_clock;

double seconds_since(Clock::time_point start) {
    return std::chrono::duration<double>(Clock::now() - start).count();
}

// Raw mt19937 output only, so the generated world is the same on every standard library.
std::uint32_t pick(std::mt19937& rng, std::uint32_t n) {
    return static_cast<std::uint32_t>(rng() % n);
}

struct Scenario {
    const char* name;
    std::uint32_t chains;          // independent community chains
    std::uint32_t depth;           // communities per chain (root .. leaf)
    std::uint32_t communities_per_character;
    std::uint32_t stance_entries;  // random explicit stances
};

void bench_weak_opinions(const Scenario& scenario) {
    std::mt19937 rng(1234u);
    StanceTable stances;
    const std::uint32_t community_count = scenario.chains * scenario.depth;
    std::vector<CommunityId> leaves;
    for (std::uint32_t chain = 0; chain < scenario.chains; ++chain) {
        const std::uint32_t base = chain * scenario.depth;
        for (std::uint32_t level = 1; level < scenario.depth; ++level) {
            (void)stances.set_parent(CommunityId{base + level + 1}, CommunityId{base + level});
        }
        leaves.push_back(CommunityId{base + scenario.depth});
    }
    for (std::uint32_t i = 0; i < scenario.stance_entries; ++i) {
        const CommunityId from{1 + pick(rng, community_count)};
        const std::uint32_t to = 1 + pick(rng, community_count);
        const TargetId target = pick(rng, 10) == 0 ? *TargetId::from(TopicId{to}) : *TargetId::from(CommunityId{to});
        (void)stances.set_stance(from, target, static_cast<int>(pick(rng, 201)) - 100);
    }

    constexpr std::uint32_t CHARACTERS = 1000;
    CharacterRegistry registry;
    for (std::uint32_t i = 0; i < CHARACTERS; ++i) {
        const CharacterInit init{.stability = static_cast<int>(pick(rng, 201)) - 100,
                                 .openness = static_cast<int>(pick(rng, 201)) - 100,
                                 .extraversion = static_cast<int>(pick(rng, 201)) - 100,
                                 .conscientiousness = static_cast<int>(pick(rng, 201)) - 100,
                                 .agreeableness = static_cast<int>(pick(rng, 201)) - 100,
                                 .reputation = static_cast<int>(pick(rng, 201)) - 100};
        const CharacterId id = registry.create(NameId{i + 1}, Gender::Female, Date{0}, init);
        Character& c = *registry.find(id);
        while (c.involvement().size() < scenario.communities_per_character) {
            (void)c.set_involvement(leaves[pick(rng, static_cast<std::uint32_t>(leaves.size()))], 1 + pick(rng, 255));
        }
    }
    const std::span<const Character> characters = registry.characters();

    const OpinionConfig config{};
    const WorldSeed seed{0xBE7Cu};
    constexpr std::uint32_t EVALUATIONS = 1'000'000;
    double checksum = 0.0;
    const auto start = Clock::now();
    for (std::uint32_t i = 0; i < EVALUATIONS; ++i) {
        const std::uint32_t a = i % CHARACTERS;
        std::uint32_t b = (i * 7 + 1) % CHARACTERS;
        if (b == a) {
            b = (b + 1) % CHARACTERS;
        }
        checksum += weak_opinion(characters[a], characters[b], stances, config, seed);
    }
    const double elapsed = seconds_since(start);
    std::printf("weak(A -> B) %-8s: %u communities per character, chains of depth %u, %zu stances, %zu parents\n",
                scenario.name, scenario.communities_per_character, scenario.depth, stances.stance_count(),
                stances.parent_count());
    std::printf("  %u evaluations in %.3f s = %.1f ns each (checksum %.6f)\n", EVALUATIONS, elapsed,
                elapsed * 1e9 / EVALUATIONS, checksum);
}

void bench_stance_inserts() {
    constexpr std::uint32_t COUNT = 50'000;
    std::mt19937 rng(99u);
    // Distinct keys in random order (Fisher-Yates with raw output).
    std::vector<std::pair<CommunityId, TargetId>> keys;
    keys.reserve(COUNT);
    for (std::uint32_t i = 0; i < COUNT; ++i) {
        keys.emplace_back(CommunityId{1 + i / 50}, *TargetId::from(CommunityId{1 + i % 50}));
    }
    for (std::uint32_t i = COUNT - 1; i > 0; --i) {
        std::swap(keys[i], keys[pick(rng, i + 1)]);
    }
    StanceTable stances;
    const auto start = Clock::now();
    for (const auto& [from, to] : keys) {
        (void)stances.set_stance(from, to, 1);
    }
    const double elapsed = seconds_since(start);
    std::printf("stance inserts: %u in random order in %.3f s = %.1f ns each (%zu stored)\n", COUNT, elapsed,
                elapsed * 1e9 / COUNT, stances.stance_count());
}

} // namespace

int main() {
#ifndef NDEBUG
    std::printf("WARNING: sim_bench built without NDEBUG; timings are not representative. Use the Release build.\n");
#endif
    bench_weak_opinions(Scenario{"typical", 300, 3, 3, 20'000});
    bench_weak_opinions(Scenario{"worst", 300, 6, 8, 20'000});
    bench_stance_inserts();
    return 0;
}
