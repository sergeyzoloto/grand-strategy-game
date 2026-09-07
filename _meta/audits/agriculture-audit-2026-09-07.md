# Agriculture Domain Audit

**Date:** 2026-09-07
**Scope:** `domains/agriculture/` — 22 entries, AGR-001 through AGR-022.
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/agriculture/`, and cross-checks against `domains/commerce/`, `domains/engineering/`, `domains/health/`, `domains/law/`, `domains/migration/`, `domains/war/`.

## Summary of the pattern

Agriculture is the best-constructed domain audited so far by several measures — it already has genuine pool, branch, gate, event, and civic mechanics (not just modifiers), its region balance is the best in the project (15.0% Europe+Mediterranean), and 15 of 22 entries are honestly contested. But it has one specific, repeating tell: **the entries' own prose keeps announcing a hard dependency — "jointly necessary," "the total labor coercion X supplies," "(MAR)" — and then failing to encode that dependency as a `prereqs` entry**, leaving it in `see_also`, in a bare unlinked domain-code parenthetical, or not linked at all. This happens five separate times, on five unrelated cross-domain relationships, which makes it a pattern rather than a coincidence — the writing process was tracking these dependencies faithfully in the argument and then not carrying them through to the frontmatter. Separately, the domain sets up two payoffs it never delivers: AGR-004 explicitly names an unresolved question and answers it nowhere, and AGR-018 builds a "genuine agricultural windfall" case for the potato without ever reaching the windfall's most famous catastrophic reversal.

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **PARTIAL FAIL**

The domain's actual origin chain (AGR-004 Holocene Bargain → AGR-001 Founder Crop Domestication) is genuinely good — better than most domains audited, since AGR-004 explicitly addresses the climate-precondition question a lesser version of this domain would have skipped entirely. But AGR-004's own Contested section names a question and never answers it: "the region-specific question of why some Holocene populations respond to that removed constraint with domestication and others don't [stays] a separate, unresolved variable." No entry anywhere in the 22 picks this up. The two live candidate explanations in the actual archaeological literature — Boserup's population-pressure-forces-intensification thesis and Carneiro's circumscription theory (geographically bounded populations can't disperse instead of intensifying) — are absent. The domain correctly identifies its own missing piece and then never delivers it.

### 2. SUBSTRATE GAP — **FAIL**

Three concrete absences, checked against the full wiki (not just AGR):

- **Manure and night-soil nutrient cycling** — the single most universal pre-industrial soil-fertility technique globally — appears only as a comparative aside inside AGR-016's absence note ("Chinese agriculture had sustained intensive crop rotation and organic-fertilization... manure, night-soil, green manure"). It has never been its own entry despite being the technique AGR-016 is explicitly contrasted against, and despite `soil fertility` already being one of the domain's two most-reused stats.
- **Livestock containment** (fence, hedge, corral, fold) — grepped the whole wiki: only passing mentions in AGR-014 and AGR-020's prose, never a mechanic of its own. This is a real gap with a real downstream consequence: AGR-016's Bakewell selective-breeding program requires _controlled mating_, which requires containment infrastructure that, as written, doesn't exist anywhere in the wiki for that entry to prereq.
- **Grain milling** (quern, watermill, windmill) — grepped the whole wiki for `mill|quern|millstone`: zero hits for grain-processing technology anywhere in AGR _or_ ENG. Every domesticated cereal in this record gets from harvested grain to usable flour by a mechanism the wiki never names.

### 3. SCALE GAP — **MOSTLY PASS**

Sorting the 22: world-system/climate (AGR-004, AGR-018, AGR-019), regional-system (AGR-005, AGR-008, AGR-011, AGR-013, AGR-017, AGR-022), state/imperial (AGR-012), class/institutional (AGR-015, AGR-020), household/farm (AGR-002, AGR-007, AGR-009, AGR-014, AGR-016, AGR-021). Unlike Commerce's total absence of household-level texture, Agriculture already has real household presence — AGR-007's gendered field-labor division and AGR-021's per-farm labor economics both operate at exactly that scale. Two genuine gaps remain, both checked against the full wiki with zero hits: **crop diversification/polyculture as a household risk-management choice** (the Andean vertical archipelago, Three Sisters companion planting — the direct mechanical mirror-image of AGR-017's monoculture and the famine risk it creates), and **sharecropping** as a tenure form distinct from AGR-012's hereditary colonate, AGR-015's serfdom, and AGR-020's enclosure-consolidation — a landlord-tenant _risk-split_ arrangement none of the three existing tenure entries actually model.

### 4. REVERSE-DIRECTION GAP — **MOSTLY PASS, ONE GLARING HOLE**

Unlike Commerce, Agriculture already has real collapse/reversal mechanics: AGR-010 (Salinization) is an explicit depleting pool, AGR-018's American side is explicitly framed as land-abandonment collapse, and AGR-019 (Little Ice Age) models yield-ceiling contraction directly. This is a genuine strength. But grepping the whole wiki for `famine`, `blight`, `phytophthora`, `potato famine` returns **zero hits anywhere in the entire project**. The Irish Potato Famine — arguably the single most consequential crop-failure event in the timeframe this wiki covers, killing roughly a million people and displacing a million or two more — is completely absent, despite AGR-018 building the potato's "genuine agricultural windfall" case in loving detail and never once mentioning that the same genetic uniformity behind that windfall is exactly what made the 1840s blight catastrophic rather than merely bad. This is the domain's single sharpest missing entry.

### 5. PREREQ ORPHANS — **FAIL**

Five concrete cases, all following the same pattern — a dependency stated explicitly in the entry's own prose, never promoted to `prereqs`:

- **AGR-009** (Iron Farm Tools, prereqs `[AGR-001]` only) — its own Mechanism says "iron, once smelting technique matures, is cheaper to produce," but the smelting-maturity entry (`COM-013`, Metallurgy as Craft and Commodity) sits only in `see_also`, never `prereqs`.
- **AGR-012** (Roman Grain Order) — the body text literally writes the unlinked parenthetical "shipped across the Mediterranean (MAR)" — a bare domain-code reference, not a real entry ID, pointing at nothing a reader or a build script could resolve.
- **AGR-017** (Plantation Complex, prereqs `[]`) — its own Hook states the entry runs "entirely on the total labor coercion MIG-013's forced-migration mechanic supplies," yet MIG-013 sits only in `see_also`. An entry whose own Hook names a hard dependency and lists no prereqs at all is the cleanest possible case of this pattern.
- **AGR-021** (Mechanisation) — the Mechanism section names "the gasoline tractor" as the entry's central technology; the internal combustion engine (`ENG-020`) is absent from both `prereqs` and `see_also`.
- **AGR-022** (Green Revolution) — the Mechanism section states the breeding innovation and fertilizer input "are jointly necessary" — the strongest possible dependency language in the entire domain — yet `ENG-012` sits only in `see_also`.

### 6. MECHANISM DIVERSITY — **PASS**

This is the domain's strongest probe result. Genuine pools: AGR-010 (soil fertility as an explicit depleting pool, design principle 4), AGR-017 (monoculture soil-fertility drawdown). Genuine branches: AGR-005 (control centralizes on prior institutions), AGR-008 (sustainable vs. frontier-consuming, population-pressure-gated), AGR-014 (coexistence vs. conflict), AGR-015 (identical shock, opposite institutional fork — the cleanest design-principle-1 case in the domain), AGR-022 (aggregate gain vs. distributional consequence tracked separately). Genuine gates: AGR-004 (unlocks the _option_ of domestication), AGR-006 (unlocks accounting technology), AGR-016 (unlocks breeding-as-program). Genuine events: AGR-012, AGR-018, AGR-019, AGR-020. Only about six of twenty-two (AGR-003, AGR-009, AGR-011, AGR-013, AGR-021, and half of AGR-002) lean on a close-to-pure modifier with no gate/branch/event/pool/civic texture — comfortably below the "nearly all modifier" failure threshold the probe warns about.

### 7. BOUNDARY INTEGRITY — **MOSTLY PASS, ONE REAL VIOLATION**

Most entries carry the same explicit, well-reasoned disclaimer sentences seen in Commerce's best cases — AGR-005 versus ENG-001, AGR-009 versus COM-013, AGR-014 versus WAR-006/MIG-007, AGR-017 versus MIG-013/COM-017 all correctly name what they don't own and point at what does. The one real violation: **AGR-021 (Mechanisation) never disclaims ENG's artifact territory at all** — unlike AGR-005, which explicitly separates "irrigation's agronomic and social consequence" (AGR's) from "the construction technique itself" (ENG-001's), AGR-021 just narrates the tractor and combine harvester directly as if they were AGR's own artifacts, with no ENG entry cited anywhere in the file. This is the same underlying gap as the AGR-021/ENG-020 prereq orphan above, but it's also a boundary-discipline lapse, not just a missing link. Separately, AGR-021's own _title_ — "Mechanisation and the Cold Chain" — still promises content (the cold chain) the entry's own opening paragraph explicitly disclaims to COM-025 in full. The title is a leftover from the original bundled topic list and now actively misleads a reader browsing titles alone.

---

## PART B — Proposed Additions

### LOAD-BEARING (4)

**1. Manure and Night-Soil Nutrient Cycling**

- Regions: east-asia (China's night-soil system, most thoroughly documented), south-asia, europe (open-field manuring) — era, roughly -6000 to 1800
- Answers: Probe 2 (substrate) directly
- Mechanism: converts a waste product (animal manure, human night-soil) into a labor-intensive nutrient-cycling input that restores `soil fertility` without requiring land to lie fallow — the universal, labor-driven predecessor and real-world alternative to AGR-016's later land-rotation-driven British solution to the identical nitrogen-limitation problem.
- Hook (seed): a `soil fertility` restoration path gated on labor availability rather than land availability — a region with abundant labor and scarce land (China) and a region with abundant land and scarce labor (much of medieval Europe before AGR-016) reach comparable soil-fertility outcomes through opposite input trade-offs.
- Prereqs: AGR-001, AGR-002 (needs both crops and livestock/human waste stream). Enables: AGR-016 (as the technique it's explicitly compared against).
- Contested: true — how much manure/night-soil cycling alone versus fallow-and-rotation explains sustained high pre-industrial yields in labor-abundant East Asian agriculture specifically is debated among agricultural historians (this is the flip side of AGR-016's own already-contested "British Agricultural Revolution" framing). Model labor-intensity and land-intensity as two independently viable routes to the same soil-fertility outcome, echoing AGR-002's dairying/fermentation "two independent unlock paths" pattern.
- Confidence: high.

**2. Livestock Containment: Fence, Fold, and Corral**

- Regions: middle-east, europe, east-asia — era, roughly -8000 to 1800
- Answers: Probe 2 (substrate) and closes a real gap in AGR-016's own prerequisite chain
- Mechanism: converts free-ranging herding (requiring constant herder labor to prevent crop damage, theft, and predation, and making mating essentially random) into a fixed-infrastructure investment trading upfront capital for reduced ongoing labor — and, critically, for the _controlled mating_ any deliberate selective-breeding program requires.
- Hook (seed): a gate, not a modifier — AGR-016's Bakewell-style programmatic breeding is mechanically impossible without this infrastructure existing first, and containment also directly reduces the land-use friction AGR-014's pastoralist-cultivator conflict cycle describes.
- Prereqs: AGR-002. Enables: AGR-016 (reciprocal fix to a currently-missing dependency), see_also AGR-014, AGR-020 (distinct from AGR-020's legal/tenure enclosure — this entry owns the physical containment technology, not the land-tenure transformation built partly on top of it).
- Contested: false.
- Confidence: medium — the general mechanism is solid; comparative regional dating specifics are thinner in my own knowledge than for the other load-bearing proposals.

**3. Monoculture Dependency and Famine (the Irish Potato Famine)**

- Regions: europe (Ireland) — decade precision, 1845–1852
- Answers: Probe 4 (reverse-direction gap) directly, and completes AGR-018's own unfinished argument
- Mechanism: a population that shifts dependence onto one high-yield but genetically uniform crop variety gains `yield ceiling` in ordinary years at the cost of catastrophic, fully-correlated failure risk once a pathogen specifically suited to that variety appears — the same uniformity that produces the yield gain removes the diversification that would have contained a single pathogen's damage to one crop among several.
- Hook (seed): a genetic-uniformity risk multiplier layered on top of AGR-018's yield-ceiling gain, resolving as a branch on prior political and market institutions (design principle 1) rather than a fixed death toll — Ireland's food exports continuing during the famine under the prevailing administration is a documented, separate variable from the blight itself.
- Prereqs: AGR-018 (potato adoption). See_also: AGR-020 (Ireland's land-tenure structure at the time of the famine).
- Contested: true — how much of the roughly one million deaths and one-to-two million emigrants should be attributed to the blight itself versus contemporary political and economic policy (continued food exports, the character of relief efforts) is a genuine, long-running historiographical dispute, sharply enough contested that "Famine" versus "Great Hunger" is itself a contested naming choice in the literature. Model the blight as the triggering shock and political response as an independently-weighted multiplier on the resulting death toll, not a single scripted outcome.
- Confidence: high on the basic event, medium on the precise causal weighting in the contested debate.

**4. Population Pressure and Agricultural Intensification**

- Regions: global/east-asia/south-asia — era, framed as a standing theoretical entry rather than one dated event
- Answers: Probe 1 (emergence gap) directly — closes the exact question AGR-004 names and leaves open
- Mechanism: Ester Boserup's population-pressure thesis inverts the naive causal arrow this domain otherwise assumes by default (more food → more population): population growth itself forces agricultural intensification (shorter fallow, more labor per unit land, AGR-008's swidden cycle compression under population pressure) rather than waiting for intensification to happen first and population to follow.
- Hook (seed): a branch on which causal direction binds first — Malthusian (yield ceiling caps population) versus Boserupian (population pressure forces yield-ceiling-raising intensification) — with AGR-008's swidden fallow-cycle mechanic already implicitly running on the Boserupian reading without ever citing the theory that explains why.
- Prereqs: AGR-004. See_also: AGR-008, AGR-001.
- Contested: true — this _is_ the contested debate (Malthus versus Boserup remains genuinely live in agricultural economics and historical demography), which is the entry's whole point rather than an incidental complication.
- Confidence: medium — the theoretical framework is well-established; my confidence in exactly how the empirical evidence currently weighs between the two readings is lower than for the other load-bearing proposals.

### VALUABLE (3)

**5. Grain Processing and the Mill**
Regions: middle-east (early querns), east-asia, europe (watermills — the Domesday Book's roughly 6,000 English mills is a good concrete anchor). Era, roughly -8000 to 1200. Answers Probe 2 (substrate — grain-to-flour conversion is completely absent from both AGR and ENG). Mechanism: hand-grinding grain into flour by quern-stone is extraordinarily labor-intensive and falls overwhelmingly on women's labor historically; water- and wind-powered milling mechanizes this specific bottleneck, freeing that labor for other work — a processing-stage habitus mechanic parallel to AGR-007's field-preparation-stage gendered-labor mechanic. Note for the record: the mill's own _construction_ technique arguably belongs to ENG once ENG has an entry for it, which it currently doesn't — this entry would need to either wait for that ENG entry or explicitly own the technique itself the way AGR-005 does not for irrigation. Hook: modifier, converting flour-production labor cost sharply downward. Prereqs: AGR-001. Contested: false. Confidence: medium.

**6. Sharecropping as a Tenure Form** [REJECTED]
Regions: americas (post-Civil-War US South), south-asia. Era, roughly 1865–1950. Answers Probe 3 (scale gap) — the one tenant-labor risk-allocation model AGR-012 (hereditary colonate), AGR-015 (serfdom), and AGR-020 (enclosure/consolidation) don't cover. Mechanism: converts a capital-constrained tenant relationship into a yield-risk-sharing arrangement (a crop-split rather than fixed rent) — landlord and tenant share a bad harvest's cost together, a different risk allocation from both a fixed-rent tenancy (tenant bears all yield risk) and wage labor (landlord bears all yield risk). Hook: branch — the same underlying capital constraint resolves into fixed rent, crop-share, or wage labor depending on which party is better positioned to bear yield risk. Contested: true — whether sharecropping in the post-bellum American South functioned primarily as continued labor coercion under a different name or as a genuine risk-sharing accommodation both parties preferred to the available alternatives is a live, substantial historiographical dispute. Confidence: medium.

**7. Crop Diversification and Polyculture** [REJECTED]
Regions: americas (Three Sisters companion planting, Andean vertical-archipelago multi-altitude cropping), africa. Era, broad/recurring. Answers Probe 3 (scale gap, household risk-management decision) and mirrors AGR-017's monoculture as a direct mechanical contrast. Mechanism: planting multiple ecologically distinct crops together spreads failure risk (a pathogen or weather event that devastates one crop leaves others intact) at the cost of lower peak yield-per-crop relative to specialized monoculture — the household-scale risk/yield trade-off the new Famine entry's catastrophic-tail-risk case makes vivid. Hook: modifier/branch, trading peak `yield ceiling` for reduced catastrophic-failure variance — the mirror-image choice to AGR-017's plantation monoculture and the new Famine entry's failure mode. Contested: false. Confidence: medium.

### OPTIONAL (2)

**8. Terraced Agriculture** [REJECTED]
Flavor entry on Andean and Southeast Asian (Ifugao) rice/potato terracing — real, visually striking history, but the actual mechanic (raising `yield ceiling` via engineered land modification on steep terrain) substantially overlaps with AGR-005's irrigation/water-control logic already. Honest flag: this is a specific, dramatic application of a mechanism the domain already has, not a new one.

**9. Viticulture and Perennial-Crop Economics** [REJECTED]
Flavor entry on vineyards and olive groves as multi-year capital investments distinct from annual grain cycles, directly picking up AGR-012's own Italy-shifts-to-wine-and-olive absence note. The underlying mechanic (a perennial crop's multi-year maturation horizon versus an annual crop's yearly reset) is real and genuinely distinct, but the domain isn't structurally incoherent without it the way it is without the Famine or Population Pressure entries — naming it mainly because AGR-012 already gestures at exactly this case without following through.

---

## PART C — Proposed Modifications

### SPLIT

**AGR-012 (The Roman Grain Order and the Colonate)** bundles two mechanisms its own text calls "related but distinct": the annona grain-export-quota logistics system, and the colonate's hereditary tenant-land-binding. The entry's own Mechanism section uses almost exactly the "meanwhile, in..." construction the probe describes ("The colonate's later tenant-binding responds to a related but distinct problem..."). Split into:

- **AGR-012 retained**: the grain order / annona — provisioning logistics, Egypt's export quota, Italy's specialization response. Narrower resulting scope; see RETIER below.
- **The colonate half**: rather than spinning up a new standalone ID, fold this directly into **LAW-005 (Feudalism and Vassalage)** as a new region_variant — Rome's late-imperial hereditary tenant-binding as a documented institutional precursor to the medieval land-for-service bond LAW-005 already covers across Europe, Japan, and Ethiopia. LAW-005's own definition ("internal enforceable obligation... within a polity") fits the colonate better than agriculture's own territory does, and LAW-005 already has the comparative-institution structure this content would slot into cleanly.

**AGR-016 (Fodder Crops, Legume Rotation, and Selective Breeding)** bundles two mechanisms that, unlike AGR-013's three mutually-reinforcing plow/collar/rotation technologies, don't actually interact with each other at all: legume rotation is a soil-chemistry mechanic, Bakewell's selective breeding is a livestock-genetics mechanic. The entry's own Hook papers over this with a thin unifying frame ("both applying deliberate, repeatable selection logic... to already-established domesticates") that's weaker than the specific content underneath it. Split into:

- **AGR-016 retained**: fodder crops and legume rotation, soil fertility only.
- **A new entry**: Bakewell-style selective livestock breeding as its own mechanism, prereq'd on the new Livestock Containment entry (#2 above) rather than tucked inside a soil-fertility entry it has no chemical relationship to.

### MERGE

None found within the domain proper. Considered and rejected AGR-008/AGR-009 (swidden soil-cycling and iron-tool cost-ceiling are genuinely different levers that happen to share a frontier-expansion theme, not one mechanism) and AGR-015/AGR-019 (both are shock-then-institutional-divergence entries, but two and a half centuries and unrelated triggering mechanisms apart). If anything the domain's problem runs the other direction — see SPLIT above.

### RE-DOMAIN

- **AGR-012's colonate content** → LAW-005, per SPLIT.
- **AGR-018 (Columbian Exchange and Collapse)** — `domains_touched` is currently `[migration, health]` with no maritime touch at all, despite the entire premise depending on transatlantic shipping capability. Add `maritime` to `domains_touched` and add `MAR-008` (Ocean Navigation, 1405–1522, which predates and directly enables AGR-018's 1492 start) to `prereqs`.
- **AGR-009 → COM-013** promoted from `see_also` to `prereqs`.
- **AGR-017 → MIG-013** promoted from `see_also` to `prereqs`.
- **AGR-021 → ENG-020** (Internal Combustion Engine) added to `prereqs`, currently absent entirely.
- **AGR-022 → ENG-012** promoted from `see_also` to `prereqs`.
- **AGR-012's bare "(MAR)" parenthetical** — replace with a real ID, most likely `MAR-001` (Coastal Trade) or a comparable Mediterranean-shipping entry, added to `prereqs`.

### RETIER

21 of 22 entries currently carry `tier: core`; only AGR-003 is `standard`. Three further candidates for downgrade:

- **AGR-011** (Wet-Rice Intensification / Champa Rice) — a real, well-scoped, single-region mechanism with `enables: []`; nothing in the domain is structurally built on top of it.
- **AGR-012** (Roman Grain Order, post-SPLIT) — once trimmed to the annona logistics content alone, a narrower entry than its current scope suggests.
- **AGR-019** (Little Ice Age and the General Crisis) — a strong, well-evidenced entry, but `enables: []` and nothing downstream depends on it specifically; its weight is in the quality of its own Contested section, not in what it unlocks.

Not touching the rest — most of the remaining `enables: []` entries (AGR-018, AGR-020, AGR-021, AGR-022) sit near the end of the domain's covered timeline, where having nothing built on top of them is expected rather than a sign of thinness.

### CUT

**AGR-012's colonate half** is the domain's one real cut — not because it lacks a mechanic (it has a perfectly good one), but because that mechanic's canonical home is LAW-005, not a second, separately-numbered AGR ID. This is a cut-and-fold rather than a cut-and-delete: no historical content is lost, but the domain loses one entry's worth of scope it was carrying past its own boundary. I looked hard for a second, genuine no-mechanic-or-duplicate-mechanic cut elsewhere in the 22 and didn't find one I could defend — this domain was written tightly enough that its problems are gaps and mislinked prereqs, not padding.

---

## PART D — Verdict

**Worst structural problem:** the domain's own prose repeatedly states a hard cross-domain dependency in the clearest possible language — "jointly necessary," "runs entirely on," a literal unlinked "(MAR)" — and then fails to encode it as a `prereqs` entry, five separate times across five unrelated relationships; the writing's own argument is more rigorous about what depends on what than the frontmatter that's supposed to formalize it.

**Entry count:** 22 before. With all four LOAD-BEARING proposals adopted, the AGR-012 split-and-fold, and the AGR-016 split: 22 + 4 (load-bearing) + 1 (AGR-016's breeding half, new ID) + 0 (AGR-012's colonate half moves to LAW-005, not a new AGR ID) = **27**. With VALUABLE also adopted: 30. With OPTIONAL: 32.

**Proposed out of symmetry, not mechanical need:** entries #8 (Terraced Agriculture) and #9 (Viticulture and Perennial-Crop Economics), both OPTIONAL and named as such — #8 substantially reuses AGR-005's irrigation mechanic on dramatic terrain rather than adding a new one, and #9 is real but the domain isn't incoherent without it, unlike the Famine or Population Pressure entries.

**Does Agriculture justify separate existence?** Yes, more clearly than any other domain audited so far. Its mechanism diversity is already the richest of any domain checked (genuine pools, branches, gates, events, and civic mechanics all present without prompting), its region balance is the best in the project, and — once the five prereq-orphan fixes land — its dependency graph reaches out to touch nearly every other domain in the wiki (COM, FIN, HLT, LAW, MIG, WAR, ENG, MAR) as a genuine load-bearing foundation layer, not a decorative adjunct sitting beside them.
