#include <doctest.h>

#include <algorithm>
#include <array>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <limits>
#include <map>
#include <random>
#include <span>
#include <tuple>
#include <type_traits>
#include <vector>

#include "sim/character_registry.hpp"
#include "sim/opinion.hpp"
#include "sim/personal_opinion.hpp"
#include "sim/stance_table.hpp"

using namespace sim;

namespace {

TargetId topic(std::uint32_t id) {
    return *TargetId::from(TopicId{id});
}

TargetId community(std::uint32_t id) {
    return *TargetId::from(CommunityId{id});
}

// A registry of `count` characters with the given init.
CharacterRegistry make_registry(std::uint32_t count, const CharacterInit& init = {}) {
    CharacterRegistry registry;
    for (std::uint32_t i = 1; i <= count; ++i) {
        (void)registry.create(NameId{i}, Gender::Female, Date{0}, init);
    }
    return registry;
}

// The stored order key: CharacterId value, or TargetId raw value.
std::uint32_t key_of(CharacterId id) {
    return id.value;
}

std::uint32_t key_of(TargetId id) {
    return id.raw();
}

template<class Target>
std::vector<std::uint32_t> targets_of(std::span<const LongOpinion<Target>> list) {
    std::vector<std::uint32_t> out;
    for (const LongOpinion<Target>& e : list) {
        out.push_back(key_of(e.target));
    }
    return out;
}

template<class T>
concept CanAddPersonModifier = requires(CharacterRegistry& r, T effect) {
    r.add_modifier(CharacterId{1}, CharacterId{2}, ModifierId{1}, effect);
};
template<class T>
concept CanAddTargetModifier = requires(CharacterRegistry& r, T effect) {
    r.add_modifier(CharacterId{1}, TargetId{}, ModifierId{1}, effect);
};
template<class T>
concept CanAddPersonLong = requires(CharacterRegistry& r, T delta) {
    r.add_long_opinion(CharacterId{1}, CharacterId{2}, delta);
};
template<class T>
concept CanAddTargetLong = requires(CharacterRegistry& r, T delta) {
    r.add_long_opinion(CharacterId{1}, TargetId{}, delta);
};

const OpinionConfig CONFIG{};
const LifecycleConfig LIFECYCLE{};
using O = LongOpinionOutcome;

} // namespace

// ---- layout --------------------------------------------------------------------------------

static_assert(sizeof(PersonLongOpinion) == 8 && std::has_unique_object_representations_v<PersonLongOpinion>);
static_assert(sizeof(TargetLongOpinion) == 8 && std::has_unique_object_representations_v<TargetLongOpinion>);
static_assert(sizeof(OpinionModifier) == 8 && std::has_unique_object_representations_v<OpinionModifier>);
// Offsets (long_people_ 448, long_targets_ 772, modifiers_ 904 and every earlier one) are
// pinned by static_asserts in the Character constructor.
static_assert(sizeof(Character) == 1164);
static_assert(std::is_trivially_copyable_v<Character>);
static_assert(sizeof(ModifierId) == 2 && !ModifierId{}.valid());
static_assert(MODIFIER_EFFECT_MAX == BIPOLAR_MAX && LONG_DELTA_MAX == 2 * LONG_VALUE_MAX);
// Whole numbers only: floats and bool match no overload.
static_assert(CanAddPersonModifier<int> && CanAddTargetModifier<std::int64_t> && CanAddPersonModifier<unsigned>);
static_assert(!CanAddPersonModifier<double> && !CanAddTargetModifier<float> && !CanAddPersonModifier<bool>);
static_assert(CanAddPersonLong<int> && CanAddTargetLong<std::uint64_t>);
static_assert(!CanAddPersonLong<float> && !CanAddTargetLong<double> && !CanAddTargetLong<bool>);

TEST_CASE("personal opinions: sizes") {
    MESSAGE("sizeof(LongOpinion) = " << sizeof(PersonLongOpinion) << ", sizeof(OpinionModifier) = "
                                     << sizeof(OpinionModifier) << ", sizeof(Character) = " << sizeof(Character));
    CHECK(sizeof(Character) == 1164);
}

// ---- modifiers ---------------------------------------------------------------------------------

TEST_CASE("modifiers: add, remove, Duplicate and short = sum of effects per target") {
    CharacterRegistry r = make_registry(3);
    const CharacterId a{1}, b{2}, c{3};
    CHECK(r.add_modifier(a, b, ModifierId{7}, 25) == EditResult::Ok);
    CHECK(r.add_modifier(a, b, ModifierId{3}, -40) == EditResult::Ok);
    CHECK(r.add_modifier(a, b, ModifierId{7}, 90) == EditResult::Duplicate); // no stacking, effect unchanged
    CHECK(r.add_modifier(a, c, ModifierId{7}, 10) == EditResult::Ok);        // same modifier, another target
    CHECK(r.add_modifier(a, topic(5), ModifierId{7}, 5) == EditResult::Ok);
    CHECK(r.add_modifier(a, b, ModifierId{9}, 0) == EditResult::Ok); // a zero effect is stored

    const Character& ca = *r.find(a);
    CHECK(ca.modifiers().size() == 5);
    CHECK(short_opinion(ca, b) == -15);
    CHECK(short_opinion(ca, c) == 10);
    CHECK(short_opinion(ca, topic(5)) == 5);
    CHECK(short_opinion(ca, topic(6)) == 0);
    CHECK(short_opinion(*r.find(b), a) == 0);

    CHECK(r.remove_modifier(a, b, ModifierId{3}) == EditResult::Ok);
    CHECK(r.remove_modifier(a, b, ModifierId{3}) == EditResult::NotFound);
    CHECK(r.remove_modifier(a, b, ModifierId{9}) == EditResult::Ok); // zero effects remove symmetrically
    CHECK(short_opinion(*r.find(a), b) == 25);
    CHECK(short_opinion(*r.find(a), c) == 10); // untouched
    CHECK(r.find(a)->modifiers().size() == 3);
}

TEST_CASE("modifiers: sorted by (domain, target, modifier); a person and a target with one number don't collide") {
    CharacterRegistry r = make_registry(6);
    const CharacterId a{1};
    // TargetId raw for community 5 is 5 (kind 0), equal to CharacterId{5}.
    REQUIRE(community(5).raw() == 5);
    REQUIRE(r.add_modifier(a, community(5), ModifierId{2}, 30) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, CharacterId{5}, ModifierId{2}, -20) == EditResult::Ok); // not a Duplicate
    REQUIRE(r.add_modifier(a, CharacterId{5}, ModifierId{1}, 4) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, CharacterId{2}, ModifierId{9}, 1) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, topic(1), ModifierId{1}, 2) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, community(3), ModifierId{8}, 3) == EditResult::Ok);

    const Character& ca = *r.find(a);
    CHECK(short_opinion(ca, CharacterId{5}) == -16);
    CHECK(short_opinion(ca, community(5)) == 30);
    const auto key = [](const OpinionModifier& m) {
        return std::tuple{static_cast<int>(m.domain), m.target, m.modifier.value};
    };
    const std::span<const OpinionModifier> mods = ca.modifiers();
    CHECK(std::is_sorted(mods.begin(), mods.end(), [&](const auto& x, const auto& y) { return key(x) < key(y); }));
    CHECK(mods[0].domain == ModifierDomain::Person);
    CHECK((mods[0].target == 2 && mods[1].target == 5 && mods[1].modifier == ModifierId{1}));
    CHECK(mods.back().domain == ModifierDomain::Target);
    CHECK(mods.back().target == topic(1).raw());
    CHECK(r.remove_modifier(a, community(5), ModifierId{2}) == EditResult::Ok);
    CHECK(short_opinion(*r.find(a), CharacterId{5}) == -16);
    CHECK(short_opinion(*r.find(a), community(5)) == 0);
}

TEST_CASE("modifiers: effects clamp to +-100; Invalid and NotFound") {
    CharacterRegistry r = make_registry(2);
    const CharacterId a{1}, b{2};
    CHECK(r.add_modifier(a, b, ModifierId{1}, 150) == EditResult::Ok);
    CHECK(r.add_modifier(a, b, ModifierId{2}, std::numeric_limits<std::int64_t>::min()) == EditResult::Ok);
    CHECK(r.add_modifier(a, b, ModifierId{3}, std::numeric_limits<std::uint64_t>::max()) == EditResult::Ok);
    CHECK(r.find(a)->modifiers()[0].effect == 100);
    CHECK(r.find(a)->modifiers()[1].effect == -100);
    CHECK(r.find(a)->modifiers()[2].effect == 100);

    CHECK(r.add_modifier(CharacterId{}, b, ModifierId{1}, 1) == EditResult::Invalid);
    CHECK(r.add_modifier(a, CharacterId{}, ModifierId{1}, 1) == EditResult::Invalid);
    CHECK(r.add_modifier(a, a, ModifierId{1}, 1) == EditResult::Invalid);
    CHECK(r.add_modifier(a, b, ModifierId{}, 1) == EditResult::Invalid);
    CHECK(r.add_modifier(a, TargetId{}, ModifierId{1}, 1) == EditResult::Invalid);
    CHECK(r.add_modifier(a, topic(1), ModifierId{}, 1) == EditResult::Invalid);
    CHECK(r.add_modifier(CharacterId{9}, CharacterId{9}, ModifierId{1}, 1) == EditResult::Invalid); // a == b first
    CHECK(r.add_modifier(CharacterId{9}, b, ModifierId{1}, 1) == EditResult::NotFound);
    CHECK(r.add_modifier(a, CharacterId{9}, ModifierId{1}, 1) == EditResult::NotFound);
    CHECK(r.add_modifier(CharacterId{9}, topic(1), ModifierId{1}, 1) == EditResult::NotFound);
    CHECK(r.add_modifier(CharacterId{9}, topic(1), ModifierId{}, 1) == EditResult::Invalid); // Invalid before NotFound

    CHECK(r.remove_modifier(a, a, ModifierId{1}) == EditResult::Invalid);
    CHECK(r.remove_modifier(a, b, ModifierId{}) == EditResult::Invalid);
    CHECK(r.remove_modifier(a, TargetId{}, ModifierId{1}) == EditResult::Invalid);
    CHECK(r.remove_modifier(CharacterId{9}, b, ModifierId{1}) == EditResult::NotFound);
    CHECK(r.remove_modifier(CharacterId{9}, topic(1), ModifierId{1}) == EditResult::NotFound);
    CHECK(r.remove_modifier(a, topic(1), ModifierId{1}) == EditResult::NotFound);
    CHECK(r.find(a)->modifiers().size() == 3);
}

TEST_CASE("modifiers: Full at MODIFIER_CAP leaves state unchanged; modifiers are never evicted") {
    CharacterRegistry r = make_registry(2);
    const CharacterId a{1}, b{2};
    for (std::uint16_t m = 1; m <= MODIFIER_CAP; ++m) {
        REQUIRE(r.add_modifier(a, m % 2 == 0 ? TargetId{topic(m)} : community(m), ModifierId{m}, -1) == EditResult::Ok);
    }
    const std::vector<OpinionModifier> before(r.find(a)->modifiers().begin(), r.find(a)->modifiers().end());
    CHECK(r.add_modifier(a, b, ModifierId{1}, 100) == EditResult::Full);
    CHECK(r.add_modifier(a, topic(999), ModifierId{1}, 100) == EditResult::Full);
    CHECK(r.add_modifier(a, topic(2), ModifierId{2}, 100) == EditResult::Duplicate); // Duplicate before Full
    const std::span<const OpinionModifier> after = r.find(a)->modifiers();
    REQUIRE(after.size() == before.size());
    CHECK(std::equal(after.begin(), after.end(), before.begin(), [](const auto& x, const auto& y) {
        return x.target == y.target && x.modifier == y.modifier && x.effect == y.effect && x.domain == y.domain;
    }));
    CHECK(r.remove_modifier(a, topic(2), ModifierId{2}) == EditResult::Ok);
    CHECK(r.add_modifier(a, b, ModifierId{1}, 100) == EditResult::Ok);
}

TEST_CASE("modifiers: short is the raw sum of effects; only the opinion total clamps") {
    StanceTable stances;
    CharacterRegistry r = make_registry(2);
    const CharacterId a{1}, b{2};
    REQUIRE(r.add_modifier(a, b, ModifierId{1}, -100) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, b, ModifierId{2}, -50) == EditResult::Ok);
    CHECK(short_opinion(*r.find(a), b) == -150);
    const OpinionBreakdown o = opinion_breakdown(*r.find(a), *r.find(b), stances, CONFIG, WorldSeed{1});
    CHECK(o.short_term == -150);
    CHECK(o.weak.total > -50.0); // the -100 below comes from clamping, not from the weak opinion
    CHECK(o.total == -100.0);
}

// ---- long-term opinions --------------------------------------------------------------------------

TEST_CASE("long opinions: create, update, removal at 0, Unchanged, clamping") {
    CharacterRegistry r = make_registry(3);
    const CharacterId a{1}, b{2}, c{3};
    CHECK(r.add_long_opinion(a, b, 0).outcome == O::Unchanged); // absent, nothing written
    CHECK(r.find(a)->long_people().empty());
    CHECK(r.add_long_opinion(a, b, 30).outcome == O::Created);
    CHECK(long_opinion(*r.find(a), b) == 30);
    CHECK(r.add_long_opinion(a, b, -45).outcome == O::Updated);
    CHECK(long_opinion(*r.find(a), b) == -15);
    CHECK(r.add_long_opinion(a, b, 0).outcome == O::Unchanged); // existing, nothing written
    CHECK(r.add_long_opinion(a, b, 15).outcome == O::Removed);
    CHECK(r.find(a)->long_people().empty());
    CHECK(long_opinion(*r.find(a), b) == 0);

    // Deltas clamp to +-400, results to +-200; a saturated value doesn't move further.
    CHECK(r.add_long_opinion(a, c, 1000).outcome == O::Created);
    CHECK(long_opinion(*r.find(a), c) == 200);
    CHECK(r.add_long_opinion(a, c, 5).outcome == O::Unchanged);
    CHECK(r.add_long_opinion(a, c, std::numeric_limits<std::int64_t>::min()).outcome == O::Updated); // -400
    CHECK(long_opinion(*r.find(a), c) == -200);
    CHECK(r.add_long_opinion(a, c, -1).outcome == O::Unchanged);
    CHECK(r.add_long_opinion(a, c, std::numeric_limits<std::uint64_t>::max()).outcome == O::Updated); // +400
    CHECK(long_opinion(*r.find(a), c) == 200);
}

TEST_CASE("long opinions: Invalid and NotFound; targets") {
    CharacterRegistry r = make_registry(2);
    const CharacterId a{1}, b{2};
    CHECK(r.add_long_opinion(CharacterId{}, b, 5).outcome == O::Invalid);
    CHECK(r.add_long_opinion(a, a, 5).outcome == O::Invalid);
    CHECK(r.add_long_opinion(a, CharacterId{}, 5).outcome == O::Invalid);
    CHECK(r.add_long_opinion(a, TargetId{}, 5).outcome == O::Invalid);
    CHECK(r.add_long_opinion(CharacterId{3}, b, 5).outcome == O::NotFound);
    CHECK(r.add_long_opinion(a, CharacterId{3}, 5).outcome == O::NotFound);
    CHECK(r.add_long_opinion(CharacterId{3}, topic(1), 5).outcome == O::NotFound);

    CHECK(r.add_long_opinion(a, topic(4), -70).outcome == O::Created);
    CHECK(r.add_long_opinion(a, community(4), 20).outcome == O::Created);
    CHECK(long_opinion(*r.find(a), topic(4)) == -70);
    CHECK(long_opinion(*r.find(a), community(4)) == 20);
    CHECK(long_opinion(*r.find(a), CharacterId{4}) == 0); // separate list
    CHECK(targets_of(r.find(a)->long_targets()) == std::vector<std::uint32_t>{community(4).raw(), topic(4).raw()});
}

TEST_CASE("long opinions: the people limit is 8, 24 and 40 at extraversion -100, 0 and +100") {
    for (const auto& [extraversion, limit] : {std::pair{-100, 8u}, std::pair{0, 24u}, std::pair{100, 40u}}) {
        CAPTURE(extraversion);
        CharacterRegistry r = make_registry(50, CharacterInit{.extraversion = extraversion});
        const CharacterId a{1};
        CHECK(person_limit(*r.find(a)) == limit);
        for (std::uint32_t k = 2; k < 2 + limit; ++k) {
            REQUIRE(r.add_long_opinion(a, CharacterId{k}, 50).outcome == O::Created);
        }
        CHECK(r.add_long_opinion(a, CharacterId{2 + limit}, 50).outcome == O::Dropped);
        CHECK(r.find(a)->long_people().size() == limit);
    }
}

TEST_CASE("long opinions: the weakest entry is evicted with the tie-break; equal strength is Dropped") {
    CharacterRegistry r = make_registry(20, CharacterInit{.extraversion = -100}); // limit 8
    const CharacterId a{1};
    const std::array<int, 8> values{90, -12, 50, 12, -70, 200, 12, 33}; // targets 2..9; weakest |12| at 3, 5 and 8
    for (std::uint32_t k = 0; k < values.size(); ++k) {
        REQUIRE(r.add_long_opinion(a, CharacterId{2 + k}, values[k]).outcome == O::Created);
    }
    CHECK(r.add_long_opinion(a, CharacterId{15}, -12).outcome == O::Dropped); // equal strength
    CHECK(r.add_long_opinion(a, CharacterId{15}, 5).outcome == O::Dropped);

    const LongOpinionResult<CharacterId> first = r.add_long_opinion(a, CharacterId{15}, -13);
    CHECK(first.outcome == O::CreatedWithEviction);
    CHECK(first.evicted == CharacterId{3}); // ties go to the smaller target
    const LongOpinionResult<CharacterId> second = r.add_long_opinion(a, CharacterId{10}, 13);
    CHECK(second.outcome == O::CreatedWithEviction);
    CHECK(second.evicted == CharacterId{5});
    CHECK(r.add_long_opinion(a, CharacterId{11}, 12).outcome == O::Dropped); // 12 at 8 left, not stronger
    CHECK(targets_of(r.find(a)->long_people()) == std::vector<std::uint32_t>{2, 4, 6, 7, 8, 9, 10, 15});

    // Updates of existing entries never evict.
    CHECK(r.add_long_opinion(a, CharacterId{8}, 1).outcome == O::Updated);
    CHECK(r.find(a)->long_people().size() == 8);
}

TEST_CASE("long opinions: TARGET_LIMIT holds and evicted targets are typed") {
    CharacterRegistry r = make_registry(1);
    const CharacterId a{1};
    for (std::uint32_t k = 1; k <= TARGET_LIMIT; ++k) {
        REQUIRE(r.add_long_opinion(a, k == 4 ? community(k) : topic(k), k == 4 ? -3 : 40).outcome == O::Created);
    }
    CHECK(r.add_long_opinion(a, topic(100), 3).outcome == O::Dropped);
    const LongOpinionResult<TargetId> result = r.add_long_opinion(a, topic(100), -4);
    CHECK(result.outcome == O::CreatedWithEviction);
    CHECK(result.evicted == community(4));
    CHECK(result.evicted.kind() == TargetKind::Community);
    CHECK(r.find(a)->long_targets().size() == TARGET_LIMIT);
    CHECK(r.add_long_opinion(a, topic(1), 1).outcome == O::Updated); // no evicted target
    CHECK(!r.add_long_opinion(a, topic(1), 1).evicted.valid());
}

TEST_CASE("long opinions: modifiers affect neither the limit nor eviction") {
    CharacterRegistry r = make_registry(40, CharacterInit{.extraversion = -100}); // limit 8
    const CharacterId a{1};
    for (std::uint32_t k = 2; k <= 9; ++k) {
        REQUIRE(r.add_long_opinion(a, CharacterId{k}, k == 6 ? 10 : 80).outcome == O::Created);
    }
    // Strong modifiers on the weakest entry's target and many other people.
    REQUIRE(r.add_modifier(a, CharacterId{6}, ModifierId{1}, 100) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, CharacterId{6}, ModifierId{2}, 100) == EditResult::Ok);
    for (std::uint32_t k = 10; k <= 30; ++k) {
        REQUIRE(r.add_modifier(a, CharacterId{k}, ModifierId{1}, -100) == EditResult::Ok);
    }
    // A new entry just below |10| is still Dropped although target 20 has modifiers.
    CHECK(r.add_long_opinion(a, CharacterId{20}, 10).outcome == O::Dropped);
    const LongOpinionResult<CharacterId> result = r.add_long_opinion(a, CharacterId{20}, 11);
    CHECK(result.outcome == O::CreatedWithEviction);
    CHECK(result.evicted == CharacterId{6}); // |long| only; its +200 short term doesn't count
    CHECK(r.find(a)->long_people().size() == 8);
    CHECK(r.maintain() == 0);
    CHECK(r.find(a)->modifiers().size() == 23); // modifiers untouched by eviction and maintain
}

TEST_CASE("maintain: trims after extraversion drops, weakest first; idempotent") {
    CharacterRegistry r = make_registry(50, CharacterInit{.extraversion = 100});
    const CharacterId a{1}, b{2};
    for (std::uint32_t k = 2; k <= 41; ++k) {
        REQUIRE(r.add_long_opinion(a, CharacterId{k}, static_cast<int>(k) * (k % 2 == 0 ? 1 : -1)).outcome
                == O::Created);
    }
    REQUIRE(r.add_long_opinion(b, CharacterId{1}, 5).outcome == O::Created);
    CHECK(r.maintain() == 0);
    r.find(a)->set_extraversion(-100); // limit 8
    CHECK(r.find(a)->long_people().size() == 40);
    CHECK(r.maintain() == 32);
    CHECK(targets_of(r.find(a)->long_people()) == std::vector<std::uint32_t>{34, 35, 36, 37, 38, 39, 40, 41});
    CHECK(r.maintain() == 0);
    CHECK(r.find(a)->long_people().size() == 8);
    CHECK(r.find(b)->long_people().size() == 1);
    // A new entry into a list above its limit evicts only one entry.
    r.find(a)->set_extraversion(100);
    for (std::uint32_t k = 2; k <= 33; ++k) {
        REQUIRE(r.add_long_opinion(a, CharacterId{k}, 100).outcome == O::Created);
    }
    r.find(a)->set_extraversion(-100);
    CHECK(r.add_long_opinion(a, CharacterId{42}, 200).outcome == O::CreatedWithEviction);
    CHECK(r.find(a)->long_people().size() == 40);
    CHECK(r.maintain() == 32);
}

// ---- opinion -----------------------------------------------------------------------------------

TEST_CASE("opinion: equals weak without records, clamp(weak + long + short) with them; breakdown adds up") {
    StanceTable stances;
    REQUIRE(stances.set_stance(CommunityId{1}, community(1), 30) == EditResult::Ok);
    CharacterRegistry r = make_registry(3, CharacterInit{.reputation = 20});
    const CharacterId a{1}, b{2}, c{3};
    REQUIRE(r.find(a)->set_involvement(CommunityId{1}, 5) == EditResult::Ok);
    REQUIRE(r.find(b)->set_involvement(CommunityId{1}, 5) == EditResult::Ok);
    const WorldSeed seed{77};

    const OpinionBreakdown none = opinion_breakdown(*r.find(a), *r.find(c), stances, CONFIG, seed);
    CHECK(none.total == weak_opinion(*r.find(a), *r.find(c), stances, CONFIG, seed));
    CHECK((none.long_term == 0 && none.short_term == 0));
    CHECK(opinion(*r.find(a), topic(4), stances, CONFIG, seed) == weak_opinion(*r.find(a), topic(4), stances, CONFIG, seed));

    REQUIRE(r.add_long_opinion(a, c, -25).outcome == O::Created);
    REQUIRE(r.add_modifier(a, c, ModifierId{1}, 9) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, c, ModifierId{2}, -4) == EditResult::Ok);
    const OpinionBreakdown o = opinion_breakdown(*r.find(a), *r.find(c), stances, CONFIG, seed);
    CHECK(o.weak.total == weak_opinion(*r.find(a), *r.find(c), stances, CONFIG, seed));
    CHECK(o.long_term == -25);
    CHECK(o.short_term == 5);
    CHECK(o.total == std::clamp(o.weak.total - 20.0, -100.0, 100.0));
    CHECK(opinion(*r.find(a), *r.find(c), stances, CONFIG, seed) == o.total);

    REQUIRE(r.add_long_opinion(a, topic(4), 60).outcome == O::Created);
    REQUIRE(r.add_modifier(a, topic(4), ModifierId{1}, 100) == EditResult::Ok);
    const OpinionBreakdown t = opinion_breakdown(*r.find(a), topic(4), stances, CONFIG, seed);
    CHECK((t.long_term == 60 && t.short_term == 100));
    CHECK(t.total == 100.0); // clamped

    REQUIRE(r.add_long_opinion(b, a, -200).outcome == O::Created);
    const OpinionBreakdown low = opinion_breakdown(*r.find(b), *r.find(a), stances, CONFIG, seed);
    CHECK(low.total == -100.0);
}

TEST_CASE("opinion: personal terms pull back from a saturated weak opinion (clamped weak total)") {
    StanceTable stances;
    REQUIRE(stances.set_stance(CommunityId{1}, community(1), 100) == EditResult::Ok);
    CharacterRegistry r = make_registry(2, CharacterInit{.reputation = 100});
    const CharacterId a{1}, b{2};
    REQUIRE(r.find(a)->set_involvement(CommunityId{1}, 1) == EditResult::Ok);
    REQUIRE(r.find(b)->set_involvement(CommunityId{1}, 1) == EditResult::Ok);
    const WorldSeed seed{5};
    REQUIRE(r.add_long_opinion(a, b, -30).outcome == O::Created);
    REQUIRE(r.add_modifier(a, b, ModifierId{1}, -5) == EditResult::Ok);

    const OpinionBreakdown o = opinion_breakdown(*r.find(a), *r.find(b), stances, CONFIG, seed);
    const double unclamped = o.weak.community + o.weak.reputation + o.weak.compat + o.weak.noise;
    MESSAGE("unclamped weak terms = " << unclamped);
    CHECK(unclamped - 35.0 > 100.0); // clamping only once, after adding, would still give 100
    CHECK(o.weak.total == 100.0);
    CHECK(o.total == 65.0);
}

TEST_CASE("opinion: relations don't change opinions in either direction") {
    StanceTable stances;
    REQUIRE(stances.set_stance(CommunityId{1}, community(2), 35) == EditResult::Ok);
    REQUIRE(stances.set_stance(CommunityId{2}, community(1), -60) == EditResult::Ok);
    CharacterRegistry r;
    const CharacterId a = r.create(NameId{1}, Gender::Female, Date{0},
                                   CharacterInit{.stability = 30, .openness = -20, .extraversion = 50, .reputation = 12});
    const CharacterId b = r.create(NameId{2}, Gender::Male, Date{0},
                                   CharacterInit{.stability = -70, .agreeableness = 40, .reputation = -33});
    REQUIRE(r.find(a)->set_involvement(CommunityId{1}, 9) == EditResult::Ok);
    REQUIRE(r.find(b)->set_involvement(CommunityId{2}, 4) == EditResult::Ok);
    REQUIRE(r.add_long_opinion(a, b, 17).outcome == O::Created);
    REQUIRE(r.add_modifier(b, a, ModifierId{1}, -8) == EditResult::Ok);
    const WorldSeed seed{4242};
    const auto ab = [&] { return opinion(*r.find(a), *r.find(b), stances, CONFIG, seed); };
    const auto ba = [&] { return opinion(*r.find(b), *r.find(a), stances, CONFIG, seed); };
    const double ab_before = ab();
    const double ba_before = ba();

    REQUIRE(r.set_relation(a, RelationType::Friend, b) == EditResult::Ok);
    CHECK(ab() == ab_before);
    CHECK(ba() == ba_before);
    REQUIRE(r.set_relation(b, RelationType::Rival, a) == EditResult::Ok);
    REQUIRE(r.link(a, RelationType::Spouse, b) == EditResult::Ok);
    CHECK(ab() == ab_before);
    CHECK(ba() == ba_before);
    REQUIRE(r.link(a, RelationType::Employer, b) == EditResult::Ok);
    CHECK(ab() == ab_before);
    CHECK(ba() == ba_before);
    REQUIRE(r.unlink(b, RelationType::Spouse, a) == EditResult::Ok);
    REQUIRE(r.unlink(a, RelationType::Employer, b) == EditResult::Ok);
    REQUIRE(r.clear_relation(a, RelationType::Friend, b) == EditResult::Ok);
    CHECK(ab() == ab_before);
    CHECK(ba() == ba_before);
}

TEST_CASE("typed reads: long-term entries hold typed ids; modifiers convert through their domain") {
    CharacterRegistry r = make_registry(6, CharacterInit{.extraversion = 100});
    const CharacterId a{1};
    REQUIRE(r.add_long_opinion(a, CharacterId{5}, 40).outcome == O::Created);
    REQUIRE(r.add_long_opinion(a, community(5), -30).outcome == O::Created);
    REQUIRE(r.add_long_opinion(a, topic(5), 20).outcome == O::Created);
    REQUIRE(r.add_modifier(a, CharacterId{5}, ModifierId{1}, 9) == EditResult::Ok);
    REQUIRE(r.add_modifier(a, topic(5), ModifierId{1}, -9) == EditResult::Ok);
    const Character& c = *r.find(a);

    REQUIRE(c.long_people().size() == 1);
    CHECK(c.long_people()[0].target == CharacterId{5});
    REQUIRE(c.long_targets().size() == 2);
    CHECK(c.long_targets()[0].target == community(5)); // TargetId order: communities before topics
    CHECK(c.long_targets()[1].target == topic(5));
    CHECK(c.long_targets()[1].target.as_topic() == TopicId{5});

    REQUIRE(c.modifiers().size() == 2);
    const OpinionModifier& person = c.modifiers()[0];
    const OpinionModifier& target = c.modifiers()[1];
    CHECK(person.person() == CharacterId{5});
    CHECK(!person.target_id().has_value());
    CHECK(target.target_id() == topic(5));
    CHECK(!target.person().has_value());
    // A raw value with a reserved kind never decodes.
    const OpinionModifier forged{.target = (std::uint32_t{2} << TargetId::INDEX_BITS) | 5,
                                 .modifier = ModifierId{1},
                                 .effect = 0,
                                 .domain = ModifierDomain::Target};
    CHECK(!forged.target_id().has_value());
    // Value-initialized long-term slots hold the invalid ids.
    CHECK(!PersonLongOpinion{}.target.valid());
    CHECK(!TargetLongOpinion{}.target.valid());
}

// ---- golden values ---------------------------------------------------------------------------------------

TEST_CASE("personal opinions: golden values (identical in Debug and Release)") {
    CharacterRegistry reg;
    const CharacterId a = reg.create(NameId{1}, Gender::Female, Date{0},
                                     CharacterInit{.stability = -40, .openness = 60, .extraversion = 10,
                                                   .conscientiousness = 5, .agreeableness = -15, .reputation = 12});
    const CharacterId b = reg.create(NameId{2}, Gender::Male, Date{0},
                                     CharacterInit{.stability = 70, .openness = -30, .extraversion = -80,
                                                   .conscientiousness = 40, .agreeableness = 25, .reputation = -45});
    const CharacterId c = reg.create(NameId{3}, Gender::Female, Date{0}, CharacterInit{});
    const TargetId topic7 = topic(7);

    CHECK(reg.add_long_opinion(a, b, 80).outcome == O::Created);
    CHECK(reg.add_long_opinion(a, b, -30).outcome == O::Updated);          // 50
    CHECK(reg.add_long_opinion(a, topic7, 60).outcome == O::Created);
    CHECK(reg.add_long_opinion(a, topic7, -25).outcome == O::Updated);     // 35
    CHECK(reg.add_long_opinion(b, a, -40).outcome == O::Created);
    CHECK(reg.add_long_opinion(b, c, 0).outcome == O::Unchanged);
    CHECK(reg.add_modifier(a, b, ModifierId{1}, 15) == EditResult::Ok);
    CHECK(reg.add_modifier(a, b, ModifierId{2}, -40) == EditResult::Ok);   // short -25
    CHECK(reg.add_modifier(a, topic7, ModifierId{1}, 10) == EditResult::Ok);
    CHECK(reg.add_modifier(b, a, ModifierId{3}, -150) == EditResult::Ok);  // effect clamped to -100 at add
    CHECK(reg.add_modifier(c, a, ModifierId{1}, 7) == EditResult::Ok);
    CHECK(reg.add_modifier(c, a, ModifierId{4}, 30) == EditResult::Ok);
    CHECK(reg.remove_modifier(c, a, ModifierId{4}) == EditResult::Ok);     // short +7
    CHECK(reg.maintain() == 0);

    StanceTable stances;
    REQUIRE(stances.set_stance(CommunityId{1}, topic7, 40) == EditResult::Ok);
    REQUIRE(reg.find(a)->set_involvement(CommunityId{1}, 10) == EditResult::Ok);
    const WorldSeed seed{0xABCDu};

    // Weak values are the Step 4/5 golden values for this setup; totals add whole numbers.
    const OpinionBreakdown o_ab = opinion_breakdown(*reg.find(a), *reg.find(b), stances, CONFIG, seed);
    CHECK(o_ab.weak.total == 0x1.8eac8ac193ca4p+1); // 3.114640564460915
    CHECK((o_ab.long_term == 50 && o_ab.short_term == -25));
    CHECK(o_ab.total == 0x1.c1d5915832794p+4);      // 28.114640564460913
    const OpinionBreakdown o_ba = opinion_breakdown(*reg.find(b), *reg.find(a), stances, CONFIG, seed);
    CHECK((o_ba.long_term == -40 && o_ba.short_term == -100));
    CHECK(o_ba.total == -100.0);                    // weak 13.18... - 140; only the total is clamped
    const OpinionBreakdown o_at = opinion_breakdown(*reg.find(a), topic7, stances, CONFIG, seed);
    CHECK(o_at.weak.total == 0x1.6e590eb3e1b3cp+5); // 45.793485074353583
    CHECK((o_at.long_term == 35 && o_at.short_term == 10));
    CHECK(o_at.total == 0x1.6b2c8759f0d9ep+6);      // 90.79348507435358
    const OpinionBreakdown o_ca = opinion_breakdown(*reg.find(c), *reg.find(a), stances, CONFIG, seed);
    CHECK(o_ca.weak.total == 0x1.4f25802f1e53p+3);  // 10.47332772448371
    CHECK((o_ca.long_term == 0 && o_ca.short_term == 7));
    CHECK(o_ca.total == 0x1.1792c0178f298p+4);      // 17.47332772448371

    // Step 6: b dies at week 60. a's long entry about b is rebased by
    // round(weak_before - weak_after) = round(3.1146... + 2.2853...) = 5; c has no entry about b.
    const OpinionBreakdown c_b = opinion_breakdown(*reg.find(c), *reg.find(b), stances, CONFIG, seed);
    CHECK(c_b.total == 0x1.1b10d07832da6p+3);        // 8.8458025310163286 (weak only)
    // Step 7: holders and fame. a remembers b (long entry and modifiers); b and c remember a.
    CHECK(reg.holders(b) == 1);
    CHECK(reg.holders(a) == 2);
    CHECK(reg.holders(c) == 0);
    const KillResult death_b = reg.kill(b, Date{60}, WorldContext{stances, CONFIG, LIFECYCLE, seed});
    REQUIRE(death_b.result == EditResult::Ok);
    CHECK(death_b.fame == 46); // 1 holder + |reputation -45|
    CHECK(!death_b.legendary);
    CHECK(reg.find_dead(b)->fame == 46);
    CHECK(reg.holders(b) == 1); // a still remembers b
    CHECK(reg.holders(a) == 1); // b's references stopped counting
    const OpinionBreakdown d_ab = opinion_breakdown(*reg.find(a), *reg.find_dead(b), stances, CONFIG, seed);
    CHECK(d_ab.weak.total == -0x1.2486a8719f69p+1);  // -2.2853594355390854
    CHECK((d_ab.long_term == 55 && d_ab.short_term == -25));
    CHECK(d_ab.total == 0x1.bb6f2af1cc12ep+4);       // 27.714640564460915, was 28.114640564460913
    const OpinionBreakdown d_cb = opinion_breakdown(*reg.find(c), *reg.find_dead(b), stances, CONFIG, seed);
    CHECK(d_cb.total == -0x1.5aacaf719c6p+0);        // -1.3541974689836707, was 8.8458...: the dead-record base
    CHECK(opinion(*reg.find(a), topic7, stances, CONFIG, seed) == 0x1.6b2c8759f0d9ep+6); // unchanged
    CHECK(opinion(*reg.find(c), *reg.find(a), stances, CONFIG, seed) == 0x1.1792c0178f298p+4);

    // Step 7: removing a's last reference forgets b.
    CHECK(reg.remove_modifier(a, b, ModifierId{1}) == EditResult::Ok);
    CHECK(reg.remove_modifier(a, b, ModifierId{2}) == EditResult::Ok);
    CHECK(reg.holders(b) == 1);
    CHECK(reg.find_dead(b) != nullptr);
    CHECK(reg.add_long_opinion(a, b, -55).outcome == O::Removed);
    CHECK(!reg.exists(b));
    CHECK(reg.holders(b) == 0);
    CHECK(reg.dead_count() == 0);
    CHECK(reg.add_modifier(a, b, ModifierId{1}, 1) == EditResult::NotFound);
    CHECK(opinion(*reg.find(a), topic7, stances, CONFIG, seed) == 0x1.6b2c8759f0d9ep+6); // unchanged
}

// ---- property test against a reference model ----------------------------------------------

namespace {

// Naive model of personal opinions: std::map per list, rules written out directly.
class PersonalModel {
public:
    struct Person {
        int extraversion = 0;
        std::map<std::uint32_t, int> people;
        std::map<std::uint32_t, int> targets;
        std::map<std::tuple<int, std::uint32_t, std::uint16_t>, int> modifiers; // (domain, target, modifier) -> effect
    };

    explicit PersonalModel(std::size_t count) : persons_(count) {}

    Person& at(CharacterId id) { return persons_[id.value - 1]; }

    static std::size_t limit(int extraversion) {
        return 8 + static_cast<std::size_t>((32 * (extraversion + 100) + 100) / 200);
    }

    static EditResult add_modifier(Person& p, int domain, std::uint32_t target, std::uint16_t modifier, int effect) {
        const auto key = std::tuple{domain, target, modifier};
        if (p.modifiers.count(key) != 0) {
            return EditResult::Duplicate;
        }
        if (p.modifiers.size() >= MODIFIER_CAP) {
            return EditResult::Full;
        }
        p.modifiers[key] = std::clamp(effect, -100, 100);
        return EditResult::Ok;
    }

    static EditResult remove_modifier(Person& p, int domain, std::uint32_t target, std::uint16_t modifier) {
        return p.modifiers.erase(std::tuple{domain, target, modifier}) != 0 ? EditResult::Ok : EditResult::NotFound;
    }

    static int short_sum(const Person& p, int domain, std::uint32_t target) {
        int sum = 0;
        for (const auto& [key, effect] : p.modifiers) {
            if (std::get<0>(key) == domain && std::get<1>(key) == target) {
                sum += effect;
            }
        }
        return sum;
    }

    // Returns the outcome and the evicted raw target (0 if none).
    static std::pair<LongOpinionOutcome, std::uint32_t> add_long(std::map<std::uint32_t, int>& list, std::size_t limit,
                                                                 std::uint32_t target, std::int64_t raw_delta) {
        const int delta = static_cast<int>(std::clamp<std::int64_t>(raw_delta, -400, 400));
        const auto it = list.find(target);
        if (it != list.end()) {
            const int next = std::clamp(it->second + delta, -200, 200);
            if (next == it->second) {
                return {O::Unchanged, 0};
            }
            if (next == 0) {
                list.erase(it);
                return {O::Removed, 0};
            }
            it->second = next;
            return {O::Updated, 0};
        }
        if (delta == 0) {
            return {O::Unchanged, 0};
        }
        const int value = std::clamp(delta, -200, 200);
        if (list.size() >= limit) {
            auto weakest = list.begin();
            for (auto e = list.begin(); e != list.end(); ++e) {
                if (std::abs(e->second) < std::abs(weakest->second)) {
                    weakest = e;
                }
            }
            if (std::abs(value) <= std::abs(weakest->second)) {
                return {O::Dropped, 0};
            }
            const std::uint32_t evicted = weakest->first;
            list.erase(weakest);
            list[target] = value;
            return {O::CreatedWithEviction, evicted};
        }
        list[target] = value;
        return {O::Created, 0};
    }

    // Trims every list above its limit, weakest first (ties to the smaller target).
    std::size_t maintain() {
        std::size_t evicted = 0;
        for (Person& p : persons_) {
            for (auto [list, lim] : {std::pair{&p.people, limit(p.extraversion)}, std::pair{&p.targets, TARGET_LIMIT}}) {
                while (list->size() > lim) {
                    auto weakest = list->begin();
                    for (auto e = list->begin(); e != list->end(); ++e) {
                        if (std::abs(e->second) < std::abs(weakest->second)) {
                            weakest = e;
                        }
                    }
                    list->erase(weakest);
                    ++evicted;
                }
            }
        }
        return evicted;
    }

    std::vector<Person>& persons() { return persons_; }

private:
    std::vector<Person> persons_;
};

// Test-side inverse of TargetId::raw() for the kinds used here.
TargetId target_from_raw(std::uint32_t raw) {
    const std::uint32_t index = raw & (TargetId::INDEX_LIMIT - 1);
    return (raw >> TargetId::INDEX_BITS) == 0 ? community(index) : topic(index);
}

template<class Target>
bool same_long(std::span<const LongOpinion<Target>> actual, const std::map<std::uint32_t, int>& expected) {
    if (actual.size() != expected.size()) {
        return false;
    }
    auto it = expected.begin();
    for (const LongOpinion<Target>& e : actual) {
        if (key_of(e.target) != it->first || e.value != it->second || e.reserved != 0) {
            return false;
        }
        ++it;
    }
    return true;
}

bool same_modifiers(std::span<const OpinionModifier> actual,
                    const std::map<std::tuple<int, std::uint32_t, std::uint16_t>, int>& expected) {
    if (actual.size() != expected.size()) {
        return false;
    }
    auto it = expected.begin();
    for (const OpinionModifier& m : actual) {
        if (std::tuple{static_cast<int>(m.domain), m.target, m.modifier.value} != it->first || m.effect != it->second) {
            return false;
        }
        ++it;
    }
    return true;
}

} // namespace

TEST_CASE("property: random modifiers, long opinions, extraversion changes and maintain match a reference model") {
    constexpr std::uint32_t CHARACTERS = 16;
    constexpr std::uint32_t TOPICS = 24;
    constexpr std::uint32_t COMMUNITIES = 4;
    constexpr int STEPS = 8000;
    std::mt19937 rng(20260916u);
    const auto pick = [&](std::uint32_t n) { return static_cast<std::uint32_t>(rng() % n); };

    CharacterRegistry reg;
    PersonalModel model(CHARACTERS);
    for (std::uint32_t i = 1; i <= CHARACTERS; ++i) {
        const int extraversion = i % 2 == 0 ? 100 : -100; // alternating high and low limits
        (void)reg.create(NameId{i}, Gender::Female, Date{0}, CharacterInit{.extraversion = extraversion});
        model.at(CharacterId{i}).extraversion = extraversion;
    }
    const auto random_target = [&]() -> TargetId {
        if (pick(40) == 0) {
            return TargetId{};
        }
        const std::uint32_t k = pick(TOPICS + COMMUNITIES);
        return k < COMMUNITIES ? community(1 + k) : topic(1 + k - COMMUNITIES);
    };
    const auto random_source = [&]() {
        const std::uint32_t r = pick(50);
        return r == 0 ? CharacterId{} : r == 1 ? CharacterId{CHARACTERS + 1} : CharacterId{1 + pick(CHARACTERS)};
    };
    const auto random_modifier = [&]() {
        return pick(30) == 0 ? ModifierId{} : ModifierId{static_cast<std::uint16_t>(1 + pick(4))};
    };
    const auto check_character = [&](CharacterId id) {
        if (!id.valid() || id.value > CHARACTERS) {
            return;
        }
        const Character& c = *reg.find(id);
        const PersonalModel::Person& p = model.at(id);
        REQUIRE(same_long(c.long_people(), p.people));
        REQUIRE(same_long(c.long_targets(), p.targets));
        REQUIRE(same_modifiers(c.modifiers(), p.modifiers));
    };
    const auto valid_pair = [&](CharacterId a, CharacterId b) {
        return a.valid() && b.valid() && a != b && a.value <= CHARACTERS && b.value <= CHARACTERS;
    };

    std::array<std::size_t, 8> long_counts{};
    std::array<std::size_t, 6> modifier_counts{};
    std::size_t trimmed = 0;
    std::size_t short_checks = 0;

    for (int step = 0; step < STEPS; ++step) {
        CAPTURE(step);
        const std::uint32_t op = pick(100);
        const CharacterId a = random_source();
        const bool person = pick(2) == 0;
        CharacterId b{1 + pick(CHARACTERS + 1)}; // may be a, or unknown
        const TargetId t = random_target();

        if (op < 32) { // add modifier
            const ModifierId m = random_modifier();
            const int effect = static_cast<int>(pick(301)) - 150;
            EditResult expected = EditResult::Invalid;
            EditResult actual = EditResult::Invalid;
            if (person) {
                if (a.valid() && b.valid() && a != b && m.valid()) {
                    expected = !valid_pair(a, b)
                                   ? EditResult::NotFound
                                   : PersonalModel::add_modifier(model.at(a), 0, b.value, m.value, effect);
                }
                actual = reg.add_modifier(a, b, m, effect);
                if (valid_pair(a, b)) {
                    ++short_checks;
                    CHECK(short_opinion(*reg.find(a), b) == PersonalModel::short_sum(model.at(a), 0, b.value));
                }
            } else {
                if (a.valid() && t.valid() && m.valid()) {
                    expected = a.value > CHARACTERS
                                   ? EditResult::NotFound
                                   : PersonalModel::add_modifier(model.at(a), 1, t.raw(), m.value, effect);
                }
                actual = reg.add_modifier(a, t, m, effect);
                if (a.valid() && a.value <= CHARACTERS && t.valid()) {
                    ++short_checks;
                    CHECK(short_opinion(*reg.find(a), t) == PersonalModel::short_sum(model.at(a), 1, t.raw()));
                }
            }
            REQUIRE(actual == expected);
            ++modifier_counts[static_cast<std::size_t>(actual)];
        } else if (op < 44) { // remove modifier, mostly an existing one
            ModifierId m = random_modifier();
            if (a.valid() && a.value <= CHARACTERS && !model.at(a).modifiers.empty() && pick(3) != 0) {
                auto it = model.at(a).modifiers.begin();
                std::advance(it, pick(static_cast<std::uint32_t>(model.at(a).modifiers.size())));
                const auto [domain, target, modifier] = it->first;
                m = ModifierId{modifier};
                const EditResult removed = domain == 0 ? reg.remove_modifier(a, CharacterId{target}, m)
                                                       : reg.remove_modifier(a, target_from_raw(target), m);
                REQUIRE(removed == EditResult::Ok);
                model.at(a).modifiers.erase(it);
            } else if (person) {
                EditResult expected = EditResult::Invalid;
                if (a.valid() && b.valid() && a != b && m.valid()) {
                    expected = !valid_pair(a, b) ? EditResult::NotFound
                                                 : PersonalModel::remove_modifier(model.at(a), 0, b.value, m.value);
                }
                REQUIRE(reg.remove_modifier(a, b, m) == expected);
            } else {
                EditResult expected = EditResult::Invalid;
                if (a.valid() && t.valid() && m.valid()) {
                    expected = a.value > CHARACTERS ? EditResult::NotFound
                                                    : PersonalModel::remove_modifier(model.at(a), 1, t.raw(), m.value);
                }
                REQUIRE(reg.remove_modifier(a, t, m) == expected);
            }
        } else if (op < 88) { // long opinion change
            std::int64_t delta = 0;
            const std::uint32_t kind = pick(20);
            if (kind < 2) {
                delta = 0;
            } else if (kind < 6) {
                delta = static_cast<std::int64_t>(pick(41)) - 20;
            } else if (kind < 9) {
                delta = pick(2) == 0 ? 400 : -400;
            } else if (kind < 11) {
                delta = pick(2) == 0 ? std::numeric_limits<std::int64_t>::max() : std::numeric_limits<std::int64_t>::min();
            } else {
                delta = static_cast<std::int64_t>(pick(801)) - 400;
            }
            if (person) {
                std::pair<LongOpinionOutcome, std::uint32_t> expected{O::Invalid, 0};
                if (a.valid() && b.valid() && a != b) {
                    if (!valid_pair(a, b)) {
                        expected = {O::NotFound, 0};
                    } else {
                        PersonalModel::Person& p = model.at(a);
                        if (kind == 19 && p.people.count(b.value) != 0) {
                            delta = -p.people[b.value]; // exact cancel
                        }
                        expected = PersonalModel::add_long(p.people, PersonalModel::limit(p.extraversion), b.value, delta);
                    }
                }
                const LongOpinionResult<CharacterId> actual = reg.add_long_opinion(a, b, delta);
                REQUIRE(actual.outcome == expected.first);
                REQUIRE(actual.evicted == CharacterId{expected.second});
                ++long_counts[static_cast<std::size_t>(actual.outcome)];
                if (valid_pair(a, b)) {
                    CHECK(long_opinion(*reg.find(a), b) == (model.at(a).people.count(b.value) != 0
                                                                ? model.at(a).people[b.value]
                                                                : 0));
                }
            } else {
                std::pair<LongOpinionOutcome, std::uint32_t> expected{O::Invalid, 0};
                if (a.valid() && t.valid()) {
                    if (a.value > CHARACTERS) {
                        expected = {O::NotFound, 0};
                    } else {
                        PersonalModel::Person& p = model.at(a);
                        if (kind == 19 && p.targets.count(t.raw()) != 0) {
                            delta = -p.targets[t.raw()];
                        }
                        expected = PersonalModel::add_long(p.targets, TARGET_LIMIT, t.raw(), delta);
                    }
                }
                const LongOpinionResult<TargetId> actual = reg.add_long_opinion(a, t, delta);
                REQUIRE(actual.outcome == expected.first);
                REQUIRE(actual.evicted.raw() == expected.second);
                ++long_counts[static_cast<std::size_t>(actual.outcome)];
            }
        } else if (op < 96) { // extraversion change
            if (a.valid() && a.value <= CHARACTERS) {
                const int extraversion = static_cast<int>(pick(201)) - 100;
                reg.find(a)->set_extraversion(extraversion);
                model.at(a).extraversion = extraversion;
            }
        } else { // maintain
            const std::size_t evicted = reg.maintain();
            REQUIRE(evicted == model.maintain());
            trimmed += evicted;
            for (std::uint32_t i = 1; i <= CHARACTERS; ++i) {
                check_character(CharacterId{i});
            }
            continue;
        }
        check_character(a);
    }

    MESSAGE("personal opinion property: " << STEPS << " steps; long: updated " << long_counts[0] << ", created "
                                          << long_counts[1] << ", with eviction " << long_counts[2] << ", removed "
                                          << long_counts[3] << ", unchanged " << long_counts[4] << ", dropped "
                                          << long_counts[5] << ", invalid " << long_counts[6] << ", not found "
                                          << long_counts[7] << "; modifiers: ok " << modifier_counts[0] << ", full "
                                          << modifier_counts[1] << ", duplicate " << modifier_counts[2]
                                          << ", not found " << modifier_counts[4] << ", invalid "
                                          << modifier_counts[5] << "; trimmed " << trimmed << "; short checks "
                                          << short_checks);
    CHECK(modifier_counts[static_cast<std::size_t>(EditResult::Duplicate)] > 0);
    CHECK(modifier_counts[static_cast<std::size_t>(EditResult::Full)] > 0);
    CHECK(long_counts[static_cast<std::size_t>(O::CreatedWithEviction)] > 0);
    CHECK(long_counts[static_cast<std::size_t>(O::Dropped)] > 0);
    CHECK(long_counts[static_cast<std::size_t>(O::Removed)] > 0);
    CHECK(long_counts[static_cast<std::size_t>(O::Unchanged)] > 0);
    CHECK(trimmed > 0);
}
