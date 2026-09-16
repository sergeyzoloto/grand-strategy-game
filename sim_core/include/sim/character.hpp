#pragma once

#include <cassert>
#include <cstdint>
#include <optional>
#include <span>
#include <type_traits>
#include <utility>

#include "sim/bipolar.hpp"
#include "sim/character_lists.hpp"
#include "sim/date.hpp"
#include "sim/fixed_vector.hpp"
#include "sim/ids.hpp"
#include "sim/integral.hpp"
#include "sim/strong_opinion_record.hpp"

namespace sim {

// Condition scales (health, stress, capacity) are fractional 0..100, stored in hundredths.
inline constexpr float CONDITION_MIN = 0.0f;
inline constexpr float CONDITION_MAX = 100.0f;

// Initial value of a bipolar field, used only in CharacterInit. Converts implicitly
// from any StrictIntegral and clamps to -100..+100, so designated initializers
// cannot truncate a float or wrap a wide integer. Not persistent state. Fully
// constexpr so CharacterInit{} stays usable in constant expressions.
class BipolarInit {
public:
    constexpr BipolarInit() noexcept = default;
    constexpr BipolarInit(StrictIntegral auto v) noexcept : value_(detail::clamp_bipolar(v)) {}

    [[nodiscard]] constexpr int value() const noexcept { return value_; }

private:
    std::int8_t value_ = 0; // -100..+100 units, already clamped
};

// Extensible: append new values; never renumber existing ones (they are persisted).
enum class Gender : std::uint8_t {
    Female = 0,
    Male = 1,
};

// Initial values for a new Character. Condition fields are 0..100: out-of-range
// values saturate, NaN leaves the field at its default (asserted in debug). Bipolar
// fields accept integers only and clamp to -100..+100 (see BipolarInit).
struct CharacterInit {
    float health = 100.0f;          // PLACEHOLDER default, 0..100
    float stress = 0.0f;            // PLACEHOLDER default, 0..100
    float capacity = 100.0f;        // PLACEHOLDER default, 0..100
    BipolarInit strength{};          // PLACEHOLDER default, -100..+100
    BipolarInit intelligence{};      // PLACEHOLDER default, -100..+100
    BipolarInit stability{};         // PLACEHOLDER default, -100..+100
    BipolarInit openness{};          // PLACEHOLDER default, -100..+100
    BipolarInit extraversion{};      // PLACEHOLDER default, -100..+100
    BipolarInit conscientiousness{}; // PLACEHOLDER default, -100..+100
    BipolarInit agreeableness{};     // PLACEHOLDER default, -100..+100
    BipolarInit attractiveness{};    // PLACEHOLDER default, -100..+100
    BipolarInit height{};            // PLACEHOLDER default, -100..+100
    BipolarInit shape{};             // PLACEHOLDER default, -100..+100
    BipolarInit charisma{};          // PLACEHOLDER default, -100..+100
    BipolarInit reputation{};        // PLACEHOLDER default, -100..+100
};

class CharacterRegistry;
struct OpinionConfig;

// Passkey: only CharacterRegistry can create one, so only the registry constructs
// Characters and calls the keyed mutators (strong opinions). Relies on C++20: a class with a user-declared constructor is not an
// aggregate, so CharacterKey{} cannot bypass the private constructor.
class CharacterKey {
    constexpr CharacterKey() noexcept = default;
    friend class CharacterRegistry;
};

// Persistent character state. Trivially copyable, no pointers, no floating point,
// never allocates. Derived values (age, mortality) are free functions elsewhere.
//
// Condition fields (0..100): float getters; set_* and add_* take float, round to the
// nearest hundredth and saturate; infinities saturate, NaN leaves the field unchanged
// (asserted in debug). add_* rounds the delta to whole hundredths, so a delta below
// 0.005 rounds away by design.
//
// Bipolar fields: int getters; set_* and add_* take any StrictIntegral and clamp
// to -100..+100 without overflow for every input value.
//
// Lists (nicknames, practise, involvement, sacred) start empty and are not part of
// CharacterInit. Every mutator returns a [[nodiscard]] EditResult; reads return
// spans of const entries.
class Character {
public:
    Character(CharacterKey key, CharacterId id, NameId name, Gender gender, Date birth,
              const CharacterInit& init) noexcept;

    // Copy and move construction stay public (trivially copyable). Assignment is
    // deleted so a registry slot can never take over another character's identity;
    // relocating slots uses std::destroy_at plus std::construct_at instead.
    Character(const Character&) noexcept = default;
    Character(Character&&) noexcept = default;
    Character& operator=(const Character&) = delete;
    Character& operator=(Character&&) = delete;
    ~Character() = default;

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
    [[nodiscard]] int reputation() const noexcept { return reputation_; }

    void set_health(float v) noexcept;
    void set_stress(float v) noexcept;
    void set_capacity(float v) noexcept;

    void add_health(float delta) noexcept;
    void add_stress(float delta) noexcept;
    void add_capacity(float delta) noexcept;

    void set_strength(StrictIntegral auto v) noexcept { strength_ = detail::clamp_bipolar(v); }
    void set_intelligence(StrictIntegral auto v) noexcept { intelligence_ = detail::clamp_bipolar(v); }
    void set_stability(StrictIntegral auto v) noexcept { stability_ = detail::clamp_bipolar(v); }
    void set_openness(StrictIntegral auto v) noexcept { openness_ = detail::clamp_bipolar(v); }
    void set_extraversion(StrictIntegral auto v) noexcept { extraversion_ = detail::clamp_bipolar(v); }
    void set_conscientiousness(StrictIntegral auto v) noexcept { conscientiousness_ = detail::clamp_bipolar(v); }
    void set_agreeableness(StrictIntegral auto v) noexcept { agreeableness_ = detail::clamp_bipolar(v); }
    void set_attractiveness(StrictIntegral auto v) noexcept { attractiveness_ = detail::clamp_bipolar(v); }
    void set_height(StrictIntegral auto v) noexcept { height_ = detail::clamp_bipolar(v); }
    void set_shape(StrictIntegral auto v) noexcept { shape_ = detail::clamp_bipolar(v); }
    void set_charisma(StrictIntegral auto v) noexcept { charisma_ = detail::clamp_bipolar(v); }
    void set_reputation(StrictIntegral auto v) noexcept { reputation_ = detail::clamp_bipolar(v); }

    void add_strength(StrictIntegral auto d) noexcept { strength_ = add_bipolar(strength_, d); }
    void add_intelligence(StrictIntegral auto d) noexcept { intelligence_ = add_bipolar(intelligence_, d); }
    void add_stability(StrictIntegral auto d) noexcept { stability_ = add_bipolar(stability_, d); }
    void add_openness(StrictIntegral auto d) noexcept { openness_ = add_bipolar(openness_, d); }
    void add_extraversion(StrictIntegral auto d) noexcept { extraversion_ = add_bipolar(extraversion_, d); }
    void add_conscientiousness(StrictIntegral auto d) noexcept { conscientiousness_ = add_bipolar(conscientiousness_, d); }
    void add_agreeableness(StrictIntegral auto d) noexcept { agreeableness_ = add_bipolar(agreeableness_, d); }
    void add_attractiveness(StrictIntegral auto d) noexcept { attractiveness_ = add_bipolar(attractiveness_, d); }
    void add_height(StrictIntegral auto d) noexcept { height_ = add_bipolar(height_, d); }
    void add_shape(StrictIntegral auto d) noexcept { shape_ = add_bipolar(shape_, d); }
    void add_charisma(StrictIntegral auto d) noexcept { charisma_ = add_bipolar(charisma_, d); }
    void add_reputation(StrictIntegral auto d) noexcept { reputation_ = add_bipolar(reputation_, d); }

    // ---- nicknames: insertion order, no duplicates ----

    // The span is valid only until the next mutation of this Character, or until
    // the Character is copied, moved or destroyed.
    [[nodiscard]] std::span<const NameId> nicknames() const noexcept {
        return {nicknames_.data(), nicknames_.size()};
    }
    // Ok, Invalid, Duplicate or Full. Appends.
    [[nodiscard]] EditResult add_nickname(NameId name) noexcept;
    // Ok, Invalid or NotFound. Keeps the order of the remaining nicknames.
    [[nodiscard]] EditResult remove_nickname(NameId name) noexcept;

    // ---- practise: skills as capabilities, sorted by (kind, id), never evicted ----

    // The span is valid only until the next mutation of this Character, or until
    // the Character is copied, moved or destroyed.
    [[nodiscard]] std::span<const PractiseEntry> practise() const noexcept {
        return {practise_.data(), practise_.size()};
    }
    [[nodiscard]] bool has_skill(SkillKind kind, SkillId skill) const noexcept;
    // Ok, Invalid, Duplicate or Full.
    [[nodiscard]] EditResult add_skill(SkillKind kind, SkillId skill) noexcept;
    // Ok, Invalid or NotFound.
    [[nodiscard]] EditResult remove_skill(SkillKind kind, SkillId skill) noexcept;

    // ---- involvement: weighted communities, sorted by community id ----

    // The span is valid only until the next mutation of this Character, or until
    // the Character is copied, moved or destroyed.
    [[nodiscard]] std::span<const InvolvementEntry> involvement() const noexcept {
        return {involvement_.data(), involvement_.size()};
    }
    // Whole-number weight clamped to 0..255; 0 removes (Ok also when absent).
    // Ok, Invalid or Full.
    [[nodiscard]] EditResult set_involvement(CommunityId community, StrictIntegral auto weight) noexcept {
        return set_involvement_weight(
            community, static_cast<std::uint8_t>(detail::clamp_integer(weight, 0, INVOLVEMENT_WEIGHT_MAX)));
    }
    // Exact sum of raw weights, 0..8 * 255. Use for weighted sums; divide once.
    [[nodiscard]] int involvement_total() const noexcept;
    // weight / total computed in double, 0 if absent. Never adjusted to force an
    // exact sum, so equal weights give equal shares.
    [[nodiscard]] float involvement_share(CommunityId community) const noexcept;
    // Largest weight, ties to the smaller id; empty when there is no involvement.
    [[nodiscard]] std::optional<CommunityId> main_community() const noexcept;

    // ---- sacred: signed targets, sorted by target ----

    // The span is valid only until the next mutation of this Character, or until
    // the Character is copied, moved or destroyed.
    [[nodiscard]] std::span<const SacredEntry> sacred() const noexcept {
        return {sacred_.data(), sacred_.size()};
    }
    // Ok, Invalid, Duplicate (same sign), Conflict (opposite sign) or Full.
    // Flipping a sign takes an explicit remove_sacred first.
    [[nodiscard]] EditResult add_sacred(TargetId target, SacredSign sign) noexcept;
    // Ok, Invalid or NotFound.
    [[nodiscard]] EditResult remove_sacred(TargetId target) noexcept;
    [[nodiscard]] std::optional<SacredSign> sacred_sign(TargetId target) const noexcept;

    // ---- strong opinions: remembered deviations from the weak opinion ----
    // Reads are public; mutations go only through CharacterRegistry (keyed members).

    // Opinions about people, sorted by CharacterId. The span is valid only until the
    // next mutation of this Character, or until it is copied, moved or destroyed.
    [[nodiscard]] std::span<const StrongOpinion> strong_people() const noexcept {
        return {strong_people_.data(), strong_people_.size()};
    }
    // Opinions about communities and topics, sorted by TargetId raw value. The span is
    // valid only until the next mutation of this Character, or until it is copied,
    // moved or destroyed.
    [[nodiscard]] std::span<const StrongOpinion> strong_targets() const noexcept {
        return {strong_targets_.data(), strong_targets_.size()};
    }

    // Registry only (CharacterKey). `delta` is clamped to -200..+200. Returns Invalid for
    // an invalid target or target == id(); unknown characters are the registry's NotFound.
    [[nodiscard]] OpinionEventResult<CharacterId> apply_opinion_event(CharacterKey key, CharacterId target, int delta,
                                                                      Date now, CauseId cause,
                                                                      const OpinionConfig& config) noexcept;
    [[nodiscard]] OpinionEventResult<TargetId> apply_opinion_event(CharacterKey key, TargetId target, int delta,
                                                                   Date now, CauseId cause,
                                                                   const OpinionConfig& config) noexcept;
    // Registry only (CharacterKey). Removes records with |dev(now)| < exit_threshold, then
    // evicts the weakest people records until the list fits person_limit.
    [[nodiscard]] StrongMaintainCounts maintain_strong_opinions(CharacterKey key, Date now,
                                                                const OpinionConfig& config) noexcept;

private:
    [[nodiscard]] EditResult set_involvement_weight(CommunityId community, std::uint8_t weight) noexcept;
    [[nodiscard]] bool lists_valid() const noexcept; // debug invariant check

    // Adds a delta and clamps. The delta is first clamped to [-200, 200], which
    // already spans the whole scale, so the int sum cannot overflow.
    [[nodiscard]] static constexpr std::int8_t add_bipolar(std::int8_t current, StrictIntegral auto delta) noexcept {
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
        return detail::clamp_bipolar(static_cast<int>(current) + d);
    }

    // Core field order is by alignment (4, 2, 1 bytes) for density; offsets 0..29 are fixed.
    CharacterId id_;             // u32 handle, 0 = invalid
    NameId name_;                // u32 handle, 0 = invalid
    Date birth_;                 // i32 weeks since world start, may be negative

    // Starting values match the CharacterInit defaults, so a NaN init value (which
    // leaves the field unchanged in release) still yields a defined default.
    std::uint16_t health_ = 10000;   // 0..100 in hundredths: raw 0..10000, value = raw / 100
    std::uint16_t stress_ = 0;       // 0..100 in hundredths: raw 0..10000, value = raw / 100
    std::uint16_t capacity_ = 10000; // 0..100 in hundredths: raw 0..10000, value = raw / 100

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

    // Lists. The 30-byte core above keeps its offsets; practise (align 2) takes the
    // former tail padding at offset 30, then the align-4 lists follow.
    FixedVector<PractiseEntry, PRACTISE_CAP> practise_;          // skills, sorted by (kind, id), 258 bytes
    FixedVector<NameId, NICKNAME_CAP> nicknames_;                // insertion order, unique, 20 bytes
    FixedVector<InvolvementEntry, INVOLVEMENT_CAP> involvement_; // sorted by community, weight 1..255, 68 bytes
    FixedVector<SacredEntry, SACRED_CAP> sacred_;                // sorted by target, unique targets, 68 bytes

    // Appended in Step 4 so every earlier offset stays.
    std::int8_t reputation_ = 0;        // -100..+100 units, stored directly

    // Appended in Step 5 (from offset 448) so every earlier offset stays.
    FixedVector<StrongOpinion, PERSON_LIMIT_MAX> strong_people_; // sorted by CharacterId, size may exceed person_limit until maintain; 644 bytes
    FixedVector<StrongOpinion, TARGET_LIMIT> strong_targets_;    // sorted by TargetId raw; 260 bytes
};

static_assert(CharacterInit{}.health == 100.0f && CharacterInit{}.stress == 0.0f && CharacterInit{}.capacity == 100.0f,
              "CharacterInit condition defaults changed; update Character's starting raw values");
static_assert(CharacterInit{}.strength.value() == 0);
static_assert(std::is_trivially_copyable_v<Character>);
static_assert(!std::is_copy_assignable_v<Character> && !std::is_move_assignable_v<Character>);
static_assert(std::is_standard_layout_v<Character>);
static_assert(sizeof(Character) == 1352, "Character layout changed; update the plan and field comments");

} // namespace sim
