#pragma once

#include <cassert>
#include <concepts>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "sim/date.hpp"
#include "sim/ids.hpp"

namespace sim {

// Every bipolar trait scale in the model is a whole number in -100..+100.
inline constexpr int BIPOLAR_MIN = -100;
inline constexpr int BIPOLAR_MAX = 100;

// Condition scales (health, stress, capacity) are fractional 0..100, stored in hundredths.
inline constexpr float CONDITION_MIN = 0.0f;
inline constexpr float CONDITION_MAX = 100.0f;

// Accepted argument types for bipolar mutators: any integer width and signedness.
// Floating point is rejected so fractional values never truncate silently; the
// caller rounds explicitly. bool and character types are rejected as well
// (std::cmp_less/std::cmp_greater do not accept them).
template<class T>
concept BipolarInteger = std::integral<T>
                      && !std::same_as<T, bool>
                      && !std::same_as<T, char>
                      && !std::same_as<T, wchar_t>
                      && !std::same_as<T, char8_t>
                      && !std::same_as<T, char16_t>
                      && !std::same_as<T, char32_t>;

// Extensible: append new values; never renumber existing ones (they are persisted).
enum class Gender : std::uint8_t {
    Female = 0,
    Male = 1,
};

// Initial values for a new Character. Condition fields are 0..100: out-of-range
// values saturate, NaN leaves the field at raw 0 (asserted in debug). Bipolar
// fields are whole numbers clamped to -100..+100.
struct CharacterInit {
    float health = 100.0f;          // PLACEHOLDER default, 0..100
    float stress = 0.0f;            // PLACEHOLDER default, 0..100
    float capacity = 100.0f;        // PLACEHOLDER default, 0..100
    int strength = 0;               // PLACEHOLDER default, -100..+100
    int intelligence = 0;           // PLACEHOLDER default, -100..+100
    int stability = 0;              // PLACEHOLDER default, -100..+100
    int openness = 0;               // PLACEHOLDER default, -100..+100
    int extraversion = 0;           // PLACEHOLDER default, -100..+100
    int conscientiousness = 0;      // PLACEHOLDER default, -100..+100
    int agreeableness = 0;          // PLACEHOLDER default, -100..+100
    int attractiveness = 0;         // PLACEHOLDER default, -100..+100
    int height = 0;                 // PLACEHOLDER default, -100..+100
    int shape = 0;                  // PLACEHOLDER default, -100..+100
    int charisma = 0;               // PLACEHOLDER default, -100..+100
};

// Persistent character state. Trivially copyable, no pointers, no floating point,
// never allocates. Derived values (age, mortality) are free functions elsewhere.
//
// Condition fields (0..100): float getters; set_* and add_* take float, round to the
// nearest hundredth and saturate; infinities saturate, NaN leaves the field unchanged
// (asserted in debug). add_* rounds the delta to whole hundredths, so a delta below
// 0.005 rounds away by design.
//
// Bipolar fields: int getters; set_* and add_* take any BipolarInteger and clamp
// to -100..+100 without overflow for every input value.
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

    [[nodiscard]] int strength() const noexcept { return strength_; }
    [[nodiscard]] int intelligence() const noexcept { return intelligence_; }
    [[nodiscard]] int stability() const noexcept { return stability_; }
    [[nodiscard]] int openness() const noexcept { return openness_; }
    [[nodiscard]] int extraversion() const noexcept { return extraversion_; }
    [[nodiscard]] int conscientiousness() const noexcept { return conscientiousness_; }
    [[nodiscard]] int agreeableness() const noexcept { return agreeableness_; }
    [[nodiscard]] int attractiveness() const noexcept { return attractiveness_; }
    [[nodiscard]] int height() const noexcept { return height_; }
    [[nodiscard]] int shape() const noexcept { return shape_; }
    [[nodiscard]] int charisma() const noexcept { return charisma_; }

    void set_health(float v) noexcept;
    void set_stress(float v) noexcept;
    void set_capacity(float v) noexcept;

    void add_health(float delta) noexcept;
    void add_stress(float delta) noexcept;
    void add_capacity(float delta) noexcept;

    void set_strength(BipolarInteger auto v) noexcept { strength_ = clamp_bipolar(v); }
    void set_intelligence(BipolarInteger auto v) noexcept { intelligence_ = clamp_bipolar(v); }
    void set_stability(BipolarInteger auto v) noexcept { stability_ = clamp_bipolar(v); }
    void set_openness(BipolarInteger auto v) noexcept { openness_ = clamp_bipolar(v); }
    void set_extraversion(BipolarInteger auto v) noexcept { extraversion_ = clamp_bipolar(v); }
    void set_conscientiousness(BipolarInteger auto v) noexcept { conscientiousness_ = clamp_bipolar(v); }
    void set_agreeableness(BipolarInteger auto v) noexcept { agreeableness_ = clamp_bipolar(v); }
    void set_attractiveness(BipolarInteger auto v) noexcept { attractiveness_ = clamp_bipolar(v); }
    void set_height(BipolarInteger auto v) noexcept { height_ = clamp_bipolar(v); }
    void set_shape(BipolarInteger auto v) noexcept { shape_ = clamp_bipolar(v); }
    void set_charisma(BipolarInteger auto v) noexcept { charisma_ = clamp_bipolar(v); }

    void add_strength(BipolarInteger auto d) noexcept { strength_ = add_bipolar(strength_, d); }
    void add_intelligence(BipolarInteger auto d) noexcept { intelligence_ = add_bipolar(intelligence_, d); }
    void add_stability(BipolarInteger auto d) noexcept { stability_ = add_bipolar(stability_, d); }
    void add_openness(BipolarInteger auto d) noexcept { openness_ = add_bipolar(openness_, d); }
    void add_extraversion(BipolarInteger auto d) noexcept { extraversion_ = add_bipolar(extraversion_, d); }
    void add_conscientiousness(BipolarInteger auto d) noexcept { conscientiousness_ = add_bipolar(conscientiousness_, d); }
    void add_agreeableness(BipolarInteger auto d) noexcept { agreeableness_ = add_bipolar(agreeableness_, d); }
    void add_attractiveness(BipolarInteger auto d) noexcept { attractiveness_ = add_bipolar(attractiveness_, d); }
    void add_height(BipolarInteger auto d) noexcept { height_ = add_bipolar(height_, d); }
    void add_shape(BipolarInteger auto d) noexcept { shape_ = add_bipolar(shape_, d); }
    void add_charisma(BipolarInteger auto d) noexcept { charisma_ = add_bipolar(charisma_, d); }

private:
    // Clamps any integer to -100..+100 using mixed-sign-safe comparisons.
    [[nodiscard]] static constexpr std::int8_t clamp_bipolar(BipolarInteger auto v) noexcept {
        if (std::cmp_less(v, BIPOLAR_MIN)) {
            return static_cast<std::int8_t>(BIPOLAR_MIN);
        }
        if (std::cmp_greater(v, BIPOLAR_MAX)) {
            return static_cast<std::int8_t>(BIPOLAR_MAX);
        }
        return static_cast<std::int8_t>(v);
    }

    // Adds a delta and clamps. The delta is first clamped to [-200, 200], which
    // already spans the whole scale, so the int sum cannot overflow.
    [[nodiscard]] static constexpr std::int8_t add_bipolar(std::int8_t current, BipolarInteger auto delta) noexcept {
        assert(current >= BIPOLAR_MIN && current <= BIPOLAR_MAX);
        const int span = BIPOLAR_MAX - BIPOLAR_MIN;
        int d = 0;
        if (std::cmp_less(delta, -span)) {
            d = -span;
        } else if (std::cmp_greater(delta, span)) {
            d = span;
        } else {
            d = static_cast<int>(delta);
        }
        return clamp_bipolar(static_cast<int>(current) + d);
    }

    // Field order is by alignment (4, 2, 1 bytes) for density; 2 bytes tail padding.
    CharacterId id_;             // u32 handle, 0 = invalid
    NameId name_;                // u32 handle, 0 = invalid
    Date birth_;                 // i32 weeks since world start, may be negative

    std::uint16_t health_ = 0;   // 0..100 in hundredths: raw 0..10000, value = raw / 100
    std::uint16_t stress_ = 0;   // 0..100 in hundredths: raw 0..10000, value = raw / 100
    std::uint16_t capacity_ = 0; // 0..100 in hundredths: raw 0..10000, value = raw / 100

    Gender gender_;              // u8 enum

    // Bipolar fields: -100..+100, one unit per step, stored directly (raw == value).
    std::int8_t strength_ = 0;          // -100..+100 units, stored directly
    std::int8_t intelligence_ = 0;      // -100..+100 units, stored directly
    // Big Five; stability = -neuroticism.
    std::int8_t stability_ = 0;         // -100..+100 units, stored directly
    std::int8_t openness_ = 0;          // -100..+100 units, stored directly
    std::int8_t extraversion_ = 0;      // -100..+100 units, stored directly
    std::int8_t conscientiousness_ = 0; // -100..+100 units, stored directly
    std::int8_t agreeableness_ = 0;     // -100..+100 units, stored directly
    std::int8_t attractiveness_ = 0;    // -100..+100 units, stored directly
    std::int8_t height_ = 0;            // -100..+100 units, stored directly
    std::int8_t shape_ = 0;             // -100..+100 units, stored directly
    std::int8_t charisma_ = 0;          // -100..+100 units, stored directly
};

static_assert(std::is_trivially_copyable_v<Character>);
static_assert(std::is_standard_layout_v<Character>);
static_assert(sizeof(Character) == 32, "Character layout changed; update the plan and field comments");

} // namespace sim
