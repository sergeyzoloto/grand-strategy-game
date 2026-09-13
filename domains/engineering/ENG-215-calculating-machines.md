---
id: ENG-215
title: Calculating Machines and the Embodied Arithmetic Operation
legacy_ref:
domain: engineering
domains_touched: [mathematics, commerce, finance]
tier: standard
status: drafted
confidence: medium
date_start: 1820
date_end: 1970
date_precision: decade
regions: [europe, americas, east-asia]
region_variants:
  - region: europe
    date_start: 1642
    note: Pascal's adding machine of 1642 and Leibniz's stepped reckoner of 1673 both work in principle and both fail as products — a few dozen Pascalines were made, the carry mechanism jammed, and Leibniz's machine had an error in its carry that was not reliably fixed in his lifetime. Thomas de Colmar patents the arithmometer in 1820 on Leibniz's stepped-drum principle, and it becomes the first commercially successful calculator only from the 1850s, once it can be built reliably in quantity. The two centuries between working idea and viable product are the entry.
  - region: europe
    date_start: 1873
    note: Willgodt Odhner's pinwheel design from St Petersburg becomes the most widely manufactured mechanical calculator in the world, licensed across Europe and mass-produced in the Soviet Union as the Felix from 1929. For most of a century the calculating machine an office worker across the Soviet bloc actually used was this one, which makes the diffusion story quite different from the invention story.
  - region: americas
    date_start: 1887
    note: Felt's key-driven comptometer and Burroughs' printing adding machine move calculation into the ordinary office — banks, railways, insurers — at a speed only possible with keys pressed directly rather than levers set and a crank turned. Comptometer operator becomes a large, trained, substantially female occupation, and the machine's output is only as fast as the operator's fingers, which keeps a skilled human in the loop for every operation.
  - region: east-asia
    date_start: 1923
    note: The Tiger calculator, mass-produced in Japan from 1923, spreads through Japanese commerce but competes for decades with soroban clerks fast enough to beat an American electric calculator in a widely reported 1946 Tokyo contest (MTH-060). Adoption is slow and selective not because the machine was unavailable but because skilled human calculation was cheap and quick, so the purchase paid only where the soroban workforce could not keep up.
absence:
  - region: africa
    note: Across colonial Africa calculating machines stayed largely within the offices of European banks and trading houses, and administrative arithmetic remained clerical. The volume of routine calculation outside those offices never reached the level at which an imported precision instrument cheaper than a clerk's time could pay for itself, so the mechanism that decides adoption elsewhere — the relative cost of skilled human calculation against the machine — never tipped. The instrument was available and the arithmetic to justify it was not there.
prereqs: [ENG-190, MTH-010]
enables: []
see_also: [MTH-060, ENG-330, COM-180, ENG-160, MTH-075]
contested: true
contested_note: "Why Pascal's and Leibniz's machines failed as products while the arithmometer succeeded is disputed between two explanations that are better modelled together than chosen between. The precision account holds that carry mechanisms need tolerances that seventeenth-century workshops could not hold repeatably, and that the nineteenth-century machine-tool base (ENG-190) is what made reliable production possible. The demand account holds that seventeenth-century commerce had nowhere near the volume of routine arithmetic that nineteenth-century banks, insurers and railways generated, so even a reliable Pascaline would have found few buyers. Model commercial viability as requiring both — manufacturable precision and a volume of office arithmetic large enough to amortise the instrument — so neither alone produces adoption. Separately, the 1946 soroban contest is a single widely reported event and should illustrate the relative-cost mechanism rather than establish that the abacus was generally faster."
stats_touched: [execution delegability, labour-saving incentive]
tags: [arithmometer, pascaline, stepped-reckoner, odhner, comptometer, tiger-calculator, soroban]
---

A machine that performs an arithmetic operation, so the operator no longer
has to. The invention works in 1642 and the product does not work until the
1850s, and the gap between the two is a manufacturing problem, not a
conceptual one.

Placement is ENG. A calculating machine carries out one operation inside a
procedure that the operator still conducts step by step — which figures to
enter, in what order, what to do with the result. It is an instrument, like
the soroban it competed with, and it cheapens a step rather than delegating
the procedure. Machines that hold the procedure outside a human head are
MTH's: the programmable machine and the unit-record tabulator.

## Timing

Pascal's machine in 1642 and Leibniz's in 1673 as working inventions that
failed commercially; de Colmar's arithmometer patented in 1820 and produced
in quantity from the 1850s; Odhner's pinwheel from 1873; the key-driven
comptometer and printing adding machine from the late 1880s; Japan's Tiger
from 1923; displacement by electronic calculators by about 1970.

## Mechanism

**The operation moves into the mechanism and the procedure stays with the
operator.** Adding with carry, and multiplication as repeated addition, are
embodied in gearing, so the result of each step no longer depends on the
operator's arithmetic skill — only on entering the right figures. That
raises `execution delegability` for the step: a clerk without training in
arithmetic produces correct sums. It does not touch the procedure. The
operator still decides what to compute, in what order, and what the
result means.

**The two-century gap is the entry's real content.** The idea was sound in
1642 and the machines did not work as products, because a carry mechanism
must transfer motion reliably across many digits at once and the
tolerances that requires could not be held repeatably by
seventeenth-century workshops. The machine-tool base of the late eighteenth
and nineteenth centuries (ENG-190) is what makes the arithmometer
manufacturable. And the demand arrives at the same time: nineteenth-century
banks, insurers and railways generate a volume of routine arithmetic that no
seventeenth-century office came near. Viability needs both, which is why
neither Pascal's cleverness nor a later market alone produced a product.

**Adoption turns on the relative cost of skilled human calculation.** A
calculating machine is worth buying only where it is cheaper than the time
of a person who can do the same arithmetic. That is why the Japanese case
matters. A soroban workforce was fast and inexpensive, so the machine paid
only at the margins the clerks could not cover, and adoption was slow for
reasons that had nothing to do with access. `labour-saving incentive` runs
here exactly as it does for human computing in MTH-060.

## Social consequence

Office arithmetic is reorganised around the machine and around a new
occupation. The comptometer operator is trained, fast, and substantially
female, and the job is classified as operating a machine rather than doing
mathematics — the same classification that shaped MTH-060's human
computers, applied to the person at the keys.

The machine also changes what an arithmetic error is. A clerk's mistake is
a lapse of skill; a machine's is a mis-entered figure or a mechanical fault,
so checking moves from re-working the calculation to verifying the inputs
and servicing the instrument. Accuracy becomes a question of maintenance and
data entry rather than of numeracy.

## Regional specifics

Invention and diffusion are geographically different stories and the entry
should carry both. The invention is French and German. The most widely made
design in the world came from St Petersburg and, as the Felix, was the
calculator most office workers across the Soviet bloc used. The American
contribution is the key-driven machine that brought calculation into the
ordinary office. Japan's Tiger spread through a commercial world that
already had a skilled human alternative, which is the clearest evidence that
adoption depended on the price of human calculation rather than on the
machine's availability. Across colonial Africa the instrument stayed in
European trading offices, because the arithmetic to justify it was not
there.

## Contested

Pascal's and Leibniz's failures are explained by precision and by demand,
and the model should require both — manufacturable tolerances and enough
office arithmetic to amortise the instrument. The 1946 soroban contest is
one event and illustrates the relative-cost mechanism; it does not show the
abacus was generally faster.

## Hook

Embodies the arithmetic operation in a mechanism, raising `execution
delegability` for each step while leaving the procedure with the operator —
an instrument, not a delegation of computation. Commercial viability should
require two things together, manufacturable precision (ENG-190) and a volume
of office arithmetic large enough to amortise the machine, which is why a
sound 1642 idea becomes a product only in the 1850s. Adoption should turn on
`labour-saving incentive`, the price of skilled human calculation against the
instrument, so a region with a fast cheap abacus workforce adopts late and
selectively. And it moves error from skill to data entry and maintenance,
creating a trained operator occupation classified as machine work rather than
mathematics.
