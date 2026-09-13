---
id: MTH-035
title: Algebra as Symbolic Manipulation
legacy_ref:
domain: mathematics
domains_touched: [law, commerce, philosophy]
tier: core
status: drafted
confidence: medium
date_start: -100
date_end: 1637
date_precision: era
regions: [east-asia, middle-east, south-asia, mediterranean, europe]
region_variants:
  - region: east-asia
    date_start: -100
    note: The Nine Chapters' fangcheng method solves systems of simultaneous linear equations by arranging coefficients in an array on the counting board and eliminating column against column, handling negative quantities with coloured rods. Li Ye's tianyuan shu of 1248 writes polynomials in one unknown positionally, and Zhu Shijie's work of 1303 extends the method to four unknowns. Generality is achieved by position on the board rather than by symbol, and it is fully general.
  - region: middle-east
    date_start: 820
    note: Al-Khwarizmi's book of al-jabr and al-muqabala around 820 classifies every type of quadratic, gives a general method for each, and proves each method by geometric completion of a square — stated in words, with no symbols, and general nonetheless. He writes it for commerce, surveying and the division of inheritances under fixed Quranic shares. Al-Karaji around 1000 extends algebra to arbitrary powers, and Omar Khayyam solves every class of cubic geometrically by intersecting conic sections.
  - region: south-asia
    date_start: 628
    note: Brahmagupta in 628 gives a general solution of the quadratic and names several unknowns with colour words so that more than one can be manipulated at once, and treats indeterminate equations systematically. Bhaskara II's chakravala method around 1150 solves a class of indeterminate quadratic equations that European mathematics did not treat until the seventeenth century, which makes the Indian tradition's algebraic generality independent and in places ahead.
  - region: mediterranean
    date_start: 250
    note: Diophantus's Arithmetica around 250 introduces abbreviations for the unknown and its powers — a syncopated notation part way between words and symbols — and solves determinate and indeterminate problems. His methods are usually specific to each problem, and his notation, not a general theory, is what later readers took from him.
  - region: europe
    date_start: 1545
    note: Cardano's Ars Magna of 1545 publishes the solution of the cubic, a method developed by del Ferro and Tartaglia and kept secret as a competitive asset in public problem contests. Viète from 1591 uses letters for known as well as unknown quantities, so a whole class of equations can be written and solved at once, and Descartes's notation of 1637 is essentially the one still used. This is the European realisation of the symbolic route, not algebra's completion.
absence:
  - region: europe
    note: Latin Europe before the twelfth century had no algebra. Alcuin's Problems to Sharpen the Young around 800, the era's mathematical collection, solves each puzzle separately by its own trick, with no method that transfers from one problem to the next — the mechanism's negative stated exactly. General methods arrived only with the translation of al-Khwarizmi by Robert of Chester in 1145, which is why the word itself is Arabic.
prereqs: [MTH-015, MTH-020]
enables: [MTH-045]
see_also: [MTH-025, LAW-070, MTH-040]
contested: true
contested_note: "Two disputes. First, whether Greek geometric treatments of quadratic problems in Euclid constitute a geometric algebra was the subject of a sharp historiographical controversy after Unguru's 1975 critique, which argued that reading them as algebra imposes a later concept; model Greek geometry as solving problems algebra later generalised, without calling it algebra. Second, how much al-Khwarizmi drew on Babylonian procedural quadratics, on Indian algebra, or on Greek geometry is unresolved; his geometric proofs resemble Greek methods and his problem types resemble Babylonian ones. Model the Islamic synthesis as drawing on several traditions rather than descending from one. Throughout, symbolic notation is modelled as one route to generality among three, and the most economical rather than the first or the only true one."
stats_touched: [execution delegability]
tags: [algebra, al-jabr, fangcheng, tianyuan-shu, brahmagupta, diophantus, cardano, viete]
---

A general relation stated and manipulated without reference to any particular
quantity. A solution method stops being a worked example that happened to
succeed and becomes an object that can be proved correct for every case it
covers — and there are three different ways to get there.

## Timing

From the Nine Chapters' array elimination around -100, through Diophantus
around 250, Brahmagupta in 628, al-Khwarizmi around 820, Khayyam and Bhaskara
II, Li Ye in 1248 and Zhu Shijie in 1303, to Cardano in 1545, Viète from 1591
and Descartes in 1637.

## Mechanism

**Generality is the mechanism, and notation is only one way to reach it.** A
worked example tells you how one problem was solved. A general method tells
you how every problem of a kind is solved, and a general method with a proof
tells you it will always work. That is what turns a collection of solved
problems into a body of technique that transfers, and it is the step algebra
makes.

**There are three routes and none is the destination.**
1. **Rhetorical-geometric.** Al-Khwarizmi classifies every quadratic type,
   gives each a general method and proves it by geometric completion — all in
   words. Khayyam does the same for every class of cubic using conic sections.
   Fully general without a single symbol.
2. **Positional-array.** The Nine Chapters eliminates across arrays of
   coefficients on the counting board, and Li Ye and Zhu Shijie write
   polynomials by position. Generality comes from where a number sits, which is
   a direct extension of positional notation (MTH-015) into algebra.
3. **Syncopated and symbolic.** Diophantus abbreviates, Brahmagupta names
   several unknowns with colours, and Viète and Descartes use letters for knowns
   and unknowns alike.

The symbolic route is the most economical once built — a single expression can
stand for an infinite family of equations and be manipulated mechanically — and
that economy is why it spread. It is not more general than the others, and it
arrived last.

**The payoff is manipulation that does not need understanding at each step.**
Once a relation is written generally, its transformations follow rules, which
raises `execution delegability` exactly as MTH-020 describes for arithmetic,
now at the level of whole classes of problems.

## Social consequence

A general method is valuable, and where it is valuable it can be owned. In
Renaissance Italy solutions to cubics were competitive assets: mathematicians
won positions and money in public problem contests, del Ferro and Tartaglia kept
their methods secret for that reason, and Cardano's publication of the cubic in
1545 provoked a bitter dispute over a technique treated as property. The same
pressure that makes generality useful makes it guarded.

Algebra also arrives already attached to law and commerce rather than to pure
inquiry. Al-Khwarizmi states that he wrote for trade, surveying, and the division
of estates under fixed inheritance shares (LAW-070) — calculations that recur
constantly and must come out the same for every heir. A legal system that
prescribes exact fractional shares generates demand for exactly the kind of
general method algebra supplies.

## Regional specifics

Three of the five traditions reach full generality by routes other than symbols,
and the entry is built so that none of them is a precursor. The Chinese
positional-array route is general from the Nine Chapters onward and extends to
four unknowns by 1303. The Islamic rhetorical-geometric route is general and
proved, and supplies algebra's name. Indian algebra names several unknowns and
solves indeterminate problems Europe reached only in the seventeenth century.
Diophantus contributes abbreviation. Europe realises the symbolic route late and
makes it the standard notation. The genuine absence is Latin Europe before the
twelfth century, where Alcuin's collection solves each puzzle by its own trick —
until al-Khwarizmi was translated in 1145.

## Contested

Whether Greek geometric treatments are algebra is disputed since Unguru; the
entry treats them as solving problems algebra later generalised. How much
al-Khwarizmi drew on Babylonian, Indian or Greek sources is unresolved, and the
entry models a synthesis of several. Symbolic notation is modelled throughout as
the most economical route to generality, not the first or the only real one.

## Hook

Converts worked examples into general, provable methods, raising `execution
delegability` from single calculations to whole classes of problems. Generality
should be reachable by three independent routes — words with geometric proof,
positional arrays, and symbols — with symbols the most economical once built but
neither first nor uniquely general, so a tradition without symbolic notation is
not thereby without algebra. Demand should come from recurring, exact
calculations in commerce and law, including prescribed inheritance shares. And a
general method should be treatable as a private competitive asset where
mathematical reputation pays, so secrecy of technique competes with publication.
