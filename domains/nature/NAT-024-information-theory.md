---
id: NAT-024
title: Information Theory
legacy_ref:
domain: nature
domains_touched: [engineering]
tier: standard
status: drafted
confidence: high
date_start: 1948
date_end: 1948
date_precision: year
regions: [americas]
region_variants: []
absence: []
prereqs: [NAT-003, NAT-018]
enables: []
see_also: [NAT-015, ENG-021, NAT-009, NAT-013, NAT-014]
contested: false
contested_note: ""
stats_touched: []
tags: [information-theory, shannon, entropy, channel-capacity]
---

Quantifying information itself — treating a message's uncertainty as a
mathematically measurable quantity and a communication channel's capacity
to carry it as a hard limit that holds regardless of engineering
cleverness — split out from NAT-015's algorithm-and-computability lineage
as a mathematically distinct achievement in its own right.

## Timing

Claude Shannon's "A Mathematical Theory of Communication" (1948)
establishes the field in a single, largely self-contained paper, defining
information entropy and proving the channel capacity theorem within the
same work.

## Mechanism

Shannon's entropy measure quantifies how much genuine uncertainty a
message resolves, using the same probabilistic mathematics (NAT-018) that
statistical mechanics applies to particles — applied here to symbols in a
message instead. The channel capacity theorem then proves a hard
mathematical ceiling on how much information any communication channel
can carry reliably, given its bandwidth and noise level, a limit that
exists independent of any particular engineering implementation, in the
same structural shape as NAT-009's thermodynamic efficiency ceiling for
heat engines.

## Social consequence

A provable ceiling on channel capacity redirects engineering effort the
same way NAT-009's efficiency ceiling did for heat engines: rather than
searching indefinitely for a coding scheme that could communicate more
reliably than a channel's proven capacity allows, engineers can instead
focus on approaching that known, mathematically fixed limit as closely as
practical — a reframing of communication engineering (feeding directly
into the digital hardware ENG-021 eventually implements) around a hard
mathematical constraint rather than open-ended trial and error.

## Regional specifics

This is a genuinely American achievement, developed at Bell Labs within
the same mid-century period this record's other physics entries (NAT-013,
NAT-014) note as one where American institutions were becoming central to
theoretical work that earlier in the century had been substantially
European in origin.

## Hook

A gate on how much information any channel can carry regardless of
engineering cleverness — structurally the same shape as NAT-009's
thermodynamic ceiling, and split out from NAT-015's algorithm-and-
computability lineage as a mathematically distinct achievement resting
directly on probability theory (NAT-018) rather than on computability.
