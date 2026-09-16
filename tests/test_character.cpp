#include <doctest.h>

#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "sim/character.hpp"

#include "test_support.hpp"

using namespace sim;

static_assert(std::is_trivially_copyable_v<Character>);

// ---- compile-time proof: bipolar mutators accept integers only ---------------

template<class T>
concept CanSetStrength = requires(Character& c, T v) { c.set_strength(v); };
template<class T>
concept CanAddStrength = requires(Character& c, T v) { c.add_strength(v); };
template<class T>
concept CanSetCharisma = requires(Character& c, T v) { c.set_charisma(v); };
template<class T>
concept CanAddCharisma = requires(Character& c, T v) { c.add_charisma(v); };
template<class T>
concept CanSetOpenness = requires(Character& c, T v) { c.set_openness(v); };
template<class T>
concept CanAddOpenness = requires(Character& c, T v) { c.add_openness(v); };

template<class T>
concept AcceptedEverywhere = CanSetStrength<T> && CanAddStrength<T> && CanSetCharisma<T>
                          && CanAddCharisma<T> && CanSetOpenness<T> && CanAddOpenness<T>;
template<class T>
concept RejectedEverywhere = !CanSetStrength<T> && !CanAddStrength<T> && !CanSetCharisma<T>
                          && !CanAddCharisma<T> && !CanSetOpenness<T> && !CanAddOpenness<T>;

static_assert(AcceptedEverywhere<int>);
static_assert(AcceptedEverywhere<unsigned>);
static_assert(AcceptedEverywhere<std::int8_t>);
static_assert(AcceptedEverywhere<std::uint8_t>);
static_assert(AcceptedEverywhere<short>);
static_assert(AcceptedEverywhere<long>);
static_assert(AcceptedEverywhere<std::int64_t>);
static_assert(AcceptedEverywhere<std::uint64_t>);

static_assert(RejectedEverywhere<float>);
static_assert(RejectedEverywhere<double>);
static_assert(RejectedEverywhere<long double>);
static_assert(RejectedEverywhere<bool>);
static_assert(RejectedEverywhere<char>);
static_assert(RejectedEverywhere<char8_t>);
static_assert(RejectedEverywhere<wchar_t>);

template<class T>
concept CanSetReputation = requires(Character& c, T v) { c.set_reputation(v); };
template<class T>
concept CanAddReputation = requires(Character& c, T v) { c.add_reputation(v); };
template<class T>
concept CanInitReputation = requires(T v) { CharacterInit{.reputation = v}; };

static_assert(CanSetReputation<int> && CanAddReputation<std::int64_t> && CanInitReputation<unsigned>);
static_assert(!CanSetReputation<float> && !CanAddReputation<double> && !CanInitReputation<float>);
static_assert(!CanSetReputation<bool> && !CanInitReputation<char>);
static_assert(sizeof(Character) == 1352);

// Designated initializers are a second way into bipolar fields; they must accept
// and reject exactly the same argument types as the mutators.
template<class T>
concept CanInitOpenness = requires(T v) { CharacterInit{.openness = v}; };
template<class T>
concept CanInitCharisma = requires(T v) { CharacterInit{.strength = v, .charisma = v}; };

static_assert(CanInitOpenness<int> && CanInitCharisma<int>);
static_assert(CanInitOpenness<std::int64_t> && CanInitCharisma<std::int64_t>);
static_assert(CanInitOpenness<unsigned> && CanInitCharisma<unsigned>);
static_assert(CanInitOpenness<std::uint64_t> && CanInitOpenness<std::int8_t>);

static_assert(!CanInitOpenness<float> && !CanInitCharisma<float>);
static_assert(!CanInitOpenness<double> && !CanInitCharisma<double>);
static_assert(!CanInitOpenness<bool> && !CanInitCharisma<bool>);
static_assert(!CanInitOpenness<char> && !CanInitCharisma<char>);

static_assert(std::is_convertible_v<int, BipolarInit>);
static_assert(!std::is_convertible_v<float, BipolarInit>);
static_assert(!std::is_convertible_v<double, BipolarInit>);
static_assert(!std::is_convertible_v<bool, BipolarInit>);
static_assert(!std::is_convertible_v<char, BipolarInit>);

// The wrapper clamps at compile time too, and never changes Character's layout.
static_assert(BipolarInit{}.value() == 0);
static_assert(BipolarInit{150}.value() == 100);
static_assert(BipolarInit{-150}.value() == -100);
static_assert(BipolarInit{std::numeric_limits<std::uint64_t>::max()}.value() == 100);
static_assert(BipolarInit{std::numeric_limits<std::int64_t>::min()}.value() == -100);

// ---- helpers -----------------------------------------------------------------

namespace {

Character make_default() {
    return make_character(NameId{2}, Gender::Male, Date{-1000}, CharacterInit{});
}

// Non-constexpr pass-throughs: their results are never constant expressions, so
// the init tests exercise the non-constant path (narrowing rules differ for constants).
int runtime_int(int v) {
    return v;
}

std::int64_t runtime_int64(std::int64_t v) {
    return v;
}

// Calls visit(name, get, set, add) for every bipolar field.
template<class Visit>
void for_each_bipolar(Visit&& visit) {
    visit("strength", [](const Character& c) { return c.strength(); },
          [](Character& c, auto v) { c.set_strength(v); }, [](Character& c, auto d) { c.add_strength(d); });
    visit("intelligence", [](const Character& c) { return c.intelligence(); },
          [](Character& c, auto v) { c.set_intelligence(v); }, [](Character& c, auto d) { c.add_intelligence(d); });
    visit("stability", [](const Character& c) { return c.stability(); },
          [](Character& c, auto v) { c.set_stability(v); }, [](Character& c, auto d) { c.add_stability(d); });
    visit("openness", [](const Character& c) { return c.openness(); },
          [](Character& c, auto v) { c.set_openness(v); }, [](Character& c, auto d) { c.add_openness(d); });
    visit("extraversion", [](const Character& c) { return c.extraversion(); },
          [](Character& c, auto v) { c.set_extraversion(v); }, [](Character& c, auto d) { c.add_extraversion(d); });
    visit("conscientiousness", [](const Character& c) { return c.conscientiousness(); },
          [](Character& c, auto v) { c.set_conscientiousness(v); },
          [](Character& c, auto d) { c.add_conscientiousness(d); });
    visit("agreeableness", [](const Character& c) { return c.agreeableness(); },
          [](Character& c, auto v) { c.set_agreeableness(v); }, [](Character& c, auto d) { c.add_agreeableness(d); });
    visit("attractiveness", [](const Character& c) { return c.attractiveness(); },
          [](Character& c, auto v) { c.set_attractiveness(v); },
          [](Character& c, auto d) { c.add_attractiveness(d); });
    visit("height", [](const Character& c) { return c.height(); },
          [](Character& c, auto v) { c.set_height(v); }, [](Character& c, auto d) { c.add_height(d); });
    visit("shape", [](const Character& c) { return c.shape(); },
          [](Character& c, auto v) { c.set_shape(v); }, [](Character& c, auto d) { c.add_shape(d); });
    visit("charisma", [](const Character& c) { return c.charisma(); },
          [](Character& c, auto v) { c.set_charisma(v); }, [](Character& c, auto d) { c.add_charisma(d); });
    visit("reputation", [](const Character& c) { return c.reputation(); },
          [](Character& c, auto v) { c.set_reputation(v); }, [](Character& c, auto d) { c.add_reputation(d); });
}

} // namespace

// ---- layout and construction -------------------------------------------------

TEST_CASE("construction stores identity fields") {
    const Character c = make_default();
    CHECK(c.id() == CharacterId{1});
    CHECK(c.name() == NameId{2});
    CHECK(c.gender() == Gender::Male);
    CHECK(c.birth() == Date{-1000});
}

TEST_CASE("defaults match the init struct") {
    const CharacterInit init{};
    const Character c = make_default();
    CHECK(c.health() == init.health);
    CHECK(c.stress() == init.stress);
    CHECK(c.capacity() == init.capacity);
    CHECK(c.strength() == init.strength.value());
    CHECK(c.intelligence() == init.intelligence.value());
    CHECK(c.stability() == init.stability.value());
    CHECK(c.openness() == init.openness.value());
    CHECK(c.extraversion() == init.extraversion.value());
    CHECK(c.conscientiousness() == init.conscientiousness.value());
    CHECK(c.agreeableness() == init.agreeableness.value());
    CHECK(c.attractiveness() == init.attractiveness.value());
    CHECK(c.height() == init.height.value());
    CHECK(c.shape() == init.shape.value());
    CHECK(c.charisma() == init.charisma.value());
    CHECK(c.reputation() == init.reputation.value());
    CHECK(c.reputation() == 0);
}

TEST_CASE("non-default init values are applied and clamped") {
    const Character c = make_character(NameId{6}, Gender::Female, Date{0},
                      CharacterInit{.health = 25.0f, .strength = 150, .openness = -100, .charisma = INT_MIN});
    CHECK(c.health() == 25.0f);
    CHECK(c.strength() == 100);  // clamped
    CHECK(c.openness() == -100); // exact
    CHECK(c.charisma() == -100); // clamped
    CHECK(c.capacity() == 100.0f); // untouched default
    CHECK(c.intelligence() == 0);
}

TEST_CASE("reputation: init values clamp and arrive") {
    const Character c = make_character(NameId{3}, Gender::Female, Date{0},
                                       CharacterInit{.charisma = 5, .reputation = runtime_int(-150)});
    CHECK(c.reputation() == -100);
    const Character d = make_character(NameId{4}, Gender::Male, Date{0}, CharacterInit{.reputation = 42});
    CHECK(d.reputation() == 42);
    CHECK(d.charisma() == 0);
}

TEST_CASE("init struct clamps non-constant out-of-range integers") {
    const int high = runtime_int(150);
    const int low = runtime_int(-150);
    const Character c = make_character(NameId{7}, Gender::Female, Date{0},
                      CharacterInit{.strength = high, .intelligence = low, .stability = high,
                                    .openness = low, .extraversion = high, .conscientiousness = low,
                                    .agreeableness = high, .attractiveness = low, .height = high,
                                    .shape = low, .charisma = high});
    CHECK(c.strength() == 100);
    CHECK(c.intelligence() == -100);
    CHECK(c.stability() == 100);
    CHECK(c.openness() == -100);
    CHECK(c.extraversion() == 100);
    CHECK(c.conscientiousness() == -100);
    CHECK(c.agreeableness() == 100);
    CHECK(c.attractiveness() == -100);
    CHECK(c.height() == 100);
    CHECK(c.shape() == -100);
    CHECK(c.charisma() == 100);

    // A wide integer whose low bits are in range must clamp, not wrap.
    const std::int64_t wide = runtime_int64((std::int64_t{1} << 40) + 5);
    const Character w = make_character(NameId{8}, Gender::Male, Date{0},
                      CharacterInit{.openness = wide, .charisma = -wide});
    CHECK(w.openness() == 100);
    CHECK(w.charisma() == -100);
}

// ---- condition fields (0..100 in hundredths) --------------------------------

TEST_CASE("condition defaults are health 100, stress 0, capacity 100") {
    const Character c = make_default();
    CHECK(c.health() == 100.0f);
    CHECK(c.stress() == 0.0f);
    CHECK(c.capacity() == 100.0f);
}

TEST_CASE("condition set_ stores bounds exactly and saturates") {
    Character c = make_default();
    const float inf = std::numeric_limits<float>::infinity();

    c.set_health(0.0f);
    CHECK(c.health() == CONDITION_MIN);
    c.set_health(100.0f);
    CHECK(c.health() == CONDITION_MAX);
    c.set_health(150.0f);
    CHECK(c.health() == 100.0f);
    c.set_health(-150.0f);
    CHECK(c.health() == 0.0f);

    c.set_stress(inf);
    CHECK(c.stress() == 100.0f);
    c.set_stress(-inf);
    CHECK(c.stress() == 0.0f);
    c.set_stress(std::numeric_limits<float>::max());
    CHECK(c.stress() == 100.0f);

    c.set_capacity(-0.001f);
    CHECK(c.capacity() == 0.0f);
    c.set_capacity(100.001f);
    CHECK(c.capacity() == 100.0f);
}

TEST_CASE("condition set_ rounds to the nearest hundredth") {
    Character c = make_default();
    c.set_health(12.344f);
    CHECK(c.health() == 12.34f);
    c.set_health(12.346f);
    CHECK(c.health() == 12.35f);
}

TEST_CASE("condition round trip of every raw value 0..10000") {
    Character c = make_default();
    for (int raw = 0; raw <= 10000; ++raw) {
        const auto value = static_cast<float>(static_cast<double>(raw) / 100.0);
        c.set_health(value);
        c.set_stress(value);
        c.set_capacity(value);
        if (c.health() != value || c.stress() != value || c.capacity() != value) {
            FAIL_CHECK("round trip failed at raw " << raw);
        }
    }
}

TEST_CASE("condition add_ saturates at 0 and 100") {
    Character c = make_default();
    const float inf = std::numeric_limits<float>::infinity();

    c.add_health(0.5f); // default 100
    CHECK(c.health() == 100.0f);
    c.add_stress(-0.5f); // default 0
    CHECK(c.stress() == 0.0f);
    c.add_stress(75.0f);
    c.add_stress(75.0f);
    CHECK(c.stress() == 100.0f);
    c.add_capacity(-500.0f);
    CHECK(c.capacity() == 0.0f);

    c.set_health(50.0f);
    c.add_health(inf);
    CHECK(c.health() == 100.0f);
    c.add_health(-inf);
    CHECK(c.health() == 0.0f);
    c.set_health(50.0f);
    c.add_health(std::numeric_limits<float>::max());
    CHECK(c.health() == 100.0f);
    c.add_health(std::numeric_limits<float>::lowest());
    CHECK(c.health() == 0.0f);
}

TEST_CASE("500 x add_health(0.1) gives exactly raw 5000") {
    Character c = make_default();
    c.set_health(0.0f);
    for (int i = 0; i < 500; ++i) {
        c.add_health(0.1f);
    }
    CHECK(c.health() == 50.0f); // only raw 5000 reads back as exactly 50
}

TEST_CASE("add_(d) then add_(-d) restores the value away from the bounds") {
    Character c = make_default();
    for (const float start : {0.37f, 12.34f, 50.0f, 99.63f}) {
        CAPTURE(start);
        c.set_health(start);
        const float before = c.health();
        c.add_health(0.37f);
        c.add_health(-0.37f);
        CHECK(c.health() == before);
        c.add_health(-0.37f);
        c.add_health(0.37f);
        CHECK(c.health() == before);
    }
}

// Resolution rule: add_ rounds the delta to whole hundredths, so a delta below half
// a step rounds away. The exact 0.005 boundary is deliberately not tested: it depends
// on float representation.
TEST_CASE("resolution rule: add_health(0.004) is a no-op, add_health(0.006) is one step") {
    Character c = make_default();
    c.set_health(50.0f);
    c.add_health(0.004f);
    CHECK(c.health() == 50.0f);
    c.add_health(-0.004f);
    CHECK(c.health() == 50.0f);
    c.add_health(0.006f);
    CHECK(c.health() == 50.01f);
    c.add_health(-0.006f);
    CHECK(c.health() == 50.0f);
}

#ifdef NDEBUG
// NaN is asserted in debug builds, so these run in the release test build only.
TEST_CASE("condition NaN in CharacterInit leaves the default value") {
    const float nan = std::numeric_limits<float>::quiet_NaN();
    const Character c = make_character(NameId{9}, Gender::Female, Date{0},
                      CharacterInit{.health = nan, .stress = nan, .capacity = nan});
    CHECK(c.health() == 100.0f);
    CHECK(c.stress() == 0.0f);
    CHECK(c.capacity() == 100.0f);
}

TEST_CASE("condition NaN leaves the field unchanged") {
    Character c = make_default();
    const float nan = std::numeric_limits<float>::quiet_NaN();
    c.set_health(42.5f);
    c.set_health(nan);
    CHECK(c.health() == 42.5f);
    c.add_health(nan);
    CHECK(c.health() == 42.5f);
}
#endif

// ---- bipolar fields ----------------------------------------------------------

TEST_CASE("bipolar set_ clamps +-150 and stores +-100 exactly") {
    for_each_bipolar([](const char* name, auto get, auto set, auto) {
        CAPTURE(name);
        Character c = make_default();
        set(c, 150);
        CHECK(get(c) == 100);
        set(c, -150);
        CHECK(get(c) == -100);
        set(c, 100);
        CHECK(get(c) == 100);
        set(c, -100);
        CHECK(get(c) == -100);
        set(c, 0);
        CHECK(get(c) == 0);
    });
}

TEST_CASE("bipolar set_ round trips every value in -100..+100") {
    for_each_bipolar([](const char* name, auto get, auto set, auto) {
        CAPTURE(name);
        Character c = make_default();
        for (int v = BIPOLAR_MIN; v <= BIPOLAR_MAX; ++v) {
            set(c, v);
            if (get(c) != v) {
                FAIL_CHECK("round trip failed at " << v);
            }
        }
    });
}

TEST_CASE("bipolar set_ clamps extreme values of every integer width") {
    for_each_bipolar([](const char* name, auto get, auto set, auto) {
        CAPTURE(name);
        Character c = make_default();
        set(c, INT_MAX);
        CHECK(get(c) == 100);
        set(c, INT_MIN);
        CHECK(get(c) == -100);
        set(c, std::numeric_limits<std::int64_t>::min());
        CHECK(get(c) == -100);
        set(c, std::numeric_limits<std::int64_t>::max());
        CHECK(get(c) == 100);
        set(c, std::numeric_limits<std::uint64_t>::max());
        CHECK(get(c) == 100);
        set(c, std::numeric_limits<unsigned>::max());
        CHECK(get(c) == 100);
        set(c, std::int8_t{-128});
        CHECK(get(c) == -100);
        set(c, std::uint8_t{255});
        CHECK(get(c) == 100);
        set(c, std::uint64_t{42});
        CHECK(get(c) == 42);
    });
}

TEST_CASE("bipolar add_ adds exactly and saturates at both bounds") {
    for_each_bipolar([](const char* name, auto get, auto set, auto add) {
        CAPTURE(name);
        Character c = make_default();
        add(c, 7);
        CHECK(get(c) == 7);
        add(c, -10);
        CHECK(get(c) == -3);
        set(c, 95);
        add(c, 10);
        CHECK(get(c) == 100);
        add(c, 1);
        CHECK(get(c) == 100);
        set(c, -95);
        add(c, -10);
        CHECK(get(c) == -100);
        add(c, -1);
        CHECK(get(c) == -100);
        add(c, 200);
        CHECK(get(c) == 100);
        add(c, -200);
        CHECK(get(c) == -100);
        add(c, 201);
        CHECK(get(c) == 100);
        add(c, std::uint64_t{5}); // unsigned delta does not wrap
        CHECK(get(c) == 100);
    });
}

TEST_CASE("bipolar add_ with extreme deltas saturates without overflow") {
    for_each_bipolar([](const char* name, auto get, auto set, auto add) {
        CAPTURE(name);
        for (const int start : {-100, 0, 100}) {
            CAPTURE(start);
            Character c = make_default();

            set(c, start);
            add(c, INT_MAX);
            CHECK(get(c) == 100);
            set(c, start);
            add(c, INT_MIN);
            CHECK(get(c) == -100);

            set(c, start);
            add(c, std::numeric_limits<std::int64_t>::max());
            CHECK(get(c) == 100);
            set(c, start);
            add(c, std::numeric_limits<std::int64_t>::min());
            CHECK(get(c) == -100);

            set(c, start);
            add(c, std::numeric_limits<std::uint64_t>::max());
            CHECK(get(c) == 100);
            set(c, start);
            add(c, std::numeric_limits<unsigned>::max());
            CHECK(get(c) == 100);
        }
    });
}

TEST_CASE("bipolar add_ applies deltas beyond +-100 in full") {
    // The delta clamp is +-200, the full span of the scale, not +-100.
    for_each_bipolar([](const char* name, auto get, auto set, auto add) {
        CAPTURE(name);
        Character c = make_default();
        set(c, -100);
        add(c, 150);
        CHECK(get(c) == 50);
        set(c, 100);
        add(c, -150);
        CHECK(get(c) == -50);
    });
}
