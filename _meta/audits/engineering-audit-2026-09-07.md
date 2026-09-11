IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

# Engineering Domain Audit

**Date:** 2026-09-07
**Scope:** `domains/engineering/` — 22 entries, ENG-020 through ENG-260.
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/engineering/`, and cross-checks against `domains/commerce/`, `domains/agriculture/`, `domains/nature/`, `domains/war/`, `domains/maritime/`.

## Summary of the pattern

Engineering is disciplined in exactly the place the other audited domains weren't — its NAT/ENG and WAR/ENG boundary practice is nearly flawless, every technique entry correctly hands consequence to whichever domain owns it, and its confidence is high because the history really is this well-established. But it has one specific, repeating tell that's the mirror image of Commerce's problem: **other domains keep writing checks against Engineering's name, and Engineering's own entry list never cashes them.** `COM-020` states outright, in its own prose, "the smelting chemistry itself is ENG's territory" — no smelting entry exists. `AGR-060` states outright that a mill's "construction technique belongs to ENG's territory once an entry for it exists there, which as of this record it does not" — it still doesn't. `NAT-040`'s own absence note cites Chinese mechanical clockwork (the Song-era astronomical clock tower) as a real engineering achievement — no clockwork entry exists to house it. This isn't three unrelated gaps; it's the same promise, made by three different domains, broken the same way three times. Separately, the domain models almost nothing but growth — not one entry has capability loss or engineering failure as its actual mechanic, despite sitting on the single best-documented case of exactly that (Roman concrete's genuinely lost recipe) without ever using it.

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **FAIL**

The earliest entries, ENG-020 (Irrigation and Dams, -6000) and ENG-030 (The Potter's Wheel, -4500), both open mid-capability: ENG-020 assumes a society that can already dig, channel, and coordinate labor; ENG-030's own Mechanism explicitly contrasts the wheel against hand-building ("a slow, individually-variable craft") as an _already-existing_ baseline it improves on. Nothing anywhere explains how basic tool-making, fire control, or working stone/wood/bone into a functional shape — the actual precondition for both — came to exist. More concretely: `COM-020` states, verbatim, "the smelting chemistry itself is ENG's territory" — and no entry in this domain covers smelting, ore reduction, or metalworking technique at all. This is a promise made explicitly, in writing, by another domain, and never delivered.

### 2. SUBSTRATE GAP — **FAIL**

Four concrete absences, each checked against the full wiki:

- **Smelting/metalworking technique** — see above. Grepped `smelting`: hits only in `COM-020`, `AGR-160`, `WAR-040` — all _consumers_ of a chemistry ENG is supposed to own and doesn't.
- **Papermaking** — grepped the whole wiki for `papermaking`/`paper mill`: zero hits anywhere. `ENG-150` (The Printing Press) has `prereqs: []` despite a press being useless at scale without a cheap printable surface — Gutenberg's own breakthrough was only economically viable because paper, not parchment, had become affordable by the 1450s.
- **Fired brick** — grepped `brick`: zero hits anywhere in the entire project. The single most ubiquitous premodern building material, load-bearing for the Indus Valley cities `COM-030` already credits for standardization, Mesopotamian ziggurats, and most of world construction history, has no entry.
- **Mechanical clockwork** — grepped `clockwork|escapement|mechanical clock`: the only hits are `NAT-040` (Mechanics, the physics _theory_, not the device) and `MAR-250` (the marine chronometer specifically, correctly MAR's territory). `NAT-040`'s own absence note name-drops the Song-era Chinese astronomical clock tower (Su Song's _Xin Yixiang Fayao_) as a real engineering achievement with nowhere in the wiki to actually live.

And a fourth, already-confirmed case from outside this domain: `AGR-060`'s own text states "the mill's own construction technique belongs to ENG's territory once an entry for it exists there, which as of this record it does not" — checked, and it still doesn't. `COM-230`'s cold-chain entry has the same open dependency on refrigeration technique that was never closed.

### 3. SCALE GAP — **PARTIAL FAIL**

Sorting the 22: monumental/state-scale (ENG-020, ENG-090, ENG-110, ENG-080, ENG-050 partially) and industrial-factory/urban-scale (ENG-210, ENG-180, ENG-170, ENG-190, ENG-320, ENG-280 through ENG-260) between them account for the overwhelming majority of the domain. The pre-industrial household or workshop scale — the level at which most people across most of this record's timeframe actually encountered engineered objects — is nearly empty. ENG-030 (Potter's Wheel) is the domain's one genuine case at this scale, correctly tiered `standard`. There is no entry on a hand-cranked or foot-powered household device, a small-scale water-lifting device (shaduf, noria, Archimedes' screw used at farm rather than state-canal scale — distinct from ENG-020's dam-and-reservoir framing), or any pre-industrial precision mechanism. The domain jumps from pharaonic infrastructure to Watt's steam engine with almost nothing at human scale in between.

### 4. REVERSE-DIRECTION GAP — **FAIL**

Zero entries have capability loss, technique regression, or engineering failure as their actual mechanic. The closest the domain comes is a single sentence buried in `ENG-080`'s Social consequence — hydraulic engineering expertise "once lost (as in post-Roman Europe), takes generations to reconstruct" — mentioned once, never modeled, never given its own Hook. This is a strange omission given the single best-documented, most dramatic case of exactly this sits directly on top of an entry the domain already has: Roman concrete's specific pozzolanic-ash recipe, which gave the Pantheon's dome both its remarkable durability and a span unmatched in unreinforced concrete for over 1,700 years, was not merely under-used after Rome's fall — it was genuinely lost, with post-Roman Europe reverting to weaker lime-mortar construction until 18th–19th-century engineers (Smeaton, and eventually Portland cement) reverse-engineered comparable performance through a different chemical path. Grepped `pozzolan`, `opus caementicium`: zero hits. `ENG-110` (Concrete, Vault, and Dome) covers the achievement and never mentions that it was later lost.

### 5. PREREQ ORPHANS — **FAIL**

- **ENG-150** (Printing Press, `prereqs: []`) — depends on cheap paper existing at scale to matter at all; no papermaking entry exists to prereq (see Probe 2).
- **ENG-250** (Elevator and Safety Brake, `prereqs: []`) — the entry addresses the safety-brake mechanism exclusively and never states what actually powers the car's vertical motion. The historical 1857 Haughwout Building installation was steam-hoisted; `ENG-170` (Steam Engine) is absent from both `prereqs` and the body text entirely.
- **ENG-250's own Hook overclaims.** It states the safety brake is "the specific, narrow innovation that makes the skyscraper possible as a building form at all." It isn't, alone — a load-bearing masonry building physically cannot exceed roughly 10–20 stories regardless of elevator access, because wall thickness at the base must scale with height (Chicago's Monadnock Building, 1891, sits close to that practical ceiling). Steel-frame structural engineering, which lets exterior walls stop bearing load entirely, is the other necessary precondition, and it has no entry anywhere in this domain despite `ENG-260` (cheap Bessemer steel) and `ENG-050` (architectural glass) both existing as its natural neighbors.

### 6. MECHANISM DIVERSITY — **PASS, with one clear gap**

Real diversity is present: gates (`ENG-170`, `ENG-250`, `ENG-230` — the last explicitly self-described as "a discrete jump rather than a gradual improvement"), a genuine cascading-bottleneck mechanic (`ENG-180`, each invention just relocating the constraint to the next stage), a real one-time price-collapse event (`ENG-260`), and a distinctive recursive-compounding mechanic shared identically between `ENG-190` and `ENG-330` ("each generation of more precise... capability is used to build the next" — machine tools and semiconductor fabrication both explicitly run on it). This is comfortably not a flat modifier list. But **zero entries carry a genuine depleting-pool mechanic** (design principle 4) — the same gap Commerce had before this session's fixes — despite `ENG-080`'s own prose gesturing directly at one ("expertise... once lost... takes generations to reconstruct") and never formalizing it. This is the same missing category, unclaimed a second time.

### 7. BOUNDARY INTEGRITY — **PASS on discipline, FAIL on delivery**

Every WAR/ENG and NAT/ENG split in this domain is handled correctly and explicitly — `ENG-140` versus `WAR-130`, `ENG-170`/`ENG-230`/`ENG-300`/`ENG-330` all correctly prereq the relevant `NAT` knowledge entry (`NAT-040`, `NAT-170`, `NAT-210`) rather than re-deriving the underlying science, and `ENG-290` correctly defers military aviation doctrine to `WAR-270` while keeping the engine itself. This is real discipline, better than any other domain audited so far. The actual failure runs the other direction: this domain doesn't wrongly claim territory, it **fails to occupy territory explicitly assigned to it** — see Probes 1 and 2. The one soft case worth naming: `ENG-320` (Agrochemicals) carries substantially more agronomic/economic consequence content in its own Social consequence section than the domain's usual technique-only discipline allows, closer to AGR's territory than ENG's — a precedent already partially addressed by AGR-280 being made to prereq it directly, not recommending further change here.

---

## PART B — Proposed Additions

### LOAD-BEARING (6)

**1. Metalworking and the Smelting Furnace**

- Regions: middle-east, east-asia, africa — era, roughly -6000 to -1200
- Answers: Probe 1 (emergence) and 7 (boundary — cashes `COM-020`'s explicit IOU)
- Mechanism: converts raw ore into usable metal through controlled high-temperature reduction (charcoal fuel, bellows-driven draft to reach and sustain smelting temperature) — the actual chemistry and furnace technique `COM-020` names as ENG's own and never receives.
- Hook (seed): a gate — metal-as-material doesn't exist as a tradeable commodity (`COM-020`) or a military lever (`WAR-040`) until this technique exists first; both entries currently float without their own stated foundation.
- Prereqs: none (foundational). Enables: `COM-020`, `WAR-040`, `AGR-160` (all three currently missing this as a formal prereq).
- Contested: true — whether bronze metallurgy diffused from a single Near Eastern origin or arose independently at multiple centers (Southeast Asian sites like Ban Chiang carry their own independent-origin claims) is a live archaeological dispute. Model regional smelting traditions as independently-triggerable rather than assuming one-directional diffusion.
- Confidence: medium-high.

**2. Papermaking**

- Regions: east-asia, middle-east, europe — era, roughly 100 to 1200
- Answers: Probe 2 (substrate) and 5 (prereq orphan — closes `ENG-150`'s missing dependency)
- Mechanism: converts pulped plant fiber into a cheap, foldable, mass-producible writing surface, removing the cost ceiling parchment (an entire animal hide per few pages) and papyrus (geographically restricted to reed-growing regions) both impose on written material at any scale.
- Hook (seed): a gate — cheap paper is what actually lets `ENG-150`'s press-based printing scale into mass production; without it, a press has nothing affordable to print onto.
- Prereqs: none. Enables: `ENG-150` (currently missing this).
- Contested: true — the popular "Battle of Talas" transmission legend (Chinese papermakers captured in 751 CE personally teaching the Abbasids) is a widely-repeated simplification of what was more plausibly a gradual, multi-channel transmission; model the technology's westward spread as a diffusion process with an uncertain, disputed specific mechanism rather than one dramatic transfer event.
- Confidence: high on the technology, medium on the transmission story.

**3. Fired Brick and Masonry Units**

- Regions: middle-east, south-asia, east-asia — era, roughly -3500 to -1000
- Answers: Probe 2 (substrate) directly — the single most-used premodern building material with no entry anywhere
- Mechanism: kiln-firing converts raw clay into a durable, weather-resistant, dimensionally-standardized modular unit, a structural and durability step change from sun-dried mudbrick, which crumbles under sustained moisture and can't bear comparable load.
- Hook (seed): a modifier on construction durability and a gate on standardized modular building — the Indus Valley's cross-site brick-dimension consistency (already credited to `COM-030`'s weights-and-measures entry) is this entry's own direct material precondition, not a coincidence.
- Prereqs: none. See_also: `COM-030` (Indus Valley standardization), `ENG-110` (brick vaulting predates and coexists with stone/concrete vaulting).
- Contested: false.
- Confidence: high.

**4. Steel-Frame Construction and the Skyscraper**

- Regions: americas — decade precision, 1885–1900
- Answers: Probe 5 (prereq orphan) directly — closes the gap between `ENG-250`'s overclaimed Hook and what the historical record actually required
- Mechanism: an internal steel skeleton carries a building's entire structural load, letting exterior walls become non-load-bearing "curtain walls" of minimal thickness regardless of height — replacing a masonry problem (wall thickness scaling with height, imposing a hard practical ceiling) with a steel-fabrication and fireproofing one instead.
- Hook (seed): a joint-necessity gate, mirroring `ENG-290`/`ENG-015`'s established "neither alone sufficient" pattern — the skyscraper requires this entry, `ENG-250`'s safety elevator, and cheap Bessemer steel (`ENG-260`) together; none of the three alone produces the building form each is individually credited with enabling.
- Prereqs: `ENG-260`. See_also: `ENG-250`, `ENG-050` (glass curtain walls).
- Contested: true — whether Chicago's 1885 Home Insurance Building genuinely holds "first true skeletal-frame skyscraper" priority, versus earlier partial-iron-frame precedents in Britain not fully load-bearing in the same sense, is a real, still-debated architectural-history attribution question.
- Confidence: medium.

**5. Roman Concrete's Lost Recipe**

- Regions: mediterranean, europe — era, roughly 100 to 1750
- Answers: Probe 4 (reverse-direction) directly — the domain's strongest available case, sitting on top of an entry it already has
- Mechanism: the specific pozzolanic volcanic-ash formulation behind Roman concrete's durability and the Pantheon's unreinforced dome span is not merely superseded after the Western Empire's collapse but genuinely lost — post-Roman European construction reverts to weaker lime-mortar masonry for over a millennium, with comparable durability only reverse-engineered through a different chemical pathway (Smeaton's hydraulic lime experiments, eventually Portland cement) from the 18th century onward.
- Hook (seed): a depleting-pool-adjacent event — engineering knowledge sustained by an institution (Roman state capacity to source and transport specific volcanic ash) doesn't merely stop advancing when that institution collapses, it can regress below a level not recovered for centuries, distinct from every other entry in this domain's default growth framing.
- Prereqs: `ENG-110`. See_also: `AGR-150` (a structurally similar depleting-capacity case in a different domain).
- Contested: true — recent materials-science research (self-healing lime-clast chemistry, actively studied and only partly settled as of the 2020s) complicates exactly _why_ Roman concrete was so durable, and how much of the "loss" was genuine forgotten chemistry versus economically-abandoned logistics (pozzolana required specific volcanic-ash sourcing unavailable at comparable cost once imperial shipping networks broke down). Model both the durability mechanism and the completeness of its loss as open questions rather than settled fact.
- Confidence: medium — the historical loss is solid; the specific chemistry explanation is recent, evolving research I should present cautiously.

**6. Simple Machines: Lever, Pulley, Screw, and Wedge**

- Regions: mediterranean, middle-east, east-asia — era, roughly -3000 to -250
- Answers: Probe 2 (substrate) directly — the literal foundational mechanical-advantage toolkit every construction entry in this domain implicitly assumes
- Mechanism: converts a task requiring more force than is locally available (lifting a multi-ton stone block, tensioning a structure) into one requiring less force applied over greater distance or time — cranes, block-and-tackle, and windlasses are all direct applications, and every subsequent, more complex machine in this domain builds on the same underlying force-and-distance trade-off.
- Hook (seed): a gate this domain has been quietly assuming since ENG-020 — moving irrigation-scale stone and earth, raising `ENG-110`'s dome centering, or erecting `ENG-090`'s bridge spans all require mechanical advantage devices that, as currently written, this record never establishes as existing.
- Prereqs: none (foundational). Enables: `ENG-020`, `ENG-090`, `ENG-110` (all currently missing this as an explicit precondition).
- Contested: false.
- Confidence: high.

### VALUABLE (3)

**7. Mechanical Clockwork and the Escapement**
Regions: east-asia, europe. Era, roughly 1090–1400. Answers Probe 2 (substrate — precision-mechanism lineage predating `ENG-190`'s machine tools by centuries) and gives `NAT-040`'s own Chinese-clockwork absence note somewhere to live. Mechanism: the escapement converts continuous force (a falling weight, later a coiled spring) into regular, countable, mechanically-released increments — the foundational precision-timekeeping device whose gear-cutting tolerance demands directly prefigure the machine-tool precision lineage. Hook: a modifier/gate on precision-mechanism capability, distinct from and earlier than `ENG-190`'s later industrial version of the same underlying idea. Prereqs: none. See_also: `ENG-190`. Contested: true — whether European mechanical clockwork developed independently or carries any transmitted influence from Su Song's 1092 Chinese astronomical clock tower (which predates the European mechanical clock by roughly two centuries) is a genuinely unresolved history-of-technology question. Confidence: medium.

**8. Refrigeration and Mechanical Cooling**
Regions: europe, americas. Century, roughly 1834–1876. Answers Probe 2 directly, closing the `AGR-260`/`COM-230` dependency both already gesture at without an entry to point to. Mechanism: the vapor-compression cycle mechanically moves heat against its natural gradient, converting cooling from something requiring harvested natural ice and insulated ice-house storage into an on-demand, location-independent process — the direct technical precondition `COM-230`'s 1876 refrigerated shipping case already assumes exists. Hook: a gate removing a hard prior ceiling (natural ice availability and storage duration) entirely. Prereqs: `ENG-170` (or a general thermodynamics prereq). Contested: false. Confidence: high.

**9. Windmill and Watermill Construction**
Regions: middle-east, europe. Era, roughly 850–1200. Answers Probe 2 directly, closing `AGR-060`'s explicit, named dependency. Mechanism: converts a mill's power source and drive-train (waterwheel or windmill sail, gearing down to a millstone's rotation) into ENG's own construction-technique entry, distinct from `AGR-060`'s labor-and-economic-consequence framing of the same technology. Hook: a modifier on the power-source-and-gearing side specifically, mirroring the `ENG-020`/`AGR-100` technique-versus-consequence split already established as house style. Prereqs: none. See_also: `AGR-060`, `ENG-020`. Contested: false. Confidence: medium-high — the earliest Persian vertical-axis windmills (9th century) predate the more commonly-cited European horizontal-axis design by centuries, a good non-European-priority case, though my confidence in the finer regional dating is lower than for the mechanism generally.

### OPTIONAL (2)

**10. The Crystal Palace and Prefabricated Construction**
Flavor entry on the specific 1851 case already named inside `ENG-050`'s own Timing section. The actual distinct mechanic — standardized cast-iron and glass components manufactured off-site and assembled on a compressed schedule — is real, but it substantially overlaps with `ENG-050`'s glass-panel-scale framing and the proposed steel-frame entry's structural-metal theme. Honest flag: proposing this mainly because it's already half-named in the wiki, not because the domain is incoherent without it.

**11. Cast Iron and the Iron Bridge**
Flavor entry on the 1779 Coalbrookdale Ironbridge, the first major cast-iron structural bridge. Real, vivid, well-documented history, but it reads more naturally as a region_variant addition to `ENG-090` (Roads and Bridges) than as a standalone mechanic, and it substantially anticipates the proposed steel-frame entry's structural-metal-replaces-masonry logic on a smaller scale. Naming it because it's a genuinely good story, not because the domain needs it as its own ID.

---

## PART C — Proposed Modifications

### SPLIT

**ENG-080 (Water Supply and Sanitation Engineering)** promises two distinct engineering problems in its own title and opening disclaimer ("gets water in and waste out reliably") and delivers only one. Every region_variant, the entire Mechanism section, and the entire Regional-specifics section address water _supply_ (qanats, Roman aqueducts) — sewer and waste-removal engineering, a genuinely distinct technical problem (different gradient and flow requirements, different failure modes, and an even more direct HLT-090 disease-dynamics tie via cholera and contaminated water supply specifically) never actually appears despite being named in the title. Split into:

- **ENG-080 retained**: water supply engineering only (qanats, aqueducts) — rename to drop "and Sanitation" or genuinely earn it.
- **A new entry**: Sewer and Waste Removal Engineering — the Cloaca Maxima, medieval cesspit and garderobe systems, and 19th-century engineered sewer systems (Bazalgette's post-1858 "Great Stink" London system as the paradigm case), with a direct, concrete tie to John Snow's 1854 Broad Street cholera investigation linking contaminated water supply to sewage specifically — arguably a stronger HLT-090 connection than water supply alone currently carries.

### MERGE

None found as a clean two-into-one case. `ENG-190` and `ENG-330` (Machine Tools, Semiconductors) share an identical recursive-compounding mechanic almost word-for-word, but each carries genuinely distinct historical content, regions, and eras — this is deliberate mechanic reuse across a century-plus gap, the same pattern this project already uses for stats like `yield ceiling` and `market trust radius`, not redundancy. Considered and rejected.

### RE-DOMAIN

No entry's canonical `domain` needs to change — this domain's WAR/ENG and NAT/ENG discipline is the best of any domain audited so far. One concrete missing-link fix:

- **ENG-250** (Elevator and Safety Brake) — add `ENG-170` (Steam Engine) to `prereqs`. The entry currently addresses only the safety-brake mechanism and never states what powers the car's vertical motion at all; the historical 1857 Haughwout Building installation was steam-hoisted.

### RETIER

Not proposing changes here. Unlike Commerce's original all-`core` problem, this domain already shows real tier discipline — 5 of 22 entries (`ENG-030`, `ENG-050`, `ENG-015`, `ENG-250`, `ENG-270`) are correctly `standard`, and the `core` entries with `enables: []` (`ENG-230`, `ENG-300`) earn the weight on conceptual grounds (each is the entry other domains' own stats — `information speed` — trace back to) even without a formal downstream prereq yet. I looked for a stub-carrying-too-much-weight or core-that's-really-flavour case and didn't find one worth forcing.

### CUT

**ENG-015 (The Conveyor Belt)** — its own Hook states, verbatim, that it is "a further intensification of the externally-imposed time discipline COM-170 describes rather than a qualitatively new mechanic." This is the entry admitting, in its own words, that it fails the CUT criterion's second clause: a mechanic that duplicates another entry's. The one genuinely novel content it carries — Ford's specific, dramatically quantified 1913 assembly-time reduction — doesn't need a standalone ID to survive; it reads naturally as a region_variant or elaboration on `COM-170` itself. Recommend cutting `ENG-015` as a standalone entry and folding the Ford case into `COM-170`.

This is the only entry I'd cut outright. I looked for a second candidate and didn't find one I could defend — the rest of this domain's problem is absent entries and unclaimed IOUs, not padding.

---

## PART D — Verdict

**Worst structural problem:** other domains write explicit, textual IOUs against Engineering's name — "the smelting chemistry itself is ENG's territory," "belongs to ENG's territory once an entry for it exists there" — and Engineering's own entry list has never cashed a single one of them, leaving three foundational technologies (smelting, milling, and by extension papermaking) permanently promised and permanently absent.

**Entry count:** 22 before. With all six LOAD-BEARING proposals, the one SPLIT, and the one CUT: 22 + 6 + 1 − 1 = **28**. With VALUABLE also adopted: 31. With OPTIONAL: 33.

**Proposed out of symmetry, not mechanical need:** entries #10 (Crystal Palace) and #11 (Iron Bridge), both OPTIONAL and named as such — both are genuinely good history that's already half-referenced elsewhere in the domain, but neither adds a mechanism the proposed steel-frame or brick entries don't already cover at a more load-bearing scale.

**Does Engineering justify separate existence?** Yes, unambiguously — it's the domain every other domain's own text keeps reaching for by name (`COM-020`, `AGR-100`, `AGR-260`, `AGR-060`, `COM-230`, `NAT-040`, `NAT-180`, `NAT-170`, `NAT-210` all cross-reference it directly), its boundary discipline is the cleanest of any domain audited, and its genuine mechanism diversity (cascading bottlenecks, recursive compounding, discrete-jump gates) is real rather than assumed. The problem isn't whether this domain belongs — it's that half its foundational entries are IOUs other domains have already written and Engineering has yet to sign for.
