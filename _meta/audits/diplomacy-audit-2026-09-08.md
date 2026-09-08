# Diplomacy (DIP) Domain Audit — 2026-09-08

Hostile adversarial review. All 12 entries read in full (DIP-001 through
DIP-012), plus `_meta/schema.md`, `_meta/conventions.md`,
`_meta/design-principles.md`, `_meta/boundary-rules.md`, and cross-checks
against LAW, WAR, ENG, MIG, COM, REL for prereq validity and boundary
violations.

**The pattern, stated up front:** this is the smallest domain audited this
cycle (12 entries) and it reads like a list of great diplomatic topics rather
than a connected mechanic system. Every single entry is `tier: core` — zero
`standard`, zero `stub` — and 8 of the 12 have an entirely empty `enables`
field. The graph this domain actually forms is a handful of short, disconnected
spines (DIP-001→DIP-003→DIP-005, DIP-001→DIP-004, DIP-006→{008,009,011}→012)
plus three genuinely isolated nodes (DIP-002, DIP-007, DIP-010) that neither
feed nor are fed by anything else in the domain. Layered on top of that
structural thinness: the domain's own prereq chain contains a 709-year
chronological inversion between its two earliest, most load-bearing entries.
Everything else below is real, but the underlying story is a domain that
reads as well-written prose organized by topic, not yet as a working system
of dependencies.

═══ PART A — DIAGNOSTIC SWEEP ═══

## 1. EMERGENCE GAP — FAIL

DIP-003 (Treaty and Oath, -1259) is the domain's actual earliest entry
chronologically, and it opens by asserting treaties rely on "sworn oath,
divine witness, and reputational consequence" — which already assumes two
polities can safely make contact, exchange envoys, and negotiate at all.
Nothing in the domain covers the more primitive precondition beneath every
single entry here: that a messenger, herald, or envoy — even one sent by or
to a hostile power — is treated as inviolable rather than killed on sight.
Without that norm, DIP-001's tribute negotiations, DIP-003's oath-swearing,
DIP-004's hostage exchange, and DIP-005's resident ambassadors are all
physically impossible to initiate. The domain begins at "here is how two
polities that can already safely talk to each other formalize their
relationship" and never asks how they got to safely talking in the first
place.

## 2. SUBSTRATE GAP — FAIL

The unglamorous layer every entry here silently assumes: **linguistic
mediation**. Not one of the 12 entries addresses how two polities without a
shared language actually communicate — grep across the entire wiki for
"interpreter," "dragoman," or any comparable term returns nothing relevant.
DIP-001's tribute missions, DIP-003's treaty negotiations, DIP-005's resident
embassies, and DIP-007's first-contact colonial diplomacy all require someone
in the room converting one language into another, and none of them say so.
This is exactly the kind of boring-but-load-bearing infrastructure the probe
is built to catch — the domain covers the spectacular outcome (a signed
treaty, a stationed ambassador) and skips the person who made the
conversation possible at all.

## 3. SCALE GAP — FAIL at one level

Sorting the 12 entries: multilateral/world-system (DIP-006, DIP-009, DIP-011,
DIP-012, DIP-010) and bilateral/state-to-state (DIP-001, DIP-003, DIP-004,
DIP-005, DIP-008) are both well covered. Missing entirely: **the non-sovereign
diplomatic actor** — an institution that is neither a state nor a
subordinate of one, but a full negotiating party or arbiter in its own
right. This is not a hypothetical gap: DIP-007 names the Treaty of
Tordesillas (1494) as its own founding case and never mentions that it was
personally arbitrated by Pope Alexander VI, and COM-015 (the Hanseatic
League) already carries `domains_touched: [diplomacy]` and describes itself
negotiating trade privileges and "on occasion wag[ing] war" against sovereign
states — with no DIP entry anywhere for it to actually point to. The
domain's whole framing treats "diplomacy" as something only territorial
sovereigns and their subordinates do.

## 4. REVERSE-DIRECTION GAP — FAIL, sharply

Of 12 entries, none has collapse, rupture, or capability loss as its own
Hook. DIP-009's own Mechanism section says the Concert's stability "is
exactly what eventually fails as rising and revisionist powers judge the
existing arrangement no longer serves their own interests" and then simply
stops — the 1914 cascade into general war that the entry's own date range
ends at is described, not modeled. DIP-011 similarly flags "the League's
practical failures" as color without an entry. And nothing anywhere models
the mirror image of DIP-006/DIP-008's recognition-granting mechanic:
recognition being _withdrawn_ — a state that held recognized status for
decades losing it (Taiwan's 1971 UN seat transfer to the PRC is the paradigm
case, unmentioned anywhere in this domain). Design principle 4 (depleting
pools) has zero instances here, same as every other domain audited this
cycle so far — this domain's own near-miss is DIP-009's "stability lasts
exactly as long as it's valued" language, which gestures at a depleting pool
without ever cashing it out as one.

## 5. PREREQ ORPHANS — no dead links, but one severe and three minor

chronological inversions

Every prereq target resolves to a real entry — clean on the narrow dead-link
question. But checking each prereq's `date_start` against its dependent's:

- **DIP-003 (start -1259) prereqs DIP-001 (start -550) — a 709-year
  inversion**, the worst-in-class finding for this domain and one of the
  worst found in any domain audited this cycle. The Egyptian-Hittite Treaty
  of Kadesh predates the Achaemenid tribute system DIP-003 supposedly
  requires by seven centuries. The causal logic doesn't obviously repair by
  reversing the arrow either — oath-bound treaty diplomacy between two
  roughly equal great powers (Kadesh) doesn't plausibly require hierarchical
  tribute diplomacy to exist first, and the Aztec case DIP-001 itself covers
  shows tribute relationships can rest on bare threat rather than sworn
  oath at all. This reads like a prereq assigned by topic-adjacency
  ("both are early diplomacy") rather than by actual dependency.
- **DIP-002 (start -520) prereqs ENG-003 (start -500)** — a 20-year
  inversion; minor, but real.
- **DIP-007 (start 1494) prereqs MIG-012 (start 1500)** — a 6-year
  inversion; minor, but real, and slightly embarrassing given DIP-007's own
  opening date is the Treaty of Tordesillas, which precedes any actual
  American colonization it's supposedly conditioned on.
- **DIP-008 (start 1776) prereqs LAW-011 (start 1789)** — a 13-year
  inversion; American independence is declared thirteen years before the
  U.S. Constitution DIP-008 lists as its own domestic-legitimacy
  precondition.

## 6. MECHANISM DIVERSITY — FAIL on pool and event, modifier-heavy overall

Rough Hook classification: modifier ≈8 (DIP-001 through DIP-005, DIP-007,
parts of DIP-009/012), branch ≈3 (DIP-008 and DIP-010 are genuinely strong,
symmetric branches; DIP-012 partially), gate ≈2 (DIP-006, DIP-005's immunity
precondition), civic ≈1 (DIP-011's Non-Aligned Movement clause), pool = **0**,
event = **0**. Two of this domain's Hooks are real standouts — DIP-008's
Haiti-versus-Meiji "same mechanism, opposite outcome depending on how
threatening the claim is" and DIP-010's Partition-versus-postcolonial-Africa
"same gap, resolves peacefully or catastrophically" are exactly the kind of
branch design principle 6 asks for. But two-thirds of the domain is
modifier, and there is no pool and no event anywhere — the same gap probe 4
identifies from a different angle.

## 7. BOUNDARY INTEGRITY — mostly PASS, one real domain question, one real

cross-domain linking gap

The LAW/DIP split ("a treaty is always DIP") is correctly observed — DIP-003
sits in DIP, not LAW, and DIP-008 correctly touches `law` for LAW-011 rather
than trying to own constitutional legitimacy itself. DIP-007's own deferrals
(MIG-004/012 for colonization mechanics, COM-016 for chartered-company
structure) are a clean, well-executed multi-domain split — a real PASS
example. One live question: **DIP-002** ("Roads, Garrisons, and Imperial
Unification") is framed entirely around a capital projecting administrative
reach over _its own_ subject population — that's arguably internal state
capacity, not "inter-polity relation" at all, and it's notable that this is
also one of the domain's three fully isolated, non-enabling entries. I'm not
convinced it's mis-domained outright (early empires' subject/foreign
boundary is genuinely blurry, and the entry is really about backing DIP-001's
tribute relationships with physical projection capacity), but neither DIP-001
nor DIP-002 currently references the other at all despite being obviously
related — see Part C. One confirmed **cross-domain linking gap**: COM-015
(Hanseatic League) carries `domains_touched: [diplomacy]` and describes
explicitly diplomatic behavior (collective bargaining, occasional war
against sovereigns) with zero DIP entry in its `see_also` to point to,
because no DIP entry currently owns non-sovereign diplomatic actors — see
Part B.

═══ PART B — PROPOSED ADDITIONS ═══

## LOAD-BEARING

**1. Envoy Immunity and the Sanctity of the Messenger**

- Dates: -2000 to -500, `era`. Regions: middle-east, mediterranean,
  east-asia.
- Answers: Probe 1 (Emergence Gap).
- Mechanism: A norm — near-universal across unconnected traditions —
  holding that a herald or envoy, even one sent by or to an enemy, may not
  be killed or physically punished for the message they carry, converts
  inter-polity contact from something that can't safely be initiated at all
  into a repeatable, low-risk channel every later diplomatic mechanism in
  this domain depends on.
- Hook: A gate: establishes safe, repeatable inter-polity contact as this
  domain's actual starting condition — the precondition DIP-001, DIP-003,
  DIP-004, and DIP-005 all silently assume is already available.
- Prereqs: none. Enables: DIP-001, DIP-003, DIP-004, DIP-005.
- Contested: true — how uniformly the norm was actually honored versus
  violated (the killing of an envoy is itself a recurring, deliberately
  provocative act of war precisely because it breaks so widely shared a
  norm) is a real, case-by-case historical question; model envoy immunity as
  a strong default norm whose violation is a modelable, escalatory act in
  its own right, not an absolute rule with no exceptions.
- Confidence: high.

**2. Interpreters and Linguistic Mediation in Diplomacy**

- Dates: -700 to 1800, `era`. Regions: middle-east, east-asia, americas.
- Answers: Probe 2 (Substrate Gap).
- Mechanism: Cross-linguistic negotiation requires a trusted, competent
  intermediary — the Ottoman-European dragoman, the Chinese tributary
  system's court interpreter-officials, the Nahuatl-Spanish interpreters
  (Malintzin the paradigm case) present at first contact in the Americas —
  whose own competence, loyalty, and potential for deliberate mistranslation
  becomes a real, unavoidable variable in how faithfully any cross-language
  negotiation's actual terms get transmitted at all.
- Hook: A modifier: converts the fidelity of any cross-linguistic
  negotiation into a function of a specific, fallible, potentially
  self-interested intermediary rather than a costless given — a mistrusted
  or absent interpreter is a real constraint on which of this domain's other
  mechanisms (DIP-001, DIP-005, DIP-007) can function reliably at all.
- Prereqs: none. Enables: DIP-001, DIP-005, DIP-007.
- Contested: false.
- Confidence: high.

**3. Non-Sovereign Diplomatic Actors — Papal Arbitration and Merchant-League Negotiation**

- Dates: 1358 to 1648, `century`. Regions: europe.
- Answers: Probe 3 (Scale Gap) and the confirmed COM-015 linking gap.
- Mechanism: A non-territorial-sovereign institution — the Papacy, invoking
  religious authority to arbitrate between Catholic monarchs (Alexander
  VI's 1493 bulls, formalized as the 1494 Treaty of Tordesillas, DIP-007's
  own founding case); the Hanseatic League, pooling dozens of individual
  cities' otherwise-insufficient leverage into collective bargaining power
  and occasional war-making against territorial rulers — can function as a
  full diplomatic party or arbiter in its own right, not merely as a
  subordinate or advisor to a sovereign state.
- Hook: A gate: supplies the actual mechanism behind DIP-007's own opening
  case (who arbitrated Tordesillas, and why the parties accepted it), and
  gives COM-015's already-declared diplomatic relevance an actual DIP entry
  to point to.
- Prereqs: none. Enables: DIP-007. See_also: COM-015.
- Contested: true — how much genuine independent authority the Papacy's
  arbitration carried versus how much it functioned as after-the-fact
  cover for an division the parties had already substantially agreed to is
  disputed among historians of the Iberian union; model papal arbitration's
  binding force as tracking the arbitrating monarchs' own continued
  interest in a neutral-seeming settlement, not as independent religious
  authority alone.
- Confidence: high.

**4. Alliance Cascade — The Collapse of a Diplomatic System into General War**

- Dates: 1914, `year` (the July Crisis specifically; the entry's broader
  scope is the general mechanism, dated to its clearest case). Regions:
  europe.
- Answers: Probe 4 (Reverse-Direction Gap) and Probe 6 (Mechanism
  Diversity — this domain's first genuine pool/event mechanic).
- Mechanism: A system of standing bilateral and multilateral commitments
  (DIP-009's Concert-era spheres of interest, hardened into the more rigid
  alliance network that succeeds it) converts a localized bilateral dispute
  into a rapid, largely automatic cascade once one commitment triggers the
  next — the same standing-commitment architecture that reduces
  everyday friction (DIP-009's own stated function) removes each
  individual power's ability to contain a crisis once the first
  mobilization order is issued, destroying in weeks a coordination
  structure that took decades to build.
- Hook: A pool-drain event: the accumulated inter-power trust and
  coordination capacity DIP-009 and DIP-011 both implicitly depend on is
  not just something that erodes gradually as interests diverge, but
  something a single triggering crisis can catastrophically zero out
  through the same alliance architecture that was supposed to prevent
  exactly that outcome.
- Prereqs: DIP-009. Enables: DIP-011 (the League's founding is a direct,
  explicit response to this entry's own event).
- Contested: false — the cascade mechanism itself is well-documented,
  though which specific decision at which specific point was the truly
  decisive one remains debated historical detail below this entry's own
  seed-level scope.
- Confidence: high.

## VALUABLE

**5. Diplomatic Derecognition and the Rupture of Relations**

- Dates: 1949 to 1979, `century`. Regions: east-asia, americas.
- Answers: Probe 4 (Reverse-Direction Gap — the mirror image of DIP-006/
  DIP-008's recognition-_granting_ mechanic).
- Mechanism: A state that held recognized diplomatic status for years or
  decades can lose it — not through military defeat, but through other
  states' collective decision to recognize a rival claimant instead
  (Taiwan's 1971 loss of the UN China seat to the People's Republic) —
  converting recognition from something won once and permanently banked
  into a standing status that remains contingent on continued diplomatic
  consensus indefinitely.
- Hook: A branch: recognition, once granted (DIP-006, DIP-008), remains
  reversible for as long as a rival claimant with a competing legitimacy
  case exists — the same mechanism that grants recognition to a threatening
  new claim can later withdraw it from an existing holder in favor of one.
- Prereqs: DIP-006. Enables: none. See_also: DIP-008.
- Contested: false.
- Confidence: high.

**6. Espionage, Diplomatic Cover, and Persona Non Grata**

- Dates: 1650 to 1985, `century`. Regions: europe, americas.
- Answers: Probe 6 (a distinct mechanism — the same immunity DIP-005
  establishes as protective infrastructure is simultaneously an
  intelligence-gathering loophole, and the expulsion mechanic is the only
  check on it).
- Mechanism: Diplomatic immunity (DIP-005) protects a resident ambassador
  from prosecution under host-country law, which makes an embassy an
  unusually safe base for espionage conducted under diplomatic cover — the
  host state's only real recourse against a diplomat caught spying is
  expulsion (declaring the individual persona non grata) rather than
  prosecution, a symbolic-but-real diplomatic cost that damages the
  relationship without technically breaching the immunity norm itself.
- Hook: A modifier/branch: converts DIP-005's protective immunity into a
  double-edged capability — real intelligence access, at the cost of a
  standing expulsion risk that escalates in direct proportion to how
  overtly the cover is abused.
- Prereqs: DIP-005. Enables: none.
- Contested: false.
- Confidence: high.

**7. Extraterritoriality and Consular Jurisdiction**

- Dates: 1843 to 1943, `century`. Regions: east-asia, middle-east.
- Answers: Probe 6 (a distinct legal-diplomatic mechanism — jurisdiction
  itself becomes a negotiated variable rather than a fixed territorial
  default) and closes a citation DIP-007's own absence note already makes
  in passing without giving it its own mechanic.
- Mechanism: An unequal treaty can carve out consular jurisdiction — a
  foreign power's own citizens, resident within another state's
  territory, remain subject to their own consul's law rather than the host
  state's, converting legal jurisdiction from an automatic consequence of
  physical territory into an explicitly negotiated (or imposed) diplomatic
  variable.
- Hook: A modifier: territorial sovereignty (DIP-006) and legal
  jurisdiction, normally assumed to coincide automatically, can be
  explicitly and asymmetrically decoupled by treaty — a durable source of
  resentment and a frequent target for later renegotiation once the
  power imbalance that produced the original carve-out narrows.
- Prereqs: DIP-006. Enables: none. See_also: DIP-007.
- Contested: false.
- Confidence: high.

## OPTIONAL

**8. Ceremonial Precedence and Ambassadorial Rank Disputes** [REJECTED]

- Dates: 1500 to 1815, `century`. Regions: europe.
- Answers: nothing not already covered — DIP-005's professional-embassy
  infrastructure already establishes the actors; this just adds the status
  game they played once resident. Good history (the 1661 Battle of the
  Ambassadors between French and Spanish envoys' carriages in London,
  fought specifically over processional order, is a real and vivid case),
  thin distinct mechanic.
- Hook: A modifier at best — relative diplomatic rank as its own contested
  currency, mostly flavor on top of DIP-005.
- Prereqs: DIP-005. Enables: none.
- Contested: false. Confidence: medium.

**9. Shuttle Diplomacy and Direct Leader-to-Leader Crisis Contact** [REJECTED]

- Dates: 1963 to 1979, `century`. Regions: europe, middle-east, americas.
- Answers: nothing not already covered — this is DIP-005's
  information-speed logic taken to its telecommunications-era limit (the
  Moscow-Washington hotline installed after the 1962 Cuban Missile Crisis;
  Kissinger's personal shuttle negotiations in the 1973 Arab-Israeli
  aftermath), not a new mechanism.
- Hook: A modifier, mostly flavor: personal, real-time leader contact as
  the fastest possible version of DIP-005's own information-speed stat, with
  no new lever of its own.
- Prereqs: DIP-005. Enables: none.
- Contested: false. Confidence: medium.

═══ PART C — PROPOSED MODIFICATIONS ═══

## SPLIT

**DIP-007 (Imperial Oceanic Diplomacy)** — its own Regional Specifics
section already admits the East Asian case is "a formally different
diplomatic category," and it's right: the entry's Mechanism section is
written entirely around multilateral inter-colonizer coordination (Tordesillas,
the Berlin Conference) where the colonized population is absent from
negotiation _entirely_ — but the East Asian unequal-treaty case it also
covers is a coerced _bilateral_ negotiation where the target government
remains a nominal treaty party under duress. These aren't two flavors of one
mechanism; they're two different negotiating structures (who is and isn't
in the room) currently squeezed into one Mechanism section with the second
case relegated to a Regional Specifics aside where it doesn't structurally
belong. Resulting entries: **DIP-007** retained, scoped to multilateral
inter-colonizer recognition (Tordesillas, Berlin Conference — the colonized
party absent throughout), prereqs unchanged; a new standalone
**Unequal Treaties and Coerced Bilateral Diplomacy** entry (China and Japan's
treaty-port system, extraterritoriality's home case — see also Part B #7),
prereqs [DIP-006], see_also [DIP-007, COM-022].

## MERGE

None found. I specifically checked DIP-009/DIP-011 (both explicitly and
correctly differentiate ad hoc concert coordination from standing
rules-based institutions, with DIP-011's own Hook naming the distinction)
and DIP-008/DIP-010 (both involve a new legitimacy claim seeking
recognition, but one is about a regime's constitutional rupture and the
other about a population's national-identity claim — genuinely different
mechanisms, not a relabeling of the same one). This domain's existing
differentiations are unusually careful for how thematically close several
of its entries sit to each other.

## RE-DOMAIN

- **DIP-001 ↔ DIP-002**: currently completely unlinked despite being
  obviously related — DIP-001 is the legitimacy/status layer of an
  imperial-subordinate relationship, DIP-002 is the physical-projection
  layer that backs it. Add a reciprocal `see_also`.
- **DIP-005 → DIP-006**: permanent-embassy infrastructure and the
  reciprocal-immunity norm it establishes are a real precondition for the
  formally-equal, mutually-recognizing system Westphalia codifies, and
  currently DIP-006's only prereq is REL-013. Add DIP-005 to DIP-006's
  `prereqs`.
- **COM-015 → new entry #3**: once "Non-Sovereign Diplomatic Actors" exists
  (Part B #3), add it to COM-015's `see_also` — closing the gap where
  COM-015 already claims diplomatic relevance with nothing in DIP to point
  to.

## RETIER

**DIP-002 (Roads, Garrisons, and Imperial Unification): core → standard.**
One of the domain's three fully isolated, non-enabling entries; its own
canonical-domain fit is the one genuinely open boundary question in this
audit (Probe 7); and structurally it's a single, narrow mechanism (backing
one type of relationship, DIP-001's) rather than a load-bearing hub — the
same shape this project has retiered before elsewhere (MAR-009, FIN-009,
REL-008, PHI-021).

Borderline, not proposed: **DIP-004** (Gifts, Hostages, and Marriages) bundles
three sub-mechanisms — hostage exchange and marriage-alliance both fit its
stated "personal cost the offending party bears" logic cleanly, but gift
exchange runs on a genuinely different logic (signaling and reciprocity,
not risked collateral) folded into the same paragraph. This is a real,
softer version of the same "meanwhile-in" problem the DIP-007 split fixes,
but the entry's own Regional Specifics section already handles the
resulting regional variation coherently — flagging the doubt rather than
proposing a second split on top of the one already made.

## CUT

None found. I read all twelve entries specifically hunting for a hollow
Hook or a duplicated mechanic and didn't find one — every single Hook in
this domain converts a real cost, constraint, or capability, including the
ones I'm otherwise critical of (DIP-002's legibility conversion is real even
though the entry is isolated and narrow; DIP-009's Berlin-Conference
region_variant and DIP-007's own Berlin-Conference region_variant look at
first glance like they might duplicate each other and turn out, on close
reading, to be deliberately covering the same event from two different,
explicitly cross-referenced angles — recognition-of-claims versus
avoiding-great-power-conflict — which is good practice, not a bug). This
domain's problem is thinness and disconnection, not padding: unusually for
a twelve-entry domain, I did not find one that doesn't earn its place.

═══ PART D — VERDICT ═══

**Worst structural problem:** the domain reads as twelve well-written,
independently-researched topic essays rather than one connected mechanic
system — every entry is `tier: core`, two-thirds of them (8 of 12) have a
completely empty `enables` field, and its own two earliest, most
foundational entries (DIP-001 and DIP-003) are linked by a prereq relationship
that runs 709 years backward.

**Entry count:** 12 before. Proposed: +9 (4 load-bearing, 3 valuable, 2
optional), +1 from the DIP-007 split → **22** entries.

**Proposed out of symmetry rather than mechanical need:** both Optional
entries, named honestly in their own writeups — Ceremonial Precedence and
Shuttle Diplomacy are each a flavor extension of DIP-005's existing
information-speed and status logic rather than a new lever, included for
good, vivid history rather than because the domain's mechanics require
them.

**Does this domain justify separate existence?** Yes, but on thinner margin
than most domains audited this cycle. The LAW/DIP split (internal
obligation vs. inter-polity relation) is cleanly observed, and DIP-008's and
DIP-010's branch mechanics are genuinely strong, original design work. But
this is the first domain this cycle where the honest verdict is that its
problem isn't missing content so much as missing _connective tissue_ — the
individual entries are good; the graph they're supposed to form barely
exists yet. A domain this size, this disconnected, and this uniformly
tiered would be vulnerable to a fold-into-LAW argument if its existing
entries weren't already this well-differentiated from LAW's actual
territory — the identity is real, it just needs building out, not
justifying.
