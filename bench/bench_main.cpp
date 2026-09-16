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

    // maintain over 1,500 characters with full lists (40 people, 16 topics each). Every record
    // gets its own t0 in weeks 0..519 and maintain runs at week 520, so retention_power sees
    // exponents 1..520. Deltas of 20..200 in either direction let some records decay below the
    // exit threshold; a third of the characters then get a random extraversion, so the pass also
    // trims people lists over their limit. Each pass runs on a fresh copy of the same template.
    StrongWorld full;
    std::mt19937 rng_full(4242u);
    build_strong_world(full, rng_full, 100);
    constexpr std::int32_t FILL_WEEKS = 520;
    const auto fill_delta = [&] {
        const int magnitude = 20 + static_cast<int>(pick(rng_full, 181));
        return pick(rng_full, 2) == 0 ? magnitude : -magnitude;
    };
    const auto fill_date = [&] { return Date{static_cast<std::int32_t>(pick(rng_full, FILL_WEEKS))}; };
    std::size_t fill_created = 0;
    for (std::uint32_t a = 1; a <= CHARACTERS; ++a) {
        // Distinct targets and no list reaches its limit before its last insert, so no event
        // reads another record at a date before its t0.
        for (std::uint32_t k = 1; k <= PERSON_LIMIT_MAX; ++k) {
            const CharacterId b{(a - 1 + k) % CHARACTERS + 1};
            fill_created += full.registry.apply_opinion_event(CharacterId{a}, b, fill_delta(), fill_date(), CauseId{},
                                                              config).outcome == OpinionEventOutcome::Created;
        }
        for (std::uint32_t t = 1; t <= TARGET_LIMIT; ++t) {
            fill_created += full.registry.apply_opinion_event(CharacterId{a}, *TargetId::from(TopicId{t}), fill_delta(),
                                                              fill_date(), CauseId{}, config).outcome
                            == OpinionEventOutcome::Created;
        }
        if (a % 3 == 0) {
            full.registry.find(CharacterId{a})->set_extraversion(static_cast<int>(pick(rng_full, 201)) - 100);
        }
    }
    const Date maintain_now{FILL_WEEKS};
    std::vector<double> passes;
    std::printf("maintain over %u characters at week %d (t0 in weeks 0..%d, %zu records created):\n", CHARACTERS,
                maintain_now.weeks, FILL_WEEKS - 1, fill_created);
    for (int pass = 0; pass < 7; ++pass) {
        CharacterRegistry work = full.registry; // untimed refill
        std::size_t before = 0;
        for (const Character& c : work.characters()) {
            before += c.strong_people().size() + c.strong_targets().size();
        }
        start = Clock::now();
        const StrongMaintainCounts counts = work.maintain(maintain_now, config);
        const double pass_seconds = seconds_since(start);
        passes.push_back(pass_seconds);
        std::printf("  pass %d: %zu records before, %zu removed decayed, %zu trimmed, %.3f ms = %.1f ns per record\n",
                    pass, before, counts.removed_decayed, counts.evicted_over_limit, pass_seconds * 1e3,
                    pass_seconds * 1e9 / static_cast<double>(before));
    }
    std::sort(passes.begin(), passes.end());
    std::printf("  median %.3f ms per pass (min %.3f, max %.3f) = %.1f ns per record\n",
                passes[passes.size() / 2] * 1e3, passes.front() * 1e3, passes.back() * 1e3,
                passes[passes.size() / 2] * 1e9 / static_cast<double>(fill_created));
}

// A structured world: 10 roots, 5 level-2 communities per root, 5 level-3 per level-2 and
// 6 leaves per level-3 (1,810 communities, chains of depth 4). Stances sit on the upper
// levels: every root towards every root including itself (in-group cohesion) and 20 topics,
// every level-2 community 10 stances towards roots and level-2 communities, a fifth of the
// level-3 communities 3 stances; leaves have none. Each character is in 3 leaves, 2 of them
// under the same root. Complements the random worlds above, which are the worst case for the
// block lookup (every community has stances, almost none match).
void bench_structured_world() {
    constexpr std::uint32_t ROOTS = 10;
    constexpr std::uint32_t FANOUT_2 = 5;
    constexpr std::uint32_t FANOUT_3 = 5;
    constexpr std::uint32_t LEAVES_PER_3 = 6;
    constexpr std::uint32_t LEVEL2 = ROOTS * FANOUT_2;
    constexpr std::uint32_t LEVEL3 = LEVEL2 * FANOUT_3;
    constexpr std::uint32_t LEAVES = LEVEL3 * LEAVES_PER_3;
    constexpr std::uint32_t LEAVES_PER_ROOT = LEAVES / ROOTS;
    constexpr std::uint32_t TOPICS = 100;
    // Ids: roots 1..10, level 2 next, then level 3, then leaves; index i of a level is 0-based.
    const auto root_id = [](std::uint32_t i) { return CommunityId{1 + i}; };
    const auto level2_id = [](std::uint32_t i) { return CommunityId{1 + ROOTS + i}; };
    const auto level3_id = [](std::uint32_t i) { return CommunityId{1 + ROOTS + LEVEL2 + i}; };
    const auto leaf_id = [](std::uint32_t i) { return CommunityId{1 + ROOTS + LEVEL2 + LEVEL3 + i}; };

    std::mt19937 rng(2024u);
    const auto value = [&] { return static_cast<int>(pick(rng, 201)) - 100; };
    StanceTable stances;
    for (std::uint32_t i = 0; i < LEVEL2; ++i) {
        (void)stances.set_parent(level2_id(i), root_id(i / FANOUT_2));
    }
    for (std::uint32_t i = 0; i < LEVEL3; ++i) {
        (void)stances.set_parent(level3_id(i), level2_id(i / FANOUT_3));
    }
    for (std::uint32_t i = 0; i < LEAVES; ++i) {
        (void)stances.set_parent(leaf_id(i), level3_id(i / LEAVES_PER_3));
    }
    for (std::uint32_t r = 0; r < ROOTS; ++r) {
        for (std::uint32_t other = 0; other < ROOTS; ++other) {
            const int v = other == r ? 40 + static_cast<int>(pick(rng, 61)) : value();
            (void)stances.set_stance(root_id(r), *TargetId::from(root_id(other)), v);
        }
        for (std::uint32_t t = 0; t < 20; ++t) {
            (void)stances.set_stance(root_id(r), *TargetId::from(TopicId{1 + pick(rng, TOPICS)}), value());
        }
    }
    for (std::uint32_t i = 0; i < LEVEL2; ++i) {
        for (std::uint32_t k = 0; k < 10; ++k) {
            const std::uint32_t target = pick(rng, ROOTS + LEVEL2);
            const CommunityId to = target < ROOTS ? root_id(target) : level2_id(target - ROOTS);
            (void)stances.set_stance(level2_id(i), *TargetId::from(to), value());
        }
    }
    for (std::uint32_t i = 0; i < LEVEL3; ++i) {
        if (pick(rng, 5) != 0) {
            continue;
        }
        for (std::uint32_t k = 0; k < 3; ++k) {
            const std::uint32_t target = pick(rng, ROOTS + LEVEL2);
            const CommunityId to = target < ROOTS ? root_id(target) : level2_id(target - ROOTS);
            (void)stances.set_stance(level3_id(i), *TargetId::from(to), value());
        }
    }

    constexpr std::uint32_t CHARACTERS = 1000;
    CharacterRegistry registry;
    for (std::uint32_t i = 0; i < CHARACTERS; ++i) {
        const CharacterInit init{.stability = value(), .openness = value(), .extraversion = value(),
                                 .conscientiousness = value(), .agreeableness = value(), .reputation = value()};
        (void)registry.create(NameId{i + 1}, Gender::Female, Date{0}, init);
    }
    // Characters by level-3 community, for acquaintance pairs.
    std::vector<std::vector<std::uint32_t>> members_of_level3(LEVEL3);
    for (std::uint32_t i = 0; i < CHARACTERS; ++i) {
        Character& c = *registry.find(CharacterId{i + 1});
        const std::uint32_t root = pick(rng, ROOTS);
        while (c.involvement().size() < 3) {
            const std::uint32_t leaf = c.involvement().size() < 2 ? root * LEAVES_PER_ROOT + pick(rng, LEAVES_PER_ROOT)
                                                                   : pick(rng, LEAVES);
            std::vector<std::uint32_t>& members = members_of_level3[leaf / LEAVES_PER_3];
            if (c.set_involvement(leaf_id(leaf), 1 + pick(rng, 255)) == EditResult::Ok
                && std::find(members.begin(), members.end(), i) == members.end()) {
                members.push_back(i);
            }
        }
    }
    const std::span<const Character> characters = registry.characters();

    // Pair lists: random pairs, and acquaintances sharing a leaf or a level-3 community.
    constexpr std::uint32_t PAIRS = 100'000;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> random_pairs;
    std::vector<std::pair<std::uint32_t, std::uint32_t>> acquaintance_pairs;
    while (random_pairs.size() < PAIRS) {
        const std::uint32_t a = pick(rng, CHARACTERS);
        const std::uint32_t b = pick(rng, CHARACTERS);
        if (a != b) {
            random_pairs.emplace_back(a, b);
        }
    }
    std::size_t same_leaf = 0;
    while (acquaintance_pairs.size() < PAIRS) {
        const std::uint32_t a = pick(rng, CHARACTERS);
        const auto involvement = characters[a].involvement();
        const std::uint32_t leaf =
            involvement[pick(rng, static_cast<std::uint32_t>(involvement.size()))].community.value - leaf_id(0).value;
        const std::vector<std::uint32_t>& group = members_of_level3[leaf / LEAVES_PER_3];
        const std::uint32_t b = group[pick(rng, static_cast<std::uint32_t>(group.size()))];
        if (a == b) {
            continue;
        }
        acquaintance_pairs.emplace_back(a, b);
        for (const InvolvementEntry& e : characters[b].involvement()) {
            if (e.community == leaf_id(leaf)) {
                ++same_leaf;
            }
        }
    }

    const OpinionConfig config{};
    const WorldSeed seed{0x5747u};
    std::printf("weak(A -> B) structured: %u communities (depth 4), 3 per character, %zu stances, %zu parents\n",
                ROOTS + LEVEL2 + LEVEL3 + LEAVES, stances.stance_count(), stances.parent_count());
    constexpr std::uint32_t EVALUATIONS = 1'000'000;
    const auto run_pairs = [&](const char* name, const std::vector<std::pair<std::uint32_t, std::uint32_t>>& pairs) {
        std::size_t nonzero_community = 0; // untimed: how often stances contribute at all
        for (const auto& [a, b] : pairs) {
            nonzero_community +=
                weak_opinion_breakdown(characters[a], characters[b], stances, config, seed).community != 0.0;
        }
        double checksum = 0.0;
        const auto start = Clock::now();
        for (std::uint32_t i = 0; i < EVALUATIONS; ++i) {
            const auto& [a, b] = pairs[i % pairs.size()];
            checksum += weak_opinion(characters[a], characters[b], stances, config, seed);
        }
        const double elapsed = seconds_since(start);
        std::printf("  %-13s %u evaluations in %.3f s = %.1f ns each (community term nonzero for %.1f%% of pairs, "
                    "checksum %.6f)\n",
                    name, EVALUATIONS, elapsed, elapsed * 1e9 / EVALUATIONS,
                    100.0 * static_cast<double>(nonzero_community) / static_cast<double>(pairs.size()), checksum);
    };
    run_pairs("random:", random_pairs);
    std::printf("  acquaintances share a level-3 community; %.1f%% of them also share a leaf\n",
                100.0 * static_cast<double>(same_leaf) / static_cast<double>(PAIRS));
    run_pairs("acquaintances:", acquaintance_pairs);

    double checksum = 0.0;
    const auto start = Clock::now();
    for (std::uint32_t i = 0; i < EVALUATIONS; ++i) {
        checksum += weak_opinion(characters[i % CHARACTERS], *TargetId::from(TopicId{1 + (i * 7) % TOPICS}), stances,
                                 config, seed);
    }
    const double elapsed = seconds_since(start);
    std::printf("weak(A -> topic) structured: %u evaluations in %.3f s = %.1f ns each (checksum %.6f)\n", EVALUATIONS,
                elapsed, elapsed * 1e9 / EVALUATIONS, checksum);
}

} // namespace

int main() {
#ifndef NDEBUG
    std::printf("WARNING: sim_bench built without NDEBUG; timings are not representative. Use the Release build.\n");
#endif
    bench_weak_opinions(Scenario{"typical", 300, 3, 3, 20'000});
    bench_weak_opinions(Scenario{"worst", 300, 6, 8, 20'000});
    bench_structured_world();
    bench_stance_inserts();
    bench_strong_opinions();
    return 0;
}
