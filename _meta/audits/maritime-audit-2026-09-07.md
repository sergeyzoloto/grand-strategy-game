IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

# Maritime Domain Audit

**Date:** 2026-09-07
**Scope:** `domains/maritime/` — 29 entries, MAR-020 through MAR-370.
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/maritime/`, and cross-checks against `domains/war/`, `domains/engineering/`, `domains/commerce/`, `domains/finance/`, `domains/nature/`.

## Summary of the pattern

Maritime is the most disciplined domain audited in this series so far by several measures: region balance sits at 36.9% Europe+Mediterranean (under the 40% flag line, and better than Commerce, Agriculture, or Engineering managed), tier distribution is less monolithic than most domains at first pass (22 core / 7 standard), confidence is high on 26 of 29 entries, and two entries (`MAR-120`, `MAR-140`) go out of their way to narrate their own boundary-carve-out logic in prose — a level of self-documentation no other domain audited has shown. The prereq chains (coastal trade → navigation → ocean crossing; warships → line-of-battle → ironclad → battleship → dreadnought → carrier; torpedo → Jeune École → destroyer → submarine) are genuinely elegant, and stat reuse (`route knowledge`, `protection rent`, `naval prestige`, `contract enforcement`, `legibility`) is varied rather than flattened into one bonus.

The blind spot, once you look for it, is consistent and specific: **every entry is written from the vantage of the ship as a finished strategic or economic unit — something a state fields, a merchant insures, an admiral commands — and never from the vantage of building one, catching fish with one, or being conscripted to crew one.** The domain has a warship (`MAR-040`) before it has a hull. It has ocean navigation (`MAR-160`) explicitly listing "large enough vessels to carry the food and water reserves a multi-week passage requires" as one of three necessary components, and then never once mentions the disease that made those reserves insufficient anyway. It has piracy, privateering, smuggling, and insurance — every non-state economic relationship to a cargo — but not fishing, the one maritime economic activity that isn't about moving goods between two points at all. And it has crews, oarsmen, and dockworkers mentioned as fiscal or labor-organizing footnotes inside institution-level entries, but no entry that takes the sailor's own conscripted, deserting, mutinying experience as its subject the way `MAR-060` takes the navy's own institutional experience as its subject.

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **FAIL**

The earliest entry, `MAR-020` (Coastal Trade, -3000), opens by stating coastal trade is "gated only by basic seaworthy vessel technology" — and nothing anywhere in this domain, or in Engineering (which explicitly cedes "navigation and shipbuilding technique" to Maritime in `boundary-rules.md`), explains how a hull capable of carrying bulk cargo at sea came to exist at all. The domain's basic object — a vessel that floats, carries load, and survives open water — is assumed fully solved at entry zero. This is the same shape of gap Commerce's original "exchange with a stranger" hole had: the domain begins at the first _use_ of its foundational technology, not its invention.

### 2. SUBSTRATE GAP — **FAIL**

Several unglamorous layers every other entry silently assumes:

- **Hull construction itself.** No entry anywhere covers shipwrightry — plank-on-frame versus shell-first construction, Northern European clinker-building, South Asian sewn-plank dhow hulls, East Asian bulkhead-and-junk construction. `MAR-080` covers rigging; nothing covers the hull the rigging is attached to.
- **Naval gunnery, the weapon.** `MAR-210`'s entire "rate" mechanic and `MAR-200`'s ironclad-obsolescence mechanic both depend on shipboard artillery existing and improving, and neither explains it. `WAR-150` (Artillery Doctrine) is confirmed land-siege-and-field-only — it never mentions naval guns. The weapon technology every warship-type entry from `MAR-210` onward assumes is uncovered anywhere in the wiki.
- **Provisioning and scurvy.** `MAR-160`'s own Mechanism section names "large enough vessels to carry the food and water reserves a multi-week or multi-month passage requires" as one of exactly three necessary components of ocean navigation — and the domain never once models the actual limiting factor (vitamin-C deficiency) that capped safe voyage duration independent of a ship's cargo range, or the fix that removed the cap.
- **Fishing and whaling.** Confirmed zero hits anywhere in the wiki (`grep -rliE "whaling|fishery|fisheries"`). An entire major maritime economic activity — extraction from the sea rather than transport across it — is absent, despite being economically central to multiple regions in the wiki's own timeframe (Basque and Yankee whaling, North Sea and Grand Banks fisheries, Japanese coastal fishing).
- **Lighthouses and aids to navigation.** Zero hits anywhere. `MAR-050` and `MAR-250` cover a ship's own onboard technique for finding its way; nothing covers the fixed, shared, state-or-guild-maintained infrastructure that reduces wreck risk for every vessel regardless of its own crew's skill.

### 3. SCALE GAP — **PARTIAL FAIL**

Sorting the 29: world-system (`MAR-160`, `MAR-220`, `MAR-370`), state (`MAR-060`, `MAR-210`, `MAR-140`, `MAR-200`, `MAR-280`–`028`), city/port (`MAR-020`, `MAR-090`, `MAR-240`), firm/commercial (`MAR-120`, `MAR-260`, `MAR-150`). Every one of those levels has real texture. What's missing is the individual/labor level: the sailor as a worker rather than a line item. `MAR-040` and `MAR-060` both discuss crews explicitly as a _fiscal and manpower_ problem for the state, and `MAR-240` mentions dockworkers in passing as a labor-organizing footnote — but nowhere does the domain take the sailor's own experience (wages, desertion, coerced service, mutiny) as its actual subject the way `MAR-060` takes the navy-as-institution as its subject. Confirmed zero hits for `press.?gang|impressment|mutiny` outside one throwaway clause inside `MAR-190`.

### 4. REVERSE-DIRECTION GAP — **FAIL**

The domain's one genuine capability-loss story — Ming China's treasure fleet being deliberately dismantled — is real, well-told, and correctly cross-referenced, but it is told **three times** (in `MAR-060`'s absence case, `MAR-160`'s Regional specifics, and `MAR-140`'s Regional specifics) and is the _only_ reversal case in the entire domain. That's a single data point wearing three hats, not a category. A second, historically major, and mechanically distinct reversal case is entirely missing: the collapse of the Mediterranean-to-Red-Sea spice trade once Vasco da Gama's all-water Cape route (1498) made the old multi-stage relay network's accumulated route knowledge and toll position a stranded asset almost overnight — a market-competition obsolescence, not an institutional-abandonment one, and therefore a genuinely different mechanic than the Ming case. Clipper obsolescence (`MAR-260`) is the domain's other reversal-adjacent case, but it's normal technological supersession, not the sharper "an existing valuable capability suddenly stops being valuable" story the Cape route supplies.

### 5. PREREQ ORPHANS — **FAIL** (on cross-domain links; **PASS** on internal link integrity)

Zero internal dead links — every `prereqs`/`enables` ID inside this domain resolves to a real entry. But several entries' own prose declares a dependency their frontmatter never records:

- **`MAR-050`** (Celestial Navigation) relies entirely on astronomical observation and instrument tradition, but doesn't prereq `NAT-010` (Mathematics, Geometry, and Calendrical Astronomy), which already exists and covers exactly this toolkit.
- **`MAR-250`** (Chronometer) explicitly frames its own solution as "a precision-engineering problem... rather than an astronomical or mathematical one," yet doesn't prereq `ENG-160` (Mechanical Clockwork and the Escapement), which already exists, predates it by three centuries with no chronological conflict, and is exactly the precision-mechanism lineage a marine chronometer is built from.
- **`MAR-320`** (Submarines) never prereqs `ENG-290` (The Internal Combustion Engine), which already exists, is tagged `diesel-engine` explicitly, and is the actual propulsion technology every WWI/WWII diesel-electric submarine depends on for surface running.
- **`MAR-140`** (Admiralty) states outright that its prize-court function "is a hidden precondition for privateering (`MAR-110`) to work as an incentive at all" — and neither entry cross-references the other anywhere in `prereqs`, `enables`, or `see_also`. This is the exact "prose states a real dependency, frontmatter never promotes it" bug found repeatedly in earlier domain audits.
- **`MAR-360`→`MAR-350` is a broken and, on inspection, backwards link.** `MAR-360` (Landing Craft) lists `MAR-350` (Aircraft Carriers) in `enables` — but `MAR-350`'s own `prereqs` is `[MAR-300]` only, and there's no real historical dependency in either direction: aircraft carriers don't require landing craft. This reads as a copy-paste or association error, not a genuine relationship, and should be removed rather than reciprocated.

### 6. MECHANISM DIVERSITY — **PASS, with one real gap**

This is the richest mechanism spread audited yet: gates (`MAR-020`, `MAR-050`, `MAR-160`, `MAR-280`, `MAR-360`), modifiers (`MAR-030`, `MAR-080`, `MAR-120`, `MAR-110`, `MAR-150`, `MAR-190`, `MAR-270`, `MAR-310`, `MAR-220`), civic institutions (`MAR-210`, `MAR-140`, `MAR-300`), genuine branches (`MAR-290`'s tech-maturity-conditional bet, explicitly reusing design principle 6, paid off decades later by `MAR-320`), and sharp reset-events (`MAR-200`, `MAR-330`, `MAR-350`, `MAR-370`). Five distinct stats (`route knowledge`, `protection rent`, `contract enforcement`, `legibility`, `naval prestige`) do real, non-overlapping work. What's missing: a genuine **depleting pool** (design principle 4). `MAR-060`'s own Hook describes pool-shaped behavior almost word for word ("a slow-building, slow-losing state-capacity stock") — but its `stats_touched` is empty; the domain gestures at the pool shape without ever cashing it in as a named stat, and no other entry supplies one either. This is the same gap found and fixed in every other domain audited this cycle, and Maritime — a domain whose central asset (a fleet) is one of the most naturally pool-shaped things in the entire project — is a strange place for it to still be missing.

### 7. BOUNDARY INTEGRITY — **PASS, with one arguable case**

The domain's adherence to its own carve-out is unusually clean and, twice, explicitly self-documented: `MAR-120` narrates in its own opening paragraph why it's MAR and not FIN; `MAR-140` narrates why it bundles a WAR-adjacent and a LAW-adjacent function under one roof rather than splitting. Warship classes, doctrine, and naval institutions are consistently MAR-not-WAR throughout; sail rig and navigation technique are consistently MAR-not-ENG; port infrastructure and shipping are consistently MAR-not-COM. Canal control (Suez, Panama) is correctly left to `COM-130`, which explicitly and correctly distinguishes its own peacetime-commercial-control angle from `MAR-220`'s wartime sea-lane-denial angle — a boundary handled right on both sides, not a gap. The one genuinely arguable case: **`MAR-090`** (Northern River Navigation) is about a _river-and-portage_ corridor (the Volga-Dnieper system), and `boundary-rules.md` defines MAR's carve-out specifically as "the sea as a theatre" — a river is not the sea. The entry's own framing (substituting for a sea route between two maritime zones) is a reasonable defense for keeping it here, but it's a defense, not a clean fit, and worth naming rather than waving through.

---

## PART B — Proposed Additions

### LOAD-BEARING (5)

**1. Shipbuilding and Hull Construction**

- Regions: mediterranean, europe, south-asia, east-asia — era, roughly -3000 to 1200
- Answers: Probe 1 (Emergence) and Probe 2 (Substrate) directly — the single most load-bearing gap found
- Mechanism: hull construction technique (shell-first plank building, Northern European clinker lapstrake, South Asian sewn-plank dhow hulls, East Asian bulkhead-and-junk construction) sets the ceiling on vessel size, durability, and repairability that everything else in this domain — cargo capacity, warship specialization, ocean range — is built on top of.
- Hook (seed): a gate — the substrate technology every other entry in this domain silently assumes exists before it can open.
- Prereqs: none. Enables: `MAR-020`, `MAR-040` (both currently missing this link).
- Contested: false.
- Confidence: high.

**2. Naval Gunnery and Ship Artillery**

- Regions: europe, mediterranean — era, roughly 1350 to 1858
- Answers: Probe 2 (substrate) directly, closing `MAR-210` and `MAR-200`'s silent dependency on a weapon neither WAR nor MAR currently covers
- Mechanism: mounting gunpowder artillery on a hull converts a warship's combat value from crew size and boarding capability into gun count and caliber — the precondition for both line-of-battle "rate" accounting and, once explosive shells replace solid shot (the Paixhans gun, 1820s-40s), the sudden hull-penetrating punch that makes armor (`MAR-200`) necessary at all.
- Hook (seed): a gate — the weapon technology every warship-type entry from `MAR-210` onward assumes exists and keeps improving on its own timeline.
- Prereqs: none (domains_touched: engineering, for gunpowder metallurgy). Enables: `MAR-210`, `MAR-200` (both currently missing this link).
- Contested: false.
- Confidence: high.

**3. Naval Provisioning and Scurvy**

- Regions: europe — era, roughly 1497 to 1795
- Answers: Probe 2 (substrate) directly — the specific limiting factor `MAR-160`'s own Mechanism section names as necessary and never explains
- Mechanism: a vitamin-C deficiency disease that becomes lethal to a large fraction of a crew past roughly ten to twelve weeks without fresh provisions caps effective ocean-voyage duration independent of a vessel's actual food and water storage capacity, until a systematic dietary countermeasure (citrus, mandated in the Royal Navy from 1795) removes the cap.
- Hook (seed): a gate distinct from `MAR-160`'s own three components — voyage duration is capped by crew survival, not by cargo or navigation capability, until this is solved.
- Prereqs: `MAR-160`. Enables: none.
- Contested: false.
- Confidence: high.

**4. Fishing and Whaling**

- Regions: europe, americas, east-asia — era, roughly 1000 to 1859
- Answers: Probe 2 (substrate) directly, and Probe 6 (mechanism diversity) directly — the domain's most natural fit for a genuine depleting-pool mechanic, which it currently lacks entirely
- Mechanism: a fishery or whale population is a resource extracted directly from the sea rather than moved across it, renewable under moderate pressure but genuinely exhaustible under sustained heavy extraction — a stock-depletion dynamic neither the trade nor the warfare mechanics elsewhere in this domain model at all.
- Hook (seed): a pool — a depleting resource stock (design principle 4) drawn down by extraction effort and only slowly renewed, the domain's first genuine pool mechanic with a named stat.
- Prereqs: `MAR-020` (or the proposed Shipbuilding entry). Enables: none.
- Contested: false.
- Confidence: high.

**5. Naval Impressment and the Maritime Labor Force**

- Regions: europe, americas — century, roughly 1664 to 1815
- Answers: Probe 3 (scale gap) directly — the individual/labor-level entry the domain currently lacks entirely
- Mechanism: a state without enough voluntary maritime labor supply to crew its standing navy (`MAR-060`) resorts to coercive conscription of merchant sailors, trading a cheaper crewing solution for chronic desertion, merchant-shipping labor shortages, and — in the American case — direct diplomatic and eventually military conflict (a proximate cause of the War of 1812).
- Hook (seed): a modifier/branch — the sailor's-eye-view counterpart to `MAR-060`'s fiscal-institutional view of the same standing-navy crewing problem, with its own diplomatic-friction cost.
- Prereqs: `MAR-060`. Enables: none.
- Contested: false.
- Confidence: high.

### VALUABLE (3)

**6. Lighthouses and Aids to Navigation**
Regions: mediterranean, europe. Era, roughly -280 to 1759 (Pharos of Alexandria to Smeaton's Eddystone tower). Answers Probe 2 (substrate) and adds a mechanism genuinely distinct from `MAR-050`/`MAR-250`'s onboard-technique gates: a fixed, state- or guild-maintained coastal marker (funded, in the English case, by Trinity House's toll system) converts a stretch of dangerous coastline from a private, individually-learned hazard into a shared public good reducing wreck risk for every vessel regardless of its own crew's skill. Hook: civic — a public-infrastructure investment and its own funding-and-governance question, distinct from any individual ship's onboard capability. Prereqs: none. Contested: false. Confidence: high.

**7. The Cape Route and the Collapse of the Mediterranean Spice Trade**
Regions: mediterranean, south-asia. Century, roughly 1498-1600. Answers Probe 4 (reverse-direction) directly with a mechanism genuinely distinct from the Ming case: a new all-water route to the same source market bypasses an existing multi-stage relay network entirely, converting that network's accumulated route knowledge and toll-collecting position from an asset into a stranded cost almost overnight — not because the old network's capability was abandoned (the Ming story), but because it was simply out-competed by a shorter route someone else controlled. Hook: event — a route-obsolescence shock distinct from institutional abandonment. Prereqs: `MAR-160`. Contested: false. Confidence: high.

**8. Convoy Escort**
Regions: europe, americas. Decade, roughly 1917-1943. Answers Probe 6, adding a genuinely distinct operational-doctrine mechanism: grouping merchant shipping into naval-escorted convoys converts submarine hunting (`MAR-320`) from picking off isolated targets at will into a concentrated engagement against a defended formation, trading individual-ship routing efficiency for aggregate fleet-wide survival odds — the trade-protection mirror of `MAR-210`'s line-of-battle formation logic, applied to cargo rather than combat ships. Hook: modifier — the doctrinal countermeasure that makes `MAR-220`'s sea-lane-denial logic a contest rather than a one-sided slaughter once submarines mature. Prereqs: `MAR-220`, `MAR-320`. Contested: false. Confidence: high.

### OPTIONAL (2)

**9. The Manila Galleon Trade** [REJECTED]
Regions: americas, east-asia. Century, 1565-1815. A specific, colorful single trade route (Acapulco-Manila, Mexican silver for Chinese silk) that is mechanically just a particular instance of `MAR-020`/`MAR-160` already combined — no new mechanism, purely regional color for a genuinely striking two-and-a-half-century-long fixed trade circuit. Honestly flavour.

**10. Kon-Tiki and the Pre-Columbian Transoceanic Contact Hypothesis** [REJECTED]
Regions: americas, oceania. Twentieth-century historiographical event testing a genuinely contested and unresolved question (whether pre-Columbian South American rafts could have, or did, reach Polynesia) — a footnote to `MAR-020`'s existing Americas absence case rather than a load-bearing addition. `Contested: true` by nature, but thin mechanically: the "mechanic" is essentially just re-flagging an existing absence case as actively disputed rather than settled. Honestly flavour.

---

## PART C — Proposed Modifications

### SPLIT

None found. `MAR-140` deliberately bundles a WAR-adjacent (fleet administration) and a LAW-adjacent (prize jurisdiction) function under one institution and explicitly justifies why in its own prose (shared specialized knowledge, not two independent Mechanism-worthy stories) — checked against the general Multi-domain rule's "two `## Mechanism` sections" test and found to genuinely have only one. `MAR-220` bundles blockade and commerce-raiding as mirror-image instances of one mechanism (converting trade dependency into wartime vulnerability) rather than two separate ones. No entry in this domain is straining against its own single-Mechanism structure.

### MERGE

**Considered and rejected: `MAR-030` (Piracy) and `MAR-150` (Smuggling).** `MAR-150`'s own Mechanism section states its logic is "the same enforcement-capacity-versus-coastline-length problem that determines piracy's prevalence (`MAR-030`) determines smuggling's, except the 'predation' here is on the state's tariff or monopoly revenue rather than on other merchants' cargo directly" — about as close to "one mechanism between them" as this audit template's MERGE criterion describes. Kept separate because the two entries' downstream social consequences genuinely diverge: piracy blends into privateering and produces quasi-state tribute arrangements (Barbary), while smuggling produces bottom-up policy renegotiation without violence or state blending (colonial American case) — different enough consequences to be worth two entries, but close enough in underlying mechanism that this is a real tension, not a clean pass. No other pair in the domain comes close to this level of mechanism overlap.

### RE-DOMAIN

- **`MAR-090`** (Northern River Navigation) is a genuinely arguable case: it covers a river-and-portage corridor, and `boundary-rules.md` defines MAR's entire carve-out as "the sea as a theatre." A defensible argument for keeping it MAR exists (it substitutes for a sea route between two maritime zones, prereqs `MAR-020`, shares its `route knowledge` stat) — but it is a defense, not a clean fit under the letter of the rule. Recommendation: keep it in MAR given the substitution logic, but flag this as the domain's one boundary case worth revisiting if a dedicated river-trade cluster ever emerges elsewhere in the wiki.
- Missing cross-domain links (not a wrong canonical domain, but a missing `domains_touched`-adjacent connection): none of the specific prereq fixes from Part A Probe 5 require a `domains_touched` change, since the relevant domains (`nature`, `engineering`) are already listed on `MAR-050`, `MAR-250`, and `MAR-320` — only the `prereqs` arrays themselves need the IDs added.

### RETIER

- **`MAR-250`** (Chronometer and Coordinates) — `enables: []`, a genuinely Britain-concentrated single achievement by the entry's own admission ("unlike most other entries" in this domain), structurally identical in shape to `MAR-240` (Docks) and `MAR-260` (Clippers), both already `standard`. Recommend downgrading `core` → `standard`.
- Not touching the other 21 `core` entries: each has either a real downstream `enables` chain (`MAR-020` through `MAR-050`, `MAR-080`, `MAR-210`, `MAR-200`, `MAR-280`, `MAR-300`) or stands as the domain's sole representative of a distinct mechanism type (`MAR-120`'s financial instrument, `MAR-140`'s civic institution once its `MAR-110` link is restored, `MAR-220`'s cross-cutting strategy). The domain's 22-core/7-standard split, while still core-heavy, reflects genuine load-bearing weight more often than the all-or-nothing tier splits found in earlier domains audited this cycle.

### CUT

**None.** This is the first domain in this audit series where a genuinely adversarial read did not turn up an entry with no mechanic or a cleanly duplicated one. The closest candidate (`MAR-150` vs `MAR-030`, discussed under MERGE above) survives on divergent downstream consequences despite a shared underlying mechanism description. `MAR-090` and `MAR-270` (Monitor) are both narrow, single-case entries, but each carries a mechanism (portage-bottleneck route substitution; cost-asymmetric coastal-defense niche) genuinely distinct from anything else in the domain, and both are already tiered `standard` rather than overclaiming `core` weight. I read every entry looking for a self-admitted redundancy of the kind Commerce's and Engineering's audits turned up and didn't find one here — worth reporting honestly rather than manufacturing a cut to satisfy the expectation.

---

## PART D — Verdict

**Worst structural problem:** the domain is written entirely from the vantage of the ship as a finished strategic or economic unit — something fielded, insured, or commanded — and has no entry for building one, fishing with one, or being conscripted to crew one; the physical and human base underneath every institutional and doctrinal entry here is silently assumed rather than modeled.

**Entry count:** 29 before. With all five LOAD-BEARING proposals: 34. With VALUABLE also adopted: 37. With OPTIONAL: 39.

**Proposed out of symmetry, not mechanical need:** both OPTIONAL entries (#9 Manila Galleon Trade, #10 Kon-Tiki) are named as such — genuinely good, specific history with no mechanism beyond what `MAR-020`/`MAR-160` and the existing Americas absence case already supply. Of the VALUABLE tier, **Convoy Escort (#8)** is the softest call: it answers Probe 6 only marginally (a modifier variant of formation logic already established by `MAR-210`), and its case for inclusion rests more on filling out the `MAR-290`→`MAR-280`→`MAR-320` doctrinal chain's obvious next chapter than on a genuine structural absence — worth adopting if you want that chain to feel complete, skippable if not.

**Does Maritime justify separate existence?** Yes, more clearly than any other domain audited this cycle. Its region balance is the best of the series, its boundary self-documentation is a model other domains could learn from, its prereq chains are unusually elegant, and its mechanism diversity is real. The gaps found here are not signs of a domain straining against its own premise — they're signs of a domain that got the ship right and forgot everything the ship stands on, floats because of, or is crewed by.
