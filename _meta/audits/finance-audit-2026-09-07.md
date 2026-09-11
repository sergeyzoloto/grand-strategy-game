IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

# Finance Domain Audit

**Date:** 2026-09-07
**Scope:** `domains/finance/` — 19 entries, FIN-020 through FIN-240.
**Method:** adversarial structural review against `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`, `_meta/boundary-rules.md`, every entry in `domains/finance/`, and cross-checks against `domains/commerce/`, `domains/war/`, `domains/law/`, `domains/philosophy/`.

## Summary of the pattern

Finance is, on several measures, the best-constructed domain audited this cycle. Region balance is excellent (26.6% Europe+Mediterranean, on par with Migration's series-best 26.7%). Reverse-direction coverage is the strongest of any domain checked — inflationary spirals, asset bubbles, banking panics, capital flight, and the Great Depression are not afterthoughts here, they're a third of the domain, and `FIN-230` in particular models something no other entry in this wiki attempts: a **compounding event**, where three separately-covered mechanics (`FIN-080`'s bubble collapse, `FIN-190`'s banking panic, `FIN-120`'s gold-standard rigidity) interact to produce an outcome worse than any one alone. Self-documentation of its COM and PHI boundaries is consistent and, in `FIN-180`/`FIN-200`'s case, coordinates cleanly with the other domain's own account of the same split (`LAW-200`'s explicit deferral to `FIN-200` matches `FIN-200`'s own claim to that territory exactly).

But it shares this cycle's now-near-universal tier problem in its most extreme form: **all 19 entries are `core`.** And it has a genuine, specific emergence gap the domain's own foundational entry all but names: `FIN-020` (Cadastre, Tax, and Census, -3000) opens by calling itself "the foundational fiscal infrastructure this entire domain sits on top of" — but a cadastre can only assess and tax _value_, and nothing anywhere in this wiki covers where the concept of a portable, verifiable unit of value — money itself, commodity money before it, stamped coinage after — actually comes from. Layered on that: `FIN-020` lists `WAR-060` as a prereq despite starting 2,100 years before `WAR-060`'s own corrected start date, and despite the causal arrow plausibly running backward (fiscal capacity enabling a standing army, which is exactly what `WAR-060`'s own Hook already says).

---

## PART A — Diagnostic Sweep

### 1. EMERGENCE GAP — **FAIL**

`FIN-020` opens the domain by converting territory and population into "taxable, conscriptable, legible state resources" — but taxation and lending both presuppose a shared, portable, verifiable unit of value already exists to be assessed and lent. Confirmed by grep: zero hits anywhere in the wiki for `coinage|commodity money|cowrie|barter` in any relevant sense. Neither commodity money (cattle, grain, cowrie shells functioning as an early medium of exchange) nor stamped metal coinage (Lydia, roughly -600, converting a lump of metal from something that has to be weighed and assayed case by case into something instantly countable and verifiable) is covered anywhere. `FIN-030`'s interest-bearing lending (from -1800) and `FIN-020`'s own tax assessment both silently assume a unit of account already exists.

### 2. SUBSTRATE GAP — **FAIL**

Two concrete absences:

- **Bills of exchange and negotiable instruments.** Confirmed by grep: zero hits for `bill of exchange|letter of credit|negotiable instrument`. Long before `FIN-100`'s central banks or `FIN-050`'s state-issued paper money, private merchant networks (Islamic suftaja, then Italian merchant-banking houses) settled cross-city and cross-border debts by drawing on a correspondent's balance elsewhere rather than physically shipping specie — the actual private-sector precursor to paper currency, and the specific instrument that makes long-distance medieval and early-modern trade settlement practical at all. Nowhere covered.
- **General insurance.** Confirmed by grep: zero hits for `fire insurance|life insurance|actuarial`. `MAR-120` explicitly carves out _maritime_ insurance as a domain-boundary exception — but the much broader generalization of the same risk-pooling logic to property (fire insurance, scaling after London's 1666 Great Fire) and human life (actuarial mortality tables) is absent entirely, despite being one of the most historically significant applications of the pooling principle this domain already uses elsewhere.

A third, related gap worth naming without over-weighting it: no entry owns sovereign debt or government bond markets as their own instrument — `FIN-020`'s own Hook name-drops "sovereign bonds" as something built on its legibility foundation, but nothing actually covers the specific 1690s English "financial revolution" innovation of converting future tax revenue into a tradable financial asset sold to investors today.

### 3. SCALE GAP — **PASS**

Sorting the 19: world-system (`FIN-120`, `FIN-050`, `FIN-230`, `FIN-270`), state (`FIN-020`, `FIN-100`, `FIN-140`, `FIN-200`, `FIN-240`), firm (`FIN-060`, `FIN-070`, `FIN-170`, `FIN-180`, `FIN-130`), and — unlike several domains audited this cycle — a genuine household-scale entry: `FIN-150` (Consumer Lending) covers formal household credit directly, with its own absence case (African rotating savings and credit associations) supplying real texture at exactly that scale. This domain does not have the "all state, no household" problem several others showed. The one soft spot: no entry covers a _mutual or cooperative_ ownership structure (a savings bank or credit union, owned by its own depositors rather than outside shareholders) as distinct from `FIN-070`'s investor-owned joint-stock model — a real gap, but narrow enough to be a VALUABLE addition rather than a probe failure.

### 4. REVERSE-DIRECTION GAP — **PASS, the strongest of any domain audited this cycle**

`FIN-220` (hyperinflation), `FIN-080` (bubble collapse), `FIN-190` (banking panic), `FIN-230` (the Depression as compounding crisis), and `FIN-240` (tax havens explicitly inverting `FIN-020`'s own legibility mechanic) together make roughly a quarter of this domain's entries genuine reversal or collapse mechanics — not tacked-on absence notes, but core entries whose entire Hook is the mechanism running backward. No other domain checked this cycle comes close to this ratio. Nothing further to add here; this probe is a model for what the others should look like.

### 5. PREREQ ORPHANS — **FAIL**, one concrete bug

Every `prereqs` ID resolves to a real entry, and the domain's internal chronology is otherwise clean — but `FIN-020` (`date_start: -3000`) lists `WAR-060` (`date_start: -900`, following an earlier correction) as a prereq: a **2,100-year inversion**, the most severe found in any domain audited this cycle. Worse, the dependency direction is backward on its own terms — `WAR-060`'s own Hook already states a standing army "converts sustained tax legibility directly into year-round military capability," meaning `WAR-060` depends on `FIN-020`-style fiscal capacity, not the reverse. This relationship surfaced as flagged, pre-existing, out-of-scope debt in both the War and Law domain audits' validation passes, because neither of those audits owned `FIN-020` itself. It's squarely in scope now.

Separately, the recurring prose-to-frontmatter bug shows up twice, both times aimed at the same sibling ID: `FIN-140` cites `COM-190` (Protectionism and Tariff Wars) by name twice in its own body ("removes tariff protection (COM-190)," repeated in its Hook) with `domains_touched` already listing `commerce` but `COM-190` never promoted to `see_also`. `FIN-230` does the same with both `COM-160` and `COM-190` in its own regional and contested text.

### 6. MECHANISM DIVERSITY — **PASS, with a near-miss on the pool**

An unusually rich spread: gates (`FIN-020`, `FIN-060`, `FIN-070`, `FIN-170`, `FIN-050`), civic institutions (`FIN-100`), branches that explicitly reuse design principles (`FIN-120`, `FIN-220`, `FIN-030`, `FIN-140`, `FIN-200`), sharp events (`FIN-080`, `FIN-190`), and — genuinely rare in this wiki — a compounding-event mechanic (`FIN-230`) that models multiple other entries' mechanics interacting multiplicatively rather than independently. What's missing, matching every other domain audited this cycle: a fully-realized **depleting pool** (design principle 4). Unlike other domains, this one has a real near-miss already sitting in plain sight — `FIN-120`'s own `bullion stock` stat is genuinely pool-shaped (a reserve drawn down by trade deficits and crises, replenished by inflows) but the entry never frames it explicitly as a maintained-or-depletes resource the way the design principle calls for. Worth a light-touch fix more than a whole new entry.

### 7. BOUNDARY INTEGRITY — **PASS**

Consistent, correct self-documentation: `FIN-070` narrates its own COM split against `COM-150` explicitly; `FIN-140` and `FIN-200` both narrate their PHI splits (systematic argument is PHI's, adoption-as-policy or institution-building is FIN's); `FIN-180` narrates its COM split against `COM-100`/`COM-170`. The `FIN-200`/`LAW-200` boundary is a genuine cross-domain success story worth naming: `LAW-200`'s own text explicitly defers "the broader economic critique of capitalism" to `FIN-200`, and `FIN-200`'s own text claims exactly that territory — two domains' audits, run independently, landing on the identical line. No entry's canonical domain is wrong.

---

## PART B — Proposed Additions

### LOAD-BEARING (4)

**1. Coinage and the Invention of Money**

- Regions: middle-east, mediterranean, east-asia, south-asia — era, roughly -3000 to -400
- Answers: Probe 1 (Emergence) directly
- Mechanism: before a state can tax or a lender can charge interest, a population needs a shared, portable, divisible, verifiable unit everyone accepts as representing value — commodity money (cattle, grain, cowrie shells) solves this crudely; stamped metal coinage (Lydia, ~-600) solves it far better by fixing weight and purity in a government-guaranteed, instantly verifiable form, converting value from something weighed and assayed case by case into something simply counted.
- Hook (seed): an origin gate — establishes the portable, verifiable unit of value `FIN-020`'s tax assessments and `FIN-030`'s interest-bearing loans both silently assume already exists.
- Prereqs: none. Enables: `FIN-020`, `FIN-030` (replacing the removed `WAR-060` dependency at `FIN-020`).
- Contested: true — whether coinage was independently invented in Lydia, India, and China within a few centuries of each other, or diffused from a single origin point, is genuinely disputed.
- Confidence: high.

**2. Bills of Exchange and Negotiable Instruments**

- Regions: middle-east, mediterranean, europe — era, roughly 700 to 1400
- Answers: Probe 2 (Substrate) directly
- Mechanism: a bill of exchange lets a merchant settle a debt in one city by drawing on a correspondent's balance in another, without physically transporting gold or silver across dangerous trade routes — a paper claim substituting for physical specie in inter-city settlement, the private-sector precursor to and long-running parallel of `FIN-050`'s later state-issued paper money.
- Hook (seed): a gate — removes physical bullion transport as a hard requirement for long-distance trade settlement, the private-credit precondition medieval and early-modern commerce depends on well before formal banking (`FIN-100`) exists.
- Prereqs: `FIN-030`. Enables: `FIN-060` (double-entry bookkeeping develops partly to track exactly these instruments, currently missing this link).
- Contested: false.
- Confidence: high.

**3. General Insurance and Actuarial Risk-Pooling**

- Regions: europe, east-asia — century, roughly 1666 to 1900
- Answers: Probe 2 (Substrate) directly
- Mechanism: distinct from `MAR-120`'s maritime-specific carve-out, general insurance (fire insurance scaling after London's 1666 Great Fire, then life insurance built on actuarial mortality tables) pools a large number of independent small risks so a rare catastrophic loss for any one policyholder becomes a small, predictable, budgetable premium for everyone — the same risk-pooling logic as `MAR-120`, generalized far beyond ships and cargo.
- Hook (seed): a gate — converts catastrophic individual risk into a budgetable collective premium, requiring an actuarial science of mortality and risk statistics to price correctly.
- Prereqs: `FIN-060`. Enables: none.
- Contested: false.
- Confidence: high.

**4. Sovereign Debt and Government Bond Markets**

- Regions: europe, americas — era, roughly 1690 to 1900
- Answers: Probe 2 (Substrate) directly
- Mechanism: a state converts its future tax revenue into a tradable financial instrument sold to investors today, letting it finance an expense (usually war) far beyond what current tax revenue alone could cover — the specific instrument that makes `FIN-020`'s abstract "sovereign credit capacity" operational and tradable rather than just a promise.
- Hook (seed): a gate — converts a sovereign's future tax capacity into a tradable financial asset today, with default risk directly priced into what investors will pay.
- Prereqs: `FIN-020`, `FIN-100`. Enables: none.
- Contested: false.
- Confidence: high.

### VALUABLE (3)

**5. Mutual and Cooperative Banking**
Regions: europe, south-asia. Century, roughly 1810-1900. Answers Probe 3 (scale) with a mechanism genuinely distinct from `FIN-070`'s investor-owned joint-stock model: a mutual or cooperative financial institution is owned by its own depositors or borrowers rather than outside shareholders, distributing any surplus back to members rather than investors — a structurally different ownership and incentive design aimed specifically at savers and borrowers the commercial banking and equity system finds unprofitable to serve. Hook: a modifier — the same credit-pooling function as commercial banking, achieved through a different governance structure serving a different population. Prereqs: `FIN-070` (as contrast case). Contested: false. Confidence: high.

**6. Development Finance and International Lending Institutions**
Regions: americas, europe, south-asia, africa. Decade, roughly 1944-1980. Answers Probe 6 with a distinct mechanism: an international institution capitalized by wealthy member states lends to developing or crisis-hit economies on terms private capital markets wouldn't offer, often attaching policy conditions as the price of access — a lending relationship whose goals extend beyond return, and whose conditionality can constrain a borrowing state's own policy choices in ways an ordinary bond-market loan (proposal #4) doesn't. Hook: a branch — testing whether an indebted state's own priorities survive contact with its lenders' preferred prescriptions. Prereqs: `FIN-050`. Contested: true — whether IMF/World Bank conditionality genuinely stabilizes struggling economies or imposes counterproductive austerity that deepens crises is a live, unresolved debate; model the outcome as conditional on the specific conditionality attached and the borrower's own institutional capacity to absorb it, not as a fixed-sign effect. Confidence: high.

**7. Credit Rating Agencies**
Regions: americas, europe. Decade, from 1909. Answers Probe 6 with a distinct information-intermediary mechanism: a third-party rating agency converts an issuer's creditworthiness into a standardized, publicly legible signal, letting bond and derivative markets (proposal #4, `FIN-130`) function at scale without every investor independently assessing every issuer — a `legibility` mechanic (`FIN-020`'s own core stat) applied to private credit risk rather than state fiscal capacity, with a real conflict-of-interest problem built in since issuers themselves typically pay for their own rating. Hook: a gate whose own credibility is a genuine, checkable variable, not a fixed given. Prereqs: the proposed Sovereign Debt entry (#4). Contested: false. Confidence: high.

### OPTIONAL (2)

**8. The Medici Bank** [REJECTED]
Regions: europe. Century, roughly 1397-1494. The archetypal Renaissance merchant-banking house, pioneering cross-city branch banking and bill-of-exchange networks — real and vivid, but its mechanism is already what the proposed Bills of Exchange entry (#2) would cover as a named case rather than a genuinely separate mechanism. Honestly flavour.

**9. Cryptocurrency and Decentralized Currency** [REJECTED]
Regions: global. Decade, from 2009. A currency with no state backing and no central issuer, secured by distributed cryptographic consensus instead — genuinely novel relative to every other instrument in this domain, but recent enough that its long-run economic and mechanical significance is still being determined by events after this record's usual point of confident retrospective judgment. Honestly flavour, and honestly early.

---

## PART C — Proposed Modifications

### SPLIT

**FIN-050 (Paper Money and International Monetary Regimes) — recommended.** The entry's own Mechanism section explicitly flags the seam: "once multiple states issue their own paper currencies, an _additional_ coordination problem emerges," and its own Hook calls the international-regime piece "a genuinely separate, additional mechanic layered on top." That's the split test naming itself. Recommend:

- **FIN-050 retained**: Paper Money and Fiat Currency — state-issued currency backed by trust and legal mandate rather than commodity value (806 onward), ending around the point Bretton Woods formalizes international coordination.
- **New entry**: International Monetary Coordination (Bretton Woods and Successors) — the specific institutional framework for pegging and coordinating multiple sovereign paper currencies against each other and against gold, 1944 onward, prereqing the retained `FIN-050` and enabling `FIN-270` directly rather than through it.

### MERGE

**Considered and rejected: `FIN-120` (Gold Standard) and `FIN-270` (Floating Exchange Rates).** Both are exchange-rate regime choices trading policy independence against credibility in opposite directions, and their Hooks explicitly mirror each other. Kept separate because this is a deliberate mirror pair by design, the same pattern used correctly elsewhere in this wiki (`MAR-300`/`MAR-330`'s naval-status resets, `WAR-040`'s successive chariot-to-iron-to-gunpowder chain) — the two entries model opposite policy choices available in different eras to different actors, not one mechanism told twice.

### RE-DOMAIN

- **`FIN-140`** cites `COM-190` by name twice in its own prose (`domains_touched` already lists `commerce`) without ever promoting it to `see_also`. Fix: add it, reciprocally.
- **`FIN-230`** cites both `COM-160` and `COM-190` by name in its own prose with the identical gap. Fix: add both, reciprocally.
- **`FIN-020`**'s `WAR-060` prereq (Part A, Probe 5) should be removed and replaced with the proposed Coinage entry (#1) — the chronological inversion is severe and the causal direction is backward regardless of date.
- No entry's canonical domain needs to change.

### RETIER

**FIN-170 (Limited Liability): core → standard.** The entry's own evidentiary base is narrow by this domain's standards — two regions carry real content (Europe, the Americas), the third (`south-asia`) is a pure colonial-import absence note, and `enables` extends only to `FIN-180`. This is structurally the same shape as `MAR-250` (Chronometer), retiered in an earlier audit this cycle for the identical reason: a real, well-documented, single-lineage Western legal innovation without the comparative breadth most of this domain's other `core` entries carry.

Not touching the other 18: even the domain's other `enables: []` entries (`FIN-220`, `FIN-180`, `FIN-200`, `FIN-130`, `FIN-150`, `FIN-240`) each supply a mechanism nothing else in the domain provides and carry genuine multi-region comparative texture, the same standard applied throughout this audit cycle.

### CUT

**None.** Read all 19 entries specifically hunting for a self-admitted redundancy or a Hook with no real mechanic and didn't find one — this is now a recurring, honest finding across several domains audited this cycle (Maritime, War) rather than a sign of insufficiently hostile reading: this wiki's construction has reached a point where most remaining domains are tightly built enough that padding isn't the failure mode anymore. `FIN-170`'s RETIER above is this audit's closest adjacent finding, resolved as reduced weight rather than removal because its concrete history remains genuinely load-bearing for `FIN-180`.

---

## PART D — Verdict

**Worst structural problem:** the domain's own foundational entry explicitly calls itself "the foundational fiscal infrastructure this entire domain sits on top of" while silently assuming the concept it's built to tax and lend against — a portable, verifiable unit of value — already exists, and compounds this with a concrete, checkable bug: `FIN-020` lists a prereq that starts 2,100 years after it does, in a dependency direction that's backward on the target entry's own stated terms.

**Entry count:** 19 before. With all four LOAD-BEARING proposals and the `FIN-050` split: 24. With VALUABLE also adopted: 27. With OPTIONAL: 29.

**Proposed out of symmetry, not mechanical need:** both OPTIONAL entries (#8 The Medici Bank, #9 Cryptocurrency) are named as such — the first because its mechanism is fully covered by the Bills of Exchange proposal, the second because it's honestly too recent for this record's usual retrospective confidence. Of the VALUABLE tier, **Credit Rating Agencies (#7)** is the softest call: a real and distinct legibility mechanism, but one that depends on the Sovereign Debt proposal (#4) existing first rather than answering a probe on its own independent terms.

**Does Finance justify separate existence?** Yes, more confidently than most domains audited this cycle. Its region balance and reverse-direction coverage are both among the best checked, its mechanism diversity includes a genuinely rare compounding-event design (`FIN-230`) worth treating as a model for other domains rather than an outlier, and its cross-domain boundary coordination with Law is a clean success story. The gaps found here are precise rather than systemic: an emergence gap the domain's own opening line gestures at without closing, two substrate absences with obvious real-world weight, and a data bug worth fixing regardless of anything else in this report.
