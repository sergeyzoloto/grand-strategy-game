#pragma once

#include <cstdint>

namespace sim {

// Result of every list and relation edit. Check order: Invalid (invalid id or enum
// value, wrong kind), then NotFound for unknown characters, then Duplicate / Conflict /
// NotFound against existing entries, then Full. Every non-Ok result leaves state unchanged.
enum class EditResult : std::uint8_t {
    Ok = 0,
    Full = 1,      // a new entry was needed but a capacity or limit was reached; state unchanged
    Duplicate = 2, // the entry or bit already exists; state unchanged
    Conflict = 3,  // an existing entry or bit contradicts the edit; state unchanged
    NotFound = 4,  // unknown character, or removal of an absent entry or bit; state unchanged
    Invalid = 5,   // invalid id, enum value or kind; state unchanged
};

} // namespace sim
