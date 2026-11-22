---
id: FIN-145
title: Income Tax
legacy_ref:
domain: finance
domains_touched: [law]
tier: standard
status: drafted
confidence: medium
date_start: 1799
date_end:
date_precision: year
regions: [europe, americas]
region_variants:
  - region: americas
    date_start: 1861
    note: The United States' first federal income tax (Revenue Act of 1861, a flat 3 per cent on incomes above a fixed exemption, under Civil War emergency) is a non-European adoption of the same levy, but it is flat and assessed by return rather than by a full schedular system, so it is a weaker version of the British form. Graduated rates followed in 1862, and a durable graduated federal income tax only after 1913 (see FIN-207).
absence:
  - region: east-asia
    note: Imperial Chinese fiscal administration raised revenue from land, grain and registered households (FIN-020; the Tang two-tax reform of 780 in LAW-075 levied on landholding and wealth, not on a flow of income). Officials could count land and people well, so the state had less need for a return of income; No sourced imperial income schedule is cited here before the late Qing and Republican reforms (to be checked).
  - region: middle-east
    note: Pre-modern Islamic states levied on produce (the tithe on harvests), on flocks, on non-Muslims by poll (jizya) and on specified wealth as zakat (LAW-210). These are levies on stocks, produce or persons, not a schedular tax on income with its own return. A late Ottoman case of this kind is not verified in this draft, so the absence is stated only for the pre-modern pattern.
prereqs: [FIN-020, LAW-100]
enables: [FIN-207]
see_also: [FIN-208, FIN-110, LAW-075, LAW-210, FIN-240]
contested: false
contested_note: ""
stats_touched: [withholding coverage, income evasion rate]
tags: [income-tax, taxation, withholding, schedular-tax, self-assessment, pitt]
---

A levy on a flow of income, assessed by schedule, by the taxpayer's own return, or by withholding at source, as distinct from taxes on land, persons, goods or sales.

## Timing

The anchor is William Pitt's income tax, enacted in 1799 to finance the war with France and first levied on that year's income (source: Daunton, _Trusting Leviathan: The Politics of Taxation in Britain, 1799-1914_, 2001; chapter for the 1799 act to be checked). It charged a rate on income above a threshold, with graduated relief on lower incomes. It was repealed in 1802 and reimposed in 1803 by Henry Addington, whose revision set the schedular structure (land, farm profits, public funds, trades and professions, public salaries) that this entry treats as the mechanism.

Earlier taxes come close without being the mechanism. Athenian eisphora was a levy on capital assigned to wealth groups in emergencies, and England's land tax of 1692 was assessed on land values parish by parish. The Florentine catasto of 1427 asked households to declare property and the income from it, but it taxed a stock of wealth with income estimated from that stock, not a general flow of income.

## Mechanism

A stock is easy to observe: a field or a house can be counted once and taxed each year. A flow is observed only when it is paid, and then only if someone records the payment. An income tax therefore needs a schedule that defines the base by source, a channel of information from payer to state, and a means of checking that the information is complete.

Three channels exist, each with a different enforcement cost. In assessment by return, the taxpayer declares income, usually under oath, and the state checks the declaration against outside evidence. In third-party reporting, a bank, landlord or company reports what it paid. In withholding at source, the payer deducts the tax before paying the income out, so the state collects it in the act of payment, and the cost of collection moves onto the payer. The British 1799 form used withholding for public-stock interest and public salaries, where the paymaster was an arm of the state, and relied on return for trades and professions. Mass withholding for ordinary wage earners came later, with the United Kingdom's pay-as-you-earn system from 1944 (Daunton, 2001; source to be checked).

The schedule decides what the base is, and most of the bureaucratic work sits there. Each source category needs definitions, allowances and deduction rules, and inspectors who can compare a return against outside records. That requires the census and cadastre of FIN-020, since the state must already know who lives where and who holds what, and a staff recruited and trained on the principles LAW-100 describes. Without such a staff, returns are taken at face value and the yield depends on the honesty of the declarant.

The main constraint is the share of income that reaches a payer the state can see. Call this withholding coverage. Income paid by a state office or a large employer is nearly fully covered; rents, farm profits and self-employed earnings are not. Evasion is worth most where coverage is lowest and the rate is highest. The tax is also tied to borrowing, since its revenue services war debt (FIN-110), which is why income taxes have appeared most often in wars.

## Social consequence

The first change is that a household must disclose its income to an official, as a recorded person rather than as part of an unnamed mass. The disclosure is not neutral. In the early British form the income of a married woman was assessed to her husband, so the household was taxed as a unit under the head of household (source to be checked). The tax carried a rule about family power as well as a rate.

The second change is that the burden falls differently on wage earners and on people who earn by declaration. A salaried worker whose tax is withheld sees a smaller net pay packet and cannot reduce the amount by declaring less. A shopkeeper, farmer or landlord who files a return controls part of the figure, and evasion becomes a social practice with its own norms about what "everyone" under-declares. Fairness disputes over income tax from the nineteenth century onward concentrate on this gap.

The third change is reach. The 1799 tax reached a narrow propertied group. It became a mass tax in the world wars, when withholding and a wider base brought ordinary wage households into the system, a transformation LAW-075 treats as a wartime institutional opening.

## Regional specifics

Britain is the anchor. The United States' first federal income tax came in 1861, a flat 3 per cent on incomes above a fixed exemption (the 3 per cent rate and $800 threshold are confirmed in general sources; Brownlee, _Federal Taxation in America_, 1996, still needs its page checked). Its flat rate and return-based assessment make it a weaker form than the British schedule.

East Asia's imperial states raised revenue from land, grain and registered households, which were easier to count than a flow of income. The Tang two-tax reform of 780 (LAW-075) moved taxation toward landholding and wealth, not income. Pre-modern Islamic states relied on tithes on produce, poll taxes and zakat on specified wealth (LAW-210). The late Ottoman case is not verified here and is left open.

## Hook

Once mechanised, the income tax would apply a per-class schedule to income flows, with two state-facing variables. `withholding coverage` (the share of each class's income that passes through a state or employer payer) sets how much of the base is collected without a return. `income evasion rate` (the share of uncovered income that goes unreported) rises where coverage is low and the rate is high, and feeds back into `legibility` from FIN-020 as reporting and inspection expand.
