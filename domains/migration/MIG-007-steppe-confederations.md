---
id: MIG-007
title: Steppe Confederations
legacy_ref:
domain: migration
domains_touched: [war, diplomacy]
tier: core
status: drafted
confidence: medium
date_start: -200
date_end: 1200
date_precision: century
regions: [central-asia-steppe, east-asia]
region_variants:
  - region: east-asia
    date_start: -200
    note: The Xiongnu confederation forms in direct, sustained contact and rivalry with Han China, its internal political organization and demands (tribute, trade access, marriage alliances) shaped as much by the settled empire next door as by internal steppe dynamics — an early template every later steppe confederation covered elsewhere in this record broadly repeats.
absence:
  - region: africa
    note: The Sahelian and East African savanna-pastoralist zones support large mobile herding populations without producing a comparable pattern of large, rapidly-assembled multi-tribal military confederations capable of contesting a settled empire at range — plausibly reflecting a different local balance of pasture geography, horse availability, and settled-state proximity/wealth worth raiding, rather than any lesser capacity for large-scale political organization among pastoralist populations generally.
prereqs: [MIG-001, MIG-003]
enables: [MIG-008]
see_also: []
contested: false
contested_note: ""
stats_touched: [confederation cohesion]
tags: [steppe, confederation, xiongnu, nomadic-empire, mobility]
---

The political-military organization of a steppe confederation is WAR's;
this entry owns the specific mobility infrastructure (horse riding-at-range
plus wagon-based nomadism) that makes assembling and sustaining one possible
at all, and why that capability recurs across many unrelated steppe polities
rather than being a one-off historical accident.

## Timing

Recurring from at least the Xiongnu's formation (~-200) through the medieval
period, wherever the mobility preconditions (MIG-001, MIG-003) are already in
place and a triggering event — usually pressure from or opportunity against
a wealthy settled neighbor — brings scattered pastoral groups under one
military leadership.

## Mechanism

Mounted, wagon-supported mobility lets a steppe confederation rapidly
concentrate a large mobile force from widely dispersed pastoral groups, raid
or negotiate with a settled neighbor, and disperse again before a slower
settled military can respond in kind — the confederation's real capability
isn't population size (usually much smaller than the settled empire it
rivals) but assembly and dispersal speed, a direct inheritance of the
mobility mechanics this entry sits on top of.

## Social consequence

Steppe confederations are typically loose, personality- and
success-dependent political structures — cohesion holds together only as
long as raiding or tribute income keeps flowing to subordinate chiefs, which
is why they tend to fragment quickly on a leader's death or a run of failed
campaigns, a structurally different stability profile from the settled
empires they contest.

## Regional specifics

The Xiongnu case sets a template — confederation formation and internal
cohesion shaped substantially by the wealth and policy of the neighboring
settled empire — that recurs across essentially every later steppe
confederation in this record, suggesting the mechanic is generic to
steppe/settled-empire proximity rather than specific to any one steppe
culture. Comparable large-scale pastoralist populations in Africa not
producing an equivalent confederation pattern at the same scale suggests the
mechanic additionally requires a nearby settled power rich and reachable
enough to be worth this kind of rapid mobilization against.

## Hook

Steppe confederation formation is gated on the mobility preconditions
(MIG-001, MIG-003) plus proximity to a settled power worth mobilizing
against, and produces a fast-forming, fast-dissolving `confederation
cohesion` stat tied to ongoing raid/tribute success rather than to any fixed
institutional base — cohesion that can collapse in a single bad season, unlike
a settled state's slower-decaying institutions.
