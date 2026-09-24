---
id: MED-150
title: The Global Network
legacy_ref:
domain: media
domains_touched: [law, commerce, diplomacy, philosophy, war]
tier: core
status: drafted
confidence: medium
date_start: 1969
date_end: 2010
date_precision: decade
regions: [americas, east-asia, south-asia, africa, middle-east, europe]
region_variants:
  - region: americas
    date_start: 1969
    note: ARPANET and the protocol decisions that followed build in a design assumption with enormous downstream consequence — that the network carries packets without regard to what they contain, leaving intelligence at the endpoints. That choice is what makes the marginal cost of becoming a publisher near zero, because no permission is required from anything in the middle. It was an engineering decision about robustness and became a political settlement by accident.
  - region: east-asia
    date_start: 1997
    note: China's network governance is the most developed instance anywhere of information sovereignty reasserted against a medium designed to ignore borders, and it belongs at the centre of the entry rather than as a counterexample to it. Filtering at the international gateway, licensing of domestic platforms, and intermediary liability that makes the platform police its own users together demonstrate that the border-crossing property is a policy variable rather than a fact of the technology — which is the finding, and it is not the one the medium's designers expected.
  - region: south-asia
    date_start: 2000
    note: India's adoption is mobile-first and largely skips the desktop stage, which produces a differently shaped network — access through a device that is personal rather than household, in a market where data is priced for very low incomes, reaching a population whose prior media access was broadcast and print in languages the network initially did not serve well. The sequence a region arrives by shapes what the network is for.
  - region: africa
    date_start: 2007
    note: M-Pesa in Kenya shows the network carrying value rather than text, on ordinary mobile handsets and without banks, reaching a population that no prior financial or media infrastructure had served. It is the clearest case that the network's significance in a region need not be publication at all — where the binding constraint was payment rather than speech, the same infrastructure solved a different problem first.
  - region: middle-east
    date_start: 2010
    note: The 2010-2011 uprisings are the contested case and should be carried as contested rather than as demonstration. Networked coordination was real and documented; the claim that it was causal is much weaker than it appeared at the time, and the subsequent decade — in which the same tools served surveillance, identification and counter-mobilisation at least as effectively — is part of the same case rather than a separate one.
  - region: europe
    date_start: 1989
    note: The Web is specified at CERN in 1989-1991 and released without patent or licence, which is the second decision after packet-neutrality that determines the medium's economics. A publishing system placed in the public domain by an institution with no commercial interest in it removes the last gatekeeper that a proprietary standard would have installed — and the later platform consolidation happens on top of an open standard rather than through control of one.
absence:
  - region: central-asia-steppe
    note: Terrestrial network infrastructure requires fixed plant — cable, exchanges, base stations — and therefore the same settlement density that defeated the telegraph here (MED-100) and the telephone's local loop (MED-110). Satellite and mobile eventually close most of the gap, which makes this the one point in the domain where a genuine absence is in the process of ending rather than persisting; the entry should model coverage here as late, expensive, and arriving through a different physical channel than anywhere else rather than as permanently absent.
  - region: oceania
    note: The reverse of MED-130's finding, and the cleanest inversion available. Radio served scattered islands better than anything before it because a signal crossing water costs nothing extra. A packet network reinstates the cable — undersea fibre landing at a few points, with everything beyond a landfall on expensive satellite backhaul — so the region that broadcast served best is served worst again, for exactly the reason the telegraph failed here. The wire constraint MED-130 removed returns with the network that replaced it.
prereqs: [ENG-330, MED-120, MTH-087, MTH-072]
enables: []
see_also: [MED-130, MED-140, MED-100, MED-090, LAW-225, ENG-340, MTH-080, PHI-410, MED-080, MED-070, MED-020, MED-110, REL-258, MED-085, MED-115, MED-105]
contested: true
contested_note: "Two disputes worth modelling. First, whether networked media produce political mobilisation: the strong claims made around 2010-2011 have not held up, and the same infrastructure has since served surveillance, identification and counter-mobilisation at least as effectively as it served organisation. Model the network as lowering coordination cost for whoever is coordinating, with no inherent direction — which is the same posture MED-130 takes on broadcast, where the Algerian and Rwandan cases run the identical mechanism to opposite ends, and it should be modelled the same way here for the same reason. Second, whether platform intermediation is a technical necessity or a business-model artefact is unresolved: the discovery problem at network scale is real, but ranking optimised for engagement is a choice about what to solve for rather than a consequence of solving it. Treat re-intermediation as reliably occurring and its particular shape as contingent, so that a network with different funding produces a different gatekeeper rather than none."
stats_touched: [intermediary dependence, information sovereignty, simultaneity of attention]
tags: [internet, web, platform, packet-switching, re-intermediation, great-firewall, m-pesa]
---

The first medium in which the marginal cost of becoming a publisher is
also near zero. That dissolves the publisher as a gatekeeping institution
— and then the gate returns in a form nobody elected, which is the part
worth modelling.

## Timing

ARPANET from 1969, the Web specified and released without licence between
1989 and 1991, mass adoption through the 1990s, mobile-first adoption
across much of the world from the 2000s, and platform consolidation
substantially complete by 2010.

## Mechanism

**The cost series is exact and this entry completes it.** MED-080's print
is one-to-many with a high production cost and a high distribution cost.
MED-130's broadcast is one-to-many with near-zero distribution cost and a
high production cost — that is MED-140's production-cost floor, and it is
why radio's from-below cases did not replicate on television. This is
**many-to-many with both costs near zero**, and the consequence is that
publishing stops being an institution and becomes an action. Religious
competition for attention (REL-258) shows both halves: every congregation and
preacher can publish, and reach then re-concentrates on the few preachers the
platforms surface.

`intermediary dependence` therefore falls further than at any point in this
domain's history, including MED-070's translation and MED-020's alphabet.
Both of those lowered the cost of *reading*. This lowers the cost of
*being read*, which no previous entry does at all.

**Then the intermediary returns as the platform, and this is the entry's
real content.** A medium where anyone can publish creates a discovery
problem that did not exist when publishing was scarce — with everything
available, the binding constraint moves from access to attention. Ranking,
recommendation and moderation solve it, and in solving it reconstitute an
editorial function: deciding what is seen, by whom, in what order. Read
against MED-130, **this is the same stat moving the other way** —
`intermediary dependence` collapses as publishing is democratised and
climbs again as discovery is centralised.

The second intermediary differs from the first in two ways that matter. It
is **less accountable**, because it never claimed the role — an editor
asserted judgement and could be argued with, while a ranking function
denies exercising any. And it operates at a **scale no editor ever did**,
applying one set of criteria to a substantial fraction of humanity's
reading at once.

**Border-crossing becomes contested by a different instrument.** MED-130
established that broadcasts cross borders without transit and that the only
defence is jamming — a pure standing cost with no output. A packet network
crosses borders the same way and cannot be jammed, but it can be filtered
at the gateway, and its intermediaries can be made liable for what their
users do. So `information sovereignty` is defended by **legal and
infrastructural pressure on intermediaries rather than by interference with
the signal**, which is cheaper, more precise, and more effective than
jamming ever was. The medium designed to ignore borders turns out to be
more governable than the one that merely crossed them.

## Social consequence

The networked database also completes a lineage that began with paper: the
card index (MED-085) first made records movable, extensible and searchable by
many clerks at once, and the database removes the card's physical position
altogether. The reader's experience inverts. Under scarcity the problem was obtaining
things worth reading; under abundance it is selecting among more than can
be read, which transfers real power to whoever performs the selection
without the transfer being visible as one. People experience a ranked feed
as what exists rather than as what was chosen.

`simultaneity of attention` also fragments, which reverses MED-090's
mechanism. A periodical put a population in front of the same content at
the same hour; on-demand access removes both the same and the hour. What
replaces the shared public is a large number of partially overlapping ones
— and since MED-090's account of national identification rests on
simultaneity, the entry should expect that identification to weaken where
scheduled media are displaced.

And the network reaches populations that no prior medium in this domain
served, which is why its most consequential early use in some regions is
not publication at all. Where the binding constraint was payment rather
than speech, the same infrastructure solved that first.

## Regional specifics

China's network governance belongs at the centre. Gateway filtering,
domestic platform licensing, and intermediary liability together show that
the border-crossing property is a policy variable rather than a technical
fact — the most developed instance anywhere of information sovereignty
reasserted against a medium designed to ignore it, and not the outcome the
medium's designers anticipated. India's mobile-first adoption produces a
differently shaped network because the sequence of arrival shapes what the
network is for. M-Pesa shows the infrastructure carrying value rather than
text to a population no prior system served.

The two absences invert two earlier findings. The steppe fails on fixed
plant for the same reason the telegraph (MED-100) and the telephone's local
loop (MED-110) failed there, though satellite and mobile are closing it —
the one absence in this domain that is ending rather than persisting.
Oceania is the sharp one: the region broadcast served best is served worst
again, because a packet network reinstates the cable that radio had made
unnecessary.

## Contested

Whether networked media produce mobilisation is unresolved and the strong
2010-2011 claims have not held. The same infrastructure has since served
surveillance, identification and counter-mobilisation at least as well.
Model the network as lowering coordination cost for whoever is
coordinating, with no inherent direction — the posture MED-130 already
takes, where the Algerian and Rwandan cases run one mechanism to opposite
ends.

Whether platform intermediation is technically necessary or a business-model
artefact is also unresolved. The discovery problem at this scale is real;
ranking for engagement is a choice about what to optimise rather than a
consequence of having to optimise something. Model re-intermediation as
reliably occurring and its shape as contingent, so a differently funded
network yields a different gatekeeper rather than none.

## Hook

Completes the cost series — print is one-to-many with both costs high,
broadcast is one-to-many with cheap distribution and expensive production,
this is many-to-many with both near zero — so publishing stops being an
institution and becomes an action, and `intermediary dependence` falls
further than anywhere else in this domain because every prior entry
lowered the cost of reading and this one lowers the cost of being read.
Then it should climb again, from the same mechanic rather than a new one:
abundance moves the constraint from access to attention, ranking solves
that, and solving it reconstitutes an editorial function that is less
accountable than the editor because it denies being one, and operates at a
scale no editor had. `simultaneity of attention` fragments, which should
weaken the national identification MED-090 builds on it. And
`information sovereignty` is contested by filtering and intermediary
liability rather than by jamming — cheaper, more precise, and more
effective, so the medium built to ignore borders proves more governable
than the one that merely crossed them.
