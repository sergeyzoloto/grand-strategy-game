---
id: FIN-032
title: Land Tax
legacy_ref:
domain: finance
domains_touched: [agriculture, law]
tier: standard
status: drafted
confidence: medium
date_start: -594
date_end:
date_precision: century
regions: [east-asia, south-asia, middle-east, europe]
region_variants:
  - region: south-asia
    date_start: 1580
    note: Mughal revenue administration (zabt, under Akbar's reforms) measures cultivated area with a standard rod, sets rates by crop, and fixes the assessed demand from a ten-year average of prices and yields. The base is the land's assessed productive value, not the cultivator's harvest in a given year.
  - region: middle-east
    date_start: 1450
    note: Ottoman tahrir surveys record each village's households and plough units, and the timar system assigns the revenue of those units to a cavalry holder. The assessment is a register entry collected against, not a fresh measurement of each harvest. A separate tithe-type levy on crops runs alongside it.
  - region: europe
    date_start: 1692
    note: England's land tax of 1692 charges four shillings in the pound on the true yearly value of land. From 1696 it became a fixed quota on valuations that were not revised, so the amount owed stops tracking the land's rent and the burden drifts away from its productive value.
absence:
  - region: europe
    note: Western European kingdoms before the sixteenth century had no general parcel-level land tax. Crown revenue came from demesne, feudal aids and the taille, which assessed households by estimated means. Without a parcel register (FIN-020) there was no base to charge a land tax on, so the state taxed people rather than fields.
prereqs: [FIN-020]
enables: []
see_also: [FIN-033, FIN-175, FIN-030, REL-130, LAW-080, AGR-200, AGR-050]
contested: true
contested_note: "Dispute over what the Lu 'first levy per mu' of 594 BCE actually taxed: private cultivated area, a yield share that formalised existing practice, or a shift from labour service to a levy in kind. Model it as a branch weighted by the prior cadastral state (FIN-020), not as a fixed base."
stats_touched: [assessment drift, tenant incidence share]
tags: [land-tax, assessment, cadastre, fiscal-base, incidence]
---

A levy assessed on the land itself, by area, graded quality or assessed productive value, as a state fiscal system. It is not a tax on the produce (the tithe, REL-130) and not the rent or service a lord or tenure-holder takes (LAW-080, AGR-200).

## Timing

The earliest clear case is Lu in the Spring and Autumn period. The Chunqiu records under the fifteenth year of Duke Xuan (594 BCE) a "first levy per mu" (初税亩), and the Zuozhuan commentary on the same entry calls it contrary to ritual. The record confirms a change in how the ruler took his share of cultivated land, not what the measure was in practice; hence century precision.

Mughal area-and-yield assessment was systematised under Akbar's revenue administration in the 1570s and 1580s. Ottoman tahrir surveys are attested from the fifteenth century. England's tax begins in 1692 as a standing quota until the Land Tax Redemption Act of 1798 let owners buy it out. Source pointers for the numbers below: Habib, *The Agrarian System of Mughal India 1556–1707* (1963), zabt chapter; Inalcik, *The Ottoman Empire: The Classical Age 1300–1600* (1973), timar and tahrir chapters; Brewer, *The Sinews of Power* (1989), taxation chapter. Page numbers to be checked.

## Mechanism

A land tax must settle its base first. The record shows three.

- **Area.** A fixed amount per measured unit of cultivated land, adjusted by a soil or water grade. It needs a survey and little else. It ignores yield unless grading corrects for it.
- **Yield or produce.** The state takes a share of the harvest. This responds to good and bad years alike, but the harvest must be measured and divided each season, moving the administrative weight onto the threshing floor and granary (AGR-050 covers the accounting).
- **Assessed productive value.** A valuation of what the land would yield or rent for, set once or periodically, with a rate applied. Mughal demand used a ten-year average of prices and yields with a stated share of produce, one-third in the figure usually cited (Habib, to be checked). England's 1692 tax used parish rental valuations.

The second choice is fixed or reassessed. A fixed valuation costs nothing to keep and gives owners a predictable obligation, but its real burden moves with prices and rents the valuation no longer reflects. A reassessed valuation tracks productivity but needs a standing survey office and a new round of objections each time. The gap between assessed and actual value grows with the years since the last reassessment; this entry calls it assessment drift.

Incidence separates the legal payer from the economic one. Under a fixed lease the tenant carries any rise in assessment until the lease renews; an owner who re-sets rent passes it on at once. Where collection runs through intermediaries (Mughal zamindars, Ottoman timar holders), part of the burden stays with those who keep a share of what is gathered.

Every variant rests on the cadastre (FIN-020): a parcel register, measured areas, grades, and a list of who holds and works what. Without it the base becomes a village estimate, cheaper to write and easier to negotiate. The head-based levy (FIN-033) needs a census; the tithe needs only the harvest and a local collector. The land tax needs a survey of fields, not of people.

## Social consequence

For a cultivator the land tax is tied to the field, not to the year. A bad harvest carries the same assessed sum as a good one. The usual responses are borrowing (FIN-030) and sale, and the slow result is land passing to larger holders. A reassessment reaches households as a shock. Under the Mughal system, measured reassessment met resistance from cultivators and intermediaries, and the gap between assessed and collected revenue was a standing administrative complaint.

Who pays in practice depends on who is recorded as payer. In England the occupier was often the legal payer, with deductions from rent allowed; whether that shifted the burden onto tenants depended on lease terms (to be checked). For the landless, the land tax is felt only through rent.

## Regional specifics

**East Asia.** The Lu measure of 594 BCE is the earliest attested case; what it taxed is disputed (see Contested). Han-era land taxation was assessed on cultivated area and tied to the household registers of FIN-020.

**South Asia.** Mughal revenue fixed crop-by-crop rates and set the assessed demand from averaged prices and yields. The collected revenue often fell short of the assessed demand; the difference records how far the assessment ran ahead of the land (Habib, to be checked).

**Middle East.** The timar granted the revenue of named villages to a cavalryman in return for service. The tahrir register recorded households and plough units, and the assessment on those units sat beside tithe-type levies on crops (öşür), whose religious lineage is a separate matter (REL-130). The timar is a service tenure (the feudal analogue is at LAW-080); the land tax is the register behind it.

**Europe.** England's 1692 tax charged four shillings in the pound on yearly value; after 1696 it was a fixed quota on old valuations, so its burden on land drifted from land value. Medieval Western kingdoms had no general parcel tax (see absence).

## Contested

The meaning of the 594 BCE Lu measure is disputed. Readings include: (a) a tax on the cultivated area of private fields, extending a levy that had fallen on the ruler's public fields through labour service; (b) a yield-share levy that formalised a share already taken; (c) a reform replacing labour service with a levy in kind, which the Zuozhuan commentary treats as an unauthorised change. The primary sources are short and do not settle it. Model: treat the early Lu case as a weighted branch, not a fixed fact. Weight an area base (low drift, low responsiveness) against a yield base (high responsiveness, higher collection cost) by the prior cadastral state (FIN-020). Source pointer: Chunqiu and Zuozhuan, Duke Xuan 15 (宣公十五年). Modern secondary interpretation: source to be checked; not verified in this draft.

## Hook

Each parcel carries a land-tax base fixed at institution: area (a rate per grade), yield (a share of the last harvest), or assessed value (rate times valuation). Under an assessed-value base, assessment drift accumulates each year since the last reassessment: state revenue erodes in real terms and owners' burden falls with it. A reassessment resets drift at a survey cost (a `legibility` spend), restores real revenue, and raises the burden on fixed-rent tenants in one step, which can push households into debt (FIN-030).
