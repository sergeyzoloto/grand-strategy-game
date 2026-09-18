# sim_core: file-level reference

## 1. Overview

`sim_core` is the portable C++20 simulation core (namespace `sim`): characters with conditions,
traits and inline lists, relations between them, community stances, computed opinions, mortality,
death and the forgetting of the dead. It has no engine types and no global state: stances, configs
and the world seed are passed in as parameters. `CMakeLists.txt` builds one static library,
`sim_core`, from nine sources. One of them, `src/build_guards.cpp`, defines nothing and only
stops the build in the wrong language mode. Headers are in `include/sim/` (included as `"sim/foo.hpp"`), sources in `src/`.

Layering (each header includes only modules to its left): foundations (`integral`, `bipolar`, `hundredths`, `ids`, `fixed_vector`, `vector_growth`, `edit_result`, `date`) → records and configs (`noise`, `relations`, `character_lists`, `opinion_records`, `opinion_config`, `lifecycle_config`) → `character`, `stance_table`, `relation_graph` → `dead_record`, `mortality`, `personal_opinion`, `opinion`, `world_context` → `character_registry`.
The one exception is `src/character_lists.cpp`, which defines `Character` members and so includes `character.hpp`.

## 2. Module index

| Module | Files | One-line purpose |
|---|---|---|
| `CMakeLists.txt` | `CMakeLists.txt` | Defines the `sim_core` static library target. |
| `build_guards` | `src/build_guards.cpp` | Stops the build below C++20, in a GNU dialect or with fast-math. |
| `integral` | `include/sim/integral.hpp` | `StrictIntegral` concept and mixed-sign-safe integer clamping. |
| `bipolar` | `include/sim/bipolar.hpp` | The -100..+100 scale bounds and `clamp_bipolar`. |
| `hundredths` | `include/sim/hundredths.hpp` | `clamp_round`: clamp, then round a double to an integer. |
| `ids` | `include/sim/ids.hpp` | Strong id types and the packed `TargetId`. |
| `fixed_vector` | `include/sim/fixed_vector.hpp` | Inline fixed-capacity vector with byte-deterministic contents. |
| `vector_growth` | `include/sim/vector_growth.hpp` | `reserve_one_more`: geometric pre-reservation for atomic edits. |
| `edit_result` | `include/sim/edit_result.hpp` | `EditResult`, the result enum of every edit. |
| `date` | `include/sim/date.hpp` | `Date` in whole weeks and week/year conversion. |
| `noise` | `include/sim/noise.hpp` | Deterministic SplitMix64 noise in [-1, 1] and `WorldSeed`. |
| `relations` | `include/sim/relations.hpp` | Relation types, their kind and complement, and `RelationEdge`. |
| `character_lists` | `include/sim/character_lists.hpp`, `src/character_lists.cpp` | Entry types and caps of the identity lists; their `Character` members. |
| `opinion_records` | `include/sim/opinion_records.hpp` | Stored long-term opinion and modifier records, bounds and outcomes. |
| `opinion_config` | `include/sim/opinion_config.hpp` | PLACEHOLDER coefficients of weak opinions. |
| `lifecycle_config` | `include/sim/lifecycle_config.hpp` | PLACEHOLDER fame coefficients used at death. |
| `character` | `include/sim/character.hpp`, `src/character.cpp` | `Character`, the persistent state of one living character. |
| `stance_table` | `include/sim/stance_table.hpp`, `src/stance_table.cpp` | Community stances towards targets, with inheritance through a hierarchy. |
| `relation_graph` | `include/sim/relation_graph.hpp`, `src/relation_graph.cpp` | Per-character sorted edge lists for one-way and paired relations. |
| `dead_record` | `include/sim/dead_record.hpp` | `DeadRecord`, the 24-byte remains of a dead character. |
| `mortality` | `include/sim/mortality.hpp`, `src/mortality.cpp` | Age and Gompertz mortality probabilities as free functions. |
| `personal_opinion` | `include/sim/personal_opinion.hpp`, `src/personal_opinion.cpp` | Reads of long-term values and modifiers; the keyed `Character` mutators. |
| `opinion` | `include/sim/opinion.hpp`, `src/opinion.cpp` | Weak and full opinions, computed on demand, never stored. |
| `world_context` | `include/sim/world_context.hpp` | `WorldContext`: stances, configs and seed passed as one parameter. |
| `character_registry` | `include/sim/character_registry.hpp`, `src/character_registry.cpp` | Creates and kills characters, owns relations, tracks holders, forgets the dead. |

## 3. Module entries

#### `CMakeLists.txt`
**Files:** `CMakeLists.txt`
**Purpose:** `add_library(sim_core STATIC ...)` over the nine sources in `src/`, with `include/` as a PUBLIC include directory and `cxx_std_20` as a PUBLIC compile feature. Warnings and flags come from `sim_configure_target(sim_core)`, a function defined in the root `CMakeLists.txt`, not here.
**Depends on:** no sim modules.

#### `build_guards`
**Files:** `src/build_guards.cpp`
**Purpose:** Fails the compilation of `sim_core` with `#error` unless it is built as ISO C++20 without fast-math. It is a source file, not a header, so targets that include the headers with their own flags are unaffected. It defines nothing.
**Key contents:**
- Below C++20: checks `__cplusplus`, or `_MSVC_LANG` under MSVC (which reports `__cplusplus` as 199711L without `/Zc:__cplusplus`).
- GNU dialect: GCC or Clang without `__STRICT_ANSI__` (e.g. `-std=gnu++20`).
- `__FAST_MATH__` defined.

**Rules / invariants:**
- `-ffp-contract=off` cannot be seen by the preprocessor; the file refers to the `-mfma` check in CLAUDE.md instead.

**Depends on:** none (no includes).

#### `integral`
**Files:** `include/sim/integral.hpp`
**Purpose:** `StrictIntegral`: any `std::integral` type except `bool`, `char`, `wchar_t`, `char8_t`, `char16_t` and `char32_t`. Every whole-number input uses it, so a float argument matches no overload. `detail::clamp_integer(v, lo, hi)` clamps any such value to an `int` range with `std::cmp_less`/`std::cmp_greater`.
**Depends on:** standard library only.

#### `bipolar`
**Files:** `include/sim/bipolar.hpp`
**Purpose:** `BIPOLAR_MIN` = -100 and `BIPOLAR_MAX` = 100, plus `detail::clamp_bipolar(StrictIntegral)`, which clamps to that range and returns `std::int8_t`. Every stored bipolar field and stance is set through it.
**Depends on:** integral.

#### `hundredths`
**Files:** `include/sim/hundredths.hpp`
**Purpose:** `detail::HUNDREDTHS_PER_UNIT` = 100.0 and `detail::clamp_round(scaled, lo, hi)`. The function clamps a double to [lo, hi] before converting, because an out-of-range float-to-integer conversion is UB, then rounds half away from zero with `std::round`. It asserts a non-NaN input. A comment says to round a fractional delta, not the sum. Used for condition fields (`character.cpp`) and the opinion rebase at death (`character_registry.cpp`).
**Depends on:** standard library only.

#### `ids`
**Files:** `include/sim/ids.hpp`
**Purpose:** Tag-typed identifiers that do not convert to integers or to each other, and `TargetId`, which refers to a community or a topic in one `u32`.
**Key contents:**
- `Id<Tag, Rep = std::uint32_t>`: `value` (0 = invalid), explicit constructor, `valid()`, `==`/`<=>`. `Rep` must be an unsigned `StrictIntegral`.
- Aliases: `CharacterId`, `NameId`, `CommunityId`, `TopicId` (u32); `SkillId`, `ModifierId` (u16).
- `TargetKind`: `Community` = 0, `Topic` = 1. The comment reserves 2 for persons; 3 is free.
- `TargetId`: 2 kind bits above a 30-bit index (`INDEX_BITS` = 30, `INDEX_LIMIT` = 2^30). `from(CommunityId|TopicId)` returns empty for id 0 or an index >= 2^30. `from_raw` decodes a stored `raw()` value. `as_community()`/`as_topic()` convert back.
- `std::hash` specializations for `Id` and `TargetId`.

**Rules / invariants:**
- `TargetId` compares in raw order (by kind, then index), and the sorted lists keyed by it depend on that order.
- `TargetKind` is append-only. `from_raw` switches on it with no `default`, so `-Wswitch` flags a new kind there.

**Depends on:** integral.

#### `fixed_vector`
**Files:** `include/sim/fixed_vector.hpp`
**Purpose:** An inline vector with a fixed capacity. `Character` uses it for every list, so `Character` never allocates and stays trivially copyable. Small fixed-size results use it too (`parents()`, stance chains).
**Key contents:**
- `FixedVector<T, N>` (N in 1..255): `size`, `capacity`, `empty`, `full`, `operator[]`, `data`/`begin`/`end`, `push_back`, `insert(index, value)`, `erase(index)`.
- `detail::FixedVectorSize<T>`: a size counter as wide as `alignof(T)` (1, 2, 4 or 8), so the vector has no tail padding.

**Rules / invariants:**
- Every unused slot, including one freed by `erase`, holds a value-initialized `T`. If `T` has unique object representations, equal contents mean equal bytes.
- Capacity and index bounds are only asserted. Callers check `full()` and report `Full` themselves.

**Depends on:** standard library only.

#### `vector_growth`
**Files:** `include/sim/vector_growth.hpp`
**Purpose:** `detail::reserve_one_more(v, min_capacity)` reserves max(`min_capacity`, 2 × capacity) when `v` is full. Edits call it on every container they will grow before the first write, so `bad_alloc` leaves state unchanged and the following single insert cannot reallocate. The header comment warns never to `reserve(size() + 1)`. Used by `stance_table`, `relation_graph` and `character_registry`.
**Depends on:** standard library only.

#### `edit_result`
**Files:** `include/sim/edit_result.hpp`
**Purpose:** `EditResult` (u8) has the values `Ok` = 0, `Full`, `Duplicate`, `Conflict`, `NotFound` and `Invalid` = 5. List, relation and stance edits all return it, and every result other than `Ok` leaves state unchanged. The comment gives the check order: Invalid, then NotFound for unknown characters, then Duplicate/Conflict/NotFound against existing entries, then Full.
**Depends on:** standard library only.

#### `date`
**Files:** `include/sim/date.hpp`
**Purpose:** `Date` holds `weeks`, a signed `int32` count of weeks since world start. `weeks_between(from, to)` is signed and asserts that the result fits in `int32`. `weeks_to_years` divides by `WEEKS_PER_YEAR` = 52.1775, a calendar constant rather than a gameplay coefficient.
**Depends on:** standard library only.

#### `noise`
**Files:** `include/sim/noise.hpp`
**Purpose:** Deterministic, directed noise for weak opinions. The same inputs give the same value on every platform.
**Key contents:**
- `WorldSeed { std::uint64_t value; }`: the seed of all noise in a world, passed explicitly.
- `NoiseSubject`: `Person` = 0, `Community` = 1, `Topic` = 2. It is independent of `TargetKind`, so a person and a community with the same number get different noise.
- `detail::splitmix64`: one full SplitMix64 step.
- `noise(seed, source, subject, target)`: three chained `splitmix64` steps over the seed, the source id and (subject << 32 | target). The top 53 bits are mapped onto [-1, 1] using exact conversion and `*`, `/`, `-` only.

**Rules / invariants:**
- The hash and the `NoiseSubject` values are frozen: changing either shifts the noise of the whole world. The header says golden values in the tests pin them.

**Depends on:** ids.

#### `relations`
**Files:** `include/sim/relations.hpp`
**Purpose:** The vocabulary of character relations: types, their kind and complement, and the edge record that `RelationGraph` stores.
**Key contents:**
- `RelationType`: one-way `Friend`, `Rival`, `Attraction` (0–2); paired `Parent`, `Child`, `Liege`, `Vassal`, `Employer`, `Employee`, `Spouse` (3–9). The value is also the bit index in the edge mask, so at most 32 types fit.
- `relation_info(type)` returns `RelationInfo {kind, complement}`, or empty for a value outside the enum. `Spouse` is its own complement.
- `survives_death(type)` is true for `Parent`, `Child` and `Spouse` only.
- `relation_bit(type)` gives the mask bit.
- `MAX_PARENTS` = 2.
- `RelationEdge {other, mask}` (8 bytes): a set bit on edge a → b names b's role for a.

**Rules / invariants:**
- `RelationType` is persisted and append-only. Both switches have no `default`, so `-Wswitch` flags a new type in each of them.

**Depends on:** ids.

#### `character_lists`
**Files:** `include/sim/character_lists.hpp`, `src/character_lists.cpp`
**Purpose:** The header defines the entry types and storage caps of the four identity lists inside `Character`. The source defines the `Character` members that edit and query those lists.
**Key contents:**
- Caps: `NICKNAME_CAP` = 4, `PRACTISE_CAP` = 64, `INVOLVEMENT_CAP` = 8, `SACRED_CAP` = 8. `INVOLVEMENT_WEIGHT_MAX` = 255.
- Persisted, append-only enums: `SkillKind` (`Ability`, `Education`, `Language`) and `SacredSign` (`Plus`, `Minus`).
- Entry types: `PractiseEntry` (4 B), `InvolvementEntry` (8 B) and `SacredEntry` (8 B), padded with explicit zeroed bytes; static_asserts check that they have unique object representations.
- Members: `add_nickname`/`remove_nickname`; `has_skill`/`add_skill`/`remove_skill`; `add_sacred`/`remove_sacred`/`sacred_sign`.
- Involvement members: `set_involvement_weight`, `involvement_total`, `involvement_share`, and `main_community` (the largest weight; ties go to the smaller id).
- `Character::lists_valid()`: the debug invariant check over every list, including long opinions and modifiers.

**Rules / invariants:**
- Checks run in the order Invalid, then Duplicate/Conflict/NotFound, then Full. The caps are storage bounds and nothing is evicted. `add_sacred` with the opposite sign returns `Conflict`.
- An involvement weight of 0 removes the entry, and returns `Ok` even when the entry is absent.

**Depends on:** edit_result, ids; the source includes character.

#### `opinion_records`
**Files:** `include/sim/opinion_records.hpp`
**Purpose:** Record types, bounds and outcome enums for personal opinions. `character`, `personal_opinion` and `character_registry` use them.
**Key contents:**
- `PERSON_LIMIT_MIN` = 8, `PERSON_LIMIT_MAX` = 40, `TARGET_LIMIT` = 16 (all PLACEHOLDER; compile-time constants because they size storage).
- `MODIFIER_CAP` = 32, `MODIFIER_EFFECT_MAX` = `BIPOLAR_MAX`, `LONG_VALUE_MAX` = 200, `LONG_DELTA_MAX` = 400.
- `LongOpinion<Target>` {target, `int16` value, `uint16` reserved}, 8 B, with a value that is never 0. `PersonLongOpinion` is keyed by `CharacterId`, `TargetLongOpinion` by `TargetId`.
- `ModifierDomain` (`Person` = 0, `Target` = 1) keeps the two id spaces apart. `OpinionModifier` {raw `target`, `modifier`, `effect`, `domain`} is 8 B; `person()` and `target_id()` decode the raw target.
- `LongOpinionOutcome`: `Updated`, `Created`, `CreatedWithEviction`, `Removed`, `Unchanged`, `Dropped`, `Invalid`, `NotFound`.
- `LongOpinionResult<Target> {outcome, evicted}`: `evicted` is valid only for `CreatedWithEviction`.

**Depends on:** bipolar, ids.

#### `opinion_config`
**Files:** `include/sim/opinion_config.hpp`
**Purpose:** `OpinionConfig` holds the weak-opinion coefficients, all PLACEHOLDER doubles: `k_rep` (0.25); `k_compat` (20.0); five compat weights from `w_stability` to `w_agreeableness` (1.0 each, >= 0); `k_noise` (10.0); `openness_factor_min`/`openness_factor_max` (0.5/1.5). Personal opinions have no coefficients.
**Depends on:** none (no includes).

#### `lifecycle_config`
**Files:** `include/sim/lifecycle_config.hpp`
**Purpose:** `LifecycleConfig` holds whole-number PLACEHOLDER coefficients: `fame_per_reputation` = 1 and `legendary_fame` = 100, both `uint16_t`. `CharacterRegistry::kill` reads them through `WorldContext`.
**Depends on:** standard library only.

#### `character`
**Files:** `include/sim/character.hpp`, `src/character.cpp`
**Purpose:** `Character` is the persistent state of one living character: identity, conditions, bipolar traits and reputation, and inline lists. It is trivially copyable, holds no pointers or floating point, and never allocates.
**Key contents:**
- `Character`: construction requires a `CharacterKey`. Copy and move construction are public; assignment is deleted; there is no default constructor.
- `health`/`stress`/`capacity`: float `set_`/`add_`, stored as `uint16` hundredths (raw 0..10000) via `detail::clamp_round`. `add_` rounds the delta. NaN leaves the field unchanged (asserted).
- Bipolar fields: `strength`, `intelligence`, the Big Five (`stability` … `agreeableness`), `attractiveness`, `height`, `shape`, `charisma`, `reputation`. `set_`/`add_` take a `StrictIntegral` and clamp.
- `CharacterInit` (PLACEHOLDER defaults) and `BipolarInit` (clamps on conversion).
- `CharacterKey`: only `CharacterRegistry` can construct it. It also gates the personal-opinion mutators.
- `Gender` (`Female`, `Male`); `CONDITION_MIN`/`CONDITION_MAX`.

**Rules / invariants:**
- The layout is pinned by `sizeof(Character) == 1164`, `offsetof` static_asserts in the constructor and `has_unique_object_representations_v`.
- A list span is valid until the next mutation of that `Character`, or until it is copied, moved or destroyed.
- List and opinion members are defined in `character_lists.cpp` and `personal_opinion.cpp`.

**Depends on:** bipolar, character_lists, date, fixed_vector, ids, integral, opinion_records; the source uses hundredths.

#### `stance_table`
**Files:** `include/sim/stance_table.hpp`, `src/stance_table.cpp`
**Purpose:** The world-wide table of explicit stances from a community towards a community or topic, plus the community hierarchy through which stances are inherited.
**Key contents:**
- `set_stance` (clamps to -100..+100, overwrites), `clear_stance`, `explicit_stance`.
- `set_parent`/`clear_parent`/`parent`. `set_parent` returns `Conflict` for a cycle and `Full` beyond `MAX_COMMUNITY_DEPTH` = 6 (PLACEHOLDER).
- `stance(from, to)`: tries each source in `chain(from)` against each target in `target_chain(to)`, nearest first on both sides; the first explicit entry wins, otherwise 0.
- `stances(sources, targets, out)`: batch form writing to `out[i * targets.size() + j]`.
- `chain`/`target_chain` return `CommunityChain`/`TargetChain`; a topic's chain is the topic alone.
- Storage: `StanceEntry` and `ParentEntry` in two sorted vectors, searched by binary search.

**Rules / invariants:**
- An explicit 0 is a real stance; `clear_stance` restores inheritance.
- `stances()` asserts at most `STANCE_BATCH_MAX_SOURCES` = 8 sources and a large enough `out`. In release, a failed precondition fills `out` with 0.
- Edits check, then reserve, then write. Lookups never allocate. The APIs return ids, never vector indices.

**Depends on:** bipolar, edit_result, fixed_vector, ids, integral; the source uses vector_growth.

#### `relation_graph`
**Files:** `include/sim/relation_graph.hpp`, `src/relation_graph.cpp`
**Purpose:** Stores relations as one `RelationEdge` list per node, sorted by `other`, with node ids 1..`node_count()`. The class is public, but its mutators do not maintain holders, so the registry is the only editor of its own graph.
**Key contents:**
- `reserve_node()` (may throw), then `add_node()` (cannot fail).
- `set_relation`/`clear_relation` (one-way types, a → b only) and `link`/`unlink` (paired types, both edges). Each reports `EdgeFlips {forward, backward}`: whether an edge was created from nothing or erased entirely.
- `link` checks for `Duplicate`, then `Conflict` (a type and its complement on one edge), then `Full` (more than `MAX_PARENTS` parents).
- Queries: `has_relation`, `relations` (a span), `parents`, `children`, `siblings` (via shared parents), `shared_parents`.
- Helpers for death and forgetting: `remove_non_surviving`, `clear_one_way`, `has_edge`, `forget_node`, `has_one_way_edges`.
- `allocated_bytes()`; growth minimums `MIN_NODE_CAPACITY` = 16 and `MIN_EDGE_CAPACITY` = 4.

**Rules / invariants:**
- Edits check, then reserve in both lists, then write; a failure changes neither side. An edge whose mask reaches 0 is erased.
- Check order: Invalid (id 0, a == b, wrong type or kind), then NotFound (no node), then the existing-edge results.

**Depends on:** edit_result, fixed_vector, ids, relations; the source uses vector_growth.

#### `dead_record`
**Files:** `include/sim/dead_record.hpp`
**Purpose:** `DeadRecord` is what remains of a character after `CharacterRegistry::kill`. It is written once, never changed, and deleted when the character is forgotten.
**Key contents:**
- Fields: `id`, `name`, `birth`, `death`, `main_community` (at death; invalid if none), `reputation` (frozen at death), `gender`, `fame`.
- 24 bytes with alignment 4; static_asserts check unique object representations and trivial copyability.
- Parents, children and spouses stay in the relation graph and are not copied into the record.

**Depends on:** character, date, ids.

#### `mortality`
**Files:** `include/sim/mortality.hpp`, `src/mortality.cpp`
**Purpose:** Age and death probability of a living character, as free functions. Nothing is stored, and nothing here kills.
**Key contents:**
- `MortalityConfig` (PLACEHOLDER): `gompertz_a` (0.0025), `gompertz_b` (0.07), `female_multiplier`/`male_multiplier` (1.0/1.1), `health_weight` (3.0), `stress_weight` (1.0).
- `age_years(character, now)`: age in years; negative before birth.
- `annual_mortality(character, now, config)`: hazard = a·exp(b·age)·gender multiplier·exp(health_weight·(1 − health/100))·exp(stress_weight·stress/100), summed in log space; probability = 1 − exp(−hazard). Returns 0 before birth, and 0 for a NaN result.
- `weekly_mortality(annual)` = 1 − (1 − annual)^(1/`WEEKS_PER_YEAR`), with the input clamped to [0, 1].

**Rules / invariants:**
- It uses `exp`, `log`, `expm1` and `log1p`, so unlike `opinion` it makes no cross-platform identity claim.
- The header notes a known gap: plain Gompertz has no infant or child mortality peak.

**Depends on:** character, date.

#### `personal_opinion`
**Files:** `include/sim/personal_opinion.hpp`, `src/personal_opinion.cpp`
**Purpose:** Free functions that read a character's personal opinions, and the definitions of `Character`'s keyed mutators that write them.
**Key contents:**
- `person_limit(a)`: the people-list limit, computed from extraversion with integer rounding: 8, 24 and 40 at -100, 0 and +100.
- `long_opinion(a, CharacterId|TargetId)`: the stored long-term value, or 0 without an entry.
- `short_opinion(a, CharacterId|TargetId)`: the unclamped sum of the matching modifiers' effects. `has_modifier(a, CharacterId)`: whether any person modifier targets that character.
- `Character::add_long_opinion` updates, removes at 0, or inserts. At the limit it evicts the weakest |value| (ties go to the smaller target), but only if the new value is strictly stronger; otherwise it returns `Dropped`. At most one eviction per call.
- `Character::add_modifier`/`remove_modifier`: `target_changed` reports the first or last person modifier on a target.
- `Character::trim_long_opinions` evicts down to `person_limit`; `maintain` uses it.

**Rules / invariants:**
- At most one modifier per (domain, target, `ModifierId`); a repeat returns `Duplicate`. At `MODIFIER_CAP` the result is `Full`, with no eviction.
- Nothing depends on time, and the read functions never write.

**Depends on:** character, ids, opinion_records; the source uses bipolar.

#### `opinion`
**Files:** `include/sim/opinion.hpp`, `src/opinion.cpp`
**Purpose:** Computes weak opinions (without personal history) and full opinions (weak + long + short) towards a living character, a dead character, or a community or topic. Results are never stored or cached.
**Key contents:**
- `compat(a, b, config)`: weighted closeness of five personality traits, from -`k_compat` (opposite) to +`k_compat` (identical).
- `openness_factor(a, config)`: linear between `openness_factor_min` and `openness_factor_max` over openness -100..+100.
- `weak_opinion`/`weak_opinion_breakdown`, `Character` target: Σ share_A(c)·share_B(d)·stance(c → d) + `k_rep`·reputation + compat + `k_noise`·noise, clamped to -100..+100. Stances come from one `StanceTable::stances` batch, and raw weights are divided once.
- `TargetId` target: the community term plus noise scaled by `openness_factor`. `DeadRecord` target: the stance towards its `main_community`, frozen reputation, compat 0.
- `opinion`/`opinion_breakdown` = clamp(weak.total + `long_opinion` + `short_opinion`, -100, +100).
- `WeakOpinionBreakdown`/`OpinionBreakdown` return each term separately.

**Rules / invariants:**
- Only `+ - * /` and comparisons on doubles (no exp, log or pow). The header says results are identical across platforms under the project's flags.
- a == b asserts in debug and returns a zeroed breakdown; an invalid `TargetId` returns 0. Nothing allocates.

**Depends on:** character, dead_record, ids, noise, opinion_config, stance_table; the source uses personal_opinion.

#### `world_context`
**Files:** `include/sim/world_context.hpp`
**Purpose:** `WorldContext` holds const references to a `StanceTable`, an `OpinionConfig` and a `LifecycleConfig`, plus a `WorldSeed` by value. It carries the world-wide inputs of an operation; `CharacterRegistry::kill` is its only user so far. Because it holds references, it is built at the call site and never stored.
**Depends on:** lifecycle_config, noise, opinion_config, stance_table.

#### `character_registry`
**Files:** `include/sim/character_registry.hpp`, `src/character_registry.cpp`
**Purpose:** The only creator of characters, the owner of their `RelationGraph` and the keeper of the dead. Relations and personal opinions change only through it. It counts holders per id and forgets a dead character when that count reaches 0.
**Key contents:**
- `create`: ids start at 1 and are never reused; after 2^31 − 1 ids it returns `CharacterId{}`.
- `kill(id, death, WorldContext)` → `KillResult {result, fame, legendary}`: rebases holders' long entries, drops non-surviving relations, stores a `DeadRecord`, then forgets characters left with 0 holders.
- `find` (living only), `find_dead`, `exists`, `holders`, `characters()` (the living in id order).
- `set_relation`/`clear_relation`/`link`/`unlink` wrap `RelationGraph` and update holders.
- `add_modifier`, `remove_modifier`, `add_long_opinion`; `maintain()` trims people lists above `person_limit`.
- Storage: the living sorted by id, dead records unordered, and a `u32` slot and holders count per id ever created.

**Rules / invariants:**
- A pointer, reference or span from the registry or a `Character` is valid only until the next registry mutation.
- Check order: Invalid, then NotFound (never created or forgotten), then Invalid for a dead participant where not allowed, then the list's or graph's results. `kill` returns NotFound for a dead id.
- `create`, `kill` and `maintain` reserve before writing, so `bad_alloc` changes nothing. Forgetting never allocates or cascades.

**Depends on:** bipolar, character, date, dead_record, edit_result, fixed_vector, ids, integral, noise, opinion_config, opinion_records, relation_graph, relations, stance_table, world_context; the source also uses hundredths, opinion, personal_opinion, vector_growth.

## 4. Discrepancies and open questions

- `edit_result.hpp`: the check order in its comment leaves out a step. `CharacterRegistry` (`check_relation_edit`, opinion edits) and CLAUDE.md return Invalid for a dead participant after the NotFound check.
- `relation_graph.hpp`: the class comment says "Spans and vectors obtained from the graph are invalidated by any edit or new node". But `children()` and `siblings()` return `std::vector` by value, which are independent copies, as the registry header and CLAUDE.md state. Only the `relations()` span can be invalidated.
- `character.cpp`, constructor: the comment "The registry (a later step) is the only creator" is stale, since `CharacterRegistry` now exists.
- `bipolar.hpp`: the comment lists "later opinions" among the whole-number scales stored directly. That is stale: opinions exist now, computed opinions are doubles, and stored long-term values use -200..+200. Only modifier effects are stored on -100..+100.
- `character_registry.hpp`: the personal-opinion check order says "NotFound (an id never created)", but the code (`exists`) also returns NotFound for forgotten ids, as the relation comment in the same header says. `LongOpinionOutcome::Invalid` in `opinion_records.hpp` lists "id 0, a == b, or an invalid TargetId" but not the dead holder, for which the registry also returns `Invalid`.
- CLAUDE.md (Death, opinions at death) says the rebase delta is "clamped to +-200". `kill` clamps the delta to ±`LONG_DELTA_MAX` (400), and `add_long_opinion` clamps the stored value to ±200. Behaviour is the same, because both weak totals lie in -100..+100.
- CLAUDE.md (Kill is confined by holders) says scratch for "holders(id) + degree + 1" is reserved. `kill` actually reserves holders + `long_people().size()` + `modifiers().size()` (both domains) + edge count + 1: all of the deceased's own references, not only its graph degree.
