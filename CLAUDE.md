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
- Never reset, rebase or amend commits (or otherwise rewrite history) without asking first.
- Every bipolar trait scale in the model uses integers -100..+100 (`BIPOLAR_MIN`/`BIPOLAR_MAX`),
  stored directly with raw == value, including scales added in later steps (reputation, opinions).
- Fractional values reach bipolar scales only through explicit rounding by the caller. Bipolar
  `set_`/`add_` accept integral types only (`StrictIntegral` in `sim/integral.hpp`: no floats, bool
  or character types; also used for other whole-number inputs such as involvement weights);
  a float argument matches no function and is a hard error regardless of warning flags.
  Init structs hold bipolar fields as `BipolarInit` (same concept, clamps on conversion), so
  designated initializers cannot truncate a float or wrap a wide integer either. Never close
  such holes with compiler flags: the headers are included by targets with their own flags.
- Condition scales (health, stress, capacity) are fractional 0..100 (`CONDITION_MIN`/`CONDITION_MAX`),
  stored in hundredths (uint16 raw 0..10000, value = raw / 100), with a float API on purpose.

## Design rules established so far

- **Condition scales** round to the nearest hundredth with `std::round` and saturate at 0 and
  100; infinities saturate; NaN leaves the field unchanged (asserted in debug). Values are
  clamped before any float-to-integer conversion. `add_` rounds the *delta* to whole steps, then
  adds in a wide integer: the same delta always adds the same steps and add(d), add(-d) restores
  raw unless saturated. A delta below 0.005 rounds away by design. Fields start at the
  `CharacterInit` defaults, so a NaN init value in release still yields a defined value. Non-constant int arguments may
  need an explicit cast under `-Wconversion`. The hundredths helper is private to `character.cpp`;
  extract a shared one when a later step needs hundredths again.
- **Release-only NaN branches:** NaN input is asserted in debug, so the release fallback (field
  unchanged) is tested only by the `#ifdef NDEBUG` tests in the release build; the debug/ASan
  run skips them. Run both builds' tests before committing.
- **Bipolar scales are whole numbers**, so they never round silently. `set_` clamps any integer
  to -100..+100 with `std::cmp_less`/`std::cmp_greater`; `add_` clamps the delta to [-200, 200]
  first, so no integer width or signedness can overflow. -128 is never stored.
- **Derived values** (age, mortality) are free functions and never stored.
- **Floating point** in getters and derived math is not bit-identical across platforms; fine for
  now, revisit (fixed-point) if lockstep multiplayer or replays are needed.
- `Character` has no default constructor. Only the registry (Step 3) creates characters; an
  invalid `CharacterId` is a programmer error (debug assert).
- `Character` layout is pinned by `static_assert(sizeof(Character) == 444)` and by `offsetof`
  static_asserts in the constructor. The 30-byte core (id, name, birth, conditions, gender,
  traits) keeps offsets 0..29; practise (the only align-2 list) sits at 30, then nicknames (288),
  involvement (308) and sacred (376). Don't reorder without flagging it.
- **Per-character lists** (`sim/character_lists.hpp`) are `FixedVector`s inside Character; the
  public API never exposes `FixedVector`. Reads return `std::span<const Entry>`, valid only until
  the next mutation of that Character or until it is copied, moved or destroyed (Step 3 stores
  characters in a container). Every mutator returns a `[[nodiscard]] ListResult`. Check order:
  `Invalid` (invalid id or enum value) first, then `Duplicate`/`Conflict`/`NotFound` against
  existing entries, then `Full`. A set to 0 on an absent entry is `Ok` with no change; a remove of
  an absent entry is `NotFound`. List caps are storage bounds, not gameplay rules: never evict to
  make room. Entry structs have no implicit padding (explicit zeroed bytes, pinned with
  `std::has_unique_object_representations_v`), and FixedVector value-initializes freed slots.
- **Skills are capabilities**: a character has one or doesn't; `PractiseEntry` has no value (its
  reserved byte may hold a mastery level later). Capabilities granted by membership in a structure
  are derived from the character's communities and never stored on Character.
- **Involvement shares** are `weight / total` computed in double and never adjusted to force an
  exact sum, so equal weights give equal shares. For weighted sums use raw weights and
  `involvement_total()` and divide once. A share is not a scale: the -100..+100 and 0..100 rules
  don't apply to it.
- **TargetId** packs 2 bits of kind and 30 bits of index into u32; valid iff index != 0, built only
  via `TargetId::from`, which rejects invalid ids and indices >= 2^30. `TargetKind` is
  append-only: Community = 0, Topic = 1, 2 reserved for persons, 3 free.
- Adding a `Gender` value: append (never renumber) and handle it in every `switch`
  (no `default:`, so `-Wswitch` flags omissions, e.g. in `mortality.cpp`).

## Conventions

- Types and enumerators: `PascalCase` (`CharacterId`, `Gender::Female`).
- Functions and variables: `snake_case` (`age_years`, `add_health`).
- Constants: `UPPER_CASE` (`WEEKS_PER_YEAR`).
- Private data members: `snake_case_` with trailing underscore.
- New id type: one `using XId = Id<struct XIdTag>;` line in `sim/ids.hpp` (u16 ids:
  `Id<struct XIdTag, std::uint16_t>`; Rep must be an unsigned `StrictIntegral`).
- New enums that are persisted are append-only and validated with a `switch` without `default`.
- Headers in `sim_core/include/sim/`, included as `"sim/foo.hpp"`; sources in `sim_core/src/`.
- Tests in `tests/test_<area>.cpp`, doctest, one file per area.
- Comments in English. Placeholder coefficients and defaults are marked `PLACEHOLDER`.
