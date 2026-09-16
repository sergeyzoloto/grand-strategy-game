#include "sim/opinion.hpp"

#include "sim/personal_opinion.hpp"

#include <algorithm>
#include <array>
#include <span>
#include <cassert>
#include <cstdint>

namespace sim {

// A character's involvement always fits one batched stance query.
static_assert(INVOLVEMENT_CAP <= STANCE_BATCH_MAX_SOURCES);

namespace {

double clamp_opinion(double v) noexcept {
    return std::clamp(v, static_cast<double>(BIPOLAR_MIN), static_cast<double>(BIPOLAR_MAX));
}

double trait_distance(int a, int b) noexcept {
    return static_cast<double>(a > b ? a - b : b - a);
}

NoiseSubject noise_subject(TargetKind kind) noexcept {
    switch (kind) {
    case TargetKind::Community:
        return NoiseSubject::Community;
    case TargetKind::Topic:
        return NoiseSubject::Topic;
    }
    assert(false && "unhandled TargetKind");
    return NoiseSubject::Topic;
}

} // namespace

double compat(const Character& a, const Character& b, const OpinionConfig& config) noexcept {
    assert(config.w_stability >= 0.0 && config.w_openness >= 0.0 && config.w_extraversion >= 0.0
           && config.w_conscientiousness >= 0.0 && config.w_agreeableness >= 0.0);
    const double weight_sum = config.w_stability + config.w_openness + config.w_extraversion
                            + config.w_conscientiousness + config.w_agreeableness;
    assert(weight_sum > 0.0);
    if (!(weight_sum > 0.0)) {
        return 0.0;
    }
    const double distance = config.w_stability * trait_distance(a.stability(), b.stability())
                          + config.w_openness * trait_distance(a.openness(), b.openness())
                          + config.w_extraversion * trait_distance(a.extraversion(), b.extraversion())
                          + config.w_conscientiousness * trait_distance(a.conscientiousness(), b.conscientiousness())
                          + config.w_agreeableness * trait_distance(a.agreeableness(), b.agreeableness());
    // distance / (200 * weight_sum) is 0 for identical and 1 for opposite extremes.
    const double span = static_cast<double>(BIPOLAR_MAX - BIPOLAR_MIN);
    return config.k_compat * (1.0 - 2.0 * (distance / (span * weight_sum)));
}

double openness_factor(const Character& a, const OpinionConfig& config) noexcept {
    const double span = static_cast<double>(BIPOLAR_MAX - BIPOLAR_MIN);
    return config.openness_factor_min
         + (config.openness_factor_max - config.openness_factor_min)
               * (static_cast<double>(a.openness() - BIPOLAR_MIN) / span);
}

WeakOpinionBreakdown weak_opinion_breakdown(const Character& a, const Character& b, const StanceTable& stances,
                                            const OpinionConfig& config, WorldSeed seed) noexcept {
    assert(a.id() != b.id());
    if (a.id() == b.id()) {
        return {};
    }
    WeakOpinionBreakdown result;

    const int total_a = a.involvement_total();
    const int total_b = b.involvement_total();
    if (total_a > 0 && total_b > 0) {
        // Build each involved community's chain once.
        FixedVector<CommunityChain, INVOLVEMENT_CAP> sources;
        for (const InvolvementEntry& e : a.involvement()) {
            sources.push_back(stances.chain(e.community));
        }
        FixedVector<TargetChain, INVOLVEMENT_CAP> targets;
        for (const InvolvementEntry& e : b.involvement()) {
            const std::optional<TargetId> target = TargetId::from(e.community);
            targets.push_back(target ? stances.target_chain(*target) : TargetChain{});
        }
        std::array<int, INVOLVEMENT_CAP * INVOLVEMENT_CAP> values{};
        stances.stances(std::span(sources.data(), sources.size()), std::span(targets.data(), targets.size()),
                        values);
        // Exact integer accumulation on raw weights; divide once.
        std::int64_t sum = 0;
        for (std::size_t i = 0; i < sources.size(); ++i) {
            const std::int64_t wa = a.involvement()[i].weight;
            for (std::size_t j = 0; j < targets.size(); ++j) {
                const std::int64_t wb = b.involvement()[j].weight;
                sum += wa * wb * values[i * targets.size() + j];
            }
        }
        result.community = static_cast<double>(sum) / (static_cast<double>(total_a) * static_cast<double>(total_b));
    }
    result.reputation = config.k_rep * static_cast<double>(b.reputation());
    result.compat = compat(a, b, config);
    result.noise = config.k_noise * noise(seed, a.id(), NoiseSubject::Person, b.id().value);
    result.total = clamp_opinion(result.community + result.reputation + result.compat + result.noise);
    return result;
}

double weak_opinion(const Character& a, const Character& b, const StanceTable& stances, const OpinionConfig& config,
                    WorldSeed seed) noexcept {
    return weak_opinion_breakdown(a, b, stances, config, seed).total;
}

WeakOpinionBreakdown weak_opinion_breakdown(const Character& a, TargetId target, const StanceTable& stances,
                                            const OpinionConfig& config, WorldSeed seed) noexcept {
    if (!target.valid()) {
        return {};
    }
    WeakOpinionBreakdown result;
    const int total_a = a.involvement_total();
    if (total_a > 0) {
        FixedVector<CommunityChain, INVOLVEMENT_CAP> sources;
        for (const InvolvementEntry& e : a.involvement()) {
            sources.push_back(stances.chain(e.community));
        }
        const TargetChain target_chain = stances.target_chain(target);
        std::array<int, INVOLVEMENT_CAP> values{};
        stances.stances(std::span(sources.data(), sources.size()), std::span(&target_chain, 1), values);
        std::int64_t sum = 0;
        for (std::size_t i = 0; i < sources.size(); ++i) {
            sum += static_cast<std::int64_t>(a.involvement()[i].weight) * values[i];
        }
        result.community = static_cast<double>(sum) / static_cast<double>(total_a);
    }
    result.noise = config.k_noise * openness_factor(a, config)
                 * noise(seed, a.id(), noise_subject(target.kind()), target.index());
    result.total = clamp_opinion(result.community + result.reputation + result.compat + result.noise);
    return result;
}

double weak_opinion(const Character& a, TargetId target, const StanceTable& stances, const OpinionConfig& config,
                    WorldSeed seed) noexcept {
    return weak_opinion_breakdown(a, target, stances, config, seed).total;
}

OpinionBreakdown opinion_breakdown(const Character& a, const Character& b, const StanceTable& stances,
                                   const OpinionConfig& config, WorldSeed seed) noexcept {
    OpinionBreakdown result;
    result.weak = weak_opinion_breakdown(a, b, stances, config, seed);
    if (a.id() == b.id()) {
        return result;
    }
    result.long_term = long_opinion(a, b.id());
    result.short_term = short_opinion(a, b.id());
    result.total = clamp_opinion(result.weak.total + static_cast<double>(result.long_term + result.short_term));
    return result;
}

double opinion(const Character& a, const Character& b, const StanceTable& stances, const OpinionConfig& config,
               WorldSeed seed) noexcept {
    return opinion_breakdown(a, b, stances, config, seed).total;
}

OpinionBreakdown opinion_breakdown(const Character& a, TargetId target, const StanceTable& stances,
                                   const OpinionConfig& config, WorldSeed seed) noexcept {
    OpinionBreakdown result;
    result.weak = weak_opinion_breakdown(a, target, stances, config, seed);
    if (!target.valid()) {
        return result;
    }
    result.long_term = long_opinion(a, target);
    result.short_term = short_opinion(a, target);
    result.total = clamp_opinion(result.weak.total + static_cast<double>(result.long_term + result.short_term));
    return result;
}

double opinion(const Character& a, TargetId target, const StanceTable& stances, const OpinionConfig& config,
               WorldSeed seed) noexcept {
    return opinion_breakdown(a, target, stances, config, seed).total;
}

} // namespace sim
