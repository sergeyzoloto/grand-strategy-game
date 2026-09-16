// Manual benchmarks for sim_core. Build and run in Release:
//   cmake --build build/release --target sim_bench && ./build/release/bench/sim_bench
// Not part of the test run; timings are reported, never asserted.

#include <array>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <random>
#include <algorithm>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/opinion.hpp"
#include "sim/stance_table.hpp"
#include "sim/strong_opinion.hpp"

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

// A world of 1,500 characters with 3 communities each (chains of depth 3) and random traits.
struct StrongWorld {
    CharacterRegistry registry;
    StanceTable stances;
};

void build_strong_world(StrongWorld& world, std::mt19937& rng, int extraversion_override) {
    constexpr std::uint32_t CHAINS = 300;
    constexpr std::uint32_t DEPTH = 3;
    std::vector<CommunityId> leaves;
    for (std::uint32_t chain = 0; chain < CHAINS; ++chain) {
        const std::uint32_t base = chain * DEPTH;
        for (std::uint32_t level = 1; level < DEPTH; ++level) {
            (void)world.stances.set_parent(CommunityId{base + level + 1}, CommunityId{base + level});
        }
        leaves.push_back(CommunityId{base + DEPTH});
    }
    for (std::uint32_t i = 0; i < 20'000; ++i) {
        const std::uint32_t to = 1 + pick(rng, CHAINS * DEPTH);
        const TargetId target = pick(rng, 10) == 0 ? *TargetId::from(TopicId{to}) : *TargetId::from(CommunityId{to});
        (void)world.stances.set_stance(CommunityId{1 + pick(rng, CHAINS * DEPTH)}, target,
                                       static_cast<int>(pick(rng, 201)) - 100);
    }
    for (std::uint32_t i = 0; i < 1500; ++i) {
        const auto trait = [&] { return static_cast<int>(pick(rng, 201)) - 100; };
        const CharacterInit init{.stability = trait(), .openness = trait(),
                                 .extraversion = extraversion_override != 0 ? extraversion_override : trait(),
                                 .conscientiousness = trait(), .agreeableness = trait(), .reputation = trait()};
        (void)world.registry.create(NameId{i + 1}, Gender::Female, Date{0}, init);
    }
    for (Character& c : world.registry.characters()) {
        while (c.involvement().size() < 3) {
            (void)c.set_involvement(leaves[pick(rng, CHAINS)], 1 + pick(rng, 255));
        }
    }
}

void bench_strong_opinions() {
    const OpinionConfig config{};
    const WorldSeed seed{0x5701u};
    constexpr std::uint32_t CHARACTERS = 1500;

    // 1,000,000 events: random pairs and topics, one week passes every 1,500 events.
    StrongWorld world;
    std::mt19937 rng(777u);
    build_strong_world(world, rng, 0);
    constexpr std::uint32_t EVENTS = 1'000'000;
    std::array<std::size_t, 6> outcomes{};
    auto start = Clock::now();
    for (std::uint32_t i = 0; i < EVENTS; ++i) {
        const CharacterId a{1 + pick(rng, CHARACTERS)};
        const Date now{static_cast<std::int32_t>(i / CHARACTERS)};
        const int delta = static_cast<int>(pick(rng, 401)) - 200;
        if (pick(rng, 4) == 0) {
            const auto result = world.registry.apply_opinion_event(a, *TargetId::from(TopicId{1 + pick(rng, 40)}), delta,
                                                                   now, CauseId{1}, config);
            ++outcomes[static_cast<std::size_t>(result.outcome)];
        } else {
            CharacterId b{1 + pick(rng, CHARACTERS - 1)};
            if (b.value >= a.value) {
                b = CharacterId{b.value + 1};
            }
            const auto result = world.registry.apply_opinion_event(a, b, delta, now, CauseId{1}, config);
            ++outcomes[static_cast<std::size_t>(result.outcome)];
        }
    }
    double elapsed = seconds_since(start);
    std::size_t people_records = 0;
    for (const Character& c : world.registry.characters()) {
        people_records += c.strong_people().size();
    }
    std::printf("opinion events: %u in %.3f s = %.1f ns each (updated %zu, created %zu, with eviction %zu, dropped %zu; "
                "%zu people records)\n",
                EVENTS, elapsed, elapsed * 1e9 / EVENTS, outcomes[0], outcomes[1], outcomes[2], outcomes[3],
                people_records);

    // 1,000,000 opinion reads for pairs that have records.
    std::vector<std::pair<const Character*, const Character*>> pairs;
    for (const Character& c : world.registry.characters()) {
        for (const StrongOpinion& r : c.strong_people()) {
            if (pairs.size() < 5000) {
                pairs.emplace_back(&c, world.registry.find(CharacterId{r.target}));
            }
        }
    }
    const Date read_now{static_cast<std::int32_t>(EVENTS / CHARACTERS + 5)};
    constexpr std::uint32_t READS = 1'000'000;
    double checksum = 0.0;
    start = Clock::now();
    for (std::uint32_t i = 0; i < READS; ++i) {
        const auto& [a, b] = pairs[i % pairs.size()];
        checksum += opinion(*a, *b, world.stances, config, seed, read_now);
    }
    elapsed = seconds_since(start);
    std::printf("opinion reads with records: %u in %.3f s = %.1f ns each (%zu pairs, checksum %.6f)\n", READS, elapsed,
                elapsed * 1e9 / READS, pairs.size(), checksum);

    // maintain over 1,500 characters with full lists (people limit 40, 16 targets).
    StrongWorld full;
    std::mt19937 rng_full(4242u);
    build_strong_world(full, rng_full, 100);
    for (std::uint32_t a = 1; a <= CHARACTERS; ++a) {
        for (std::uint32_t k = 1; k <= PERSON_LIMIT_MAX; ++k) {
            const CharacterId b{(a - 1 + k) % CHARACTERS + 1};
            (void)full.registry.apply_opinion_event(CharacterId{a}, b, 60 + static_cast<int>(pick(rng_full, 141)),
                                                    Date{0}, CauseId{}, config);
        }
        for (std::uint32_t t = 1; t <= TARGET_LIMIT; ++t) {
            (void)full.registry.apply_opinion_event(CharacterId{a}, *TargetId::from(TopicId{t}),
                                                    60 + static_cast<int>(pick(rng_full, 141)), Date{0}, CauseId{}, config);
        }
    }
    std::vector<double> passes;
    std::size_t removed = 0;
    for (int pass = 0; pass < 7; ++pass) {
        start = Clock::now();
        const StrongMaintainCounts counts = full.registry.maintain(Date{pass}, config); // lists stay full
        passes.push_back(seconds_since(start));
        removed += counts.removed_decayed + counts.evicted_over_limit;
    }
    std::sort(passes.begin(), passes.end());
    std::size_t records = 0;
    for (const Character& c : full.registry.characters()) {
        records += c.strong_people().size() + c.strong_targets().size();
    }
    std::printf("maintain over %u characters, %zu records: median %.3f ms per pass (min %.3f, max %.3f; removed %zu)\n",
                CHARACTERS, records, passes[passes.size() / 2] * 1e3, passes.front() * 1e3, passes.back() * 1e3, removed);
}

} // namespace

int main() {
#ifndef NDEBUG
    std::printf("WARNING: sim_bench built without NDEBUG; timings are not representative. Use the Release build.\n");
#endif
    bench_weak_opinions(Scenario{"typical", 300, 3, 3, 20'000});
    bench_weak_opinions(Scenario{"worst", 300, 6, 8, 20'000});
    bench_stance_inserts();
    bench_strong_opinions();
    return 0;
}
