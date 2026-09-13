---
id: NAT-015
title: Positional Astronomy and Predictive Tables
legacy_ref:
domain: nature
domains_touched: [mathematics, religion, maritime, agriculture]
tier: core
status: drafted
confidence: medium
date_start: -1650
date_end: 1700
date_precision: era
regions: [middle-east, mediterranean, east-asia, americas, south-asia]
region_variants:
  - region: middle-east
    date_start: -1650
    note: Babylonian observers record the risings and settings of Venus in the tablet of Ammisaduqa and compile celestial omens in Enuma Anu Enlil, and from -652 keep astronomical diaries continuously for roughly six centuries. That record is what makes the Saros cycle usable — eclipses recur in a pattern of about eighteen years, discoverable only from generations of observations — and later Babylonian lunar theory predicts positions with purely arithmetic schemes, with no geometric picture of the heavens and no claim about what moves the moon.
  - region: mediterranean
    date_start: 150
    note: Ptolemy's Almagest fits geometric models — epicycles and deferents — to observations that include Babylonian records transmitted through Hipparchus, and his Handy Tables turn the models into working tables for computing positions. The geometry predicts well and makes no causal claim about the heavens; it is a device for reproducing observed motion, and it remains the standard for fourteen centuries because it works, not because anyone believes the circles are real.
  - region: middle-east
    date_start: 1259
    note: The Islamic zij tradition produces working astronomical tables, and the observatories at Maragha under Nasir al-Din al-Tusi from 1259 and at Samarkand under Ulugh Beg from the 1420s are state-scale institutions built to improve them — large instruments, salaried staff, and systematic programmes of observation. Both depended on a ruler's patronage and declined after their patrons died, Samarkand's after Ulugh Beg's murder in 1449, so the institution's continuity was the continuity of a dynasty rather than of the knowledge.
  - region: east-asia
    date_start: -100
    note: Chinese astronomy is a state office. The imperial astronomical bureau issued the calendar in the emperor's name, and a failed eclipse prediction was a sign that the ruler's mandate was in doubt, so accuracy was a question of legitimacy rather than of scholarship. Prediction was the office's currency to such a degree that in the seventeenth century the bureau was staffed by whoever predicted best, including foreign Jesuits, which shows what the institution valued over who held it.
  - region: americas
    date_start: 900
    note: The Dresden Codex carries Maya Venus and eclipse tables that predict over spans of centuries and include correction schemes to keep the tables aligned with the sky as small errors accumulated. It is prediction built on long observation with explicit recalibration, developed with no contact with any Old World tradition, and it shows the same core structure as the Babylonian case arrived at independently.
  - region: south-asia
    date_start: 499
    note: The siddhanta tradition from Aryabhata onward produces astronomical parameters and computational handbooks (karana) for calculating planetary positions and calendar dates, and the resulting almanac is used to fix ritual and agricultural timing across the subcontinent. The emphasis falls on reliable computation from established parameters rather than on large observatory programmes, which makes the table itself, not the instrument, the institution.
absence:
  - region: oceania
    note: Polynesian navigators held extensive and precise knowledge of star rising points and paths, used to cross open ocean, and produced no predictive tables of future positions. Without a written record, observation cannot accumulate across more generations than memory spans, so patterns that emerge only over centuries — eclipse cycles, planetary periods — cannot be found. The knowledge is superb for the present sky and the navigator's lifetime, and what is absent is the multi-generational record from which prediction years ahead is derived.
  - region: europe
    note: From roughly 500 to 1080 Latin Europe had no tabular positional astronomy. The one astronomical computation sustained was the computus for fixing Easter, and no institution kept systematic observational records or produced tables of planetary positions. Tabular astronomy returned only when Arabic tables, the Toledan tables foremost, were translated from the late eleventh century — an absence of the sustaining institution for more than five hundred years, not of astronomical interest.
prereqs: [MED-010, MTH-015]
enables: [MAR-050, REL-030, NAT-120]
see_also: [NAT-090, NAT-110, NAT-040, MTH-025, MTH-045]
contested: true
contested_note: "Two disputes about transmission, both bearing on NAT-120. How far Greek geometric astronomy depended on Babylonian parameters is documented in part — Hipparchus used Babylonian eclipse records and period relations — and its full extent is debated; model it as substantial borrowing of data and period relations rather than of method. Second, whether models developed at Maragha, including the Tusi couple, reached Copernicus is unresolved: the geometrical device appears in De revolutionibus with no documented route of transmission. Model a Maragha-to-Copernicus link as plausible rather than established. The characterisation of all these traditions as prediction without a theory of cause is itself an interpretation; Ptolemy's models are geometric rather than causal and the Babylonian schemes are arithmetic, which supports it, but the entry should present it as a claim about what the tables required rather than what their makers believed."
stats_touched: [accumulated scientific corpus, intellectual authority]
tags: [astronomy, saros, almagest, zij, maragha, samarkand, dresden-codex, astronomical-bureau]
---

Systematic observation of the sky, accumulated over generations and turned
into tables that predict where things will be. Prediction without a theory of
cause — the purest case of empirical knowledge in this domain, and the
clearest contrast with the permanence of deductive proof.

## Timing

From Babylonian Venus observations around -1650 and the continuous
astronomical diaries from -652, through Ptolemy's Almagest around 150,
the siddhanta tradition from 499, Maya tables by around 900, the Maragha
and Samarkand observatories from 1259 and the 1420s, and Chinese bureau
astronomy throughout, to around 1700.

## Mechanism

**Prediction comes from the length of the record, not from understanding
the cause.** An eclipse recurs in a cycle of about eighteen years, but the
cycle is invisible to any one observer; it emerges only from centuries of
continuous, written observation. Babylonian astronomers found it by keeping
diaries for six hundred years, and their lunar theory predicted positions
with arithmetic schemes that contained no model of the heavens and no claim
about what moves the moon. Ptolemy's geometry and the Islamic zij tables are
more elaborate and share the same structure: they reproduce observed motion
well enough to forecast it, and they are judged entirely by whether the
forecast comes true.

**This is exactly the knowledge the MTH carve-out was made to separate from
proof.** A proved result is permanent and needs no observation. An
astronomical table is the opposite in every respect. It is only as good as
the observations behind it, it drifts as small errors accumulate and must be
recalibrated against the sky — which is why the Maya tables carry explicit
correction schemes — and it can be refuted tomorrow by an eclipse arriving
at the wrong hour. It is cumulative, checkable against the world, and
perishable. The boundary rule's test puts it in NAT without ambiguity.

**The record is the capital, and it can be destroyed.** Because prediction
depends on the length and continuity of observation, the asset is the
accumulated archive and the institution that keeps adding to it — which makes
`accumulated scientific corpus` the right stat. An observatory whose patron
dies, or a bureau whose dynasty falls, loses the continuity that the method
depends on, and a destroyed archive cannot be reconstructed by cleverness.

## Social consequence

Astronomy becomes a state office with legitimacy attached, because the calendar
is political. Who fixes the calendar fixes the timing of ritual, taxation and
agriculture, and a ruler whose astronomers predicted an eclipse correctly was
visibly in accord with heaven. In China a failed prediction was a sign the
mandate was in doubt, so accuracy was guarded as a matter of state, and the
bureau employed whoever predicted best — including, in the seventeenth
century, foreign Jesuits — because prediction was the currency the office
traded in.

The institution's dependence on patronage is also its fragility. Maragha and
Samarkand were built at state scale around individual rulers and declined when
those rulers died. A method whose value depends on centuries of unbroken
observation was repeatedly housed in institutions whose lifespan was a reign,
which is a structural mismatch worth modelling: the knowledge needs dynasties
to be permanent, and dynasties are not.

## Regional specifics

Every principal case is outside Europe, and Europe appears as an absence.
Babylon supplies the record-length mechanism and the Saros cycle; Ptolemy's
geometric tables draw on Babylonian data; the Islamic zij tradition and the
Maragha and Samarkand observatories carry the method at state scale; Chinese
bureau astronomy ties it to dynastic legitimacy; the Maya Dresden Codex arrives
at recalibrated long-span prediction independently; and the siddhanta
tradition makes the computational handbook the institution. Latin Europe had
no tabular positional astronomy for more than five centuries and reacquired it
from Arabic tables. Polynesian navigation shows the absence in another form —
superb knowledge of the present sky with no written record long enough to
predict years ahead.

## Contested

How much Greek astronomy borrowed from Babylon is partly documented and partly
debated; model substantial borrowing of data and period relations. Whether
Maragha's models reached Copernicus is unresolved, which matters for NAT-120,
and should be carried as plausible. Calling these traditions prediction
without a theory of cause is a claim about what their tables required, not
about what their makers believed.

## Hook

Produces prediction from the length of an observational record rather than
from a theory of cause, so its accuracy should scale with how long and
unbroken the archive is — patterns like the eclipse cycle should be
undiscoverable by any society whose written record is shorter than the
pattern. The archive is the capital: `accumulated scientific corpus`, built
over generations and destructible at once, so an observatory that loses its
patron or a bureau that loses its dynasty should lose predictive power. Tables
should drift and require recalibration against the sky, which is the property
that marks this as empirical knowledge rather than proof. And calendar
authority should confer political legitimacy, so a failed prediction costs the
ruler, not only the astronomer.
