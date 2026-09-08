# Cross-Domain Synthesis — 2026-09-08

Read: all 13 files in `_meta/audits/` (agriculture, commerce, diplomacy,
engineering, finance, health, law, maritime, migration, natural-sciences,
philosophy, religion, war — the brief says twelve; thirteen single-domain
audits actually exist on disk, and all thirteen are used below), plus a
structural pass over the full corpus (currently 379 entries across all 13
domains), plus targeted `grep` verification of every specific claim below
against the entries as they actually exist today, not as any one audit
described them at the time it was written.

**A note on provenance, because it matters for how to read this document.**
Most of the single-domain audits' own recommendations have already been
implemented — current entry counts (AGR 28, COM 23, ENG 33, FIN 27, LAW 23,
MAR 37, MIG 28, NAT 24, WAR 33) match each audit's own projected post-
load-bearing-and-valuable totals almost exactly, arithmetic confirmed
against each audit's Part D. PHI, REL, DIP, and HLT were audited and fully
implemented within this session specifically. That means this document is
not reviewing twelve or thirteen wish lists — it's reviewing a wiki where
most of those wishes already came true, independently, domain by domain,
with no domain able to see what the other twelve were simultaneously
building. That's exactly the condition under which cross-domain duplication
and cross-domain blind spots become likely, and exactly what the seven
questions below are built to catch.

---

## 1. HOMELESS ENTRIES

Two confirmed, verified against the current corpus by direct search — not
speculation about what might be missing, but things a specific domain's own
audit explicitly declined to claim, that no other domain's audit picked up
either.

**Optical instrument manufacture**
— lens-grinding and telescope/microscope construction as an engineering artifact. Nature's own audit proposed and
implemented `NAT-016` (Optical Theory: Refraction, Lenses, and Magnified
Observation) — confirmed to exist — but its own text draws the boundary
explicitly: _"grinding and mounting the actual lens is a distinct artifact
question this record has nowhere yet (a real gap for a future Engineering
pass, not proposed here)."_ Engineering's own audit, written independently,
never mentions lenses, optics, telescopes, or microscopes anywhere in its
seven-item gap list. Confirmed by direct search: zero hits for
`lens|optic` anywhere in `domains/engineering/`. NAT-016 covers the
mathematics of refraction; nothing anywhere covers the actual craft of
grinding a lens to a specified curvature, the yield-and-quality problem
that made a usable telescope objective genuinely hard to produce well into
the 18th century, or why Newton's reflecting telescope (sidestepping the
chromatic-aberration problem lens-grinding quality couldn't yet solve) was
a real engineering response to a real manufacturing ceiling. This is a
clean, load-bearing gap: propose an ENG entry, "Precision Lens Grinding and
Optical Instrument Manufacture," prereqing `NAT-016`, enabling nothing
further required but see-alsoing `NAT-006` (Heliocentrism) and `NAT-011`
(Germ Theory), both of which depend on the artifact NAT-016's own theory
alone doesn't build.

**The Late Bronze Age Collapse as a military, not only commercial, event.**
Commerce's audit proposed and implemented "The Late Bronze Age Trade
Collapse," correctly scoped to the commercial-network-fragility mechanism —
but its own `see_also` note hedges explicitly: _"for the contemporaneous
'Sea Peoples' disruptions, if/when a WAR entry covers them."_ War's own
audit, also written independently, never mentions the Sea Peoples, the
Bronze Age collapse, or anything in that era's military history at all —
War's earliest entry is `WAR-001` (formerly -750, corrected to -900), a
full two and a half centuries after the collapse COM-013's successor
already covers commercially. This is smaller than the optics gap (COM's own
entry substantially covers the causally interesting part — network
fragility under specialization) but it's a real, specifically-hedged and
specifically-unclaimed loose end: the actual military disruption (large-
scale raiding fleets, the fall of Hattusa, Ugarit's destruction) that
COM-013's successor cites as a contributing cause has no WAR-side account
of its own. Lower priority than the optics gap; worth a line in WAR's next
pass rather than a standalone proposal here.

**A softer, third case worth naming rather than proposing:** Law's own
audit built `LAW-017` (Civil Law: Property, Contract, and Inheritance)
specifically to ground the `contract enforcement` stat that Commerce and
Finance entries had been invoking without a legal foundation for two
audit cycles. It exists. But checked directly against every COM and FIN
entry that touches `contract enforcement` in its own stats (`COM-015`,
`COM-012`, `COM-029`, `FIN-021`, `FIN-005`) — not one of them links to
`LAW-017` in either `see_also` or `prereqs`. This isn't a homeless entry in
the sense of not existing; it's a **built-but-unclaimed** entry — the
foundation got poured and nobody built on it, because Law's own audit had
no way to see, and no mandate to fix, the five entries scattered across two
other domains that were the actual reason it needed to exist. This is
recorded here rather than under Boundary Failures because it isn't a rule
problem — it's a visibility problem no single-domain audit could have
caught by construction, which is this whole document's reason to exist.
Fix: add `LAW-017` to `see_also` on all five entries, reciprocally.

---

## 2. BOUNDARY FAILURES

`boundary-rules.md` has explicit sections for COM/FIN, WAR/ENG, NAT/ENG,
REL/PHI, LAW/DIP, MIG/all, and MAR/(WAR,ENG,COM,FIN). It has no section for
**LAW/WAR**, and the gap is no longer theoretical — it produced a real
duplicate (see §4 below, Finding 1). It also has no section for **HLT** at
all, which this session's own Health audit already flagged and partially
fixed (an HLT section now exists, added during that domain's
implementation) — worth confirming here that the fix landed and is doing
its job: checked, `boundary-rules.md` now contains an HLT section. Good.

**Proposed new section, LAW / WAR:**

> ## LAW / WAR
>
> - **LAW (law)** owns the land-tenure and inheritance bond itself — who
>   holds land under what obligation, how that obligation is legally
>   codified and passed down, and how it compares across jurisdictions as an
>   institutional form.
> - **WAR (war)** owns the resulting military capability and doctrine —
>   how an army raised this way fights, how its social culture (honor
>   codes, hereditary warrior class identity) differs from a salaried
>   force, and how that capability compares to alternative manpower
>   models.
>
> A feudal land grant in exchange for service is LAW. The knight or samurai
> class's own battlefield doctrine and honor-bound military culture built
> on top of that grant is WAR. Where an entry's own comparative material
> (which regions, which specific institutions, which historical dates) is
> identical to another entry's under this split, that's a sign the two
> should be one entry with two `domains_touched`, not two independently-
> written entries that happen to agree.

That last sentence is aimed directly at `WAR-024`/`LAW-005` — see §4.

**A second, softer boundary observation, not a rule failure:** the REL/PHI
split (institution vs. argument) and the NAT/ENG split (knowledge-method
vs. artifact) both produced real, load-bearing content splits this cycle
(REL-026/PHI-025 on State Shinto; the whole NAT-005/ENG chain) — and both
splits worked because each domain's own audit, written independently,
landed on the _same_ division of labor without being told to. LAW/WAR
never got that alignment because neither domain's audit knew the other was
about to write feudalism. This isn't a case for a stricter rule so much as
a case for what this document is actually for: the rule can be written
correctly and still fail in practice if nobody checks whether two
domains actually converged on it.

---

## 3. DOMAIN VIABILITY

The brief names Diplomacy and Migration as the expected candidates. Checked
both directly, plus Maritime as the third "carved out of other domains"
case worth the same scrutiny by the same logic.

**Migration: keep, and more confidently than almost any other domain
checked.** Both the original Migration audit and this domain's own later
independent re-audit (written separately, months apart in project time,
never seeing each other) reached the identical verdict in almost the
identical words: _"justifies separate existence... more clearly than any
domain audited this cycle."_ `boundary-rules.md` itself enshrines MIG's
premise (movement as a process with its own mechanics) ahead of every
other carve-out rule in the document. Practically: MIG entries are
cross-referenced from AGR, COM, DIP, ENG, HLT, LAW, PHI, REL, and WAR —
more inbound cross-domain dependency than any domain except possibly
Health. The brief's assumption that Migration is a natural fold candidate
is the one place in this synthesis where the evidence actively disagrees
with the premise, and it's worth saying so plainly rather than hedging.

**Diplomacy: keep, but this is the domain actually worth watching.** DIP's
own audit is the one candid dissent in the whole audit series: _"the
honest verdict is that its problem isn't missing content so much as
missing connective tissue... A domain this size, this disconnected, and
this uniformly tiered would be vulnerable to a fold-into-LAW argument if
its existing entries weren't already this well-differentiated from LAW's
actual territory."_ That's still true after implementation — DIP grew from
12 to 20 entries, the smallest of any domain in the wiki by a wide margin,
and its own mechanic (inter-polity recognition and relation) genuinely
doesn't reduce to LAW's internal-obligation territory or WAR's organized-
violence territory. Keep it, but if this project ever needs to consolidate
domains under resource pressure, DIP is the one where that conversation
should start, not MIG.

**Maritime: keep, not seriously in question.** Carved out of WAR, ENG, COM,
and FIN by explicit rule, and by its own audit's account the _best_-
constructed domain checked on several measures (region balance, prereq-
chain elegance, self-documented boundary discipline). The carve-out is
working as designed.

**One domain nobody asked about that's worth a half-sentence:** Health.
Not a fold candidate — the opposite risk. HLT is cross-referenced by nine
other domains (per its own audit's count) and now has no formal boundary
section conflict, but it's carrying enough of the wiki's connective tissue
that its own future audits should probably get first priority the next
time this project has synthesis-pass capacity, simply because more of the
wiki depends on it staying coherent than depends on any other single
domain.

---

## 4. DUPLICATE MECHANISMS

Two confirmed, both verified by reading the actual entry text on both
sides, not just the titles.

**Finding 1 — `WAR-024` (Feudal Land-Tenure Military Service) and `LAW-005`
(Feudalism and Vassalage).** These are not two angles on one phenomenon;
they are the same entry written twice. Both cover: land granted in
exchange for service as the core mechanism; the European case; the
Japanese case (WAR-024's region_variant: _"Japanese shoen estate holders
grant land to warrior retainers in exchange for military service, maturing
into the samurai class's own hereditary bushido warrior code"_ — LAW-005's
region_variant: _"Japanese feudalism under successive shogunates
formalizes a comparably rigorous lord-vassal legal bond... the samurai's
obligation to their daimyo"_); the same "developed with no direct contact
between the two traditions" convergent-evolution framing, twice, almost
verbatim. Checked directly: neither entry's body text or frontmatter
mentions the other's ID anywhere, in either direction — confirmed by
direct `grep`. LAW-005 additionally covers the Roman colonate and the
Ethiopian gult system as region_variants WAR-024 doesn't touch, and WAR-024
covers the Islamic iqta system LAW-005 doesn't — so there is real,
non-overlapping comparative content on both sides, which is exactly why
this should be a merge with two `domains_touched` rather than a straight
deletion. **Recommend:** merge into one entry, canonically LAW (the
land-tenure bond is the more foundational half; the military-doctrine
consequence is the derived one, matching the REL/PHI and NAT/ENG precedent
of institution-before-consequence), carrying `domains_touched: [war,
agriculture]`, with WAR-024's iqta region_variant and its distinct
"honor-bound military culture" Social Consequence material folded in as
additional region_variant and paragraph content. WAR-024's ID retires per
`conventions.md`; WAR domain entries that currently `see_also` WAR-024
(if any — check before retiring) get re-pointed to LAW-005.

**Finding 2 — `FIN-022` (General Insurance and Actuarial Risk-Pooling) and
`HLT-033` (Actuarial Life Insurance and the Pricing of Mortality Risk).**
Both entries cover the identical mechanism — Halley's mortality table
converting age-specific death probability into a sellable, priced
insurance premium — as their central claim. FIN-022's own Timing section:
_"life insurance matures from the 1750s-1760s as actuarial science
develops reliable mortality tables letting insurers price a policy against
statistical rather than guessed life expectancy."_ HLT-033's opening
sentence: _"Edmond Halley's 1693 mortality table converts age-specific
death probability into a calculable, sellable price, letting a commercial
insurer profitably underwrite individual mortality risk for a premium."_
Same mechanism, same historical anchor, two different entry points into
the archive. Checked directly: FIN-022's `see_also` is `[MAR-011]` only —
no HLT reference; HLT-033's `see_also` is `[HLT-002, HLT-029]` — no FIN
reference, despite `domains_touched: [finance]` already being declared on
the HLT side. Unlike the feudalism case, this pair genuinely doesn't need
both halves: FIN-022's own _fire_-insurance content (the 1666 Great Fire,
property risk-pooling) is real, distinct, and unrelated to mortality
pricing specifically, and needs to survive. **Recommend:** retain FIN-022
as the canonical entry for the general risk-pooling mechanism (fire
insurance and life insurance both), fold HLT-033's mortality-table-specific
material into FIN-022's existing life-insurance Regional-specifics content
as elaboration rather than duplication, and retire HLT-033 — but keep a
`see_also: [FIN-022]` pointer on `HLT-029` (Mutual Aid and Guild
Sick-Funds), since that entry's own comparison to individually-priced
insurance is real and worth preserving even after HLT-033 itself retires.

**Considered and rejected as a duplicate:** `COM-028` (Caravan as
Risk-Pooling Institution) and `MIG-025` (Camel Caravan Mobility and Desert
Crossing) look like the same topic at a glance — both "caravan," both
desert trade — and are not: COM-028 covers merchants pooling capital and
guards as a _financing and risk-sharing_ institution (explicitly prereqing
a pack-animal mobility entry rather than re-deriving it), MIG-025 covers
the camel's own physiology as a _mobility-threshold_ unlock (explicitly
parallel to MIG-001's horse entry). This is the correct split — a
commercial institution built on top of a transport technology, the same
COM/MIG relationship that already works cleanly for COM-014/MIG-001 on the
steppe side. Confirmed both entries exist and are correctly differentiated
on inspection; no action needed.

---

## 5. THREADS

Five topics span 4+ entries across 3+ domains and would benefit from a
`_meta/threads/` links-only page rather than continued ad hoc
cross-referencing. (A sixth candidate exists already: `_meta/threads/
axial-age.md`, created during this session's Philosophy implementation —
confirming the mechanism works and these five are genuine gaps in using
it, not a novel proposal.)

1. **Feudalism and land-for-service obligation.** `LAW-005`/`WAR-024`
   (post-merge, one entry), `AGR-012` (Roman colonate, the direct
   institutional precursor LAW-005's own region_variant already names),
   `AGR-015` (serfdom), `AGR-020` (enclosure, the institution's eventual
   dissolution). Three domains (LAW, WAR pre-merge, AGR), five entries pre-
   merge / four post-merge. The strongest candidate on this list — it's
   also the one domain-boundary failure this document found a concrete
   bug from, so the thread page and the merge should land together.

2. **Colonization of the Americas.** `MIG-004`/`MIG-005`/`MIG-012`,
   `DIP-007`, `HLT-017`, `AGR-018`, touching `COM-011` (silver) and WAR's
   own conquest-doctrine content. Six domains, seven-plus entries. This one
   is self-nominated: MIG-012's own text already says, verbatim, _"a
   strong future candidate for a `_meta/threads/` page once those other
   domains have entries of their own to link together."_ Those entries now
   exist. The condition MIG-012 itself set has been met.

3. **Trade-network connectivity and disease transmission.** `HLT-012`
   (Connectivity Plagues), `HLT-015` (Black Death), `COM-014` (Silk Road,
   which HLT-012 already cites for the same route), `MIG-008` (Steppe
   Waves, the Mongol-peace trade integration HLT-012's own Black Death
   case traces), and touching `AGR`'s land-abandonment consequences. Three
   to four domains depending on how the AGR link is counted, four-plus
   entries, and a genuinely two-directional relationship (the same route
   knowledge that HLT-012 tracks as pandemic-speed is COM-014's own
   commercial-throughput stat) that a thread page would make visible in
   one place instead of requiring a reader to reconstruct it from two
   separate entries' prose.

4. **Print, vernacular access, and institutional authority.** `HLT-013`
   (Print, Silent Reading, and the Interior Self), `REL-016` (Printed
   Religion), `ENG-006` (Printing Press), and PHI's own Reformation-
   adjacent entries (Renaissance Humanism prereqs PHI-015 which prereqs
   ENG-006 and NAT-005 already). Four domains, four-plus entries, and a
   case where three different domains each independently built a "what
   cheap print did to X's authority structure" entry without a map tying
   them together.

5. **Institutional confinement and reform.** `HLT-030` (The Asylum),
   `LAW-023` (Codified Criminal Law and Punishment Reform, the
   penitentiary/Beccaria entry), `REL-009` (Monasticism, the earlier
   custodial-institution precedent both later entries structurally
   echo), and `HLT-014` (Monastic Hospitals). Confirmed by direct check:
   `LAW-023` and `HLT-030` currently have zero cross-reference despite
   covering the same 18th-19th-century reform-institution wave (Beccaria's
   punishment-philosophy shift and the moral-treatment asylum movement are
   near-contemporaneous and frequently discussed together in the actual
   historiography) from two different target populations. Smallest of the
   five, but the clearest immediately-actionable link-gap of the set —
   this one is a same-day fix even without the full thread page.

---

## 6. SYSTEMIC SKEW

Three patterns visible only in aggregate — none of them would show up in
any single domain's own Part A, because each domain's own audit correctly
checked its own numbers and moved on.

**The prose-to-frontmatter citation gap is the single most repeated
finding in this entire project, named independently in nearly every one of
the thirteen audits, usually in close to identical language** ("the same
specific bug found in every domain audited this cycle"). COM, ENG, FIN,
LAW, MAR, MIG, NAT, and WAR's audits all separately catch entries that name
a cross-domain ID directly in their own body prose and never promote that
citation into `prereqs`, `enables`, or `see_also` — AGR alone found it five
separate times in one domain. This isn't a content gap at all; it's a
**consistent authorial habit** — whoever wrote these entries thinks in
narrative citations first and back-fills the structured link fields
second, and reliably loses a fraction of the citations in the back-fill,
across thirteen independently-written domains with no other trait in
common. Worth naming as the project's actual single point of process
failure, distinct from any specific missing entry: a mechanical post-pass
that greps every entry's own body for `[A-Z]{2,4}-\d{3}` patterns and
diffs against that entry's own three link fields — which several of this
session's own domain implementations already ran ad hoc during validation
— would catch the majority of this in one pass if run project-wide rather
than domain-by-domain.

**A recurring, independently-discovered scale gap: five different
domains' own audits, with no coordination, all located the same missing
rung on the same ladder.** Commerce found zero entries at ordinary
market/household scale. Law (original, pre-implementation) found zero
household-scale entries. Maritime found the sailor's own labor experience
missing beneath every institutional entry. War found the individual
soldier's combat motivation missing beneath every doctrinal entry.
Engineering found the pre-industrial household/workshop scale nearly
empty. Every one of these was independently framed as "the domain jumps
from [state/institutional grandeur] to [state/institutional grandeur] with
almost nothing at human scale in between." Each domain fixed its own
instance (COM added retail and putting-out; MAR added impressment and the
maritime labor force; WAR added small-unit cohesion). But the fact that
five unrelated domains independently diagnosed the identical shape of gap
suggests the wiki's founding design leans state/institutional by default
in a way no single domain's fix actually addresses at the project level —
worth a standing editorial reminder for any future domain audit, not
because any current gap remains unfixed, but because the next domain
audited will very likely find the same rung missing a sixth time.

**A temporal skew that survives every individual domain's own regional-
balance check.** Nature's audit found the sharpest single number (42.9%
Europe+Mediterranean, over its own 40% flag line) and correctly traced it
to one topic cluster (the 1661-1935 classical-physics sequence) rather
than systemic bias — every individual entry in that cluster credits non-
European contributions honestly where they exist. But the same 1850-1950
window recurs as the highest-density, most Europe/Americas-concentrated
period across Health (antisepsis through the therapeutic turn), Engineering
(the machine-tool/steel/semiconductor chain), War (the mechanized-warfare
sequence), and Finance (the gold-standard-through-Depression run) — five
domains independently densifying their own entry count in the identical
century, for the same underlying reason (this is where the best-documented
history concentrates for genuinely contingent reasons: industrialization's
actual geographic center of gravity). No single domain is wrong to cover
its own slice of this thoroughly. But a player moving through this wiki in
date order will experience a five-domain-wide acceleration and Europe/
Americas concentration across exactly one century that no individual
domain's own audit was positioned to notice, because "is my domain's
regional balance acceptable" and "does the whole project's density spike
in the same hundred years across five domains at once" are different
questions.

---

## 7. TOTAL LOAD

Summed directly from each audit's own Part B tier counts (load-bearing +
valuable + optional, not counting the extra IDs that Part C splits
generate separately):

| Domain      | LB     | Valuable | Optional | Total proposed |
| ----------- | ------ | -------- | -------- | -------------- |
| Agriculture | 4      | 3        | 2        | 9              |
| Commerce    | 5      | 4        | 2        | 11             |
| Diplomacy   | 4      | 3        | 2        | 9              |
| Engineering | 6      | 3        | 2        | 11             |
| Finance     | 4      | 3        | 2        | 9              |
| Health      | 5      | 3        | 2        | 10             |
| Law         | 5      | 3        | 2        | 10             |
| Maritime    | 5      | 3        | 2        | 10             |
| Migration   | 4      | 3        | 2        | 9              |
| Nature      | 5      | 3        | 2        | 10             |
| Philosophy  | 6      | 3        | 2        | 11             |
| Religion    | 4      | 3        | 2        | 9              |
| War         | 5      | 4        | 2        | 11             |
| **Total**   | **62** | **42**   | **26**   | **130**        |

130 proposed additions is not a manageable to-do list for a solo project if
read as one flat backlog — but reading it that way would misdescribe where
this project actually stands. As the provenance note at the top of this
document says: roughly 100 of these 130 are **already built**, confirmed
by the current per-domain entry counts matching each audit's own projected
post-load-bearing-and-valuable total. What's actually left open is much
smaller and much more specific than "130 items":

- The optional tiers that were explicitly `[REJECTED]` and correctly
  stayed that way — not open load, closed decisions.
- The two homeless entries in §1 (one real proposal — optical instrument
  manufacture; one minor hedge worth a line, not a full entry — the Sea
  Peoples military angle).
- The five link-and-domains_touched fixes for LAW-017 in §1.
- The one boundary-rules.md section in §2.
- The two duplicate-mechanism merges in §4 (each a same-day fix: fold
  content, retire one ID, redirect inbound references).
- The five thread pages in §5, one of which (colonization of the
  Americas) was already pre-approved by its own future host entry.

**If this project has one afternoon of cross-domain-synthesis capacity
before returning to per-domain work, that's the actual 20%: the LAW/WAR
boundary section, the two merges it motivates, and the colonization-of-
the-Americas thread page MIG-012 already asked for by name.** Those four
items are the highest-confirmation-density findings in this entire
document — each is independently corroborated by two or more domains'
own text, not just this synthesis's own judgment — and they're the only
items on this list that fix something actually broken (a real duplicate,
a real unclaimed rule gap) rather than adding something merely absent. The
five thread pages beyond that first one, and the single optical-
manufacture entry, are real but genuinely optional in the sense the
individual audits themselves use the word: good, honest, worth doing, and
the project is not structurally incoherent without them.
