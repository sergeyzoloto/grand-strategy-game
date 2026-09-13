---
id: MTH-050
title: Mechanical and Electromechanical Computation
legacy_ref:
domain: mathematics
domains_touched: [finance, law, war, commerce, engineering]
tier: core
status: drafted
confidence: medium
date_start: 1642
date_end: 1950
date_precision: century
regions: [europe, americas, east-asia]
region_variants:
  - region: europe
    date_start: 1642
    note: Pascal's adding machine of 1642 and Leibniz's stepped reckoner of 1673 embody arithmetic operations in gearing, and neither is a commercial success. Babbage's difference engine from the 1820s and analytical engine from the 1830s specify a general programmable calculating machine that was never built, and Lovelace's notes of 1843 state that such a machine could operate on symbols of any kind rather than numbers alone. Zuse's Z3 in Berlin in 1941 and the British Colossus of 1943-44 then build programmable electromechanical and electronic machines independently of the American work.
  - region: europe
    date_start: 1873
    note: The pinwheel arithmometer designed by Willgodt Odhner in St Petersburg becomes the most widely manufactured mechanical calculator design in the world, licensed across Europe and mass-produced in the Soviet Union as the Felix from 1929. For most of a century the calculating machine an office worker across Russia and the Soviet bloc actually used was this one, which makes the diffusion story quite different from the invention story.
  - region: americas
    date_start: 1890
    note: Hollerith's tabulator processes the 1890 US census on punched cards read by electrical contacts, one card per person. The 1880 census had taken most of a decade to tabulate fully; in 1890 the headline population count was announced within weeks, and full tabulation took a few years. The machines were leased rather than sold, the cards sold by the million, and the business consolidated through the punched-card firms into IBM. ENIAC in 1945 was built to compute artillery firing tables — replacing a human computing workforce doing the same job.
  - region: east-asia
    date_start: 1923
    note: The Tiger calculator, mass-produced in Japan from 1923, carries mechanical calculation across Japanese commerce and administration, where it coexists for decades with a highly skilled soroban clerical workforce (MTH-060) fast enough to compete with it. Adoption of the machine is therefore slower and more selective than in economies where calculation had no skilled human alternative, which is the relative-cost mechanism running in a region that invented neither.
absence:
  - region: africa
    note: The punched-card tabulator encodes a unit record — one card for one individual — and much colonial enumeration in Africa did not count individuals at all, estimating households, huts or taxpayers instead. Where there is no individual-level schedule there is nothing for a card to record, so the machine had no input and no market regardless of administrative will. The absence is of individual legibility, which is the machine's raw material, and it follows directly from the kind of census the administering power chose to run (FIN-020).
  - region: oceania
    note: Tabulating equipment was economic only on very large runs, since it was leased at substantial cost and paid for itself by processing millions of records. No enumeration, commercial ledger or insurance book in the region approached that volume, so the technology that transformed large censuses offered small populations nothing their existing clerical methods did not already supply at lower cost.
prereqs: [MTH-020, FIN-020]
enables: []
see_also: [MTH-060, ENG-180, ENG-190, HLT-270, WAR-200, LAW-165, ENG-330, COM-180, WAR-150]
contested: true
contested_note: "Four disputes. Babbage's influence on the builders of the 1930s and 1940s is disputed: several were unaware of his work, others encountered it late, and the lineage from the analytical engine to the electronic computer is largely a retrospective construction. Model the machines as repeatedly and independently re-invented under demand rather than as descendants. Colossus raises the same problem from the other direction, since it was kept secret until the 1970s and could influence nothing. Third, the census speed-up is real but routinely overstated as one year against eight; the headline count took weeks and full tabulation a few years. Fourth, punched-card systems processed German censuses under the Nazi state through IBM's German subsidiary Dehomag, and how far the American parent knew of or directed that use is contested — Black's account is influential and disputed. What is not contested is the mechanism, that a machine for processing population records serves whatever enumeration the state chooses to run, and that should be modelled as a property of the technology rather than of the case."
stats_touched: [legibility, execution delegability, stranded capital]
tags: [hollerith, tabulator, punched-card, babbage, lovelace, arithmometer, eniac, colossus]
---

The procedure moves from a person into a machine. MTH-060 delegated
calculation to people who did not need to understand it; this entry
delegates it to mechanisms that cannot — and the decisive case is a machine
built to count a population.

## Timing

From Pascal's adding machine in 1642 and Leibniz's reckoner in 1673,
through the Jacquard loom's punched cards in 1804 and Babbage's unbuilt
engines from the 1820s, to Hollerith's census tabulator in 1890, the
punched-card industry that followed, Zuse's Z3 in 1941, and Colossus and
ENIAC in the mid-1940s.

## Mechanism

**Calculation stops requiring a person to follow the procedure.** MTH-020
established that a written procedure can be executed without being
understood, and MTH-060 organised that into a workforce. A calculating
machine takes the last step: the procedure is embodied in the mechanism, so
`execution delegability` extends to an executor that cannot understand
anything at all. The operator supplies inputs and reads outputs, and the
arithmetic in between belongs to the machine.

**The punched card separates the instruction from both operator and
machine.** Jacquard's loom of 1804 is not a calculating device, but it
supplies the idea that matters — a pattern held in a sequence of cards,
readable by a mechanism, changeable without rebuilding the loom. Babbage
adopted it for the analytical engine. Hollerith adapted it into something
different and more consequential: the **unit record**, one card per
individual, with each attribute punched in a fixed position.

**Hollerith is where enumeration stops being bounded by clerical labour.**
Every register in the corpus — the census (FIN-020), vital registration
(HLT-270), the rolls that make conscription (WAR-200) and compulsory
schooling (LAW-165) possible — was limited by how many people could be paid
to count and cross-tabulate its contents. A tabulator reads a card in a
fraction of a second and sorts a population by any combination of punched
attributes, so the constraint on what a state can learn from its own
records moves from staffing to card stock. `legibility` becomes cheap to
exploit once it has been collected, which is a different thing from being
cheap to collect, and the difference matters: the machine makes more use of
the register without making the register easier to build.

**The business model is part of the mechanism.** Hollerith's machines were
leased rather than sold, and the cards were consumables sold in enormous
volume. The customer paid continuously for capacity rather than once for a
machine, and the supplier's revenue tracked the customer's volume of
records — an arrangement that bound large administrations to a single
supplier's card format and grew into IBM.

**These machines displace a workforce that already existed.** ENIAC was
built to compute artillery firing tables, which a human computing staff was
already producing. The machines replace MTH-060's computers task by task,
and the resulting displacement is `stranded capital` — skills that were
valuable because they were specific and became worthless for the same
reason.

## Social consequence

The first large customers are states, and the machine's first use is
counting people. That fixes the relationship between computation and
administration for the following century: the organisations that could
afford tabulating equipment were those with populations to enumerate,
revenue to assess, and armies to supply, and the capacity they bought was
capacity to see and sort their own subjects.

That capacity is indifferent to purpose. The same unit-record system that
tabulated a census for apportionment tabulated censuses under the Nazi state
through IBM's German subsidiary, identifying populations by the attributes
the state chose to punch. How much the American parent knew is disputed;
that the technology does what its operator's categories direct it to do is
not, and it should be treated as a property of the machine rather than a
misuse of it.

Inside offices the calculating machine reorganises clerical work around
itself, dividing tasks into operating, punching and checking, and the
punched-card operator becomes a large new occupational category — again
substantially female and classified as execution, continuing MTH-060's
pattern into the machine era.

## Regional specifics

Invention is concentrated in Western Europe and the United States, and
diffusion is not, and the entry should carry both. Odhner's arithmometer,
designed in St Petersburg, became the most widely made mechanical
calculator in the world and the Soviet Felix was the machine most office
workers across a sixth of the earth's land surface used. Japan's Tiger
calculator carried the technology through Japanese commerce, where it
competed for decades with soroban clerks fast enough to beat it — which
shows adoption turning on the relative cost of skilled human calculation
rather than on availability. Zuse built a programmable machine in wartime
Berlin with no knowledge of the Anglo-American work, and Colossus was built
in secret and influenced nothing, both of which argue for repeated
independent invention over lineage.

The absences tie the machine to the kind of register that exists. Where
enumeration counted huts or taxpayers rather than individuals there was no
unit record to punch, and where populations were small the leasing
economics never closed.

## Contested

Babbage's influence on later designers is disputed and the entry should not
present a lineage: several builders of the 1940s did not know his work, and
the line from the analytical engine to the electronic computer is largely
built in retrospect. Model the machines as repeatedly re-invented under
demand. Colossus, secret until the 1970s, supports that reading.

The census speed-up is real and routinely overstated — weeks for the
headline count, years for full tabulation, against most of a decade in
1880. IBM's knowledge of Dehomag's work for the Nazi state is contested; the
mechanism that a population-record machine serves any enumeration its
operator chooses is not.

## Hook

Extends `execution delegability` to an executor that understands nothing,
so calculation no longer needs a person to follow the procedure — and it
should displace MTH-060's human computers task by task, producing
`stranded capital` in a workforce whose skill was valuable because it was
specific. The unit record makes a register machine-sortable, so the binding
constraint on what a state can learn from its own records moves from how
many clerks it employs to how many cards it buys: `legibility` becomes
cheap to exploit once collected, but no cheaper to collect, so the machine
rewards states that already enumerate individuals and offers nothing where
enumeration counts households or huts. It should arrive under a leasing
model that ties large administrations to one supplier's record format. And
the capability is indifferent to purpose, so whatever categories the
operator punches are the categories by which the population becomes
sortable.
