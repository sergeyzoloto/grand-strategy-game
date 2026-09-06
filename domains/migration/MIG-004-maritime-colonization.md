---
id: MIG-004
title: Maritime Colonization (City-States and Trading Posts)
legacy_ref:
domain: migration
domains_touched: [commerce]
tier: core
status: drafted
confidence: high
date_start: -1200
date_end: 1600
date_precision: era
regions: [mediterranean, east-asia, southeast-asia, africa]
region_variants:
  - region: southeast-asia
    date_start: 600
    note: Srivijaya and later Malay/Javanese trading-post networks reproduce the low-footprint, commercially-anchored colonization pattern independently of the Mediterranean tradition, built around control of strait chokepoints rather than agricultural hinterland.
absence:
  - region: africa
    note: East African Swahili coast city-states (from roughly 800) are themselves the receiving end of this pattern rather than the colonizing end for most of this period — Indian Ocean trading-post colonization runs into the coast from Arabia, Persia, and India, making the Swahili coast a node in someone else's network before it becomes, in its city-states, a comparable network of its own.
prereqs: []
enables: [MIG-012]
see_also: [MIG-005]
contested: false
contested_note: ""
stats_touched: [settler footprint]
tags: [colonization, trading-post, city-state, maritime, phoenician, greek]
---

One of two structurally distinct colonization mechanics this domain tracks:
a small, commercially-motivated founding population establishing a fortified
trading post or city-state, dependent on maritime connection and local
commercial relationships rather than on displacing or farming a large
hinterland.

## Timing

Phoenician and Greek colonization of the Mediterranean and Black Sea coasts
runs from roughly -1200 through the classical period; comparable
trading-post-anchored colonization recurs across multiple later eras and
regions (Southeast Asian strait ports from ~600, and again with the Italian
maritime republics and early Portuguese/Dutch trading-post empires) through
1600.

## Mechanism

A trading post minimizes founding population and requires no large-scale
displacement of an existing population — it needs only a defensible harbor,
a reliable maritime supply line, and a workable commercial relationship with
whatever polity controls the adjacent hinterland. The founding cost is low
and mostly capital/logistics rather than manpower, which is what lets a
comparatively small city-state project this pattern across a very wide
maritime range simultaneously (dozens of Greek colonies within a few
centuries).

## Social consequence

Because the founding population stays small and commercially specialized,
maritime colonies tend to develop a distinct, often cosmopolitan merchant
identity tied more to the network of sister colonies and the home city than
to the immediately surrounding hinterland population, with relatively low
cultural or demographic pressure on that hinterland compared to the
frontier-settlement pattern.

## Regional specifics

The Southeast Asian strait-port pattern (Srivijaya and successors) reaches a
structurally similar low-footprint, chokepoint-control model entirely
independently of the Mediterranean tradition, suggesting this is a
close-to-inevitable colonization strategy wherever maritime trade routes
create valuable chokepoints, not a culturally specific Mediterranean
invention. The Swahili coast's position as the receiving end of Indian
Ocean trading-post colonization before becoming a source of its own
comparable network shows the same location can occupy either role at
different points in its history.

## Hook

Maritime/trading-post colonization instantiates a low `settler footprint`,
low-displacement colonization type — cheap to found, wide-ranging, dependent
on maintained sea-lane connection rather than on land control, and mechanically
distinct from the frontier-settlement type in MIG-005.
