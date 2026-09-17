// Manual benchmarks for sim_core. Build and run in Release:
//   cmake --build build/release --target sim_bench && ./build/release/bench/sim_bench
// Not part of the test run; timings are reported, never asserted.
//
// Every section counts its operations by outcome and prints the counts, and an operation
// that must succeed aborts the section with a message when it doesn't: a benchmark that
// silently measures failing calls measures the wrong thing.

#include <algorithm>
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <initializer_list>
#include <random>
#include <string>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/opinion.hpp"
#include "sim/stance_table.hpp"
#include "sim/personal_opinion.hpp"

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

// ---- outcome checks -------------------------------------------------------------------------

// Thrown when an operation that must succeed doesn't; main reports it and skips the section.
struct SectionAborted {
    std::string message;
};

const char* outcome_name(EditResult r) {
    switch (r) {
    case EditResult::Ok: return "Ok";
    case EditResult::Full: return "Full";
    case EditResult::Duplicate: return "Duplicate";
    case EditResult::Conflict: return "Conflict";
    case EditResult::NotFound: return "NotFound";
    case EditResult::Invalid: return "Invalid";
    }
    return "?";
}

const char* outcome_name(LongOpinionOutcome o) {
    switch (o) {
    case LongOpinionOutcome::Updated: return "Updated";
    case LongOpinionOutcome::Created: return "Created";
    case LongOpinionOutcome::CreatedWithEviction: return "CreatedWithEviction";
    case LongOpinionOutcome::Removed: return "Removed";
    case LongOpinionOutcome::Unchanged: return "Unchanged";
    case LongOpinionOutcome::Dropped: return "Dropped";
    case LongOpinionOutcome::Invalid: return "Invalid";
    case LongOpinionOutcome::NotFound: return "NotFound";
    }
    return "?";
}

// Outcome counts of one kind of operation. Counting is a single increment, cheap enough for
// timed loops.
class Outcomes {
public:
    explicit Outcomes(const char* operation) : operation_(operation) {}

    // Counts the result; aborts the section unless it is Ok.
    void require_ok(EditResult r) {
        ++edit_[static_cast<std::size_t>(r)];
        if (r != EditResult::Ok) {
            abort_with(outcome_name(r), "Ok");
        }
    }
    // Counts the outcome; aborts the section unless it is one of `allowed`.
    void require(LongOpinionOutcome o, std::initializer_list<LongOpinionOutcome> allowed) {
        ++long_[static_cast<std::size_t>(o)];
        if (std::find(allowed.begin(), allowed.end(), o) == allowed.end()) {
            std::string expected;
            for (const LongOpinionOutcome a : allowed) {
                expected += (expected.empty() ? "" : " or ") + std::string(outcome_name(a));
            }
            abort_with(outcome_name(o), expected.c_str());
        }
    }
    // Counts a create; aborts the section on an invalid id.
    CharacterId require_created(CharacterId id) {
        ++edit_[static_cast<std::size_t>(id.valid() ? EditResult::Ok : EditResult::Invalid)];
        if (!id.valid()) {
            abort_with("an invalid id", "a valid id");
        }
        return id;
    }

    // "operation: Ok 600" listing every outcome that occurred.
    [[nodiscard]] std::string summary() const {
        std::string text = std::string(operation_) + ":";
        for (std::size_t i = 0; i < edit_.size(); ++i) {
            if (edit_[i] != 0) {
                text += " " + std::string(outcome_name(static_cast<EditResult>(i))) + " " + std::to_string(edit_[i]);
            }
        }
        for (std::size_t i = 0; i < long_.size(); ++i) {
            if (long_[i] != 0) {
                text += " " + std::string(outcome_name(static_cast<LongOpinionOutcome>(i))) + " "
                      + std::to_string(long_[i]);
            }
        }
        return text;
    }

private:
    [[noreturn]] void abort_with(const char* got, const char* expected) const {
        throw SectionAborted{std::string(operation_) + " returned " + got + " where " + expected + " is required"};
    }

    const char* operation_;
    std::array<std::size_t, 6> edit_{};
    std::array<std::size_t, 8> long_{};
};

void print_outcomes(std::initializer_list<const Outcomes*> outcomes) {
    std::string text;
    for (const Outcomes* o : outcomes) {
        text += (text.empty() ? "" : "; ") + o->summary();
    }
    std::printf("  outcomes: %s\n", text.c_str());
}

struct Scenario {
    const char* name;
    std::uint32_t chains;          // independent community chains
    std::uint32_t depth;           // communities per chain (root .. leaf)
    std::uint32_t communities_per_character;
    std::uint32_t stance_entries;  // random explicit stances
};

void bench_weak_opinions(const Scenario& scenario) {
    Outcomes parents("set_parent");
    Outcomes stance_edits("set_stance");
    Outcomes creates("create");
    Outcomes involvement("set_involvement");
    std::mt19937 rng(1234u);
    StanceTable stances;
    const std::uint32_t community_count = scenario.chains * scenario.depth;
    std::vector<CommunityId> leaves;
    for (std::uint32_t chain = 0; chain < scenario.chains; ++chain) {
        const std::uint32_t base = chain * scenario.depth;
        for (std::uint32_t level = 1; level < scenario.depth; ++level) {
            parents.require_ok(stances.set_parent(CommunityId{base + level + 1}, CommunityId{base + level}));
        }
        leaves.push_back(CommunityId{base + scenario.depth});
    }
    for (std::uint32_t i = 0; i < scenario.stance_entries; ++i) {
        const CommunityId from{1 + pick(rng, community_count)};
        const std::uint32_t to = 1 + pick(rng, community_count);
        const TargetId target = pick(rng, 10) == 0 ? *TargetId::from(TopicId{to}) : *TargetId::from(CommunityId{to});
        stance_edits.require_ok(stances.set_stance(from, target, static_cast<int>(pick(rng, 201)) - 100));
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
        const CharacterId id = creates.require_created(registry.create(NameId{i + 1}, Gender::Female, Date{0}, init));
        Character& c = *registry.find(id);
        while (c.involvement().size() < scenario.communities_per_character) {
            involvement.require_ok(
                c.set_involvement(leaves[pick(rng, static_cast<std::uint32_t>(leaves.size()))], 1 + pick(rng, 255)));
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
    print_outcomes({&parents, &stance_edits, &creates, &involvement});
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
    Outcomes inserts("set_stance");
    const auto start = Clock::now();
    for (const auto& [from, to] : keys) {
        inserts.require_ok(stances.set_stance(from, to, 1));
    }
    const double elapsed = seconds_since(start);
    std::printf("stance inserts: %u in random order in %.3f s = %.1f ns each (%zu stored)\n", COUNT, elapsed,
                elapsed * 1e9 / COUNT, stances.stance_count());
    print_outcomes({&inserts});
}

// A world of 1,500 characters with 3 communities each (chains of depth 3) and random traits.
struct PersonalWorld {
    CharacterRegistry registry;
    StanceTable stances;
};

void build_personal_world(PersonalWorld& world, std::mt19937& rng, int extraversion_override) {
    Outcomes parents("world set_parent");
    Outcomes stance_edits("world set_stance");
    Outcomes creates("world create");
    Outcomes involvement("world set_involvement");
    constexpr std::uint32_t CHAINS = 300;
    constexpr std::uint32_t DEPTH = 3;
    std::vector<CommunityId> leaves;
    for (std::uint32_t chain = 0; chain < CHAINS; ++chain) {
        const std::uint32_t base = chain * DEPTH;
        for (std::uint32_t level = 1; level < DEPTH; ++level) {
            parents.require_ok(world.stances.set_parent(CommunityId{base + level + 1}, CommunityId{base + level}));
        }
        leaves.push_back(CommunityId{base + DEPTH});
    }
    for (std::uint32_t i = 0; i < 20'000; ++i) {
        const std::uint32_t to = 1 + pick(rng, CHAINS * DEPTH);
        const TargetId target = pick(rng, 10) == 0 ? *TargetId::from(TopicId{to}) : *TargetId::from(CommunityId{to});
        stance_edits.require_ok(world.stances.set_stance(CommunityId{1 + pick(rng, CHAINS * DEPTH)}, target,
                                                         static_cast<int>(pick(rng, 201)) - 100));
    }
    for (std::uint32_t i = 0; i < 1500; ++i) {
        const auto trait = [&] { return static_cast<int>(pick(rng, 201)) - 100; };
        const CharacterInit init{.stability = trait(), .openness = trait(),
                                 .extraversion = extraversion_override != 0 ? extraversion_override : trait(),
                                 .conscientiousness = trait(), .agreeableness = trait(), .reputation = trait()};
        (void)creates.require_created(world.registry.create(NameId{i + 1}, Gender::Female, Date{0}, init));
    }
    for (Character& c : world.registry.characters()) {
        while (c.involvement().size() < 3) {
            involvement.require_ok(c.set_involvement(leaves[pick(rng, CHAINS)], 1 + pick(rng, 255)));
        }
    }
    print_outcomes({&parents, &stance_edits, &creates, &involvement});
}

int random_nonzero(std::mt19937& rng, int max) {
    const int magnitude = 1 + static_cast<int>(pick(rng, static_cast<std::uint32_t>(max)));
    return pick(rng, 2) == 0 ? magnitude : -magnitude;
}

void bench_personal_opinions() {
    const OpinionConfig config{};
    const WorldSeed seed{0x5701u};
    constexpr std::uint32_t CHARACTERS = 1500;
    const auto other = [](std::uint32_t a, std::uint32_t k) { return CharacterId{(a - 1 + k) % CHARACTERS + 1}; };

    // Background state: every character holds 20 long-term opinions of people and 16
    // modifiers (kinds 1 and 2 on its next 8 people), so lists are not empty.
    std::printf("personal opinions (1,500 characters):\n");
    PersonalWorld world;
    std::mt19937 rng(777u);
    build_personal_world(world, rng, 100);
    Outcomes background_longs("background add_long_opinion");
    Outcomes background_modifiers("background add_modifier");
    for (std::uint32_t a = 1; a <= CHARACTERS; ++a) {
        for (std::uint32_t k = 1; k <= 20; ++k) {
            background_longs.require(
                world.registry.add_long_opinion(CharacterId{a}, other(a, k), random_nonzero(rng, 200)).outcome,
                {LongOpinionOutcome::Created});
        }
        for (std::uint32_t k = 1; k <= 8; ++k) {
            for (std::uint16_t m = 1; m <= 2; ++m) {
                background_modifiers.require_ok(
                    world.registry.add_modifier(CharacterId{a}, other(a, k), ModifierId{m}, random_nonzero(rng, 100)));
            }
        }
    }
    print_outcomes({&background_longs, &background_modifiers});

    // 1,000,000 add-and-remove pairs: kinds 3..6 on random people, so every add is Ok and
    // lands at a varying position in a 16-entry list.
    constexpr std::uint32_t PAIRS = 1'000'000;
    Outcomes adds("add_modifier");
    Outcomes removes("remove_modifier");
    auto start = Clock::now();
    for (std::uint32_t i = 0; i < PAIRS; ++i) {
        const CharacterId a{1 + pick(rng, CHARACTERS)};
        const CharacterId b = other(a.value, 1 + pick(rng, CHARACTERS - 1));
        const ModifierId m{static_cast<std::uint16_t>(3 + pick(rng, 4))};
        adds.require_ok(world.registry.add_modifier(a, b, m, static_cast<int>(pick(rng, 201)) - 100));
        removes.require_ok(world.registry.remove_modifier(a, b, m));
    }
    double elapsed = seconds_since(start);
    std::printf("modifier add+remove: %u pairs in %.3f s = %.1f ns per pair (16 modifiers per character)\n", PAIRS,
                elapsed, elapsed * 1e9 / PAIRS);
    print_outcomes({&adds, &removes});

    // 1,000,000 long-term changes on the 20 people each character already has an opinion of
    // (small deltas: mostly Updated, sometimes Removed and later Created again).
    constexpr std::uint32_t LONG_CHANGES = 1'000'000;
    Outcomes long_changes("add_long_opinion");
    start = Clock::now();
    for (std::uint32_t i = 0; i < LONG_CHANGES; ++i) {
        const CharacterId a{1 + pick(rng, CHARACTERS)};
        const CharacterId b = other(a.value, 1 + pick(rng, 20));
        // At most 20 entries under a limit of 40: no eviction or drop can occur.
        long_changes.require(world.registry.add_long_opinion(a, b, static_cast<int>(pick(rng, 41)) - 20).outcome,
                             {LongOpinionOutcome::Updated, LongOpinionOutcome::Created, LongOpinionOutcome::Removed,
                              LongOpinionOutcome::Unchanged});
    }
    elapsed = seconds_since(start);
    std::printf("long-term changes: %u in %.3f s = %.1f ns each\n", LONG_CHANGES, elapsed,
                elapsed * 1e9 / LONG_CHANGES);
    print_outcomes({&long_changes});

    // 1,000,000 link-and-unlink pairs (Employee) between random characters.
    constexpr std::uint32_t LINKS = 1'000'000;
    Outcomes links("link");
    Outcomes unlinks("unlink");
    start = Clock::now();
    for (std::uint32_t i = 0; i < LINKS; ++i) {
        const CharacterId a{1 + pick(rng, CHARACTERS)};
        const CharacterId b = other(a.value, 1 + pick(rng, CHARACTERS - 1));
        links.require_ok(world.registry.link(a, RelationType::Employee, b));
        unlinks.require_ok(world.registry.unlink(a, RelationType::Employee, b));
    }
    elapsed = seconds_since(start);
    std::printf("link+unlink: %u pairs in %.3f s = %.1f ns per pair\n", LINKS, elapsed, elapsed * 1e9 / LINKS);
    print_outcomes({&links, &unlinks});

    // 1,000,000 opinion reads for pairs with a long-term entry and two modifiers.
    std::vector<std::pair<const Character*, const Character*>> pairs;
    for (std::uint32_t a = 1; a <= CHARACTERS; ++a) {
        for (std::uint32_t k = 1; k <= 8; ++k) {
            pairs.emplace_back(world.registry.find(CharacterId{a}), world.registry.find(other(a, k)));
        }
    }
    constexpr std::uint32_t READS = 1'000'000;
    double checksum = 0.0;
    start = Clock::now();
    for (std::uint32_t i = 0; i < READS; ++i) {
        const auto& [a, b] = pairs[i % pairs.size()];
        checksum += opinion(*a, *b, world.stances, config, seed);
    }
    elapsed = seconds_since(start);
    std::printf("opinion reads with records: %u in %.3f s = %.1f ns each (%zu pairs, checksum %.6f)\n", READS, elapsed,
                elapsed * 1e9 / READS, pairs.size(), checksum);

    // maintain trimming over 1,500 characters: a template with 40 people and 16 targets each
    // at extraversion +100, then a random extraversion for every character. Each pass runs
    // on a fresh copy of the template.
    PersonalWorld full;
    std::mt19937 rng_full(4242u);
    build_personal_world(full, rng_full, 100);
    Outcomes template_people("template add_long_opinion (people)");
    Outcomes template_targets("template add_long_opinion (targets)");
    for (std::uint32_t a = 1; a <= CHARACTERS; ++a) {
        for (std::uint32_t k = 1; k <= PERSON_LIMIT_MAX; ++k) {
            template_people.require(
                full.registry.add_long_opinion(CharacterId{a}, other(a, k), random_nonzero(rng_full, 200)).outcome,
                {LongOpinionOutcome::Created});
        }
        for (std::uint32_t t = 1; t <= TARGET_LIMIT; ++t) {
            template_targets.require(full.registry
                                         .add_long_opinion(CharacterId{a}, *TargetId::from(TopicId{t}),
                                                           random_nonzero(rng_full, 200))
                                         .outcome,
                                     {LongOpinionOutcome::Created});
        }
        full.registry.find(CharacterId{a})->set_extraversion(static_cast<int>(pick(rng_full, 201)) - 100);
    }
    std::vector<double> passes;
    std::printf("maintain trimming over %u characters:\n", CHARACTERS);
    print_outcomes({&template_people, &template_targets});
    for (int pass = 0; pass < 7; ++pass) {
        CharacterRegistry work = full.registry; // untimed refill
        std::size_t before = 0;
        for (const Character& c : work.characters()) {
            before += c.long_people().size() + c.long_targets().size();
        }
        start = Clock::now();
        const std::size_t trimmed = work.maintain();
        const double pass_seconds = seconds_since(start);
        passes.push_back(pass_seconds);
        std::printf("  pass %d: %zu entries before, %zu trimmed, %.3f ms = %.1f ns per entry\n", pass, before, trimmed,
                    pass_seconds * 1e3, pass_seconds * 1e9 / static_cast<double>(before));
        if (trimmed == 0 || work.maintain() != 0) {
            throw SectionAborted{"maintain trimmed nothing, or a second pass trimmed again"};
        }
    }
    std::sort(passes.begin(), passes.end());
    std::printf("  median %.3f ms per pass (min %.3f, max %.3f)\n", passes[passes.size() / 2] * 1e3,
                passes.front() * 1e3, passes.back() * 1e3);
}

// A world for kills: 1,500 characters with full long-term people lists (40), 16 target
// entries, 16 modifiers and about 22 relation edges each (4 vassals and 4 lieges, 4
// employees and 4 employers, 2 friends, 2 spouses, a child and a parent).
void build_kill_world(PersonalWorld& world, std::mt19937& rng) {
    constexpr std::uint32_t N = 1500;
    build_personal_world(world, rng, 100);
    CharacterRegistry& r = world.registry;
    const auto other = [](std::uint32_t a, std::uint32_t k) { return CharacterId{(a - 1 + k) % N + 1}; };
    Outcomes longs("kill world add_long_opinion");
    Outcomes modifiers("kill world add_modifier");
    Outcomes links("kill world link");
    Outcomes one_way("kill world set_relation");
    for (std::uint32_t a = 1; a <= N; ++a) {
        const CharacterId id{a};
        for (std::uint32_t k = 1; k <= PERSON_LIMIT_MAX; ++k) {
            longs.require(r.add_long_opinion(id, other(a, k), random_nonzero(rng, 200)).outcome,
                          {LongOpinionOutcome::Created});
        }
        for (std::uint32_t t = 1; t <= TARGET_LIMIT; ++t) {
            longs.require(r.add_long_opinion(id, *TargetId::from(TopicId{t}), random_nonzero(rng, 200)).outcome,
                          {LongOpinionOutcome::Created});
        }
        for (std::uint32_t k = 1; k <= 8; ++k) {
            for (std::uint16_t m = 1; m <= 2; ++m) {
                modifiers.require_ok(r.add_modifier(id, other(a, k), ModifierId{m}, random_nonzero(rng, 100)));
            }
        }
        for (std::uint32_t k = 1; k <= 4; ++k) {
            links.require_ok(r.link(id, RelationType::Vassal, other(a, k)));
            links.require_ok(r.link(id, RelationType::Employee, other(a, 4 + k)));
        }
        for (std::uint32_t k = 9; k <= 10; ++k) {
            one_way.require_ok(r.set_relation(id, RelationType::Friend, other(a, k)));
        }
        links.require_ok(r.link(id, RelationType::Spouse, other(a, 13)));
        links.require_ok(r.link(id, RelationType::Child, other(a, 14)));
    }
    print_outcomes({&longs, &modifiers, &links, &one_way});
}

// Kills every third character of the 1,500 in the kill world (500 kills), timing each pass.
void time_kills(const char* name, PersonalWorld& world, const OpinionConfig& config, WorldSeed seed, Date death) {
    const LifecycleConfig lifecycle{};
    std::size_t edges = 0;
    for (std::uint32_t id = 3; id <= 1500; id += 3) {
        edges += world.registry.relations(CharacterId{id}).size();
    }
    Outcomes kills("kill");
    std::size_t killed = 0;
    std::size_t legendary = 0;
    const auto start = Clock::now();
    for (std::uint32_t id = 3; id <= 1500; id += 3) {
        const KillResult result =
            world.registry.kill(CharacterId{id}, death, WorldContext{world.stances, config, lifecycle, seed});
        kills.require_ok(result.result);
        ++killed;
        legendary += result.legendary ? 1u : 0u;
    }
    const double elapsed = seconds_since(start);
    std::printf("  %s: %zu kills in %.3f s = %.1f us per kill (%.1f edges per victim, %zu living, %zu dead after)\n",
                name, killed, elapsed, elapsed * 1e6 / static_cast<double>(killed),
                static_cast<double>(edges) / static_cast<double>(killed), world.registry.size(),
                world.registry.dead_count());
    std::printf("  outcomes: %s (legendary %zu)\n", kills.summary().c_str(), legendary);
}

void bench_death() {
    const OpinionConfig config{};
    const LifecycleConfig lifecycle{};
    const WorldSeed seed{0xDEADu};
    std::printf("kill (1,500 living with full opinion lists and ~22 edges each; every third one killed):\n");
    {
        PersonalWorld world;
        std::mt19937 rng(31u);
        build_kill_world(world, rng);
        time_kills("no dead yet   ", world, config, seed, Date{100});
    }
    {
        // 450 rounds: create 1,000 characters (spouse pairs), then kill them from the back,
        // so at most 2,500 are alive and batch kills never shift the base world.
        PersonalWorld world;
        std::mt19937 rng(31u);
        build_kill_world(world, rng);
        Outcomes setup_creates("setup create");
        Outcomes setup_links("setup link");
        Outcomes setup_kills("setup kill");
        const auto setup = Clock::now();
        auto chunk = Clock::now();
        std::printf("  setup chunks, us per kill for each 50,000 rounds' kills:");
        for (int round = 0; round < 450; ++round) {
            // Ids come from create: forgotten ids leave no trace in size() or dead_count().
            const std::uint32_t first =
                setup_creates.require_created(world.registry.create(NameId{1}, Gender::Female, Date{0}, CharacterInit{}))
                    .value;
            for (std::uint32_t i = 1; i < 1000; ++i) {
                (void)setup_creates.require_created(
                    world.registry.create(NameId{1}, Gender::Female, Date{0}, CharacterInit{}));
            }
            for (std::uint32_t i = 0; i < 1000; i += 2) {
                setup_links.require_ok(
                    world.registry.link(CharacterId{first + i}, RelationType::Spouse, CharacterId{first + i + 1}));
            }
            for (std::uint32_t i = 1000; i-- > 0;) {
                setup_kills.require_ok(
                    world.registry
                        .kill(CharacterId{first + i}, Date{50}, WorldContext{world.stances, config, lifecycle, seed})
                        .result);
            }
            if ((round + 1) % 50 == 0) {
                std::printf(" %.1f", seconds_since(chunk) * 1e6 / 50'000.0);
                std::fflush(stdout);
                chunk = Clock::now();
            }
        }
        std::printf("\n");
        const double setup_seconds = seconds_since(setup);
        std::printf("  (setup: 450,000 create+kill in %.1f s = %.1f us per kill with 1,500 full characters alive)\n",
                    setup_seconds, setup_seconds * 1e6 / 450'000.0);
        print_outcomes({&setup_creates, &setup_links, &setup_kills});
        time_kills("450,000 dead  ", world, config, seed, Date{100});
        std::printf("  memory: dead records %.1f MB (%zu B each), slots+holders %.1f MB, relation graph %.1f MB "
                    "(%zu ids, %zu B per node outer vector), living %.1f MB\n",
                    static_cast<double>(world.registry.dead_record_bytes()) / 1048576.0, sizeof(DeadRecord),
                    static_cast<double>(world.registry.slot_bytes()) / 1048576.0,
                    static_cast<double>(world.registry.relation_bytes()) / 1048576.0,
                    std::size_t{1500 + 450 * 1000}, sizeof(std::vector<RelationEdge>),
                    static_cast<double>(world.registry.allocated_bytes() - world.registry.dead_record_bytes()
                                        - world.registry.slot_bytes() - world.registry.relation_bytes())
                        / 1048576.0);
    }
    {
        // Relocation alone: 15,000 living characters with empty lists; 500 kills spread over
        // the id range (a kill near the front moves almost all of them).
        constexpr std::uint32_t LIVING = 15'000;
        CharacterRegistry registry;
        const StanceTable stances;
        Outcomes creates("relocation create");
        Outcomes kills("relocation kill");
        for (std::uint32_t i = 0; i < LIVING; ++i) {
            (void)creates.require_created(registry.create(NameId{i + 1}, Gender::Female, Date{0}, CharacterInit{}));
        }
        std::size_t moved = 0;
        std::size_t killed = 0;
        double elapsed = 0.0;
        for (std::uint32_t id = 1; id <= LIVING; id += LIVING / 500) {
            const std::span<const Character> living = registry.characters();
            moved += static_cast<std::size_t>(
                living.end() - std::find_if(living.begin(), living.end(), [&](const Character& c) {
                                   return c.id().value > id;
                               }));
            const auto start = Clock::now();
            const EditResult result = registry.kill(CharacterId{id}, Date{1}, WorldContext{stances, config, lifecycle, seed}).result;
            elapsed += seconds_since(start);
            kills.require_ok(result);
            ++killed;
        }
        std::printf("kill relocation: %u living, %zu kills in %.3f s = %.1f us per kill (%.0f characters, %.2f MB moved "
                    "per kill on average)\n",
                    LIVING, killed, elapsed, elapsed * 1e6 / static_cast<double>(killed),
                    static_cast<double>(moved) / static_cast<double>(killed),
                    static_cast<double>(moved * sizeof(Character)) / static_cast<double>(killed) / 1048576.0);
        print_outcomes({&creates, &kills});
    }
}

// Generational churn: 1,500 living characters; each step a newborn gets two distinct living
// parents and the oldest living character dies, for 450,000 deaths. A dead parent is
// remembered only while one of its children lives, so the dead count should stay bounded;
// the per-id arrays (slots, holders, graph node headers) grow with every id.
void bench_churn() {
    constexpr std::uint32_t LIVING = 1500;
    constexpr std::uint32_t DEATHS = 450'000;
    constexpr std::uint32_t SAMPLE = 50'000;
    const OpinionConfig config{};
    const LifecycleConfig lifecycle{};
    const StanceTable stances;
    const WorldContext context{stances, config, lifecycle, WorldSeed{0xC4u}};
    CharacterRegistry registry;
    std::mt19937 rng(8080u);
    Outcomes creates("create");
    Outcomes links("link");
    Outcomes kills("kill");
    for (std::uint32_t i = 0; i < LIVING; ++i) {
        (void)creates.require_created(registry.create(NameId{1}, Gender::Female, Date{0}, CharacterInit{}));
    }
    const auto mib = [](std::size_t bytes) { return static_cast<double>(bytes) / 1048576.0; };
    std::printf("generational churn: %u living, newborn with two distinct living parents, oldest dies:\n", LIVING);
    std::size_t max_dead = 0;
    std::size_t forgotten_at_kill = 0;
    auto interval = Clock::now();
    for (std::uint32_t death = 1; death <= DEATHS; ++death) {
        const std::span<const Character> living = registry.characters();
        const auto first = static_cast<std::uint32_t>(pick(rng, static_cast<std::uint32_t>(living.size())));
        auto second = static_cast<std::uint32_t>(pick(rng, static_cast<std::uint32_t>(living.size() - 1)));
        second += second >= first ? 1u : 0u;
        const CharacterId mother = living[first].id();
        const CharacterId father = living[second].id();
        const CharacterId oldest = living[0].id();
        const CharacterId child = creates.require_created(
            registry.create(NameId{1}, Gender::Female, Date{static_cast<std::int32_t>(death)}, CharacterInit{}));
        links.require_ok(registry.link(mother, RelationType::Child, child));
        links.require_ok(registry.link(father, RelationType::Child, child));
        const std::size_t dead_before = registry.dead_count();
        kills.require_ok(registry.kill(oldest, Date{static_cast<std::int32_t>(death)}, context).result);
        forgotten_at_kill += dead_before + 1 - registry.dead_count();
        max_dead = std::max(max_dead, registry.dead_count());
        if (death % SAMPLE == 0) {
            const double seconds = seconds_since(interval);
            std::printf("  %6u deaths: dead %4zu (max so far %4zu), dead records %.3f MB, slots+holders %.1f MB, "
                        "graph %.1f MB, %.1f us per step\n",
                        death, registry.dead_count(), max_dead, mib(registry.dead_record_bytes()),
                        mib(registry.slot_bytes()), mib(registry.relation_bytes()), seconds * 1e6 / SAMPLE);
            interval = Clock::now();
        }
    }
    const std::size_t ids = LIVING + DEATHS;
    const std::size_t per_id = 2 * sizeof(std::uint32_t) + sizeof(std::vector<RelationEdge>);
    std::printf("  max dead count %zu over %u deaths (%zu forgotten); %zu ids created\n", max_dead, DEATHS,
                forgotten_at_kill, ids);
    print_outcomes({&creates, &links, &kills});
    std::printf("  per-id arrays: %zu B per id (slot %zu, holders %zu, graph node header %zu); %.1f MB at %zu ids, "
                "projected %.1f MB at 4,500,000 ids (15,000 living over the timeline), of which %.1f MB graph node "
                "headers\n",
                per_id, sizeof(std::uint32_t), sizeof(std::uint32_t), sizeof(std::vector<RelationEdge>),
                mib(per_id * ids), ids, mib(per_id * 4'500'000), mib(sizeof(std::vector<RelationEdge>) * 4'500'000));
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
    Outcomes parents("set_parent");
    Outcomes stance_edits("set_stance");
    Outcomes creates("create");
    Outcomes involvement_edits("set_involvement");
    for (std::uint32_t i = 0; i < LEVEL2; ++i) {
        parents.require_ok(stances.set_parent(level2_id(i), root_id(i / FANOUT_2)));
    }
    for (std::uint32_t i = 0; i < LEVEL3; ++i) {
        parents.require_ok(stances.set_parent(level3_id(i), level2_id(i / FANOUT_3)));
    }
    for (std::uint32_t i = 0; i < LEAVES; ++i) {
        parents.require_ok(stances.set_parent(leaf_id(i), level3_id(i / LEAVES_PER_3)));
    }
    for (std::uint32_t r = 0; r < ROOTS; ++r) {
        for (std::uint32_t other = 0; other < ROOTS; ++other) {
            const int v = other == r ? 40 + static_cast<int>(pick(rng, 61)) : value();
            stance_edits.require_ok(stances.set_stance(root_id(r), *TargetId::from(root_id(other)), v));
        }
        for (std::uint32_t t = 0; t < 20; ++t) {
            stance_edits.require_ok(
                stances.set_stance(root_id(r), *TargetId::from(TopicId{1 + pick(rng, TOPICS)}), value()));
        }
    }
    for (std::uint32_t i = 0; i < LEVEL2; ++i) {
        for (std::uint32_t k = 0; k < 10; ++k) {
            const std::uint32_t target = pick(rng, ROOTS + LEVEL2);
            const CommunityId to = target < ROOTS ? root_id(target) : level2_id(target - ROOTS);
            stance_edits.require_ok(stances.set_stance(level2_id(i), *TargetId::from(to), value()));
        }
    }
    for (std::uint32_t i = 0; i < LEVEL3; ++i) {
        if (pick(rng, 5) != 0) {
            continue;
        }
        for (std::uint32_t k = 0; k < 3; ++k) {
            const std::uint32_t target = pick(rng, ROOTS + LEVEL2);
            const CommunityId to = target < ROOTS ? root_id(target) : level2_id(target - ROOTS);
            stance_edits.require_ok(stances.set_stance(level3_id(i), *TargetId::from(to), value()));
        }
    }

    constexpr std::uint32_t CHARACTERS = 1000;
    CharacterRegistry registry;
    for (std::uint32_t i = 0; i < CHARACTERS; ++i) {
        const CharacterInit init{.stability = value(), .openness = value(), .extraversion = value(),
                                 .conscientiousness = value(), .agreeableness = value(), .reputation = value()};
        (void)creates.require_created(registry.create(NameId{i + 1}, Gender::Female, Date{0}, init));
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
            involvement_edits.require_ok(c.set_involvement(leaf_id(leaf), 1 + pick(rng, 255)));
            if (std::find(members.begin(), members.end(), i) == members.end()) {
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
    print_outcomes({&parents, &stance_edits, &creates, &involvement_edits});
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
    int aborted = 0;
    const auto run = [&](const char* name, void (*section)()) {
        try {
            section();
        } catch (const SectionAborted& e) {
            std::printf("\nSECTION ABORTED (%s): %s\n", name, e.message.c_str()); // may interrupt a line
            ++aborted;
        }
    };
    run("weak opinions, typical", [] { bench_weak_opinions(Scenario{"typical", 300, 3, 3, 20'000}); });
    run("weak opinions, worst", [] { bench_weak_opinions(Scenario{"worst", 300, 6, 8, 20'000}); });
    run("structured world", bench_structured_world);
    run("stance inserts", bench_stance_inserts);
    run("personal opinions", bench_personal_opinions);
    run("death", bench_death);
    run("generational churn", bench_churn);
    if (aborted != 0) {
        std::printf("%d section(s) aborted; their timings are missing or incomplete\n", aborted);
        return 1;
    }
    return 0;
}
