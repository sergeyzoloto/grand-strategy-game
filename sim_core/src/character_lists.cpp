#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <span>
#include <tuple>

#include "sim/character.hpp"

namespace sim {

namespace {

// Switches without default: appending an enum value triggers -Wswitch here.
bool is_valid(SkillKind kind) noexcept {
    switch (kind) {
    case SkillKind::Ability:
    case SkillKind::Education:
    case SkillKind::Language:
        return true;
    }
    return false;
}

bool is_valid(ModifierDomain domain) noexcept {
    // No default: appending a ModifierDomain triggers -Wswitch here.
    switch (domain) {
    case ModifierDomain::Person:
    case ModifierDomain::Target:
        return true;
    }
    return false;
}

bool is_valid(SacredSign sign) noexcept {
    switch (sign) {
    case SacredSign::Plus:
    case SacredSign::Minus:
        return true;
    }
    return false;
}

// Sort key of a practise entry: (kind, skill id).
bool practise_less(const PractiseEntry& a, SkillKind kind, SkillId skill) noexcept {
    if (a.kind != kind) {
        return a.kind < kind;
    }
    return a.skill < skill;
}

template<class Vector, class Pred>
std::size_t lower_index(const Vector& items, Pred less_than_key) noexcept {
    return static_cast<std::size_t>(std::partition_point(items.begin(), items.end(), less_than_key) - items.begin());
}

} // namespace

// ---- nicknames ------------------------------------------------------------------

EditResult Character::add_nickname(NameId name) noexcept {
    if (!name.valid()) {
        return EditResult::Invalid;
    }
    if (std::find(nicknames_.begin(), nicknames_.end(), name) != nicknames_.end()) {
        return EditResult::Duplicate;
    }
    if (nicknames_.full()) {
        return EditResult::Full;
    }
    nicknames_.push_back(name);
    assert(lists_valid());
    return EditResult::Ok;
}

EditResult Character::remove_nickname(NameId name) noexcept {
    if (!name.valid()) {
        return EditResult::Invalid;
    }
    const auto* it = std::find(nicknames_.begin(), nicknames_.end(), name);
    if (it == nicknames_.end()) {
        return EditResult::NotFound;
    }
    nicknames_.erase(static_cast<std::size_t>(it - nicknames_.begin()));
    assert(lists_valid());
    return EditResult::Ok;
}

// ---- practise -------------------------------------------------------------------

bool Character::has_skill(SkillKind kind, SkillId skill) const noexcept {
    if (!skill.valid() || !is_valid(kind)) {
        return false;
    }
    const std::size_t i = lower_index(practise_, [&](const PractiseEntry& e) { return practise_less(e, kind, skill); });
    return i < practise_.size() && practise_[i].kind == kind && practise_[i].skill == skill;
}

EditResult Character::add_skill(SkillKind kind, SkillId skill) noexcept {
    if (!skill.valid() || !is_valid(kind)) {
        return EditResult::Invalid;
    }
    const std::size_t i = lower_index(practise_, [&](const PractiseEntry& e) { return practise_less(e, kind, skill); });
    if (i < practise_.size() && practise_[i].kind == kind && practise_[i].skill == skill) {
        return EditResult::Duplicate;
    }
    // PRACTISE_CAP is a storage bound: never evict to make room.
    if (practise_.full()) {
        return EditResult::Full;
    }
    practise_.insert(i, PractiseEntry{.skill = skill, .kind = kind, .reserved = 0});
    assert(lists_valid());
    return EditResult::Ok;
}

EditResult Character::remove_skill(SkillKind kind, SkillId skill) noexcept {
    if (!skill.valid() || !is_valid(kind)) {
        return EditResult::Invalid;
    }
    const std::size_t i = lower_index(practise_, [&](const PractiseEntry& e) { return practise_less(e, kind, skill); });
    if (i == practise_.size() || practise_[i].kind != kind || practise_[i].skill != skill) {
        return EditResult::NotFound;
    }
    practise_.erase(i);
    assert(lists_valid());
    return EditResult::Ok;
}

// ---- involvement ----------------------------------------------------------------

EditResult Character::set_involvement_weight(CommunityId community, std::uint8_t weight) noexcept {
    if (!community.valid()) {
        return EditResult::Invalid;
    }
    const std::size_t i =
        lower_index(involvement_, [&](const InvolvementEntry& e) { return e.community < community; });
    const bool present = i < involvement_.size() && involvement_[i].community == community;
    if (weight == 0) {
        if (present) {
            involvement_.erase(i);
        }
    } else if (present) {
        involvement_[i].weight = weight;
    } else if (involvement_.full()) {
        return EditResult::Full;
    } else {
        involvement_.insert(i, InvolvementEntry{.community = community, .weight = weight, .padding = {}});
    }
    assert(lists_valid());
    return EditResult::Ok;
}

int Character::involvement_total() const noexcept {
    int total = 0;
    for (const InvolvementEntry& e : involvement_) {
        total += e.weight;
    }
    return total;
}

float Character::involvement_share(CommunityId community) const noexcept {
    for (const InvolvementEntry& e : involvement_) {
        if (e.community == community) {
            // Plain division, never adjusted: equal weights give bit-identical shares.
            return static_cast<float>(static_cast<double>(e.weight) / static_cast<double>(involvement_total()));
        }
    }
    return 0.0f;
}

std::optional<CommunityId> Character::main_community() const noexcept {
    std::optional<CommunityId> best;
    std::uint8_t best_weight = 0;
    // Entries are sorted by id, so a strict comparison keeps the smaller id on ties.
    for (const InvolvementEntry& e : involvement_) {
        if (e.weight > best_weight) {
            best = e.community;
            best_weight = e.weight;
        }
    }
    return best;
}

// ---- sacred ---------------------------------------------------------------------

EditResult Character::add_sacred(TargetId target, SacredSign sign) noexcept {
    if (!target.valid() || !is_valid(sign)) {
        return EditResult::Invalid;
    }
    const std::size_t i = lower_index(sacred_, [&](const SacredEntry& e) { return e.target < target; });
    if (i < sacred_.size() && sacred_[i].target == target) {
        return sacred_[i].sign == sign ? EditResult::Duplicate : EditResult::Conflict;
    }
    if (sacred_.full()) {
        return EditResult::Full;
    }
    sacred_.insert(i, SacredEntry{.target = target, .sign = sign, .padding = {}});
    assert(lists_valid());
    return EditResult::Ok;
}

EditResult Character::remove_sacred(TargetId target) noexcept {
    if (!target.valid()) {
        return EditResult::Invalid;
    }
    const std::size_t i = lower_index(sacred_, [&](const SacredEntry& e) { return e.target < target; });
    if (i == sacred_.size() || sacred_[i].target != target) {
        return EditResult::NotFound;
    }
    sacred_.erase(i);
    assert(lists_valid());
    return EditResult::Ok;
}

std::optional<SacredSign> Character::sacred_sign(TargetId target) const noexcept {
    if (!target.valid()) {
        return std::nullopt;
    }
    const std::size_t i = lower_index(sacred_, [&](const SacredEntry& e) { return e.target < target; });
    if (i == sacred_.size() || sacred_[i].target != target) {
        return std::nullopt;
    }
    return sacred_[i].sign;
}

// ---- invariants -----------------------------------------------------------------

bool Character::lists_valid() const noexcept {
    for (std::size_t i = 0; i < nicknames_.size(); ++i) {
        if (!nicknames_[i].valid() || std::find(nicknames_.begin() + i + 1, nicknames_.end(), nicknames_[i]) != nicknames_.end()) {
            return false;
        }
    }
    for (std::size_t i = 0; i < practise_.size(); ++i) {
        const PractiseEntry& e = practise_[i];
        if (!e.skill.valid() || !is_valid(e.kind) || e.reserved != 0) {
            return false;
        }
        if (i > 0 && !practise_less(practise_[i - 1], e.kind, e.skill)) {
            return false;
        }
    }
    for (std::size_t i = 0; i < involvement_.size(); ++i) {
        const InvolvementEntry& e = involvement_[i];
        if (!e.community.valid() || e.weight == 0 || (i > 0 && !(involvement_[i - 1].community < e.community))) {
            return false;
        }
    }
    for (std::size_t i = 0; i < sacred_.size(); ++i) {
        const SacredEntry& e = sacred_[i];
        if (!e.target.valid() || !is_valid(e.sign) || (i > 0 && !(sacred_[i - 1].target < e.target))) {
            return false;
        }
    }
    for (const auto list : {long_people(), long_targets()}) {
        for (std::size_t i = 0; i < list.size(); ++i) {
            const LongOpinion& e = list[i];
            if (e.target == 0 || e.value == 0 || e.value < -LONG_VALUE_MAX || e.value > LONG_VALUE_MAX
                || e.reserved != 0 || (i > 0 && !(list[i - 1].target < e.target))) {
                return false;
            }
        }
    }
    for (std::size_t i = 0; i < modifiers_.size(); ++i) {
        const OpinionModifier& m = modifiers_[i];
        if (m.target == 0 || !m.modifier.valid() || !is_valid(m.domain) || m.effect < -MODIFIER_EFFECT_MAX
            || m.effect > MODIFIER_EFFECT_MAX) {
            return false;
        }
        if (i > 0) {
            const OpinionModifier& p = modifiers_[i - 1];
            const auto key = [](const OpinionModifier& x) {
                return std::tuple{static_cast<std::uint8_t>(x.domain), x.target, x.modifier.value};
            };
            if (!(key(p) < key(m))) {
                return false;
            }
        }
    }
    return true;
}

} // namespace sim
