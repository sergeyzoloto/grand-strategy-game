#pragma once

#include <cstdint>
#include <type_traits>

#include "sim/character.hpp"
#include "sim/date.hpp"
#include "sim/ids.hpp"

namespace sim {

// What remains of a character after death: identity, dates, fame and the few values a weak
// opinion of the deceased needs. Deleted when no living character remembers it any more. Parents, children and spouses stay in the relation
// graph and are not copied here. Written once by CharacterRegistry::kill, never changed.
struct DeadRecord {
    CharacterId id;             // u32 handle, always valid
    NameId name;                // u32 handle
    Date birth;                 // i32 weeks since world start
    Date death;                 // i32 weeks since world start, >= birth
    CommunityId main_community; // u32 handle: Character::main_community() at death; invalid if none
    std::int8_t reputation = 0; // -100..+100 units, stored directly; frozen at death
    Gender gender = Gender::Female; // u8 enum
    std::uint16_t fame = 0;     // holders just before death + fame_per_reputation * |reputation|, saturating at 65535
};

static_assert(sizeof(DeadRecord) == 24 && alignof(DeadRecord) == 4);
static_assert(std::has_unique_object_representations_v<DeadRecord>);
static_assert(std::is_trivially_copyable_v<DeadRecord>);

} // namespace sim
