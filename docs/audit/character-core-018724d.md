# Audit: character core at commit 018724d

Read-only audit of `sim_core/` at `018724dc4eda1e6f29df5c6c96599457fdf8a799`, run on 2026-09-17.
Before the audit, HEAD was 018724d and `git status --porcelain` printed nothing.
The evidence comes from the code, the tests, fresh builds and small scratch probes compiled outside the repository.
Comments, commit messages and CLAUDE.md were not taken as evidence. Paths are relative to the repository root, and
`sim/x.hpp` means `sim_core/include/sim/x.hpp`.

Toolchain: GCC 13.3.0 (Ubuntu 24.04), CMake 3.28.3, Ninja, x86-64. MSVC was not built.

## 0. Summary

| Status | Count |
|---|---|
| Implemented | 41 |
| Unguarded | 14 |
| Deviates | 4 |
| Missing | 0 |
| Unverified | 0 (two MSVC-only sub-claims inside R2 and R3 are Unverified; see those rows) |

Rows that are not Implemented:

- R1 Unguarded: no check enforces C++20-only, `CMAKE_CXX_EXTENSIONS OFF` or no `#pragma pack`. The code complies (it uses `#pragma once` in 24 headers).
- R2 Unguarded: the flags are present and both builds have zero warnings, but no check notices if a flag is removed. MSVC: Unverified.
- R3 Unguarded: `-ffp-contract=off` is set, but the golden values would not notice its removal on baseline x86-64, which has no FMA.
- R4 Unguarded: there are no floats or pointers in persistent structs. Floats are excluded only for entry types and DeadRecord (by `has_unique_object_representations`); nothing excludes them from Character, and nothing excludes pointers anywhere.
- R5 Unguarded: trivially copyable and deleted assignment are static_asserted. "No default constructor" and "never allocates" are not.
- R6 Unguarded: grep finds no mutable globals, RNG or stored WorldContext. Nothing enforces it.
- R7 Unguarded: every config field is marked PLACEHOLDER, but no check enforces it. It conflicts with R41, whose limits are hard-coded constants.
- R8 Deviates: `WorldSeed::value` (`sim/noise.hpp:11`) has no units/quantization comment.
- R9 Unguarded: there are no `throw` statements and invariants are asserted. Nothing prevents a future throw.
- R10 Unguarded: several tests show that failed edits leave state unchanged. The bad_alloc path and "no allocation after reserve" are untested; the second is asserted only in RelationGraph.
- R11 Unguarded: all three property tests comply, but the rule itself is a convention.
- R21 Deviates: `involvement_share` returns `float`; the division is done in double and then narrowed.
- R23 Deviates: `FixedVector` appears in the public API (`Character::trim_long_opinions`, `CharacterRegistry::parents`, `CommunityChain`/`TargetChain`).
- R26 Deviates: the lifetime contract in `sim/character_registry.hpp:54-57` (and CLAUDE.md:107) omits `maintain()`, which erases list entries.
- R37 Unguarded: no test shows that relations leave opinions unchanged.
- R42 Unguarded: nothing in the API depends on time, but no check enforces that.
- R53 Unguarded: forgetting behaviour is tested. Id-order processing, "never allocates" and "never cascades" are not.
- R59 Unguarded: the bench counts outcomes, aborts sections and exits 1, but the abort path is never exercised and sim_bench is not in ctest.

## 1. Build and mechanical checks

Clean build directories were created in a scratch directory, never `build/`:

| Config | Command | Result |
|---|---|---|
| Debug + ASan/UBSan | `cmake -S . -B <tmp>/b-asan -G Ninja -DCMAKE_BUILD_TYPE=Debug -DSIM_SANITIZE=ON && cmake --build … && ctest …` | 27/27 steps, 0 warnings (`grep -ci warning` = 0), ctest 1/1 passed (7.11 s); doctest 123 cases passed, 0 skipped, 103,013 assertions |
| Release | same, `-DCMAKE_BUILD_TYPE=Release` | 27/27 steps, 0 warnings, ctest 1/1 passed (0.14 s); doctest 127 cases passed, 103,020 assertions |

The 4 extra Release cases are `#ifdef NDEBUG` blocks: `tests/test_character.cpp:352,361` and `tests/test_opinion.cpp:73,196`.
The compile lines in `compile_commands.json` contain `-std=c++20 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion
-Wshadow -ffp-contract=off -Werror`, and tests include doctest with `-isystem`.

| Check | Command (run at repo root) | Result |
|---|---|---|
| float/double members in persistent structs | `grep -nE '^\s*(float\|double\|long double)\s+\w+' sim/{character,character_lists,opinion_records,dead_record,relations,stance_table,ids,date,fixed_vector,noise}.hpp` | Only `CharacterInit::health/stress/capacity` (`sim/character.hpp:50-52`), which are init inputs, not stored state. None in Character, entries, records or DeadRecord. |
| pointers/references in persistent structs | `grep -nE '^\s*(const\s+)?[A-Za-z_:<>, ]+[*&]\s*\w+\s*(;\|=)' sim_core/include/sim/*.hpp` | Only `WorldContext`'s three references (`sim/world_context.hpp:15-17`), which is non-persistent by design. |
| #pragma, compiler attributes, -ffast-math | `grep -rnE '#\s*pragma\|__attribute__\|__builtin\|\[\[gnu::\|\[\[clang::\|__declspec\|__int128\|ffast-math\|Ofast' sim_core tests bench CMakeLists.txt` | Only `#pragma once` (24 headers) and a comment "Never use -ffast-math" (`CMakeLists.txt:31`). |
| exp/log/pow/sqrt outside mortality | `grep -rnE '\b(std::)?(exp\|expm1\|log\|log1p\|pow\|sqrt\|…)\s*\(' sim_core` | Hits only in `src/mortality.cpp:43-49,66` and in comments in `sim/mortality.hpp`. `std::round` is used in the kill rebase (`src/character_registry.cpp:189` via `clamp_round`); it is exact, so not a transcendental function. |
| rand, random_device, unseeded engines, mutable static/global | `grep -rnE 'mt19937\|_distribution\|random_device\|std::rand\|srand' sim_core`; `grep -rnE '\bstatic\b' sim_core \| grep -vE 'static_assert\|static_cast\|static constexpr…'`; `grep -rnE 'thread_local\|extern\|mutable' sim_core` | No hits in sim_core. Engines appear only in tests and bench, all `std::mt19937` with literal seeds. |
| throw in sim_core | `grep -rnE '\bthrow\b\|\btry\b\|\bcatch\b' sim_core` | Only comments ("may throw bad_alloc"). No `.value()` on optionals and no `.at()`. |
| PLACEHOLDER on every config field | read `OpinionConfig`, `LifecycleConfig`, `MortalityConfig`, `CharacterInit` | 10/10, 2/2, 6/6 and 15/15 fields marked. |

## 2. Requirements table

Rows under G9 cover whether the limit is recorded in CLAUDE.md and matches the code and this bench run. No check can guard a documentation requirement, so "Implemented" there means "recorded and accurate".

| Req | Status | Implementation | Checks |
|---|---|---|---|
| **G1** | | | |
| R1 | Unguarded | `CMakeLists.txt:5-7` (C++20, required, extensions OFF); std-only includes | None. `-Wpedantic -Werror` would reject most language extensions; sizeof static_asserts would catch a `#pragma pack` only on the pinned types. |
| R2 | Unguarded | `sim_configure_target` `CMakeLists.txt:17-44`; `SIM_WARNINGS_AS_ERRORS` ON `:11`; PRIVATE per target (`sim_core/CMakeLists.txt:14`, `tests/CMakeLists.txt:23`, `bench/CMakeLists.txt:4`); doctest `SYSTEM INTERFACE` `tests/CMakeLists.txt:2` | Zero warnings in both builds (section 1). Nothing notices a removed flag. The MSVC branch (`/W4 /permissive- /WX`) was not built: Unverified. |
| R3 | Unguarded | `-ffp-contract=off` `CMakeLists.txt:32`; no `-ffast-math` anywhere | Golden hexfloat tests catch contraction only on targets with FMA; the default x86-64 baseline has none. MSVC uses `/fp:precise` (`:19`); whether that disables contraction there is Unverified. |
| R4 | Unguarded | Character fields `sim/character.hpp:285-324` are integers or FixedVectors of integer entries; DeadRecord `sim/dead_record.hpp:15-24` | `has_unique_object_representations_v` (rejects floats) on all entry types and DeadRecord (`sim/character_lists.hpp:58-60`, `sim/opinion_records.hpp:34,54`, `sim/relations.hpp:104`, `sim/stance_table.hpp:44-45`, `sim/dead_record.hpp:27`). Character: sizeof only. Pointers: nothing. |
| R5 | Unguarded | Ctor needs a key `sim/character.hpp:93`; copy/move ctors defaulted, assignments deleted `:99-102`; storage is FixedVector | static_asserts `sim/character.hpp:330-331`, `tests/test_registry.cpp:13-17`. Not checked: `!is_default_constructible_v<Character>` (the probe says false today) and "never allocates". |
| R6 | Unguarded | `WorldSeed` passed as a parameter (`sim/noise.hpp:40`); `WorldContext` reference struct `sim/world_context.hpp:14-19`; configs passed by reference | Section 1 greps only. Nothing stops a class member of type WorldContext. |
| R7 | Unguarded | `sim/opinion_config.hpp:8-17`, `sim/lifecycle_config.hpp:10-11`, `sim/mortality.hpp:18-23`, `sim/character.hpp:50-64` | None. **Conflict:** the R41 limits (`PERSON_LIMIT_MIN/MAX`, `TARGET_LIMIT` in `sim/opinion_records.hpp:15-17`) and the `person_limit` formula are gameplay rules as hard-coded constants, not config fields and not PLACEHOLDER. |
| R8 | Deviates | Every stored field in Character, entries, DeadRecord, RelationEdge, StanceEntry, ParentEntry, FixedVector, Id and TargetId has a unit comment. Exception: `sim/noise.hpp:11` `std::uint64_t value = 0;` has none. | None. Ambiguous whether WorldSeed counts as persistent; it must be saved for noise to replay. No decision commit found (introduced in 8f3fcd3). |
| R9 | Unguarded | Debug invariants: `Character::lists_valid` (`src/character_lists.cpp:229`), `RelationGraph::pair_valid` (`src/relation_graph.cpp:332`), holders asserts (`src/character_registry.cpp:65,82,145,164`); errors via `EditResult`/`LongOpinionOutcome`; no `throw` | Grep (section 1). Release-only tests of the fallbacks: `tests/test_character.cpp:352,361`, `tests/test_opinion.cpp:73,196`. Nothing prevents adding a throw. |
| R10 | Unguarded | `detail::reserve_one_more` `sim/vector_growth.hpp:14`; used before writes in `src/character_registry.cpp:32-35,119-120`, `src/relation_graph.cpp:106,161-164`, `src/stance_table.cpp:36,96`; maintain reserves at `:458` | Asserts `src/relation_graph.cpp:52,75`. Unchanged-state tests: "Conflict leaves both sides unchanged", "parent limit: a third parent is Full…", "modifiers: Full at MODIFIER_CAP leaves state unchanged…", "hierarchy: the depth limit returns Full…". No bad_alloc injection test; kill and maintain have no post-reserve allocation assert. |
| R11 | Unguarded | `tests/test_relations.cpp:379`, `tests/test_personal_opinion.cpp:664`, `tests/test_death.cpp:171,712`: `std::mt19937` with literal seeds, `rng() % n` only | The three property tests compare against `ReferenceModel`, `PersonalModel` and `LifecycleModel`. No check enforces the convention. |
| **G2** | | | |
| R12 | Implemented | `sim/date.hpp:11` (`52.1775`), `:14-21` (`std::int32_t weeks`) | `static_assert(sizeof(Date) == 4)` `tests/test_date.cpp:14`; "negative birth dates work"; "mortality matches results recorded…" (a changed WEEKS_PER_YEAR moves results beyond 1e-6). |
| R13 | Implemented | int8 traits/reputation `sim/character.hpp:298-319`; `StanceEntry::value` int8; `OpinionModifier::effect` int8; `LongOpinion::value` int16 clamped to ±200 (`src/personal_opinion.cpp` `add_long`) | "bipolar set_ clamps…", "bipolar add_ … saturates…", "stances: set clamps…", "modifiers: effects clamp to +-100…", "long opinions: create, update, removal at 0, Unchanged, clamping". |
| R14 | Implemented | `StrictIntegral` `sim/integral.hpp:13-19`; `BipolarInit` `sim/character.hpp:29-38`; `set_involvement` `:195`; registry `add_modifier`/`add_long_opinion` `sim/character_registry.hpp:145-166`; `set_stance` `sim/stance_table.hpp:61` | Concept static_asserts `tests/test_character.cpp:39-96`, `tests/test_character_lists.cpp:42-45`, `tests/test_personal_opinion.cpp:85-88`, `tests/test_stances.cpp:37-38`. **Ambiguity:** `signed char`/`unsigned char` are character types in the standard but accepted (they are `int8_t`/`uint8_t`; asserted accepted at `test_character.cpp:41-42`). `char16_t`/`char32_t` are rejected by the concept but not tested on setters. |
| R15 | Implemented | `src/character.cpp:25-46` (`set_hundredths`, `add_hundredths`); `detail::clamp_round` `sim/hundredths.hpp:20`; defaults `sim/character.hpp:50-52,291-293` | static_assert `sim/character.hpp:327`; "condition defaults are health 100, stress 0, capacity 100", "condition set_ rounds to the nearest hundredth", "condition add_ saturates…", "resolution rule: add_health(0.004)…", NaN cases (Release only). |
| R16 | Implemented | `age_years`/`annual_mortality`/`weekly_mortality` `src/mortality.cpp:25,29-56,58-67`; not stored | sizeof(Character) pin; "annual mortality stays in [0, 1]…", "weekly probability compounds back…", "mortality matches results recorded…", "worse health, more stress and male gender raise mortality". |
| **G3** | | | |
| R17 | Implemented | `sim/character.hpp:285-319`; `Gender` `:41-44`; comment "stability = -neuroticism" `:300`; no neuroticism field | Offset static_asserts `src/character.cpp:54-63`; "construction stores identity fields", "non-default init values are applied and clamped". |
| R18 | Implemented | Layout in section 5 | `static_assert(sizeof(Character) == 1164)` `sim/character.hpp:333`; offsets `src/character.cpp:54-63`. |
| R19 | Implemented | `NICKNAME_CAP` 4 `sim/character_lists.hpp:12`; `add_nickname` `src/character_lists.cpp:62` | "nicknames: add keeps insertion order…", "nicknames: full, remove keeps order, NotFound". |
| R20 | Implemented | `PractiseEntry` `sim/character_lists.hpp:34-38`; `PRACTISE_CAP` 64; `add_skill` `src/character_lists.cpp:100` | "practise: …" (5 cases), incl. "a full list returns Full without changing state…". |
| R21 | Deviates | `set_involvement_weight` `src/character_lists.cpp:132`; `involvement_share` `:162-170` **returns float** (`static_cast<float>(double / double)`); `main_community` `:172` | "involvement: …" (5 cases); "three equal weights give three identical shares" compares floats. Returning float is not in R21; introduced in 00dad8a, no recorded decision found. A negative weight clamps to 0 and silently removes the entry (tested at `tests/test_character_lists.cpp:250`). |
| R22 | Implemented | `add_sacred` `src/character_lists.cpp:187-201` | "sacred: Duplicate, Conflict, remove then re-add with the other sign", "sacred: invalid target or sign, sorted by target, full". |
| R23 | Deviates | Spans `sim/character.hpp:165-238`; no list in `CharacterInit`; entries padding-free (static_asserts `sim/character_lists.hpp:58-60`); `FixedVector::erase` value-initializes `sim/fixed_vector.hpp:61-68`. **But** FixedVector is public at `sim/character.hpp:261-262` (`trim_long_opinions(CharacterKey, FixedVector<CharacterId, PERSON_LIMIT_MAX>&)`), `sim/character_registry.hpp:127` (`parents`) and `sim/stance_table.hpp:25,28` (chain types) | "lists start empty", "FixedVector: erase shifts left and value-initializes freed slots". Introduced in 938c882, 48e07ef and 8f3fcd3 respectively; no decision found. **Ambiguity:** CLAUDE.md:217 scopes the rule to per-character list reads, which comply; R23 does not scope it. |
| R24 | Implemented | `TargetKind` `sim/ids.hpp:40-43`; `TargetId` `:49-98`; `make` rejects 0 and `>= 2^30` | `tests/test_ids.cpp:51,70-72`; "TargetId round trip for both kinds", "TargetId rejects invalid ids…", "TargetId orders by kind, then index". |
| **G4** | | | |
| R25 | Implemented | `CharacterKey` `sim/character.hpp:72-75`; `create` `src/character_registry.cpp:26-44` (`id = slots_.size() + 1`) | `!is_default_constructible_v<CharacterKey>` and `!is_constructible_v<Character, … without key>` `tests/test_registry.cpp:13-14`; "registry: ids start at 1…"; id reuse checked in "kill: Invalid and NotFound…" and "forgetting: the record, its edges… gone". (Forgeable by `std::bit_cast`; see section 9.) |
| R26 | Deviates | Contract comment `sim/character_registry.hpp:54-57` lists create, kill, relation and personal-opinion edits, **not maintain**. `maintain` (`src/character_registry.cpp:448`) erases `long_people_` entries, which invalidates spans. | None possible (caller contract). CLAUDE.md:107 has the same omission. Introduced in dd55f11. |
| R27 | Implemented | `RelationType`/`relation_info` `sim/relations.hpp:14-63`; `RelationEdge` `:98-101`; graph edits `src/relation_graph.cpp:92-193`; empty edges erased in `remove_bits` `:79-90` | static_asserts `tests/test_relations.cpp:58-70`; "one-way: only the source edge changes…", "paired: both sides get the right complement", "paired: unlink removes both sides…", property test. |
| R28 | Implemented | Conflict `src/relation_graph.cpp:151-153`; Full `:154-157`; no cycle or birth-date checks in code | "Conflict leaves both sides unchanged", "parent limit: a third parent is Full…", property test. |
| R29 | Implemented | `siblings` `src/relation_graph.cpp:236-248`; `shared_parents` `:250-261`; nothing stored | "siblings: derived from shared parents; full and half siblings". |
| **G5** | | | |
| R30 | Implemented | `set_stance_value`/`clear_stance` `src/stance_table.cpp:25-52`; no from==to check | "stances: an explicit 0 differs from absent; clear restores inheritance", "stances: a topic target walks no chain; self-stance". |
| R31 | Implemented | `set_parent` `src/stance_table.cpp:75-101`: cycle `:82-85`, depth `:86-88` via `subtree_height` `:63` | "hierarchy: set, re-set, clear, invalid and cycles", "hierarchy: the depth limit returns Full, including when re-parenting a subtree". |
| R32 | Implemented | `stances` `src/stance_table.cpp:158-218` (source loop outer, target chain inner, first hit wins); `target_chain` topic = itself `:141-156`; public API returns id chains only | "stances: inheritance on the source side, the target side and both", "stances: source-side priority", golden weak-opinion tests. |
| R33 | Implemented | `weak_opinion_breakdown` `src/opinion.cpp:67-107` (int64 sum of raw weights, one division), target `:114-139`; clamp `:20-22` | "weak opinion: hand-computed example…", "weak opinion: clamping at both ends; breakdown terms sum…", "weak opinion: golden values…". |
| R34 | Implemented | `compat` `src/opinion.cpp:41-58` | "compat: identical gives +k_compat, opposite extremes give -k_compat, symmetric"; the zero weight sum is asserted in debug (`:46`) and returns 0 only in release ("compat: a zero weight sum returns 0", Release only). |
| R35 | Implemented | `splitmix64`/`noise` `sim/noise.hpp:27-48`; `NoiseSubject` `:17-21` | Compile-time goldens `tests/test_noise.cpp:11-16`; "noise: golden values at run time", "noise: directed, seed-dependent, and person differs from community with the same number". |
| R36 | Implemented | `src/opinion.cpp` uses only `+ - * /`, `std::clamp`, comparisons | Hexfloat goldens pass in both builds: "weak opinion: golden values…", "personal opinions: golden values…". Caveat under R3. |
| R37 | Unguarded | No cache members anywhere; `opinion.cpp` never touches `RelationGraph` | sizeof(Character) would catch a cache stored in Character. No test shows that a relation leaves an opinion unchanged. |
| **G6** | | | |
| R38 | Implemented | `opinion_breakdown` `src/opinion.cpp:182-229` | "opinion: equals weak without records, clamp(weak + long + short)…", "opinion: personal terms pull back from a saturated weak opinion". |
| R39 | Implemented | `sum_effects` `src/personal_opinion.cpp` (int sum, no clamp); `short_opinion` `:201-207` | "modifiers: short is the raw sum of effects; only the opinion total clamps". |
| R40 | Implemented | `OpinionModifier` `sim/opinion_records.hpp:46-51`; `add_to` in `src/personal_opinion.cpp` (Duplicate, then Full, no eviction); `MODIFIER_CAP` 32 `:20` | "modifiers: add, remove, Duplicate…", "modifiers: sorted by (domain, target, modifier)…", "modifiers: Full at MODIFIER_CAP…", personal property test. |
| R41 | Implemented | `add_long` `src/personal_opinion.cpp:52-91`; `person_limit` `:177-183`; `trim_long_opinions` `:274-289`; `TARGET_LIMIT` 16 | "long opinions: …" (6 cases), "maintain: trims after extraversion drops…", personal and lifecycle property tests. |
| R42 | Unguarded | No `Date` parameter in any personal-opinion API (`sim/personal_opinion.hpp`, registry `:145-171`) | None. |
| R43 | Implemented | Keyed members `sim/character.hpp:245-262`; registry-only wrappers | `!is_default_constructible_v<CharacterKey>` `tests/test_registry.cpp:13`. |
| **G7** | | | |
| R44 | Implemented | `kill` `src/character_registry.cpp:102-224` (checks `:103-112`) | "kill: Invalid and NotFound; find and find_dead; DeadRecord fields", "fame: … Failed kills report no fame". |
| R45 | Implemented | `sim/dead_record.hpp:15-24`; record built at `src/character_registry.cpp:126-133` | static_asserts `sim/dead_record.hpp:26-28`, `tests/test_death.cpp:102-106`; "kill: Invalid and NotFound…" (reputation changed after creation, frozen). |
| R46 | Implemented | `survives_death` `sim/relations.hpp:69-86`; `remove_non_surviving` `src/relation_graph.cpp:263-290`; `clear_one_way` `:292`; kill `src/character_registry.cpp:176,195` | static_asserts `tests/test_death.cpp:107-110`; "kill: surviving links stay on both sides…", lifecycle property test. |
| R47 | Implemented | The Character is destroyed at `src/character_registry.cpp:204-209`; others' modifiers are not touched | "kill: own opinions go; modifiers about the deceased stay…". |
| R48 | Implemented | `src/character_registry.cpp:184-194` | "rebase: opinions move by at most 0.5 unless long saturates; 0 removes; no entry means the base" (4 subcases), lifecycle property test. |
| R49 | Implemented | `weak_opinion_breakdown(Character, DeadRecord)` `src/opinion.cpp:146-175` | "weak opinion of a dead character: hand-computed; noise is identical before and after death"; golden values after death in "personal opinions: golden values…". |
| R50 | Implemented | `check_relation_edit` `src/character_registry.cpp:249-263`; dead-holder checks `:337-340,357-360,404-407,441-444` | "posthumous links: surviving types only; one-way edits with the dead are Invalid", "kill: own opinions go…", lifecycle property test. |
| R51 | Implemented | `find`/`find_dead` `src/character_registry.cpp:226-245`; living vector sorted by id | "kill: characters() excludes the dead in id order…", "kill: siblings through a dead parent…". |
| **G8** | | | |
| R52 | Implemented | `reference_flipped` `src/character_registry.cpp:55-68`; call sites in every edit, kill `:137-166,181-199`, maintain `:462-470` | "holders: …" (3 cases); the lifecycle property test compares every id with a full recount after each of 5,000 steps (`tests/test_death.cpp:1123`). |
| R53 | Unguarded | `forget` `src/character_registry.cpp:81-92`; `forget_node` `src/relation_graph.cpp:305-317`; order via `forget_in_order` `:70-79`, sorted scratch `:161,215,471` | Behaviour: "forgetting: …" (4 cases, 5 subcases) and the lifecycle property test. Not checked: id order (only affects `dead_` layout, not observable through the API), no allocation, no cascade. This property run forgot 0 characters by eviction and 0 by trim (MESSAGE output); those paths are covered by unit subcases only. |
| R54 | Implemented | `src/character_registry.cpp:123-133,221-223`; `LifecycleConfig` `sim/lifecycle_config.hpp:9-12` | "fame: holders before the kill plus \|reputation\|…" (incl. 65535 saturation and a legendary character forgotten at once), lifecycle property test. |
| **G9** | | | |
| R55 | Implemented | CLAUDE.md:192-198. Code: `slots_`, `holders_` (4 B each) and a `std::vector<RelationEdge>` node (24 B, probe) per id ever created | Bench: "per-id arrays: 32 B per id". |
| R56 | Implemented | CLAUDE.md:199-202 and :135-137. Code: `src/character_registry.cpp:204-208` | Bench: kill relocation and churn figures (section 8). |
| R57 | Implemented | CLAUDE.md:245-246. Code: `src/stance_table.cpp:36-37` (single sorted insert) | Bench "stance inserts" (50,000 inserts, not large enough to show the quadratic cost). |
| R58 | Implemented | CLAUDE.md:95-99. Code: `std::log`/`exp`/`expm1`/`log1p` in `src/mortality.cpp`; float getters `src/character.cpp:84-86` | n/a |
| **G10** | | | |
| R59 | Unguarded | `Outcomes` `bench/bench_main.cpp:75-133`; `SectionAborted` `:43`; per-section catch `:823-830`; `return 1` `:838-841` | This run: 0 aborted, exit 0. Nothing runs the abort path; sim_bench is not registered with ctest. |

## 3. Original specification mapping

| Original item | Now | Changed by |
|---|---|---|
| Name: [Full Name] | `NameId name_` handle (strings live outside the core) | 02bcdff |
| Age | Derived `age_years(Character, Date)`, not stored | 02bcdff |
| Health, Stress, Capacity 0..1 | 0..100 in hundredths (uint16), float API | 0..1 at 02bcdff; rescaled in 92fa965 |
| Chance to die 0..1 | Derived `annual_mortality` (Gompertz) and `weekly_mortality`, not stored | 02bcdff |
| Strength, Intelligence, Conscientiousness, Emotional stability, Openness, Extraversion, Agreeableness −1..+1 | int8 −100..+100 (`stability` = emotional stability) | Integers since 4d6d58a |
| Neuroticism −1..+1 | Not stored; the comment defines stability = −neuroticism | 02bcdff |
| Practise [abilities], [education], [languages] −1..+1 | Skills as capabilities without values: `SkillKind` Ability/Education/Language, up to 64 | 00dad8a |
| Attractiveness, Height, Shape −1..+1 | int8 −100..+100 | 4d6d58a |
| Charisma −1..+1 | int8 −100..+100 | 4d6d58a |
| Relations: whoever: status | `RelationGraph` edges with bitmasks: 3 one-way and 7 paired types | 48e07ef; death fates in dd55f11 |
| Involvement: community shares summing to 1 (tribe, family, city) | Raw weights 1..255 (≤ 8 communities); share = weight/total computed; community hierarchy in `StanceTable` | 00dad8a; hierarchy in 8f3fcd3 |
| Opinion whoever: short −1..+1, long −1..+1 | Weak opinion computed (stances, reputation, compat, noise) + long int −200..+200 (stored, limited) + short = sum of modifier effects (each −100..+100, unclamped); total clamped to ±100 | 8f3fcd3 (weak); a43d276 (decaying deviations) replaced by 370df2f (modifiers + long) |
| Opinion whatever: short, long | Same for communities and topics via `TargetId` (long limit 16, target-domain modifiers) | 8f3fcd3, 370df2f |
| Sacred: plus [..], minus [..] | One list of up to 8 `{TargetId, SacredSign}` | 00dad8a |
| Nicknames: [short and given names] | Up to 4 `NameId`s, insertion order | 00dad8a |

Present now but not in the original specification: `CharacterId` and the registry (ids from 1, never reused);
`Gender`; birth `Date` (int32 weeks) and `WEEKS_PER_YEAR`; reputation; `EditResult` and `LongOpinionOutcome`;
`TargetId`; `StanceTable` (stances, hierarchy, depth 6); deterministic `noise` and `WorldSeed`; personality `compat`
and `openness_factor`; `OpinionConfig`, `MortalityConfig` and `LifecycleConfig`; `maintain()`; death (`kill`,
`DeadRecord`, relation fates, rebase, posthumous links); holders, forgetting, fame and legendary;
`WorldContext`; memory accounting (`allocated_bytes`, `relation_bytes`, …); `sim_bench`.

## 4. Public API inventory

Families with identical signatures are collapsed into one line. `[[nodiscard]]` and `noexcept` are omitted.

**sim/integral.hpp**: `concept StrictIntegral`; `detail::clamp_integer(StrictIntegral, int lo, int hi) -> int`.
**sim/bipolar.hpp**: `BIPOLAR_MIN = -100`, `BIPOLAR_MAX = 100`; `detail::clamp_bipolar(StrictIntegral) -> int8_t`.
**sim/hundredths.hpp**: `detail::HUNDREDTHS_PER_UNIT`; `detail::clamp_round(double, int64 lo, int64 hi) -> int64`.
**sim/date.hpp**: `WEEKS_PER_YEAR`; `struct Date {int32 weeks}` (explicit ctor, `==`, `<=>`); `weeks_between(Date, Date) -> int32`; `weeks_to_years(int32) -> double`.
**sim/ids.hpp**:
- `Id<Tag, Rep>` (`value`, `valid()`, `==`, `<=>`); aliases `CharacterId`, `NameId`, `CommunityId`, `TopicId`, `SkillId` (u16), `ModifierId` (u16)
- `enum TargetKind {Community, Topic}`
- `class TargetId`: `INDEX_BITS`, `INDEX_LIMIT`, `from(CommunityId)`, `from(TopicId)`, `kind()`, `index()`, `valid()`, `raw()`, `as_community()`, `as_topic()`, `==`, `<=>`
- `std::hash` specializations for `Id` and `TargetId`

**sim/edit_result.hpp**: `enum EditResult {Ok, Full, Duplicate, Conflict, NotFound, Invalid}`.
**sim/vector_growth.hpp**: `detail::reserve_one_more(std::vector<T>&, size_t min_capacity)`.
**sim/fixed_vector.hpp**: `FixedVector<T, N>`: `size`, `capacity`, `empty`, `full`, `operator[]`, `data`, `begin`, `end`, `push_back`, `insert`, `erase`.
**sim/character_lists.hpp**: `NICKNAME_CAP`, `PRACTISE_CAP`, `INVOLVEMENT_CAP`, `SACRED_CAP`, `INVOLVEMENT_WEIGHT_MAX`; `enum SkillKind`; `enum SacredSign`; `struct PractiseEntry`, `InvolvementEntry`, `SacredEntry`.
**sim/opinion_records.hpp**: `PERSON_LIMIT_MIN/MAX`, `TARGET_LIMIT`, `MODIFIER_CAP`, `MODIFIER_EFFECT_MAX`, `LONG_VALUE_MAX`, `LONG_DELTA_MAX`; `struct LongOpinion`; `enum ModifierDomain`; `struct OpinionModifier`; `enum LongOpinionOutcome`; `struct LongOpinionResult<Target>`.
**sim/character.hpp**:
- `CONDITION_MIN`, `CONDITION_MAX`; `class BipolarInit` (ctor from StrictIntegral, `value()`); `enum Gender {Female, Male}`; `struct CharacterInit` (15 fields); `class CharacterKey` (registry-only ctor)
- `class Character`: ctor `(CharacterKey, CharacterId, NameId, Gender, Date, const CharacterInit&)`; copy and move ctors; assignments deleted
- Identity: `id()`, `name()`, `gender()`, `birth()`
- Condition: `health()`, `stress()`, `capacity()` → float; `set_health/stress/capacity(float)`; `add_health/stress/capacity(float)`
- Traits, 12 fields (strength, intelligence, stability, openness, extraversion, conscientiousness, agreeableness, attractiveness, height, shape, charisma, reputation): getter → int; `set_<field>(StrictIntegral)`; `add_<field>(StrictIntegral)`
- `nicknames()`, `add_nickname(NameId)`, `remove_nickname(NameId)`
- `practise()`, `has_skill(SkillKind, SkillId)`, `add_skill`, `remove_skill`
- `involvement()`, `set_involvement(CommunityId, StrictIntegral)`, `involvement_total() -> int`, `involvement_share(CommunityId) -> float`, `main_community() -> optional<CommunityId>`
- `sacred()`, `add_sacred(TargetId, SacredSign)`, `remove_sacred(TargetId)`, `sacred_sign(TargetId) -> optional<SacredSign>`
- `long_people()`, `long_targets()`, `modifiers()` (spans)
- Keyed: `add_modifier(CharacterKey, CharacterId, ModifierId, int, bool&)`, `add_modifier(CharacterKey, TargetId, ModifierId, int)`, `remove_modifier` (same two forms), `add_long_opinion(CharacterKey, CharacterId|TargetId, int)`, `trim_long_opinions(CharacterKey, FixedVector<CharacterId, 40>&) -> size_t`

**sim/personal_opinion.hpp**: `person_limit(const Character&) -> size_t`; `long_opinion(Character, CharacterId|TargetId) -> int`; `has_modifier(Character, CharacterId) -> bool`; `short_opinion(Character, CharacterId|TargetId) -> int`.
**sim/dead_record.hpp**: `struct DeadRecord`.
**sim/relations.hpp**: `enum RelationType` (10 values); `enum RelationKind`; `struct RelationInfo`; `relation_info(RelationType) -> optional<RelationInfo>`; `survives_death(RelationType) -> bool`; `relation_bit(RelationType) -> u32`; `MAX_PARENTS`; `struct RelationEdge`.
**sim/relation_graph.hpp**: `class RelationGraph`: `MIN_NODE_CAPACITY`, `MIN_EDGE_CAPACITY`, `reserve_node`, `add_node`, `node_count`, `struct EdgeFlips`, `set_relation`, `clear_relation`, `link`, `unlink` (with `EdgeFlips&`), `has_relation`, `relations`, `parents`, `children`, `siblings`, `shared_parents`, `remove_non_surviving`, `clear_one_way`, `has_edge`, `forget_node`, `has_one_way_edges`, `allocated_bytes`. All public, although the class comment says access goes through the registry.
**sim/noise.hpp**: `struct WorldSeed`; `enum NoiseSubject {Person, Community, Topic}`; `detail::splitmix64(u64)`; `noise(WorldSeed, CharacterId, NoiseSubject, u32) -> double`.
**sim/stance_table.hpp**:
- `MAX_COMMUNITY_DEPTH`, `STANCE_BATCH_MAX_SOURCES`; `CommunityChain`, `TargetChain`; `struct StanceEntry`, `ParentEntry`
- `class StanceTable`: `set_stance(CommunityId, TargetId, StrictIntegral)`, `clear_stance`, `set_parent`, `clear_parent`, `parent`, `explicit_stance`, `stance(CommunityId, TargetId)`, `stance(const CommunityChain&, const TargetChain&)`, `stances(span, span, span<int>)`, `chain`, `target_chain`, `stance_count`, `parent_count`

**sim/opinion_config.hpp**: `struct OpinionConfig` (10 doubles).
**sim/lifecycle_config.hpp**: `struct LifecycleConfig` (2 × u16).
**sim/mortality.hpp**: `struct MortalityConfig` (6 doubles); `age_years`; `annual_mortality`; `weekly_mortality`.
**sim/world_context.hpp**: `struct WorldContext {const StanceTable&, const OpinionConfig&, const LifecycleConfig&, WorldSeed}`.
**sim/opinion.hpp**:
- `struct WeakOpinionBreakdown`; `compat`; `openness_factor`
- `weak_opinion_breakdown` / `weak_opinion` for `(Character, Character)`, `(Character, TargetId)`, `(Character, DeadRecord)`
- `struct OpinionBreakdown`; `opinion_breakdown` / `opinion` for the same three target forms

**sim/character_registry.hpp**:
- `struct KillResult {EditResult result; u16 fame; bool legendary}`
- `class CharacterRegistry`: `create`, `kill`, `find` (const and non-const), `find_dead`, `exists`, `holders`, `characters` (const and non-const span), `size`, `dead_count`
- Relations: `set_relation`, `clear_relation`, `link`, `unlink`, `has_relation`, `relations`, `parents`, `children`, `siblings`, `shared_parents`
- Opinions and memory: `add_modifier` (person and target), `remove_modifier` (×2), `add_long_opinion` (×2), `maintain`, `allocated_bytes`, `relation_bytes`, `dead_record_bytes`, `slot_bytes`

## 5. Layouts

GCC 13, x86-64, measured with a scratch probe (`offsetof`, `-Dprivate=public`, outside the repo).

**Character**: sizeof 1164, align 4, trivially copyable, `has_unique_object_representations` **false**.

| Offset | Size | Field | Notes |
|---|---|---|---|
| 0 | 4 | id_ | |
| 4 | 4 | name_ | |
| 8 | 4 | birth_ | |
| 12, 14, 16 | 2 each | health_, stress_, capacity_ | |
| 18 | 1 | gender_ | |
| 19..29 | 1 each | strength_, intelligence_, stability_, openness_, extraversion_, conscientiousness_, agreeableness_, attractiveness_, height_, shape_, charisma_ | |
| 30 | 258 | practise_ (items 256 + size_ at +256) | 1 B tail padding |
| 288 | 20 | nicknames_ (16 + size_ at +16) | 3 B tail padding |
| 308 | 68 | involvement_ (64 + size_ at +64) | 3 B tail padding |
| 376 | 68 | sacred_ (64 + size_ at +64) | 3 B tail padding |
| 444 | 1 | reputation_ | 3 B padding (445..447) |
| 448 | 324 | long_people_ (320 + size_ at +320) | 3 B tail padding |
| 772 | 132 | long_targets_ (128 + size_ at +128) | 3 B tail padding |
| 904 | 260 | modifiers_ (256 + size_ at +256) | 3 B tail padding |

Implicit padding in Character: 22 bytes in total, none in offsets 0..29.

| Type | sizeof/align | Offsets (size) | Padding |
|---|---|---|---|
| DeadRecord | 24/4 | id 0(4), name 4(4), birth 8(4), death 12(4), main_community 16(4), reputation 20(1), gender 21(1), fame 22(2) | none (unique repr) |
| PractiseEntry | 4/2 | skill 0(2), kind 2(1), reserved 3(1) | none |
| InvolvementEntry | 8/4 | community 0(4), weight 4(1), padding[3] 5 | explicit only |
| SacredEntry | 8/4 | target 0(4), sign 4(1), padding[3] 5 | explicit only |
| LongOpinion | 8/4 | target 0(4), value 4(2), reserved 6(2) | none |
| OpinionModifier | 8/4 | target 0(4), modifier 4(2), effect 6(1), domain 7(1) | none |
| RelationEdge | 8/4 | other 0(4), mask 4(4) | none |
| StanceEntry | 12/4 | from 0(4), to 4(4), value 8(1), padding[3] 9 | explicit only |
| ParentEntry | 8/4 | child 0(4), parent 4(4) | none |
| Date, CharacterId, NameId, CommunityId, TopicId, TargetId | 4/4 | value 0(4) | none |
| SkillId, ModifierId | 2/2 | value 0(2) | none |
| WorldSeed | 8/8 | value 0(8) | none |
| Non-persistent: CharacterInit 24/4, BipolarInit 1/1, KillResult 6/2, WorldContext 32/8, `std::vector<RelationEdge>` node 24/8 | | | |

## 6. Test inventory

| File | Cases (Debug / Release) | Static checks |
|---|---|---|
| tests/test_character.cpp | 19 / 21 | trivially copyable, sizeof, StrictIntegral acceptance/rejection, BipolarInit clamps |
| tests/test_character_lists.cpp | 17 / 17 | entry sizes, `set_involvement` types |
| tests/test_date.cpp | 3 / 3 | sizeof(Date), no implicit int conversion |
| tests/test_death.cpp | 10 / 10 | DeadRecord layout, `survives_death` |
| tests/test_fixed_vector.cpp | 5 / 5 | trivially copyable, N bounds |
| tests/test_ids.cpp | 5 / 5 | id sizes and conversions, TargetId basics |
| tests/test_memory.cpp | 8 / 8 | DeadRecord fame offset |
| tests/test_mortality.cpp | 6 / 6 | none |
| tests/test_noise.cpp | 4 / 4 | 5 compile-time golden values |
| tests/test_opinion.cpp | 6 / 8 | none |
| tests/test_personal_opinion.cpp | 17 / 17 | record sizes, argument types |
| tests/test_registry.cpp | 5 / 5 | passkey, copy/assign traits |
| tests/test_relations.cpp | 11 / 11 | `relation_info` table |
| tests/test_stances.cpp | 7 / 7 | `set_stance` types, sizeof(StanceEntry) |
| **Total** | **123 / 127** | |

Coverage per group (test case names abbreviated to their prefix where a file has a family):

- **G1:** "Conflict leaves both sides unchanged", "parent limit: a third parent is Full…", "modifiers: Full at MODIFIER_CAP…", "hierarchy: the depth limit…" (unchanged state); Release-only NaN, compat and a == b cases; the three property tests.
- **G2:** test_character.cpp "condition …" (8 cases) and "bipolar …" (6); "init struct clamps non-constant out-of-range integers"; test_date.cpp (3); test_mortality.cpp (6).
- **G3:** "construction stores identity fields", "defaults match the init struct", "non-default init values…", "reputation: init values clamp and arrive"; all of test_character_lists.cpp; test_fixed_vector.cpp; "TargetId …" (3).
- **G4:** test_registry.cpp (5); test_relations.cpp (11), incl. "property: random edits match a reference model…" (20,000 steps).
- **G5:** test_stances.cpp (7); test_noise.cpp (4); test_opinion.cpp (8).
- **G6:** test_personal_opinion.cpp (17), incl. "property: random modifiers, long opinions, extraversion changes and maintain match a reference model" (8,000 steps).
- **G7:** test_death.cpp (10), incl. "property: lifecycle … matches a reference model" (5,000 steps, 160 ids, 119 kills).
- **G8:** test_memory.cpp (8); lifecycle property test; the Step 7 part of "personal opinions: golden values…".
- **G9:** none (documentation). **G10:** none.

Requirements with no test and no static_assert at all: R1, R2, R3 (beyond the golden values), R6, R7, R8, R9 (no-throw part), R11, R26, R37, R42, R55–R58, R59.
Partly untested: R5 (no default ctor, no allocation), R10 (bad_alloc), R14 (`char16_t`/`char32_t` on setters), R53 (order, no allocation, no cascade).

## 7. CLAUDE.md consistency

Rules or claims in CLAUDE.md that the code does not follow:

1. CLAUDE.md:107-109 (Lifetimes) lists "create, kill, any relation edit or personal opinion edit". `maintain()` also invalidates spans into `long_people()`. See R26.
2. CLAUDE.md:56 says every persistent field has a units comment; `WorldSeed::value` (`sim/noise.hpp:11`) has none.
3. CLAUDE.md:217 says the public API never exposes `FixedVector`. It does in `Character::trim_long_opinions` (public, keyed), in `CharacterRegistry::parents` and in the `StanceTable` chain types. Only the first is a per-character-list API.
4. CLAUDE.md:274-275 says every API converts stored raw targets to `CharacterId`/`TargetId` at the boundary. The public spans `long_people()`, `long_targets()` and `modifiers()` expose raw `std::uint32_t target` values.
5. CLAUDE.md:228 describes shares as computed in double. `involvement_share` returns `float`.
6. CLAUDE.md:131-137 claims a kill costs about 1.2 ms at 15,000 living. This run measured 0.83 ms for a kill spread over the id range (section 8); timings vary by machine.
7. CLAUDE.md:110 says relations are "edited only through" the registry. `RelationGraph`'s mutators (`link`, `forget_node`, `remove_non_surviving`, …) are public on a public class; only the registry owns an instance, but nothing prevents a standalone graph.
8. CLAUDE.md:166-169 says holders "cannot go out of sync" because mutations are registry-only or key-gated. A `CharacterKey` can be forged with `std::bit_cast` (verified; section 9).

Behaviour in the code that CLAUDE.md does not describe:

- `kill` returns Invalid when `death < birth`, checked after NotFound (`src/character_registry.cpp:110`).
- `create` returns an invalid id once ids are exhausted (`src/character_registry.cpp:28-30`).
- `set_involvement` with a negative weight clamps to 0 and removes the entry.
- `add_nickname`, `add_skill` and `add_sacred` return Invalid for id 0.
- `set_parent` to the current parent returns Ok.
- `StanceTable::stances` returns silently in release if the batch preconditions fail (`src/stance_table.cpp:162-164`).
- `weak(A -> T)` scales noise by `openness_factor`, while person targets do not.
- `compat` asserts a positive weight sum in debug.
- Character has 22 bytes of implicit padding (section 5), which matters for any byte-wise serialization.
- The bench "450,000 dead" kill section actually has no dead records left before timing: every setup pair is forgotten, and the memory line prints "dead records 0.0 MB".

## 8. Benchmarks

Command: `cmake --build <tmp>/b-rel --target sim_bench && <tmp>/b-rel/bench/sim_bench`, Release build of 018724d.
**Exit code 0**, no section aborted.

| Section | Figures | Outcome counts |
|---|---|---|
| weak(A→B) typical (3 communities, depth 3, 19,823 stances) | 1,000,000 evals in 2.932 s = 2,931.7 ns (checksum 6857273.270347) | set_parent Ok 600; set_stance Ok 20000; create Ok 1000; set_involvement Ok 3010 |
| weak(A→B) worst (8 communities, depth 6, 19,955 stances) | 26.220 s = 26,219.9 ns (checksum 6247049.484553) | set_parent Ok 1500; set_stance Ok 20000; create Ok 1000; set_involvement Ok 8095 |
| weak(A→B) structured (1,810 communities, 929 stances) | random pairs 2,040.2 ns; acquaintances 2,035.5 ns; community term nonzero for 100.0% of both | set_parent Ok 1800; set_stance Ok 989; create Ok 1000; set_involvement Ok 3009 |
| weak(A→topic) structured | 959.6 ns (checksum -638143.541448) | (same world) |
| stance inserts | 50,000 random-order inserts, 2,207.5 ns each | set_stance Ok 50000 |
| personal world (1,500 characters) | setup only | world: set_parent Ok 600, set_stance Ok 20000, create Ok 1500, set_involvement Ok 4517; background: add_long_opinion Created 30000, add_modifier Ok 24000 |
| modifier add+remove | 82.1 ns per pair | add_modifier Ok 1000000; remove_modifier Ok 1000000 |
| long-term changes | 47.2 ns each | Updated 952840, Created 2370, Removed 2445, Unchanged 42345 |
| link+unlink | 127.5 ns per pair | link Ok 1000000; unlink Ok 1000000 |
| opinion reads with records | 2,092.4 ns (12,000 pairs) | world: set_parent Ok 600, set_stance Ok 20000, create Ok 1500, set_involvement Ok 4512 |
| maintain trimming (1,500 chars) | 7 passes, 84,000 entries, 24,594 trimmed each; median 1.242 ms (min 1.238, max 1.255), about 14.8 ns per entry | template add_long_opinion: people Created 60000, targets Created 24000 |
| kill, no dead yet | 500 kills, 196.7 µs each (22.0 edges per victim; 1,000 living, 500 dead after) | kill world: add_long_opinion Created 84000, add_modifier Ok 24000, link Ok 15000, set_relation Ok 3000; kill Ok 500 (legendary 206) |
| kill after 450,000 create+kill | setup chunks 20.5, 20.1, 20.3, 20.0, 20.4, 20.3, 20.4, 20.5, 20.4 µs; setup 9.2 s = 20.4 µs per kill; timed 500 kills at 195.8 µs; memory: dead records 0.0 MB, slots+holders 4.0 MB, graph 12.4 MB (451,500 ids), living 4.5 MB | setup create Ok 450000, link Ok 225000, kill Ok 450000; kill Ok 500 (legendary 206) |
| kill relocation (15,000 living) | 500 kills, 825.8 µs each (7,514 characters and 8.34 MB moved per kill on average) | create Ok 15000; kill Ok 500 |
| generational churn (1,500 living) | every 50,000 deaths: dead 830–876, max 898; dead records 0.023 MB; slots+holders 0.5 → 4.0 MB; graph 1.6 → 12.1 MB; 195.0–197.1 µs per step; 449,157 forgotten; 451,500 ids; per-id arrays 32 B, projected 137.3 MiB at 4.5 M ids (103.0 MiB node headers) | create Ok 451500; link Ok 900000; kill Ok 450000 |

## 9. Risks

Known limits (confirmed in code and figures):

- Per-id growth: 32 B per id ever created (R55). The bench projects 137 MiB at 15,000 living over the whole timeline.
- Kill relocation is O(living) (R56): 0.83 ms per kill at 15,000 living with victims spread over the id range. Oldest-first deaths shift almost the whole vector. There is no batch kill.
- Bulk stance loading inserts one entry at a time (R57); `subtree_height` scans every parent entry per visited node (`src/stance_table.cpp:63-73`).
- Mortality and condition getters use non-bit-identical floating point (R58).
- `CharacterKey` is trivially copyable. `std::bit_cast<CharacterKey>(char{0})` compiles without warnings, and a forged call to `Character::add_long_opinion` stored an entry while `holders()` stayed 0 (scratch probe). The passkey guards against accidents, not deliberate bypass.

The five places I am least sure are correct:

1. **Forgetting through eviction and trim inside kill-heavy histories.** The lifecycle property test creates every character with extraversion 100, and this run forgot 0 characters "by eviction" and 0 "by trim". Those paths (`src/character_registry.cpp:427-430,462-474`) are exercised only by the fixed unit subcases in "forgetting: triggered by removing…". Interactions with kill-time decrements and dead-to-dead edges are therefore reasoned about, not tested.
2. **Atomicity under bad_alloc.** No test injects allocation failure. The claim relies on reserve-before-write, on `std::sort` not allocating (true for libstdc++, not required by the standard) and on `vector::insert`/`erase` not reallocating when capacity suffices. Kill and maintain assert nothing after their reserves.
3. **Cross-platform determinism of opinion math.** The golden values pass on GCC x86-64 without FMA. That build cannot tell whether `-ffp-contract=off` is still effective, and MSVC (`/fp:precise`) has never been built. The kill rebase also depends on `std::round(before - after)` of those doubles, so a 1-ULP difference at a .5 boundary would store a different long value.
4. **Character's 22 padding bytes.** Character is trivially copyable and `FixedVector` promises no stale data in slots, but the tail padding after each `size_` and after `reputation_` is indeterminate. A future memcpy-based save or a hash of Character bytes would not be deterministic. No static check covers Character's object representation.
5. **Kill ordering assumptions.** `kill` decrements the deceased's targets before rebasing and recounting holders, relocates living slots while `victim` still refers into `living_`, and forgets candidates in one pass (`src/character_registry.cpp:148-220`). `victim` is not used after relocation, and each candidate is unique, but the correctness argument is spread over several invariants: dead nodes hold no one-way edges, paired edges are symmetric, and a referenced id is never forgotten. Those invariants are checked by the property test at 160 ids only. The debug-only check of dead neighbours (`:171-175`) is not active in Release.
