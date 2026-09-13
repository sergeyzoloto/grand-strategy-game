---
id: MTH-065
title: The Programmable Machine
legacy_ref:
domain: mathematics
domains_touched: [engineering, war, commerce]
tier: core
status: drafted
confidence: medium
date_start: 1822
date_end: 1946
date_precision: decade
regions: [europe, americas]
region_variants:
  - region: europe
    date_start: 1822
    note: Babbage's difference engine from 1822 automates the tabulation of polynomial tables and his analytical engine from the 1830s specifies a general programmable machine, taking its instruction cards from the Jacquard loom (ENG-180), where a pattern held in a chain of cards changed the weave without rebuilding the loom. Neither engine was completed. Lovelace's notes of 1843 state that such a machine could operate on any symbols whose relations could be expressed, not only on numbers, and include a worked sequence of operations for computing Bernoulli numbers.
  - region: europe
    date_start: 1941
    note: Zuse's Z3 in Berlin in 1941 is a working program-controlled machine reading instructions from punched film, built without knowledge of the Anglo-American work. Colossus at Bletchley from 1943-44 is an electronic machine configured for cryptanalysis of German high-command teleprinter traffic, and it was kept secret until the 1970s, with most machines destroyed after the war. Two independent machines on opposite sides of a war, one of which could influence no one.
  - region: americas
    date_start: 1944
    note: Aiken's Harvard Mark I from 1944 reads instructions from punched tape, and Aiken encountered Babbage's work only after conceiving his own machine. ENIAC in 1945 is electronic and built to compute artillery firing tables, a job a human computing staff was already doing. It is programmed by setting switches and plugging cables — the procedure held in the machine's configuration rather than in anyone's head — and its first programmers were women drawn from the human computing pool.
absence:
  - region: south-asia
    note: Every programmable machine before 1946 was paid for by a specific, urgent state demand — ballistic tables, cryptanalysis of machine ciphers, the tables Babbage sought Admiralty funds to produce — and in South Asia the equivalent demand for large tables was already met by human computers such as the Survey of India's (MTH-060) at low cost. With a cheap skilled calculating workforce in place and no wartime cryptanalytic or ballistics programme sited there, the incentive that forced programmable machines into existence elsewhere never arose.
  - region: africa
    note: No state sponsor with both the demand and the precision electromechanical industry existed on the continent. Colonial administrations had their tables, surveys and military calculations computed in metropolitan offices, so the demand that produced these machines was administered from elsewhere, and the machines were built where the demand was administered rather than where it applied.
prereqs: [MTH-020, ENG-180, ENG-215]
enables: [MTH-087]
see_also: [MTH-060, MTH-075, MTH-080, WAR-150, WAR-330, MTH-072]
contested: true
contested_note: "Three disputes, and together they argue against presenting a lineage. Babbage's influence on the builders of the 1940s is disputed: Aiken found Babbage only after conceiving the Mark I, several others did not know his work, and the line from the analytical engine to ENIAC is largely constructed in retrospect. Colossus was secret until the 1970s and could influence nothing. Model programmable machines as repeatedly and independently invented under specific state demand rather than as descendants of one another. Separately, how much of the program in Lovelace's notes is hers rather than Babbage's is disputed among historians; the claim that such a machine could manipulate arbitrary symbols is clearly stated in her notes and is the part the entry relies on. Finally, whether Babbage failed because of precision engineering limits, funding, or his own repeated redesigns is unresolved; the 1991 construction of Difference Engine No. 2 to period tolerances shows the design was buildable, which weakens the pure precision explanation."
stats_touched: [execution delegability, stranded capital]
tags: [babbage, lovelace, jacquard, analytical-engine, zuse, colossus, eniac, programmable]
---

The procedure is held in a medium separate from both the operator and the
machine, so the machine's task changes without the machine changing. That
is a different thing from ENG-215, where a machine performs an operation for
a person still conducting the procedure: here the procedure itself has left
the person.

## Timing

From Babbage's difference engine in 1822 and analytical engine design in
the 1830s, with the Jacquard loom's cards (1804) as the transmitted idea and
Lovelace's notes in 1843, to Zuse's Z3 in 1941, Colossus in 1943-44, the
Harvard Mark I in 1944 and ENIAC in 1945.

## Mechanism

**The instruction leaves both the operator and the mechanism.** MTH-020
established that a procedure written precisely enough can be executed by
someone who does not understand it; MTH-060 organised that into a workforce.
A programmable machine holds the procedure in cards, tape, film or plugged
cables, so a sequence of operations runs without a person choosing each
step, and a different sequence can be loaded without rebuilding the
machine. `execution delegability` reaches its limit for procedures: nothing
in the loop needs to understand anything.

**The idea arrives before the machine.** Jacquard's loom supplies the
decisive notion — a pattern held in a chain of cards, changeable without
changing the loom — and Babbage adopts it for the analytical engine.
Lovelace's notes then state the consequence that matters most: such a
machine operates on symbols of any kind whose relations can be expressed,
not only on quantities. That is a claim about what a procedure held outside
a head can be about, and it is made a century before anything could test it.

**Invention recurs under demand rather than descending.** Babbage's engines
were not completed and his influence on the 1940s builders is at best
indirect. Zuse built in Berlin without knowledge of the British or American
work. Colossus was secret and influenced nothing. Aiken found Babbage after
designing his own machine. The machines appear where a state has an urgent,
specific demand — firing tables, cryptanalysis — together with an
electromechanical or electronic industry able to meet it, and they appear
independently. The model should reinvent the capability wherever those two
conditions meet rather than transmitting it along a chain.

**What this entry is not.** In every machine here the procedure is held in a
medium distinct from the data it operates on — cards, tape, switches and
cables. Holding instructions in the same memory as data, so that a program
is itself data, is a later and separate step.

## Social consequence

These machines displace a workforce that already existed. ENIAC's job was
producing firing tables, which a human computing staff was producing by hand
and desk calculator, so the machine replaced MTH-060's computers task by task
— the source of the `stranded capital` that entry describes. The replacement
also recruited from the replaced: ENIAC's first programmers were women drawn
from the computing pool, because programming was at first classified as
clerical, the same classification that had shaped the pool.

Because every machine here was built for a war or a state table programme,
the capability arrives inside military and administrative institutions and
under secrecy. Colossus's destruction and decades-long classification meant
that one of the most advanced machines of its period left no public trace,
which shaped who could later claim priority and how the history was told.

## Regional specifics

Every programmable machine before 1946 was built in Britain, Germany or the
United States, and the reason is the entry's mechanism rather than an
accident of geography: each was paid for by an urgent state demand and built
by an existing electromechanical or electronic industry, and those two
conditions met only there in this window. The Z3 and Colossus, on opposite
sides of a war and in mutual ignorance, are the evidence for independent
invention. The absences carry the wider geography. In South Asia the
demand for large tables was already met by cheap, skilled human computers,
so the incentive never formed; across Africa the demand was administered
from metropolitan offices, so the machines were built where the demand was
managed rather than where it applied.

## Contested

Babbage's influence on 1940s designers is disputed and Colossus influenced
nothing, so the entry models repeated independent invention under demand,
not a lineage. How much of Lovelace's program is hers is disputed; the
symbols-not-quantities claim is clearly hers and is what the entry uses.
Why Babbage's engines were not built is unresolved, and the 1991
construction of Difference Engine No. 2 to period tolerances weakens the
pure precision explanation in favour of funding and redesign.

## Hook

Holds the procedure in a medium separate from operator and machine, so the
task changes without the machine changing and nothing in the loop needs to
understand anything — `execution delegability` at its limit for procedures.
It should be invented independently wherever urgent state demand (ballistic
tables, cryptanalysis) meets an electromechanical or electronic industry able
to build it, rather than transmitted along a lineage. It should displace a
human computing workforce task by task, producing `stranded capital`, while
recruiting its first programmers from that workforce under the same clerical
classification. And it arrives inside military and state institutions under
secrecy, so priority and influence are shaped by what was allowed to be known.
