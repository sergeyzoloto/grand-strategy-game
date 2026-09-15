#include <doctest.h>

#include <cmath>
#include <limits>
#include <type_traits>

#include "sim/character.hpp"

using namespace sim;

static_assert(std::is_trivially_copyable_v<Character>);

namespace {

Character make_default() {
    return Character(CharacterId{1}, NameId{2}, Gender::Male, Date{-1000}, CharacterInit{});
}

} // namespace

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

TEST_CASE("non-default init values are applied") {
    const Character c(CharacterId{5}, NameId{6}, Gender::Female, Date{0},
                      CharacterInit{.health = 0.25f, .openness = -1.0f, .charisma = 3.0f});
    CHECK(c.health() == doctest::Approx(0.25).epsilon(1e-4));
    CHECK(c.openness() == -1.0f);
    CHECK(c.charisma() == 1.0f); // saturated
    CHECK(c.capacity() == 1.0f); // untouched default
}

TEST_CASE("set_ saturates") {
    Character c = make_default();
    const float inf = std::numeric_limits<float>::infinity();

    c.set_health(2.0f);
    CHECK(c.health() == 1.0f);
    c.set_stress(-3.0f);
    CHECK(c.stress() == 0.0f);
    c.set_capacity(inf);
    CHECK(c.capacity() == 1.0f);

    c.set_strength(2.0f);
    CHECK(c.strength() == 1.0f);
    c.set_intelligence(-2.0f);
    CHECK(c.intelligence() == -1.0f);
    c.set_stability(-inf);
    CHECK(c.stability() == -1.0f);
    c.set_openness(10.0f);
    CHECK(c.openness() == 1.0f);
    c.set_extraversion(-10.0f);
    CHECK(c.extraversion() == -1.0f);
    c.set_conscientiousness(1.5f);
    CHECK(c.conscientiousness() == 1.0f);
    c.set_agreeableness(-1.5f);
    CHECK(c.agreeableness() == -1.0f);
    c.set_attractiveness(inf);
    CHECK(c.attractiveness() == 1.0f);
    c.set_height(-1.01f);
    CHECK(c.height() == -1.0f);
    c.set_shape(1.01f);
    CHECK(c.shape() == 1.0f);
    c.set_charisma(-7.0f);
    CHECK(c.charisma() == -1.0f);
}

TEST_CASE("add_ saturates") {
    Character c = make_default();

    c.add_health(0.5f); // default 1.0
    CHECK(c.health() == 1.0f);
    c.add_stress(-0.5f); // default 0.0
    CHECK(c.stress() == 0.0f);
    c.add_stress(0.75f);
    c.add_stress(0.75f);
    CHECK(c.stress() == 1.0f);
    c.add_capacity(-5.0f);
    CHECK(c.capacity() == 0.0f);

    for (int i = 0; i < 4; ++i) {
        c.add_strength(0.4f);
        c.add_intelligence(-0.4f);
        c.add_stability(0.4f);
        c.add_openness(-0.4f);
        c.add_extraversion(0.4f);
        c.add_conscientiousness(-0.4f);
        c.add_agreeableness(0.4f);
        c.add_attractiveness(-0.4f);
        c.add_height(0.4f);
        c.add_shape(-0.4f);
        c.add_charisma(0.4f);
    }
    CHECK(c.strength() == 1.0f);
    CHECK(c.intelligence() == -1.0f);
    CHECK(c.stability() == 1.0f);
    CHECK(c.openness() == -1.0f);
    CHECK(c.extraversion() == 1.0f);
    CHECK(c.conscientiousness() == -1.0f);
    CHECK(c.agreeableness() == 1.0f);
    CHECK(c.attractiveness() == -1.0f);
    CHECK(c.height() == 1.0f);
    CHECK(c.shape() == -1.0f);
    CHECK(c.charisma() == 1.0f);
}

TEST_CASE("resolution rule: add_strength(0.001) is a no-op") {
    Character c = make_default();
    c.add_strength(0.001f);
    CHECK(c.strength() == 0.0f);
    for (int i = 0; i < 500; ++i) {
        c.add_strength(0.001f);
    }
    CHECK(c.strength() == 0.0f);
}

TEST_CASE("accepted uint16 bias: 500 x add_health(0.001) lands within 0.5 +- 0.005") {
    Character c = make_default();
    c.set_health(0.0f);
    for (int i = 0; i < 500; ++i) {
        c.add_health(0.001f);
    }
    MESSAGE("health after 500 x 0.001 = " << c.health());
    CHECK(std::abs(static_cast<double>(c.health()) - 0.5) <= 0.005);
}
