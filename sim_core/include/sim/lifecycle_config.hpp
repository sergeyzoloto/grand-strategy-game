#pragma once

#include <cstdint>

namespace sim {

// Coefficients of the character lifecycle (memory, fame). All values are PLACEHOLDERS.
// Whole numbers, so fame stays exact.
struct LifecycleConfig {
    std::uint16_t fame_per_reputation = 1; // PLACEHOLDER: fame per point of |reputation| at death
    std::uint16_t legendary_fame = 100;    // PLACEHOLDER: fame at or above this is legendary
};

} // namespace sim
