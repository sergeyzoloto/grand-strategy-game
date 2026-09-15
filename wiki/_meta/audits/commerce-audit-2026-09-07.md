IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

# Commerce Domain Audit

**Date:** 2026-09-07
**Scope:** `domains/commerce/` — 16 entries, COM-140 through COM-130 (COM-001–010 were never assigned; COM-140 was the project's original scaffolding example, not a gap).
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/commerce/`, and cross-checks against `domains/finance/`, `domains/maritime/`, `domains/engineering/`, `domains/migration/`, `domains/law/`, `domains/war/`, `domains/agriculture/`.

## Summary of the pattern

Before the individual probes: Commerce is not randomly incomplete, it is **systematically mid-scene**. Every one of the 16 entries assumes a functioning market, a trusted unit of measure, an enforceable-enough contract, or an already-integrated trade network, and then narrates what happens _to_ that assumed substrate under some shock, innovation, or coercion. Nothing in the domain — or in any domain that touches it — explains how a stranger becomes someone you can trade with at all. The domain also skews almost entirely toward world-system and state-scale institutions (Silk Road, Hanseatic League, chartered companies, railways, tariff wars) with almost nothing at the ordinary city-market or household level, and it models growth and expansion far more readily than contraction or collapse. These three gaps (emergence, scale, reversal) are connected: they are all cases of the domain jumping straight to the dramatic, documented, state-scale event and skipping the boring, underdocumented, universal layer beneath it.

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **FAIL**

The earliest entries are COM-020 (Metallurgy, -3300) and COM-030 (Weights and Measures, -2600). Both presuppose a functioning market already exists: COM-020 assumes ore-to-commodity conversion has a buyer; COM-030's own Mechanism opens with "a buyer and seller can only trust a transaction's stated quantity if both sides _already agree to transact_." Neither entry, nor any entry anywhere in the wiki (checked FIN, LAW, MIG), explains how exchange with a non-kin stranger becomes possible in the first place — no entry on barter-to-market transition, neutral/sacred trading ground, market-day truce, third-party guarantee, or the marketplace as a physical-legal institution. `MIG-070` (Diasporas as Trade Networks) is the closest existing entry and is explicitly _not_ this: its whole mechanism is trust substituted by pre-existing kin/ethnic/religious bonds between network members, which is a solution available only to people who already share an identity — it doesn't touch the harder, more universal problem of trading with someone who shares nothing with you at all. Grepped the full repo for `barter`, `bazaar`, `periodic fair`, `law merchant`, `lex mercatoria`, `market peace`: zero hits. This is the domain's single biggest structural hole.

### 2. SUBSTRATE GAP — **FAIL**

Named absences: no entry on caravanserai/waystation infrastructure, no entry on pack-animal freight unit-economics (cost per ton-mile by camel/mule/cart, distinct from MIG-050/MIG-040's mobility-_threshold_ technology), no entry on land-cargo risk-pooling (the caravan as an institution, distinct from COM-080's route-network-level toll analysis), and — strikingly, given the project's Africa-co-equal mandate — **no entry anywhere on the trans-Saharan gold-salt trade**, one of history's most important long-distance commercial systems and the commercial foundation of the Ghana/Mali/Songhai state sequence. Grepped for `camel` and `caravanserai` across the whole wiki: zero hits. COM-080 covers the Silk Road as a _network_ (tolls, political fragmentation) but never the physical/organizational unit underneath it — how a caravan itself is financed, guarded, and provisioned. Also: COM-230 (Cold Chain) rests entirely on mechanical refrigeration technology that **does not exist as an entry in ENG or anywhere else in the wiki** — the domain's most modern entry has an invisible foundation exactly as much as its oldest one does.

### 3. SCALE GAP — **FAIL**

Sorting the 16 entries: world-system (COM-140, COM-080, COM-160, COM-210, COM-130 — 5), state/quasi-state (COM-150, COM-190, COM-100, COM-200 — 4), city/confederation (COM-110 — 1), firm/workshop (COM-100 partial, COM-180, COM-170 — 2.5). **Zero entries sit at the ordinary local-market or household-retail level.** Every entry is either about a system spanning continents or a state/company-scale institution. There is no peddler, no shopkeeper, no village market day, no small workshop selling directly to neighbors. A player experiencing this domain would see global bullion flows and chartered-company sovereignty long before ever seeing an ordinary transaction — the texture Part A's probe warns about specifically.

### 4. REVERSE-DIRECTION GAP — **FAIL**

Only one entry has contraction as its actual mechanic: COM-180 (Technological Displacement of Craft), and even that is framed as a population's _loss_ rather than a _network's_ collapse. Every other entry either grows (COM-030 standardization spreading, COM-110/016 institutions scaling up, COM-100/023 organizational intensification, COM-200 rail networks expanding) or is a state-imposed policy tool (COM-190 tariffs, COM-210 forced access) rather than a systemic failure. COM-080 (Silk Road) _mentions_ its own eventual displacement by maritime trade in Timing, but the entry's Mechanism and Hook never model route abandonment or network collapse as a mechanic in their own right — decline is narrated, not mechanized. There is no entry anywhere on a documented, catastrophic commercial-network collapse. The most obvious missing case: the Late Bronze Age trade-system collapse (c. 1200 BCE), when the entire eastern Mediterranean's tin/copper/luxury exchange network — interlocking Egypt, the Hittites, Mycenaean Greece, and Cyprus — came apart within a few decades alongside the "Sea Peoples" disruptions. No entry in COM or WAR covers this as a _commercial_ phenomenon. The domain has a clean progress-narrative bias.

### 5. PREREQ ORPHANS — **FAIL**

Four concrete cases, verified against actual IDs and dates in the repo:

- **COM-140** (Silver Shock, starts 1545) has `prereqs: []` despite its entire mechanism depending on the trans-Pacific Manila Galleon route — i.e., on `MAR-160` (Ocean Navigation, 1405–1522, which predates COM-140 and has open `enables: []`). Currently unlinked.
- **COM-020** (Metallurgy, starts **-3300**) lists `prereqs: [WAR-040]` — but WAR-040 (Mass Iron and the End of the Elite Warrior Monopoly) runs **-1200 to -500**, a full 2,100 years _after_ COM-020's own start date. This is a genuine chronological inversion, not a stylistic quibble: COM-020's Bronze Age craft-guild formation cannot depend on an Iron Age event that hasn't happened yet. The dependency almost certainly runs the other way — WAR-040's iron-democratizes-the-battlefield thesis presupposes iron-as-commodity already existing, which is COM-020's own content.
- **COM-210** (Opium Wars) has `prereqs: []` despite its own Regional-specifics section stating the war was fought to preserve a trade "the Bengal opium monopoly directly funded and supplied" — i.e., depending on `COM-150` (Chartered Trading Company, which explicitly covers the East India Company). No naval/military capability entry is linked either, despite the war being decided by naval bombardment.
- **COM-200** (Railways) lists `prereqs: [COM-020]` only, despite being explicitly about _steam_ railways and never citing `ENG-170` (The Steam Engine, 1712–1830, which predates COM-200's 1825 start and already lists `COM-170` in its own `enables`, but not COM-200).

Additionally, **`FIN-070` (Joint-Stock Companies) already lists `see_also: [COM-150]`**, but COM-150 does not reciprocate — a one-directional cross-reference the project's own linking convention (used consistently in every other domain-population pass this session) requires to be symmetric.

### 6. MECHANISM DIVERSITY — **PASS, with one clear gap**

Classifying each entry's Hook: modifier (COM-030, COM-080, COM-160, COM-190, COM-170, COM-130 — 6), gate (COM-110, COM-150, COM-019, COM-100, COM-230 — 5), branch (COM-140, COM-210, COM-200 — 3), event (COM-020's Bessemer half, COM-190's escalation, COM-210 — overlapping with branch), civic (COM-110, COM-150 — 2). This is real diversity — the domain will not play as a flat stat-bonus list, which is the specific failure mode the probe warns against. But **zero entries use a true pool mechanic** (a depleting resource requiring maintenance investment, design principle 4) — every other populated domain in this wiki (HLT's antibiotic pool, AGR's soil fertility, MAR's naval prestige, MIG's confederation cohesion) has at least one. Commerce has none. `route knowledge` (COM-080, COM-200) is the closest candidate and is currently treated as a one-way accumulating stat, not a decaying one — nothing in the domain models a trade route's _physical_ infrastructure silting up, falling out of maintenance, or decaying absent investment.

### 7. BOUNDARY INTEGRITY — **PASS on domain assignment, FAIL on link discipline**

No entry's canonical `domain` is actually wrong under `boundary-rules.md` — COM-020, COM-080, COM-110, COM-019, COM-130 all carry explicit, correctly-reasoned disclaimer sentences pointing the reader to the right domain for the parts they don't own (ENG for smelting chemistry, MAR for wartime sea-lane denial, MAR for maritime smuggling). This is a real strength; whoever wrote these was thinking about the boundary rules while writing, not after.

The one soft case: **COM-140** (Silver Shock) is COM-canonical per the project's own founding precedent (`boundary-rules.md` cites it by name as the template for staying COM-canonical while touching FIN), but a genuinely hostile read of its own Hook and stats (`price level`, `sovereign credit capacity` — both FIN-flavored; only `bullion stock` reads as COM) suggests the balance has tipped further toward FIN than the boundary rule's own example implies. Not recommending a change — the precedent is explicit and deliberate — but naming it, since a rule citing its own only example as justification is worth periodically re-examining.

The concrete failure here isn't domain assignment, it's **missing entry-level links where the domain touch is correctly flagged but never cashed out as an actual `prereqs`/`see_also` ID** — see Probe 5's four cases plus the COM-150↔FIN-070 asymmetry. `domains_touched` is being used correctly as a category flag but not consistently followed through into the specific cross-reference the schema's own linking convention calls for.

---

## PART B — Proposed Additions

### LOAD-BEARING (5)

**1. The Marketplace and the Stranger**

- Regions: middle-east, south-asia, east-asia, africa — era precision, roughly -3500 to -1000 (multiple independent origins)
- Answers: Probe 1 (emergence gap) — directly
- Mechanism: converts exchange from something possible only between kin/community members into something possible between two people who share nothing — via neutral/sacred ground (temple markets), oath-bound market truces, or a third-party guarantor (state or temple) who absorbs default risk in exchange for a cut. This is the precondition every other COM entry silently assumes.
- Hook (seed): unlocks "trade with an unrelated stranger" as a capability at all — before this exists, exchange is bounded by kinship/community reach (MIG-070's mechanism); after, a market's size is bounded only by how far the trust-guarantee institution's reach and credibility extend.
- Prereqs: none (foundational). Enables: COM-030, COM-020, and effectively everything downstream.
- Contested: true — whether market exchange gradually differentiated out of embedded gift-reciprocity economies (the Polanyi "embeddedness" thesis) or coexisted as a separate "silent trade"/stranger-trade sphere alongside kin exchange from very early on is a live, unresolved debate in economic anthropology. Model both as available, region-dependent starting configurations rather than one universal sequence.
- Confidence: medium — the underlying anthropological literature is real but my grasp of its current consensus (or lack of one) is not expert-level.

**2. The Caravan as a Risk-Pooling Institution**

- Regions: africa (trans-Saharan gold-salt trade — flagship case), middle-east, central-asia-steppe, south-asia — era, roughly -1000 to 1850 (superseded by COM-200)
- Answers: Probe 2 (substrate gap) and Probe 3 (scale gap — this is the missing intermediate-scale entry)
- Mechanism: individual merchants can't afford the guards, pack animals, and provisioning a long desert or steppe crossing requires alone; pooling into one large guarded caravan converts an individually unbearable fixed cost and catastrophic risk into a shared, survivable one — a genuinely different mechanism from COM-080's network-of-tolls analysis, which assumes the caravan unit already exists and asks only how much friction it meets along the way.
- Hook (seed): caravan formation is a pooling gate — below a minimum number of participating merchants/capital, the route isn't viable at any price; above it, marginal merchants can join cheaply. Route viability is a threshold function of aggregate participation, not a smooth cost curve.
- Prereqs: MIG-050 (Horse Husbandry) or an equivalent pack-animal domestication entry, region-dependent. Enables: COM-080.
- Contested: false.
- Confidence: medium — the trans-Saharan case specifically is well-documented in the historical literature I'm drawing on, but my confidence in the finer organizational details (exact guard-to-merchant ratios, financing structures) is lower than for the Old World caravan cases.

**3. The Periodic Fair and the Law Merchant**

- Regions: europe (Champagne fairs), east-asia, middle-east — century precision, roughly 1100–1600
- Answers: Probe 1 (emergence, later/institutional end) and Probe 6 (civic mechanism, currently underused relative to gate/modifier)
- Mechanism: a temporary, legally-protected, periodically-recurring gathering lets merchants from many otherwise-unconnected regions converge to set prices and clear credit at once, policed by an on-the-spot merchant court (the historical "law merchant" / pie-powder courts) rather than the host territory's ordinary legal system — a genuinely different institutional form from both COM-110's permanent city confederation and MIG-070's kin-based network.
- Hook (seed): a fair is a scheduled, temporary civic institution — it converts scattered bilateral bargaining into periodic multilateral price discovery, and the merchant-court jurisdiction it carries is a distinct, non-state form of `contract enforcement` (reuse existing stat) that operates only while the fair is in session.
- Prereqs: COM-030 (needs a shared standard to be useful across attendees). See_also: COM-110, LAW-120.
- Contested: false.
- Confidence: medium.

**4. The Late Bronze Age Trade Collapse**

- Regions: mediterranean, middle-east — decade precision, roughly -1200 to -1150
- Answers: Probe 4 (reverse-direction gap) — directly, and the strongest available case
- Mechanism: a highly specialized, interdependent exchange network (bronze production requires tin from a handful of distant sources, funneled through a small number of palace-controlled redistribution economies) is _fragile_ in direct proportion to its specialization — a small number of broken links (raiding, a couple of palace centers falling) can cascade into total network failure within a generation, in a way a more redundant, diversified trade system would not. This is the mirror-image mechanic to COM-080's Pax-Mongolica-style fragmentation-vs-unification logic, but modeling collapse as an emergent property of specialization itself rather than of political fragmentation along a route.
- Hook (seed): network specialization is a double-edged modifier — it raises peak throughput and value while raising collapse risk given a shock of a given size; the same variable cuts both ways rather than being a pure bonus.
- Prereqs: COM-020 (bronze-commodity trade must already exist to collapse). See_also: WAR (for the contemporaneous "Sea Peoples" disruptions, if/when a WAR entry covers them).
- Contested: true — whether the collapse is best explained by systems-collapse dynamics (Eric Cline's "1177 BC" synthesis), climate-driven drought and famine, invasion, or some combination is a genuinely live, multi-causal debate in Bronze Age archaeology. Model as several independently-triggerable stressors (drought, raiding, elite-network disruption) whose combination — not any single one — produces cascade failure, echoing the multi-cause structure COM-140 already uses for the Price Revolution.
- Confidence: medium.

**5. Route Infrastructure Decay and Maintenance**

- Regions: east-asia (Grand Canal siltation), mediterranean (Roman road network post-collapse), middle-east (qanat maintenance lapse) — era precision, recurring
- Answers: Probe 2 (substrate) and Probe 6 (the missing pool mechanic) simultaneously
- Mechanism: physical trade infrastructure (canals, roads, caravanserai, harbor works) silts up, washes out, or falls out of repair by default, not just under attack — requiring standing maintenance investment to hold its carrying capacity rather than only construction investment to create it once. This is design principle 4's depleting-pool logic, currently entirely absent from Commerce's stat vocabulary.
- Hook (seed): a `route capacity` pool decays by a standing rate absent maintenance spend, independent of any shock — the first genuine pool mechanic in the domain, distinct from `route knowledge`'s currently one-way-accumulating treatment.
- Prereqs: COM-080, AGR-100 (Irrigation, for the canal-siltation case specifically). See_also: AGR-150 (Salinization, a structurally similar depleting-pool case in a different domain).
- Contested: false.
- Confidence: medium-high for the mechanism generally, medium for the specific comparative maintenance-investment figures across the three regional cases.

### VALUABLE (4)

**6. The Putting-Out System**
Regions: europe, south-asia. Era, roughly 1400–1800. Answers Probe 3 (household-scale, the one genuinely missing production-organization mode between independent craft and the manufactory). Mechanism: a merchant-capitalist supplies raw material and collects finished goods from many dispersed rural households, financing and coordinating production without concentrating it under one roof the way COM-100's manufactory does — a distinct organizational form, not a weaker manufactory. Hook: converts household labor into wage-adjacent piece-work without requiring the worker to relocate, a different labor-supply mechanic from both craft independence and factory concentration. Prereqs: none needed beyond existing market infrastructure; see_also COM-100. Contested: false. Confidence: high.

**7. Trademark, Brand, and Trust at a Distance**
Regions: europe, americas, east-asia. Century, roughly 1860–1920. Answers Probe 1's later bookend — once COM-170's factories make goods anonymous and mass-produced, the marketplace trust problem from entry #1 recurs at industrial scale and needs a new solution (legally protected brand identity, advertising) rather than the ancient one. Hook: brand recognition functions as a portable, transferable trust-substitute a producer can build once and spend repeatedly, distinct from both the Marketplace entry's transaction-bound trust and MIG-070's kin-bound trust. Prereqs: COM-170. Contested: false. Confidence: medium.

**8. The Consumer Boycott and Non-Importation** [REJECTED]
Regions: americas, south-asia, east-asia. Era, roughly 1765–1930. Answers Probe 6 (a genuinely bottom-up civic/event mechanism, distinct from COM-190's top-down state tariff policy). Mechanism: organized, voluntary collective refusal to buy a specific good or supplier's goods, requiring no state capacity at all — a commercial weapon available to actors with zero formal sovereignty. Hook: a boycott's effectiveness is a function of collective-action coordination capacity, not state power — the inverse resource-requirement from COM-190's tariff mechanic. Prereqs: none required; see_also COM-190, DIP-160 (nationalism). Contested: false. Confidence: medium.

**9. Retail Trade and the Urban Shop**
Regions: middle-east, europe, east-asia. Era, broad. Answers Probe 3 directly (the plainly missing local/household consumption-side entry). Mechanism: a fixed-location shop selling directly to a walk-in local consumer, distinct from both the periodic fair (entry #3, temporary/wholesale) and manufactory/factory (production side) — the retail layer most premodern commercial activity actually ran through day to day. Hook: a modifier on local market depth/reach rather than a gate. Confidence: medium — this is the entry in the load-bearing-adjacent zone I'm least sure earns core weight; see Part D.

### OPTIONAL (2)

**10. The Department Store** [REJECTED]
Flavor entry on a specific 19th-century retail institutional form (Bon Marché, etc.) — real history, but its mechanic is a thin restatement of entry #9 (Retail) at a larger scale plus COM-170's factory-supply logic. Honest flag: proposing this mostly for period-flavor completeness, not because it adds a mechanism nothing else provides.

**11. Company Towns and Scrip** [REJECTED]
Flavor entry on employer-issued currency and company-store monopoly in industrial-era mining/mill towns. The scrip-as-currency angle genuinely belongs to FIN, not COM, and the company-store monopoly angle is a narrow special case of COM-150's captive-market logic rather than a new mechanism. Weak standalone case; naming it mainly because it's the kind of vivid, specific history this project favors, not because the domain structurally needs it.

---

## PART C — Proposed Modifications

### SPLIT

**COM-020 (Metallurgy as Craft and Commodity — Bronze to Steel)** spans -3300 to 1870 and its single Mechanism section does two genuinely unrelated jobs stitched together with "the Bessemer process breaks this pattern": (a) a slow, ancient, social-status-conferring craft-guild-formation story, and (b) a single 1856 industrial process innovation collapsing a specific metal's price by an order of magnitude overnight. These are different mechanism _types_ (a gradual social-organizational claim vs. a sudden price-collapse event) as well as different eras. Split into:

- **COM-020 retained**: craft-guild metalworking as commodity trade, -3300 to roughly 1500 — keep prereqs/social-consequence content, drop the Bessemer material.
- **A new ENG entry** (outside this audit's numbering authority, flagged for the engineering domain, not proposed here as a COM ID): the Bessemer process itself as _technique_ — decarburization mechanization, the actual engineering innovation — which COM-020's commercial consequence (bulk steel commodification) would then cite as a prereq rather than narrate directly. This also resolves the WAR-040 chronology problem below, since a trimmed COM-020 ending around 1500 no longer needs WAR-040 (-1200 to -500) as a forward-pointing prereq at all — if anything, the relationship should run COM-020 → WAR-040 (iron-as-commodity existing enables the elite-monopoly-breaking consequence), not the reverse currently on file.

### MERGE

None found. Adversarially re-reading all 16 for a genuine one-mechanism-between-two-entries case turned up nothing — if anything the domain's problem runs the other direction (COM-020 is over-fused, see SPLIT above). Reporting zero merges honestly rather than manufacturing one.

### RE-DOMAIN

No entry's canonical `domain` needs to change. What needs fixing is entry-level link discipline, not domain reassignment:

- **COM-150 ↔ FIN-070** (Joint-Stock Companies): FIN-070 already lists `see_also: [COM-150]`; COM-150 does not reciprocate. Add FIN-070 to COM-150's `see_also`.
- **COM-140**: add `MAR-160` to `prereqs`.
- **COM-020**: remove `WAR-040` from `prereqs` (chronologically backwards — see SPLIT); consider adding COM-020 to WAR-040's own `prereqs` instead once the split lands.
- **COM-210**: add `COM-150` to `prereqs` (the Bengal opium monopoly this war defends is COM-150's East India Company).
- **COM-200**: add `ENG-170` (Steam Engine) to `prereqs`, and add COM-200 to ENG-170's `enables` reciprocally.

### RETIER

All 16 entries currently carry `tier: core` — zero `standard`, zero `stub`. A field where nothing is ever anything but the top value carries no editorial signal. Two candidates for downgrade to `standard`:

- **COM-130** (Sea Chokepoint Races) — a real, distinct mechanic (relative geographic value, creatable/destroyable) but a narrower, more specific application than the domain's true structural pillars; nothing else in COM depends on it (`enables: []`).
- **COM-230** (Cold Chain) — similarly self-contained (`enables: []`), a clean and genuinely distinct all-or-nothing gate mechanic, but load-bearing for nothing downstream in the domain as currently written.

Not proposing to touch the other 14 — their `enables` lists show real downstream dependency, which is a more honest core/standard signal than my own judgment of "importance."

### CUT

**COM-019 (Smuggling and Gray Markets)** — its own Mechanism section states its core logic _is_ MAR-150's, transposed from sea to land ("the same evasion incentive MAR-150 describes... but along a land border"). The one genuinely novel content it carries — subsistence-good tax evasion escalating into organized political resistance (the Salt March) — doesn't require a standalone entry to preserve; it reads more naturally as a region_variant or Regional-specifics elaboration on COM-190 (Protectionism and Tariff Wars, which COM-019 already lists as its sole prereq) or as an added region_variant on MAR-150 itself. Recommend cutting COM-019 as a standalone ID and folding the Salt March case into COM-190.

This is the only entry I'd cut outright. I looked hard for a second candidate and didn't find one I could defend with a straight face — the rest of the domain's problem is gaps and mis-linked prereqs, not padding.

---

## PART D — Verdict

**Worst structural problem:** Commerce has no origin story — every entry, from the oldest (-3300) to the newest (1950), assumes a functioning market, a trustworthy stranger, and an enforceable-enough contract already exist, and narrates only what happens to that assumed substrate afterward; this is compounded by four broken or missing prereq chains (COM-140, COM-020, COM-210, COM-200) that leave even the entries that do exist under-anchored to each other.

**Entry count:** 16 before. If every LOAD-BEARING proposal is adopted and the one CUT is taken: 16 − 1 + 5 = **20**. With VALUABLE also adopted: 24. With OPTIONAL: 26. I'd treat 20 as the number that actually fixes what's structurally broken; the rest is genuinely optional, not disguised necessity.

**Proposed out of symmetry, not mechanical need:** entries #9 (Retail Trade), #10 (Department Store), and #11 (Company Towns). #9 is defensible on the scale-gap probe alone but I'm not fully convinced it earns LOAD-BEARING weight rather than VALUABLE — I placed it in VALUABLE for that reason. #10 and #11 (OPTIONAL tier) I'm naming honestly as flavor-for-completeness: good history, and I'd write them if asked, but the domain is not structurally incoherent without them the way it is without the Marketplace entry.

**Does Commerce justify separate existence?** Yes, clearly — the COM/FIN split is load-bearing across the entire wiki (dozens of other domains' entries cross-reference it correctly), and once the emergence gap is closed, the mechanism diversity within Commerce (gate, branch, civic, and now pool) is real and distinct from FIN's value-across-time territory, MAR's sea-theatre carve-out, and AGR's production-side economics. The domain's problem isn't that it shouldn't exist — it's that it currently reads as sixteen scenes from the middle of a story whose first act was never written.
