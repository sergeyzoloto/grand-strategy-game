# Natural Sciences Domain Audit

**Date:** 2026-09-07
**Scope:** `domains/nature/` — 15 entries, NAT-001 through NAT-015.
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/nature/`, and cross-checks against `domains/engineering/`, `domains/agriculture/`, `domains/philosophy/`, `domains/health/`.

## Summary of the pattern

Natural Sciences has the cleanest boundary discipline of any domain audited — its NAT/ENG split is immaculate, and it goes further than the rules require by imposing the same instrumental-versus-argument discipline against Philosophy on its own initiative (`NAT-001`, `NAT-003` both explicitly hand off "the fuller philosophical treatment" without being told to). But it has the single worst raw number of any domain checked: **Europe and the Mediterranean account for 42.9% of this domain's regional footprint, past the 40% line this audit is explicitly told to flag.** This isn't a case of any one entry being dishonest — every entry that touches a non-European tradition (`Ibn al-Haytham`, the Maragha school, Bose, al-Khwarizmi, Ibn al-Khatib) credits it seriously and specifically. The skew is compositional: seven of the fifteen entries (`NAT-008` through `NAT-014`) cover the 1661–1935 "classical and modern physics" sequence, a period this record's own evidence agrees really was concentrated in European institutions, and that one topic cluster alone outweighs the domain's genuinely strong earlier multi-civilizational entries by sheer entry count. Separately, and just as tellingly: this is a domain literally defined as "how a society comes to know things," and it has not one single entry about a society _losing_ that knowledge — `NAT-005` explicitly frames translation as the alternative to "being lost with the society that produced it" and then never once shows what the loss side of that sentence actually looks like, including for its own subject institution.

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **PASS**

`NAT-002` (Mathematics, Geometry, and Calendrical Astronomy, -1800) and `NAT-001` (Metaphysics as an Explanatory Framework, -600) do real work establishing preconditions rather than opening mid-capability — `NAT-001`'s own framing is explicitly about what makes a question askable _before_ any inquiry into it starts, which is a genuinely earlier and more foundational move than most domains manage at their own starting point. This is one of the domain's real strengths, on the same level as Agriculture's Holocene Bargain. The one loose thread — `NAT-002`'s notational and calendrical toolkit has no formal link to `AGR-006` (Storage, Sealing, and Agrarian Accounting), which already documents the actual practical proto-numeral and token-accounting precursor to formalized mathematics, predating `NAT-002`'s own -1800 start by six thousand years — is a real gap, but it's a missing _link_ to content the wiki already has, not a missing concept. Recorded under Probe 5 rather than double-counted here.

### 2. SUBSTRATE GAP — **FAIL**

Two concrete absences, both silently load-bearing for entries this domain already has:

- **Optical instrumentation.** Grepped the whole wiki for `telescope|microscope|lens-grind`: zero hits anywhere. `NAT-006` (Heliocentrism) never mentions Galileo's telescopic observations — the moons of Jupiter, the phases of Venus — despite these being the decisive empirical evidence that converted heliocentrism from "mathematically cleaner" to "observationally confirmed." `NAT-011` (Germ Theory) is worse: Pasteur's and Koch's entire experimental program required optical instruments capable of resolving microorganisms, and the entry's own Mechanism section never once mentions how anyone could see what they were postulating.
- **The scientific society as ongoing infrastructure.** Grepped for `peer review|scientific journal|royal society|academy of sciences`: the only hits are inside `NAT-007` itself, mentioned in one clause ("institutionalization... through bodies like the Royal Society") and never given its own treatment. Every entry from `NAT-008` onward silently assumes this institution — a standing, adversarial, publication-and-priority-based knowledge-validation system — already exists and keeps working, without the wiki ever describing what it actually does or what happens when it's contested (a real, well-documented case: the Newton–Leibniz calculus priority dispute).

### 3. SCALE GAP — **PARTIAL FAIL**

Sorting the 15: civilizational/comparative (`NAT-001`, `NAT-002`, `NAT-003`, `NAT-004` partially), institutional-patronage (`NAT-005`), and — dominating everything from `NAT-006` onward — individual-genius narrative (Copernicus, Carnot, Darwin, Pasteur/Koch, Maxwell, Einstein, Bose, Turing). This isn't wrong — a lot of documented scientific history really does turn on named individuals — but it leaves a real gap at the _sustained institutional_ scale specifically for natural science: not the one-time "Scientific Revolution" method-shift `NAT-007` already covers, but the ongoing, routine machinery (peer review, journal publication, priority disputes) that converts individual claims into a cumulative collective enterprise generation after generation. The domain has civilizations and geniuses; it's thin on the standing institution sitting between them.

### 4. REVERSE-DIRECTION GAP — **FAIL**

Zero entries model knowledge loss, institutional destruction, or a research programme collapsing. This is a sharper failure here than in any other domain audited, because the domain's own stated territory — "how a society comes to know things" — makes losing that knowledge the most on-topic possible reversal, and one entry already gestures directly at it without ever delivering: `NAT-005`'s own opening line frames translation as "the mechanism by which knowledge actually survives a civilizational transition rather than being lost with the society that produced it" — naming loss as the alternative outcome and then never once showing what it looks like. Worse, `NAT-005`'s own subject institution, the Abbasid House of Wisdom, meets a documented, dramatic, well-dated violent end — the 1258 Mongol destruction of Baghdad — that falls just three years past `NAT-005`'s own stated `date_end` of 1200, meaning the entry doesn't even reach far enough to cover its own institution's ending. Grepped `library of alexandria|sack of baghdad|1258`: zero hits anywhere in the wiki.

### 5. PREREQ ORPHANS — **FAIL**

Four concrete cases:

- **`NAT-002`** (prereqs `[]`) has no link to `AGR-006`, which already documents the practical token-and-tally accounting precursor to formal mathematical notation this entry's own Mechanism section assumes existed first.
- **`NAT-010`** (Evolution, prereqs `[NAT-007]`) depends on a concept of geological deep time this wiki has nowhere — Darwin explicitly credited Lyell's uniformitarian geology as foundational to making gradual natural selection over millions of years even plausible, and no geology entry exists anywhere to prereq.
- **`NAT-013`** (Relativity, prereqs `[NAT-004]`) never lists `NAT-012` (Electromagnetism) as a prereq despite special relativity's actual historical origin being the specific puzzle of reconciling Newtonian mechanics with the constancy of light speed in Maxwell's equations — the entry's own Mechanism section never mentions electromagnetism at all, despite it being the central motivating problem.
- **`NAT-014`** (Quantum Theory, `date_start: 1900`) lists `NAT-013` (Relativity, `date_start: 1905`) as a prereq — a chronological inversion of the same kind found and fixed in Commerce and Agriculture: Planck's foundational 1900 blackbody-radiation work predates special relativity by five years and doesn't depend on it. The historically accurate dependency is thermodynamics (`NAT-009`) specifically, since blackbody radiation is a thermodynamics problem — not relativity at all.

### 6. MECHANISM DIVERSITY — **PASS, with one clear gap**

This is genuinely rich: a civic institutional mechanic (`NAT-005`'s patronage-funded canon assembly), three real branch mechanics (`NAT-006`'s contested transmission, `NAT-010`'s validity-versus-acceptance split explicitly reusing design principle 3, `NAT-014`'s settled-math-versus-open-interpretation split), a distinctive paired sequence-order contrast (`NAT-009` theory-follows-practice against `NAT-012` theory-precedes-practice, each entry aware of and citing the other), and clean gates (`NAT-002`, `NAT-004`, `NAT-011`). This comfortably clears the flat-modifier-list failure mode. But **zero entries carry a genuine depleting-pool mechanic** — the same gap found and then fixed in every other domain audited this cycle. Given this domain is _about_ knowledge, and knowledge is exactly the kind of thing that can accumulate, decay, and require maintenance investment, this is a particularly on-the-nose category to have missed.

### 7. BOUNDARY INTEGRITY — **PASS**

The NAT/ENG split is immaculate throughout — every theory entry that has a matching later technology (`NAT-004`/`ENG-009`, `NAT-008`/`ENG-008`, `NAT-009`/`ENG-009`, `NAT-012`/`ENG-018`-`019`, `NAT-013`-`014`/`ENG-021`, `NAT-015`/`ENG-021`) states the split explicitly and never re-derives the artifact side. More than the rules require: `NAT-001` and `NAT-003` both impose a self-directed NAT/PHI discipline (boundary-rules.md doesn't even list this pair) with the same explicit hand-off language used everywhere else. The one soft case worth naming without recommending a change: `NAT-005`'s content (caliphal- and royal-patronage-funded institutional translation work) sits close enough to REL's "institution... endowment" territory that a stricter reading could argue for it, the same way `COM-011` sits close enough to FIN's territory to be worth periodically re-examining — not a violation, just a boundary worth remembering exists.

---

## PART B — Proposed Additions

### LOAD-BEARING (5)

**1. Optical Theory: Refraction, Lenses, and Magnified Observation**

- Regions: middle-east, europe — era, roughly 984 to 1610
- Answers: Probe 2 (substrate) directly, closing `NAT-006` and `NAT-011`'s silent dependencies
- Mechanism: a mathematical theory of how a curved lens bends light to magnify or focus an image converts what any unaided eye can see into what a constructed instrument can reveal instead — the knowledge-side precondition for both the telescopic evidence `NAT-006` needs and the microscopic evidence `NAT-011` needs; grinding and mounting the actual lens is a distinct artifact question this record has nowhere yet (a real gap for a future Engineering pass, not proposed here).
- Hook (seed): a gate on what counts as observable evidence at all — heliocentrism's telescopic confirmation and germ theory's microorganism identification both require this theory (or at least the working practice it explains) to exist first, not just controlled observation in the abstract sense `NAT-007` already covers.
- Prereqs: `NAT-007`. Enables: `NAT-006`, `NAT-011` (both currently missing this link).
- Contested: false.
- Confidence: high on Ibn Sahl's priority, medium on how directly Kepler's later dioptrics theory drew on the Islamic-world work versus reinventing it independently.
- Note: Ibn Sahl's 10th-century Baghdad treatise contains a correct mathematical statement of the law of refraction roughly six centuries before Snell's name became attached to it in Europe — a real, well-documented, currently-uncredited priority case exactly in this record's own established pattern (Maragha/Copernicus, Ibn al-Haytham/Bacon).

**2. Geology and Deep Time**

- Regions: europe, east-asia — era, roughly 1088 to 1830
- Answers: Probe 5 (prereq orphan) directly, closing `NAT-010`'s missing dependency
- Mechanism: uniformitarian geology argues the same slow processes observable today (erosion, sedimentation, gradual uplift), extrapolated over a timescale of millions rather than thousands of years, are sufficient to explain Earth's observed geological features — establishing the vast timescale gradual natural selection requires to be even conceivable, against a prior default assumption of a young Earth shaped mainly by past catastrophe.
- Hook (seed): a gate — `NAT-010`'s gradual, generation-by-generation selection mechanism needs a timescale deep enough for small effects to accumulate into large ones, and nothing in this record currently supplies that timescale as an available concept.
- Prereqs: none. Enables: `NAT-010` (currently missing this).
- Contested: false.
- Confidence: high.
- Note: Song-dynasty polymath Shen Kuo's 1088 observation of marine fossil shells embedded in mountain rock, correctly interpreted as evidence of ancient uplifted seabeds, predates Hutton's and Lyell's uniformitarian synthesis by seven centuries — a real, specific, independently-arrived-at case worth crediting on its own terms rather than as a footnote to the European sequence.

**3. Probability and Statistical Reasoning**

- Regions: middle-east, europe — era, roughly 800 to 1900
- Answers: Probe 2 (substrate) directly — a foundational mathematical tool silently assumed by entries this domain already has
- Mechanism: converts uncertain, variable outcomes from something judged only qualitatively into something quantified as a distribution and reasoned about systematically — the mathematical machinery `NAT-009`'s statistical-mechanics reading of entropy and `NAT-014`'s inherently probabilistic quantum formalism both depend on existing first.
- Hook (seed): a gate this domain has been assuming since at least `NAT-009` — treating entropy or quantum indeterminacy as fundamentally statistical requires a mathematics of probability to already exist, which nothing in this record currently establishes.
- Prereqs: `NAT-002`. Enables: `NAT-009`, `NAT-014` (both currently missing this link).
- Contested: false.
- Confidence: medium — the mechanism and the European 17th-19th-century sequence (Pascal, Fermat, Gauss, Boltzmann) are solid; my confidence in the precise scope of the Islamic-world contribution below is more circumstantial than for the other load-bearing proposals.
- Note: al-Kindi's 9th-century development of frequency analysis for breaking substitution ciphers is widely credited as an early, genuine instance of statistical reasoning — using the known relative frequency of letters in a language to infer a hidden pattern — centuries before European probability theory's classical development.

**4. The Scientific Society and Peer Review**

- Regions: europe — era, roughly 1660 to 1900
- Answers: Probe 3 (scale gap) and Probe 6 (civic mechanic) directly
- Mechanism: converts an individual's claim about the natural world into one validated (or rejected) by a standing community of specialists through submission, critique, and publication — a routine, repeatable institutional process distinct from `NAT-007`'s one-time argument that this method should exist at all, and the actual sustained machinery every entry from `NAT-008` onward silently assumes keeps functioning.
- Hook (seed): a civic mechanic — creates and sustains a knowledge-validating institution whose own internal disputes (the Newton–Leibniz calculus priority conflict is the paradigm case) are as mechanically real as the science it validates, since priority and credit within the institution can be contested independently of whether the underlying claim is correct.
- Prereqs: `NAT-007`. See_also: none strong — see note below on why this entry resists an easy non-European comparison.
- Contested: false.
- Confidence: high.
- Note on region balance: I looked for a genuine non-European parallel institution to this specific combination (open publication, adversarial peer critique, priority-by-publication-date) and didn't find one strong enough to include honestly — Islamic-world knowledge transmission (the ijaza personal-license system) and Chinese imperial academies both sustained serious scholarly communities through different, non-adversarial-peer-review mechanisms, which belongs in this entry's `absence` field as a genuine structural difference, not a gap to paper over with a forced comparison.

**5. The Sack of Baghdad and the Loss of the House of Wisdom**

- Regions: middle-east — decade precision, 1258
- Answers: Probe 4 (reverse-direction) directly — the domain's strongest available case, continuing an entry (`NAT-005`) it already has past the point that entry's own date range stops short of
- Mechanism: the Mongol conquest of Baghdad in 1258 ends the House of Wisdom's centuries-long translation-and-extension project (`NAT-005`) abruptly through violent institutional destruction rather than gradual decline or succession — a knowledge-producing institution's accumulated capacity doesn't merely stop growing, it's destroyed outright, with the scale of the actual loss genuinely disputed rather than as total as the popular "books thrown in the Tigris until it ran black with ink" account suggests.
- Hook (seed): a depleting-pool event — an `accumulated scientific corpus` pool that institutions like the House of Wisdom build up over generations can be destroyed in a single violent event rather than only fading through neglect, with recovery (where it happens at all) requiring renewed investment from a different institutional base entirely.
- Prereqs: `NAT-005`. See_also: none needed.
- Contested: true — the degree of actual intellectual loss is disputed among historians; the popular account significantly postdates the event and may exaggerate both the volume of material destroyed and how uniquely catastrophic it was for Islamic scientific activity overall, given serious scholarship continued for centuries afterward in Cairo, Persia, and later Ottoman, Safavid, and Mughal courts. Model the institutional destruction itself as certain while treating the magnitude of the resulting knowledge loss, versus the field's actual centers of gravity having already been dispersing beyond Baghdad before 1258, as a genuinely open historical question.
- Confidence: high on the event, medium on the loss-magnitude question the Contested section turns on.

### VALUABLE (3)

**6. Taxonomy and Systematic Classification**
Regions: europe, east-asia. Century, roughly 1550–1758. Answers Probe 6 (a distinct categorization-as-knowledge-tool mechanic, different from `NAT-001`'s framework-setting mechanic) and sets up `NAT-010` more directly than `NAT-007` alone does — a classification system that reveals patterns of similarity and difference across species is what evolutionary theory then needs to explain causally. Mechanism: a systematic, hierarchical naming and classification scheme converts scattered individual natural-historical observation into a shared, comparable, cumulative record any later observer can extend without renegotiating the whole system from scratch. Hook: a modifier on how comparable and extensible natural-historical knowledge becomes across different observers and generations. Prereqs: none. See*also: `NAT-010`. Contested: false. Confidence: high. Note: Li Shizhen's 1578 \_Bencao Gangmu*, a massive systematic materia medica classification of nearly 1,900 medicinal substances, predates and parallels Linnaeus's later European system, developed under a substantially different organizing logic (medicinal use and property rather than reproductive structure) reaching comparable practical rigor.

**7. Mendeleev's Periodic Table and Predictive Classification**
Regions: europe. Decade, 1869. Answers Probe 6, adding a genuinely distinct mechanism from Taxonomy's descriptive classification: a classification system organized correctly enough to predict the existence and properties of things not yet observed. Mechanism: arranging known elements by atomic weight and chemical behavior reveals a periodic pattern with gaps — Mendeleev's specific achievement is treating those gaps as predictions (undiscovered elements with calculable properties) rather than as errors in the data, and being vindicated when several were found within his own lifetime matching his predictions closely. Hook: a distinct predictive-gate mechanic — a correctly-structured classification doesn't just organize existing knowledge, it generates falsifiable claims about what hasn't been found yet. Prereqs: `NAT-008`. Contested: false. Confidence: high.

**8. Plate Tectonics and the Delayed Vindication of Continental Drift**
Regions: europe, americas. Era, 1912–1965. Answers Probe 6, contributing a mechanism this domain doesn't otherwise have: a scientifically correct theory rejected by the professional establishment for want of a plausible mechanism, rather than for religious or cultural reasons (`NAT-006`, `NAT-010`'s pattern). Mechanism: Wegener's 1912 continental-drift proposal, based on genuine geological and fossil-distribution evidence, is widely dismissed for decades specifically because he couldn't supply a physical mechanism capable of actually moving continents — plate tectonics, developed independently from mid-century seafloor-spreading evidence, finally supplies that missing mechanism and vindicates the original observation. Hook: a branch distinct from `NAT-006`/`NAT-010`'s religious-resistance pattern — correct evidence without a plausible causal mechanism can be professionally rejected for decades on genuinely scientific grounds, not merely institutional or religious ones. Prereqs: the proposed Geology entry (#2). Contested: false. Confidence: high.

### OPTIONAL (2)

**9. The Antikythera Mechanism** [REJECTED]
Flavor entry on the single, extraordinary artifact (an ancient Greek geared analog astronomical computer, c. 150–100 BCE) recovered from a shipwreck in 1901. Real and genuinely startling history — nothing of comparable mechanical complexity is known to exist again for over a thousand years — but its broader significance (ancient astronomical and mechanical sophistication) is already covered by `NAT-002` and `NAT-004`, and as a single artifact it doesn't generalize into its own mechanism. Naming it because it's a great story, not because the domain needs it.

**10. The Rosetta Stone and the Decipherment of Hieroglyphs** [REJECTED]
Flavor entry on the 1799 discovery and Champollion's 1822 decipherment, the recovery side of this record's own knowledge-loss theme rather than the loss side (#5) — real, narrow, single-event history whose actual consequence (recovering access to a body of ancient Egyptian scientific, religious, and administrative texts) is more a historiographical event than an ongoing mechanic this domain's model would need to track.

---

## PART C — Proposed Modifications

### SPLIT

**NAT-015 (Computer Science and Computation Theory)** bundles three genuinely separate intellectual achievements 1,128 years apart under one Mechanism section: al-Khwarizmi's algorithm concept (820), Turing's computability theory (1936), and Shannon's information theory (1948). The first two are legitimately continuous — Turing's formalization is a direct, traceable answer to exactly the "precise step-by-step procedure" question al-Khwarizmi's work first poses — but Shannon's entropy-of-information and channel-capacity work is a mathematically distinct achievement currently reduced to a single throwaway clause in the Timing section and never developed in Mechanism, Social consequence, or Regional specifics at all. Split into:

- **NAT-015 retained**: the algorithm concept through computability theory (820–1936) — al-Khwarizmi and Turing, a genuinely unified lineage.
- **A new entry**: Information Theory (Shannon, 1948) — quantifying information itself, entropy as a measure of uncertainty, and channel capacity as a hard limit on communication — a distinct mechanism (a gate on how much information any channel can carry regardless of engineering cleverness, structurally similar in shape to `NAT-009`'s thermodynamic efficiency ceiling) that deserves its own Mechanism section rather than a clause.

### MERGE

None found. `NAT-009` and `NAT-012` are a deliberate mirror-image pair (theory-follows-practice against theory-precedes-practice), each explicitly citing the other for contrast — considered as a possible merge and rejected, since collapsing them would destroy the comparison that's actually the more interesting content. No other pair shares one mechanism closely enough to justify combining.

### RE-DOMAIN

No entry's canonical `domain` needs to change — this domain's NAT/ENG discipline is the cleanest of any domain audited. Two concrete missing-or-wrong link fixes:

- **NAT-013** — add `NAT-012` to `prereqs`. Special relativity's actual historical motivation was reconciling Newtonian mechanics with Maxwell's electromagnetism specifically; the entry's own Mechanism section never mentions electromagnetism despite this.
- **NAT-014** — remove `NAT-013` from `prereqs` (a chronological inversion: quantum theory's 1900 starting point predates relativity's 1905 by five years) and replace it with `NAT-009` (Thermodynamics), since Planck's founding blackbody-radiation work is a thermodynamics problem specifically, not a relativity-dependent one.

### RETIER

All 15 entries currently carry `tier: core` — the most undifferentiated tier field of any domain audited (worse than Commerce's original 16-for-16, since Commerce at least had 16 entries to Nature's 15 and this is a smaller, tighter set that should show even more editorial differentiation, not less). Two candidates for downgrade to `standard`:

- **NAT-008** (The Chemical Revolution) — `enables: []`, a strong and complete entry, but nothing else in this domain is structurally built on top of it.
- **NAT-013** (Relativity) — `enables: [NAT-014]` currently, but that link is being removed above as a chronological error, leaving it with nothing downstream; separately, its Hook is the most purely narrative one in the domain ("a demonstration that consensus remains revisable") rather than a crisp gate, branch, or modifier the way `NAT-009` or `NAT-010` manage with comparable subject matter.

Not touching the other 13 — each has a real downstream `enables` entry or a load-bearing cross-domain dependency (`NAT-004`, `NAT-011`, `NAT-012` especially) that earns the weight on its own terms.

### CUT

**None.** I read all 15 entries specifically looking for a self-admitted redundancy (the kind `COM-019` and `ENG-015` supplied in their own audits) or a genuinely duplicated mechanic, and didn't find one I could defend without inventing a problem the text doesn't actually have. The weakest entry by mechanism specificity, `NAT-013`, still has a real, distinct historical achievement behind it — it's a RETIER case, not a CUT case, and I'd rather report that honestly than force a cut to satisfy the expectation that one exists. This domain's problems are absence and mislinked chronology, not padding.

---

## PART D — Verdict

**Worst structural problem:** Europe and the Mediterranean make up 42.9% of this domain's regional footprint, past the 40% line this audit is explicitly told to flag — not from any single dishonest entry, but because seven of fifteen entries cover the 1661–1935 European classical-physics sequence, a real historical concentration this record credits honestly at the level of individual entries but never counterbalances at the level of which topics got chosen to have entries at all.

**Entry count:** 15 before. With all five LOAD-BEARING proposals and the one SPLIT: 15 + 5 + 1 = **21**. With VALUABLE also adopted: 24. With OPTIONAL: 26.

**Proposed out of symmetry, not mechanical need:** entries #9 (Antikythera Mechanism) and #10 (Rosetta Stone), both OPTIONAL and named as such — both are single-artifact or single-event stories whose broader significance the domain already covers elsewhere, included because they're genuinely good history the domain's own knowledge-loss-and-recovery theme (#5) makes newly relevant, not because the model needs them.

**Does Natural Sciences justify separate existence?** Yes, clearly — its NAT/ENG split is the reason Engineering's own entries can stay disciplined about technique-versus-theory, its self-imposed NAT/PHI discipline shows the boundary logic generalizing usefully beyond what the rules formally require, and its mechanism diversity (civic, branch, paired sequence-contrast, gate) is real. The problems here are the most fixable kind: a regional-balance skew traceable to a specific, nameable topic cluster rather than a structural default, a chronology bug of a type this project already knows how to catch, and a reverse-direction gap that — unusually — the domain's own existing text already names and just needs to follow through on.
