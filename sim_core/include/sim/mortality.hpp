#pragma once

#include "sim/character.hpp"
#include "sim/date.hpp"

namespace sim {

// Coefficients of the annual mortality model. All values are PLACEHOLDERS.
//
// hazard(age) = gompertz_a * exp(gompertz_b * age)
//             * gender_multiplier
//             * exp(health_weight * (1 - health / 100))
//             * exp(stress_weight * stress / 100)
// annual probability = 1 - exp(-hazard)
//
// Known gap: plain Gompertz has no infant/child mortality peak.
struct MortalityConfig {
    double gompertz_a = 0.0025;      // PLACEHOLDER: annual hazard at age 0, >= 0
    double gompertz_b = 0.07;        // PLACEHOLDER: hazard growth per year of age
    double female_multiplier = 1.0;  // PLACEHOLDER: hazard multiplier, >= 0
    double male_multiplier = 1.1;    // PLACEHOLDER: hazard multiplier, >= 0
    double health_weight = 3.0;      // PLACEHOLDER: log-hazard added at health 0
    double stress_weight = 1.0;      // PLACEHOLDER: log-hazard added at stress 100
};

// Age in years at `now`; negative if `now` is before birth.
[[nodiscard]] double age_years(const Character& character, Date now) noexcept;

// Probability of dying within one year, in [0, 1]. Returns 0 before birth.
[[nodiscard]] double annual_mortality(const Character& character, Date now, const MortalityConfig& config) noexcept;

// Converts an annual probability to a per-week probability with the same
// compounded risk: 1 - (1 - annual)^(1 / WEEKS_PER_YEAR). Input is clamped to [0, 1].
[[nodiscard]] double weekly_mortality(double annual) noexcept;

} // namespace sim
