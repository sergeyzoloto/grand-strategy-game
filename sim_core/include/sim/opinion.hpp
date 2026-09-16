#pragma once

#include "sim/character.hpp"
#include "sim/ids.hpp"
#include "sim/noise.hpp"
#include "sim/stance_table.hpp"

namespace sim {

// Coefficients of weak opinions. All values are PLACEHOLDERS.
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

// Terms of a weak opinion before clamping, plus the clamped total. For tuning and
// for opinion tooltips later. total = clamp(community + reputation + compat + noise).
struct WeakOpinionBreakdown {
    double community = 0.0;  // stance term from involvement shares
    double reputation = 0.0; // k_rep * reputation of the target (0 for non-person targets)
    double compat = 0.0;     // personality compatibility (0 for non-person targets)
    double noise = 0.0;      // scaled deterministic noise
    double total = 0.0;      // sum of the terms clamped to -100..+100
};

// Weak opinions are what a character thinks of another character, a community or a
// topic without personal history. They are computed, never stored, in double on the
// -100..+100 scale using only + - * / and comparisons (no exp, log or pow), so results
// are identical across platforms under the project's compiler flags. Rounding to whole
// numbers is the caller's choice. No function here allocates.

// Symmetric weighted closeness of the five personality traits, mapped from -k_compat
// (opposite extremes) to +k_compat (identical). Weights must be >= 0 with a positive
// sum (asserted); a zero sum returns 0.
[[nodiscard]] double compat(const Character& a, const Character& b, const OpinionConfig& config) noexcept;

// Linear in openness: openness_factor_min at -100, openness_factor_max at +100.
[[nodiscard]] double openness_factor(const Character& a, const OpinionConfig& config) noexcept;

// weak(A -> B) = sum_c sum_d share_A(c) * share_B(d) * stance(c -> d)
//              + k_rep * reputation(B) + compat(A, B) + k_noise * noise(A, B)
// Community shares use raw weights, divided once. a == b: debug assert, returns 0.
[[nodiscard]] WeakOpinionBreakdown weak_opinion_breakdown(const Character& a, const Character& b,
                                                          const StanceTable& stances, const OpinionConfig& config,
                                                          WorldSeed seed) noexcept;
[[nodiscard]] double weak_opinion(const Character& a, const Character& b, const StanceTable& stances,
                                  const OpinionConfig& config, WorldSeed seed) noexcept;

// weak(A -> T) = sum_c share_A(c) * stance(c -> T) + k_noise * openness_factor(A) * noise(A, T)
// for a community or topic target. An invalid target returns 0.
[[nodiscard]] WeakOpinionBreakdown weak_opinion_breakdown(const Character& a, TargetId target,
                                                          const StanceTable& stances, const OpinionConfig& config,
                                                          WorldSeed seed) noexcept;
[[nodiscard]] double weak_opinion(const Character& a, TargetId target, const StanceTable& stances,
                                  const OpinionConfig& config, WorldSeed seed) noexcept;

} // namespace sim
