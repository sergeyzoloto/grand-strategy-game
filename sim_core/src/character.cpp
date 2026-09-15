#include "sim/character.hpp"

#include <cassert>
#include <cstdint>

#include "sim/quantize.hpp"

namespace sim {

namespace {

// All trait ranges are symmetric -1..+1.
constexpr double TRAIT_RANGE = 1.0;

void store_u16(std::uint16_t& field, float v) noexcept {
    assert(!detail::is_nan(static_cast<double>(v)));
    field = store_unit(field, static_cast<double>(v));
}

void add_u16(std::uint16_t& field, float delta) noexcept {
    assert(!detail::is_nan(static_cast<double>(delta)));
    field = add_unit(field, static_cast<double>(delta));
}

void store_i8(std::int8_t& field, float v) noexcept {
    assert(!detail::is_nan(static_cast<double>(v)));
    field = store_signed(field, static_cast<double>(v), TRAIT_RANGE);
    assert(field != INT8_MIN);
}

void add_i8(std::int8_t& field, float delta) noexcept {
    assert(!detail::is_nan(static_cast<double>(delta)));
    field = add_signed(field, static_cast<double>(delta), TRAIT_RANGE);
    assert(field != INT8_MIN);
}

float get_u16(std::uint16_t raw) noexcept { return dequantize_unit(raw); }
float get_i8(std::int8_t raw) noexcept { return dequantize_signed(raw, TRAIT_RANGE); }

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
float Character::strength() const noexcept { return get_i8(strength_); }
float Character::intelligence() const noexcept { return get_i8(intelligence_); }
float Character::stability() const noexcept { return get_i8(stability_); }
float Character::openness() const noexcept { return get_i8(openness_); }
float Character::extraversion() const noexcept { return get_i8(extraversion_); }
float Character::conscientiousness() const noexcept { return get_i8(conscientiousness_); }
float Character::agreeableness() const noexcept { return get_i8(agreeableness_); }
float Character::attractiveness() const noexcept { return get_i8(attractiveness_); }
float Character::height() const noexcept { return get_i8(height_); }
float Character::shape() const noexcept { return get_i8(shape_); }
float Character::charisma() const noexcept { return get_i8(charisma_); }

void Character::set_health(float v) noexcept { store_u16(health_, v); }
void Character::set_stress(float v) noexcept { store_u16(stress_, v); }
void Character::set_capacity(float v) noexcept { store_u16(capacity_, v); }
void Character::set_strength(float v) noexcept { store_i8(strength_, v); }
void Character::set_intelligence(float v) noexcept { store_i8(intelligence_, v); }
void Character::set_stability(float v) noexcept { store_i8(stability_, v); }
void Character::set_openness(float v) noexcept { store_i8(openness_, v); }
void Character::set_extraversion(float v) noexcept { store_i8(extraversion_, v); }
void Character::set_conscientiousness(float v) noexcept { store_i8(conscientiousness_, v); }
void Character::set_agreeableness(float v) noexcept { store_i8(agreeableness_, v); }
void Character::set_attractiveness(float v) noexcept { store_i8(attractiveness_, v); }
void Character::set_height(float v) noexcept { store_i8(height_, v); }
void Character::set_shape(float v) noexcept { store_i8(shape_, v); }
void Character::set_charisma(float v) noexcept { store_i8(charisma_, v); }

void Character::add_health(float delta) noexcept { add_u16(health_, delta); }
void Character::add_stress(float delta) noexcept { add_u16(stress_, delta); }
void Character::add_capacity(float delta) noexcept { add_u16(capacity_, delta); }
void Character::add_strength(float delta) noexcept { add_i8(strength_, delta); }
void Character::add_intelligence(float delta) noexcept { add_i8(intelligence_, delta); }
void Character::add_stability(float delta) noexcept { add_i8(stability_, delta); }
void Character::add_openness(float delta) noexcept { add_i8(openness_, delta); }
void Character::add_extraversion(float delta) noexcept { add_i8(extraversion_, delta); }
void Character::add_conscientiousness(float delta) noexcept { add_i8(conscientiousness_, delta); }
void Character::add_agreeableness(float delta) noexcept { add_i8(agreeableness_, delta); }
void Character::add_attractiveness(float delta) noexcept { add_i8(attractiveness_, delta); }
void Character::add_height(float delta) noexcept { add_i8(height_, delta); }
void Character::add_shape(float delta) noexcept { add_i8(shape_, delta); }
void Character::add_charisma(float delta) noexcept { add_i8(charisma_, delta); }

} // namespace sim
