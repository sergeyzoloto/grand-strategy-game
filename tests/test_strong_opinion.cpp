#include <doctest.h>

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <map>
#include <random>
#include <type_traits>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/opinion.hpp"
#include "sim/strong_opinion.hpp"

using namespace sim;

namespace {

TargetId topic(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

TargetId community(std::uint32_t id) {
    return *TargetId::from(CommunityId{id});
}

std::vector<StrongOpinion> copy(std::span<const StrongOpinion> list) {
    return {list.begin(), list.end()};
}

bool same_records(std::span<const StrongOpinion> a, std::span<const StrongOpinion> b) {
    return a.size() == b.size()
        && std::equal(a.begin(), a.end(), b.begin(), [](const StrongOpinion& x, const StrongOpinion& y) {
               return x.target == y.target && x.short_dev == y.short_dev && x.long_dev == y.long_dev
                   && x.t0 == y.t0 && x.cause == y.cause && x.reserved == y.reserved;
           });
}

const StrongOpinion* record(std::span<const StrongOpinion> list, std::uint32_t target) {
    for (const StrongOpinion& r : list) {
        if (r.target == target) {
            return &r;
        }
    }
    return nullptr;
}

// A registry of `count` characters with the given init.
CharacterRegistry make_registry(std::uint32_t count, const CharacterInit& init = {}) {
    CharacterRegistry registry;
    for (std::uint32_t i = 1; i <= count; ++i) {
        (void)registry.create(NameId{i}, Gender::Female, Date{0}, init);
    }
    return registry;
}

template<class T>
concept CanApplyPersonEvent = requires(CharacterRegistry& r, T delta) {
    r.apply_opinion_event(CharacterId{1}, CharacterId{2}, delta, Date{0}, CauseId{}, OpinionConfig{});
};
template<class T>
concept CanApplyTargetEvent = requires(CharacterRegistry& r, T delta) {
    r.apply_opinion_event(CharacterId{1}, TargetId{}, delta, Date{0}, CauseId{}, OpinionConfig{});
};

const OpinionConfig CONFIG{};

} // namespace

// ---- layout --------------------------------------------------------------------------------

static_assert(sizeof(StrongOpinion) == 16);
static_assert(std::has_unique_object_representations_v<StrongOpinion>);
static_assert(sizeof(Character) == 1352); // offsets are pinned by static_asserts in the constructor
static_assert(std::is_trivially_copyable_v<Character>);
static_assert(sizeof(CauseId) == 2 && !CauseId{}.valid());
static_assert(CanApplyPersonEvent<int> && CanApplyPersonEvent<std::int64_t> && CanApplyTargetEvent<unsigned>);
static_assert(!CanApplyPersonEvent<double> && !CanApplyTargetEvent<float> && !CanApplyPersonEvent<bool>);

TEST_CASE("strong opinions: sizes") {
    MESSAGE("sizeof(StrongOpinion) = " << sizeof(StrongOpinion) << ", sizeof(Character) = " << sizeof(Character));
    CHECK(sizeof(Character) == 1352);
}

// ---- retention power ----------------------------------------------------------------------

TEST_CASE("retention power: 1 at 0 weeks, matches naive multiplication, cutoff, golden values") {
    for (const double r : {0.5, 0.95, 0.965, 0.98, 0.999, 1.0}) {
        CAPTURE(r);
        CHECK(retention_power(r, 0) == 1.0);
        CHECK(retention_power(r, -5) == 1.0);
        double naive = 1.0;
        for (int n = 1; n <= 10; ++n) {
            naive *= r;
            CHECK(std::abs(retention_power(r, n) - naive) <= 1e-15 * naive);
        }
    }
    CHECK(retention_power(1.0, 1'000'000'000) == 1.0);
    // 2^-39 ~ 1.8e-12 is kept; 2^-40 ~ 9.1e-13 is below the cutoff.
    CHECK(retention_power(0.5, 39) == 0x1p-39);
    CHECK(retention_power(0.5, 40) == 0.0);
    CHECK(retention_power(0.5, 1'000'000) == 0.0);

    CHECK(retention_power(0.965, 13) == 0x1.42331acf5dacbp-1);
    CHECK(retention_power(0.95, 100) == 0x1.8401ff605846ep-8);
    CHECK(retention_power(0.98, 1000) == 0x1.ce9c4ca6be05p-30);
    CHECK(retention_power(0.98, 1367) == 0x1.1d6baa3bf72fcp-40); // just above 1e-12
    CHECK(retention_power(0.98, 1368) == 0.0);                   // just below
}

// ---- traits ------------------------------------------------------------------------------------

TEST_CASE("amplitude and retention follow stability; person limit follows extraversion") {
    CharacterRegistry r;
    const CharacterId unstable = r.create(NameId{1}, Gender::Male, Date{0}, CharacterInit{.stability = -100});
    const CharacterId stable = r.create(NameId{2}, Gender::Male, Date{0}, CharacterInit{.stability = 100});
    const CharacterId shy = r.create(NameId{3}, Gender::Male, Date{0}, CharacterInit{.extraversion = -100});
    const CharacterId average = r.create(NameId{4}, Gender::Male, Date{0}, CharacterInit{.extraversion = 0});
    const CharacterId outgoing = r.create(NameId{5}, Gender::Male, Date{0}, CharacterInit{.extraversion = 100});
    CHECK(amplitude(*r.find(unstable), CONFIG) == 1.5);
    CHECK(amplitude(*r.find(stable), CONFIG) == 0.5);
    CHECK(retention(*r.find(unstable), CONFIG) == 0.98);
    CHECK(retention(*r.find(stable), CONFIG) == 0.95);
    CHECK(person_limit(*r.find(shy)) == 8);
    CHECK(person_limit(*r.find(average)) == 24);
    CHECK(person_limit(*r.find(outgoing)) == 40);
}

// ---- events ---------------------------------------------------------------------------------------

TEST_CASE("events: dropped below the threshold, created with the right short and long") {
    CharacterRegistry r = make_registry(3); // stability 0: amplitude 1
    const CharacterId a{1}, b{2}, c{3};
    CHECK(r.apply_opinion_event(a, b, 9, Date{5}, CauseId{1}, CONFIG).outcome == OpinionEventOutcome::Dropped);
    CHECK(r.find(a)->strong_people().empty());

    const auto created = r.apply_opinion_event(a, b, 50, Date{5}, CauseId{7}, CONFIG);
    CHECK(created.outcome == OpinionEventOutcome::Created);
    CHECK(!created.evicted.valid());
    const StrongOpinion* rec = record(r.find(a)->strong_people(), b.value);
    REQUIRE(rec != nullptr);
    CHECK(rec->short_dev == 5000);
    CHECK(rec->long_dev == 500); // plasticity 0.1 * 5000
    CHECK(rec->t0 == Date{5});
    CHECK(rec->cause == CauseId{7});
    CHECK(rec->reserved == 0);

    CHECK(r.apply_opinion_event(a, c, -10, Date{5}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    CHECK(record(r.find(a)->strong_people(), c.value)->short_dev == -1000);
    CHECK(r.find(a)->strong_people().size() == 2);
    CHECK(r.find(b)->strong_people().empty()); // one-directional
}

TEST_CASE("events: the update formula, t0 and cause") {
    CharacterRegistry r = make_registry(2);
    const CharacterId a{1}, b{2};
    REQUIRE(r.apply_opinion_event(a, b, 50, Date{0}, CauseId{1}, CONFIG).outcome == OpinionEventOutcome::Created);
    const double rho = retention(*r.find(a), CONFIG);

    const auto updated = r.apply_opinion_event(a, b, -30, Date{12}, CauseId{2}, CONFIG);
    CHECK(updated.outcome == OpinionEventOutcome::Updated);
    const double dev = 500.0 + (5000.0 - 500.0) * retention_power(rho, 12);
    const auto s = static_cast<std::int64_t>(std::round(dev - 3000.0));
    const auto long_dev = 500 + static_cast<std::int64_t>(std::round(0.1 * static_cast<double>(s - 500)));
    const StrongOpinion* rec = record(r.find(a)->strong_people(), b.value);
    REQUIRE(rec != nullptr);
    CHECK(rec->short_dev == s);
    CHECK(rec->long_dev == long_dev);
    CHECK(rec->t0 == Date{12});
    CHECK(rec->cause == CauseId{2});

    // A tiny update below the enter threshold still updates an existing record.
    CHECK(r.apply_opinion_event(a, b, 1, Date{12}, CauseId{3}, CONFIG).outcome == OpinionEventOutcome::Updated);
    CHECK(record(r.find(a)->strong_people(), b.value)->short_dev == s + 100);
    CHECK(record(r.find(a)->strong_people(), b.value)->cause == CauseId{3});
}

TEST_CASE("events: the long delta is rounded, not the sum") {
    // long = -3 raw with k * (s - long) = 0.5: rounding the delta gives -2, rounding the sum -3.
    // Build that state: plasticity 0.5, short and long at -3 ... reached by a create with d = -6 raw.
    OpinionConfig config = CONFIG;
    config.plasticity = 0.5;
    config.enter_threshold = 0.01;
    config.exit_threshold = 0.001;
    CharacterRegistry r = make_registry(2, CharacterInit{.stability = 100}); // amplitude 0.5
    const CharacterId a{1}, b{2};
    // delta -1 at amplitude 0.5: d = -0.5 -> s = -50 raw, long = round(0.5 * -50) = -25.
    REQUIRE(r.apply_opinion_event(a, b, -1, Date{0}, CauseId{}, config).outcome == OpinionEventOutcome::Created);
    CHECK(record(r.find(a)->strong_people(), b.value)->long_dev == -25);
    // Same week: dev = short = -50; delta +1 -> s = 0; long step = round(0.5 * (0 - -25)) = round(12.5) = 13.
    REQUIRE(r.apply_opinion_event(a, b, 1, Date{0}, CauseId{}, config).outcome == OpinionEventOutcome::Updated);
    CHECK(record(r.find(a)->strong_people(), b.value)->short_dev == 0);
    CHECK(record(r.find(a)->strong_people(), b.value)->long_dev == -12); // -25 + 13; round(-12.5) would give -13
}

TEST_CASE("events: clamping at +-200 and extreme integer deltas") {
    CharacterRegistry r = make_registry(3, CharacterInit{.stability = -100}); // amplitude 1.5
    const CharacterId a{1}, b{2}, c{3};
    for (int i = 0; i < 5; ++i) {
        REQUIRE(r.apply_opinion_event(a, b, std::numeric_limits<std::int64_t>::max(), Date{0}, CauseId{}, CONFIG).outcome
                != OpinionEventOutcome::Invalid);
    }
    const StrongOpinion* rec = record(r.find(a)->strong_people(), b.value);
    REQUIRE(rec != nullptr);
    CHECK(rec->short_dev == STRONG_DEV_MAX_RAW);
    CHECK(rec->long_dev <= STRONG_DEV_MAX_RAW);
    CHECK(rec->long_dev > 0);

    REQUIRE(r.apply_opinion_event(a, c, std::numeric_limits<std::int64_t>::min(), Date{0}, CauseId{}, CONFIG).outcome
            == OpinionEventOutcome::Created);
    CHECK(record(r.find(a)->strong_people(), c.value)->short_dev == -STRONG_DEV_MAX_RAW); // 1.5 * -200 clamps
    CHECK(strong_deviation(*r.find(a), c, Date{0}, CONFIG) == -200.0);
}

TEST_CASE("events: amplitude follows stability; plasticity for targets follows openness") {
    CharacterRegistry r;
    const CharacterId unstable = r.create(NameId{1}, Gender::Male, Date{0}, CharacterInit{.stability = -100});
    const CharacterId stable = r.create(NameId{2}, Gender::Male, Date{0}, CharacterInit{.stability = 100});
    const CharacterId open = r.create(NameId{3}, Gender::Male, Date{0}, CharacterInit{.openness = 100});
    const CharacterId closed = r.create(NameId{4}, Gender::Male, Date{0}, CharacterInit{.openness = -100});
    REQUIRE(r.apply_opinion_event(unstable, open, 20, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    REQUIRE(r.apply_opinion_event(stable, open, 20, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    CHECK(record(r.find(unstable)->strong_people(), open.value)->short_dev == 3000);
    CHECK(record(r.find(stable)->strong_people(), open.value)->short_dev == 1000);

    REQUIRE(r.apply_opinion_event(open, topic(1), 50, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    REQUIRE(r.apply_opinion_event(closed, topic(1), 50, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    CHECK(record(r.find(open)->strong_targets(), topic(1).raw())->long_dev == 750);   // 0.1 * 1.5 * 5000
    CHECK(record(r.find(closed)->strong_targets(), topic(1).raw())->long_dev == 250); // 0.1 * 0.5 * 5000
    // People use plain plasticity regardless of openness.
    REQUIRE(r.apply_opinion_event(open, closed, 50, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    REQUIRE(r.apply_opinion_event(closed, open, 50, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    CHECK(record(r.find(open)->strong_people(), closed.value)->long_dev == 500);
    CHECK(record(r.find(closed)->strong_people(), open.value)->long_dev == 500);
}

TEST_CASE("events: Invalid and NotFound") {
    CharacterRegistry r = make_registry(2);
    const CharacterId a{1}, b{2};
    using O = OpinionEventOutcome;
    CHECK(r.apply_opinion_event(CharacterId{}, b, 50, Date{0}, CauseId{}, CONFIG).outcome == O::Invalid);
    CHECK(r.apply_opinion_event(a, CharacterId{}, 50, Date{0}, CauseId{}, CONFIG).outcome == O::Invalid);
    CHECK(r.apply_opinion_event(a, a, 50, Date{0}, CauseId{}, CONFIG).outcome == O::Invalid);
    CHECK(r.apply_opinion_event(a, TargetId{}, 50, Date{0}, CauseId{}, CONFIG).outcome == O::Invalid);
    CHECK(r.apply_opinion_event(CharacterId{}, topic(1), 50, Date{0}, CauseId{}, CONFIG).outcome == O::Invalid);
    CHECK(r.apply_opinion_event(a, CharacterId{3}, 50, Date{0}, CauseId{}, CONFIG).outcome == O::NotFound);
    CHECK(r.apply_opinion_event(CharacterId{3}, a, 50, Date{0}, CauseId{}, CONFIG).outcome == O::NotFound);
    CHECK(r.apply_opinion_event(CharacterId{3}, topic(1), 50, Date{0}, CauseId{}, CONFIG).outcome == O::NotFound);
    // Invalid is reported before NotFound.
    CHECK(r.apply_opinion_event(CharacterId{3}, CharacterId{3}, 50, Date{0}, CauseId{}, CONFIG).outcome == O::Invalid);
    CHECK(r.find(a)->strong_people().empty());
    CHECK(r.find(a)->strong_targets().empty());
}

// ---- decay ----------------------------------------------------------------------------------------

TEST_CASE("decay: dev at t0 equals short, moves monotonically towards long, reads never write") {
    CharacterRegistry r = make_registry(2);
    const CharacterId a{1}, b{2};
    REQUIRE(r.apply_opinion_event(a, b, -80, Date{100}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    const std::vector<StrongOpinion> before = copy(r.find(a)->strong_people());
    CHECK(strong_deviation(*r.find(a), b, Date{100}, CONFIG) == -80.0);

    const double long_value = static_cast<double>(before[0].long_dev) / 100.0;
    double previous_gap = std::abs(-80.0 - long_value);
    for (int week = 101; week <= 600; ++week) {
        const double dev = strong_deviation(*r.find(a), b, Date{week}, CONFIG);
        const double gap = std::abs(dev - long_value);
        if (!(gap <= previous_gap) || !(dev <= long_value)) {
            FAIL_CHECK("not monotonic towards long at week " << week);
        }
        previous_gap = gap;
    }
    CHECK(strong_deviation(*r.find(a), b, Date{5000}, CONFIG) == long_value); // factor cut off to 0
    (void)opinion(*r.find(a), *r.find(b), StanceTable{}, CONFIG, WorldSeed{1}, Date{300});
    CHECK(same_records(r.find(a)->strong_people(), before));
    CHECK(strong_deviation(*r.find(a), CharacterId{2}, Date{100}, CONFIG) == -80.0);
    CHECK(strong_deviation(*r.find(b), a, Date{100}, CONFIG) == 0.0); // no record
}

#ifdef NDEBUG
// now < t0 is asserted in debug builds.
TEST_CASE("decay: now before t0 counts as 0 weeks") {
    CharacterRegistry r = make_registry(2);
    REQUIRE(r.apply_opinion_event(CharacterId{1}, CharacterId{2}, 40, Date{50}, CauseId{}, CONFIG).outcome
            == OpinionEventOutcome::Created);
    CHECK(strong_deviation(*r.find(CharacterId{1}), CharacterId{2}, Date{10}, CONFIG) == 40.0);
}
#endif

TEST_CASE("decay: retention uses the current stability over the whole elapsed time") {
    CharacterRegistry r = make_registry(2, CharacterInit{.stability = -100});
    const CharacterId a{1}, b{2};
    REQUIRE(r.apply_opinion_event(a, b, 60, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    const StrongOpinion rec = r.find(a)->strong_people()[0];
    const auto expected = [&](double retention_factor) {
        return (rec.long_dev + (rec.short_dev - rec.long_dev) * retention_power(retention_factor, 20)) / 100.0;
    };
    CHECK(strong_deviation(*r.find(a), b, Date{20}, CONFIG) == expected(0.98));
    r.find(a)->set_stability(100); // accepted model limitation: re-times the whole elapsed decay
    CHECK(strong_deviation(*r.find(a), b, Date{20}, CONFIG) == expected(0.95));
    CHECK(same_records(r.find(a)->strong_people(), std::span(&rec, 1)));
}

// ---- limits and eviction ------------------------------------------------------------------------

TEST_CASE("limits: the weakest record is evicted with the tie-break; equal strength doesn't evict") {
    CharacterRegistry r = make_registry(12, CharacterInit{.extraversion = -100}); // limit 8, amplitude 1
    const CharacterId a{1};
    // Persons 2..9: |dev| 30, 20(+), 40, 20(-), 50, 60, 70, 80.
    const std::array<int, 8> deltas{30, 20, 40, -20, 50, 60, 70, 80};
    for (std::uint32_t i = 0; i < deltas.size(); ++i) {
        REQUIRE(r.apply_opinion_event(a, CharacterId{i + 2}, deltas[i], Date{0}, CauseId{}, CONFIG).outcome
                == OpinionEventOutcome::Created);
    }
    CHECK(r.find(a)->strong_people().size() == 8);

    // Equal strength (|d| = 20 == weakest |dev|): dropped.
    CHECK(r.apply_opinion_event(a, CharacterId{10}, -20, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Dropped);
    CHECK(r.find(a)->strong_people().size() == 8);

    // Stronger: evicts the weakest; ties (person 3 at +20, person 5 at -20) go to the smaller id.
    const auto first = r.apply_opinion_event(a, CharacterId{10}, 21, Date{0}, CauseId{}, CONFIG);
    CHECK(first.outcome == OpinionEventOutcome::CreatedWithEviction);
    CHECK(first.evicted == CharacterId{3});
    const auto second = r.apply_opinion_event(a, CharacterId{11}, 25, Date{0}, CauseId{}, CONFIG);
    CHECK(second.evicted == CharacterId{5});
    CHECK(r.find(a)->strong_people().size() == 8);
    CHECK(record(r.find(a)->strong_people(), 3) == nullptr);
    CHECK(record(r.find(a)->strong_people(), 10) != nullptr);
    CHECK(std::is_sorted(r.find(a)->strong_people().begin(), r.find(a)->strong_people().end(),
                         [](const StrongOpinion& x, const StrongOpinion& y) { return x.target < y.target; }));

    // Updating an existing record at the limit is not an eviction.
    CHECK(r.apply_opinion_event(a, CharacterId{2}, 5, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Updated);
}

TEST_CASE("limits: TARGET_LIMIT holds and evicted targets are typed") {
    CharacterRegistry r = make_registry(1);
    const CharacterId a{1};
    for (std::uint32_t i = 1; i <= TARGET_LIMIT; ++i) {
        REQUIRE(r.apply_opinion_event(a, topic(i), 20 + static_cast<int>(i), Date{0}, CauseId{}, CONFIG).outcome
                == OpinionEventOutcome::Created);
    }
    CHECK(r.apply_opinion_event(a, community(3), 21, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Dropped);
    const auto result = r.apply_opinion_event(a, community(3), 90, Date{0}, CauseId{}, CONFIG);
    CHECK(result.outcome == OpinionEventOutcome::CreatedWithEviction);
    CHECK(result.evicted == topic(1)); // |dev| 21 is the weakest
    CHECK(result.evicted.kind() == TargetKind::Topic);
    CHECK(r.find(a)->strong_targets().size() == TARGET_LIMIT);
    CHECK(record(r.find(a)->strong_targets(), community(3).raw()) != nullptr);
}

TEST_CASE("limits: an event into a list above its limit evicts only one record") {
    CharacterRegistry r = make_registry(12, CharacterInit{.extraversion = 100}); // limit 40
    const CharacterId a{1};
    for (std::uint32_t i = 2; i <= 11; ++i) {
        REQUIRE(r.apply_opinion_event(a, CharacterId{i}, 20 + static_cast<int>(i), Date{0}, CauseId{}, CONFIG).outcome
                == OpinionEventOutcome::Created);
    }
    r.find(a)->set_extraversion(-100); // limit 8, list holds 10
    CHECK(r.apply_opinion_event(a, CharacterId{12}, 99, Date{0}, CauseId{}, CONFIG).outcome
          == OpinionEventOutcome::CreatedWithEviction);
    CHECK(r.find(a)->strong_people().size() == 10); // still above the limit until maintain
}

// ---- maintain ----------------------------------------------------------------------------------------

TEST_CASE("maintain: hysteresis, decayed removal, trimming, idempotence and counts") {
    CharacterRegistry r = make_registry(25, CharacterInit{.extraversion = 100});
    const CharacterId a{1}, b{2};
    // delta 10 at amplitude 1: short 1000, long 100 raw; dev = 1 + 9 * 0.965^w in value units.
    REQUIRE(r.apply_opinion_event(a, b, 10, Date{0}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    const double at_10 = strong_deviation(*r.find(a), b, Date{10}, CONFIG);
    CHECK(at_10 > CONFIG.exit_threshold);
    CHECK(at_10 < CONFIG.enter_threshold);
    StrongMaintainCounts counts = r.maintain(Date{10}, CONFIG);
    CHECK(counts.removed_decayed == 0); // between the thresholds: survives
    CHECK(r.find(a)->strong_people().size() == 1);
    CHECK(strong_deviation(*r.find(a), b, Date{40}, CONFIG) < CONFIG.exit_threshold);
    counts = r.maintain(Date{40}, CONFIG);
    CHECK(counts.removed_decayed == 1);
    CHECK(r.find(a)->strong_people().empty());

    // Trimming after extraversion drops: 20 records, limit 40 -> 8.
    for (std::uint32_t i = 2; i <= 21; ++i) {
        REQUIRE(r.apply_opinion_event(a, CharacterId{i}, 20 + static_cast<int>(i), Date{40}, CauseId{}, CONFIG).outcome
                == OpinionEventOutcome::Created);
    }
    REQUIRE(r.apply_opinion_event(b, topic(1), 50, Date{40}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    r.find(a)->set_extraversion(-100);
    counts = r.maintain(Date{40}, CONFIG);
    CHECK(counts.removed_decayed == 0);
    CHECK(counts.evicted_over_limit == 12);
    const auto people = r.find(a)->strong_people();
    REQUIRE(people.size() == 8);
    for (const StrongOpinion& rec : people) {
        CHECK(rec.target >= 14); // the strongest eight (persons 14..21) remain
    }
    CHECK(r.find(b)->strong_targets().size() == 1);

    // Idempotent for the same now.
    const std::vector<StrongOpinion> after = copy(r.find(a)->strong_people());
    counts = r.maintain(Date{40}, CONFIG);
    CHECK(counts.removed_decayed == 0);
    CHECK(counts.evicted_over_limit == 0);
    CHECK(same_records(r.find(a)->strong_people(), after));
}

// ---- opinion reads ----------------------------------------------------------------------------------

TEST_CASE("opinion: equals weak without a record, clamp(weak + dev) with one; breakdown adds up") {
    StanceTable stances;
    REQUIRE(stances.set_stance(CommunityId{1}, community(1), 70) == EditResult::Ok);
    CharacterRegistry r = make_registry(3, CharacterInit{.reputation = 50});
    const CharacterId a{1}, b{2}, c{3};
    REQUIRE(r.find(a)->set_involvement(CommunityId{1}, 5) == EditResult::Ok);
    REQUIRE(r.find(b)->set_involvement(CommunityId{1}, 5) == EditResult::Ok);
    const WorldSeed seed{77};
    const Date now{30};

    CHECK(opinion(*r.find(a), *r.find(c), stances, CONFIG, seed, now)
          == weak_opinion(*r.find(a), *r.find(c), stances, CONFIG, seed));
    CHECK(opinion(*r.find(a), topic(4), stances, CONFIG, seed, now)
          == weak_opinion(*r.find(a), topic(4), stances, CONFIG, seed));

    REQUIRE(r.apply_opinion_event(a, c, -40, Date{10}, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    const OpinionBreakdown o = opinion_breakdown(*r.find(a), *r.find(c), stances, CONFIG, seed, now);
    CHECK(o.weak.total == weak_opinion(*r.find(a), *r.find(c), stances, CONFIG, seed));
    CHECK(o.deviation == strong_deviation(*r.find(a), c, now, CONFIG));
    CHECK(o.deviation < 0.0);
    CHECK(o.total == std::clamp(o.weak.total + o.deviation, -100.0, 100.0));

    // Clamping: a strong positive deviation on top of a high weak opinion.
    for (int i = 0; i < 3; ++i) {
        REQUIRE(r.apply_opinion_event(a, b, 200, now, CauseId{}, CONFIG).outcome != OpinionEventOutcome::Invalid);
    }
    const OpinionBreakdown high = opinion_breakdown(*r.find(a), *r.find(b), stances, CONFIG, seed, now);
    CHECK(high.weak.total + high.deviation > 100.0);
    CHECK(high.total == 100.0);
    REQUIRE(r.apply_opinion_event(b, a, -200, now, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Created);
    REQUIRE(r.apply_opinion_event(b, a, -200, now, CauseId{}, CONFIG).outcome == OpinionEventOutcome::Updated);
    CHECK(opinion(*r.find(b), *r.find(a), stances, CONFIG, seed, now) == -100.0);
}

// ---- golden values ---------------------------------------------------------------------------------------

TEST_CASE("strong opinions: golden values (identical in Debug and Release)") {
    CharacterRegistry reg;
    const CharacterId a = reg.create(NameId{1}, Gender::Female, Date{0},
                                     CharacterInit{.stability = -40, .openness = 60, .extraversion = 10,
                                                   .conscientiousness = 5, .agreeableness = -15, .reputation = 12});
    const CharacterId b = reg.create(NameId{2}, Gender::Male, Date{0},
                                     CharacterInit{.stability = 70, .openness = -30, .extraversion = -80,
                                                   .conscientiousness = 40, .agreeableness = 25, .reputation = -45});
    const CharacterId c = reg.create(NameId{3}, Gender::Female, Date{0}, CharacterInit{});
    const TargetId topic7 = topic(7);
    using O = OpinionEventOutcome;
    CHECK(reg.apply_opinion_event(a, b, 80, Date{0}, CauseId{1}, CONFIG).outcome == O::Created);
    CHECK(reg.apply_opinion_event(a, b, -30, Date{3}, CauseId{2}, CONFIG).outcome == O::Updated);
    CHECK(reg.apply_opinion_event(a, topic7, 60, Date{10}, CauseId{3}, CONFIG).outcome == O::Created);
    CHECK(reg.apply_opinion_event(a, b, 15, Date{25}, CauseId{4}, CONFIG).outcome == O::Updated);
    CHECK(reg.apply_opinion_event(b, a, -120, Date{40}, CauseId{5}, CONFIG).outcome == O::Created);
    CHECK(reg.apply_opinion_event(b, c, 9, Date{41}, CauseId{6}, CONFIG).outcome == O::Dropped);
    CHECK(reg.apply_opinion_event(a, topic7, 25, Date{52}, CauseId{7}, CONFIG).outcome == O::Updated);
    const StrongMaintainCounts counts = reg.maintain(Date{60}, CONFIG);
    CHECK(counts.removed_decayed == 0);
    CHECK(counts.evicted_over_limit == 0);

    const StrongOpinion* ab = record(reg.find(a)->strong_people(), b.value);
    const StrongOpinion* at = record(reg.find(a)->strong_targets(), topic7.raw());
    const StrongOpinion* ba = record(reg.find(b)->strong_people(), a.value);
    REQUIRE(ab != nullptr);
    REQUIRE(at != nullptr);
    REQUIRE(ba != nullptr);
    CHECK(record(reg.find(b)->strong_people(), c.value) == nullptr);
    CHECK((ab->short_dev == 5221 && ab->long_dev == 1774 && ab->t0 == Date{25} && ab->cause == CauseId{4}));
    CHECK((at->short_dev == 5756 && at->long_dev == 1563 && at->t0 == Date{52} && at->cause == CauseId{7}));
    CHECK((ba->short_dev == -7800 && ba->long_dev == -780 && ba->t0 == Date{40} && ba->cause == CauseId{5}));

    StanceTable stances;
    REQUIRE(stances.set_stance(CommunityId{1}, topic7, 40) == EditResult::Ok);
    REQUIRE(reg.find(a)->set_involvement(CommunityId{1}, 10) == EditResult::Ok);
    const WorldSeed seed{0xABCDu};
    const Date now{61};
    const OpinionBreakdown o_ab = opinion_breakdown(*reg.find(a), *reg.find(b), stances, CONFIG, seed, now);
    CHECK(o_ab.weak.total == 0x1.8eac8ac193ca4p+1);  // 3.114640564460915
    CHECK(o_ab.deviation == 0x1.db065e88d5da9p+4);   // 29.689054998887205
    CHECK(o_ab.total == 0x1.066df7f08429fp+5);       // 32.803695563348121
    const OpinionBreakdown o_ba = opinion_breakdown(*reg.find(b), *reg.find(a), stances, CONFIG, seed, now);
    CHECK(o_ba.deviation == -0x1.119d0ec179576p+5);  // -34.201688300624411
    CHECK(o_ba.total == -0x1.504a8d5fb52c4p+4);      // -21.018201230867604
    const OpinionBreakdown o_at = opinion_breakdown(*reg.find(a), topic7, stances, CONFIG, seed, now);
    CHECK(o_at.weak.total == 0x1.6e590eb3e1b3cp+5);  // 45.793485074353583
    CHECK(o_at.deviation == 0x1.7e6d82c6de121p+5);   // 47.803472093250328
    CHECK(o_at.total == 0x1.766348bd5fe2ep+6);       // 93.596957167603904
    CHECK(opinion(*reg.find(c), *reg.find(a), stances, CONFIG, seed, now) == 0x1.4f25802f1e53p+3); // weak only
}

// ---- property test against a reference model ----------------------------------------------

namespace {

// Naive model of strong opinions: std::map per list, rules written out directly. Reuses
// the tested retention_power; every other formula is restated here.
class StrongModel {
public:
    struct Record {
        std::int64_t short_dev = 0;
        std::int64_t long_dev = 0;
        std::int32_t t0 = 0;
        std::uint16_t cause = 0;
    };
    using List = std::map<std::uint32_t, Record>; // target -> record, in target order

    enum class Kind { Updated, Created, CreatedWithEviction, DroppedBelowThreshold, DroppedAtLimit };
    struct Outcome {
        Kind kind;
        std::uint32_t evicted = 0;
    };

    explicit StrongModel(std::size_t count) : people_(count + 1), targets_(count + 1) {}

    static double linear(int trait, double at_min, double at_max) {
        return at_min + (at_max - at_min) * (static_cast<double>(trait + 100) / 200.0);
    }
    static double dev(const Record& r, double rho, std::int32_t now) {
        const double factor = retention_power(rho, static_cast<std::int64_t>(now) - r.t0);
        return static_cast<double>(r.long_dev) + static_cast<double>(r.short_dev - r.long_dev) * factor;
    }
    static std::int64_t round_clamped(double v, double bound) {
        return static_cast<std::int64_t>(std::round(std::clamp(v, -bound, bound)));
    }

    Outcome event(const Character& a, bool person, std::uint32_t target, std::int64_t delta_in, std::int32_t now,
                  std::uint16_t cause) {
        List& list = person ? people_[a.id().value] : targets_[a.id().value];
        const double rho = linear(a.stability(), CONFIG.retention_unstable, CONFIG.retention_stable);
        const double amp = linear(a.stability(), CONFIG.amplitude_unstable, CONFIG.amplitude_stable);
        const double k = person ? CONFIG.plasticity
                                : CONFIG.plasticity * linear(a.openness(), CONFIG.openness_factor_min,
                                                             CONFIG.openness_factor_max);
        const std::size_t limit = person ? 8 + static_cast<std::size_t>((32 * (a.extraversion() + 100) + 100) / 200)
                                         : TARGET_LIMIT;
        const std::int64_t delta = std::clamp<std::int64_t>(delta_in, -200, 200);
        const double d = amp * static_cast<double>(delta);
        const double d_raw = d * 100.0;

        if (auto it = list.find(target); it != list.end()) {
            Record& r = it->second;
            const std::int64_t s = round_clamped(dev(r, rho, now) + d_raw, 20000);
            r.long_dev = std::clamp<std::int64_t>(r.long_dev + round_clamped(k * static_cast<double>(s - r.long_dev), 40000),
                                                  -20000, 20000);
            r.short_dev = s;
            r.t0 = now;
            r.cause = cause;
            return {Kind::Updated};
        }
        if (std::abs(d) < CONFIG.enter_threshold) {
            return {Kind::DroppedBelowThreshold};
        }
        Record fresh;
        fresh.short_dev = round_clamped(d_raw, 20000);
        fresh.long_dev = std::clamp<std::int64_t>(round_clamped(k * static_cast<double>(fresh.short_dev), 40000), -20000, 20000);
        fresh.t0 = now;
        fresh.cause = cause;
        if (list.size() >= limit) {
            auto weakest = list.begin();
            for (auto it = list.begin(); it != list.end(); ++it) {
                if (std::abs(dev(it->second, rho, now)) < std::abs(dev(weakest->second, rho, now))) {
                    weakest = it;
                }
            }
            if (!(std::abs(d_raw) > std::abs(dev(weakest->second, rho, now)))) {
                return {Kind::DroppedAtLimit};
            }
            const std::uint32_t evicted = weakest->first;
            list.erase(weakest);
            list.emplace(target, fresh);
            return {Kind::CreatedWithEviction, evicted};
        }
        list.emplace(target, fresh);
        return {Kind::Created};
    }

    StrongMaintainCounts maintain(const CharacterRegistry& registry, std::int32_t now) {
        StrongMaintainCounts counts;
        for (std::uint32_t id = 1; id < people_.size(); ++id) {
            const Character& a = *registry.find(CharacterId{id});
            const double rho = linear(a.stability(), CONFIG.retention_unstable, CONFIG.retention_stable);
            for (List* list : {&people_[id], &targets_[id]}) {
                for (auto it = list->begin(); it != list->end();) {
                    if (std::abs(dev(it->second, rho, now)) < CONFIG.exit_threshold * 100.0) {
                        it = list->erase(it);
                        ++counts.removed_decayed;
                    } else {
                        ++it;
                    }
                }
            }
            const std::size_t limit = 8 + static_cast<std::size_t>((32 * (a.extraversion() + 100) + 100) / 200);
            while (people_[id].size() > limit) {
                auto weakest = people_[id].begin();
                for (auto it = people_[id].begin(); it != people_[id].end(); ++it) {
                    if (std::abs(dev(it->second, rho, now)) < std::abs(dev(weakest->second, rho, now))) {
                        weakest = it;
                    }
                }
                people_[id].erase(weakest);
                ++counts.evicted_over_limit;
            }
        }
        return counts;
    }

    static bool matches(std::span<const StrongOpinion> actual, const List& expected) {
        if (actual.size() != expected.size()) {
            return false;
        }
        auto it = expected.begin();
        for (const StrongOpinion& r : actual) {
            if (r.target != it->first || r.short_dev != it->second.short_dev || r.long_dev != it->second.long_dev
                || r.t0.weeks != it->second.t0 || r.cause.value != it->second.cause || r.reserved != 0) {
                return false;
            }
            ++it;
        }
        return true;
    }

    const List& people(std::uint32_t id) const { return people_[id]; }
    const List& targets(std::uint32_t id) const { return targets_[id]; }

private:
    std::vector<List> people_;  // index = character id
    std::vector<List> targets_; // index = character id
};

// 30% weak deltas (magnitude 10..30) so weak events meet full lists; otherwise uniform in +-bound.
std::int64_t random_delta(std::mt19937& rng, std::int64_t bound) {
    if (rng() % 100 < 30) {
        const auto magnitude = static_cast<std::int64_t>(10 + rng() % 21);
        return rng() % 2 == 0 ? magnitude : -magnitude;
    }
    return static_cast<std::int64_t>(rng() % static_cast<std::uint64_t>(2 * bound + 1)) - bound;
}

} // namespace

TEST_CASE("property: random events, trait changes and maintain match a reference model") {
    constexpr std::uint32_t COUNT = 16;
    constexpr std::uint32_t TOPICS = 24; // more than TARGET_LIMIT
    constexpr int STEPS = 6000;
    CharacterRegistry registry;
    for (std::uint32_t i = 1; i <= COUNT; ++i) {
        (void)registry.create(NameId{i}, Gender::Female, Date{0},
                              CharacterInit{.stability = static_cast<int>(i * 13 % 201) - 100,
                                            .openness = static_cast<int>(i * 37 % 201) - 100,
                                            .extraversion = i % 2 == 0 ? 100 : -100});
    }
    StrongModel model(COUNT);
    // Raw mt19937 output only: standard distributions differ between standard libraries.
    std::mt19937 rng(20260920u);
    std::int32_t now = 0;

    std::size_t created_with_eviction = 0, dropped_at_limit = 0, removed_decayed = 0, trimmed = 0, updated = 0;
    for (int step = 0; step < STEPS; ++step) {
        now += static_cast<std::int32_t>(rng() % 4);
        const auto roll = rng() % 100;
        const auto a = static_cast<std::uint32_t>(1 + rng() % COUNT);
        const Character& source = *registry.find(CharacterId{a});

        if (roll < 50) { // person event
            auto b = static_cast<std::uint32_t>(1 + rng() % (COUNT - 1));
            if (b >= a) {
                ++b;
            }
            const std::int64_t delta = random_delta(rng, 230); // some beyond +-200
            const auto cause = static_cast<std::uint16_t>(rng() % 50);
            const StrongModel::Outcome expected = model.event(source, true, b, delta, now, cause);
            const auto actual = registry.apply_opinion_event(CharacterId{a}, CharacterId{b}, delta, Date{now},
                                                             CauseId{cause}, CONFIG);
            using K = StrongModel::Kind;
            const OpinionEventOutcome expected_outcome =
                expected.kind == K::Updated                ? OpinionEventOutcome::Updated
                : expected.kind == K::Created              ? OpinionEventOutcome::Created
                : expected.kind == K::CreatedWithEviction ? OpinionEventOutcome::CreatedWithEviction
                                                           : OpinionEventOutcome::Dropped;
            if (actual.outcome != expected_outcome || actual.evicted != CharacterId{expected.evicted}) {
                FAIL("step " << step << ": person event outcome " << int(actual.outcome) << " evicted "
                             << actual.evicted.value << ", model " << int(expected_outcome) << " evicted "
                             << expected.evicted);
            }
            created_with_eviction += expected.kind == K::CreatedWithEviction;
            dropped_at_limit += expected.kind == K::DroppedAtLimit;
            updated += expected.kind == K::Updated;
        } else if (roll < 75) { // target event
            const std::uint32_t index = static_cast<std::uint32_t>(1 + rng() % (TOPICS + 4));
            const TargetId target = index <= TOPICS ? topic(index) : community(index - TOPICS);
            const std::int64_t delta = random_delta(rng, 200);
            const auto cause = static_cast<std::uint16_t>(rng() % 50);
            const StrongModel::Outcome expected = model.event(source, false, target.raw(), delta, now, cause);
            const auto actual = registry.apply_opinion_event(CharacterId{a}, target, delta, Date{now}, CauseId{cause}, CONFIG);
            using K = StrongModel::Kind;
            const bool outcome_ok =
                (expected.kind == K::Updated && actual.outcome == OpinionEventOutcome::Updated)
                || (expected.kind == K::Created && actual.outcome == OpinionEventOutcome::Created)
                || (expected.kind == K::CreatedWithEviction && actual.outcome == OpinionEventOutcome::CreatedWithEviction
                    && actual.evicted.raw() == expected.evicted)
                || ((expected.kind == K::DroppedBelowThreshold || expected.kind == K::DroppedAtLimit)
                    && actual.outcome == OpinionEventOutcome::Dropped);
            if (!outcome_ok || (expected.kind != K::CreatedWithEviction && actual.evicted.valid())) {
                FAIL("step " << step << ": target event outcome " << int(actual.outcome) << ", model kind "
                             << int(expected.kind));
            }
            created_with_eviction += expected.kind == K::CreatedWithEviction;
            dropped_at_limit += expected.kind == K::DroppedAtLimit;
        } else if (roll < 87) { // alternate extraversion so people lists overflow and get trimmed
            Character& c = *registry.find(CharacterId{a});
            c.set_extraversion(c.extraversion() > 0 ? -100 : 100);
        } else if (roll < 92) {
            registry.find(CharacterId{a})->set_stability(static_cast<int>(rng() % 201) - 100);
        } else {
            const StrongMaintainCounts expected = model.maintain(registry, now);
            const StrongMaintainCounts actual = registry.maintain(Date{now}, CONFIG);
            if (actual.removed_decayed != expected.removed_decayed
                || actual.evicted_over_limit != expected.evicted_over_limit) {
                FAIL("step " << step << ": maintain counts " << actual.removed_decayed << "/" << actual.evicted_over_limit
                             << ", model " << expected.removed_decayed << "/" << expected.evicted_over_limit);
            }
            removed_decayed += actual.removed_decayed;
            trimmed += actual.evicted_over_limit;
        }

        for (std::uint32_t id = 1; id <= COUNT; ++id) {
            const Character& c = *registry.find(CharacterId{id});
            if (!StrongModel::matches(c.strong_people(), model.people(id))
                || !StrongModel::matches(c.strong_targets(), model.targets(id))) {
                FAIL("step " << step << ": lists of character " << id << " differ from the model");
            }
        }
    }
    MESSAGE("strong opinion property: " << STEPS << " steps; updated " << updated << ", created with eviction "
                                        << created_with_eviction << ", dropped at limit " << dropped_at_limit
                                        << ", removed decayed " << removed_decayed << ", trimmed " << trimmed);
    CHECK(created_with_eviction > 0);
    CHECK(dropped_at_limit > 0);
    CHECK(removed_decayed > 0);
    CHECK(trimmed > 0);
}
