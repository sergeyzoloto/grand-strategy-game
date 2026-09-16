#pragma once

#include "sim/character.hpp"
#include "sim/date.hpp"
#include "sim/ids.hpp"
#include "sim/noise.hpp"
#include "sim/opinion_config.hpp"
#include "sim/stance_table.hpp"

namespace sim {

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

// ---- opinions with strong records -----------------------------------------------------

// A weak opinion plus the remembered deviation. total = clamp(weak.total + deviation).
struct OpinionBreakdown {
    WeakOpinionBreakdown weak; // weak terms and the clamped weak total
    double deviation = 0.0;    // strong deviation at `now` in value units; 0 without a record
    double total = 0.0;        // clamp(weak.total + deviation, -100, +100)
};

// opinion(A, X) = clamp(weak(A -> X) + dev(now), -100, +100). Without a record it equals
// the weak opinion. Reading never writes; nothing is stored or cached.
[[nodiscard]] OpinionBreakdown opinion_breakdown(const Character& a, const Character& b, const StanceTable& stances,
                                                 const OpinionConfig& config, WorldSeed seed, Date now) noexcept;
[[nodiscard]] double opinion(const Character& a, const Character& b, const StanceTable& stances,
                             const OpinionConfig& config, WorldSeed seed, Date now) noexcept;
[[nodiscard]] OpinionBreakdown opinion_breakdown(const Character& a, TargetId target, const StanceTable& stances,
                                                 const OpinionConfig& config, WorldSeed seed, Date now) noexcept;
[[nodiscard]] double opinion(const Character& a, TargetId target, const StanceTable& stances,
                             const OpinionConfig& config, WorldSeed seed, Date now) noexcept;

} // namespace sim
