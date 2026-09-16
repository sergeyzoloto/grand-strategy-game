#include "sim/character_registry.hpp"

#include <cassert>
#include <cstddef>
#include <cstdint>
#include <limits>

#include "sim/vector_growth.hpp"

namespace sim {

namespace {
constexpr std::size_t MIN_CHARACTER_CAPACITY = 16;
} // namespace

CharacterId CharacterRegistry::create(NameId name, Gender gender, Date birth, const CharacterInit& init) {
    if (characters_.size() >= std::numeric_limits<std::uint32_t>::max()) {
        return CharacterId{};
    }
    // Reserve in both containers before the first write, so bad_alloc creates nothing.
    detail::reserve_one_more(characters_, MIN_CHARACTER_CAPACITY);
    relations_.reserve_node();

    const CharacterId id{static_cast<std::uint32_t>(characters_.size() + 1)};
    characters_.emplace_back(CharacterKey{}, id, name, gender, birth, init);
    relations_.add_node();
    assert(characters_.size() == relations_.node_count());
    return id;
}

Character* CharacterRegistry::find(CharacterId id) noexcept {
    if (!id.valid() || id.value > characters_.size()) {
        return nullptr;
    }
    return &characters_[id.value - 1];
}

const Character* CharacterRegistry::find(CharacterId id) const noexcept {
    if (!id.valid() || id.value > characters_.size()) {
        return nullptr;
    }
    return &characters_[id.value - 1];
}

EditResult CharacterRegistry::add_person_modifier(CharacterId a, CharacterId b, ModifierId modifier,
                                                 int effect) noexcept {
    if (!a.valid() || !b.valid() || a == b || !modifier.valid()) {
        return EditResult::Invalid;
    }
    Character* source = find(a);
    if (source == nullptr || find(b) == nullptr) {
        return EditResult::NotFound;
    }
    return source->add_modifier(CharacterKey{}, b, modifier, effect);
}

EditResult CharacterRegistry::add_target_modifier(CharacterId a, TargetId target, ModifierId modifier,
                                                 int effect) noexcept {
    if (!a.valid() || !target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    Character* source = find(a);
    if (source == nullptr) {
        return EditResult::NotFound;
    }
    return source->add_modifier(CharacterKey{}, target, modifier, effect);
}

EditResult CharacterRegistry::remove_modifier(CharacterId a, CharacterId b, ModifierId modifier) noexcept {
    if (!a.valid() || !b.valid() || a == b || !modifier.valid()) {
        return EditResult::Invalid;
    }
    Character* source = find(a);
    if (source == nullptr || find(b) == nullptr) {
        return EditResult::NotFound;
    }
    return source->remove_modifier(CharacterKey{}, b, modifier);
}

EditResult CharacterRegistry::remove_modifier(CharacterId a, TargetId target, ModifierId modifier) noexcept {
    if (!a.valid() || !target.valid() || !modifier.valid()) {
        return EditResult::Invalid;
    }
    Character* source = find(a);
    if (source == nullptr) {
        return EditResult::NotFound;
    }
    return source->remove_modifier(CharacterKey{}, target, modifier);
}

LongOpinionResult<CharacterId> CharacterRegistry::add_person_long(CharacterId a, CharacterId b, int delta) noexcept {
    if (!a.valid() || !b.valid() || a == b) {
        return {LongOpinionOutcome::Invalid, CharacterId{}};
    }
    Character* source = find(a);
    if (source == nullptr || find(b) == nullptr) {
        return {LongOpinionOutcome::NotFound, CharacterId{}};
    }
    return source->add_long_opinion(CharacterKey{}, b, delta);
}

LongOpinionResult<TargetId> CharacterRegistry::add_target_long(CharacterId a, TargetId target, int delta) noexcept {
    if (!a.valid() || !target.valid()) {
        return {LongOpinionOutcome::Invalid, TargetId{}};
    }
    Character* source = find(a);
    if (source == nullptr) {
        return {LongOpinionOutcome::NotFound, TargetId{}};
    }
    return source->add_long_opinion(CharacterKey{}, target, delta);
}

std::size_t CharacterRegistry::maintain() noexcept {
    std::size_t evicted = 0;
    for (Character& c : characters_) { // id order
        evicted += c.trim_long_opinions(CharacterKey{});
    }
    return evicted;
}

std::size_t CharacterRegistry::allocated_bytes() const noexcept {
    return sizeof(*this) + characters_.capacity() * sizeof(Character) + relations_.allocated_bytes();
}

} // namespace sim
