---
id: MTH-045
title: The Calculus
legacy_ref:
domain: mathematics
domains_touched: [nature, engineering, war]
tier: core
status: drafted
confidence: medium
date_start: 1400
date_end: 1872
date_precision: century
regions: [south-asia, east-asia, europe]
region_variants:
  - region: south-asia
    date_start: 1400
    note: Madhava of Sangamagrama and the Kerala school derive infinite series for sine, cosine and arctangent, with correction terms to accelerate convergence, some two centuries before comparable European results. Nilakantha's Tantrasangraha of 1501 and Jyesthadeva's Yuktibhasa of around 1530 carry the results with demonstrations. The work serves astronomical computation and is organised as series methods rather than as a general theory of derivatives and integrals.
  - region: east-asia
    date_start: 1680
    note: Japanese wasan mathematics develops independently under Seki Takakazu from the 1680s and Takebe Katahiro, whose 1722 infinite series for the square of the arcsine is found by numerical pattern and generalised. It is organised in schools that transmitted methods as guarded lineage knowledge, alongside sangaku — problem tablets hung publicly in shrines — so the same tradition was simultaneously secret in its methods and public in its results.
  - region: europe
    date_start: 1665
    note: Newton from the mid-1660s and Leibniz, publishing in 1684, develop the general calculus of derivatives and integrals and the theorem connecting them. The priority dispute between their followers was settled in Britain by a 1712 Royal Society report substantially written by Newton himself, and British loyalty to his fluxional notation isolated its mathematicians from Continental analysis for roughly a century, until the Analytical Society's reforms from 1812.
absence:
  - region: middle-east
    note: Sharaf al-Din al-Tusi around 1200 analysed cubic equations using a procedure that amounts to finding a maximum by what is in effect a derivative, and the line was not taken further into a general calculus. The absence is not of the insight but of a consumer. Islamic astronomy was organised around predictive tables (NAT-015) rather than a quantitative mechanics of continuous motion, so no problem pressed repeatedly for a general method of calculating change, and a technique without a demanding application remained a technique.
prereqs: [MTH-015, MTH-025, MTH-035]
enables: [NAT-170, NAT-180, NAT-210, NAT-220, MTH-070]
see_also: [NAT-040, NAT-120, NAT-015]
contested: true
contested_note: "Three disputes. First, whether the Kerala series reached Europe through Jesuit contacts on the Malabar coast in the sixteenth century is proposed, notably by Joseph and by Almeida, and unproven: no document of transmission has been found, and model it as plausible rather than established. Second, whether the Kerala results constitute calculus depends on definition — they are powerful infinite-series methods without a general concept of derivative and integral or the theorem linking them — so model independent development of series methods rather than an independent full calculus. Third, the Newton-Leibniz priority dispute is resolved in modern historiography as independent invention, and the entry should carry the dispute's institutional cost to British mathematics rather than its original question."
stats_touched: [intellectual authority]
tags: [calculus, kerala-school, madhava, wasan, seki-takakazu, newton, leibniz, priority-dispute]
---

Continuous change becomes calculable. A physics that could describe motion
becomes one that can predict it from a law and a present state, and almost
everything NAT does from 1820 onward is written in the language this entry
supplies.

## Timing

From Madhava's series around 1400 and the Kerala texts of 1501 and 1530,
through Newton and Leibniz from the 1660s and 1680s and Seki's wasan from the
1680s, a century of productive but unrigorous practice attacked by Berkeley in
1734, to Cauchy's rigour from 1821 and Weierstrass's by 1872.

## Mechanism

**Rates and accumulations become objects of calculation.** Before the calculus,
the speed of something whose speed is itself changing, or the area under a
curve, could be handled only case by case and approximately. The calculus
makes both general: a derivative for the rate of change at any instant, an
integral for accumulation, and a theorem connecting them. Continuous change —
motion, flow, growth, cooling — becomes something a formula can follow.

**That converts physics from description to prediction, by a route different
from NAT-015's.** Positional astronomy predicts by the length of its record: a
cycle observed often enough recurs. The calculus predicts from a law of change
and a present state, so a system never observed before can be forecast if its
governing law is known. A differential equation says how a thing changes; solve
it and the future follows from now. That is why electromagnetism (NAT-170),
thermodynamics (NAT-180), quantum theory (NAT-210) and relativity (NAT-220) are
all written in it. Newton's Principia itself is written in geometric form, so
the consumer is less Newton than the eighteenth-century analytical mechanics
that followed and every later NAT entry.

**It worked for a century without foundations.** Its practitioners used
infinitely small quantities that could not be made coherent, Berkeley mocked
them in 1734 as the ghosts of departed quantities, and results kept coming.
Cauchy and then Weierstrass rebuilt the foundations on limits, and that
arithmetisation of analysis is what set theory was later asked to underwrite —
the direct route into MTH-070.

## Social consequence

The priority dispute shows an institutional quarrel with a measurable
intellectual price. A Royal Society report of 1712, largely written by Newton,
settled it for Britain, and loyalty to his notation kept British mathematicians
outside the Continental development of analysis for about a century — a
national community choosing allegiance to a founder over the better notation,
and paying in lost results. `intellectual authority` attached to a person
rather than a method can cost a whole tradition.

Wasan shows the opposite arrangement coexisting in one tradition. Methods were
transmitted within schools as guarded lineage knowledge, while problems and
solutions were displayed publicly on sangaku tablets in shrines — secrecy of
technique alongside public display of achievement, which spreads prestige while
restricting the capability that produces it.

## Regional specifics

Kerala comes first, by two centuries, with infinite series for the
trigonometric functions serving astronomical computation, and whether those
results reached Europe is unproven. Japanese wasan develops series methods
independently from the 1680s. Newton and Leibniz produce the general calculus
with the fundamental theorem, and the dispute between their camps costs British
mathematics a century. The genuine absence is instructive: al-Tusi's analysis of
cubics around 1200 contains the key move and was not generalised, because
Islamic astronomy was built on tables rather than a mechanics of continuous
motion and nothing pressed for a general method.

## Contested

The Kerala-to-Europe transmission is plausible and undocumented. Whether the
Kerala series are calculus depends on definition; the entry models independent
series methods rather than an independent full calculus. The Newton-Leibniz
question itself is settled as independent invention, and the entry carries the
dispute's institutional cost rather than its original claim.

## Hook

Makes continuous change calculable, so prediction can run from a law and a
present state instead of from a long record — the capability every later NAT
physics entry is written in. It should be reachable as series methods by more
than one route, but become a general calculus only where a problem of
continuous motion keeps demanding a general method, so a tradition with the
technique and no such consumer does not generalise it. It should function for a
long period without secure foundations, producing results while accumulating the
foundational debt MTH-070 later calls in. And attaching `intellectual authority`
to a founder rather than a method should be able to cost a whole national
tradition its access to the better tool.
