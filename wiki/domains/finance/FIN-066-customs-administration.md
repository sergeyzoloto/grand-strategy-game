---
id: FIN-066
title: Customs Administration
legacy_ref:
domain: finance
domains_touched: [commerce, maritime, law]
tier: standard
status: drafted
confidence: medium
date_start: -400
date_end:
date_precision: century
regions: [europe, mediterranean, east-asia, middle-east, south-asia, americas]
region_variants:
  - region: mediterranean
    date_start: -400
    note: Athens levies a harbour duty of about 2 per cent (the eikoste) on goods landed and shipped at Piraeus, and Rome collects portoria at frontiers, with the quadragesima Galliarum of about 2.5 per cent on goods entering Gaul. Both are fixed-rate duties at a chokepoint, collected by posts rather than by household assessment. The date is approximate and the rates need checking against the sources.
  - region: east-asia
    date_start: 971
    note: The Song dynasty sets up a maritime trade office (shibosi) at Guangzhou to assess and collect duties on foreign ships and cargo. Under the Qing, the Canton system confines foreign trade to one port, and the superintendent of customs there collects the duties on the same chokepoint logic.
  - region: europe
    date_start: 1429
    note: The Sound Toll, levied by Denmark on ships passing between the Baltic and the North Sea, is collected at one strait from 1429 until 1857. Its surviving registers make it the best-documented early chokepoint customs in Europe.
absence:
  - region: americas
    note: The Inca state moved goods through its road network, state storehouses and labour tribute rather than through duties at frontier posts. No customs duty on border-crossing goods is documented as a revenue head. The state controlled the flow by controlling its storehouses, so a chokepoint toll had no work to do. The absence claim needs checking against the sources.
prereqs: [FIN-020, FIN-065]
enables: [MAR-150, COM-190]
see_also: [FIN-035, FIN-068, MAR-185, COM-130, MAR-150, COM-190]
contested: false
contested_note: ""
stats_touched: [entrepôt share, duty evasion rate, inspection coverage, trade-flow throughput, legibility]
tags: [customs, duties, bonded-warehouse, valuation, drawback, transit, chokepoint]
---

The institution that collects duties on goods crossing a border or a port: where goods are valued, bonded, held and inspected, and what that costs the trade passing through.

## Timing

Duties on goods at a point of passage are far older than the 1500 anchor that this ID carries. Athens took its harbour duty at Piraeus in the fifth and fourth centuries BCE (the eikoste, about 2 per cent; source to be checked against Xenophon's Poroi for the wider revenue context). Rome collected portoria at frontiers, with the quadragesima Galliarum on goods entering Gaul (source to be checked in the Digest title on portoria). Kautilya's Arthashastra describes a superintendent of tolls and customs at market and frontier posts, but its date is disputed, somewhere between the late Mauryan period and the early centuries CE. The Song set up its Guangzhou maritime trade office in 971 (confirmed in general sources; the page in Lo Jung-pang, China as a Sea Power, 1127-1368, still needs checking). The Sound Toll ran from 1429 to 1857 (confirmed in general sources; the Sound Toll Registers, Danish National Archives, still need checking).

Western Europe reaches the common form of this mechanism around 1500: rate books with official valuations, salaried officers at fixed posts, and bonded storage as a regular right. The earlier chokepoint duties are the older form the entry starts from.

## Mechanism

A customs post is cheap to run for a simple reason: every good crossing a line passes through a few places, so the state assesses and collects at those places instead of at every household and parcel. The cadastre (FIN-020) needs a record of each holding; a customs post needs a record of goods passing one gate. The chokepoint is what makes the duty cheap, and the same chokepoint is what makes it a strategic object (COM-130).

Three design choices shape the revenue.

First, valuation. A specific duty charges per unit (per cask, per bale, per hundredweight). It is easy to administer but does not follow price, so its real value falls when prices rise. An ad valorem duty follows price but needs a valuation, either an official book of rates or the declared invoice value checked by audit. Under-declaration is the main evasion route, and its payoff rises with the rate.

Second, bonds, warehouses and transit. Goods can be landed into a bonded warehouse with the duty deferred until they are released for home use. Goods re-exported are refunded the duty (drawback; see FIN-065 on English and Dutch practice). Goods in transit can cross a territory under bond and pay nothing if they leave by the same route. These arrangements let an entrepôt work (MAR-185), but each one moves the risk onto the state's own records, because the state now has to track goods it has not yet taxed.

Third, inspection and throughput. Each inspection adds dwell time, and dwell time costs freight and the value of the goods. The state chooses how intensively to inspect: the optimum is where the marginal revenue from one more inspection equals the marginal throughput it costs the port. A higher inspection coverage raises the duty collected and lowers the number of ships that come back.

The rate also has a ceiling. Above some duty rate, smuggling pays better than declaration, and the revenue curve bends down (MAR-150). The clearest case is the English tea duty, which stood at about 119 per cent before the Commutation Act of 1784 cut it to about 12.5 per cent, and revenue rose once smuggling fell (the 119 and 12.5 per cent rates are confirmed in standard references on the Commutation Act of 1784; the page in Ashworth, Customs and Excise, 2003, still needs checking). Customs can also be farmed (FIN-035), which trades the state's valuation and inspection costs for a lessee's margin.

## Social consequence

Importers pay the duty on the border, but they pass it on in price, so the household that buys sugar, wine or cloth pays it whether or not it ever sees a customs house. The burden falls most on households far from the port, where the price includes carriage and the duty.

Those who handle goods at the post are exposed to changes in the bonded system. Porters, boatmen and carters lose work when handling moves into a bonded warehouse, and they gain when a port grows. Small traders, who lack the capital to post a bond or to hold goods while the duty is deferred, lose the bonded privilege to large merchants with credit. The customs officer's position also carries local power, fees and perquisites, so inspection can become a source of delay and of bribery.

Where the duty is high, coastal families come to depend on smuggling, which is the subject of MAR-150, and the customs house becomes the local enemy. Enforcement then sets communities against the state's own officers.

## Regional specifics

In Europe, the Sound Toll made one strait the revenue base for two centuries and gave Denmark a stake in the Baltic trade routes (COM-130 covers the wider contest over narrow seas). England's export duty on wool, the Old Custom, is attested from the 1270s (source to be checked), and the official Book of Rates gave valuations the state could enforce. The Dutch and English re-export drawbacks, and the Spanish gates at Seville and Cadiz, are covered in FIN-065; this entry takes only the duty mechanism from them.

In the Mediterranean, Athens' 2 per cent harbour duty and the Roman portoria show a duty placed at the chokepoint from an early date. The Ottoman capitulations set fixed low import duties, around 3 to 5 per cent, for European merchants (FIN-065), so the Ottoman state's customs revenue was constrained by treaty as well as by need. Ottoman customs were also farmed (FIN-035).

In East Asia, the Song maritime office at Guangzhou assessed foreign ships and cargo at a single port. Under the Qing Canton system, the superintendent of customs (the Hoppo) collected duties under the licensed-merchant arrangement, and part of the surplus went to the imperial household (Van Dyke, The Canton Trade, 2005; the allocation needs checking). Ming prohibitions on private overseas trade turned legal trade into smuggling by reclassification (MAR-150).

In South Asia, the Arthashastra's superintendent of tolls is an early case, and the Mughal customs at Surat charged a few per cent (FIN-065 covers that absence and its fiscal reasons).

## Hook

Customs posts placed at chokepoint nodes collect a value duty on flows through the node: revenue = flow value x duty rate x (1 - duty evasion rate), where evasion rises once the rate passes the cost of smuggling and falls with the inspection coverage the state pays officers to maintain. Each post adds dwell time to every flow it inspects, so raising inspection coverage cuts trade-flow throughput and can shift entrepôt share to a rival port (MAR-185).
