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

# Manual benchmarks (Release only; not part of ctest; timings reported, never asserted)
cmake --build build/release --target sim_bench && ./build/release/bench/sim_bench
```

CMake options: `SIM_SANITIZE` (OFF), `SIM_WARNINGS_AS_ERRORS` (ON), `SIM_BUILD_TESTS` (ON),
`SIM_BUILD_BENCH` (ON).
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
- Every bipolar scale in the model uses integers -100..+100 (`BIPOLAR_MIN`/`BIPOLAR_MAX` in
  `sim/bipolar.hpp`), stored directly with raw == value: traits, reputation, stances, and scales
  added in later steps (opinions). Computed weak opinions are doubles on the same scale; rounding
  them to whole numbers is the caller's explicit choice.
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
  now, revisit (fixed-point) if lockstep multiplayer or replays are needed. Exception: opinion
  math (`sim/opinion.hpp`, `sim/noise.hpp`) uses only + - * / and comparisons on doubles, never
  exp, log or pow, so its results are identical across platforms under our flags; golden values
  (exact hexfloat comparisons) pin them in both builds.
- **CharacterRegistry is the only creator of characters** (passkey `CharacterKey`, whose private
  constructor only the registry can call; this relies on C++20, where a class with a user-declared
  constructor is not an aggregate, so `CharacterKey{}` cannot bypass it). Tests create characters
  in a local registry and copy them (`tests/test_support.hpp`). Ids start at 1, increase by one
  and are never reused. `Character` has no default constructor; copy and move construction are
  public, copy and move assignment are deleted so a slot never takes another character's identity.
  When characters can be removed later, relocate slots with `std::destroy_at` plus
  `std::construct_at`.
- **Lifetimes:** any pointer, reference or span obtained from the registry or from a Character is
  valid only until the next registry mutation (create or any relation edit): the registry may
  reallocate. Vectors returned by queries are independent copies.
- **Relations** live in `RelationGraph`, owned by the registry and edited only through it. One
  `RelationEdge {other, mask}` per ordered pair, stored per source and sorted by other; a bit on
  a -> b names b's role for a. `relation_info` is the single source of kind and complement:
  one-way types (Friend, Rival, Attraction) have no complement and touch only a -> b via
  `set_relation`/`clear_relation`; paired types always change both edges via `link`/`unlink`
  (Spouse is its own complement). Conflict checks and the symmetry invariant apply to paired
  types only. Empty edges are removed. Siblings are derived from shared parents and never stored;
  at most `MAX_PARENTS` = 2 parents. Not checked yet: longer cycles and birth-date sanity.
- **Edit results** are `EditResult` (`sim/edit_result.hpp`, formerly `ListResult`). Check order:
  Invalid (id 0, a == b, unknown enum, wrong kind), NotFound (unknown character), then Duplicate /
  Conflict / NotFound against existing state, then Full. A failed edit changes nothing.
- **Atomicity under allocation:** check everything first, then reserve room in every container
  an edit will grow (`detail::reserve_one_more`, geometric doubling), then write. Never
  `reserve(size() + 1)`: it reallocates on every insert.
- `Character` layout is pinned by `static_assert(sizeof(Character) == 448)` and by `offsetof`
  static_asserts in the constructor. The 30-byte core (id, name, birth, conditions, gender,
  traits) keeps offsets 0..29; practise (the only align-2 list) sits at 30, then nicknames (288),
  involvement (308), sacred (376) and reputation (444, appended in Step 4; 3 bytes tail padding).
  New fields are appended so existing offsets stay. Don't reorder without flagging it.
- **Per-character lists** (`sim/character_lists.hpp`) are `FixedVector`s inside Character; the
  public API never exposes `FixedVector`. Reads return `std::span<const Entry>`, valid only until
  the next mutation of that Character or until it is copied, moved or destroyed (Step 3 stores
  characters in a container). Every mutator returns a `[[nodiscard]] EditResult`. Check order:
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

- **StanceTable** (`sim/stance_table.hpp`) stores explicit stances from a community towards a
  TargetId (community or topic), -100..+100, and the community hierarchy (at most
  `MAX_COMMUNITY_DEPTH` = 6 per chain). An explicit 0 is a real value, not a removal. Resolution:
  for each source in chain(from), nearest first, try each target in chain(to), nearest first (a
  topic walks no chain); the first explicit entry wins, else 0. Storage is two sorted vectors
  with binary search; lookups never allocate. Public chain APIs return ids only: never expose
  index ranges into the vectors (they go stale after any edit). Caching index ranges waits
  until the benchmark shows it's needed, and then stays private. Bulk loading one insert at a
  time is O(n^2); a sort-once bulk loader comes later.
- **Noise** (`sim/noise.hpp`): full SplitMix64 steps over (world seed, source id, NoiseSubject,
  target id), mapped to [-1, 1]. `WorldSeed` is always passed explicitly. The hash and
  `NoiseSubject` values are frozen: changing them shifts the noise of the whole world, and
  golden values in the tests catch it.
- **Weak opinions** (`sim/opinion.hpp`) are computed, never stored or cached: community stances
  weighted by raw involvement weights (integer accumulation, one division), plus reputation,
  personality compatibility and noise; every coefficient is in `OpinionConfig`. Relations don't
  affect weak opinions.

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
- Randomized tests use `std::mt19937` with a fixed seed and derive values from its raw output
  (e.g. `rng() % n`); standard distributions differ between standard libraries. Property tests
  compare against a naive reference model and must stay fast in the Debug + ASan build.
- Tests must follow the lifetime rule too: create every character first, then take references
  (a reference into the registry dangles after the next create once the vector reallocates).
