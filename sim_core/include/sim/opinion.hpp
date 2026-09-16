#pragma once

#include "sim/character.hpp"
#include "sim/dead_record.hpp"
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

// weak(A -> D) for a dead character D:
//   sum_c share_A(c) * stance(c -> main_community(D)) + k_rep * reputation(D) + k_noise * noise(A, D)
// compat is 0. The noise inputs are the same as while D was alive. Without a main
// community the community term is 0. A dead holder can't be passed: DeadRecord is not a
// Character.
[[nodiscard]] WeakOpinionBreakdown weak_opinion_breakdown(const Character& a, const DeadRecord& d,
                                                          const StanceTable& stances, const OpinionConfig& config,
                                                          WorldSeed seed) noexcept;
[[nodiscard]] double weak_opinion(const Character& a, const DeadRecord& d, const StanceTable& stances,
                                  const OpinionConfig& config, WorldSeed seed) noexcept;

// ---- opinions with personal history -----------------------------------------------------

// A weak opinion plus the personal terms. total = clamp(weak.total + long_term + short_term),
// where weak.total is already clamped: personal history can pull an opinion back from a
// saturated community stance.
struct OpinionBreakdown {
    WeakOpinionBreakdown weak; // weak terms and the clamped weak total
    int long_term = 0;         // stored long-term value, -200..+200; 0 without an entry
    int short_term = 0;        // sum of active modifier effects; 0 without modifiers
    double total = 0.0;        // clamp(weak.total + long_term + short_term, -100, +100)
};

// opinion(A, X) = clamp(weak(A -> X) + long(A -> X) + short(A -> X), -100, +100). Without
// personal records it equals the weak opinion. Nothing depends on time; nothing is stored
// or cached.
[[nodiscard]] OpinionBreakdown opinion_breakdown(const Character& a, const Character& b, const StanceTable& stances,
                                                 const OpinionConfig& config, WorldSeed seed) noexcept;
[[nodiscard]] double opinion(const Character& a, const Character& b, const StanceTable& stances,
                             const OpinionConfig& config, WorldSeed seed) noexcept;
// A dead target: the dead-record weak opinion plus a's personal terms about it.
[[nodiscard]] OpinionBreakdown opinion_breakdown(const Character& a, const DeadRecord& d, const StanceTable& stances,
                                                 const OpinionConfig& config, WorldSeed seed) noexcept;
[[nodiscard]] double opinion(const Character& a, const DeadRecord& d, const StanceTable& stances,
                             const OpinionConfig& config, WorldSeed seed) noexcept;
[[nodiscard]] OpinionBreakdown opinion_breakdown(const Character& a, TargetId target, const StanceTable& stances,
                                                 const OpinionConfig& config, WorldSeed seed) noexcept;
[[nodiscard]] double opinion(const Character& a, TargetId target, const StanceTable& stances,
                             const OpinionConfig& config, WorldSeed seed) noexcept;

} // namespace sim
