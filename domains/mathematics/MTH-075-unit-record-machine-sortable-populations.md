---
id: MTH-075
title: The Unit Record and Machine-Sortable Populations
legacy_ref:
domain: mathematics
domains_touched: [finance, law, war, health, migration]
tier: core
status: drafted
confidence: medium
date_start: 1890
date_end: 1970
date_precision: decade
regions: [americas, europe]
region_variants:
  - region: americas
    date_start: 1890
    note: Hollerith's tabulator processes the 1890 US census on punched cards read by electrical contacts, one card per person. The 1880 census had taken most of a decade to tabulate fully; in 1890 the headline population count was announced within weeks and full tabulation took a few years. The machines were leased rather than sold, the cards were sold by the million, and the business consolidated through the punched-card firms into IBM.
  - region: europe
    date_start: 1910
    note: Dehomag, the German Hollerith licensee founded in 1910 and later IBM's subsidiary, processes German censuses including those of 1933 and 1939 under the Nazi state, with attributes including religion punched as standard fields. The same unit-record design that apportioned congressional seats sorted a population by the categories its operator chose to record. How far the American parent knew of or directed the use is disputed; that the machine sorted by whatever was punched is not.
absence:
  - region: africa
    note: The punched card encodes a unit record — one card for one individual — and much colonial enumeration in Africa did not count individuals at all, estimating households, huts or taxpayers instead. Where there is no individual-level schedule there is nothing for a card to record, so the machine had no input and no market regardless of administrative will. What is absent is individual legibility, which is the machine's raw material, and it follows directly from the kind of census the administering power chose to run (FIN-020).
  - region: oceania
    note: Tabulating equipment was economic only on very large runs, leased at substantial cost and paying for itself by processing millions of records. No enumeration, commercial ledger or insurance book in the region approached that volume, so the technology that transformed large censuses offered small populations nothing their existing clerical methods did not already supply at lower cost.
prereqs: [FIN-020, HLT-270]
enables: [MIG-250]
see_also: [WAR-200, LAW-165, MTH-060, COM-180, ENG-215, MTH-020, MTH-065, MTH-084, MTH-062]
contested: true
contested_note: "Two disputes. The census speed-up is real and routinely overstated as one year against eight: the 1890 headline count took weeks, full tabulation a few years, against most of a decade for 1880. Model the gain as an order of magnitude in turnaround for standard counts and a smaller gain for full cross-tabulation. Second, IBM's knowledge of and responsibility for Dehomag's work for the Nazi state is contested — Black's account is influential and disputed by business historians on the degree of direction from New York. What is not contested is the mechanism, that a population-record machine sorts by whatever categories the operator punches, and that should be modelled as a property of the technology rather than of any one case."
stats_touched: [legibility, execution delegability]
tags: [hollerith, unit-record, punched-card, census, tabulator, ibm, dehomag]
---

One card per individual, sortable by any attribute punched into it. That
single design moves the constraint on what a state can learn from its own
records from how many clerks it employs to how many cards it buys.

Placement is MTH under the procedure-outside-the-head test. A tabulator's
counting and sorting plan is set in the machine and the population is held
on the cards — neither lives in an operator's head, unlike a calculating
machine (ENG-215), which performs a single operation for someone still
conducting the procedure.

## Timing

From Hollerith's tabulation of the 1890 US census, through the punched-card
industry's consolidation into IBM and its licensees abroad, to the
displacement of the unit record by magnetic storage around 1970.

## Mechanism

**The unit record makes a population machine-sortable.** Each individual is
one card, and each attribute — age, sex, birthplace, occupation, religion —
is a hole in a fixed position. A tabulator reads a card in a fraction of a
second and counts or sorts a whole population by any combination of punched
attributes. The cross-tabulation that took a clerical workforce years now
takes a machine run.

**Enumeration stops being bounded by clerical labour.** Every register in
the corpus — the census (FIN-020), vital registration (HLT-270), the rolls
that make conscription (WAR-200) and compulsory schooling (LAW-165) possible
— was limited by how many people could be paid to count and cross-tabulate
its contents. With the unit record the limit moves to card stock and machine
hours. **`legibility` becomes cheap to exploit once it has been collected,
which is not the same as cheap to collect.** The machine makes far more use
of a register without making the register any easier to build, so it rewards
states that already enumerate individuals and gives nothing to those that do
not.

**The business model is part of the mechanism.** Machines were leased rather
than sold, and the cards were consumables bought in enormous volume. The
customer paid continuously for capacity, the supplier's revenue tracked the
customer's volume of records, and large administrations became bound to one
supplier's card format — a dependency on a record standard rather than on a
machine.

**The capability is indifferent to purpose.** The machine sorts by whatever
categories its operator chooses to punch. The design that apportioned
legislative seats in 1890 sorted German census returns by religion under the
Nazi state. This is a property of the technology, and a model that treats
the second use as misuse rather than as the machine working as designed has
misread what the machine is.

## Social consequence

The first large customers are states, and the machine's first use is
counting people. That fixes the relationship between computation and
administration for the following century: the organisations that could
afford tabulating equipment were those with populations to enumerate,
revenue to assess and armies to supply, and what they bought was capacity to
see and sort their own subjects.

Once a population is sortable by attribute, acting on a category becomes
administratively cheap. Identifying everyone of a given birthplace,
confession or occupation, which was previously a matter of months of clerical
search, becomes a machine run — and that is the capability forced population
movement depends on (MIG-250). Inside offices the punched-card operator becomes
a large new occupation, substantially female and classified as execution,
continuing MTH-060's pattern into the machine era.

## Regional specifics

This is genuinely an Atlantic technology in its adoption scale, and the
reason is economic rather than incidental: leasing paid only for very large
runs, so only very large administrations and firms took it up. The American
census is the founding case and the German censuses under Dehomag are the
case that establishes purpose-indifference. The absences are the entry's
wider geography. Where colonial enumeration counted huts or households
rather than individuals there was no unit record to punch, which ties the
machine's absence directly to the kind of legibility a state had chosen to
build; and where populations were small the leasing economics never closed.

## Contested

The census speed-up is real and overstated: weeks for the headline count, a
few years for full tabulation, against most of a decade in 1880. IBM's
knowledge of Dehomag's work for the Nazi state is contested; the mechanism,
that a population-record machine sorts by whatever is punched, is not, and it
should be modelled as a property of the technology.

## Hook

Makes a population machine-sortable by any recorded attribute, so the
binding constraint on what a state learns from its registers moves from
clerical staffing to card stock. `legibility` becomes cheap to exploit once
collected but no cheaper to collect, so the machine should pay out only for
states that already enumerate individuals and do nothing where censuses count
households or huts. It arrives under a leasing model that binds large
administrations to one supplier's record format. And it is indifferent to
purpose: whatever categories are punched are the categories by which a
population can be found and acted on, which makes category-based
administration — up to and including forced movement — cheap once the
register exists.
