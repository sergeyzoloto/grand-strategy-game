#include <doctest.h>

#include <climits>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

#include "sim/character.hpp"

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

// ---- helpers -----------------------------------------------------------------

namespace {

Character make_default() {
    return Character(CharacterId{1}, NameId{2}, Gender::Male, Date{-1000}, CharacterInit{});
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
}

} // namespace

// ---- layout and construction -------------------------------------------------

TEST_CASE("sizeof(Character)") {
    MESSAGE("sizeof(Character) = " << sizeof(Character) << ", alignof = " << alignof(Character));
    CHECK(sizeof(Character) == 32);
}

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
    CHECK(c.strength() == init.strength);
    CHECK(c.intelligence() == init.intelligence);
    CHECK(c.stability() == init.stability);
    CHECK(c.openness() == init.openness);
    CHECK(c.extraversion() == init.extraversion);
    CHECK(c.conscientiousness() == init.conscientiousness);
    CHECK(c.agreeableness() == init.agreeableness);
    CHECK(c.attractiveness() == init.attractiveness);
    CHECK(c.height() == init.height);
    CHECK(c.shape() == init.shape);
    CHECK(c.charisma() == init.charisma);
}

TEST_CASE("non-default init values are applied and clamped") {
    const Character c(CharacterId{5}, NameId{6}, Gender::Female, Date{0},
                      CharacterInit{.health = 25.0f, .strength = 150, .openness = -100, .charisma = INT_MIN});
    CHECK(c.health() == 25.0f);
    CHECK(c.strength() == 100);  // clamped
    CHECK(c.openness() == -100); // exact
    CHECK(c.charisma() == -100); // clamped
    CHECK(c.capacity() == 100.0f); // untouched default
    CHECK(c.intelligence() == 0);
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
// NaN is asserted in debug builds, so this runs in release only.
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
