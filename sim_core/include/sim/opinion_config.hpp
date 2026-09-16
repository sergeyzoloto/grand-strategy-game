#pragma once

namespace sim {

// Coefficients of weak opinions. All values are PLACEHOLDERS. Personal opinions (long
// values and modifiers) are whole numbers set by gameplay and have no coefficients here.
struct OpinionConfig {
    double k_rep = 0.25;               // PLACEHOLDER: opinion per reputation point of the target
    double k_compat = 20.0;            // PLACEHOLDER: compat range, -k_compat..+k_compat
    double w_stability = 1.0;          // PLACEHOLDER: compat weight, >= 0
    double w_openness = 1.0;           // PLACEHOLDER: compat weight, >= 0
    double w_extraversion = 1.0;       // PLACEHOLDER: compat weight, >= 0
    double w_conscientiousness = 1.0;  // PLACEHOLDER: compat weight, >= 0
    double w_agreeableness = 1.0;      // PLACEHOLDER: compat weight, >= 0
    double k_noise = 10.0;             // PLACEHOLDER: noise amplitude
    double openness_factor_min = 0.5;  // PLACEHOLDER: noise factor at openness -100
    double openness_factor_max = 1.5;  // PLACEHOLDER: noise factor at openness +100
};

} // namespace sim
