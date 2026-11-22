---
id: FIN-033
title: Poll Tax
legacy_ref:
domain: finance
domains_touched: [law, religion]
tier: standard
status: drafted
confidence: medium
date_start: -300
date_end:
date_precision: century
regions: [europe, middle-east, south-asia, east-asia]
region_variants:
  - region: europe
    date_start: 1377
    note: England's parliamentary poll taxes of 1377, 1379 and 1380 were levied per head on the adult population; the first flat, the second graded by rank, the third flat again. The 1381 collection, with commissioners checking ages and status on the spot, triggered the Peasants' Revolt. A flat rate on labouring households and the inspection of persons are the two features that made it provoke revolt.
  - region: europe
    date_start: 1722
    note: Russia's poll tax (podushnaya podat') was enacted by Peter the Great's decree of 1722 and collected from 1724, after the first revision census, which had been ordered in 1718. It charged each male "soul" on the census roll, so the census was the tax base, and the lag between revisions turned the roll into a landlord's liability.
  - region: south-asia
    date_start: 1564
    note: Under the Mughals the jizya on non-Muslim subjects was abolished by Akbar in 1564 (some sources date full abolition to 1579) and reimposed by Aurangzeb in 1679. The levy moved with the ruler's policy as much as with the fiscal state.
  - region: east-asia
    date_start: 624
    note: The Tang zu-yong-diao combined a grain levy, a labour service and a textile levy, each charged per registered adult male (ding) against the land allotted to him. The ding was the fiscal unit, and the register that counted it was the base.
absence:
  - region: central-asia-steppe
    note: No sourced poll tax on the steppe is cited in this draft. Herding societies were assessed through herds and households instead (FIN-175), because a count of people who move with their animals is a poor base for a head levy.
prereqs: [FIN-020]
enables: []
see_also: [FIN-032, FIN-175, REL-040, LAW-080]
contested: true
contested_note: "Dispute over the purpose of the jizya: a fee for protection and exemption from military service, a status marker of subordination, or a mix whose weights changed by period and place. Model it as a categorical levy whose purpose weights follow the ruler's policy record, not as one fixed reading."
stats_touched: [concealed adult share, per-head burden]
tags: [poll-tax, capitation, census, jizya, registration]
---

A flat per-person levy on the adult population, charged regardless of land, income or goods, assessed through a census and tied to registration. It is the head-based counterpart of the land tax (FIN-032) and rests on the same register (FIN-020). It is not the herd levy (FIN-175).

## Timing

Roman sources attest a head-based element in the tributum from about the later fourth century BCE: the tributum capitis paid by the capite censi, citizens registered by head alone, with no property to assess. The start date is approximate and to be checked. The jizya is attested from the early Islamic conquests (the seventh century is the conventional date). The late-antique capitatio, reorganised under Diocletian in the late third century, paired a count of heads with a count of land units (Jones, *The Later Roman Empire* (1964), taxation chapter; page to be checked). Its land-linked side is the subject of FIN-032. The Tang zu-yong-diao (from 624) charged per adult male. The entry has no end date.

## Mechanism

A poll tax charges a rate per head, so its base is a count of people in a category, not what they own. Three consequences follow.

- **Revenue is rate times count.** There is no valuation, harvest or inventory. Collection cost per unit of revenue falls with scale, which is why head taxes appear in large states with weak assessment capacity.
- **It is regressive by construction.** A fixed sum takes a larger share of a poor household's income. Exemptions (age, sex, poverty, service, religion) are the only way to bend it, and each one is a classification someone must check on the ground.
- **The base is the register.** An unregistered adult pays nothing; every registration creates a liability. Households respond to the census as well as the rate. The concealed adult share measures how much of the base the state fails to see, and its upstream driver is `legibility` (FIN-020): a state that cannot enumerate people cannot collect a head tax from them.

The jizya adds a category to the base. Non-Muslim subjects are liable as a status, and classical jurists set rates by wealth tier (Abu Yusuf, *Kitab al-Kharaj*, gives three tiers of 48, 24 and 12 dirhams; figure to be checked). The category decides liability and wealth decides the rate. REL-040 covers the toleration arrangement the levy sat inside.

## Social consequence

Each adult pays whatever their wealth. A household with three adult sons pays for three. Women and children are often outside the base, which shifts the load onto adult men and makes the age and sex cut-off the most contested line in the law. The poorest households carry the heaviest share of income, and a flat rate is easiest to collect where people have least.

Flight is the household's answer. A registered adult who leaves changes the register, and a village or landholder may go on paying for someone gone. In Russia, landlords paid on male souls until the next revision, so deaths and absconded serfs stayed on the rolls as a loss; Gogol's *Dead Souls* (1842) turns that census lag into an image.

The English case shows the other edge. The 1377 and 1379 levies fell on labouring households whose wages could not easily carry a flat sum. The 1381 collection added commissioners who examined people's ages and status on the spot. The inspection of bodies, not the sum alone, turned grievance into revolt (Dobson, *The Peasants' Revolt of 1381* (1970); schedules and rates to be checked). The jizya's payment also marked subordination, which is why it recurs as a status question and not only a fiscal one.

## Regional specifics

**Europe.** Roman tributum and the late-antique capitatio (see Timing); England's 1377 to 1381 levies (region variant); Russia's podushnaya, decreed in 1722 and collected from 1724 on the census ordered in 1718, the clearest case of a tax that was the census.

**Middle East.** The jizya on non-Muslim subjects, a per-head category with rates set by wealth tier.

**South Asia.** Mughal jizya, abolished by Akbar in 1564 and reimposed by Aurangzeb in 1679 (Richards, *The Mughal Empire* (1993); to be checked).

**East Asia.** The Tang zu-yong-diao charged each registered adult male against land allotted to him; the ding register was the base (Twitchett, *Financial Administration under the T'ang Dynasty* (1963); to be checked).

**Central Asian steppe.** Not sourced here; herding peoples were assessed through their herds (FIN-175).

## Contested

The purpose of the jizya is disputed. One reading treats it as a fee for protection and for exemption from military service, which the Quranic text (9:29) and later jurisprudence support. Another treats it as a status marker of subordination. A third sees a mix whose weights changed across periods and places. The Quranic text, the legal treatises and the administrative records do not agree on the weighting. Source pointers: Tritton, *The Caliphs and their Non-Muslim Subjects* (1930), relevant chapters to be checked; Abu Yusuf, *Kitab al-Kharaj*, for the wealth tiers (to be checked). Model: the levy is categorical; weight its purpose between revenue and status by the ruler's policy record rather than fixing one reading.

## Hook

Once mechanised, a poll tax yields registered adults times a per-head rate, so revenue rises with census investment and never with wealth. The concealed adult share falls with census investment and rises with the rate and with flight. Revolt risk rises when the rate exceeds a configurable share of a household's subsistence surplus while collection involves inspection of persons (the 1381 combination), so the outcome follows the prior register and policy rather than a player choice (design principle 1). Category exemptions, jizya-style, let the designer set two tiers without changing the base.
