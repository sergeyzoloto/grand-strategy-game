#include "sim/mortality.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>

namespace sim {

namespace {

double gender_multiplier(Gender gender, const MortalityConfig& config) noexcept {
    // No default: adding a Gender value triggers -Wswitch until it is handled here.
    switch (gender) {
    case Gender::Female:
        return config.female_multiplier;
    case Gender::Male:
        return config.male_multiplier;
    }
    assert(false && "unhandled Gender");
    return 1.0;
}

} // namespace

double age_years(const Character& character, Date now) noexcept {
    return weeks_to_years(weeks_between(character.birth(), now));
}

double annual_mortality(const Character& character, Date now, const MortalityConfig& config) noexcept {
    assert(config.gompertz_a >= 0.0);
    assert(config.female_multiplier >= 0.0 && config.male_multiplier >= 0.0);

    const double age = age_years(character, now);
    if (age < 0.0) {
        return 0.0;
    }

    // Condition scales are 0..100; the model works on fractions 0..1.
    const double health = static_cast<double>(character.health()) / static_cast<double>(CONDITION_MAX);
    const double stress = static_cast<double>(character.stress()) / static_cast<double>(CONDITION_MAX);

    // Summed in log space so exp(b * age) cannot overflow into inf * 0 = NaN.
    const double log_hazard = std::log(config.gompertz_a)
                            + config.gompertz_b * age
                            + std::log(gender_multiplier(character.gender(), config))
                            + config.health_weight * (1.0 - health)
                            + config.stress_weight * stress;
    const double hazard = std::exp(log_hazard);
    const double p = -std::expm1(-hazard);

    // NaN (only from an invalid config) maps to 0.
    if (!(p >= 0.0)) {
        return 0.0;
    }
    return std::min(p, 1.0);
}

double weekly_mortality(double annual) noexcept {
    assert(!std::isnan(annual));
    if (!(annual > 0.0)) {
        return 0.0;
    }
    if (annual >= 1.0) {
        return 1.0;
    }
    return -std::expm1(std::log1p(-annual) / WEEKS_PER_YEAR);
}

} // namespace sim
