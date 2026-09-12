> **Migration note (2026-09-12):** IDs in this file were rewritten by `id-migrations.md` Run 3, the ENG→MED domain carve-out. Prose is otherwise unaltered.

# Literacy and Written Communication — Coverage Check

**Date:** 2026-09-12
**Scope:** the chain from the emergence of writing through script systems,
canon languages, translation, print, periodical journalism, mass schooling,
and networked text.
**Method:** corpus-wide grep for writing, script, cuneiform, hieroglyph,
alphabet, literacy, scribe, scriptorium, vernacular, translation, Sanskrit,
Literary Chinese, canon, newspaper, press, journalism, censorship, school,
education, examination, library, archive, internet, web — plus khipu, oracle
bone, papyrus, parchment and paper; false positives filtered by word boundary
(`scribe` matching "describe", `press` matching "pressure", `script` matching
"prescription" account for most of the raw hit count). Followed by inspection
of every entry in the chain and sort-position checks in ENG, PHI, REL and LAW.
**No entries written.** Proposals only.

---

## PART 1 — INVENTORY

### What the chain actually contains

Unlike the telecommunications and energy audits, this one found a real chain.
**The middle is built and both ends are missing.**

| Entry | Span | Own Mechanism, or incidental? |
| --- | --- | --- |
| `AGR-050` Storage, Sealing, and Agrarian Accounting | -8000..-3100 | **Own.** Clay tokens, bullae, sealing as accounting — and its `contested_note` carries the token-to-cuneiform lineage thesis explicitly as disputed. This is the closest thing to a writing-origin entry and it is an agriculture entry. |
| `FIN-020` Cadastre, Tax, and Census | -3000..-100 | **Own.** Holds `legibility`, and its americas variant already covers the Inca quipu with the line "state legibility doesn't require a script, only a sufficiently structured record." |
| `LAW-010` Customary Law and Pre-Written Dispute Resolution | -10000..-1754 | **Own**, and it is the corpus's explicit pre-literate baseline: "A functioning legal system requires no literacy at all." |
| `LAW-020` Written Law and the Codification of Inequality | -1754..-450 | **Own.** What writing does to law; holds `legibility`. |
| `MED-050` Papermaking | -200..1200 | **Own.** `enables: [MED-080]`. |
| `MED-080` The Printing Press | 1040..1450 | **Own.** Carries the alphabet-versus-logograph typesetting cost comparison incidentally. |
| `REL-070` Canon — The Institutional Fixation of Doctrine | -250..1382 | **Own** (doctrinal fixation). See item 4. |
| `REL-090` Religious Education: Madrasas, Yeshivas | 220..1500 | **Own.** |
| `HLT-190` Print, Silent Reading, and the Interior Self | 1040..1650 | **Own** (reading as habitus); holds `privacy`. |
| `REL-230` Printed Religion and the Fragmentation of Interpretation | 1522..1650 | **Own** (vernacular print and schism). |
| `LAW-100` Meritocracy and State Bureaucracy — Examination | 605..1854 | **Own.** **The imperial examination system exists**, from Sui 605. |
| `PHI-010` Pre-Axial Wisdom Literature | -2400..-900 | **Own**, and the best existing scribal-class material — "a scribe learns Shuruppak's instructions the same way he learns cuneiform, as professional formation." |
| `WAR-200` Conscription and Mass Armies | 1793..1918 | **Own**, and now holds `literacy` after the conscription fix. |
| `MED-130` Broadcast Radio | 1920..1994 | **Own**, holds `intermediary dependence` and `information sovereignty`. |
| `PHI-260` Enlightenment Sociability | 1650..1800 | **Incidental** — names "the periodical" in a list of venues and nothing more. |
| `DIP-030` Interpreters and Linguistic Mediation | -700..1800 | **Own**, but see the Amarna flag below. |
| `NAT-014` The Algorithm as Delegable Procedure | -1800..1800 | **Own**; holds `literacy`, and its absences already turn on notation. |

### Where it breaks

**Zero hits** for: hieroglyph, scriptorium, Literary Chinese, journalism,
internet, web, khipu (as such), oracle bone. `newspaper` returns one hit, in
`MED-130`, describing editors as an intermediary class radio displaced — a
class no entry documents.

The chain reads: *accounting (AGR-050) → written law (LAW-020) → [gap] →
paper (MED-050) → press (MED-080) → reading habitus (HLT-190) → print
religion (REL-230) → [gap] → broadcast (MED-130)*.

**The four breaks:**

1. **No entry on writing itself.** Writing is everywhere as an assumption and
   nowhere as a mechanism. `AGR-050` gets closest and is about storage.
2. **No entry on script systems.** No alphabet, no logographic writing, no
   treatment of sign count as a cost variable — despite `MED-080` relying on
   exactly that comparison to explain why European movable type scaled faster.
3. **No entry between print and broadcast.** Three centuries of periodical
   journalism missing entirely, which leaves `MED-130`'s central claim —
   that broadcast collapsed an intermediary layer of editors and notables —
   resting on an intermediary class the corpus never establishes.
4. **No entry on networked text.** `ENG-330` semiconductors (1947) and
   `ENG-340` satellite positioning (1973) exist; nothing carries the network.

---

## PART 2 — PROPOSALS

All eight slots below were verified **free and clean of burned `former_ids`**
(the constraint that forced a reuse in the NAT-060 split). No renumbering
required; `renumber.py` not run.

### 1. The Emergence of Writing — ABSENT. Propose `ENG-052`.

**Placement: ENG**, and it is arguable. ENG already owns the entire
recording-and-transmission chain — `MED-030` relay messaging, `MED-050` paper,
`MED-080` press, `MED-100` telegraph, `MED-130` broadcast — and writing is the
foundational member of exactly that series. The counter is real and should be
recorded: `AGR-050` already owns the token-and-sealing accounting practice and
`FIN-020` owns state enumeration, so a new entry must be disciplined about
owning **the notation system itself** and what it makes possible, leaving the
storage practice to AGR and the census apparatus to FIN. If it cannot hold
that line it should be folded into `AGR-050` instead.
`domains_touched: [law, finance, agriculture, philosophy]`. **-3300..900,
`era`.** Prereq `AGR-050`; **`FIN-020` is the legibility entry to cross-link**
— note that the brief's suggestion to check AGR for one is not borne out, since
no AGR entry uses the stat at all.

**Mechanism.** Writing originates as accounting, not expression. Proto-cuneiform
at Uruk around -3300 records rations, livestock and grain, and the earliest
substantial corpus anywhere is administrative rather than literary or sacred.
The mechanism is that **a record makes a quantity checkable by someone who did
not witness it** — which detaches verification from presence and from memory,
and is the precondition for `legibility` as `FIN-020` defines it. A witness can
be mistaken, bought, or dead; a sealed tablet can be produced years later in
front of someone with no connection to the transaction.

**Region_variants — four independent inventions, no European instance at all.**
Mesopotamia c. -3300; Egypt c. -3250, with the stimulus-diffusion question
flagged as contested rather than settled either way; Shang oracle bones c.
-1200, which invert the usual origin story by being divinatory rather than
administrative and are the case that stops "writing begins as accounting" from
being stated as a law; and Mesoamerica, Zapotec c. -500 and Maya, developing
with no Old World contact whatever. The Mesoamerican case is not optional and
must not be a footnote — it is the control that makes the accounting-origin
claim testable rather than a generalisation from one region.

**The khipu decision: region_variant, not absence.** The entry's mechanism is
recording-for-delegated-verification, not language-encoding. Khipu administered
an empire of ten million on exactly that mechanism — census, tribute, storehouse
contents — so by this entry's own definition it qualifies, and classifying it
as an absence would require redefining the entry around glottography, which
would make it a linguistics entry rather than an administrative one. Carry the
genuinely unresolved question (whether narrative khipu encode language) as
contested. `FIN-020` already states the principle and should be cross-linked
rather than re-argued.

**Absence (genuine).** *central-asia-steppe* — mobile pastoral polities of
enormous extent operate for millennia without indigenous script, and the
mechanism explains why: writing is invented to count things held in a fixed
place, and a polity with no granary and no storehouse has nothing for a tally
to refer to. Script arrives later by adoption, for diplomacy and religion
rather than administration. *oceania* — complex chiefdoms with deep hierarchy
and no recording system of any kind, where verification runs through personal
hierarchy and memory specialists, which caps the scale at which a single
authority can check anything against a claim.

### 2. The Alphabet — ABSENT. Propose `ENG-082`.

**Placement: ENG. -1800..800, `era`.** Prereq `ENG-052`.
`domains_touched: [commerce, law, religion, philosophy]`.

**Mechanism.** Sign count collapses from hundreds or thousands to a few dozen,
and **the training cost of literacy collapses with it** — which means literacy
can stop being a profession. That is the whole entry. A script requiring years
of specialist schooling produces a scribal class with a monopoly on records; a
script learnable in months does not, and a merchant can keep their own books.
Proto-Sinaitic c. -1800, Phoenician c. -1050 and its spread with trade, Greek
vowel notation c. -800, Aramaic and its enormous descendant family across Asia,
and Brahmi with the Indic scripts. The regional weight sits in the Levant and
Asia, not Europe.

**CONTESTED, and this is the entry's most important content.** The strong claim
that alphabetic writing caused Greek rationalism, democracy, or abstract thought
— Havelock, Goody, Ong — is largely rejected and the entry must not carry it.
Two facts kill it: Qing examination literacy was substantial on a logographic
script, and alphabetic Europe ran at low literacy for two millennia after
acquiring the alphabet. **Script complexity is a modifier on training cost, not
a determinant of intellectual or political outcome.** Model as a conditional per
design principle 6 — sign count sets the cost of producing a reader, and what a
society does with cheap readers depends on institutions the script has no
bearing on. A model in which the alphabet produces democracy is falsified by
every case in item 3.

### 3. Logographic Script and Written Unity — ABSENT. Propose `ENG-105`.

**Placement: ENG**, with `domains_touched: [diplomacy, law, philosophy,
religion]` — this is a political technology and the touches should carry that.
**-200..1900, `era`.** Prereq `ENG-052`; `see_also` `ENG-082` as the direct
contrast.

**Mechanism — the exact inverse of the alphabet's.** A phonetic script encodes
sound, so it is readable only by someone who knows the language. **A
semantically anchored script encodes meaning, so it is readable by speakers of
mutually unintelligible languages** — and writing can therefore unify a polity
that speech cannot. Literary Chinese was read across Cantonese, Hokkien, Wu and
Mandarin speakers who could not converse, and beyond the empire entirely as
*kanbun* in Japan, *hanmun* in Korea and *chữ Hán* in Vietnam. **Brush-talk**
is the concrete practice to carry: educated men from different countries,
sharing no spoken language, holding extended written conversations in person by
passing paper. That is the mechanism made visible in a single image.

The cost side is real and must be stated without making this a deficit entry:
high training investment, a literate class bound to a canon, and a written
register that must be acquired separately from any spoken language. **Both
scripts trade the same variable in opposite directions** — the alphabet buys
cheap readers at the price of fragmenting the readership along language lines;
the logograph buys a readership spanning languages at the price of expensive
readers. Neither is the better technology and the entry should make it
impossible to score them against each other.

### 4. Canon as the Fixation of a Language of Knowledge — SEPARATE ENTRY. Propose `PHI-165`.

**Verdict on the brief's question: this does not fit inside `REL-070`.**
`REL-070`'s Mechanism is canon-fixation as a power-allocating institutional act
— who convened the council, what was excluded, who lost. The language mechanism
is a different causal story with its own social consequence, its own
absence structure, and four instances that are only partly religious. Under the
one-entry-one-mechanism rule that is a separate entry, not an extension.

**Placement: PHI, not REL**, and the argument matters. `boundary-rules.md` gives
REL institution, practice and community, and PHI systematic argument — neither
fits cleanly, but the deciding fact is that **two of the four instances are
substantially secular**. Literary Chinese as a language of learning is not a
religious formation at all, and the Latin *res publica litterarum* outlives its
ecclesiastical origins. Housing this in REL would make religion the owner of
the medium of all higher learning, including its non-religious majority. PHI
already holds `intellectual authority` and `PHI-030`'s schools of thought, which
are the right neighbours. `domains_touched: [religion, law, engineering,
diplomacy]`, prereq `REL-070`. **-250..1800, `era`.**

**Mechanism.** Fixing a canon freezes the language it is written in. Because the
canon is authoritative, its language becomes the medium of all higher learning —
and because the canon cannot change, the language stops evolving while
vernaculars continue to drift. Three consequences:

- **Diglossia widens by itself.** The gap between the language of knowledge and
  the language of speech grows for centuries without anyone deciding it should,
  until the medium of learning is one nobody speaks natively and everyone must
  acquire deliberately.
- **A second gate on literacy.** Knowing letters is not enough. You must learn a
  second language before you can read anything that matters, which **raises
  `intermediary dependence`** — reusing the existing stat rather than inventing
  one, per the brief. This is the mechanism by which a script that is cheap to
  learn still fails to produce a reading public.
- **A transnational scholarly community.** A shared written language creates a
  network larger than any polity. **Latin, Arabic, Sanskrit and Literary Chinese
  are four co-equal instances of one mechanism** and the entry must be built
  that way — Latin is not the frame and the other three are not variants of it.
  The Sanskrit cosmopolis across South and Southeast Asia and the Arabic
  scholarly world are each larger in extent than Latin Christendom. Ge'ez,
  Church Slavonic, Pali and Hebrew as further cases if there is room.

### 5. Vernacular Translation — PARTIALLY EXISTS. Propose `REL-085`.

`REL-230` covers vernacular print and the Reformation (1522-1650) and is good,
but it owns *print-driven schism*, not translation. The translation mechanism
runs for fourteen centuries before Luther and its most important instances are
not European.

**Placement: REL** (translation of a canon is an institutional act on an
institution's own authority), `domains_touched: [philosophy, law, engineering,
migration]`. **200..1650, `era`.** Prereq `PHI-165`; enables `REL-230`.

**Mechanism, and it is double-edged.** Translating the canon destroys the
intermediary's monopoly — and simultaneously **creates the vernacular as a
language capable of carrying serious content**, which it was not before. Luther
standardises German and the King James standardises English as by-products of a
religious act.

**The regional content is the entry, not decoration.**

- **The Chinese Buddhist translation project** — centuries of sustained,
  institutionally organised work rendering Sanskrit into Chinese, with
  Kumārajīva's team and Xuanzang's state-backed bureau, among the largest
  translation efforts in human history. Critically, **it strengthened the
  institution rather than fracturing it**, which is the direct counterexample to
  reading translation as inherently schismatic.
- **The bhakti movements** composing devotional literature in Tamil, Marathi,
  Hindi and Bengali against Sanskrit's monopoly — translation as a challenge
  from below, and the clearest case of the vernacular being *made* serious by
  the act.
- **Islam as the branch that proves the mechanism is conditional.** The doctrine
  of *i'jaz*, the Qur'an's inimitability, meant a translation was never
  scripture but only an interpretation — so the same technology produced no
  vernacularisation, and Arabic's position as the language of learning was
  preserved rather than dissolved. This is the entry's most valuable case
  because it shows the outcome is a doctrinal variable, not a mechanical one.

**The combination requirement must be stated.** Print alone did not produce a
Reformation — **China had print for seven centuries without one**, a fact
`REL-230` already notes and this entry should inherit. The condition is
translation *plus* print *plus* a doctrinal schism with institutional backing,
and any two of the three produce nothing.

### 6. Newspapers and Periodical Journalism — ABSENT. Propose `ENG-167`.

**Placement: ENG**, on the `MED-130` precedent — broadcast radio is an
institution built on a transmission technology and sits in ENG, carrying its own
funding-model and censorship material. A newspaper entry is the same shape and
should sit beside it rather than in COM. `domains_touched: [commerce, law,
diplomacy, philosophy]`. **1605..1900, `decade`.** Prereq `MED-080`;
`see_also` `MED-100` and `MED-130`.

**Mechanism: periodicity, which is distinct from print as an artifact.** A book
is read whenever; **a newspaper arrives on a schedule**, and that synchronises a
population's attention. The reader knows thousands of strangers are reading the
same thing this morning — Anderson's imagined community — and that is the
nation-formation mechanism, operating through simultaneity rather than content.

Also carries: **the funding model determining the institution** (subscription,
advertising, party subsidy, state ownership), which is the same insight
`MED-130` uses for the Volksempfänger-versus-BBC contrast and should be
cross-linked rather than re-derived; censorship and licensing regimes; **wire
services after the telegraph**, standardising both content and the "objective"
style, which links directly to `MED-100`'s arbitrage-rent argument since the
same cable that killed the merchant's information premium created the news
agency; and the penny press and mass circulation.

**Regional.** The Ottoman and Egyptian Arabic press from the 1820s-1870s,
including *Takvim-i Vekayi* and the Beirut-Cairo publishing world; the Chinese
treaty-port press; and **the Indian vernacular press with the Vernacular Press
Act of 1878** — the clearest case anywhere of a colonial state fearing a medium
it could not read, since the Act applied to vernacular papers and not English
ones, which states the mechanism in the legislation itself.

**Note:** the entry that carries the famine-and-free-press argument does not
exist. `HLT-110` Famine and Starvation Mortality covers physiological mortality
channels with no entitlement or information material, and `AGR-270` covers
monoculture. Sen's argument has no home; flagged below rather than assumed.

### 7. Compulsory Education — ABSENT. Propose `LAW-165`.

**Placement: LAW. 1763..1950, `decade`.** `domains_touched: [war, commerce,
philosophy, health]`.

**Mechanism, and the structural identity is the point.** The state takes custody
of children for a fixed portion of every day for years. **This is the same
instrument as conscription** — a legal obligation reaching every household,
requiring a register of births and residences, with exemption adjudication and
enforcement against evasion — and the entry should say so explicitly and link
`WAR-200` both ways. The conscription fix flagged this entry as its missing
counterpart; `WAR-200` currently argues that dispersed tactics made reading
competence a military requirement and has nothing to point at.

Consequences: `literacy`, language standardisation, nationalisation, `time
discipline`, and **the removal of children from the household labour force** —
a real economic shock to poor families and the main historical source of
resistance, which is what makes truancy enforcement the expensive half.

**Two cases that complicate the standard story and should be load-bearing.**
**Sweden's household examination system from 1686** produced near-universal
reading ability with no state schools at all — the parish tested household
members on reading and catechism, and marriage required passing. That is
near-universal literacy achieved by a church using a legal lever, and it
falsifies any model in which schooling is the only route. **The Chinese
examination system** (`LAW-100`, from 605) generated sustained elite literacy
demand for a millennium with no compulsion whatever, by making literacy the sole
path to office — demand-side rather than supply-side, and quite possibly the
single strongest pre-modern literacy-driving institution anywhere. It exists in
the corpus and should be prereq'd, not re-derived.

Further cases: Prussia from 1763, Meiji Japan's 1872 system (which `WAR-200`
already notes preceded conscription by a year), and the Soviet *likbez*
campaign as mass adult literacy by state mobilisation.

### 8. The Global Network — ABSENT. Propose `ENG-335`.

**Placement: ENG. 1969..2010, `decade`.** Prereq `ENG-330`; `see_also`
`MED-130`, `LAW-225`, `MED-100`.

**Mechanism: many-to-many at near-zero production and distribution cost.** The
series is exact and should be built as one. Print is one-to-many with high
production cost. Broadcast is one-to-many with near-zero *distribution* cost and
a high production cost — which is `MED-140`'s production-cost floor. This is the
first medium in which **the marginal cost of becoming a publisher is also near
zero**, which dissolves the publisher as a gatekeeping institution.

**The inversion is the entry's best content.** The intermediary returns as the
platform. Ranking, recommendation and moderation reconstitute an editorial
function that nobody elected and that operates at a scale no editor ever did.
Read against `MED-130`'s intermediary-collapse mechanism this is **the same stat
moving the other way** — `intermediary dependence` falls to near zero as
publishing is democratised and then climbs again as discovery is centralised,
and the second intermediary is less accountable than the first because it
never claimed the role.

Also: the border-crossing property radio introduced becomes both harder to jam
and harder to bound, so **`information sovereignty` is contested by a different
instrument** — filtering at the network edge, platform pressure, and
infrastructure control rather than transmitter interference.

**Regional, and this one needs deliberate work or it defaults to a US story.**
China's network governance and filtering architecture is the single most
developed instance of information sovereignty reasserted against the medium, and
belongs at the centre rather than as a counterexample. India's mobile-first
adoption, skipping the desktop stage entirely, is a different network shape with
different consequences. Kenyan M-Pesa shows the network carrying value rather
than text and reaching populations no prior medium served. The Arab Spring
belongs as the **contested** case precisely because the strong claim has not
held up.

---

## PART 3 — THREAD

Propose **`_meta/threads/literacy-and-intermediaries.md`** — links only, per the
threads convention and the `axial-age.md` precedent (a short framing paragraph
establishing what the thread claims, then `[[ID]]` links with one-line labels
and no prose duplicating the entries).

The arc: **scribal class → alphabet lowers the gate → canon in a frozen language
raises it → translation and print lower it → compulsory schooling demolishes it
→ broadcast bypasses literacy entirely → platforms rebuild the gate in a new
form.**

This spans ENG, REL, LAW, COM, PHI, WAR, HLT and FIN, and it tracks a single
variable — `intermediary dependence` — across roughly five thousand years, which
no existing thread does. It is the strongest thread candidate in the corpus and
it is worth noting that **it cannot be assembled today**: four of its seven
stages are the proposals above. The thread should be written after items 1, 2,
4 and 7 exist, not before.

---

## FLAGGED, NOT WRITTEN

- **Writing materials as a cost gate.** Parchment costs a herd — a single large
  codex consumes the skins of a small flock, which is why palimpsests exist at
  all. Papyrus is regionally bounded by where the plant grows and by Egyptian
  export control. Paper changes the economics of everything downstream.
  **`MED-050` does link forward to `MED-080`**, correctly, but the cost
  mechanism is stated nowhere in the corpus: `MED-050` has `stats_touched: []`
  and describes technique and diffusion rather than price per page. The cheapest
  fix is to add the cost argument to `MED-050` rather than write a new entry.
- **Libraries and archives** as institutions of accumulated written knowledge.
  `NAT-110` (Sack of Baghdad) already models their *destruction* and
  `accumulated scientific corpus` exists as a stat, so the corpus can destroy a
  library it never built.
- **Cuneiform as a diplomatic lingua franca.** Akkadian cuneiform in the Amarna
  correspondence — courts sharing no spoken language conducting sustained
  diplomacy in a third written one. **`DIP-030` has zero Amarna or Akkadian
  content**, despite being the interpreters-and-linguistic-mediation entry and
  spanning -700 onward. Probably a `DIP-030` extension rather than a new entry,
  and it connects directly to item 3's mechanism.
- **The famine-and-free-press argument** has no home (above).
- **`REL-070`'s `absence` field holds an alternative institutional choice** —
  Chinese Buddhism's deliberately open canon — rather than an absence. This is
  the **sixth** confirmed instance of the defect (after `MED-100`, `ENG-290`,
  `WAR-200`, the energy case, and `NAT-060`). The corpus-wide sweep is overdue.

---

## STATS

Reused throughout, per the brief: `legibility`, `intermediary dependence`,
`information sovereignty`, `literacy`, `intellectual authority`, `religious
authority`, `information speed`, `time discipline`, `privacy`.

**Two new stats proposed, both serving multiple entries:**

| stat | entries | definition sketch |
| --- | --- | --- |
| `literacy training cost` | items 2, 3, 4, 7 | the time and expense of producing one functionally literate person — set by sign count, and doubled where a canon language must be acquired before anything worth reading becomes accessible. The variable the alphabet lowers, the logograph raises, the canon language raises again, and compulsory schooling socialises. |
| `simultaneity of attention` | items 6, 8 | the share of a population consuming the same content within the same short window. Distinct from `broadcast reach`, which is how many *could* receive: this is how many receive *at once*, which is what converts an audience into a public. Produced by periodicity, maximised by scheduled broadcast, and fragmented again by on-demand networked media. |

## REGIONAL BALANCE

Checked deliberately, since this chain is the easiest in the corpus to write as
a European story with appendices. As proposed: item 1 has **no European
instance at all** (Mesopotamia, Egypt, China, Mesoamerica); item 2 sits in the
Levant and Asia; item 3 is entirely East Asian; item 4 treats Latin, Arabic,
Sanskrit and Literary Chinese as four co-equal instances; item 5's centre of
gravity is the Chinese Buddhist translation project and the Islamic
counter-case; item 6's sharpest case is the Vernacular Press Act; item 7's two
complicating cases are Swedish and Chinese; item 8 centres Chinese network
governance. **No proposal skews European, and three are majority non-European
by weight.**

## ID SLOT SUMMARY

All free, and all clean of burned `former_ids`. No renumbering.

| # | Proposal | Domain | ID | Sorts between |
| --- | --- | --- | --- | --- |
| 1 | The Emergence of Writing | ENG | `ENG-052` | ENG-050 (-3500..1851) / ENG-055 (-3300..1800) |
| 2 | The Alphabet | ENG | `ENG-082` | ENG-080 (-2500) / MED-030 (-550) |
| 3 | Logographic Script and Written Unity | ENG | `ENG-105` | MED-050 (-200..1200) / ENG-110 (-100) |
| 4 | Canon as a Language of Knowledge | PHI | `PHI-165` | PHI-160 / PHI-170 |
| 5 | Vernacular Translation | REL | `REL-085` | REL-080 / REL-090 |
| 6 | Newspapers and Periodical Journalism | ENG | `ENG-167` | ENG-165 (1550..1900) / ENG-170 (1712) |
| 7 | Compulsory Education | LAW | `LAW-165` | LAW-160 (1679..1900) / LAW-170 (1764..1900) |
| 8 | The Global Network | ENG | `ENG-335` | ENG-330 (1947..1990) / ENG-340 (1973) |

Neighbours verified, and two are thematically apt as well as chronologically
correct: `REL-085` lands between `REL-080` Missionary Work and `REL-090`
Religious Education, making missionary expansion → translation → clerical
training a coherent run; `ENG-082` lands immediately before `MED-030` Relay
Messaging, so writing → alphabet → organised relay reads as one chain.

**One fragility to record.** `LAW-165` depends on a one-year margin —
`LAW-170` Codified Criminal Law starts in 1764, so a compulsory-education entry
dated 1763 (Prussia's *Generallandschulreglement*) sorts before it and one dated
1765 or later sorts after, into `LAW-171`-`179`. Either slot is free and the
entry works in both; the point is that the date should be fixed on the evidence
first and the slot read off afterwards, not the reverse.

## BUILD ORDER

`ENG-052` → `ENG-082` → `ENG-105` → `PHI-165` → `REL-085` → `ENG-167` →
`LAW-165` → `ENG-335` → thread. Each depends on the one before for a prereq or
a contrast, and the thread cannot be assembled until items 1, 2, 4 and 7 exist.

## PRIORITY

1. **`ENG-052` writing** — the chain has no foundation; `legibility` is used by
   29 entries and nothing establishes where a record comes from.
2. **`ENG-167` newspapers** — closes the print-to-broadcast gap and supplies the
   intermediary class `MED-130` already assumes.
3. **`LAW-165` compulsory education** — `WAR-200` was written pointing at it.
4. **`PHI-165` canon language** — highest-value non-obvious mechanism.
5. **`ENG-082` alphabet** and **`ENG-105` logographic** — a matched pair;
   writing either alone loses the contrast that makes both work.
6. **`REL-085` vernacular translation**.
7. **`ENG-335` global network**.
