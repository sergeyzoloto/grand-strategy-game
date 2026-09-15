> **Migration note (2026-09-13):** IDs in this file were rewritten by `id-migrations.md` Run 4, the NAT→MTH domain carve-out. Prose is otherwise unaltered.

# Computation and Computing Machinery — Coverage Check

**Date:** 2026-09-13
**Scope:** computation from organised human calculation through mechanical,
electromechanical and stored-program machines, the general-purpose computer and
its cost curve, software as a good, networking, and machine learning.
**Method:** corpus-wide word-boundary grep for computer, computing, calculation,
calculator, tabulator, punched card, abacus, Babbage, Lovelace, Hollerith,
ENIAC/Colossus/EDVAC/EDSAC, transistor, integrated circuit, microprocessor,
software, program(me), algorithm, memory, mainframe, personal computer,
packet/TCP/protocol, artificial intelligence, machine learning; inspection of
`MTH-020`, `MTH-080`, `ENG-330`, `FIN-020`, `COM-170`, `COM-180`, `HLT-270`,
`WAR-250`, `LAW-225`, `MED-150`; ENG region footprint; sort-position and
burned-ID checks.
**No entries written.** Proposals only.

---

## PART 1 — INVENTORY

### What exists between the algorithm and the semiconductor

**Nothing.** `MTH-020` The Algorithm as Delegable Procedure ends at 1800;
`MTH-080` Computability ends at 1936; `ENG-330` Semiconductors begins at 1947.
Between them the corpus contains no calculating machine, no organised
calculation, no tabulator, no computer of any kind, no program, and no software.

| Term | Hits | Own Mechanism, or incidental? |
| --- | --- | --- |
| computer / computing | `MTH-020`, `MTH-080` | **Incidental.** `MTH-020` names "the calculating office to the computer" as downstream of the algorithm; `MTH-080` notes its limits were proved "a decade before any electronic computer runs." Both point at an entry that does not exist. |
| calculation | `FIN-200`, `LAW-210`, `LAW-170`, `PHI-320`, `WAR-090` | **False positives** — calculation as reckoning or deliberation, never as computation. |
| transistor / integrated circuit | `ENG-330`; `MED-130`, `MED-140` | **Own** in `ENG-330`; **incidental** in the media entries (the transistor radio as a cheap receiver). |
| algorithm | `MTH-020`, `MTH-010` | **Own** in `MTH-020`; `MTH-010` carries the Nine Chapters' algorithmic character. |
| memory | `MED-010`, `AGR-050`, `DIP-020`, `DIP-030`, `MAR-340`, `COM-210` | **All human or institutional memory.** No machine memory anywhere. |
| program(me) | `MAR-060`, `LAW-165`, `PHI-230`, `WAR-190`, `DIP-190`, others | **All policy programmes.** No computer program anywhere. |
| packet / protocol | `MED-150`; `WAR-300` | **Own** in `MED-150` (packet neutrality, ARPANET); `WAR-300` is the Geneva Protocol. |
| tabulator, punched card, abacus, Babbage, Lovelace, Hollerith, ENIAC, Colossus, EDVAC, EDSAC, software, mainframe, personal computer, artificial intelligence, machine learning | — | **Zero hits, every one.** |

### Where the chain breaks, and what is already leaning on the gap

- **`MTH-080` has `enables: []`**, shipped that way deliberately in the NAT-060
  split because its descendants did not exist. They still do not.
- **`MTH-020`'s Mechanism names the calculating office and the computer** as its
  consequences, with no entry for either.
- **`ENG-330` carries half of item 4.** Its Mechanism is fabrication precision
  compounding recursively — transistor density rising by orders of magnitude.
  It does **not** carry the price curve's consequence, the sequence of
  institutions each price threshold creates. That is a clean split rather than an
  overlap.
- **`MED-150` rests on software economics without documenting them.** Its claim
  that "the marginal cost of becoming a publisher is near zero" and its treatment
  of ranking as a "business-model artefact" both presuppose near-zero-marginal-cost
  software, licensing as a substitute for sale, and platform economics. None has
  an entry.
- **`HLT-270`** Public Health Statistics and Vital Registration has
  `prereqs: []` and `enables: []` — a registration entry with no connection to the
  machine that later processed registration at scale.
- **`COM-180`** Technological Displacement of Craft has `enables: []`; the
  displacement of human computers is its largest later instance.

### Incidental defects found in `ENG-330` (flagged, not fixed)

`date_precision: century` on a 43-year span; `stats_touched` holds only
`skill differential`; no Soviet or South Asian material. Its `absence` field
(Europe's weak frontier fabrication despite strong research) is **borderline but
legitimate** — a genuine absence of leading-edge fabrication with a stated
consequence, not presence material — and is **not** counted as an eighth misuse.

---

## PART 2 — PROPOSALS

Six proposals: the five assessed mechanisms, all absent, plus machine learning.
Networking is recommended **against** as a separate entry.

### ID selection note

Four slots were chosen to avoid numbers already written into earlier audits as
proposed IDs for *different* entries. `ENG-167` (the literacy audit's newspapers
slot, which became `MED-090`) and `ENG-335` (its global-network slot, which
became `MED-150`) are unassigned and technically free, but reusing them would
make the audit trail ambiguous. `ENG-325` and `ENG-327` are burned as
`former_ids` of `MED-130` and `MED-140`. All six IDs below are free, unburned,
and not previously named.

### 1. Calculation as Organised Labour — ABSENT. Propose `COM-175`.

**Placement: COM, overriding the ENG default** — and the entry's own mechanism
forces it. Nothing here is engineered: there is no machine, no technique, no
artefact. The mechanism is **the division of labour applied to arithmetic**, and
`COM-170` The Factory and Division of Labor owns that model. Prony applied it
explicitly and by name, citing Smith's pin factory. `domains_touched: [nature,
engineering, war, maritime]`. **1790..1970, `century`.** Sorts between `COM-170`
(1771) and `COM-180` (1811).

**Relationship to `MTH-020`, resolved: `MTH-020` is the prereq.** The algorithm
entry's mechanism is that a written procedure makes labour *delegable*; this entry
is that delegation *organised* at industrial scale. `execution delegability` is
the stat `MTH-020` defines and this entry spends. Also prereq `COM-170`.

**Mechanism.** Before machines, computation was a job, and it was organised as a
factory. Prony's tables for the French cadastre in the 1790s ran a three-tier
hierarchy — a handful of mathematicians choosing formulae, a small group reducing
them to procedures, and a large bottom tier performing only addition and
subtraction, requiring no mathematical training at all (many reportedly
unemployed hairdressers). The procedure carried the expertise; the tier supplied
accuracy and volume. The same model runs through the Nautical Almanac's
distributed home computers, the Harvard Observatory computers, the WPA
Mathematical Tables Project, and the computing pools at NACA and JPL.

**The gendering is mechanism, not colour.** The work was substantially female and
low-paid *because* it had been deskilled — classified as execution rather than
expertise — which is `MTH-020`'s social consequence (expertise separates from
execution, and the two acquire different social positions) made concrete in a
workforce. The Harvard computers are the case where the boundary visibly failed
to hold: Leavitt, Cannon and Fleming produced first-rank results from inside a
role defined as clerical.

**This entry makes mechanisation legible as substitution, not invention.** When
ENIAC arrives, it replaces a job that already existed in precisely specified form.
Reuse **`stranded capital`** (`ENG-265`), whose definition already covers "a deep
body of skill with nowhere to go" and which is largest where capital is most
specific — a computing pool's skill is maximally specific. Reuse
**`labour-saving incentive`** (`WAR-035`) in its inverse direction: cheap
deskilled calculating labour *delayed* mechanisation, exactly as cheap coerced
labour did in `WAR-035`, which is a real and testable parallel.

**Regional — Atlantic-weighted, stated rather than hidden, and corrected.** The
demand for printed tables (navigation, astronomy, geodesy, actuarial, ballistic)
sat overwhelmingly in imperial and naval metropoles, so the institution
concentrates there. Two co-equal non-Atlantic anchors correct the frame:
- **South Asia** — the Survey of India's Computing Office, where Radhanath
  Sikdar, as chief computer, calculated the height of Peak XV in 1852. The Great
  Trigonometrical Survey was among the largest computing operations of the
  century and it ran in Calcutta and Dehradun.
- **East Asia** — organised soroban clerical computation in Japanese banks,
  railways and insurance offices, fast enough that a soroban operator beat an
  electric calculator in the widely reported 1946 Tokyo contest. The same
  mechanism on a different instrument, and the reason mechanisation arrived later
  there.

**Absence (genuine).** *africa* — geodetic, census and survey computation for
colonial Africa was performed in metropolitan offices, so the demand for tables
existed but no computing office formed on the continent; the institution follows
the location of the demand, not of the territory measured. *central-asia-steppe*
— no standing demand for printed tables of any kind (no almanac, no insurance
market, no artillery bureaucracy, no cadastre), so nothing to organise.

### 2. Mechanical and Electromechanical Computation — ABSENT. Propose `ENG-168`.

**Placement: ENG.** Machines embodying arithmetic. `domains_touched: [finance,
law, war, commerce]`. **1642..1950, `century`.** Sorts between `ENG-165` (1550)
and `ENG-170` (1712).

**Anchored on Hollerith, as the brief proposes, and linked both ways to
`FIN-020`.** The 1890 US census tabulator is a machine built to process state
enumeration — `legibility` mechanised. It is where the capacity of a register
stops being bounded by how many clerks can be paid to count, and it connects
directly to `WAR-200` and `LAW-165`, both of which are gated on registers this
machine made cheap to process. The data-processing industry that followed —
punched-card firms consolidating into IBM — is a consequence of that first
customer being a state.

**Correction to the brief's figure.** The claim that the 1890 census was processed
in "roughly a year against the previous decade's eight" compresses two different
things. The 1880 census took most of a decade to tabulate fully; in 1890 the
headline population count was announced within weeks, and fuller tabulation took
a few years. The speed-up is real and dramatic; the one-year figure is a common
simplification and the entry should give the distinction rather than repeat it.

**Mechanism.** Pascal (1642) and Leibniz (1673) embody the arithmetic operation
in gearing. The Jacquard loom (1804) supplies an idea rather than a computing
device — instructions stored on punched cards in a medium separate from both
operator and machine. Babbage's difference and analytical engines specify a
general programmable machine; Lovelace's 1843 note states that such a machine
could operate on symbols of any kind rather than numbers alone. Hollerith (1890)
makes it pay. Colossus (1943-44) and ENIAC (1945) end the period.

**Prereqs, constrained by chronology.** `MTH-020` and `FIN-020`. The entry *draws
on* `ENG-180` looms (1733), `ENG-190` machine tools (1775) and `HLT-270` vital
registration (1662) — but all three post-date its 1642 start, so the prereq rule
forbids them. Carry them as `see_also`, and note that this is a structural
feature of a three-century entry rather than a defect.

**CONTESTED — Babbage.** The analytical engine was never built, and its influence
on the designers of the 1930s and 1940s is disputed: several were unaware of it,
Aiken encountered it late, and the lineage from Babbage to ENIAC is largely a
retrospective construction. **Present Babbage as a design that was not realised
and whose causal influence is uncertain, not as a link in a clean chain.**
Colossus raises a parallel problem from the other direction — kept secret until
the 1970s, it could influence nothing. Model the machines as independently
re-invented under demand rather than as a lineage.

**Regional — invention is Atlantic, diffusion is not, and the entry should say
both.** Odhner's pinwheel arithmometer (St Petersburg, from the 1870s) and its
mass-produced Soviet successor, the Felix, were the dominant calculating machine
across the Russian empire and the USSR for most of a century. Japan's Tiger
calculator, mass-produced from 1923, carried the technology across East Asian
commerce. Zuse's Z3 (Berlin, 1941) was an independent programmable machine built
without knowledge of the Anglo-American work.

**Absence (genuine).** *africa* — much colonial enumeration counted households,
huts or taxpayers by estimate rather than individuals by schedule, so the unit
record the punched card encodes did not exist and a tabulator had nothing to
process. The absence is of individual-level legibility, which is the machine's
input, and it ties the entry back to `FIN-020` as a negative. *oceania* — no
enumeration, commerce or insurance volume large enough to justify leasing
equipment whose economics depended on very large runs.

### 3. The Stored-Program Architecture — ABSENT. Propose `ENG-323`.

**Placement: ENG. 1945..1960, `decade`.** Sorts between `ENG-320` (1909) and
`ENG-330` (1947). `domains_touched: [nature, war, commerce]`.

**Prereqs: `ENG-168` and `MTH-080` — and this entry finally fills `MTH-080`'s
empty `enables`.** Turing's universal machine (1936) states the same property
abstractly a decade before it is built: one machine that reads a description of
another and behaves as it. The NAT-060 split left `MTH-080` without descendants
on purpose; this is the descendant.

**Mechanism — the conceptual break.** Instructions and data occupy the same
memory, so **a program is itself data**. A program can therefore read, generate,
modify and operate on other programs — which is what makes assemblers, compilers,
operating systems, and everything later possible. Every prior machine changed task
by being rewired or re-plugged; this one changes task by being reloaded. Changing
what the machine does stops being a physical act.

**CONTESTED — attribution.** The 1945 *First Draft of a Report on the EDVAC* bears
von Neumann's name alone and draws substantially on Eckert and Mauchly's ENIAC
work, and the resulting "von Neumann architecture" naming is disputed; Turing's
contemporaneous ACE design and Zuse's work complicate it further. **Carry the
attribution as contested, not settled**, and model the architecture as arriving
from a small, interconnected group rather than a single author.

**Social consequence worth carrying — the regendering.** The first programmers of
ENIAC were women drawn from the human computing pools, assigned the work because
programming was initially classified as clerical — item 1's mechanism continuing
directly. As programming acquired status it was redefined as engineering and its
workforce changed. The same work was feminised while classified as execution and
masculinised once reclassified as expertise, which is the cleanest demonstration
in the corpus that the classification drove the composition, not the reverse.

**Regional — genuinely wider than the usual story.** Manchester Baby (1948) and
EDSAC (1949) in Britain; the EDVAC and IAS machines in the United States; **MESM
in Kiev (1950)** under Lebedev, independently designed; **CSIRAC in Sydney
(1949)**, among the first handful of stored-program machines anywhere and the
entry's Oceania case; FUJIC in Japan (1956); **TIFRAC in Bombay (1960)**; China's
first machines from 1958 on Soviet designs.

**Absence (genuine).** *africa* and *middle-east* — every early stored-program
machine was paid for by a specific state demand: nuclear weapons calculation,
ballistics, cryptanalysis, or weather prediction. Where that demand and the
electronics base to meet it were both absent — thousands of valves and a
permanent maintenance corps — no machine was built, regardless of mathematical
capacity. The absence is of the sponsoring demand, not of competence.

### 4. The General-Purpose Machine and the Cost Curve — ABSENT. Propose `ENG-333`.

**Placement: ENG. 1951..2010, `decade`.** Sorts between `ENG-330` (1947) and
`ENG-340` (1973). `domains_touched: [commerce, finance, law, war, media]`.

**Prereqs `ENG-323` and `ENG-330`.** `ENG-330` supplies the fabrication curve;
this entry owns what the falling price does to institutions, and must not restate
the fabrication.

**Mechanism.** One machine performs arbitrary tasks, so **the capital does not
specialise** — unlike every prior machine in this record, which embodied its
function. That property plus a falling cost curve produces a sequence in which
**each order-of-magnitude price drop creates a different institution rather than
more of the same**: the mainframe, which an organisation owns; the minicomputer,
which a department owns; the microcomputer, which a household owns; the
smartphone, which a person owns. Each threshold changes who can decide what the
machine is for, which is the institutional consequence.

**New stat proposed: `capital specificity`** — the degree to which a machine
embodies a single function. The general-purpose computer drives it toward zero.
Checked against `stats-observed.md`: nothing covers it — but **`stranded capital`
already depends on it implicitly** ("largest where capital is most specific"), so
this names a modifier the corpus is already using unnamed rather than adding a
parallel concept. Reuse `strategic input dependency` for fabrication
concentration and `skill differential` from `ENG-330`.

**REGIONAL SKEW — this proposal defaults to a US story, and the fix is built into
it rather than noted.** ENG's footprint is not itself the problem (Europe appears
in 23% of ENG region tags, well under the 40% flag), but the canonical narrative
for this entry is Californian. Anchors:
- **East Asia originates the microprocessor's commission.** The Intel 4004 (1971)
  was built for **Busicom**, a Japanese calculator firm, in the middle of the
  calculator wars between Sharp, Casio and their rivals — so the institution
  threshold from department to household was driven by Japanese consumer
  electronics demand, not by computing demand.
- **Taiwan and South Korea** own the fabrication the whole later curve runs on
  (link `ENG-330`).
- **The Soviet decision to clone IBM System/360** as the ES EVM series from the
  late 1960s ended the independent BESM line and tied the bloc to a trailing
  architecture — a case where the cost curve's institutional sequence was
  deliberately entered late, and the household stage never formed.
- **India's software services firms** (TCS from 1968) as an institution the
  general-purpose machine plus cheap connectivity made possible, with IBM's 1978
  exit from India as a formative event.
- **Africa enters at the last threshold.** For most of the continent the first
  general-purpose computer a person owns is a phone, so the organisational,
  departmental and household institutions never form — the sequence begins at its
  end.
- **China** as manufacturing scale and later platform scale.

**Absence (genuine).** *africa* — **the intermediate institutions**: organisational
mainframe, departmental minicomputer and household PC largely never formed,
because each price threshold was crossed later than the next one arrived. One
absence is proposed rather than two; a second would be contrived for a machine
that is, by 2010, nearly everywhere.

### 5. Software as a Good — ABSENT. Propose `LAW-240`.

**Placement: LAW, not COM.** The mechanism the brief identifies is a
**property question**, and its genuine novelty is legal. Software is functional
rather than expressive, so extending copyright — built for expression — to it is a
new legal object in exactly the sense `LAW-225` is: a right manufactured to fit
something the existing categories did not contemplate. **Use `LAW-225`'s framing
directly**, as the brief suggests; it fits. `domains_touched: [commerce, finance,
engineering, media]`. **1969..2000, `decade`.** Sorts above `LAW-230` (1948).

**Prereqs `ENG-333` and `LAW-215`.** The second matters and is not obvious: the
**1969 IBM unbundling decision**, which made software a separately priced good for
the first time, was taken as the US antitrust suit against IBM was filed that same
January. Software as a purchasable good is a by-product of `LAW-215`'s instrument.

**Mechanism.** Near-zero marginal cost of reproduction means the first copy carries
all the cost and every later copy none — reuse **`production cost floor`**
(`MED-140`), which states exactly this for broadcast content. That forces a choice
nothing in the corpus covers: either the good is non-rival and effectively public,
or a legal right is manufactured to make it excludable. **Licensing substitutes for
sale** — the buyer acquires permission, not an object — which is a new relation
between purchaser and thing. Free software (1983-85) and open source (1998) are
deliberate counter-institutions using the same copyright to enforce openness, which
is legally ingenious and mechanically important: the instrument of enclosure
repurposed as the instrument of commons. Reuse `contract enforcement`.

**`MED-150` rests on this without documenting it**, and should gain it as a prereq
(1969 = 1969, so chronology permits): the platform economics that entry treats as
given presuppose near-zero-marginal-cost software and licensing.

**REGIONAL SKEW — flagged and fixed in the proposal.** The default story is US law
exported. The better entry is about **the category choice being political**, and
that is visible outside the US:
- **Japan** debated a *sui generis* "program right" in the early 1980s and adopted
  copyright under US trade pressure in 1985 — the clearest case that fitting
  software into copyright was a decision, not a discovery.
- **Brazil's 1984 informatics law** reserved the domestic market for national firms
  — software and hardware as industrial policy rather than property.
- **India** built an export services industry largely on licensing and contract
  rather than on domestic IP enforcement.
- **China** made enforcement a trade instrument around WTO accession in 2001.
- **Europe** extended copyright to software (1991 directive) and then **rejected
  software patents** in 2005 — the same category argument resolved differently.
- **TRIPS (1994)** as the instrument that globalised the American answer.

**Absence (genuine).** *africa* — where enforcement capacity for licences was
absent, the property right existed on paper and could not bind, so software
functioned as a de facto non-rival good regardless of statute. This is an absence
of the enforceable regime itself, and it is the entry's `LAW-225` lesson — a right
to exclude that cannot exclude yields the unregulated outcome — recurring.

---

## ALSO ASSESSED

### Networking as distinct from `MED-150` — RECOMMEND AGAINST a separate entry.

**The proposed split does not hold under the settled boundary rule.**
`boundary-rules.md`'s MED section assigns MED "the transmission channel, the
network, and the access gate". Packet switching and TCP/IP are the network itself,
so they are MED's by rule — splitting them into ENG would reopen a membership
decision already closed. There is also a mechanical reason: `MED-150`'s central
claim, that publishing needs no permission from anything in the middle, *is* the
end-to-end design principle of packet switching. The infrastructure and the
consequence are one mechanism. **One entry carries both.**

If `MED-150` needs strengthening on infrastructure, the right additions are a
paragraph on packet switching's origin as survivable communication (Baran and
Davies independently, in the US and Britain) and a **Soviet OGAS region_variant**
— Glushkov's national computing network proposal, never built because ministries
would not surrender control of their data. That is the network failing on
institutional grounds rather than technical ones, and it is the strongest case
available that the end-to-end design was a political settlement.

### Machine Learning and Contemporary AI — RECOMMEND FOR. Propose `ENG-350`.

**The inversion is real, and it is the only thing that justifies a separate
entry.** `MTH-020`'s premise is that a procedure is something a human writes down,
and that writing it down is what makes execution delegable. Machine learning
inverts this: **capability is derived from data rather than specified by a
programmer**, so procedure ceases to be something any human has written or can
fully read. Delegability survives; authorship does not. That is a distinct
mechanism, not a further step on `ENG-333`'s cost curve.

**Placement: ENG. 2012..2025, `decade`.** Above `ENG-340`. Prereqs `ENG-333`,
`ENG-330`. `domains_touched: [nature, commerce, war, media, law]`.
`confidence: low`, `contested: true` throughout.

**Stats — reuse only.** `execution delegability` (now decoupled from a written
procedure); `strategic input dependency` (`ENG-010`, `WAR-275`) — leading-edge
chips are concentrated in a handful of fabrication sites, which is Bronze Age tin
and 1941 oil again; `energy budget` (`ENG-055`), since training at scale is a
material draw on electricity supply.

**It closes an arc the corpus now contains.** Contemporary models depend on large
workforces labelling data and moderating output, substantially in Kenya, the
Philippines, India and Venezuela — **organised, deskilled calculation labour
returning behind the machine that replaced it.** Item 1's mechanism reappears at
global scale, and the entry should link back to `COM-175`.

**Regional.** United States and China as the two centres of capability; Taiwan as
the fabrication chokepoint; Gulf states' investment in compute as a sovereignty
purchase with the same logic as `ENG-340`'s redundant constellations; the
labelling labour geography above.

**Absence (genuine).** *Languages without large digitised corpora* — capability
cannot be derived where the data does not exist, so speakers of low-resource
languages across Africa, South and Southeast Asia receive markedly weaker
capability. The medium-access gate from `MED-040` returns in a new form: a
language's standing is now set by how much of it was ever written down and
digitised.

---

## LINKS

| Proposal | prereqs | enables | key see_also |
| --- | --- | --- | --- |
| `COM-175` | `MTH-020`, `COM-170` | — | `ENG-168`, `COM-180`, `ENG-323`, `MAR-250` |
| `ENG-168` | `MTH-020`, `FIN-020` | `ENG-323` | `ENG-180`, `ENG-190`, `HLT-270`, `WAR-200`, `LAW-165`, `COM-175` |
| `ENG-323` | `ENG-168`, `MTH-080` | `ENG-333` | `COM-175`, `WAR-315` |
| `ENG-333` | `ENG-323`, `ENG-330` | `LAW-240`, `ENG-350` | `MED-150`, `WAR-275` |
| `LAW-240` | `ENG-333`, `LAW-215` | `MED-150` | `LAW-225`, `MED-140` |
| `ENG-350` | `ENG-333`, `ENG-330` | — | `MTH-020`, `COM-175`, `ENG-010`, `WAR-275`, `ENG-055`, `MED-040` |

**Chronology checked on every prereq.** The constraint that matters is on
`ENG-168`: its 1642 start predates `ENG-180`, `ENG-190` and `HLT-270`, and
`COM-175` (1790) cannot prereq it either, so those relationships are `see_also`.

**Existing entries that should gain links once written:** `MTH-080` `enables`
`ENG-323` (retiring its deliberate empty field); `FIN-020` `enables` `ENG-168`;
`MED-150` `prereqs` `LAW-240`; `COM-180` `see_also` `COM-175`; `HLT-270`
`see_also` `ENG-168` (its first links of any kind).

## STATS

**One new stat, `capital specificity`.** Everything else reuses:
`execution delegability`, `stranded capital`, `labour-saving incentive`,
`legibility`, `production cost floor`, `contract enforcement`,
`strategic input dependency`, `skill differential`, `energy budget`.

## ID SLOT SUMMARY

All free, unburned, and not previously named in any audit. No renumbering.

| # | Proposal | Domain | ID | Sorts between |
| --- | --- | --- | --- | --- |
| 1 | Calculation as Organised Labour | COM | `COM-175` | COM-170 (1771) / COM-180 (1811) |
| 2 | Mechanical and Electromechanical Computation | ENG | `ENG-168` | ENG-165 (1550) / ENG-170 (1712) |
| 3 | The Stored-Program Architecture | ENG | `ENG-323` | ENG-320 (1909) / ENG-330 (1947) |
| 4 | The General-Purpose Machine and the Cost Curve | ENG | `ENG-333` | ENG-330 (1947) / ENG-340 (1973) |
| 5 | Software as a Good | LAW | `LAW-240` | above LAW-230 (1948) |
| — | Machine Learning and Contemporary AI | ENG | `ENG-350` | above ENG-340 (1973) |

## PRIORITY

1. **`ENG-168`** — the anchor; Hollerith ties the gap to `FIN-020`, `WAR-200` and
   `LAW-165`.
2. **`ENG-323`** — the conceptual break, and it retires `MTH-080`'s empty `enables`.
3. **`COM-175`** — makes mechanisation legible as substitution.
4. **`ENG-333`** — the institutional sequence; highest skew risk, fix built in.
5. **`LAW-240`** — `MED-150` currently rests on it undocumented.
6. **`ENG-350`** — valuable, low confidence, write last.

## REGIONAL BALANCE

Stated per proposal rather than in aggregate, because the risks differ. Items 1
and 2 are Atlantic-weighted in *invention* for a stated reason (where the demand
for tables and tabulation sat) and corrected in *diffusion and practice*: the
Survey of India, soroban offices, Odhner and Felix, Tiger. Item 3 is wider than
its usual telling (Kiev, Sydney, Bombay). **Items 4 and 5 skewed US in their
default form and the skew is fixed in the proposals**, not noted: Busicom and the
calculator wars, the Soviet clone decision, Taiwanese fabrication and phone-first
Africa for item 4; the Japanese program-right fight, Brazilian market reserve and
European patent rejection for item 5.
