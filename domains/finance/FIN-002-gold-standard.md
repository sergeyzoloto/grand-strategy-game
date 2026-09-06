---
id: FIN-002
title: Gold Standard
legacy_ref:
domain: finance
domains_touched: [diplomacy]
tier: core
status: drafted
confidence: high
date_start: 1717
date_end: 1971
date_precision: century
regions: [europe, east-asia, south-asia, africa]
region_variants:
  - region: east-asia
    date_start: 1935
    note: China remains on a silver standard (see COM-011's silver-shock lineage) far longer than the major Western powers, only abandoning it in 1935 under severe deflationary pressure from global silver-price movements largely outside its own control — a case of a major economy's monetary regime choice being effectively dictated by a commodity market it doesn't set.
absence:
  - region: africa
    note: Colonial African currencies in this era are typically pegged directly to the colonizing power's own currency and gold backing through currency-board arrangements, rather than participating in the gold standard as independent monetary sovereigns — monetary policy is set entirely by the colonial metropole, with the colony holding no independent gold-standard membership of its own.
prereqs: [COM-011]
enables: [FIN-003]
see_also: []
contested: true
contested_note: "Whether the classical gold standard (roughly 1870s-1914) genuinely stabilized international trade and investment by providing credible, predictable exchange rates, or primarily transmitted deflationary shocks and constrained governments' ability to respond to domestic downturns (a major line of argument for why it worsened the Great Depression, see FIN-016), is debated among monetary historians. Model the gold standard as delivering real exchange-rate predictability benefits to international trade and capital flows while simultaneously removing a state's ability to use monetary policy countercyclically — both effects real and simultaneous, with the net verdict depending on which one matters more in a given crisis rather than one effect canceling the other out."
stats_touched: [bullion stock, price level]
tags: [gold-standard, monetary-regime, currency-board, silver-standard]
---

Pegging a currency's value to a fixed weight of gold, redeemable on demand
— a monetary regime that trades away independent domestic monetary policy
for cross-border exchange-rate predictability and credibility.

## Timing

Isaac Newton's 1717 mint valuation effectively puts Britain on a de facto
gold standard; the regime spreads internationally and formalizes as the
"classical" gold standard from the 1870s, persisting with interruptions
(suspended during major wars) until the final break from gold convertibility
in 1971 (see FIN-018).

## Mechanism

A currency's value under a gold standard is fixed by law to a specific
weight of gold rather than left to float against other currencies or
domestic conditions, which gives foreign creditors and trading partners
strong confidence in the currency's future value (reducing exchange-rate
risk on cross-border trade and lending) at the direct cost of removing the
issuing state's ability to expand or contract its money supply in response
to domestic economic conditions — monetary policy becomes a function of gold
flows in and out of the country rather than of domestic policy choice.

## Social consequence

A state on the gold standard facing a domestic downturn cannot devalue or
print money to ease it without abandoning the standard entirely, which
concentrates the adjustment cost onto wages, prices, and employment instead
— gold-standard adherence functions as a credible commitment device for
international creditors precisely because it ties the domestic population's
economic experience to that commitment, whether or not domestic conditions
would otherwise call for a different policy response.

## Regional specifics

China's much longer adherence to a silver rather than gold standard, tracing
directly back to the silver-market dynamics COM-011 describes, leaves its
monetary conditions hostage to global silver price movements set mostly by
decisions made elsewhere, forcing an eventual 1935 abandonment under
deflationary pressure it had little ability to independently counteract.
Colonial African currency-board arrangements peg colonial currencies
directly to the metropole's own gold-backed currency, meaning the colony
never holds independent gold-standard membership or monetary sovereignty
of its own — monetary policy is simply inherited from the colonizing power
wholesale.

## Contested

Whether the classical gold standard stabilized international trade and
investment through credible predictability, or primarily transmitted and
worsened deflationary shocks by stripping governments of countercyclical
monetary tools, is debated, with the latter argument central to explanations
of why it deepened the Great Depression (FIN-016). Model both effects as
real and simultaneous — genuine cross-border predictability benefit,
genuine domestic-policy-flexibility cost — rather than treating the
standard's net historical verdict as settled either way.

## Hook

The gold standard trades a state's independent monetary policy for
cross-border credibility, fixing `price level` stability to gold flows
rather than domestic conditions — a commitment device whose real cost
(no countercyclical response available) only becomes visible once a state
actually needs the flexibility it gave up.
