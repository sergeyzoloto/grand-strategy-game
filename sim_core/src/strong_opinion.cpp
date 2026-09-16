#include "sim/strong_opinion.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>

#include "sim/hundredths.hpp"
#include "sim/opinion.hpp"

namespace sim {

namespace {

constexpr double POWER_CUTOFF = 1e-12;

double linear_in_trait(int trait, double at_min, double at_max) noexcept {
    return at_min + (at_max - at_min) * (static_cast<double>(trait - BIPOLAR_MIN)
                                         / static_cast<double>(BIPOLAR_MAX - BIPOLAR_MIN));
}

template<std::size_t N>
const StrongOpinion* find_record(const FixedVector<StrongOpinion, N>& list, std::uint32_t target) noexcept {
    const auto* it = std::lower_bound(list.begin(), list.end(), target,
                                      [](const StrongOpinion& r, std::uint32_t t) { return r.target < t; });
    return (it != list.end() && it->target == target) ? it : nullptr;
}

// Index of the record with the smallest |dev(now)|, ties to the smaller target (the list
// is sorted by target, so a strict comparison keeps the first). Requires a non-empty list.
template<std::size_t N>
std::size_t weakest_index(const FixedVector<StrongOpinion, N>& list, double r, Date now) noexcept {
    assert(!list.empty());
    std::size_t weakest = 0;
    double weakest_abs = std::abs(strong_deviation_raw(list[0], r, now));
    for (std::size_t i = 1; i < list.size(); ++i) {
        const double abs_dev = std::abs(strong_deviation_raw(list[i], r, now));
        if (abs_dev < weakest_abs) {
            weakest = i;
            weakest_abs = abs_dev;
        }
    }
    return weakest;
}

// Converts a stored raw value back to a typed TargetId at the boundary. Stored values
// always came from valid TargetIds; anything else yields an invalid TargetId.
TargetId target_from_raw(std::uint32_t raw) noexcept {
    const std::uint32_t index = raw & (TargetId::INDEX_LIMIT - 1);
    // No default: appending a TargetKind triggers -Wswitch here.
    switch (static_cast<TargetKind>(raw >> TargetId::INDEX_BITS)) {
    case TargetKind::Community:
        return TargetId::from(CommunityId{index}).value_or(TargetId{});
    case TargetKind::Topic:
        return TargetId::from(TopicId{index}).value_or(TargetId{});
    }
    return TargetId{};
}

struct EventCore {
    OpinionEventOutcome outcome;
    std::uint32_t evicted; // raw target, 0 if none
};

// Shared event logic for both lists. `limit` may be below the list's capacity; a list
// already above its limit still evicts only one record per event (maintain trims the rest).
template<std::size_t N>
EventCore apply_event(FixedVector<StrongOpinion, N>& list, std::size_t limit, std::uint32_t target, int delta,
                      double amp, double k, double r, Date now, CauseId cause, const OpinionConfig& config) noexcept {
    assert(delta >= -EVENT_DELTA_MAX && delta <= EVENT_DELTA_MAX);
    assert(limit <= list.capacity());
    const double d = amp * static_cast<double>(delta); // value units
    const double d_raw = d * detail::HUNDREDTHS_PER_UNIT;
    const std::int64_t span = 2 * static_cast<std::int64_t>(STRONG_DEV_MAX_RAW);

    auto it = std::lower_bound(list.begin(), list.end(), target,
                               [](const StrongOpinion& rec, std::uint32_t t) { return rec.target < t; });
    if (it != list.end() && it->target == target) {
        // s first, then long from the rounded s; the fractional long delta is rounded, not the sum.
        const std::int64_t s = detail::clamp_round(strong_deviation_raw(*it, r, now) + d_raw,
                                                   -STRONG_DEV_MAX_RAW, STRONG_DEV_MAX_RAW);
        const std::int64_t long_step = detail::clamp_round(k * static_cast<double>(s - it->long_dev), -span, span);
        it->long_dev = static_cast<std::int16_t>(
            std::clamp<std::int64_t>(it->long_dev + long_step, -STRONG_DEV_MAX_RAW, STRONG_DEV_MAX_RAW));
        it->short_dev = static_cast<std::int16_t>(s);
        it->t0 = now;
        it->cause = cause;
        return {OpinionEventOutcome::Updated, 0};
    }

    if (std::abs(d) < config.enter_threshold) {
        return {OpinionEventOutcome::Dropped, 0};
    }
    // Created as if dev(now) were 0.
    const std::int64_t s = detail::clamp_round(d_raw, -STRONG_DEV_MAX_RAW, STRONG_DEV_MAX_RAW);
    const std::int64_t long_dev = std::clamp<std::int64_t>(detail::clamp_round(k * static_cast<double>(s), -span, span),
                                                           -STRONG_DEV_MAX_RAW, STRONG_DEV_MAX_RAW);
    const StrongOpinion record{.target = target,
                               .short_dev = static_cast<std::int16_t>(s),
                               .long_dev = static_cast<std::int16_t>(long_dev),
                               .t0 = now,
                               .cause = cause,
                               .reserved = 0};
    auto index = static_cast<std::size_t>(it - list.begin());

    if (list.size() >= limit) {
        const std::size_t weakest = weakest_index(list, r, now);
        // Evict only if the new event is strictly stronger (compared in hundredths).
        if (!(std::abs(d_raw) > std::abs(strong_deviation_raw(list[weakest], r, now)))) {
            return {OpinionEventOutcome::Dropped, 0};
        }
        const std::uint32_t evicted = list[weakest].target;
        list.erase(weakest);
        if (weakest < index) {
            --index;
        }
        list.insert(index, record);
        return {OpinionEventOutcome::CreatedWithEviction, evicted};
    }
    list.insert(index, record);
    return {OpinionEventOutcome::Created, 0};
}

template<std::size_t N>
std::size_t remove_decayed(FixedVector<StrongOpinion, N>& list, double r, Date now, const OpinionConfig& config) noexcept {
    const double exit_raw = config.exit_threshold * detail::HUNDREDTHS_PER_UNIT;
    std::size_t removed = 0;
    for (std::size_t i = list.size(); i-- > 0;) {
        if (std::abs(strong_deviation_raw(list[i], r, now)) < exit_raw) {
            list.erase(i);
            ++removed;
        }
    }
    return removed;
}

} // namespace

double retention_power(double r, std::int64_t weeks) noexcept {
    assert(r > 0.0 && r <= 1.0);
    if (weeks <= 0) {
        return 1.0;
    }
    auto n = static_cast<std::uint64_t>(weeks);
    double result = 1.0;
    double base = r;
    while (true) {
        if ((n & 1u) != 0) {
            result *= base;
            if (result < POWER_CUTOFF) {
                return 0.0;
            }
        }
        n >>= 1u;
        if (n == 0) {
            return result;
        }
        base *= base;
        // A remaining bit multiplies result (<= 1) by at most base.
        if (base < POWER_CUTOFF) {
            return 0.0;
        }
    }
}

double retention(const Character& a, const OpinionConfig& config) noexcept {
    return linear_in_trait(a.stability(), config.retention_unstable, config.retention_stable);
}

double amplitude(const Character& a, const OpinionConfig& config) noexcept {
    return linear_in_trait(a.stability(), config.amplitude_unstable, config.amplitude_stable);
}

std::size_t person_limit(const Character& a) noexcept {
    const auto span = static_cast<int>(PERSON_LIMIT_MAX - PERSON_LIMIT_MIN);
    const int scaled = span * (a.extraversion() - BIPOLAR_MIN); // 0..6400
    const int range = BIPOLAR_MAX - BIPOLAR_MIN;                // 200
    // Round half up; with span 32 no value lands exactly on a half.
    return PERSON_LIMIT_MIN + static_cast<std::size_t>((scaled + range / 2) / range);
}

double strong_deviation_raw(const StrongOpinion& record, double retention_factor, Date now) noexcept {
    const std::int64_t weeks = static_cast<std::int64_t>(now.weeks) - static_cast<std::int64_t>(record.t0.weeks);
    assert(weeks >= 0 && "now is before the record's t0");
    const double factor = retention_power(retention_factor, weeks);
    const auto long_dev = static_cast<double>(record.long_dev);
    return long_dev + (static_cast<double>(record.short_dev) - long_dev) * factor;
}

double strong_deviation(const Character& a, CharacterId target, Date now, const OpinionConfig& config) noexcept {
    for (const StrongOpinion& record : a.strong_people()) {
        if (record.target == target.value) {
            return strong_deviation_raw(record, retention(a, config), now) / detail::HUNDREDTHS_PER_UNIT;
        }
    }
    return 0.0;
}

double strong_deviation(const Character& a, TargetId target, Date now, const OpinionConfig& config) noexcept {
    for (const StrongOpinion& record : a.strong_targets()) {
        if (record.target == target.raw()) {
            return strong_deviation_raw(record, retention(a, config), now) / detail::HUNDREDTHS_PER_UNIT;
        }
    }
    return 0.0;
}

// ---- keyed Character members ----------------------------------------------------------

OpinionEventResult<CharacterId> Character::apply_opinion_event(CharacterKey /*key*/, CharacterId target, int delta,
                                                               Date now, CauseId cause,
                                                               const OpinionConfig& config) noexcept {
    assert(config.exit_threshold < config.enter_threshold);
    if (!target.valid() || target == id_) {
        return {OpinionEventOutcome::Invalid, CharacterId{}};
    }
    const EventCore core = apply_event(strong_people_, person_limit(*this), target.value,
                                       std::clamp(delta, -EVENT_DELTA_MAX, EVENT_DELTA_MAX), amplitude(*this, config),
                                       config.plasticity, retention(*this, config), now, cause, config);
    return {core.outcome, CharacterId{core.evicted}};
}

OpinionEventResult<TargetId> Character::apply_opinion_event(CharacterKey /*key*/, TargetId target, int delta, Date now,
                                                            CauseId cause, const OpinionConfig& config) noexcept {
    assert(config.exit_threshold < config.enter_threshold);
    if (!target.valid()) {
        return {OpinionEventOutcome::Invalid, TargetId{}};
    }
    const EventCore core = apply_event(strong_targets_, TARGET_LIMIT, target.raw(),
                                       std::clamp(delta, -EVENT_DELTA_MAX, EVENT_DELTA_MAX), amplitude(*this, config),
                                       config.plasticity * openness_factor(*this, config), retention(*this, config),
                                       now, cause, config);
    return {core.outcome, core.evicted != 0 ? target_from_raw(core.evicted) : TargetId{}};
}

StrongMaintainCounts Character::maintain_strong_opinions(CharacterKey /*key*/, Date now,
                                                         const OpinionConfig& config) noexcept {
    assert(config.exit_threshold < config.enter_threshold);
    const double r = retention(*this, config);
    StrongMaintainCounts counts;
    counts.removed_decayed += remove_decayed(strong_people_, r, now, config);
    counts.removed_decayed += remove_decayed(strong_targets_, r, now, config);
    const std::size_t limit = person_limit(*this);
    while (strong_people_.size() > limit) {
        strong_people_.erase(weakest_index(strong_people_, r, now));
        ++counts.evicted_over_limit;
    }
    return counts;
}

} // namespace sim
