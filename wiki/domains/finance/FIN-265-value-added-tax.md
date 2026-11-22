---
id: FIN-265
title: Value-Added Tax
legacy_ref:
former_ids: []
domain: finance
domains_touched: [commerce]
tier: standard
status: drafted
confidence: medium
date_start: 1954
date_end:
date_precision: year
regions: [europe, americas]
region_variants:
  - region: americas
    date_start: 1967
    note: Latin American states adopted value-added-type taxes mainly in the 1960s to 1980s, and Brazil's 1960s tax reform introduced a multi-stage tax on goods that works on the same invoice-credit logic. Dates, rates and design differ by country and are not checked here; treat the 1967 date as a placeholder to be verified.
absence:
  - region: americas
    note: The United States never adopted a federal value-added tax. Its consumption taxes are state and local retail sales taxes levied at a single point of sale, which avoids the stage-by-stage invoice trail but leaves intermediate transactions untaxed and concentrates the evasion risk at the retailer.
prereqs: [FIN-020]
enables: []
see_also: [COM-190, MAR-150, LAW-210]
contested: false
contested_note: ""
stats_touched: [vat_rate, vat_exempt_share, vat_gap, vat_compliance_cost]
tags: [vat, consumption-tax, invoice-credit, regressivity, evasion]
---

A consumption tax levied at every stage of production and sale on the value each firm adds, collected through an invoice trail that lets each firm credit the tax it paid on its inputs.

## Timing

France introduced the taxe sur la valeur ajoutée by law no. 54-404 of 10 April 1954, first applying it at the manufacturing stage and extending it stage by stage; the retail stage came only in 1966. The design is credited to Maurice Lauré, a senior official in the French tax administration, who drew on the earlier academic discussion of a value-added form of turnover tax, most visibly Carl Shoup's writing (exact dates and attribution to be checked against Shoup's own work). The 1954 date is the legal introduction; the full coverage of the consumer stage came later. Source pointers: the 1954 law (to be checked against the French Journal Officiel); Ebrill, Keen, Bodin and Summers, The Modern VAT (IMF, 2001), for the history and design.

## Mechanism

A firm's value added is its sales minus its purchases from other taxable firms. A VAT charges the tax on that difference. In the invoice-credit method used almost everywhere, the firm charges tax on its sales, deducts the tax shown on its purchase invoices, and remits the net. The tax on final consumption is therefore the sum of the value-added taxes at each stage, and no good is taxed twice on the same value. That is the main advantage over a turnover tax, which taxes the same value again each time a good changes hands and so rewards vertical integration.

The method is self-policing. The buyer can claim credit only with a valid invoice from the seller, and the seller's sales record is the buyer's purchase record. Each transaction is written down twice, by parties with opposite incentives, so the state gets a cross-check without auditing every firm.

The design also has a border logic. Exports are zero-rated, so the input tax on them is refunded, and imports are taxed at the same rate as domestic goods. The state therefore taxes consumption where it occurs, which is the destination principle. Zero-rating gives a firm a reason to claim export status for sales that did not leave the country, and to invent purchase invoices that generate credit with no real transaction behind them. The carousel fraud that has affected some value-added systems is this incentive in its most organised form. The enforcement problem is like smuggling (MAR-150): the rate gap between taxed and zero-rated sales is the reward, and the checks are only as good as the records behind them.

The burden is regressive in its base. Poor households spend most of their income on consumption, so a flat rate takes a larger share of their income. Governments answer with exempt goods and reduced rates, typically food, medicine, books and some services, but each exemption costs revenue, and the benefit leaks to richer buyers of the same goods. Compliance is also heavy for small firms, and a registration threshold, which exempts small firms from the tax, creates a cliff that firms have a reason to stay below.

## Social consequence

The household pays the tax at every purchase, and it is included in the shelf price. Unlike an income tax, nothing arrives on a bill, so the burden is hard to see and easy to forget. A rate rise is therefore often less visible than a comparable rise in income tax.

The exempt and reduced-rate goods shape which goods the poor buy. A zero rate on basic food lowers the price of food for the poor relative to other goods, but the same zero rate applies to everyone's food, so its benefit is modest relative to the revenue it costs. The tax therefore changes not only the total bill but the pattern of what is affordable.

Small shopkeepers and home businesses carry the compliance cost. They must keep invoices, file returns and understand which goods carry which rate. Those who stay below the registration threshold may remain outside the system, and those who cross it face a sudden new burden.

## Regional specifics

France's 1954 adoption is the origin, and the European Community later made a common VAT a condition of its internal market (the Sixth VAT Directive of 1977 is the usual reference; source to be checked). Most European states now use VAT as their main consumption tax. The United States is the clear absence: it has no federal VAT, and its state and local sales taxes collect at retail only. Latin American adoptions began in the 1960s and continued through the 1980s; the dates and designs listed in the region variant are placeholders, not checked figures, so the region variant should be verified before the entry moves past drafted.

## Hook

Once mechanised, the entry collects a vat_rate (PLACEHOLDER) on each firm's value added and credits the tax on its purchases, so state_revenue rises with the rate and the number of taxable stages. A vat_exempt_share sets how much of household consumption is zero-rated or reduced, which shifts the regressive burden between income groups. A vat_gap, the share of potential revenue lost to false zero-rating and invoice fraud, grows with the rate-to-zero spread. Firms below a threshold pay vat_compliance_cost in lost sales and paperwork, which the mechanic applies as a cost to small businesses.
