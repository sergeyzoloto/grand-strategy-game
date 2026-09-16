#include "sim/character.hpp"

#include "sim/hundredths.hpp"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>

namespace sim {

namespace {

using detail::clamp_round;
using detail::HUNDREDTHS_PER_UNIT;

// Condition fields: raw 0..10000, value = raw / 100.
constexpr std::int64_t HUNDREDTHS_MAX = 10000;

float get_hundredths(std::uint16_t raw) noexcept {
    return static_cast<float>(static_cast<double>(raw) / HUNDREDTHS_PER_UNIT);
}

void set_hundredths(std::uint16_t& raw, float value) noexcept {
    assert(!std::isnan(value));
    if (std::isnan(value)) {
        return;
    }
    const double scaled = static_cast<double>(value) * HUNDREDTHS_PER_UNIT;
    raw = static_cast<std::uint16_t>(clamp_round(scaled, 0, HUNDREDTHS_MAX));
}

// Rounds the delta to whole steps, then adds (see clamp_round): add(d) followed by
// add(-d) restores raw unless saturation intervened, and repeated adds accumulate no
// float error.
void add_hundredths(std::uint16_t& raw, float delta) noexcept {
    assert(!std::isnan(delta));
    if (std::isnan(delta)) {
        return;
    }
    const double scaled = static_cast<double>(delta) * HUNDREDTHS_PER_UNIT;
    const std::int64_t steps = clamp_round(scaled, -HUNDREDTHS_MAX, HUNDREDTHS_MAX);
    const std::int64_t sum = static_cast<std::int64_t>(raw) + steps;
    raw = static_cast<std::uint16_t>(std::clamp<std::int64_t>(sum, 0, HUNDREDTHS_MAX));
}

} // namespace

Character::Character(CharacterKey /*key*/, CharacterId id, NameId name, Gender gender, Date birth,
                     const CharacterInit& init) noexcept
    : id_(id), name_(name), birth_(birth), gender_(gender) {
    // Layout: the 30-byte core keeps its Step 1 offsets; lists follow.
    static_assert(offsetof(Character, id_) == 0 && offsetof(Character, name_) == 4 && offsetof(Character, birth_) == 8);
    static_assert(offsetof(Character, health_) == 12 && offsetof(Character, stress_) == 14
                  && offsetof(Character, capacity_) == 16);
    static_assert(offsetof(Character, gender_) == 18 && offsetof(Character, strength_) == 19
                  && offsetof(Character, charisma_) == 29);
    static_assert(offsetof(Character, practise_) == 30 && offsetof(Character, nicknames_) == 288
                  && offsetof(Character, involvement_) == 308 && offsetof(Character, sacred_) == 376);
    static_assert(offsetof(Character, reputation_) == 444);
    static_assert(offsetof(Character, strong_people_) == 448 && offsetof(Character, strong_targets_) == 1092);

    // The registry (a later step) is the only creator; an invalid id is a programmer error.
    assert(id.valid());
    set_health(init.health);
    set_stress(init.stress);
    set_capacity(init.capacity);
    set_strength(init.strength.value());
    set_intelligence(init.intelligence.value());
    set_stability(init.stability.value());
    set_openness(init.openness.value());
    set_extraversion(init.extraversion.value());
    set_conscientiousness(init.conscientiousness.value());
    set_agreeableness(init.agreeableness.value());
    set_attractiveness(init.attractiveness.value());
    set_height(init.height.value());
    set_shape(init.shape.value());
    set_charisma(init.charisma.value());
    set_reputation(init.reputation.value());
}

float Character::health() const noexcept { return get_hundredths(health_); }
float Character::stress() const noexcept { return get_hundredths(stress_); }
float Character::capacity() const noexcept { return get_hundredths(capacity_); }

void Character::set_health(float v) noexcept { set_hundredths(health_, v); }
void Character::set_stress(float v) noexcept { set_hundredths(stress_, v); }
void Character::set_capacity(float v) noexcept { set_hundredths(capacity_, v); }

void Character::add_health(float delta) noexcept { add_hundredths(health_, delta); }
void Character::add_stress(float delta) noexcept { add_hundredths(stress_, delta); }
void Character::add_capacity(float delta) noexcept { add_hundredths(capacity_, delta); }

} // namespace sim
