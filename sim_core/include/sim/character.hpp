#pragma once

#include <cstdint>
#include <type_traits>

#include "sim/date.hpp"
#include "sim/ids.hpp"

namespace sim {

// Extensible: append new values; never renumber existing ones (they are persisted).
enum class Gender : std::uint8_t {
    Female = 0,
    Male = 1,
};

// Initial values for the quantized fields of a new Character, in value space.
// Out-of-range values saturate; NaN leaves the field at raw 0 (asserted in debug).
struct CharacterInit {
    float health = 1.0f;            // PLACEHOLDER default, 0..1
    float stress = 0.0f;            // PLACEHOLDER default, 0..1
    float capacity = 1.0f;          // PLACEHOLDER default, 0..1
    float strength = 0.0f;          // PLACEHOLDER default, -1..+1
    float intelligence = 0.0f;      // PLACEHOLDER default, -1..+1
    float stability = 0.0f;         // PLACEHOLDER default, -1..+1
    float openness = 0.0f;          // PLACEHOLDER default, -1..+1
    float extraversion = 0.0f;      // PLACEHOLDER default, -1..+1
    float conscientiousness = 0.0f; // PLACEHOLDER default, -1..+1
    float agreeableness = 0.0f;     // PLACEHOLDER default, -1..+1
    float attractiveness = 0.0f;    // PLACEHOLDER default, -1..+1
    float height = 0.0f;            // PLACEHOLDER default, -1..+1
    float shape = 0.0f;             // PLACEHOLDER default, -1..+1
    float charisma = 0.0f;          // PLACEHOLDER default, -1..+1
};

// Persistent character state. Trivially copyable, no pointers, no floating point,
// never allocates. Derived values (age, mortality) are free functions elsewhere.
//
// Getters return value space (float). set_* and add_* take value space, round to
// nearest and saturate at the bounds; infinities saturate, NaN leaves the field
// unchanged (asserted in debug). See quantize.hpp for the resolution rule on int8
// fields: adds smaller than half a step (1/254) are no-ops by design.
class Character {
public:
    Character(CharacterId id, NameId name, Gender gender, Date birth, const CharacterInit& init) noexcept;

    [[nodiscard]] CharacterId id() const noexcept { return id_; }
    [[nodiscard]] NameId name() const noexcept { return name_; }
    [[nodiscard]] Gender gender() const noexcept { return gender_; }
    [[nodiscard]] Date birth() const noexcept { return birth_; }

    [[nodiscard]] float health() const noexcept;
    [[nodiscard]] float stress() const noexcept;
    [[nodiscard]] float capacity() const noexcept;
    [[nodiscard]] float strength() const noexcept;
    [[nodiscard]] float intelligence() const noexcept;
    [[nodiscard]] float stability() const noexcept;
    [[nodiscard]] float openness() const noexcept;
    [[nodiscard]] float extraversion() const noexcept;
    [[nodiscard]] float conscientiousness() const noexcept;
    [[nodiscard]] float agreeableness() const noexcept;
    [[nodiscard]] float attractiveness() const noexcept;
    [[nodiscard]] float height() const noexcept;
    [[nodiscard]] float shape() const noexcept;
    [[nodiscard]] float charisma() const noexcept;

    void set_health(float v) noexcept;
    void set_stress(float v) noexcept;
    void set_capacity(float v) noexcept;
    void set_strength(float v) noexcept;
    void set_intelligence(float v) noexcept;
    void set_stability(float v) noexcept;
    void set_openness(float v) noexcept;
    void set_extraversion(float v) noexcept;
    void set_conscientiousness(float v) noexcept;
    void set_agreeableness(float v) noexcept;
    void set_attractiveness(float v) noexcept;
    void set_height(float v) noexcept;
    void set_shape(float v) noexcept;
    void set_charisma(float v) noexcept;

    void add_health(float delta) noexcept;
    void add_stress(float delta) noexcept;
    void add_capacity(float delta) noexcept;
    void add_strength(float delta) noexcept;
    void add_intelligence(float delta) noexcept;
    void add_stability(float delta) noexcept;
    void add_openness(float delta) noexcept;
    void add_extraversion(float delta) noexcept;
    void add_conscientiousness(float delta) noexcept;
    void add_agreeableness(float delta) noexcept;
    void add_attractiveness(float delta) noexcept;
    void add_height(float delta) noexcept;
    void add_shape(float delta) noexcept;
    void add_charisma(float delta) noexcept;

private:
    // Field order is by alignment (4, 2, 1 bytes) for density; 2 bytes tail padding.
    CharacterId id_;             // u32 handle, 0 = invalid
    NameId name_;                // u32 handle, 0 = invalid
    Date birth_;                 // i32 weeks since world start, may be negative

    std::uint16_t health_ = 0;   // 0..1, value = raw / 65535
    std::uint16_t stress_ = 0;   // 0..1, value = raw / 65535
    std::uint16_t capacity_ = 0; // 0..1, value = raw / 65535

    Gender gender_;              // u8 enum

    std::int8_t strength_ = 0;          // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t intelligence_ = 0;      // -1..+1, raw in [-127, 127], value = raw / 127
    // Big Five; stability = -neuroticism.
    std::int8_t stability_ = 0;         // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t openness_ = 0;          // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t extraversion_ = 0;      // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t conscientiousness_ = 0; // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t agreeableness_ = 0;     // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t attractiveness_ = 0;    // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t height_ = 0;            // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t shape_ = 0;             // -1..+1, raw in [-127, 127], value = raw / 127
    std::int8_t charisma_ = 0;          // -1..+1, raw in [-127, 127], value = raw / 127
};

static_assert(std::is_trivially_copyable_v<Character>);
static_assert(std::is_standard_layout_v<Character>);
static_assert(sizeof(Character) == 32, "Character layout changed; update the plan and field comments");

} // namespace sim
