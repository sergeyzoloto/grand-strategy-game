# Migration Domain Audit

**Date:** 2026-09-07
**Scope:** `domains/migration/` — 22 entries, MIG-001 through MIG-022.
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/migration/`, and cross-checks against `domains/health/`, `domains/agriculture/`, `domains/war/`, `domains/commerce/`, `domains/engineering/`.

## Summary of the pattern

Migration shares Maritime's best qualities: region balance is strong (26.7% Europe+Mediterranean, the best of any domain audited this cycle, with Africa pulling an unusually equal 16% share), the contested ratio is the highest of any domain audited (6 of 22, 27%), and at least six entries — `MIG-001`, `MIG-003`, `MIG-007`, `MIG-011`, `MIG-012`, `MIG-013`, `MIG-017` — go out of their way to narrate their own boundary split from AGR, ENG, WAR, COM, or LAW in their own opening paragraph, more instances of that self-documenting discipline than any other domain checked.

Two real problems recur, though, and one of them is close to systemic. First: this is a domain about how populations get from one place to another, and it never explains how any population got to the place every one of its entries assumes it's already standing in — the peopling of the Americas, and human dispersal into every other region this record covers, doesn't exist anywhere in this wiki. Second, and more interesting because it's a genuine pattern rather than one entry's oversight: three separate entries (`MIG-012`, `MIG-013`, `MIG-016`) cite a specific Health-domain entry by ID in their own prose to explain a public-health or demographic consequence of migration, and none of the three ever promotes that citation into `domains_touched` or `see_also`. The same specific bug, in the same specific direction (crediting HLT correctly in prose, forgetting it in frontmatter), three times. That's not three isolated typos — that's a domain that reliably under-credits health consequences of movement the moment it reaches for one.

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **FAIL**

The earliest entry, `MIG-002` (Wave of Farmers, -9000), opens with Neolithic demic diffusion — a process that presupposes hunter-gatherer populations already occupy Europe, the Fertile Crescent, East Asia, and Africa for farmers to spread into or displace. Nothing anywhere in this wiki covers how anatomically modern humans reached any of those regions in the first place, and nothing covers the single most load-bearing case for this specific domain: the Beringia land-bridge crossing into the Americas (roughly -20000 to -14000), without which `MIG-001`'s own absence note ("no horse existed in the Americas... Plains Indigenous horse cultures are consequently a post-1500 phenomenon") and `MIG-005`/`MIG-012`'s entire frontier-colonization-of-the-Americas framework have no population already there to encounter. Confirmed by grep: zero hits anywhere in the wiki for `beringia|land bridge|peopling of the americas|out-of-africa`. This domain's basic object is movement itself, and its first, most foundational instance is missing entirely.

### 2. SUBSTRATE GAP — **FAIL**

Two concrete absences:

- **Desert mobility.** `MIG-001` gives the domain a detailed, dedicated mobility-threshold entry for the horse and steppe grassland travel; no equivalent exists for the camel and desert travel, despite the camel being mentioned in three other domains (`AGR-027`, `COM-028`, `HLT-012`) as a working assumption. `MIG-009`'s own Sahelian and trans-Saharan diaspora-network cases (Hausa/Dyula) have no mobility-precondition entry to prereq the way the steppe-migration entries (`MIG-006`, `MIG-007`, `MIG-008`) prereq `MIG-001`/`MIG-003` — an asymmetry between the two mobility technologies this domain otherwise treats as structurally parallel.
- **The refugee/asylum legal track.** `MIG-017` covers the ordinary passport/visa gate and `MIG-018`/`MIG-020` cover a state actively producing forced movement — but nothing covers a person fleeing violence or persecution on their own initiative, claiming protection under a legal category built specifically to not depend on ordinary immigration criteria. Confirmed by grep: zero hits anywhere in the wiki for `refugee|asylum`. Given this domain's 20th-century arc (`MIG-017` through `MIG-022`) is explicitly about the state's growing administrative grip on movement, the one major 20th-century migration-governance institution that exists specifically to constrain that grip (the 1921 Nansen system, then the 1951 Refugee Convention and UNHCR) is a conspicuous absence.

### 3. SCALE GAP — **PARTIAL FAIL**

Sorting the 22: world-system (`MIG-002`, `MIG-006`, `MIG-009`), state (`MIG-007`, `MIG-008`, `MIG-012`, `MIG-017`, `MIG-018`, `MIG-020`, `MIG-021`, `MIG-022`), city/regional (`MIG-014`, `MIG-016`), household/individual (`MIG-001`, `MIG-003`, `MIG-011`, `MIG-013`, `MIG-019`). This is a genuinely well-distributed spread — better than most domains audited. What's missing is a scale this domain is unusually well-positioned to have and doesn't: the **ongoing economic tie between a migrant and the household or economy they left**, as distinct from the one-time cost of the move itself. `MIG-019`'s own Hook comes right up to naming this gap and stops: "a transfer of investment that shows up nowhere in ordinary trade or migration accounting." Nothing in this domain — not `MIG-016`'s wage-differential pull, not `MIG-022`'s postcolonial channel — models the return flow (remittances) that sustains chain migration and can exceed formal foreign aid to a sending economy.

### 4. REVERSE-DIRECTION GAP — **PARTIAL FAIL**

This domain does better than most on reversal in absolute terms — `MIG-020` (Repatriation) is a dedicated reverse-flow entry, and `MIG-018` covers state-forced expulsion — so it isn't a near-zero case. But the specific reversal missing is the one that would complete this domain's own passenger-line arc: nothing models a mass-migration channel being **legislatively slammed shut** rather than administratively regulated or gradually saturated. `MIG-015` establishes cheap, scheduled mass emigration as "the single biggest drop in the effective cost and uncertainty of long-distance migration in this record"; `MIG-014` mentions Chinese exclusion acts only as a passing absence note inside a gold-rush entry, never as its own event. The 1882-1924 run of American immigration restriction (Chinese Exclusion through the National Origins Act) is a well-documented, high-consequence case of an open channel being closed by unilateral legislative act — a mechanically distinct event from `MIG-017`'s general documentation-gate mechanic, and currently nowhere.

### 5. PREREQ ORPHANS — **PASS on all internal and cross-domain link resolution; FAIL on prose-to-frontmatter promotion**

Every `prereqs`/`enables`/`see_also` ID in this domain — internal and cross-domain (`COM-028`, `REL-014`, `REL-010`, `AGR-017`, `LAW-012`, `DIP-004`, `AGR-014`, `DIP-007`, `AGR-008`) — resolves to a real entry, and no chronological inversions were found anywhere in the domain's prereq chains (checked systematically; every prereq's own `date_start` precedes the dependent entry's `date_start`). The genuine finding is a different, more interesting bug: **`MIG-012`, `MIG-013`, and `MIG-016` each cite `HLT` entries by ID directly in their own prose and never once put those IDs anywhere in frontmatter.** `MIG-012`'s and `MIG-013`'s opening paragraphs both cite `HLT-017` ("the demographic collapse of indigenous populations") — verified against `HLT-017`'s actual content, which does cover exactly this — with neither entry's `domains_touched` including `health` nor either ID appearing in `see_also` in either direction. `MIG-016`'s Social-consequence section cites both `HLT-002` and `HLT-006` by name with the same gap. Three entries, one specific and consistent failure mode.

### 6. MECHANISM DIVERSITY — **PASS, with the domain's usual missing pool**

A genuinely rich spread: gates (`MIG-001`, `MIG-003`, `MIG-011`, `MIG-017`), an emergent population-pressure diffusion type found nowhere else in this record (`MIG-002`), branches that explicitly reuse design principle 1 (`MIG-006`, `MIG-008`), a volatile civic-institution stat (`MIG-007`'s `confederation cohesion`), a genuinely novel authority/vulnerability-decoupling mechanic reusing design principle 3 (`MIG-010`), a zero-agency event type with no equivalent anywhere else in this domain (`MIG-013`), sudden-trigger events (`MIG-014`, `MIG-018`, `MIG-020`), and a deliberate direct-inverse pairing (`MIG-017`/`MIG-021`). Roughly ten of twenty-two lean toward straightforward "modifier," which is worth naming though it doesn't tip into a flat bonus-list failure given the diversity elsewhere. What's absent, matching every other domain audited this cycle before its fixes: a genuine **depleting pool** (design principle 4). `MIG-007`'s `confederation cohesion` is fragile and collapse-prone but isn't framed as something requiring standing investment to maintain; `MIG-009`'s `diaspora reach` is the closest candidate (built up over generations, erodible) but isn't modeled with pool language either.

### 7. BOUNDARY INTEGRITY — **PASS, with one soft case worth naming**

This domain's boundary discipline is the most extensively self-documented of any audited: `MIG-001` (AGR/MIG), `MIG-003` (ENG/MIG), `MIG-007` (WAR/MIG), `MIG-011` (WAR/MIG), `MIG-012` (explicitly flags itself as a future thread-page candidate rather than overclaiming scope), and `MIG-013` (COM/LAW/MIG) all narrate their own split in their opening paragraph. The one soft case: `MIG-009`/`MIG-010` (diaspora merchant networks) sit close enough to COM's "merchant networks" territory that a stricter reading could argue for it — but both entries are explicitly about the settled population and the trust structure its existence creates, cross-referencing `COM-027` for the trade-mechanic sibling rather than duplicating it, which is the same correct-split pattern `MIG-013` uses for its own COM boundary. Worth naming as a boundary worth periodically re-examining, not a violation.

---

## PART B — Proposed Additions

### LOAD-BEARING (4)

**1. Peopling of the Continents (Pleistocene Human Dispersal)**

- Regions: middle-east, europe, east-asia, americas — era, roughly -70000 to -12000
- Answers: Probe 1 (Emergence) directly — the single most load-bearing gap found
- Mechanism: unassisted, on-foot dispersal into previously unpopulated territory, paced by a small founding population's demographic growth and foraging-range expansion rather than any single long-distance journey — mechanically similar in shape to `MIG-002`'s wave-of-advance but pre-agricultural, and terminating in the first human presence in a landmass rather than displacing an existing population there.
- Hook (seed): an origin gate — establishes the human population in each of this record's focus regions that every other migration mechanic in this domain silently assumes is already there to move, resettle, or be displaced.
- Prereqs: none. Enables: `MIG-002` (currently missing this link).
- Contested: true — Beringia's exact timing, route (coastal versus interior ice-free corridor), and number of distinct migration pulses are actively revised by ancient-DNA and pre-Clovis archaeological evidence against the older "Clovis First" consensus.
- Confidence: high on the overall event and its necessity to this record; medium on the specific dates given ongoing scientific revision.

**2. Refugee Status and Asylum as a Legal Category**

- Regions: europe, middle-east, south-asia, east-asia — century, roughly 1921 to 1980
- Answers: Probe 2 (substrate) directly — the bottom-up-flight track missing from this domain's 20th-century state-control arc
- Mechanism: a legal category distinct from ordinary voluntary economic migration, granting a person fleeing persecution or violence a claim to protection (non-refoulement) that doesn't depend on a destination state's ordinary quota or economic-migration criteria, increasingly administered through international institutional machinery (League of Nations Nansen offices from 1921, then UNHCR from 1950) rather than purely bilateral state discretion.
- Hook (seed): a gate parallel to `MIG-017`'s ordinary passport/visa gate but triggered by persecution rather than economic calculus, and partly administered by an international institution rather than the destination state alone.
- Prereqs: `MIG-017`. Enables: none.
- Contested: true — the boundary between a "genuine" persecution-based refugee and an economic migrant using an asylum claim strategically is a live, politically contested question in every era this entry covers, not only the present; model recognition outcomes as branching on host-state institutional capacity and political climate rather than a fixed legal test always applied consistently.
- Confidence: high.

**3. Camel Caravan Mobility and Desert Crossing**

- Regions: middle-east, africa, central-asia-steppe — era, roughly -1000 to 1500
- Answers: Probe 2 (substrate) directly, closing the asymmetry between this domain's detailed steppe-mobility foundation and its complete absence of a desert-mobility equivalent
- Mechanism: a camel can travel multiple days between water sources and carry substantial cargo across true desert terrain no horse, ox, or human porter makes economically survivable at comparable scale — the specific mobility unlock that converts trans-Saharan and Arabian desert crossing into a repeatable migration and trade corridor rather than a one-off, high-mortality gamble.
- Hook (seed): a gate structurally parallel to `MIG-001`'s horse but keyed to water-scarcity-over-distance rather than open-terrain speed, gating trans-Saharan and Arabian population and trade movement the way the horse gates steppe movement.
- Prereqs: none. Enables: `MIG-009` (currently missing this link for its own Sahelian/trans-Saharan cases).
- Contested: false.
- Confidence: high.

**4. Immigration Restriction and Quota Systems**

- Regions: americas, oceania — decade, roughly 1882 to 1924
- Answers: Probe 4 (reverse-direction) directly — the missing legislative-closure mirror to `MIG-015`'s channel-opening
- Mechanism: a destination state legislates a hard numerical or categorical cap on immigration (national-origin quota, or outright exclusion by ethnicity) independent of any individual migrant's ability to pay passage or otherwise qualify, converting a previously open or lightly-regulated mass-migration channel (`MIG-015`) into a closed or sharply throttled one by unilateral legislative act rather than gradual market saturation or destination-side wage decline.
- Hook (seed): an event distinct from `MIG-017`'s general documentation-gate mechanic — a sudden, legislated closure of a specific existing channel rather than a general administrative-permission layer applied evenly.
- Prereqs: `MIG-015`. Enables: none.
- Contested: false.
- Confidence: high.

### VALUABLE (3)

**5. Migrant Networks: Remittances and Chain Migration**
Regions: europe, americas, middle-east, south-asia. Century, roughly 1850-2000. Answers Probe 3 (scale gap) directly — the household-economic-tie level `MIG-019`'s own prose names as missing and this domain otherwise lacks entirely. Mechanism: a migrant sends earnings back to an origin household on an ongoing basis, and prior migrants' presence at a destination lowers the risk and cost of the next migrant's journey from the same village or kin network — two faces of one self-reinforcing network effect, distinct from any one-time transport or settlement cost `MIG-015`/`MIG-016` already track, and capable of exceeding formal foreign aid or investment to a sending economy. Hook: a standing income-and-information flow between migrant and origin household that funds and directs further migration, the closest thing to a genuine depleting/building pool this domain currently lacks (design principle 4, though building rather than depleting). Prereqs: `MIG-015`. Contested: false. Confidence: high.

**6. Transhumance (Seasonal Pastoral Migration)**
Regions: mediterranean, middle-east, central-asia-steppe. Era, roughly -6000 to 1900. Answers Probe 6, adding a mechanism genuinely distinct from `MIG-001`/`MIG-003`'s threshold-unlock gates: cyclical, short-range movement between fixed seasonal pastures requiring no mobility-technology breakthrough at all, just herd-based subsistence and customary or contested grazing rights — the most geographically universal and technologically undemanding migration pattern in this domain, already named as a contrast case inside `MIG-003`'s own prose but never covered on its own terms. Hook: a modifier — a steady-state, non-expanding, non-threshold-gated mobility pattern that recurs independent of horse, wagon, or camel technology, and generates its own land-use conflict dynamic wherever it overlaps with settled agriculture. Prereqs: none. Contested: false. Confidence: high.

**7. Circular and Return Migration**
Regions: europe, americas. Decade, roughly 1880-1920. Answers Probe 6, adding a mechanism `MIG-015`'s one-way-passage framing and `MIG-020`'s state-driven-repatriation framing both miss: a migrant traveling specifically intending a savings target and voluntary return rather than permanent settlement, a household economic strategy only viable once steamship service (`MIG-015`) makes a round trip cheap and fast enough to plan around — a large, well-documented share (often a third or more) of the era's transatlantic emigrants historically returned home permanently. Hook: a branch — an individual-choice return pattern running at real scale alongside permanent emigration within the same passenger-line era, complicating any simple one-way emigration count this domain's other entries implicitly assume. Prereqs: `MIG-015`. Contested: false. Confidence: high.

### OPTIONAL (2)

**8. Immigration Processing Stations (Ellis Island, Angel Island)** [REJECTED]
Regions: americas. Decade, 1892-1954. The specific administrative-institutional color of `MIG-015`/`MIG-017`'s already-covered mechanics made physical — genuinely good, specific history (Angel Island's markedly harsher processing of Chinese arrivals under exclusion-era law versus Ellis Island's comparatively lighter European processing is a real, telling contrast) but no new mechanism beyond what those two entries already establish. Honestly flavour.

**9. The Great Trek (Voortrekker Frontier Migration)** [REJECTED]
Regions: africa. Decade, 1830s-1840s. A specific, well-documented instance of `MIG-005`'s general frontier-colonization mechanic (land-for-risk, direct conflict with existing occupants, a distinct emergent frontier social profile) that would add genuine African-continent texture to a mechanic this domain currently illustrates mostly through European and American cases, but contributes no mechanism `MIG-005` doesn't already fully cover. Honestly flavour.

---

## PART C — Proposed Modifications

### SPLIT

None found. `MIG-012` explicitly flags itself in its own prose as "a strong future candidate for a `_meta/threads/` page" — checked seriously against the Multi-domain rule's "two `## Mechanism` sections" test and found to genuinely have only one: its three settlement channels (free family, indentured, administrative/extractive) are three instantiations of a single mechanism (channel choice shapes downstream colonial social structure), the same pattern `MIG-008` uses for its three steppe-wave resolutions. No split needed now; a thread page tying `MIG-012` to the WAR/COM/HLT/LAW facets of the same broader colonization topic remains a good idea once those domains have more to link, but that's an organizational aid, not a structural requirement.

### MERGE

**`MIG-018` (Deportations and Population Exchanges) and `MIG-020` (Repatriation) — recommended.** Both entries run the identical underlying mechanism: a state uses the same documentation/border-defining apparatus `MIG-017` establishes to force population movement by category, with the outcome branching on whether a negotiated diplomatic framework governs the transfer (orderly) or not (violent) — `MIG-020`'s own opening paragraph has to work to justify a separate mechanic ("a distinct mechanic... since here the state acting is typically either the migrants' own origin state pulling them back or a destination state pushing its own resident population out"), but the actual branching variable in both entries' own Hooks is the same negotiated-versus-violent axis, not who initiates. Unlike Maritime's Piracy/Smuggling case (kept separate on genuinely divergent downstream social consequences), these two don't diverge enough to earn separate entries: recommend merging into one entry — "State-Driven Forced Population Movement" — with the bilateral-exchange cases (Greek-Turkish, Partition) and the unilateral-repatriation cases (Mexican, Japanese, pied-noir) both represented as region/case variants of one mechanism rather than as two entries independently reinventing the same branch logic.

### RE-DOMAIN

- **`MIG-012`, `MIG-013`, `MIG-016`** each cite an `HLT` entry by ID in their own prose (`HLT-017` twice, `HLT-002`/`HLT-006` once) without `health` appearing in `domains_touched` on any of the three, and without the citation ever appearing in `see_also` in either direction. Recommend: add `health` to all three entries' `domains_touched`; add `HLT-017` to `MIG-012` and `MIG-013`'s `see_also` (and reciprocally on `HLT-017`'s side); add `HLT-002` and `HLT-006` to `MIG-016`'s `see_also` (and reciprocally on both HLT entries' sides).
- No entry's canonical `domain` needs to change. The `MIG-009`/`MIG-010` soft case against COM (noted under Probe 7) is a boundary worth remembering exists, not a violation to fix.

### RETIER

None recommended. The domain's 18-core/4-standard split is core-heavy by ratio, but every `core` entry checked contributes either a unique stat (`migration range`, `settler footprint`, `displacement pressure`, `confederation cohesion`, `diaspora reach`, `migration trigger intensity`, `urban absorption capacity`, `border friction`, `skill differential` each originate somewhere specific and get reused deliberately rather than duplicated) or a distinct mechanism type found nowhere else in the domain. The four existing `standard` entries (`MIG-003`, `MIG-011`, `MIG-020`, `MIG-021`) are already correctly humble, single-instance or directly-inverse entries. I looked for a `MIG-009`-style isolated, `enables: []`, single-region case the way Maritime's `MAR-009` supplied and didn't find one here that wasn't already tiered appropriately.

### CUT

**None as a standalone cut** — but the `MIG-018`/`MIG-020` MERGE above functions as this audit's version of one, reducing two mechanically-overlapping entries to one. I read every entry specifically looking for a self-admitted redundancy or a Hook with no real mechanic and, `MIG-018`/`MIG-020` aside, didn't find one: even the domain's single-instance entries (`MIG-011`, `MIG-014`) generalize into a real, distinct mechanic type rather than merely retelling one event.

---

## PART D — Verdict

**Worst structural problem:** this is a domain built entirely around the mechanics of how populations move, resettle, get expelled, and stay connected across four thousand years of recorded history — and it has no entry for how any population arrived at the places every other entry assumes they already occupy, the Beringia crossing chief among them given how directly `MIG-001`, `MIG-005`, and `MIG-012` depend on an American population already being there.

**Entry count:** 22 before. With the `MIG-018`/`MIG-020` merge: 21. With all four LOAD-BEARING proposals: 25. With VALUABLE also adopted: 28. With OPTIONAL: 30.

**Proposed out of symmetry, not mechanical need:** both OPTIONAL entries (#8 Immigration Processing Stations, #9 The Great Trek) are named as such — real, specific history contributing no mechanism beyond what `MIG-005`/`MIG-015`/`MIG-017` already establish. Of the VALUABLE tier, **Transhumance (#6)** is the softest call: it's genuinely universal, well-documented texture, but the domain's structural coherence doesn't depend on it the way it depends on the LOAD-BEARING tier's four proposals — worth adding for a mechanism-diversity dividend, skippable if you want to hold the line at four new entries.

**Does Migration justify separate existence?** Yes, as clearly as Maritime did. Its region balance is the best of any domain audited this cycle, its self-documented boundary reasoning against AGR, ENG, WAR, COM, and LAW is extensive and correct, its contested-scholarship ratio is the highest checked, and its mechanism diversity is real even before the missing-pool fix. The problems found here are specific and fixable rather than structural: an origin story the domain never told, one recurring prose-to-frontmatter credit bug against a single sibling domain, and two entries doing one job that could do it better combined.
