# Computation Chain — Gaps Flagged

**Date:** 2026-09-12
**Context:** recorded during the NAT-060 split into NAT-014 (The Algorithm as
Delegable Procedure) and NAT-215 (Computability and the Limits of Formal
Systems).
**No entries written.** Flags only.

---

## 1. The missing middle: mechanical and electromechanical computation

The corpus has the algorithm (`NAT-014`, from -1800), it has the theory of
computability (`NAT-215`, 1900-1936), and it has semiconductors (`ENG-330`,
1947). **Between the abacus and the transistor there is nothing.** Roughly
three centuries of mechanical and electromechanical computation are absent:

- Pascal's calculator (1642) and Leibniz's stepped reckoner (1673) — the
  arithmetic operation embodied in gearing.
- The Jacquard loom (1804) — punched cards as stored instructions, and the
  first case of a procedure held in a medium separate from both the operator
  and the machine.
- Babbage's difference and analytical engines, and Lovelace's 1843 notes —
  the general-purpose programmable machine specified and not built, with the
  clearest early statement that such a machine operates on symbols rather
  than quantities.
- **Hollerith's 1890 census tabulator** and the data-processing industry it
  founded, running through the punched-card firms to IBM.
- Colossus (1943) and ENIAC (1945).

### Why the Hollerith case is the priority

It connects directly to the state-enumeration material established in the
`WAR-200` conscription work. `WAR-200` now argues that conscription is gated
on `legibility` — a state cannot conscript a population it cannot enumerate —
with `FIN-020`, `REL-210` and `LAW-110` as its registration prereqs. The 1890
US census tabulator is **a machine built to process exactly that kind of
record**, and it marks the point at which enumeration stops being bounded by
clerical labour. Every register in that chain was previously limited by how
many people could be paid to count; Hollerith removes the limit, and the
same technology is later applied to far darker uses of population registers.
That is a mechanism with real consequences for several existing entries and
it currently has no home.

### Placement note

ENG on the boundary rules — these are artifacts and technique, where NAT owns
the knowledge-generating side. `ENG-330`'s prereqs would then reach back into
this chain rather than to `ENG-190` and `NAT-210` alone.

### Consequence for NAT-215

`NAT-215` ships with `enables: []` **deliberately**. Computability theory did
not enable semiconductors, and Turing's machine was a proof device rather than
a design — its influence on actual architecture is a separate ENG question.
The honest position is that `NAT-215`'s descendants live in the missing middle
above, so the empty field is evidence for this gap rather than a defect in the
entry. It should be populated when these entries exist, and not before.

## 2. Human computers as organised labour

Between `NAT-014`'s delegable procedure and mechanical computation sits a
stage the corpus does not have: **computation organised as a division of
labour among people.**

- **Prony's logarithmic and trigonometric tables** (1790s) — the decisive
  case, because Prony applied Smith's division of labour to calculation
  explicitly and by name, splitting the work into a few mathematicians who
  chose the formulae, a handful of competent calculators who organised the
  method, and a large body of workers performing only addition and
  subtraction. Many of the last group were reportedly unemployed hairdressers.
  This is `NAT-014`'s `execution delegability` mechanic operating at
  industrial scale and it is the direct conceptual ancestor of Babbage, who
  cited it.
- The Harvard Computers (1880s-1920s) — women employed to classify stellar
  spectra, several of whom (Leavitt, Cannon, Fleming) produced first-rank
  results from within a role defined as execution rather than research. The
  entry's expertise-separates-from-execution mechanism, with the boundary
  visibly failing to hold.
- NACA and NASA computing pools through the 1940s-1960s.

### Placement

Genuinely contested between COM and NAT. **COM** owns the division of labour
(`COM-170` The Factory and Division of Labor), and Prony's own framing was
Smithian — this is a factory applied to arithmetic. **NAT** owns the
knowledge-producing institution. The deciding question is whether the entry's
claim is about *how the work was organised* (COM) or about *what kind of
knowledge institution it constituted* (NAT). On the Prony evidence, COM looks
right, with `NAT-014` as prereq and `nature` in `domains_touched`.

It should also carry the gendered-labour material, since the composition of
these workforces is not incidental — computation was routed to women
precisely because it was classified as execution rather than expertise, which
is the social consequence `NAT-014` describes, made concrete.

## 3. No foundational-crisis entry

`NAT-215` prereqs `NAT-030` (Formal Logic, -350..1000) because nothing closer
exists. Corpus-wide grep returns **zero hits** for Frege, Russell, set theory,
or the foundational crisis; the only Hilbert and Godel mentions are the ones
just written into `NAT-215`. The gap is roughly 1874-1910: Cantor's set
theory, the paradoxes, Frege's Begriffsschrift and its collapse under
Russell's letter, *Principia Mathematica*.

This matters because it is the reason Hilbert's programme existed at all —
`NAT-215` currently has to state its own motivation rather than inherit it
from a prereq. `PHI-390` (Logical Positivism and the Linguistic Turn, 1921) is
the nearest neighbour and is `see_also`-linked, but it is downstream
philosophy, not the mathematical crisis.

## 4. Two corpus-hygiene findings

- **`NAT-011` through `NAT-019` are all burned as `former_ids`** from
  renumbering Run 1, which forced `NAT-014` to reuse one. `conventions.md`
  prohibits reuse of *cut* IDs (recorded in `retired-ids.md`, which holds no
  NAT entries) and is silent on renumbered `former_ids`. `NAT-015` was avoided
  specifically because old NAT-015 became NAT-060, the entry retired in this
  operation. **Worth a convention decision**: either state that former_ids are
  reusable and rely on `id-migrations.md` to resolve, or reserve them and
  accept that dense early domains will need renumbering to absorb insertions.
- The `absence`-field misuse (holding presence or difference rather than
  absence) now stands at **five confirmed instances**: `ENG-230`, `ENG-290`,
  `WAR-200`, the energy-audit case, and `NAT-060` here — whose claim that East
  Asia lacked a formal theory of algorithmic procedure was not merely
  misplaced but **false, and contradicted inside the corpus** by `NAT-010`'s
  own east-asia variant. A corpus-wide sweep for this defect is overdue, and
  would pair efficiently with a sweep for `stats_touched: []`.
