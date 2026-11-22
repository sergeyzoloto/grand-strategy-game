---
id: FIN-207
title: Progressive Taxation
legacy_ref:
domain: finance
domains_touched: [law]
tier: standard
status: drafted
confidence: medium
date_start: 1891
date_end:
date_precision: year
regions: [europe, americas]
region_variants:
  - region: americas
    date_start: 1913
    note: The United States' graduated federal income tax, made possible by the Sixteenth Amendment and set by the Revenue Act of 1913, applies a low normal rate and a graduated surtax above an exemption. It repeats the Prussian mechanism a generation later in a federal state with a much larger base of salaried and self-declared income. Rate-table details are to be checked against the 1913 text.
absence:
  - region: east-asia
    note: Imperial Chinese levies on land and grain (FIN-020; the Tang two-tax of 780 in LAW-075, levied on landholding and wealth) were set as charges on holdings, not as a rising rate on income. I did not find a sourced graduated income schedule before the late Qing and Republican reforms (to be checked).
prereqs: [FIN-145]
enables: []
see_also: [FIN-208, FIN-200, FIN-240, LAW-210, LAW-075, FIN-020]
contested: true
contested_note: "The dispute is about the size of the behavioural response to a steeper schedule at the top. Feldstein (1995) reads the taxable-income response to rate changes as large and mostly avoidance, so a steeper top rate loses revenue faster than the static model says. Saez, Slemrod and Giertz (2012) review the evidence and find the reported-income response large but a real labour-supply response small. Model it as a branch: the top-band yield is reduced by a reporting elasticity and a real elasticity, with the split set by config rather than picked."
stats_touched: [rate progression, exemption threshold]
tags: [progressive-tax, rate-schedule, prussia, miquel, graduated-income-tax, bracket]
---

A rate schedule in which the rate rises with the taxed base, as distinct from a flat rate on income or a fixed levy per person.

## Timing

The first systematic graduated income tax in a major state is the Prussian income tax of 1891, the work of the finance minister Johannes von Miquel. The law is dated 24 June 1891 and applied from 1892 (Ullmann, _Der deutsche Steuerstaat_, 2005; contemporary German sources confirm the date). It replaced a per-class tax on persons with a tax on income from all sources, rising from under one per cent at the exemption line to four per cent at the top band; the 4 per cent top rate is confirmed by a 1909 British parliamentary answer. The exemption line is 900 marks in a contemporary British report, but another German source gives 420 marks, so the line stays open until the 1891 text is checked.

Earlier systems come close without being rate schedules on income. Athenian eisphora assigned liability to wealth groups in emergencies. Prussia's class tax of 1820 charged a fixed sum by social class, not by income (to be checked). The nearest precedent is Britain's 1799 income tax (FIN-145), which gave graduated relief on incomes from £60 to £200 and then charged a flat rate above that. That is graduation at the bottom of the schedule only, with a flat top, so it is a partial precedent.

## Mechanism

A progressive schedule maps taxable income to a rate, through thresholds (bands) with a marginal rate for each. The tax is the sum of each slice of income times that slice's rate, so the average rate rises with income and the marginal rate on the next mark rises too. Below the exemption line the rate is zero. The state controls three settings: the exemption line (who pays at all), the slope (how fast the marginal rate rises) and the top rate (what the highest slice bears).

Progression meets FIN-145's information problem. A schedule can only be as steep as the state can see income. The steepest bands fall on the highest incomes, which most often come from rents, interest and business profit rather than wages, and so are the least covered by withholding. Raising the slope therefore raises the return to concealment exactly where information is weakest. Deductions work the same way: a deduction is worth more to a top-band payer, so a generous allowance can flatten the schedule without changing the printed rates. Thresholds not indexed to prices also shift burden over time (bracket creep).

A schedule also moves burden between classes, and the direction depends on the comparison. Against a per-person levy, it moves liability from the poor majority to richer households; against a flat income tax, from the middle to the top. Each comparison has its own winner and loser.

## Social consequence

The exemption line divides the population into people who pay the income tax and people who do not. In Prussia in 1892 the taxed group was a small minority of households (source to be checked). Most workers and peasants stood outside the line, while the taxed minority had a direct stake in every budget decision. The line works as a notch: a household just above it faces a visible jump in its bill.

Progression is widely accepted as a principle of fairness, and disputes concentrate on thresholds and deductions, since those decide who actually pays. In Prussia assessment was made by local committees, so the assessment was itself a social process. Accounts of the reform's passage place landowning and conservative interests among its opponents (to be checked), and the fight was over who would be assessed and on what, more than over the printed rates.

The link to the welfare state is indirect. Bismarck's social insurance of the 1880s was financed by contributions, not by a progressive income tax (LAW-210). Progressive income taxation became the fiscal base for later welfare spending in Britain, where the 1909 budget introduced a super-tax on high incomes (source to be checked; Daunton, _Trusting Leviathan_, 2001). There the top band carried a politically chosen share of the cost of social programmes.

## Regional specifics

Prussia is the anchor, and the German Reich's income taxes followed its form. The 1913 federal graduated tax in the United States is the second clear case, after the Sixteenth Amendment (Brownlee, _Federal Taxation in America_, 1996; figures to be checked).

Imperial China's levies on land and grain were charges on holdings, assessed through the registration system in FIN-020, not rates that rose with income. Graduation needs a base the state can measure per taxpayer, and those registers did not give that.

## Contested

The dispute concerns how much revenue a steeper top rate loses to behaviour. Feldstein, "The Effect of Marginal Tax Rates on Taxable Income: A Panel Study of the 1986 Tax Reform Act" (_Journal of Political Economy_, 1995), reads large responses in reported taxable income to top-rate changes as mainly avoidance, so a steeper top band yields less than a static calculation predicts. Saez, Slemrod and Giertz, "The Elasticity of Taxable Income: Evidence and Implications" (_Journal of Economic Literature_, 2012), review the evidence: reported-income responses at the top are large, but the real labour-supply response is small, and the split between the two decides the optimal top rate.

The conditional model keeps both channels and sets the split by config. The top band's yield is reduced by a reporting elasticity (avoidance, which FIN-145's coverage limits) and by a real elasticity (work and investment), with both weights as placeholders rather than a chosen answer. A steeper top band then costs revenue through reporting in proportion to uncovered income, which ties this dispute to FIN-145's `withholding coverage`.

## Hook

Once mechanised, the schedule would be a list of thresholds and marginal rates applied to declared income, with `rate progression` (how fast the marginal rate rises across bands) and `exemption threshold` (the income below which a household pays nothing) as adjustable settings. The top band's yield would fall as `income evasion rate` rises with slope in uncovered income, so a steeper schedule would raise class burden on covered incomes while losing revenue on uncovered ones.
