# Mathematics Domain — Audit and Proposals

**Date:** 2026-09-13
**Scope:** the MTH domain after the MTH-050 split (Task 1, executed separately),
with proposals for additions.
**Method:** full read of all MTH entries; overlap greps for positional notation,
algebra, calculus and its assumed consumers, non-Euclidean geometry, statistics
and sampling, operations research and modelling, and cryptography across every
domain; `stats-observed.md` check; sort-position and burned-ID checks in MTH and
NAT.
**No entries written.** Proposals only.

---

## Domain state after Task 1

Ten entries: `MTH-010`, `MTH-020`, `MTH-030`, `MTH-040`, `MTH-060`, `MTH-065`,
`MTH-070`, `MTH-075`, `MTH-080`, `MTH-090`. `MTH-050` retired into `ENG-215`,
`MTH-065` and `MTH-075`, with the calculating machine sent to ENG under the
amended procedure-outside-the-head test.

---

## MTH-010 — SPLIT IT

**Verdict: `MTH-010` should split, and its three title words map to three
different outcomes.** It is doing to this domain exactly what `MTH-050` was.

`MTH-010` Mathematics, Geometry, and Calendrical Astronomy (-1800..1200) carries:

1. **Notation** — its Mechanism's first paragraph: "the specific notational
   system a tradition uses shapes what calculations are practical: a positional
   decimal system with true zero makes arithmetic … dramatically easier." A
   genuine MTH mechanism, currently one paragraph.
2. **Calendrical astronomy** — its Mechanism's second paragraph: observation-based
   calendar-keeping that "pays for itself in practical coordination value." This
   **fails the MTH refutation test outright** — a calendar's accuracy is checked
   against the sky. It is NAT content that travelled in with the carve-out
   because it was bundled.
3. **Geometry** — **appears in the title and nowhere in the Mechanism.** There is
   no geometry mechanism in the entry to split out; the word is a label.

**Proposed decomposition:**

| Part | Goes to | Why |
| --- | --- | --- |
| Notation | **`MTH-015` Numeral Systems and Positional Notation** (proposal 1 below) | the entry's real MTH mechanism, underdeveloped |
| Calendrical astronomy | **a new NAT entry** | observational; three existing entries consume it (`NAT-120` Heliocentrism, `MAR-050` Celestial Navigation, and `REL-030`'s liturgical calendar as a natural link) |
| Geometry | **absorbed into proof as an institution** (proposal 3) | Greek geometry is the axiomatic exemplar, and the title word finally gets a mechanism |

`MTH-010` then retires, with `MTH-015`, the NAT astronomy entry and `MTH-025`
as successors. Its current consumers re-point: `NAT-040` Mechanics and
`MTH-040` to the notation successor; `NAT-120` and `MAR-050` to the NAT astronomy
entry.

### ⚠ Slot problem for the NAT successor — reported, not compressed

Calendrical astronomy dates on merits to roughly -2000 (the Babylonian
omen-and-observation compilations; Egyptian civil calendar earlier still). That
sorts **below NAT's floor**, `NAT-020` (-600). **NAT has no genuinely clean slot
there.** NAT-001 through NAT-024 are all burned as `former_ids`, and NAT-010 and
NAT-014 were burned again by Run 4.

The probe shows `NAT-015` as the one free number, and **it is not clean**: it
appears free only because its last live holder, `NAT-060`, was retired, while
`id-migrations.md` Run 1 still resolves stale NAT-015 → NAT-060. Reusing it
would make a stale reference ambiguous between two entries.

Options, in order of preference: (a) a convention decision permitting reuse of a
`former_id` whose holder is retired, relying on `id-migrations.md` plus
`retired-ids.md` to disambiguate — the same question the computation flags file
raised, now recurring; (b) a local NAT renumber, which per instruction needs your
approval first; (c) keep astronomy inside the notation successor, which violates
the refutation test and is not recommended.

---

## PROPOSALS

### Slot map

MTH slots are not exhausted. Every proposal below has a clean slot, and none of
the MTH candidates has any former-ID history, since the domain is new.

| # | Proposal | ID | Sorts between |
| --- | --- | --- | --- |
| 1 | Numeral Systems and Positional Notation | `MTH-015` | floor (after MTH-010 retires) / MTH-020 (-1800) |
| 3 | Proof as an Institution | `MTH-025` | MTH-020 (-1800) / MTH-030 (-350) |
| 2 | Algebra as Symbolic Manipulation | `MTH-035` | MTH-030 (-350) / MTH-040 (850) |
| 4 | The Calculus | `MTH-045` | MTH-040 (850) / MTH-060 (1767) |
| 6 | Sampling and Statistical Inference | `MTH-062` | MTH-060 (1767) / MTH-065 (1822) |
| 5 | Non-Euclidean Geometry | `MTH-067` | MTH-065 (1822) / MTH-070 (1874) |
| 8 | Cryptography | `MTH-072` | MTH-070 (1874) / MTH-075 (1890) |
| 7 | Mathematical Modelling as a Method | `MTH-084` | MTH-080 (1900) / MTH-090 (1948) |
| — | The Stored-Program Architecture | `MTH-087` | MTH-084 (1939) / MTH-090 (1948) |

Three ranges hold only four slots (061-064, 066-069, 071-074), which is
adequate for these proposals but will be the first to exhaust.

---

### LOAD-BEARING

#### 3. Proof as an Institution — ABSENT. `MTH-025`. -500..1900, `era`.

**This is the domain's most load-bearing absence.** The MTH boundary rule rests
on the claim that derivation produces knowledge that does not decay and needs no
observation — and no entry establishes it. `MTH-070` describes proof's brittleness
at the base without any entry establishing proof's permanence, so the domain
currently carries the failure mode of its defining property and not the property.

**Mechanism.** A claim established by deduction requires no observation and does
not decay, so a proved result is permanent in a way no empirical result is — and
because it is permanent it can be *built on* indefinitely, which is what makes a
body of mathematics accumulate rather than turn over. The institution is the
shared standard: what counts as a demonstration, who can check it, and how it is
transmitted. `MTH-070` is the other side of the same property, and the two must
link.

**Carries the geometry from `MTH-010`**, since Greek axiomatic geometry is the
case where deductive proof was institutionalised and transmitted.

**Regional handling — without ranking.** The badly-written version is "Greek
proof, with precursors." The better entry treats **several standards of
demonstration** as parallel institutions:
- Greek axiomatic deduction, fixed by **Euclid's *Elements*** as the transmitted
  form — and transmitted substantially *through Arabic*: the Elements reached
  Latin Europe from the translations of al-Hajjaj and Thabit ibn Qurra, so the
  Islamic world is the institution's main carrier for centuries, not a relay.
- **Indian *upapatti*** — demonstration of a rule's correctness, attached to
  results rather than derived from one axiomatic base.
- **Chinese commentary justification** — Liu Hui's commentary of 263 on the Nine
  Chapters, justifying procedures rather than deducing theorems.
- **Babylonian verification** — procedures checked by worked instances.

All produced correct results. They differ in what they make permanent and what
they make portable, which is the variable to model. This also connects directly
to `MTH-070`'s finding that distributed justification avoids foundational crisis.

**Prereqs** `MTH-015`, `MTH-020`. **Enables** `MTH-070`, `MTH-030` as see_also.
**Stats:** `intellectual authority`, and the proposed new stat `latent capability`
below.

#### 1. Numeral Systems and Positional Notation — PARTIALLY EXISTS. `MTH-015`. -2100..1500, `era`.

**Does `MTH-010` or `MTH-020` carry this? Partially, and not adequately.**
`MTH-010` states the mechanism in one paragraph and bundles it with astronomy.
`MTH-020` carries the European algorism-versus-abacism contest in a region_variant,
framed as delegability rather than notation. Neither treats zero, placeholder
versus number, or independent invention as the mechanism. **Written as
`MTH-010`'s notation successor, not as a new entry alongside it.**

**Mechanism.** Notation determines what calculation is tractable. Roman or other
additive numerals make multiplication a specialist skill done on a counting board;
positional notation with zero makes it a written procedure teachable to a child.
Zero is the decisive element because it is what makes place-value unambiguous.

**Date on merits, not on slot.** Sexagesimal place-value emerges in Ur III, c.
-2100, and is standard in Old Babylonian texts — hence -2100, which sits below the
current floor and is available only because `MTH-010` retires.

**Regional — genuinely non-European, with Europe as the late adopter.**
Babylonian sexagesimal place-value with a placeholder but no true zero (c. -2100);
**Maya positional vigesimal with a true zero, invented independently**; Indian
decimal place-value with zero as a number in its own right (by c. 500); Chinese
rod numerals, positional on the counting board; transmission through
al-Khwarizmi's arithmetic and then Fibonacci (1202); and the Italian
abacists-versus-algorists contest as Europe's late, resisted adoption.

**Absence (genuine).** Carry `MTH-010`'s new africa absence here: counting was
highly developed and no written positional notation formed, so a calculation could
not outlast its performance.

**Stats:** `execution delegability`.

#### 4. The Calculus — ABSENT. `MTH-045`. 1400..1872, `era`.

**What NAT assumes and does not prereq.** `NAT-040` Mechanics names Newton's
*Principia* as "the mathematical" synthesis, and `NAT-170`, `NAT-180`, `NAT-210`
and `NAT-220` all do physics that is inexpressible without calculus — yet their
prereqs are `NAT-040`, `MTH-040` and each other. **No entry anywhere supplies the
calculus.** A worthwhile nuance: the *Principia* itself is written in geometric
form, so the real consumer is eighteenth-century analytical mechanics (Euler,
Lagrange) and everything in NAT from 1820 on.

**Mechanism.** Continuous change becomes calculable, which converts physics from
describing motion into predicting it — rates, accumulations, and the evolution of a
system from its present state.

**Regional — Kerala first, and two independent traditions besides Europe's.**
- **Madhava and the Kerala school** (from c. 1400) derive infinite series for sine,
  cosine and arctangent two centuries before Europe. Transmission to Europe via
  Jesuit contacts is proposed and **unproven — flag contested.**
- **Japanese *wasan***, Seki Takakazu and Takebe Katahiro (1680s-1720s), develop
  determinants and calculus-like methods independently.
- **Newton and Leibniz**, and the **priority dispute's cost to British mathematics**
  — Britain's loyalty to Newton's fluxion notation isolated it from Continental
  analysis for roughly a century, a clean instance of an institutional quarrel with
  a measurable intellectual price.
- A century of productive but unrigorous foundations (Berkeley's 1734 attack on
  "the ghosts of departed quantities") before Cauchy and Weierstrass — the direct
  prelude to `MTH-070`.

**Prereqs** `MTH-015`, `MTH-035`. **Enables** `NAT-170`, `NAT-180`, `NAT-210`,
`NAT-220` (all dated later). `NAT-040` and `NAT-120` predate 1400 and take it as
see_also. **Stats:** `intellectual authority`.

#### The Stored-Program Architecture — ABSENT. RECOMMEND FOR. `MTH-087`. 1945..1960, `decade`.

**MTH, not ENG.** Under the procedure-outside-the-head test it is squarely MTH —
it is the limit case of holding a procedure outside a head, since the procedure is
now held *as data*.

**Distinct from `MTH-065`, and the distinction is the mechanism.** In every
machine in `MTH-065` the procedure lives in a medium separate from the data —
cards, tape, switches, cables. Here **instructions and data occupy the same
memory, so a program is itself data**, which means programs can read, generate and
modify other programs. Assemblers, compilers and operating systems all follow, and
none is possible under `MTH-065`. `MTH-065`'s Mechanism already states this boundary.

**Fills two deliberately empty fields:** `MTH-065`'s `enables` and `MTH-080`'s
`enables`. Prereqs `MTH-065` and `MTH-080` (Turing's universal machine is the same
property stated abstractly in 1936). The computation audit's provisional `ENG-323`
becomes this. **Attribution contested** — the 1945 EDVAC report under von
Neumann's name drawing on Eckert and Mauchly. **Regional:** Manchester Baby
(1948), EDSAC (1949), **MESM in Kiev (1950), CSIRAC in Sydney (1949), TIFRAC in
Bombay (1960)**. **Stats:** `execution delegability`.

---

### VALUABLE

#### 7. Mathematical Modelling as a Method — ABSENT. `MTH-084`. 1939..2000, `decade`.

**Zero corpus coverage** — no hits for operations research, linear programming,
game theory, Monte Carlo, econometrics, input-output, Gosplan, Mahalanobis or
Kantorovich anywhere. `MAR-340` Convoy Escort has code-breaking but none of the
operational research that famously reset convoy sizing.

**Mechanism.** A formal model substitutes for the system it represents, so
consequences are derived rather than observed — and the system can be optimised
before it is built, or policy tested before it is enacted. **This is where
mathematics becomes an instrument of administration rather than of science**, and
it is the strongest candidate for the domain's modern anchor. Reuse `legibility`:
a state sees its economy or campaign through the model.

**Regional — co-equal, and not American by default.** **Kantorovich's linear
programming (Leningrad, 1939)**, developed for plywood production and unused for
years under Soviet planning; **the Mahalanobis model** behind India's Second
Five-Year Plan (1955-56); British operational research under Blackett; RAND,
von Neumann and game theory; Monte Carlo methods at Los Alamos; Leontief's
input-output analysis, rooted in the 1920s Soviet balance of the national economy.

**Link** `MAR-340` (convoy OR), `WAR-330` (game theory and deterrence), `FIN`
planning entries.

#### 8. Cryptography — ABSENT as its own mechanism. `MTH-072`. 1883..2000, `decade`.

**Overlap checked.** `MTH-040` carries al-Kindi's frequency analysis as an
application of probability; `MAR-340` mentions code-breaking; `MTH-065` carries
Colossus; `DIP-110` Espionage has no cryptographic content. None carries the
mechanism below, so this is not duplication.

**Boundary question — arguable, and resolved for MTH.** MED owns "the access gate
that determines who can use" a medium, and encryption controls who can read. But
encryption does not restrict *use of the channel* — anyone can receive the
ciphertext — it makes content unintelligible to receivers, and its guarantee is a
mathematical hardness claim that no observation can refute. MTH, with `media` in
`domains_touched`.

**Mechanism.** Security comes from mathematical structure rather than secrecy of
method. Kerckhoffs' principle (1883): a system must stay secure even when the
enemy knows everything but the key. Shannon's 1949 secrecy paper formalises it.
**Public-key cryptography (1976-77) supplies the counterintuitive result that two
parties can agree a secret over an entirely open channel.** Reuse
`information sovereignty` — strong encryption is a direct challenge to a state's
ability to read traffic, and the export-control disputes of the 1990s are exactly
that contest.

**The domain's strongest deferred-applicability case:** number theory from Euclid
through Fermat's little theorem (1640) and Euler (1763) becomes the security of
global commerce in 1977, after two thousand years with no application at all.

**Regional.** **The Arabic cryptological tradition is the first systematic one** —
al-Kindi (c. 850), Ibn Adlan and Ibn al-Durayhim, and al-Qalqashandi's encyclopedia
of 1412 — and anchors the entry rather than preceding it. Then Kerckhoffs,
Bletchley, Shannon, Diffie–Hellman and RSA — and GCHQ's secret prior invention of
public-key methods (Ellis, Cocks, 1970-73), which repeats `MTH-065`'s Colossus
pattern of priority erased by secrecy.

#### 2. Algebra as Symbolic Manipulation — ABSENT. `MTH-035`. 250..1637, `era`.

**Mechanism.** Symbols let a general relation be stated and manipulated
independently of any particular quantity, so a solution method becomes a proved
object rather than a worked example.

**REGIONAL WARNING — this is the proposal most easily written as "European
notation completing non-European beginnings," and it is fixed in the proposal.**
Build it as **three routes to generality**, with symbolic notation one route and
not the destination:
1. **Rhetorical-geometric** — al-Khwarizmi's *al-jabr* (c. 820) solves every
   quadratic type by a general method and proves each by geometric completion;
   Omar Khayyam classifies and solves cubics geometrically.
2. **Positional-array** — the Nine Chapters' *fangcheng* (c. -100) solves linear
   systems by array elimination, and Li Ye's *tianyuan shu* (1248) writes
   polynomials positionally on the counting board — generality by position rather
   than symbol.
3. **Syncopated and symbolic** — Diophantus (c. 250), Brahmagupta (628) using
   named colours for multiple unknowns, then Cardano's cubic (1545), Viète (1591)
   and Descartes (1637).

Viète and Descartes then read as the European realisation of route 3, not as
algebra's completion. **Prereqs** `MTH-015`, `MTH-020`. **Stats:**
`execution delegability`.

#### 6. Sampling and Statistical Inference — PROPOSED NARROWLY. `MTH-062`. 1786..1950, `century`.

**Check against existing entries, as instructed:**
- `MTH-040` covers **aggregate regularity** (al-Kindi, Pascal–Fermat, Boltzmann)
  and has no inference from sample to whole.
- `HLT-270` **already owns Graunt and the Bills of Mortality** — state arithmetic of
  vital records.
- `FIN-090` owns actuarial pricing.

So "statistics as state knowledge" is largely covered, and **the brief's framing
would duplicate `HLT-270`.** What is absent is narrower: **inference from sample to
population.**

**Mechanism.** A state can know a population without counting all of it. This is
the exact counterpart to `MTH-075`: the unit record counts everyone cheaply;
sampling counts a few and infers. Laplace's ratio estimate of France's population
(1786-1802), Kiaer's representative method (1895), Fisher's experimental design
(1925), Neyman's sampling theory (1934). Reuse `legibility`.

**Regional — a strong non-European anchor.** **Mahalanobis and the Indian
Statistical Institute (1931)** and **India's National Sample Survey (1950)** make
South Asia central, not peripheral. **The Soviet census of 1937 was suppressed and
its statisticians arrested** because it reported what the state did not want known —
a state refusing its own legibility. Reuse `information sovereignty`.

**Contested:** the eugenics entanglement of Galton, Pearson and Fisher, carried as
the field's founding context rather than footnoted.

---

### OPTIONAL

#### 5. Non-Euclidean Geometry — `MTH-067`. 1829..1915, `decade`.

**Mechanism distinct, cases overlapping.** The mechanism — mathematics stops being
a description of physical space and becomes the study of consistent formal systems,
any of which might or might not describe anything — is distinct from `MTH-070`'s
contradiction-at-the-base. **But `MTH-070` already carries the case material** in a
region_variant: Khayyam and al-Tusi on the parallel postulate, Saccheri, Lobachevsky
and Bolyai, and relativity 1915 via `NAT-220`.

**Recommendation: optional, and worth writing only as the vehicle for deferred
applicability** — Lobachevsky (Kazan, 1829) and Riemann (1854) producing geometries
with no physical application, which become general relativity's in 1915. If
written, `MTH-070`'s parallel-postulate variant should be shortened to a pointer.

### NOT PROPOSED

**An entry per discovery.** Specific theorems, conjectures, and named results are
instances; none has a mechanism of its own beyond the proposals above.

---

## DEFERRED APPLICABILITY — RECOMMEND ADDING AS DESIGN PRINCIPLE 9

**Recommend for.** It is visible in at least four entries or proposals already:
`MTH-070` (Khayyam's doubt becoming relativity's geometry), proposal 5 (the purest
case), proposal 8 (number theory to cryptography, two thousand years), and the
Kerala series in proposal 4. It also generalises beyond MTH — `NAT-170` already
notes theory preceding its applications.

Suggested wording, not added:

> **9. Some capabilities are produced long before any application exists.** A
> derived result can sit unused for centuries and pay out only when a later domain
> supplies a use. The producing society need not be the one that benefits, and
> nothing about the result signals its eventual value. Model it as a stock that
> neither decays nor pays until a trigger arrives from elsewhere — not as a slow
> return on investment.

### One new stat, proposed

**`latent capability`** — the stock of derived results with no current
application, which does not decay (because proof is permanent) and pays out when a
later domain supplies a use. Checked against `stats-observed.md`: nothing covers
it. It would implement design principle 9 the way `habitus formation` implements
principle 2. Used by proposals 3, 5 and 8.

(Also still pending from the computation audit and unlogged: `capital specificity`.)

## REGIONAL BALANCE

Checked per proposal, with the fix written in:
- **Algebra** was the highest risk and is restructured as three routes rather than
  a European culmination.
- **Proof** is framed as parallel standards of demonstration, with the Islamic
  world as the *Elements*' main carrier rather than a relay.
- **Numerals** is non-European in invention, with Europe as the resisted late
  adopter.
- **Calculus** puts Kerala first and adds Japanese *wasan*.
- **Modelling** anchors on Kantorovich and Mahalanobis.
- **Cryptography** anchors on the Arabic tradition.
- **Sampling** makes the Indian Statistical Institute central.

**No proposal reads as European mathematics with non-European precursors.**

## PROJECTED SIZE

10 now. Splitting `MTH-010` is net zero in MTH (retire one, add `MTH-015`, plus a
NAT successor). Adding the three other load-bearing proposals, the four valuable
and the one optional gives **18**. That is short of DIP's 20, but now a coherent
set rather than a thin one.

## BUILD ORDER

1. **Split `MTH-010`** — blocked on the NAT slot convention decision above for its
   astronomy half; the MTH half (`MTH-015`) can proceed.
2. `MTH-025` proof as an institution.
3. `MTH-087` stored-program — fills two empty `enables`.
4. `MTH-045` calculus — supplies what NAT's physics assumes.
5. `MTH-084` modelling, `MTH-072` cryptography, `MTH-035` algebra, `MTH-062`
   sampling.
6. `MTH-067` non-Euclidean, optional.
