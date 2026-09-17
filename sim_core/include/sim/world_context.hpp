#pragma once

#include "sim/lifecycle_config.hpp"
#include "sim/noise.hpp"
#include "sim/opinion_config.hpp"
#include "sim/stance_table.hpp"

namespace sim {

// The world-wide inputs an operation reads besides the registry: stances, coefficients and
// the world seed. Holds references, so it is only ever passed as a parameter (built at the
// call site) and never stored: a stored WorldContext would dangle once the referenced
// objects move or die. Not persistent state.
struct WorldContext {
    const StanceTable& stances;
    const OpinionConfig& opinion;
    const LifecycleConfig& lifecycle;
    WorldSeed seed;
};

} // namespace sim
