#pragma once

#include <cstddef>

#include "sim/character.hpp"
#include "sim/ids.hpp"
#include "sim/opinion_records.hpp"

namespace sim {

// Personal opinions: a stored long-term value per target, and short-term modifiers whose
// effects are summed on read. Nothing here depends on time. Writes go only through
// CharacterRegistry; these reads never write.

// PERSON_LIMIT_MIN + (PERSON_LIMIT_MAX - PERSON_LIMIT_MIN) * (extraversion + 100) / 200,
// rounded, in integer arithmetic: 8, 24 and 40 at -100, 0 and +100.
[[nodiscard]] std::size_t person_limit(const Character& a) noexcept;

// Long-term value of a about the target, -200..+200; 0 without an entry.
[[nodiscard]] int long_opinion(const Character& a, CharacterId target) noexcept;
[[nodiscard]] int long_opinion(const Character& a, TargetId target) noexcept;

// True if a has at least one modifier whose person target is `target` (any effect,
// including 0).
[[nodiscard]] bool has_modifier(const Character& a, CharacterId target) noexcept;

// Sum of the effects of a's modifiers on the target; 0 without any. At most
// MODIFIER_CAP * 100 in magnitude.
[[nodiscard]] int short_opinion(const Character& a, CharacterId target) noexcept;
[[nodiscard]] int short_opinion(const Character& a, TargetId target) noexcept;

} // namespace sim
