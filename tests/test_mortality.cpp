#include <doctest.h>

#include <cmath>

#include "sim/character.hpp"
#include "sim/mortality.hpp"

using namespace sim;

namespace {

Date weeks_after_birth(Date birth, double years) {
    return Date{birth.weeks + static_cast<int>(std::lround(years * WEEKS_PER_YEAR))};
}

} // namespace

TEST_CASE("annual mortality stays in [0, 1] across ages, genders, health and stress") {
    const MortalityConfig config{};
    const Date birth{-2000};
    for (const Gender gender : {Gender::Female, Gender::Male}) {
        for (const float health : {0.0f, 0.5f, 1.0f}) {
            for (const float stress : {0.0f, 0.5f, 1.0f}) {
                const Character c(CharacterId{1}, NameId{1}, gender, birth,
                                  CharacterInit{.health = health, .stress = stress});
                for (const double years : {-10.0, 0.0, 1.0, 20.0, 60.0, 100.0, 150.0, 500.0, 100000.0}) {
                    CAPTURE(years);
                    const double p = annual_mortality(c, weeks_after_birth(birth, years), config);
                    CHECK(p >= 0.0);
                    CHECK(p <= 1.0);
                }
            }
        }
    }
}

TEST_CASE("annual mortality grows with age under the default config") {
    const MortalityConfig config{};
    const Date birth{0};
    const Character c(CharacterId{1}, NameId{1}, Gender::Female, birth, CharacterInit{});
    double previous = annual_mortality(c, birth, config);
    for (int years = 1; years <= 100; ++years) {
        CAPTURE(years);
        const double p = annual_mortality(c, weeks_after_birth(birth, years), config);
        CHECK(p > previous);
        previous = p;
    }
    MESSAGE("annual mortality at 20: " << annual_mortality(c, weeks_after_birth(birth, 20), config)
            << ", 60: " << annual_mortality(c, weeks_after_birth(birth, 60), config)
            << ", 80: " << annual_mortality(c, weeks_after_birth(birth, 80), config));
}

TEST_CASE("before birth mortality is 0") {
    const Character c(CharacterId{1}, NameId{1}, Gender::Male, Date{100}, CharacterInit{});
    CHECK(annual_mortality(c, Date{0}, MortalityConfig{}) == 0.0);
}

TEST_CASE("worse health, more stress and male gender raise mortality") {
    const MortalityConfig config{};
    const Date birth{0};
    const Date now = weeks_after_birth(birth, 40);
    const Character base(CharacterId{1}, NameId{1}, Gender::Female, birth, CharacterInit{});
    const Character sick(CharacterId{2}, NameId{1}, Gender::Female, birth, CharacterInit{.health = 0.2f});
    const Character stressed(CharacterId{3}, NameId{1}, Gender::Female, birth, CharacterInit{.stress = 0.8f});
    const Character male(CharacterId{4}, NameId{1}, Gender::Male, birth, CharacterInit{});
    const double p = annual_mortality(base, now, config);
    CHECK(annual_mortality(sick, now, config) > p);
    CHECK(annual_mortality(stressed, now, config) > p);
    CHECK(annual_mortality(male, now, config) > p);
}

TEST_CASE("weekly probability compounds back to the annual one") {
    for (const double annual : {0.0, 1e-9, 1e-6, 1e-3, 0.01, 0.1, 0.3, 0.5, 0.9, 0.999, 1.0}) {
        CAPTURE(annual);
        const double weekly = weekly_mortality(annual);
        CHECK(weekly >= 0.0);
        CHECK(weekly <= annual);
        const double compounded = 1.0 - std::pow(1.0 - weekly, WEEKS_PER_YEAR);
        CHECK(std::abs(compounded - annual) <= 1e-5);
    }

    const MortalityConfig config{};
    const Character c(CharacterId{1}, NameId{1}, Gender::Male, Date{0}, CharacterInit{.health = 0.6f});
    for (int years = 0; years <= 110; years += 5) {
        CAPTURE(years);
        const double annual = annual_mortality(c, weeks_after_birth(Date{0}, years), config);
        const double compounded = 1.0 - std::pow(1.0 - weekly_mortality(annual), WEEKS_PER_YEAR);
        CHECK(std::abs(compounded - annual) <= 1e-5);
    }
}
