---
id: COM-070
former_ids: [COM-031]
title: Route Infrastructure Decay and Maintenance
legacy_ref:
domain: commerce
domains_touched: [agriculture, engineering]
tier: core
status: drafted
confidence: medium
date_start: -500
date_end: 1500
date_precision: era
regions: [east-asia, middle-east]
region_variants:
  - region: east-asia
    date_start: 600
    note: China's Grand Canal requires continuous dredging and lock maintenance to stay navigable at all, since silt deposition is constant rather than occasional — periods of dynastic weakness or transition (most visibly around the Yuan-Ming transition) show up directly as reduced grain-transport capacity to the capital as maintenance lapses, with restoration requiring renewed, deliberate state investment rather than the canal simply recovering on its own.
  - region: middle-east
    date_start: -500
    note: Qanat underground channel systems require constant manual desilting and shaft maintenance to keep drawing groundwater to the surface at all; archaeologically documented abandoned qanats across the Iranian plateau show maintenance lapse, not merely aquifer depletion, as a recurring cause of a system going from productive to worthless within a generation or two of neglect.
absence:
  - region: mediterranean
    note: The Roman road network's exceptionally durable construction (deep multi-layer foundations, cambered drainage) let a meaningful share of the network remain physically usable for travel and local trade for centuries after the Western Empire's 476 collapse ended any centralized maintenance program — a case where high enough upfront construction investment substitutes for standing maintenance spend, rather than every infrastructure type requiring it in equal measure.
prereqs: [COM-080, AGR-100]
enables: []
see_also: [AGR-150]
contested: false
contested_note: ""
stats_touched: [route capacity]
tags: [grand-canal, qanat, roman-roads, infrastructure-maintenance]
---

Physical trade infrastructure — canals, roads, underground channels,
caravanserai — silts up, washes out, or falls out of repair by default,
not only under attack or deliberate destruction, requiring standing
maintenance investment to hold its carrying capacity rather than only
construction investment to create it once.

## Timing

Qanat systems require ongoing maintenance from their construction (from
roughly -500 onward across the Iranian plateau); the Grand Canal's
maintenance-dependency is visible from its major early expansions (from
around 600) through repeated documented cycles of neglect and restoration
across subsequent dynasties, with this entry's window extending through
1500 to cover several full neglect-and-restoration cycles.

## Mechanism

Unlike a one-time construction cost, a route's physical carrying capacity
decays continuously by default — silt accumulates in a canal or
underground channel whether or not anyone is using the route, and a road
surface degrades under weather regardless of traffic volume — meaning
`route capacity` behaves as a depleting pool (design principle 4) rather
than a fixed asset: it requires standing, recurring investment merely to
hold steady, and an interruption in that investment (a weak or
transitioning state, a diverted budget) shows up as reduced throughput
even with no external shock, attack, or change in demand at all.

## Social consequence

Because route maintenance requires sustained, boring, recurring
investment rather than a single dramatic achievement, it's exactly the
kind of capacity a weakening or distracted state lets lapse first — the
resulting capacity loss then compounds, since a partially-silted canal
or partially-abandoned qanat is more expensive to fully restore than it
would have been to simply maintain continuously, converting deferred
maintenance into a growing rather than a fixed restoration cost.

## Regional specifics

The Grand Canal's maintenance history shows political transition
translating directly and immediately into physical capacity loss —
dredging lapses during dynastic weakness show up as reduced grain
shipments to the capital well before any other sign of state decline is
visible elsewhere. Qanat abandonment across the Iranian plateau shows the
same mechanism at a smaller, more local scale, with maintenance lapse
alone (not aquifer exhaustion) explaining many documented cases of a
system going from productive to worthless within a generation. The Roman
road network's unusual durability shows the depleting-pool dynamic is
conditional on construction quality relative to maintenance investment,
not universal — a road built durable enough can substitute years of
absent maintenance for the higher upfront construction cost that bought
that durability.

## Hook

`Route capacity` decays by a standing rate absent maintenance spend,
independent of any shock, raid, or change in demand — the domain's first
genuine pool mechanic, distinct from `route knowledge`'s currently
one-way-accumulating treatment, and one where deferred maintenance
compounds into a restoration cost larger than continuous upkeep would
ever have required.
