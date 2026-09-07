---
id: MAR-009
title: Chronometer and Coordinates
legacy_ref:
domain: maritime
domains_touched: [nature, engineering]
tier: standard
status: drafted
confidence: high
date_start: 1714
date_end: 1773
date_precision: decade
regions: [europe]
region_variants: []
absence:
  - region: middle-east
    note: Earlier Islamic and Chinese astronomical traditions achieve highly accurate latitude determination and star-position catalogs (feeding directly into MAR-003's instrument lineage) without producing a comparable solution to the specific longitude-at-sea problem, which requires precision timekeeping rather than better astronomical observation alone — a case where a closely related but distinct technical problem stayed unsolved elsewhere despite strong adjacent capability.
prereqs: [MAR-003, ENG-029]
enables: []
see_also: []
contested: false
contested_note: ""
stats_touched: [route knowledge]
tags: [chronometer, longitude, harrison, navigation]
---

Latitude (position north-south) is directly readable from star angles and
was solved by MAR-003's techniques centuries earlier; longitude (position
east-west) requires knowing the time difference between a ship's location
and a fixed reference point precisely enough that no astronomical
observation alone could solve it — only a clock accurate enough to survive
a long sea voyage could.

## Timing

Britain's 1714 Longitude Act offers a large cash prize for a workable
solution; John Harrison's marine chronometers, developed and refined from
the 1730s, are validated as meeting the accuracy requirement by 1773 sea
trials.

## Mechanism

A sufficiently accurate, temperature- and motion-resistant mechanical clock
kept set to a home-port reference time lets a navigator compare that fixed
time to local noon (readable from the sun) and convert the difference
directly into longitude — the entire solution is a precision-engineering
problem (building a clock that keeps time through months of a rolling,
temperature-swinging voyage) rather than an astronomical or mathematical one,
which is why it resists solution for so long despite latitude having been
solved for centuries.

## Social consequence

Reliable longitude determination sharply reduces shipwreck and
misnavigation risk on long ocean passages, with a corresponding reduction in
marine insurance risk premiums (MAR-011) and a corresponding increase in the
commercial and naval value of a captain and crew's ability to specify
position with real confidence rather than dead-reckoning estimate.

## Regional specifics

This is a genuinely Britain-concentrated achievement in this domain, unlike
most other entries — there is no comparably successful independent
longitude-by-timekeeping solution elsewhere in this era to region-balance
against. The absence case is instructive rather than merely acknowledging a
gap: Islamic and Chinese astronomical traditions solved a closely adjacent
problem (accurate star-position and latitude determination) to a high
standard, showing the longitude problem's resistance to solution wasn't
about general astronomical sophistication but specifically about precision
mechanical timekeeping under maritime conditions, a much narrower and
harder engineering target.

## Hook

A sufficiently accurate marine chronometer converts longitude from an
unsolvable estimation problem into precise, calculable `route knowledge`,
compounding with celestial latitude-finding (MAR-003) to give a navigator
confident position anywhere on Earth — a precision-engineering unlock, not
an astronomical one.
