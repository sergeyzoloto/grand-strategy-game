---
id: NAT-014
title: The Algorithm as Delegable Procedure
legacy_ref:
domain: nature
domains_touched: [commerce, engineering, law]
tier: core
status: drafted
confidence: medium
date_start: -1800
date_end: 1800
date_precision: era
regions: [middle-east, mediterranean, south-asia, east-asia, europe]
region_variants:
  - region: middle-east
    date_start: -1800
    note: Old Babylonian mathematical tablets are the earliest surviving instances of the mechanism and they are written exactly as the mechanism requires — second-person imperative recipes, step by step, stating what to do and never why it works. Scribal training reproduced them as procedures to be executed rather than arguments to be followed. Al-Khwarizmi's treatise of around 820 then names and systematises a practice roughly two and a half millennia old, which is a more interesting claim than originating it and does not require overstating the case; the word algorithm descends from his name because he organised the field, not because he began it.
  - region: mediterranean
    date_start: -300
    note: Euclid supplies the contrast that defines the entry. The GCD procedure is an algorithm with a proof that it terminates and returns the right answer, which is procedure plus justification and therefore a different and later achievement than the Babylonian recipe. Greek mathematics treats deduction as the point and computation as a means; the procedural traditions treat computation as the point. Both are mathematics, and a model that scores one as more advanced has mistaken a choice of aim for a level of attainment.
  - region: south-asia
    date_start: 500
    note: Indian arithmetic supplies the notation without which a written procedure is barely tractable — decimal place-value with a true zero, mature by roughly 500 (NAT-010) — and then general rules over it, with Brahmagupta in 628 giving the rules for zero and for negative quantities as operations anyone can apply without reasoning about what a negative quantity is. That is the mechanism in its purest form, since the rule is correct in the hands of someone who finds the object it operates on unintelligible.
  - region: east-asia
    date_start: 100
    note: The Nine Chapters on the Mathematical Art organises mathematics as 246 problems each resolved by a stated general procedure, which is algorithmic mathematics as a formal subject of study rather than as a collection of tricks — with Liu Hui's commentary of 263 supplying justifications alongside the procedures. Qin Jiushao's work of 1247 gives the general procedure for simultaneous congruences now called the Chinese remainder theorem. This tradition is as developed as any in the entry and is corrected here from a prior claim that it lacked a formal theory of procedure.
  - region: europe
    date_start: 1202
    note: Fibonacci's Liber Abaci transmits the Indian-Arabic procedures into Latin Europe, and the ensuing contest between algorism (calculation with written figures) and abacism (calculation on a counting board) is a fight about the entry's mechanism rather than about arithmetic. Written procedure leaves an auditable record and can be taught from a book to someone the teacher never meets; board reckoning leaves nothing and must be shown in person. The written method won because it was delegable, not because it was faster.
absence:
  - region: oceania
    note: Polynesian wayfinding is computation of a high order — integrating heading, swell, star bearings, and elapsed time over weeks of open ocean — carried out with no notation of any kind. Because nothing is written, the procedure cannot be separated from the person performing it. Transmission runs through years of apprenticeship in which the navigator learns to perceive as well as to calculate, so the skill is inseparable from understanding and cannot be handed to an executor who lacks it. This is the entry's mechanism failing for want of a recording medium rather than for want of computational sophistication, and it shows notation rather than mathematics is the binding constraint.
  - region: africa
    note: West African Ifa divination runs a genuinely formal combinatorial system over 256 figures, each keyed to a memorised body of verses, and it is transmitted as an oral corpus held in the diviner's memory rather than as written instructions. The result is a formal procedure whose transmission cost is initiation rather than instruction — years of memorisation gated by a lineage of teachers, not a text a stranger can pick up. What is absent is the written recipe, and with it the possibility of delegating execution to someone outside the tradition; the formalism itself is fully present.
prereqs: [NAT-010]
enables: [NAT-215]
see_also: [NAT-030, NAT-050, NAT-240, ENG-330, COM-170, COM-180, AGR-050]
contested: true
contested_note: "Whether Old Babylonian procedural texts should be called algorithms is genuinely disputed. Knuth argued in 1972 that they are algorithms in every functional sense; Hoyrup, Robson and others caution that reading them through a modern computational category imports assumptions the scribes did not hold, and that the texts are better understood as worked examples within a cut-and-paste geometric practice than as general procedures. The dispute matters here because it sets the entry's start date. Model the capability as arriving with written procedural instruction rather than with any particular formal definition of what a procedure is, and treat the date as a band from roughly -1800 to -300 rather than a point — the Babylonian end if delegable execution is the criterion, the Euclidean end if generality and proof of correctness are required. The mechanism this entry actually turns on is the first, which is why it is dated there."
stats_touched: [execution delegability, literacy]
tags: [algorithm, babylonian-tablets, euclid, nine-chapters, al-khwarizmi, brahmagupta, procedure]
---

A procedure specified precisely enough to be executed correctly by someone
who does not understand why it works. That property, and not any machine,
is what makes mathematical labour delegable — and everything from the
scribe to the clerk to the calculating office to the computer is
downstream of it.

## Timing

From Old Babylonian procedural tablets around -1800 through Euclid, the
Nine Chapters, Indian positional arithmetic, al-Khwarizmi's systematisation
around 820, and Qin Jiushao, to the eve of mechanical calculation around
1800. The concept is stable across all of it; what changes is notation,
generality, and who is permitted to execute.

## Mechanism

An algorithm's defining property is not that it is correct, nor that it
terminates, but that **it can be followed without being understood**. A
recipe that says multiply this by that and subtract the other produces the
right answer in the hands of someone with no idea why those operations
bear on the question. That is an unusual kind of knowledge — it packages
understanding into a form that does not have to be transmitted along with
the result.

The consequence is that **mathematical labour becomes divisible**. Once a
procedure is written down, the person who devised it and the person who
executes it need not be the same person, need not be equally trained, and
need not be in the same place or century. Execution can be handed to a
scribe, a clerk, a schoolchild, a room full of hired calculators, and
eventually a machine — and the quality of the output does not degrade,
because the procedure carries the expertise and the executor supplies only
accuracy and patience.

Two conditions gate this and both are about recording rather than
mathematics. The procedure must be **written in a notation compact enough
to be followed**, which is why positional decimal notation with a zero
matters so much (NAT-010) — the same computation in a non-positional
system is a procedure in principle and unusable in practice. And the
notation must be **transmissible to someone the author will never meet**,
which is what separates a written recipe from an apprenticeship. Where
either condition fails, computation of great sophistication can exist and
remain permanently bound to the person doing it, which is what the
absences below record.

This also makes the algorithm the earliest instance in this record of a
pattern that recurs constantly later: **capability stored in a procedure
rather than in a practitioner**. The same move underlies the drill manual,
the assembly line's task card, the clinical protocol, and the program.

## Social consequence

Expertise separates from execution, and the two acquire different social
positions. The person who devises a procedure holds something scarce and
transferable; the person who runs it holds something that the procedure has
made abundant. Scribal cultures show this immediately — the tablets are
training material, and a scribe's value lies in accurate execution of
inherited methods rather than in devising new ones.

The effect on access runs in two directions at once and both should be
carried. A written procedure **democratises**, because a merchant's son
with a book can now do arithmetic that formerly required years with a
master, which is the substance of the algorism-against-abacism contest in
late medieval Europe. It also **de-skills**, because the same book makes
the master's decades of practice purchasable for the price of a text, and
the reckoning masters who had made a living from board arithmetic lost it.
The gain in access and the destruction of a craft are the same event
described from two sides.

The written procedure also creates auditability. A calculation done on a
counting board leaves nothing behind, while one worked on paper leaves
every step exposed to a checker who was not present — which makes written
arithmetic the form that accounting, taxation, and administration adopt,
for reasons that have more to do with verification than with speed.

## Regional specifics

The traditions differ in aim rather than in level, and the entry should be
built so that no ordering between them is possible. Mesopotamian
mathematics is procedural from the start and does not seek proof. Greek
mathematics seeks proof and treats computation as subordinate. Chinese
mathematics, systematised in the Nine Chapters and extended by Liu Hui and
Qin Jiushao, is algorithmic as a formal subject with its own standards and
commentary tradition. Indian mathematics supplies the notation that makes
written procedure practical and then general rules over it, including rules
for objects — zero, negatives — that the rule-follower need not find
intelligible.

The Islamic contribution is best stated precisely because the imprecise
version is so common. Al-Khwarizmi did not invent the algorithm; he
organised, systematised, and taught a practice already ancient in several
traditions, and the field carries his name because systematisation is what
made it a field. That is a real achievement and it does not need to be
inflated into an origin.

The two absences isolate the binding constraint. Polynesian navigation and
Ifa divination are both computationally serious and both lack written
procedure, and in both the result is that execution cannot be separated
from understanding. Notation, not mathematical sophistication, is what this
entry runs on.

## Contested

Whether the Babylonian tablets are algorithms is disputed, and the dispute
sets the start date. Knuth read them as algorithms in the functional
sense; Hoyrup and Robson caution that the modern category imports
assumptions the scribes did not hold, and that the texts work as worked
examples inside a geometric cut-and-paste practice rather than as general
procedures.

Model the capability as arriving with **written procedural instruction**
rather than with any formal definition of procedure, and treat the onset as
a band from roughly -1800 to -300 rather than a point — the early end if
delegable execution is the criterion, the late end if generality and proof
of correctness are required. This entry takes the early end because
delegable execution is the mechanism it turns on, and a model that requires
proof before granting the capability will misdate every procedural
tradition outside Greece.

## Hook

Introduces `execution delegability` — the share of a skilled task that can
be handed to an untrained executor without loss of output quality. Raising
it converts scarce expertise into a transferable object, so a state or firm
can buy accuracy and patience instead of buying understanding, and can
scale computation by adding people rather than by training specialists.
It is gated on notation rather than on mathematical attainment, so a
tradition with sophisticated computation and no writing gets none of the
benefit and keeps the skill locked in the practitioner. The same increment
should pay out twice with opposite signs — widening access to a competence
while destroying the livelihood of those who had held it scarce — which
makes it the earliest instance of a pattern the mechanisation entries
repeat. And because a written procedure leaves its steps exposed to a
checker who was not present, adopting it should raise auditability, which
is why administrations adopt written calculation for reasons unrelated to
its speed.
