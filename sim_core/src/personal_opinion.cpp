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
template<std::size_t N>
std::size_t weakest_index(const FixedVector<LongOpinion, N>& list) noexcept {
    assert(!list.empty());
    std::size_t weakest = 0;
    for (std::size_t i = 1; i < list.size(); ++i) {
        if (std::abs(list[i].value) < std::abs(list[weakest].value)) {
            weakest = i;
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

struct LongCore {
    LongOpinionOutcome outcome;
    std::uint32_t evicted; // raw target, 0 if none
};

// Shared logic for both long-term lists. `limit` may be below the list's capacity; a list
// already above its limit still evicts only one entry per call (maintain trims the rest).
template<std::size_t N>
LongCore add_long(FixedVector<LongOpinion, N>& list, std::size_t limit, std::uint32_t target, int delta) noexcept {
    assert(delta >= -LONG_DELTA_MAX && delta <= LONG_DELTA_MAX);
    assert(limit >= 1 && limit <= list.capacity());
    auto* it = std::lower_bound(list.begin(), list.end(), target,
                                [](const LongOpinion& e, std::uint32_t t) { return e.target < t; });
    if (it != list.end() && it->target == target) {
        const int next = std::clamp(it->value + delta, -LONG_VALUE_MAX, LONG_VALUE_MAX);
        if (next == it->value) {
            return {LongOpinionOutcome::Unchanged, 0};
        }
        if (next == 0) {
            list.erase(static_cast<std::size_t>(it - list.begin()));
            return {LongOpinionOutcome::Removed, 0};
        }
        it->value = static_cast<std::int16_t>(next);
        return {LongOpinionOutcome::Updated, 0};
    }
    if (delta == 0) {
        return {LongOpinionOutcome::Unchanged, 0};
    }
    const int value = std::clamp(delta, -LONG_VALUE_MAX, LONG_VALUE_MAX);
    const LongOpinion entry{.target = target, .value = static_cast<std::int16_t>(value), .reserved = 0};
    auto index = static_cast<std::size_t>(it - list.begin());
    if (list.size() >= limit) {
        const std::size_t weakest = weakest_index(list);
        // Evict only if the new entry is strictly stronger.
        if (!(std::abs(value) > std::abs(list[weakest].value))) {
            return {LongOpinionOutcome::Dropped, 0};
        }
        const std::uint32_t evicted = list[weakest].target;
        list.erase(weakest);
        if (weakest < index) {
            --index;
        }
        list.insert(index, entry);
        return {LongOpinionOutcome::CreatedWithEviction, evicted};
    }
    list.insert(index, entry);
    return {LongOpinionOutcome::Created, 0};
}

template<std::size_t N>
std::size_t trim_to(FixedVector<LongOpinion, N>& list, std::size_t limit) noexcept {
    std::size_t evicted = 0;
    while (list.size() > limit) {
        list.erase(weakest_index(list));
        ++evicted;
    }
    return evicted;
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

EditResult add_to(ModifierList& list, ModifierDomain domain, std::uint32_t target, ModifierId modifier,
                  int effect) noexcept {
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
    return EditResult::Ok;
}

EditResult remove_from(ModifierList& list, ModifierDomain domain, std::uint32_t target, ModifierId modifier) noexcept {
    const std::size_t i = modifier_lower(list, domain, target, modifier.value);
    if (i == list.size() || !matches(list[i], domain, target) || list[i].modifier != modifier) {
        return EditResult::NotFound;
    }
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

int long_value(std::span<const LongOpinion> list, std::uint32_t target) noexcept {
    const auto it = std::lower_bound(list.begin(), list.end(), target,
                                      [](const LongOpinion& e, std::uint32_t t) { return e.target < t; });
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
    return long_value(a.long_people(), target.value);
}

int long_opinion(const Character& a, TargetId target) noexcept {
    return long_value(a.long_targets(), target.raw());
}

int short_opinion(const Character& a, CharacterId target) noexcept {
    return sum_effects(a.modifiers(), ModifierDomain::Person, target.value);
}

int short_opinion(const Character& a, TargetId target) noexcept {
    return sum_effects(a.modifiers(), ModifierDomain::Target, target.raw());
}

// ---- keyed Character members ----------------------------------------------------------

EditResult Character::add_modifier(CharacterKey /*key*/, CharacterId target, ModifierId modifier,
                                   int effect) noexcept {
    if (!target.valid() || target == id_ || !modifier.valid()) {
        return EditResult::Invalid;
    }
    const EditResult result = add_to(modifiers_, ModifierDomain::Person, target.value, modifier, effect);
    assert(lists_valid());
    return result;
}

EditResult Character::add_modifier(CharacterKey /*key*/, TargetId target, ModifierId modifier, int effect) noexcept {
    if (!target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    const EditResult result = add_to(modifiers_, ModifierDomain::Target, target.raw(), modifier, effect);
    assert(lists_valid());
    return result;
}

EditResult Character::remove_modifier(CharacterKey /*key*/, CharacterId target, ModifierId modifier) noexcept {
    if (!target.valid() || target == id_ || !modifier.valid()) {
        return EditResult::Invalid;
    }
    const EditResult result = remove_from(modifiers_, ModifierDomain::Person, target.value, modifier);
    assert(lists_valid());
    return result;
}

EditResult Character::remove_modifier(CharacterKey /*key*/, TargetId target, ModifierId modifier) noexcept {
    if (!target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    const EditResult result = remove_from(modifiers_, ModifierDomain::Target, target.raw(), modifier);
    assert(lists_valid());
    return result;
}

LongOpinionResult<CharacterId> Character::add_long_opinion(CharacterKey /*key*/, CharacterId target,
                                                           int delta) noexcept {
    if (!target.valid() || target == id_) {
        return {LongOpinionOutcome::Invalid, CharacterId{}};
    }
    const LongCore core = add_long(long_people_, person_limit(*this), target.value,
                                   std::clamp(delta, -LONG_DELTA_MAX, LONG_DELTA_MAX));
    assert(lists_valid());
    return {core.outcome, CharacterId{core.evicted}};
}

LongOpinionResult<TargetId> Character::add_long_opinion(CharacterKey /*key*/, TargetId target, int delta) noexcept {
    if (!target.valid()) {
        return {LongOpinionOutcome::Invalid, TargetId{}};
    }
    const LongCore core = add_long(long_targets_, TARGET_LIMIT, target.raw(),
                                   std::clamp(delta, -LONG_DELTA_MAX, LONG_DELTA_MAX));
    assert(lists_valid());
    return {core.outcome, core.evicted != 0 ? target_from_raw(core.evicted) : TargetId{}};
}

std::size_t Character::trim_long_opinions(CharacterKey /*key*/) noexcept {
    const std::size_t evicted = trim_to(long_people_, person_limit(*this)) + trim_to(long_targets_, TARGET_LIMIT);
    assert(lists_valid());
    return evicted;
}

} // namespace sim
