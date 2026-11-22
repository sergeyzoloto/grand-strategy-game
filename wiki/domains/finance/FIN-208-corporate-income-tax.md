---
id: FIN-208
title: Corporate Income Tax
legacy_ref:
former_ids: []
domain: finance
domains_touched: [law]
tier: standard
status: drafted
confidence: medium
date_start: 1909
date_end:
date_precision: year
regions: [europe, americas]
region_variants:
  - region: europe
    date_start: 1842
    note: Britain's income tax, reintroduced by Peel in 1842, charged the profits of companies and businesses under Schedule D and collected tax at source, but as one income tax on persons and bodies rather than as a separate levy on the corporate form. It is a precedent for taxing business profit, not for the separate corporate tax described here.
absence:
  - region: americas
    note: Before 1909 the United States reached profit held inside a corporation only when it was distributed to owners, through the owners' own tax. The absence of a corporate levy was a gap in the income tax's reach, not a deliberate policy of exempting corporate profit.
prereqs: [LAW-130, FIN-170]
enables: []
see_also: [FIN-070, FIN-240, FIN-020]
contested: true
contested_note: "Who bears the corporate income tax (shareholders as owners of capital, workers through lower wages, or consumers through higher prices) is disputed, and the answer depends on whether capital is mobile across sectors and countries. Model incidence as a branching split of the same levy, selected by the economy's openness and capital mobility, rather than as one fixed burden. See the Contested section."
stats_touched: [corporate_tax_rate, retained_earnings_share, profit_shift_rate, corporate_tax_incidence]
tags: [corporate-tax, double-taxation, retained-earnings, incidence, fiscal-capacity]
---

A levy on the net profit of an incorporated firm, assessed on the firm as a legal person (LAW-130) and separate from the income tax owners pay on what they receive.

## Timing

The United States enacted its Corporation Excise Tax in 1909, inside the Payne-Aldrich Tariff Act: a tax of 1 per cent on the net income of incorporated firms above an exemption. The Supreme Court upheld it in Flint v. Stone Tracy Co. (1911) as an excise on the privilege of doing business in corporate form. An earlier federal income tax of 1894 had also reached corporate profit, but the Court struck it down in Pollock v. Farmers' Loan and Trust Co. (1895), so 1909 is the first durable federal tax on corporate net income. The 1909 rate (1 per cent) and exemption ($5,000) are confirmed in contemporary and later summaries of the Payne-Aldrich Tariff Act; section 38 of the statute itself and W. Elliot Brownlee, Federal Taxation in America (2nd ed., 2004), still need checking against the text. The federal income tax of 1913 (16th Amendment) then made the corporate levy a backstop to the personal tax.

## Mechanism

A personal income tax falls on the owner when income reaches the owner. A corporation can hold its profit: it retains earnings, reinvests them, or keeps them as cash, and the owner's tax waits until a dividend is paid or the shares are sold. Left alone, that deferral lets a large part of the income tax base sit untaxed inside the corporate form. The corporate levy is the backstop: it reaches profit where it is earned and held, whether or not it is ever distributed.

The levy works because limited liability (FIN-170) makes the firm a separate, bounded legal unit with its own books, and legal personhood (LAW-130) lets the state assess the firm as the holder of that income. Without the separate entity, the same profit would simply be assessed on its owners.

A second effect comes from taxing the same profit twice: once at the firm, and again when it is paid as a dividend and taxed as the owner's income. This raises the cost of distributing relative to retaining, which pushes firms towards retention, towards debt (interest is usually deductible, dividends are not), and towards payments deductible at the firm, such as salaries or buy-backs in place of dividends. King and Fullerton, The Taxation of Income from Capital (1984), set out the framework (source pointer only; page to be checked).

The firm also responds on the base side. Profit can be booked where the rate is lowest through intra-group loans, transfer prices and the placement of patents. The fiscal leak is the one FIN-240 describes for tax havens. The effective rate is simple: retained profit faces the corporate rate alone, while distributed profit faces the corporate rate and then the owner's rate on what is left. The gap between those two drives retention, and the shifting response decides how much of the base the levy keeps.

## Social consequence

The household experiences this tax as a levy on "the company" rather than on people. A family does not see the levy in its pay packet. Its effect arrives indirectly: through a pension fund's returns, a firm's wage offers, or the price of a product. That indirection is the political appeal of the tax and also its problem, since the people who bear the economic cost are often not the ones who receive the statutory bill.

Who in the household bears it is contested (see the Contested section). Shareholders bear it when capital cannot move. Workers bear some of it when capital can move but labour cannot, through lower wages over time. Consumers bear some of it through prices in concentrated markets. Local communities bear it when a firm uses exemptions or relocation to avoid the levy, leaving the services it would have funded to be paid for by residents and smaller firms.

Incorporation becomes a status line. A sole proprietor or partnership pays the owner's tax alone, while a firm that incorporates to obtain limited liability takes on the separate levy. The burden therefore falls on firms large enough to need the corporate form.

## Regional specifics

The United States is the anchor case. Its 1909 tax was the first federal levy on corporate net income that survived in court, and the 1913 income tax then gave the state a personal and corporate pair. Britain's 1842 income tax under Schedule D charged business and company profit as part of one income tax, collected at source; it is a precedent for taxing profit, not for a separate corporate tax (source pointer: Peter Daunton, Trusting Leviathan, 2001, on Schedule D; chapter to be checked). Continental European states often taxed firms through trade and business levies rather than a separate corporate income tax. Germany's Gewerbesteuer is the standing example; its history is not checked here.

## Contested

The dispute is about incidence: who bears the economic burden of the tax, whatever the statutory payer. Arnold Harberger's 1962 model (The Incidence of the Corporation Income Tax, Journal of Political Economy, 70(3)) argues that in a closed economy with capital mobile across corporate and non-corporate sectors, the burden falls broadly on all owners of capital, not only on corporate shareholders. Other analyses, including those surveyed by Alan Auerbach (Who Bears the Corporate Tax? A Review of What We Know, NBER Tax Policy and the Economy, 2006; page to be checked), find that in an open economy, or where capital is less mobile, a larger share falls on immobile factors such as local labour or the rents a firm earns. Estimates vary widely by country and period.

Model this as a branching incidence split rather than a fixed fact. The levy is the same in every branch; only the split changes. A regime flag for capital openness selects a weight for each of three holders (shareholders, workers, consumers), and those weights decide which households feel the levy in their wealth, income or prices. The weights are placeholders until a source-backed split is chosen.

## Hook

Once mechanised, the entry levies a per-firm tax on annual net profit at a placeholder corporate_tax_rate and pays it into state revenue, with the base reduced by retained_earnings_share (retained profit escapes the owner's tax until distribution, so the backstop's value rises with retention). A profit_shift_rate lets a fraction of the base leave for a haven (FIN-240), and corporate_tax_incidence splits the levy's cost across shareholders, workers and consumers in the household view.
