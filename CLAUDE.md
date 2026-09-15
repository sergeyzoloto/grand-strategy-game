# Grand strategy simulation core

Portable C++20 simulation core (`sim_core/`, namespace `sim`) for a historical grand
strategy game spanning ~12,000 years. `wiki/` holds design documentation, not code.
Engine not chosen yet; no engine types.

## Build and test

Requires CMake >= 3.20 and a C++20 compiler. doctest is vendored in `third_party/doctest/`.

```sh
# Debug + ASan/UBSan (default development build)
cmake -S . -B build/debug-asan -G Ninja -DCMAKE_BUILD_TYPE=Debug -DSIM_SANITIZE=ON
cmake --build build/debug-asan
ctest --test-dir build/debug-asan --output-on-failure

# Release
cmake -S . -B build/release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build/release
ctest --test-dir build/release --output-on-failure

# Verbose test output (shows MESSAGE lines such as sizeof(Character))
./build/debug-asan/tests/sim_tests -s
```

CMake options: `SIM_SANITIZE` (OFF), `SIM_WARNINGS_AS_ERRORS` (ON), `SIM_BUILD_TESTS` (ON).
Warnings: `-Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -Wshadow -Werror`
(MSVC: `/W4 /WX /permissive-`), applied to our own targets only via `sim_configure_target`;
doctest is a SYSTEM include. `CMAKE_CXX_EXTENSIONS OFF`, `-ffp-contract=off`; never `-ffast-math`.

## Standing rules

- Work only on the current step. Plan first, with no code, then stop and wait for confirmation.
- Don't change an existing public API or data layout without flagging it in the plan.
- C++20 and the standard library only; no engine types or APIs; no compiler extensions; no #pragma pack (density comes from field order).
- Persistent state holds no pointers and no floating point. Character stays trivially copyable and never allocates.
- No global state and no hidden RNG: seeds and configs are passed explicitly.
- Every gameplay coefficient lives in a config struct and is marked as a placeholder.
- Every persistent field has a comment stating its units and quantization.
- Invariants are asserted in debug builds; input errors are reported through return values, not exceptions.
- The build has zero warnings and all tests pass at the end of every step.
- No SoA, SIMD, pools or hot/cold splitting unless profiling asks for it.
- Commit once at the end of each step, after the tests pass and the report is written.

## Design rules established so far

- **Quantization** (`sim/quantize.hpp`): signed -R..+R stores raw in [-MAX, +MAX], never the
  type minimum; unit 0..1 stores raw / MAX. Writes round to nearest (halves away from zero)
  and saturate; infinities saturate; NaN leaves the field unchanged (asserted in debug).
  Bounds are checked before any float-to-int conversion.
- **Resolution rule:** int8 fields (step 1/127) are for rare, meaningful changes of at least one
  step; an add smaller than half a step is a no-op by design. If a trait needs slow weekly
  drift, widen that field to int16. No random rounding. The small rounding bias from repeated
  adds on uint16 fields is accepted.
- **Derived values** (age, mortality) are free functions and never stored.
- **Floating point** in getters and derived math is not bit-identical across platforms; fine for
  now, revisit (fixed-point) if lockstep multiplayer or replays are needed.
- `Character` has no default constructor. Only the registry (Step 3) creates characters; an
  invalid `CharacterId` is a programmer error (debug assert).
- `Character` layout is pinned by `static_assert(sizeof(Character) == 32)`.
- Adding a `Gender` value: append (never renumber) and handle it in every `switch`
  (no `default:`, so `-Wswitch` flags omissions, e.g. in `mortality.cpp`).

## Conventions

- Types and enumerators: `PascalCase` (`CharacterId`, `Gender::Female`).
- Functions and variables: `snake_case` (`age_years`, `add_health`).
- Constants: `UPPER_CASE` (`WEEKS_PER_YEAR`).
- Private data members: `snake_case_` with trailing underscore.
- New id type: one `using XId = Id<struct XIdTag>;` line in `sim/ids.hpp`.
- Headers in `sim_core/include/sim/`, included as `"sim/foo.hpp"`; sources in `sim_core/src/`.
- Tests in `tests/test_<area>.cpp`, doctest, one file per area.
- Comments in English. Placeholder coefficients and defaults are marked `PLACEHOLDER`.
