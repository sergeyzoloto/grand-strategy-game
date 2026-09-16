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
        for (const float health : {0.0f, 50.0f, 100.0f}) {
            for (const float stress : {0.0f, 50.0f, 100.0f}) {
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
    const Character sick(CharacterId{2}, NameId{1}, Gender::Female, birth, CharacterInit{.health = 20.0f});
    const Character stressed(CharacterId{3}, NameId{1}, Gender::Female, birth, CharacterInit{.stress = 80.0f});
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
    const Character c(CharacterId{1}, NameId{1}, Gender::Male, Date{0}, CharacterInit{.health = 60.0f});
    for (int years = 0; years <= 110; years += 5) {
        CAPTURE(years);
        const double annual = annual_mortality(c, weeks_after_birth(Date{0}, years), config);
        const double compounded = 1.0 - std::pow(1.0 - weekly_mortality(annual), WEEKS_PER_YEAR);
        CHECK(std::abs(compounded - annual) <= 1e-5);
    }
}

TEST_CASE("mortality matches results recorded before the 0..100 condition rescale") {
    // Recorded with the 0..1 condition API at commit 4d6d58a. Sample conditions are
    // multiples of 20, exact in both the old (raw / 65535) and new (raw / 100) storage.
    struct Sample {
        Gender gender;
        int health;
        int stress;
        int age_weeks;
        double expected;
    };
    const Sample samples[] = {
        {Gender::Female, 100, 0, 0, 0.0024968776025398763},
        {Gender::Female, 100, 0, 1044, 0.010092844091220953},
        {Gender::Female, 100, 0, 2348, 0.056671914220568179},
        {Gender::Female, 100, 0, 3652, 0.28504314735083114},
        {Gender::Female, 100, 0, 4957, 0.85518775370263667},
        {Gender::Male, 100, 0, 0, 0.0027462222137641687},
        {Gender::Male, 100, 0, 1044, 0.011096508852077277},
        {Gender::Male, 100, 0, 2348, 0.062159374993675007},
        {Gender::Male, 100, 0, 3652, 0.30863432091136589},
        {Gender::Male, 100, 0, 4957, 0.88063257666050221},
        {Gender::Female, 60, 40, 0, 0.012306231558481949},
        {Gender::Female, 60, 40, 1044, 0.049002801882035721},
        {Gender::Female, 60, 40, 2348, 0.25096198183723817},
        {Gender::Female, 60, 40, 3652, 0.81022310541653464},
        {Gender::Female, 60, 40, 4957, 0.99993026646765881},
        {Gender::Male, 20, 80, 0, 0.065239056928179703},
        {Gender::Male, 20, 80, 1044, 0.23947559905089758},
        {Gender::Male, 20, 80, 2348, 0.79286434374425274},
        {Gender::Male, 20, 80, 3652, 0.99988315042046816},
        {Gender::Male, 20, 80, 4957, 1.0},
        {Gender::Female, 0, 0, 0, 0.048973966847216728},
        {Gender::Female, 0, 0, 1044, 0.18433385211596864},
        {Gender::Female, 0, 0, 2348, 0.69019528069496372},
        {Gender::Female, 0, 0, 3652, 0.998816598249751},
        {Gender::Female, 0, 0, 4957, 1.0},
        {Gender::Male, 60, 0, 0, 0.0090887660705096275},
        {Gender::Male, 60, 0, 1044, 0.036369769398527478},
        {Gender::Male, 60, 0, 2348, 0.19189989101696572},
        {Gender::Male, 60, 0, 3652, 0.70636268331980223},
        {Gender::Male, 60, 0, 4957, 0.99913870246106773},
        {Gender::Female, 100, 80, 0, 0.0055484028271668533},
        {Gender::Female, 100, 80, 1044, 0.022323224843865858},
        {Gender::Female, 100, 80, 2348, 0.12176458600041626},
        {Gender::Female, 100, 80, 3652, 0.52609226130112186},
        {Gender::Female, 100, 80, 4957, 0.98643756079991896},
        {Gender::Male, 0, 40, 0, 0.079097651118776327},
        {Gender::Male, 0, 40, 1044, 0.28420068847114854},
        {Gender::Male, 0, 40, 2348, 0.85382543729414007},
        {Gender::Male, 0, 40, 3652, 0.99998426057199286},
        {Gender::Male, 0, 40, 4957, 1.0},
    };
    const MortalityConfig config{};
    for (const Sample& s : samples) {
        CAPTURE(s.health);
        CAPTURE(s.stress);
        CAPTURE(s.age_weeks);
        const Character c(CharacterId{1}, NameId{1}, s.gender, Date{0},
                          CharacterInit{.health = static_cast<float>(s.health),
                                        .stress = static_cast<float>(s.stress)});
        CHECK(std::abs(annual_mortality(c, Date{s.age_weeks}, config) - s.expected) <= 1e-6);
    }
}
