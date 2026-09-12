---
id: MAR-335
title: Radio Navigation
legacy_ref:
domain: maritime
domains_touched: [engineering, war, commerce, diplomacy]
tier: standard
status: drafted
confidence: high
date_start: 1910
date_end: 1970
date_precision: decade
regions: [europe, americas, oceania, east-asia, global]
region_variants:
  - region: europe
    date_start: 1942
    note: Gee and then Decca were built under wartime pressure for bombing and minesweeping accuracy and converted afterwards into civil chains that European coastal shipping and fishing depended on for decades. Decca's commercial model is the interesting half — receivers were leased rather than sold, so the operator kept a continuing revenue stream from a signal it could not prevent anyone from receiving, which is the same non-excludability problem LAW-225 handles solved by contract rather than by law.
  - region: americas
    date_start: 1943
    note: LORAN was built for the North Atlantic and Pacific war effort and became the long-range civil standard, and its coverage map is the point — chains were sited where the United States had wartime need and postwar bases, so the accuracy a given stretch of ocean enjoyed was decided by someone else's strategic geography rather than by its own traffic density.
  - region: oceania
    date_start: 1945
    note: The Pacific is where the technology matters most and is provided least, since the enormous distances between fixes are exactly the conditions under which celestial navigation is worst and the chains are sparsest. A region's navigational accuracy under this system is a function of how many transmitters someone else chose to build near it.
  - region: global
    date_start: 1912
    note: Direction finding comes first and is the simplest form — a ship takes bearings on known shore transmitters and fixes its position by intersection, requiring no new infrastructure beyond stations that already existed for wireless telegraphy. This is the cheap version that spread everywhere, and the hyperbolic systems that follow are a different and much more expensive proposition.
absence:
  - region: africa
    note: Coverage followed the strategic interests of the states that built the chains, and African coastal waters outside the approaches to a handful of major ports were largely outside any hyperbolic system through the whole period. Shipping there continued on celestial and dead-reckoning practice, which is not a failure of adoption but of provision — the capability could not be bought locally at any price, because it is supplied by transmitters ashore that someone else has to build and keep running.
  - region: south-asia
    note: The same provision problem, with the additional feature that a regional power could not unilaterally acquire the capability even with the money to do so, since a hyperbolic fix requires a synchronised chain of stations spanning a wide baseline and therefore requires the cooperation of whoever holds the coastline at the other end. Navigation accuracy becomes a diplomatic question rather than a procurement one.
prereqs: [ENG-315, NAT-170]
enables: [ENG-340]
see_also: [MAR-340, MAR-350, ENG-325, LAW-225, WAR-315, ENG-230]
contested: false
contested_note: ""
stats_touched: [route knowledge, information speed]
tags: [radio-navigation, loran, decca, direction-finding, hyperbolic, position-fixing]
---

Navigation technique, and therefore MAR's under the maritime carve-out.
What changes is not how fast a ship moves but whether it knows where it is
when the sky is not visible.

## Timing

Direction finding on shore transmitters from around 1910, the wartime
hyperbolic systems Gee, Decca and LORAN from 1942-43, and their conversion
into civil chains that remained the long-range standard until satellite
positioning displaced them from the 1970s onward.

## Mechanism

Celestial navigation requires seeing the sky, and the constraint is
absolute rather than degrading — an overcast week yields no fixes at all,
not poor ones, so a vessel's position error grows without bound on dead
reckoning until the cloud breaks. Radio navigation removes the weather and
daylight dependency entirely, which converts position-fixing from an
intermittent capability into a continuous one.

The deeper change is **where the skill and the cost live**. Celestial
navigation is knowledge carried aboard in a trained navigator, a sextant,
and tables — expensive to acquire, portable, and entirely under the ship's
own control. A hyperbolic system moves all of that ashore into a
synchronised chain of transmitters, leaving the vessel with a receiver and
a chart overlay that a far less trained officer can read. The capability is
cheaper per ship and better, and it is no longer the ship's own. A vessel
in a covered area navigates well; the same vessel outside the chain has a
receiver and a crew whose celestial practice has atrophied.

This makes accuracy **a provided public good rather than a purchased
private one**, with three consequences worth modelling. Coverage follows
the strategic interests of whoever built the chains, so a stretch of ocean
gets the accuracy someone else's basing decisions gave it. The capability
cannot be acquired unilaterally, since a hyperbolic fix needs a wide
synchronised baseline and therefore the cooperation of whoever holds the
far coastline. And it can be withdrawn or degraded by the provider, which
makes dependency on it a standing strategic exposure rather than a
procurement decision — a theme ENG-340 inherits and sharpens.

## Social consequence

The master navigator's standing erodes. Celestial competence had been a
scarce, hard-won skill that conferred real authority aboard, and reading a
Decca overlay is not — so a shipboard hierarchy partly built on who could
fix the ship's position loses one of its supports. The skill also decays
collectively rather than individually: once a generation of officers has
qualified without needing it, the practice is not merely unused but
largely unavailable, which is why the loss of a chain is far more
dangerous than never having had one.

For fishing fleets the effect is sharper than for cargo shipping and runs
in an unexpected direction. Repeatable position-fixing lets a skipper
return to a productive spot precisely rather than approximately, which
converts accumulated fishing knowledge from imprecise local lore into
transferable coordinates — and makes sustained pressure on a specific
ground possible in a way it had not been. The technology that made fishing
safer also made a stock easier to exhaust.

## Regional specifics

Provision, not adoption, is the variable throughout. European and North
Atlantic waters were densely covered because that is where the wartime need
and the postwar bases were; the Pacific, where the distances between fixes
are longest and celestial navigation is least adequate, was covered
thinnest. African coastal waters outside a few port approaches had no
hyperbolic coverage at all, and South Asian operators could not have built
it unilaterally even with the funds, because the baseline required
coastline they did not hold.

Decca's leased-receiver model deserves carrying as a mechanic. A radio
signal cannot be withheld from a receiver, so the operator could not sell
access — it leased the equipment instead and collected rent on the hardware
rather than on the signal, which is the same non-excludability problem
LAW-225 resolves through licensing, solved here privately by contract.

## Hook

Removes the absolute weather and daylight gate on position-fixing, and
moves the capability from the ship to the shore. `route knowledge` stops
being carried aboard in a trained navigator and becomes a provided service
— cheaper and better inside a chain, absent outside one, and unbuyable
locally because a fix requires a synchronised baseline that needs another
polity's coastline. Coverage is therefore inherited from someone else's
strategic geography rather than chosen, it can be degraded or withdrawn by
its provider, and the dependency erodes the celestial competence that was
the fallback — so losing a chain is materially worse than never having had
one. For fishing it converts local lore into transferable coordinates,
which raises effort against a `fishery stock` rather than merely making the
voyage safer.
