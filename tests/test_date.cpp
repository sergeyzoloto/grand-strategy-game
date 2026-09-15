#include <doctest.h>

#include <cmath>
#include <type_traits>

#include "sim/character.hpp"
#include "sim/date.hpp"
#include "sim/mortality.hpp"

using namespace sim;

static_assert(sizeof(Date) == 4);
static_assert(!std::is_convertible_v<int, Date>);

namespace {

Character make_born(Date birth) {
    return Character(CharacterId{1}, NameId{1}, Gender::Female, birth, CharacterInit{});
}

} // namespace

TEST_CASE("weeks_between and weeks_to_years") {
    CHECK(weeks_between(Date{10}, Date{25}) == 15);
    CHECK(weeks_between(Date{25}, Date{10}) == -15);
    CHECK(weeks_between(Date{-100}, Date{100}) == 200);
    CHECK(weeks_to_years(0) == 0.0);
    CHECK(weeks_to_years(-52) < 0.0);
}

TEST_CASE("age is 0 at birth and about 1.0 after WEEKS_PER_YEAR weeks") {
    const double one_week_in_years = 1.0 / WEEKS_PER_YEAR;
    for (const int birth_weeks : {0, 1234, -5000, -600000}) {
        CAPTURE(birth_weeks);
        const Date birth{birth_weeks};
        const Character c = make_born(birth);
        CHECK(age_years(c, birth) == 0.0);

        const auto year_later = Date{birth_weeks + static_cast<int>(std::lround(WEEKS_PER_YEAR))};
        CHECK(std::abs(age_years(c, year_later) - 1.0) <= one_week_in_years);
    }
}

TEST_CASE("negative birth dates work") {
    const Character c = make_born(Date{-52 * 30});
    const double age = age_years(c, Date{0});
    CHECK(age > 29.8);
    CHECK(age < 30.0);
    CHECK(age_years(c, Date{-52 * 31}) < 0.0);
}
