#pragma once

namespace sim {

// Coefficients of weak and strong opinions. All values are PLACEHOLDERS.
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

    // Strong opinions. Retention is a per-week factor applied as retention^weeks by
    // repeated squaring; for a half-life of h weeks, retention = 0.5^(1/h), computed offline.
    double retention_unstable = 0.98;  // PLACEHOLDER: retention at stability -100 (a grudge halves in ~34 weeks)
    double retention_stable = 0.95;    // PLACEHOLDER: retention at stability +100 (halves in ~13.5 weeks)
    double amplitude_unstable = 1.5;   // PLACEHOLDER: event delta multiplier at stability -100
    double amplitude_stable = 0.5;     // PLACEHOLDER: event delta multiplier at stability +100
    double plasticity = 0.1;           // PLACEHOLDER: share of the new short deviation absorbed into long
    double enter_threshold = 10.0;     // PLACEHOLDER: |d| needed to create a record
    double exit_threshold = 5.0;       // PLACEHOLDER: records below this |dev| are removed by maintain; < enter
};

} // namespace sim
