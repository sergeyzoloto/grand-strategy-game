#include "sim/personal_opinion.hpp"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <span>

#include "sim/bipolar.hpp"

namespace sim {

namespace {

// Index of the entry with the smallest |value|, ties to the smaller target (the list is
// sorted by target, so a strict comparison keeps the first). Requires a non-empty list.
template<class Target, std::size_t N>
std::size_t weakest_index(const FixedVector<LongOpinion<Target>, N>& list) noexcept {
    assert(!list.empty());
    std::size_t weakest = 0;
    for (std::size_t i = 1; i < list.size(); ++i) {
        if (std::abs(list[i].value) < std::abs(list[weakest].value)) {
            weakest = i;
        }
    }
    return weakest;
}

// Shared logic for both long-term lists. `limit` may be below the list's capacity; a list
// already above its limit still evicts only one entry per call (maintain trims the rest).
// `evicted` is invalid unless the outcome is CreatedWithEviction.
template<class Target, std::size_t N>
LongOpinionResult<Target> add_long(FixedVector<LongOpinion<Target>, N>& list, std::size_t limit, Target target,
                                   int delta) noexcept {
    assert(delta >= -LONG_DELTA_MAX && delta <= LONG_DELTA_MAX);
    assert(limit >= 1 && limit <= list.capacity());
    assert(target.valid());
    auto* it = std::lower_bound(list.begin(), list.end(), target,
                                [](const LongOpinion<Target>& e, Target t) { return e.target < t; });
    if (it != list.end() && it->target == target) {
        const int next = std::clamp(it->value + delta, -LONG_VALUE_MAX, LONG_VALUE_MAX);
        if (next == it->value) {
            return {LongOpinionOutcome::Unchanged, Target{}};
        }
        if (next == 0) {
            list.erase(static_cast<std::size_t>(it - list.begin()));
            return {LongOpinionOutcome::Removed, Target{}};
        }
        it->value = static_cast<std::int16_t>(next);
        return {LongOpinionOutcome::Updated, Target{}};
    }
    if (delta == 0) {
        return {LongOpinionOutcome::Unchanged, Target{}};
    }
    const int value = std::clamp(delta, -LONG_VALUE_MAX, LONG_VALUE_MAX);
    const LongOpinion<Target> entry{.target = target, .value = static_cast<std::int16_t>(value), .reserved = 0};
    auto index = static_cast<std::size_t>(it - list.begin());
    if (list.size() >= limit) {
        const std::size_t weakest = weakest_index(list);
        // Evict only if the new entry is strictly stronger.
        if (!(std::abs(value) > std::abs(list[weakest].value))) {
            return {LongOpinionOutcome::Dropped, Target{}};
        }
        const Target evicted = list[weakest].target;
        list.erase(weakest);
        if (weakest < index) {
            --index;
        }
        list.insert(index, entry);
        return {LongOpinionOutcome::CreatedWithEviction, evicted};
    }
    list.insert(index, entry);
    return {LongOpinionOutcome::Created, Target{}};
}

// Modifiers are sorted by (domain, target, modifier).
bool modifier_less(const OpinionModifier& m, ModifierDomain domain, std::uint32_t target,
                   std::uint16_t modifier) noexcept {
    if (m.domain != domain) {
        return static_cast<std::uint8_t>(m.domain) < static_cast<std::uint8_t>(domain);
    }
    if (m.target != target) {
        return m.target < target;
    }
    return m.modifier.value < modifier;
}

using ModifierList = FixedVector<OpinionModifier, MODIFIER_CAP>;

std::size_t modifier_lower(const ModifierList& list, ModifierDomain domain, std::uint32_t target,
                           std::uint16_t modifier) noexcept {
    const auto* it = std::lower_bound(list.begin(), list.end(), 0, [&](const OpinionModifier& m, int /*unused*/) {
        return modifier_less(m, domain, target, modifier);
    });
    return static_cast<std::size_t>(it - list.begin());
}

bool matches(const OpinionModifier& m, ModifierDomain domain, std::uint32_t target) noexcept {
    return m.domain == domain && m.target == target;
}

// `target_changed`: true if this add made the first modifier on the target, or this remove
// took the last one (checked on the sorted neighbours, no extra search).
bool alone_at(const ModifierList& list, std::size_t i, ModifierDomain domain, std::uint32_t target) noexcept {
    return !(i > 0 && matches(list[i - 1], domain, target))
        && !(i + 1 < list.size() && matches(list[i + 1], domain, target));
}

EditResult add_to(ModifierList& list, ModifierDomain domain, std::uint32_t target, ModifierId modifier,
                  int effect, bool& target_changed) noexcept {
    target_changed = false;
    const std::size_t i = modifier_lower(list, domain, target, modifier.value);
    if (i < list.size() && matches(list[i], domain, target) && list[i].modifier == modifier) {
        return EditResult::Duplicate;
    }
    // MODIFIER_CAP is a storage bound: never evict to make room.
    if (list.full()) {
        return EditResult::Full;
    }
    list.insert(i, OpinionModifier{.target = target,
                                   .modifier = modifier,
                                   .effect = detail::clamp_bipolar(effect),
                                   .domain = domain});
    target_changed = alone_at(list, i, domain, target);
    return EditResult::Ok;
}

EditResult remove_from(ModifierList& list, ModifierDomain domain, std::uint32_t target, ModifierId modifier,
                       bool& target_changed) noexcept {
    target_changed = false;
    const std::size_t i = modifier_lower(list, domain, target, modifier.value);
    if (i == list.size() || !matches(list[i], domain, target) || list[i].modifier != modifier) {
        return EditResult::NotFound;
    }
    target_changed = alone_at(list, i, domain, target);
    list.erase(i);
    return EditResult::Ok;
}

int sum_effects(std::span<const OpinionModifier> list, ModifierDomain domain, std::uint32_t target) noexcept {
    auto it = std::lower_bound(list.begin(), list.end(), 0, [&](const OpinionModifier& m, int /*unused*/) {
        return modifier_less(m, domain, target, 0);
    });
    int sum = 0;
    for (; it != list.end() && matches(*it, domain, target); ++it) {
        sum += it->effect;
    }
    return sum;
}

template<class Target>
int long_value(std::span<const LongOpinion<Target>> list, Target target) noexcept {
    const auto it = std::lower_bound(list.begin(), list.end(), target,
                                      [](const LongOpinion<Target>& e, Target t) { return e.target < t; });
    return (it != list.end() && it->target == target) ? it->value : 0;
}

} // namespace

std::size_t person_limit(const Character& a) noexcept {
    const auto span = static_cast<int>(PERSON_LIMIT_MAX - PERSON_LIMIT_MIN);
    const int scaled = span * (a.extraversion() - BIPOLAR_MIN); // 0..6400
    const int range = BIPOLAR_MAX - BIPOLAR_MIN;                // 200
    // Round half up; with span 32 no value lands exactly on a half.
    return PERSON_LIMIT_MIN + static_cast<std::size_t>((scaled + range / 2) / range);
}

int long_opinion(const Character& a, CharacterId target) noexcept {
    return long_value(a.long_people(), target);
}

int long_opinion(const Character& a, TargetId target) noexcept {
    return long_value(a.long_targets(), target);
}

bool has_modifier(const Character& a, CharacterId target) noexcept {
    const std::span<const OpinionModifier> list = a.modifiers();
    const auto it = std::lower_bound(list.begin(), list.end(), 0, [&](const OpinionModifier& m, int /*unused*/) {
        return modifier_less(m, ModifierDomain::Person, target.value, 0);
    });
    return it != list.end() && matches(*it, ModifierDomain::Person, target.value);
}

int short_opinion(const Character& a, CharacterId target) noexcept {
    return sum_effects(a.modifiers(), ModifierDomain::Person, target.value);
}

int short_opinion(const Character& a, TargetId target) noexcept {
    return sum_effects(a.modifiers(), ModifierDomain::Target, target.raw());
}

// ---- keyed Character members ----------------------------------------------------------

EditResult Character::add_modifier(CharacterKey /*key*/, CharacterId target, ModifierId modifier, int effect,
                                   bool& target_changed) noexcept {
    target_changed = false;
    if (!target.valid() || target == id_ || !modifier.valid()) {
        return EditResult::Invalid;
    }
    const EditResult result = add_to(modifiers_, ModifierDomain::Person, target.value, modifier, effect, target_changed);
    assert(lists_valid());
    return result;
}

EditResult Character::add_modifier(CharacterKey /*key*/, TargetId target, ModifierId modifier, int effect) noexcept {
    if (!target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    bool unused = false;
    const EditResult result = add_to(modifiers_, ModifierDomain::Target, target.raw(), modifier, effect, unused);
    assert(lists_valid());
    return result;
}

EditResult Character::remove_modifier(CharacterKey /*key*/, CharacterId target, ModifierId modifier,
                                      bool& target_changed) noexcept {
    target_changed = false;
    if (!target.valid() || target == id_ || !modifier.valid()) {
        return EditResult::Invalid;
    }
    const EditResult result = remove_from(modifiers_, ModifierDomain::Person, target.value, modifier, target_changed);
    assert(lists_valid());
    return result;
}

EditResult Character::remove_modifier(CharacterKey /*key*/, TargetId target, ModifierId modifier) noexcept {
    if (!target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    bool unused = false;
    const EditResult result = remove_from(modifiers_, ModifierDomain::Target, target.raw(), modifier, unused);
    assert(lists_valid());
    return result;
}

LongOpinionResult<CharacterId> Character::add_long_opinion(CharacterKey /*key*/, CharacterId target,
                                                           int delta) noexcept {
    if (!target.valid() || target == id_) {
        return {LongOpinionOutcome::Invalid, CharacterId{}};
    }
    const LongOpinionResult<CharacterId> result =
        add_long(long_people_, person_limit(*this), target, std::clamp(delta, -LONG_DELTA_MAX, LONG_DELTA_MAX));
    assert(lists_valid());
    return result;
}

LongOpinionResult<TargetId> Character::add_long_opinion(CharacterKey /*key*/, TargetId target, int delta) noexcept {
    if (!target.valid()) {
        return {LongOpinionOutcome::Invalid, TargetId{}};
    }
    const LongOpinionResult<TargetId> result =
        add_long(long_targets_, TARGET_LIMIT, target, std::clamp(delta, -LONG_DELTA_MAX, LONG_DELTA_MAX));
    assert(lists_valid());
    return result;
}

std::size_t Character::trim_long_opinions(CharacterKey /*key*/,
                                          FixedVector<CharacterId, PERSON_LIMIT_MAX>& evicted) noexcept {
    // TARGET_LIMIT is fixed and every insert respects it, so only people lists can exceed
    // their limit (after extraversion dropped).
    assert(long_targets_.size() <= TARGET_LIMIT);
    evicted = {};
    const std::size_t limit = person_limit(*this);
    while (long_people_.size() > limit) {
        const std::size_t weakest = weakest_index(long_people_);
        evicted.push_back(long_people_[weakest].target);
        long_people_.erase(weakest);
    }
    assert(lists_valid());
    return evicted.size();
}

} // namespace sim
