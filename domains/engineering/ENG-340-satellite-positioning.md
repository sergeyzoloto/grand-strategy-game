---
id: ENG-340
title: Satellite Positioning
legacy_ref:
domain: engineering
domains_touched: [maritime, war, commerce, diplomacy, nature]
tier: core
status: drafted
confidence: medium
date_start: 1973
date_end: 2000
date_precision: decade
regions: [americas, europe, east-asia, south-asia, global]
region_variants:
  - region: americas
    date_start: 1973
    note: NAVSTAR is built as a military system and opened to civil use by policy rather than by design. The 1983 destruction of KAL 007 after it strayed into Soviet airspace prompted a public commitment to civilian availability, and Selective Availability — a deliberate degradation of the civil signal to roughly a hundred metres — remained in force until it was switched off in 2000. The sequence is the entry's core in miniature, a single operator setting everyone else's accuracy by decision.
  - region: europe
    date_start: 1999
    note: Galileo is justified explicitly and publicly as a response to dependence on a system another state can degrade or withhold, which makes it the clearest case that the strategic exposure is understood by the parties rather than merely visible to analysts. The expenditure is large, the technical gain over an undegraded GPS is modest, and the purchase is sovereignty rather than accuracy.
  - region: east-asia
    date_start: 1994
    note: BeiDou follows the same logic on a longer timescale and reaches global coverage, while Japan's QZSS takes the cheaper option of augmenting an existing constellation rather than replacing it — a real strategic choice between full independence at very high cost and improved local performance that leaves the underlying dependency intact.
  - region: south-asia
    date_start: 1999
    note: India's NavIC is a regional rather than global constellation, deliberately sized to cover the subcontinent and its approaches instead of the world, and Indian official accounts attribute the decision to an episode during the Kargil conflict in which access to precision data was said to have been refused. The account is not independently verifiable and should be treated as contested, but the resulting design choice is not — a regional system is far cheaper than a global one and buys independence only where it is actually needed.
  - region: global
    date_start: 2000
    note: Switching off Selective Availability in 2000 improved civil accuracy by roughly an order of magnitude overnight, worldwide, at no cost to any user and by a single administrative decision — the clearest demonstration available that the capability's quality is a policy variable held by one party rather than a technical property of the system.
absence:
  - region: africa
    note: The signal reaches everywhere on the surface equally, so for the first time in this chain there is no coverage gap at all. What is absent instead is any share in provision — no African state operates a constellation or an augmentation system, so the region is a pure consumer of a capability whose accuracy, continuity, and terms are decided entirely elsewhere. This is a different kind of absence from the rest of the chain and the more durable one, since it cannot be closed by building anything locally short of a satellite programme.
  - region: oceania
    note: Likewise fully covered and wholly dependent, with the additional exposure that the region's economies rest disproportionately on activities positioning has become load-bearing for — long-haul aviation, shipping, and fisheries enforcement across enormous exclusive economic zones that cannot be patrolled without it. Full signal availability coexists with maximal vulnerability to its withdrawal.
prereqs: [MAR-335, ENG-330, NAT-220]
enables: []
see_also: [MAR-370, LAW-225, WAR-315, ENG-327, MAR-100, ENG-085]
contested: true
contested_note: "Two distinct disputes. The widely-repeated account that the United States denied or degraded precision positioning to India during the 1999 Kargil conflict, prompting NavIC, appears in Indian official and semi-official statements and is not independently corroborated; treat it as a motivating narrative whose truth is uncertain but whose effect on procurement is real, which is the useful modelling posture either way. Separately, estimates of the economic cost of a prolonged positioning outage vary by more than an order of magnitude between studies, because they disagree about how quickly substitutes and fallback practice could be restored in sectors whose prior competence has decayed. Model outage cost as a wide band that widens with the number of years since the dependency was established, rather than as a point estimate."
stats_touched: [route knowledge, legibility, information speed]
tags: [gps, navstar, galileo, beidou, navic, selective-availability, positioning]
---

Canonically ENG rather than MAR. The maritime carve-out exists because
maritime instances are numerous and coherent enough to deserve one home;
satellite positioning is the opposite, serving ships, aircraft, vehicles,
artillery, container logistics, agricultural machinery and eventually
individuals, so housing it in MAR would make the sea the owner of a
capability whose maritime share is small.

## Timing

From the NAVSTAR programme's start in 1973, through initial operational
capability and the 1983 commitment to civil availability, to the ending of
Selective Availability in 2000 and the parallel constellations begun in
response to the dependency the American system created.

## Mechanism

MAR-335 moved position-fixing from the ship to the shore. This completes the
move and globalises it — the same capability, now available everywhere on
the surface simultaneously, with no coverage gaps to inherit from anyone's
basing decisions and no requirement that a user hold any territory at all.
The coverage problem that organised the entire prior chain simply
disappears.

What replaces it is a sharper version of MAR-335's provision problem.
**Accuracy becomes a single operator's policy variable**, set for everyone
at once. Selective Availability degraded the civil signal to roughly a
hundred metres for years and was switched off by decision in 2000,
improving worldwide civil accuracy by an order of magnitude overnight at
zero cost to any user. No prior capability in this record has that shape —
one party setting a quality level that every other party consumes, revisable
unilaterally and without warning, with no local investment able to change
it.

The result is a global public good with a **selectively degradable civilian
tier**, which makes precision a lever one state holds over everyone else's
economy rather than a thing others can buy. The rational response is to
build a redundant constellation at very large cost for modest technical
gain, which is what Galileo and BeiDou are and why their justifications are
stated in sovereignty terms rather than performance ones. A regional
constellation like NavIC is the cheaper variant of the same purchase,
buying independence only across the footprint that matters.

Two further properties matter mechanically. The system distributes
**precise common time** as well as position, which is what couples it to
financial settlement, power grid synchronisation and telecommunications,
and is why an outage propagates into sectors that have no apparent
connection to navigation. And dependency on it **erodes its own
alternatives** faster than MAR-335's did, because the fallback practices
are now a generation further out of use — which is why estimates of outage
cost widen the longer the dependency has been in place.

*Gap flagged, not filled: the system's operation depends on atomic
timekeeping and on relativistic corrections to clock rate. NAT-220 covers
relativity and is prereq'd here, but no entry anywhere in the corpus covers
atomic clocks or caesium standards. That is a genuine hole in NAT or ENG and
is out of scope for this build.*

## Social consequence

Wayfinding stops being a skill. Every prior navigation technology in this
record demanded trained competence from someone — celestial from a
navigator, hyperbolic from an officer reading an overlay — and satellite
positioning demands none from anyone, which removes an entire category of
expertise from ships, aircraft, armies and eventually ordinary life within
about one generation. The loss is collective rather than individual, and is
what makes a prolonged outage qualitatively worse than the same outage
would have been in 1970.

It also makes movement legible in a way no previous technology did. A
position fix that a vehicle can compute can also be reported, so fleet
tracking, catch documentation in fisheries, and individual location history
all become routine — `legibility` extended from what a state can count
where it governs to where any tracked thing has been, continuously. The
enforcement of maritime zones that had been unpatrollable in practice is
the clearest concrete instance.

## Regional specifics

Provision, not coverage, is now the entire story. Africa and Oceania have
complete signal availability and no share whatsoever in supplying it, which
is a different and more durable dependency than the coverage gaps earlier
in this chain, because it cannot be closed by building anything short of a
satellite programme. Oceania's exposure is the sharpest, since enormous
exclusive economic zones are unpatrollable without positioning and the
regional economies rest on exactly the activities that depend on it.

Europe, China, Japan and India each made a different purchase against the
same exposure — full independence, full independence on a longer timescale,
augmentation that leaves the dependency intact, and regional independence
sized to the footprint that matters. That spread of responses to one
identical risk is the most useful thing in the entry, because it shows the
decision is about how much sovereignty is worth rather than about what the
technology can do.

## Contested

The Kargil denial account is repeated widely, appears in Indian official
statements, and is not independently corroborated. Its truth is uncertain;
its procurement effect is not, which is the modelling posture to adopt — a
motivating narrative that produces a real constellation regardless of
whether the underlying episode happened as described.

Estimates of what a prolonged outage would cost vary by more than an order
of magnitude across studies, and the disagreement is not about the sectors
affected but about how fast fallback competence could be restored in
sectors that no longer teach it. Model outage cost as a wide band that
widens with the number of years the dependency has been in place, rather
than as a point estimate — the uncertainty is the finding.

## Hook

Ends the coverage problem that organised this whole chain, and replaces it
with a provision problem that no amount of local investment can solve. The
signal is available everywhere on the surface at once and requires holding
no territory, but its accuracy is one operator's policy variable, revisable
unilaterally and applying to everyone simultaneously — so a state's
precision is inherited rather than purchased, and can be degraded without
warning. The rational counter is a redundant constellation costing
enormously for modest technical gain, which is a sovereignty purchase and
should be priced as one, with a cheaper regional variant available.
Distributing precise common time as well as position couples an outage into
finance, power and telecommunications, sectors with no apparent navigational
exposure. And dependency erodes its own fallback, so outage cost should grow
with the years since adoption rather than staying fixed — the only mechanic
in this chain where the penalty for losing a capability increases the longer
you have safely had it.
