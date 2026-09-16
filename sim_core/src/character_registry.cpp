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

OpinionEventResult<CharacterId> CharacterRegistry::apply_person_event(CharacterId a, CharacterId b, int delta, Date now,
                                                                      CauseId cause, const OpinionConfig& config) noexcept {
    if (!a.valid() || !b.valid() || a == b) {
        return {OpinionEventOutcome::Invalid, CharacterId{}};
    }
    Character* source = find(a);
    if (source == nullptr || find(b) == nullptr) {
        return {OpinionEventOutcome::NotFound, CharacterId{}};
    }
    return source->apply_opinion_event(CharacterKey{}, b, delta, now, cause, config);
}

OpinionEventResult<TargetId> CharacterRegistry::apply_target_event(CharacterId a, TargetId target, int delta, Date now,
                                                                   CauseId cause, const OpinionConfig& config) noexcept {
    if (!a.valid() || !target.valid()) {
        return {OpinionEventOutcome::Invalid, TargetId{}};
    }
    Character* source = find(a);
    if (source == nullptr) {
        return {OpinionEventOutcome::NotFound, TargetId{}};
    }
    return source->apply_opinion_event(CharacterKey{}, target, delta, now, cause, config);
}

StrongMaintainCounts CharacterRegistry::maintain(Date now, const OpinionConfig& config) noexcept {
    StrongMaintainCounts total;
    for (Character& c : characters_) { // id order
        const StrongMaintainCounts counts = c.maintain_strong_opinions(CharacterKey{}, now, config);
        total.removed_decayed += counts.removed_decayed;
        total.evicted_over_limit += counts.evicted_over_limit;
    }
    return total;
}

std::size_t CharacterRegistry::allocated_bytes() const noexcept {
    return sizeof(*this) + characters_.capacity() * sizeof(Character) + relations_.allocated_bytes();
}

} // namespace sim
