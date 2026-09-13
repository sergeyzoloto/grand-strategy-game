---
id: MTH-015
title: Numeral Systems and Positional Notation
legacy_ref:
domain: mathematics
domains_touched: [media, commerce, finance, agriculture]
tier: core
status: drafted
confidence: high
date_start: -2100
date_end: 1500
date_precision: era
regions: [middle-east, americas, south-asia, east-asia, europe]
region_variants:
  - region: middle-east
    date_start: -2100
    note: Sexagesimal place-value emerges in Ur III administration and is standard in Old Babylonian mathematical texts, so a sign's value depends on its position. There is no true zero for most of this history — an empty place is left as a space, and the same sign can mean one, sixty or one-sixtieth depending on context the reader must supply. A separator mark for an empty internal place appears only in the Seleucid period, and never at the end of a number.
  - region: americas
    date_start: -100
    note: Maya notation is positional and vigesimal with a true zero sign, used in Long Count dates recorded by the first century BCE and developed with no contact whatever with Old World traditions. The calendrical positions depart from strict base twenty in one place to fit the year, which shows the notation shaped around the problem it was built for, and it is one of only two independent inventions of zero as a working positional sign.
  - region: south-asia
    date_start: 500
    note: Indian decimal place-value with zero as a number in its own right, not merely an empty place, is established by around 500, and Brahmagupta in 628 gives rules for arithmetic with zero and negative quantities. This is the notation that transmits — through the Islamic world and al-Khwarizmi's arithmetic — into nearly all later written calculation, which is why its consequences are global rather than regional.
  - region: east-asia
    date_start: -300
    note: Chinese counting-rod numerals are positional and decimal on the counting board, with an empty square standing for zero and alternating vertical and horizontal forms keeping adjacent places distinct. The calculation happens on the board rather than on paper, so positional arithmetic was fully developed here long before a written zero sign appears in the thirteenth century — a working place-value system whose zero was a location before it was a symbol.
  - region: europe
    date_start: 1202
    note: Fibonacci's Liber Abaci of 1202 brings Hindu-Arabic numerals into Latin commerce, and adoption is slow and resisted. Florence's money-changers' guild prohibited the new numerals in account books in 1299 on the ground that a written zero could be altered into another digit, and the contest between reckoning on the board and calculating in written figures ran for three centuries. Europe is the late and reluctant adopter of a notation invented elsewhere.
absence:
  - region: africa
    note: Sub-Saharan traditions counted with real sophistication, including large vigesimal verbal systems, but developed no written positional notation before contact with Arabic numerals. Without a written notation a calculation exists only while someone is performing it, so it cannot be checked afterwards by a third party, stored, or handed on as a worked result. What that forecloses is calculation at administrative and calendrical scale, where results must outlast the person who produced them.
prereqs: [AGR-050, MED-010]
enables: [MTH-020, MTH-040, NAT-040, ENG-215, NAT-015, MTH-025, MTH-045]
see_also: [NAT-020, MTH-070, MED-020]
contested: true
contested_note: "Two disputes. The Bakhshali manuscript's radiocarbon dating in 2017 placed some folios, which use a dot for zero, as early as the third or fourth century, and the result is disputed because the folios date to widely different periods and the manuscript may be a later copy on older material. Model the Indian zero as established by around 500 and treat earlier dates as possible rather than settled. Second, whether the Indian positional zero drew on Babylonian or Greek astronomical placeholders transmitted through astronomy is unresolved — the Greek astronomers used a zero-like sign in sexagesimal tables — so model it as one plausible route among independent development rather than as a documented lineage. The Maya zero is uncontested as independent."
stats_touched: [execution delegability]
tags: [positional-notation, zero, sexagesimal, maya-numerals, counting-rods, hindu-arabic-numerals, abacists]
---

Notation decides what calculation is tractable. The same multiplication is a
specialist skill in additive numerals and a written procedure a child can
learn in positional ones, and zero is the element that makes place-value
unambiguous.

## Timing

From Ur III sexagesimal place-value around -2100, through Maya positional
notation with zero by the first century BCE, Chinese rod numerals, and the
Indian decimal zero by around 500, to the transmission through al-Khwarizmi
and Fibonacci and Europe's slow adoption, complete by about 1500.

## Mechanism

**A positional notation makes the written number do the arithmetic's
bookkeeping.** In an additive system the symbols are counted up, so
multiplying two large numbers is a matter of skilled manipulation on a
counting board, and the work leaves nothing checkable behind. In a positional
system a symbol's value is set by where it stands, so the standard procedures
of written arithmetic — carrying, long multiplication, long division — become
mechanical operations on columns. That is what makes MTH-020's delegable
procedure practical for calculation at all: the same algorithm in a
non-positional script is a procedure in principle and unusable in practice.

**Zero is the decisive element, and it is two different inventions.** A
place-value system needs a way to mark an empty place, or a sign can mean one
or sixty or one-sixtieth by context — the Babylonian problem for most of its
history. Marking the empty place fixes the ambiguity. Treating zero as a
*number*, something that can be added, subtracted and multiplied, is a second
and separate step, and it is the step that turns positional writing into
algebraic manipulation. Babylonian notation took the first step late and not
the second; the Maya took the first fully; Indian mathematics took both.

**Independent invention recurs, which means notation answers a pressure
rather than an idea.** Place-value appears in Mesopotamia, in China on the
counting board, in Mesoamerica, and in India, and zero as a positional sign
at least twice independently. What the cases share is a large, recurring
volume of calculation — administration, calendar, astronomy — heavy enough
that the bookkeeping of the notation itself became the constraint.

## Social consequence

A positional notation lowers the cost of producing a person who can
calculate, in the way MED-020's alphabet lowers the cost of producing a
reader. Written arithmetic can be taught from a book to someone the teacher
never meets, and checked afterwards by someone who was not present, which is
why accounting, taxation and trade adopt it.

The same property creates a new vulnerability, and the European case records
it plainly. Florence's money-changers banned the new numerals from account
books in 1299 because a written zero could be altered into a six or nine
after the fact. A notation that makes calculation cheap and checkable also
makes forgery of a figure easy, where Roman numerals resisted it — so
adoption is a trust problem as much as a calculating one, and the resolution
is institutional: writing sums out in words, standardised ledgers, audit.

## Regional specifics

The invention is overwhelmingly non-European and Europe is the late,
reluctant adopter. Babylonian sexagesimal place-value is the earliest and
worked for millennia without a true zero. Maya positional notation with zero
is independent of everything in the Old World. Chinese rod numerals were
fully positional on the counting board, with zero as an empty square — a
location before it was a symbol — and a written zero only in the thirteenth
century. Indian decimal notation with zero as a number is the one that
transmits everywhere. Fibonacci brought it into Latin commerce in 1202, and
three centuries of resistance followed, part conservatism and part a
legitimate fear of forgery.

The African absence records the constraint as a negative: counting was highly
developed and no written positional notation formed, so a calculation could
not outlast its performance.

## Contested

The Bakhshali manuscript's early radiocarbon dates are disputed because the
folios span widely different periods, so the Indian zero should be modelled
as established by about 500 with earlier dates possible. Whether the Indian
zero drew on Babylonian or Greek astronomical placeholders is unresolved and
should be one route among independent development. The Maya zero is
uncontested as independent.

## Hook

Converts calculation from skilled manipulation into written procedure, so
`execution delegability` for arithmetic rises sharply and MTH-020's
algorithms become practical rather than merely possible. It should arise
wherever a large recurring volume of calculation makes the notation's own
bookkeeping the constraint, which is why it is invented independently several
times. Zero should be modelled as two steps — marking an empty place, which
removes ambiguity, and treating zero as a number, which opens algebraic
manipulation — reachable separately. And adoption should carry a trust cost
the older notation did not have, because a figure that is cheap to calculate
is also cheap to alter, so uptake depends on auditing institutions as well as
on the notation's advantages.
