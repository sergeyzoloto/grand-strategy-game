# Religion Domain Audit

**Date:** 2026-09-08
**Scope:** `domains/religion/` — 17 entries, REL-001 through REL-017.
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/religion/`, and cross-checks against `domains/philosophy/`, `domains/law/`, `domains/finance/`, `domains/migration/`, `domains/engineering/`, `domains/health/`.

## Summary of the pattern

Religion has the best-documented boundary discipline of any domain audited this cycle: seven separate entries (`REL-002`, `REL-003`, `REL-004`, `REL-009`, `REL-012`, `REL-015`, `REL-016`) narrate their own split from PHI, ENG, HLT, or LAW explicitly in their own text — `REL-016` alone threads a three-way split (ENG owns the press, HLT owns the reading-habit shift, REL owns what mass scripture access does to institutional authority) in a single opening paragraph. Its mechanism diversity leans unusually hard toward genuine branches with real conditional logic — seven of seventeen entries (`REL-002`, `REL-003`, `REL-006`, `REL-007`, `REL-010`, `REL-016`, `REL-017`) explicitly reject a single-outcome framing in favor of a stated, evidence-weighted fork, more than any other domain checked. Region balance is solid (28.6% Europe+Mediterranean) and it shares this cycle's now-familiar 100%-core tier problem (17 of 17).

But this domain also has the messiest prereq graph found in this audit series: **three separate chronological inversions**, one of them 2,040 years off and using the wrong frontmatter field entirely. And it has a real, specific emergence gap: `REL-001` (Temple Economy, -3000) opens the domain already assuming "the temple," "ritual authority," and an organized priesthood exist as trusted institutions — with nothing anywhere covering how religious practice, sacred authority, or a distinguishable priestly role emerged from the tens of thousands of years of pre-institutional human religious behavior (ancestor veneration, shamanic practice, monumental ritual sites like Göbekli Tepe) that precede it.

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **FAIL**

`REL-001` opens by calling the temple "the most secure, most trusted, and most administratively capable institution available" — already assuming ritual authority and priesthood carry real social trust before explaining how either came to exist. Confirmed by grep: zero hits anywhere in the wiki for `gobekli tepe|shaman|ancestor (worship|cult)|animis` outside one passing mention inside `REL-006`'s own African absence note (decentralized lineage-based ancestor cults, cited as a contrast case, not covered on its own terms). Organized religious ritual and a socially-recognized priestly role plausibly predate `REL-001`'s -3000 starting point by tens of thousands of years — monumental ritual sites like Göbekli Tepe (roughly -9600) precede settled agriculture itself — and none of that deep prehistory is modeled.

### 2. SUBSTRATE GAP — **FAIL**

Three concrete absences:

- **Indulgences and the economy of salvation.** Confirmed by grep: zero hits anywhere in the wiki for `indulgence|purgatory|95 theses`. This is a genuinely striking gap given `REL-016` already covers the Reformation's print-technology dimension in detail — but the actual proximate trigger for Luther's 1517 protest (the sale of indulgences, resting on a developed doctrine of purgatory and merit transfer) is nowhere covered. `REL-016` tells the story of *how* the Reformation's fragmentation spread; nothing tells the story of what actually lit the fuse.
- **Religious education.** Confirmed by grep: zero hits for `madrasa|yeshiva|seminary|cathedral school` outside one unrelated hit in `PHI-015` (European university scholasticism, not religious training specifically). `REL-009`'s monasticism and `NAT-005`'s translation movements both silently assume a religious-education pipeline exists somewhere; nothing owns it.
- **The liturgical calendar.** Confirmed by grep: zero hits for `liturgical calendar|sabbath|feast day`. No entry covers how a religious tradition converts ordinary time itself into a resource-structuring device — mandated rest days, fasting periods, festival cycles — despite `REL-015`'s tithe entry already treating religious institutions as extracting a standardized share of a household's *economic* output; nothing extracts a standardized share of its *time*.

### 3. SCALE GAP — **PASS, with a minor gap**

Sorting the 17: world-system (`REL-003`, `REL-014`, `REL-017`), state (`REL-006`, `REL-007`, `REL-008`, `REL-011`, `REL-013`), institution (`REL-001`, `REL-004`, `REL-009`, `REL-012`), household/individual (`REL-005`, `REL-010`, `REL-015`, `REL-016`). This is a reasonably well-distributed spread — better than several domains checked this cycle. The one real gap: no entry covers lay religious association below the level of clergy or vowed monastic life — a confraternity, burial guild, or devotional lay association organizing charity and mutual aid among ordinary believers, distinct from `REL-009`'s specifically vowed and ordained monastic institutions. Narrow enough to be a VALUABLE addition rather than a probe failure.

### 4. REVERSE-DIRECTION GAP — **FAIL**

This domain spends a dozen entries building religious institutional wealth and authority — temple reserves (`REL-001`), monastic land (`REL-009`), tithe revenue (`REL-015`), cathedral capital (`REL-004`) — and never once covers a state seizing and dissolving it. Confirmed by grep: zero hits anywhere for `dissolution of the monaster|henry viii.*monaster|secularization of church`. Henry VIII's 1536-1541 Dissolution of the Monasteries, which transferred roughly a quarter of England's landed wealth out of religious institutional hands in five years, is the paradigm case and it's entirely absent — alongside comparable confiscations during the French Revolution and under 20th-century state-socialist regimes (`FIN-014`'s own command-economy entry gestures at this pattern without ever naming the religious-property angle specifically). `REL-007`'s heresy suppression is the closest existing reversal content, but it's about suppressing *belief*, not confiscating accumulated *institutional wealth* — a genuinely different mechanic.

### 5. PREREQ ORPHANS — **FAIL**, three concrete bugs — the most found in any domain audited this cycle

Every `prereqs` ID resolves to a real entry, but the chronology is badly broken in three separate places:

- **`REL-002`** (Canon, `date_start: -250`) lists `NAT-005` (Translation Movements, `date_start: 750`) as a prereq — a **1,000-year inversion**. `NAT-005` covers the Abbasid House of Wisdom and Toledo translation movements; nothing connects Third Buddhist Council canon-fixation to a translation program a millennium later. This looks like a mismatched ID with no plausible salvageable relationship — recommend removing it outright.
- **`REL-013`** (Confessional State, `date_start: 1501`) lists `LAW-011` (Constitution and Separation of Powers, `date_start: 1789`) as a prereq — a **288-year inversion**. Safavid Iran and the Peace of Augsburg have no plausible dependency on the 1789 US Constitution. Recommend removing it outright.
- **`REL-017`** (Religious Toleration, `date_start: -539`) lists `REL-013` (`date_start: 1501`) as a prereq — a **2,040-year inversion**, the most severe found in this audit series. This one has a clean diagnosis: `REL-017`'s own opening line calls itself "the reverse of `REL-013`'s confessional-state logic" — a mirror/contrast relationship, which belongs in `see_also`, not `prereqs`. The directional field was used for a non-directional relationship.

A fourth, smaller case: `REL-010`'s own region_variant (`date_start: 1510`) and top-level `date_start` (1500) both precede its prereq `MIG-013` (`date_start: 1526`) by 16-26 years — real but minor, and easily resolved by aligning `REL-010`'s own date rather than treating it as a modeling error.

A fifth, structural (not prereq) issue in the same family found elsewhere this cycle: `REL-007`'s top-level `date_start` (726) is later than its own cited middle-east region_variant (632, the Sunni-Shia succession dispute) — the same "own date doesn't match own earliest case" bug found and fixed in `WAR-001` earlier this audit series.

### 6. MECHANISM DIVERSITY — **PASS, unusually strong on branches**

Seven of seventeen entries are genuine, well-articulated branches: `REL-002`'s open-versus-closed canon fork, `REL-003`'s three-genuinely-different-strategies framing, `REL-006`'s hierarchy-versus-decentralization fork ("neither outcome representing an immature version of the other" — a direct, explicit rejection of a maturity-ladder framing), `REL-007`'s conviction-versus-material-interest ratio, `REL-010`'s direction-reversing syncretism, `REL-016`'s print-plus-pre-existing-contestation conjunction, and `REL-017`'s durability-conditional-on-interest framing. This is the richest branch-mechanic concentration of any domain audited this cycle. What's missing, matching every domain checked so far: a fully-realized **depleting pool** (design principle 4). There's a real near-miss already in plain sight: `religious authority` is the domain's most reused stat (touching eleven of seventeen entries), and `REL-016`'s own Reformation story is functionally a depletion event for it — but no entry frames it explicitly as a pool requiring maintenance investment. The proposed Dissolution entry (Part B) is the natural vehicle to finally cash this in properly.

### 7. BOUNDARY INTEGRITY — **PASS**

The best-documented boundary discipline of any domain checked this cycle: `REL-002` and `REL-003` both narrate their PHI split explicitly (institution and political process versus systematic theological or ethical argument); `REL-004` narrates its ENG split (vault construction versus administrative seat); `REL-009` narrates its HLT split (general monastic institution versus the specific charitable-medical function); `REL-012` narrates its own internal distinction from `LAW-001`; `REL-015` narrates its distinction from `LAW-015`'s zakat coverage; `REL-016` narrates a three-way split against ENG and HLT simultaneously. No entry's canonical domain is wrong. One missing cross-reference worth naming under Part C rather than a violation: `REL-012` (Sacred Law Courts) has no link to the Law domain's own `LAW-018` (Courts, Judges, and Trial Procedure, added in an earlier audit this cycle) despite the two being obvious complements — general judicial procedure versus the specific religious-institutional basis of a court's legitimacy.

---

## PART B — Proposed Additions

### LOAD-BEARING (4)

**1. Pre-Institutional Religion: Ancestor Cults, Shamanic Practice, and Sacred Sites**
- Regions: middle-east, europe, east-asia, americas — era, roughly -20000 to -3000
- Answers: Probe 1 (Emergence) directly
- Mechanism: religious practice — ancestor veneration, shamanic mediation with the unseen, monumental ritual construction (Göbekli Tepe, roughly -9600, predating settled agriculture itself) — exists as a real, socially organizing force for tens of thousands of years before any institution resembling `REL-001`'s temple accumulates reserves or claims administrative trust; a distinguishable priestly or shamanic role, resting on personal spiritual reputation rather than institutional office, is the actual starting condition every later entry in this domain builds on top of.
- Hook (seed): an origin gate — establishes sacred authority and a recognizable priestly role as a pre-institutional social fact `REL-001`'s temple economy silently assumes is already available to be organized.
- Prereqs: none. Enables: `REL-001`.
- Contested: true — how much continuity exists between specific Paleolithic and Neolithic ritual practice and the much later institutionalized religions this domain otherwise covers is genuinely disputed given how little direct evidence survives from purely oral, pre-literate ritual traditions.
- Confidence: medium.

**2. The Dissolution of Religious Institutions**
- Regions: europe — century, roughly 1536 to 1794
- Answers: Probe 4 (Reverse-direction) directly, and finally gives `religious authority` a genuine depleting-pool framing
- Mechanism: a state with sufficient administrative and military capacity can seize and redistribute a religious institution's accumulated land, reserves, and authority in a single deliberate act — Henry VIII's 1536-1541 Dissolution of the Monasteries (transferring roughly a quarter of England's landed wealth out of monastic hands) is the paradigm case, with the French Revolution's civil confiscation of church property (from 1789) as a second, differently-motivated instance — converting centuries of accumulated institutional wealth (`REL-001`, `REL-009`) into secular state or private assets essentially overnight.
- Hook (seed): an event — a depleting-pool mechanic for `religious authority` and accumulated institutional wealth, drawn down catastrophically by a single state action rather than eroding gradually, with no guarantee of ever being rebuilt.
- Prereqs: `REL-009`. Enables: none.
- Contested: false.
- Confidence: high.

**3. Indulgences, Purgatory, and the Economy of Salvation**
- Regions: europe — era, roughly 1200 to 1567
- Answers: Probe 2 (Substrate) directly — the actual trigger `REL-016` never names
- Mechanism: a developed doctrine of purgatory (a temporary post-death purification a living person's actions, including purchased indulgences, could shorten) converts spiritual merit into a good the church can sell directly, generating a durable revenue stream and an increasingly commercialized relationship between the institution and lay salvation anxiety — the specific practice (the 1517 indulgence campaign funding St. Peter's Basilica) that provokes Luther's protest, which `REL-016` then tracks the print-driven spread of.
- Hook (seed): a gate/modifier — converts institutional authority directly into a saleable good, at the cost of a durable vulnerability to exactly the reform backlash `REL-016` covers once that commercialization is perceived as corrupt.
- Prereqs: `REL-001`. Enables: `REL-016` (currently missing this link — `REL-016` covers the Reformation's spread mechanism without ever covering its trigger).
- Contested: false.
- Confidence: high.

**4. Religious Education: Madrasas, Yeshivas, and Cathedral Schools**
- Regions: middle-east, europe, south-asia — era, roughly 700 to 1500
- Answers: Probe 2 (Substrate) directly
- Mechanism: a dedicated institution for training clergy, jurists, and scholars in religious learning — the madrasa, the yeshiva, the cathedral school — converts religious knowledge transmission from informal apprenticeship into a systematic, credentialing pipeline, the same underlying mechanism `LAW-004`'s examination bureaucracy later applies to secular administration and `NAT-005`'s translation movements silently assume already exists to train the scholars doing the translating.
- Hook (seed): a gate — the credentialing pipeline that produces the judges (`REL-012`), canon-fixing councils (`REL-002`), and missionary corps (`REL-014`) this domain's other entries assume already exist, trained and available.
- Prereqs: `REL-009`. Enables: `REL-012`.
- Contested: false.
- Confidence: high.

### VALUABLE (3)

**5. The Liturgical Calendar and Sacred Time**
Regions: middle-east, europe, south-asia. Era, roughly -600 to 1600. Answers Probe 2 with a mechanism distinct from `REL-015`'s revenue extraction: a religious tradition converts ordinary time itself into a structured resource — mandated rest days, fasting periods (Ramadan), festival cycles — extracting a standardized share of a household's *time* the way `REL-015` extracts a share of its *income*. Hook: a modifier that structures economic and social rhythm independent of any state labor policy, distinct from and prior to the work-discipline mechanics covered elsewhere in this record. Prereqs: none. Contested: false. Confidence: high.

**6. Relic Veneration and the Cult of Saints**
Regions: europe, middle-east. Era, roughly 300 to 1500. Answers Probe 6 with a mechanism distinct from `REL-005`'s traffic-justifies-infrastructure logic: a relic (a saint's body part, a sacred object) functions as a scarce, non-fungible prestige good institutions compete to acquire, display, and defend the authenticity of — driving inter-institutional rivalry and outright relic theft or forgery in a way ordinary pilgrimage traffic alone doesn't generate. Hook: a modifier — a scarce sacred-object economy distinct from the traffic-volume logic `REL-005` already owns. Prereqs: `REL-005`. Contested: false. Confidence: high.

**7. Religious Confraternities and Lay Guilds**
Regions: europe, middle-east. Era, roughly 1200 to 1600. Answers Probe 3 (scale) directly: a lay association organizing charity, burial rites, and devotional practice among ordinary believers, distinct from `REL-009`'s vowed and ordained monastic institutions — a household-and-neighborhood scale of religious organization this domain otherwise entirely lacks. Hook: a civic mechanic — mutual-aid religious organization at a scale below clergy and above the individual household. Prereqs: `REL-009`. Contested: false. Confidence: high.

### OPTIONAL (2)

**8. The Knights Templar: Rise and Suppression**
Regions: europe, middle-east. Century, 1119-1312. A specific, vivid case combining `REL-009`'s monastic-order mechanic with `REL-001`'s temple-economy scale (the Templars became major international bankers) before Philip IV's largely fiscally-motivated suppression in 1312 — real and dramatic, but mechanically a smaller-scale instance of the Dissolution entry (#2) rather than a genuinely separate mechanism. Honestly flavour.

**9. State Shinto and the Modern Invention of a National Religion**
Regions: east-asia. Era, 1868-1945. Meiji Japan's deliberate construction of State Shinto as a national-unity ideology — a modern, self-aware instance of `REL-013`'s confessional-state logic applied to a tradition reshaped specifically for the purpose. Real and well-documented, but its mechanism is already what `REL-013` covers. Honestly flavour.

---

## PART C — Proposed Modifications

### SPLIT

None found. `REL-007` bundles iconoclasm, heresy prosecution, and holy war under one Mechanism section, but its own text explicitly treats these as points on a single severity gradient ("ranging from doctrinal argument and excommunication through outright military campaign") rather than as separate mechanisms — checked against the Multi-domain rule's two-Mechanism test and found to genuinely have one. No entry in this domain is straining against its own single-Mechanism structure.

### MERGE

**Considered and rejected: `REL-013` (Confessional State) and `REL-017` (Religious Toleration).** `REL-017`'s own Hook calls itself the direct reverse of `REL-013`. Kept separate because this is a deliberate mirror pair, the same pattern used correctly elsewhere in this wiki (`FIN-002`/`FIN-018`, `MAR-025`/`MAR-026`) — two opposite policy choices available to the same kind of actor, not one mechanism told twice. The actual bug in this pair isn't that they should merge, it's that their relationship was encoded in the wrong field (see Part A, Probe 5, and RE-DOMAIN below).

### RE-DOMAIN

- **`REL-002`**: remove `NAT-005` from `prereqs` (Part A, Probe 5) — no salvageable relationship identified.
- **`REL-003`** and **`REL-002`**: the dependency direction is backward. `REL-003`'s own Social-consequence section already argues universalism *drives* canon-fixation elaboration ("directly driving canon-fixation... specifically in traditions that make this move"), and `REL-003` (`date_start: -500`) chronologically precedes `REL-002` (`date_start: -250`). Recommend swapping: `REL-003.enables` should include `REL-002`, not the reverse.
- **`REL-013`**: remove `LAW-011` from `prereqs` — no salvageable relationship identified.
- **`REL-017`**: move `REL-013` from `prereqs` to `see_also` — a mirror/contrast relationship, not a chronological dependency.
- **`REL-010`**: adjust `date_start` (and its americas region_variant) to align with its `MIG-013` prereq rather than precede it by 16-26 years.
- **`REL-007`**: correct the top-level `date_start` (726) to match its own earliest cited region_variant (632, Sunni-Shia succession dispute).
- **`REL-012`**: add a `see_also` cross-reference to `LAW-018` (Courts, Judges, and Trial Procedure) — the two entries are natural complements (general judicial procedure versus the religious-institutional basis of a court's legitimacy) with no existing link between them.
- No entry's canonical domain needs to change.

### RETIER

**`REL-008` (Investiture Controversy): core → standard.** Single-region (`europe` only, with the Byzantine case handled entirely as an absence contrast rather than a second real region), `enables: []`, and a narrow eleven-year window (1075-1122) — structurally identical to `MAR-009` and `FIN-009`, both retiered earlier this cycle for the same shape: a real, well-documented, single-lineage institutional episode without the comparative breadth most of this domain's other `core` entries carry.

Not touching the other 16: even the domain's other narrow entries (`REL-010`, `REL-015`) carry genuine multi-region comparative texture `REL-008` lacks.

### CUT

**None.** Read all 17 entries specifically hunting for a self-admitted redundancy or a Hook with no real mechanic and didn't find one — consistent with the pattern established across most domains audited this cycle. `REL-008`'s RETIER above is this audit's closest adjacent finding, resolved as reduced weight rather than removal since its concrete history (and its role as `REL-004`'s direct successor) remains genuinely worth keeping.

---

## PART D — Verdict

**Worst structural problem:** this domain has the messiest prereq graph of any audited this cycle — three separate chronological inversions (1,000 years, 288 years, and 2,040 years), the last of which encodes an explicitly-stated mirror relationship in the wrong directional field entirely — a data-integrity problem layered on top of, and independent from, a genuine coverage gap: the domain's own foundational entry assumes organized priesthood and ritual authority already carry social trust, with tens of thousands of years of pre-institutional religious practice never modeled.

**Entry count:** 17 before. With all four LOAD-BEARING proposals: 21. With VALUABLE also adopted: 24. With OPTIONAL: 26.

**Proposed out of symmetry, not mechanical need:** both OPTIONAL entries (#8 The Knights Templar, #9 State Shinto) are named as such — real, specific history whose mechanisms the Dissolution proposal (#2) and `REL-013` respectively already substantially cover. Of the VALUABLE tier, **Religious Confraternities and Lay Guilds (#7)** is the softest call: a genuine scale gap, but a narrower and less structurally necessary one than the load-bearing tier's coverage gaps.

**Does Religion justify separate existence?** Yes, clearly — its boundary self-documentation is the best of any domain audited this cycle, its branch-mechanic concentration is the richest, and its region balance is solid. The prereq-graph bugs found here are a genuine outlier by count, but they're mechanical errors in cross-referencing, not signs of conceptual incoherence — the entries' own prose is consistently strong even where their frontmatter data disagrees with it. Fixing the data brings the graph up to the same standard the writing already meets.
