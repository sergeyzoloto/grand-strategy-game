#pragma once

#include "sim/character.hpp"
#include "sim/character_registry.hpp"

// Characters are created only by CharacterRegistry. Tests that need a standalone
// Character create it in a local registry and keep a copy (copy construction is public).
inline sim::Character make_character(sim::NameId name, sim::Gender gender, sim::Date birth,
                                     const sim::CharacterInit& init = {}) {
    sim::CharacterRegistry registry;
    const sim::CharacterId id = registry.create(name, gender, birth, init);
    return *registry.find(id);
}
