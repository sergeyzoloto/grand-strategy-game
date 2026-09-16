#include "sim/character.hpp"

#include <cassert>
#include <cstdint>

#include "sim/quantize.hpp"

namespace sim {

namespace {

void store_u16(std::uint16_t& field, float v) noexcept {
    assert(!detail::is_nan(static_cast<double>(v)));
    field = store_unit(field, static_cast<double>(v));
}

void add_u16(std::uint16_t& field, float delta) noexcept {
    assert(!detail::is_nan(static_cast<double>(delta)));
    field = add_unit(field, static_cast<double>(delta));
}

float get_u16(std::uint16_t raw) noexcept { return dequantize_unit(raw); }

} // namespace

Character::Character(CharacterId id, NameId name, Gender gender, Date birth, const CharacterInit& init) noexcept
    : id_(id), name_(name), birth_(birth), gender_(gender) {
    // The registry (a later step) is the only creator; an invalid id is a programmer error.
    assert(id.valid());
    set_health(init.health);
    set_stress(init.stress);
    set_capacity(init.capacity);
    set_strength(init.strength);
    set_intelligence(init.intelligence);
    set_stability(init.stability);
    set_openness(init.openness);
    set_extraversion(init.extraversion);
    set_conscientiousness(init.conscientiousness);
    set_agreeableness(init.agreeableness);
    set_attractiveness(init.attractiveness);
    set_height(init.height);
    set_shape(init.shape);
    set_charisma(init.charisma);
}

float Character::health() const noexcept { return get_u16(health_); }
float Character::stress() const noexcept { return get_u16(stress_); }
float Character::capacity() const noexcept { return get_u16(capacity_); }

void Character::set_health(float v) noexcept { store_u16(health_, v); }
void Character::set_stress(float v) noexcept { store_u16(stress_, v); }
void Character::set_capacity(float v) noexcept { store_u16(capacity_, v); }

void Character::add_health(float delta) noexcept { add_u16(health_, delta); }
void Character::add_stress(float delta) noexcept { add_u16(stress_, delta); }
void Character::add_capacity(float delta) noexcept { add_u16(capacity_, delta); }

} // namespace sim
