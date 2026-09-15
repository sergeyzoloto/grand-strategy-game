IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

# Social-Formation Audit — 2026-09-08 [REJECTED]

**Scope:** the composition of human groups — kinship, descent, household form, tribal
and confederative organisation, local self-government — across the full 377-entry
corpus, all 13 domains.
**Method:** read `_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`
(now 8 principles), `_meta/boundary-rules.md` (now includes HLT and LAW/WAR sections),
and `_meta/audits/00-cross-domain-2026-09-08.md` in full; then `grep` the entire corpus
directly for the terms below and read every substantive hit's actual body text, not
just its title or tags. No single-domain audit is relied on for this question, per
instruction — by construction, none of the thirteen could see it, since it cuts across
all of them.

**A note on the hypothesis and a note on the prompt's own claims.** The hypothesis —
no domain owns group composition, and what exists is fragmented as incidental
background inside entries about something else — is **confirmed**, and confirmed more
completely than the hypothesis itself anticipated: 11 of 15 probed clusters are absent
or effectively absent, not merely fragmented. Two specific claims in the prompt itself
were checked directly and **not substantiated** by the current corpus, and are reported
as such rather than assumed: `AGR-130` (the ard/plow entry) contains no reference to
Goody, Alesina, dowry, or brideprice anywhere in its text — the plow-and-gendered-labor
mechanism it actually carries is Boserup's, not Goody's, and the two are genuinely
different arguments. And no entry anywhere in the corpus — under any title, including
`HLT-300` (The Demographic Transitions) — cites the Hajnal line, the Western European
Marriage Pattern, late female marriage, permanent celibacy, neolocal residence, or
life-cycle service as a pre-modern fertility mechanism; `HLT-300` covers only the
industrial-era (1800–2020) transition. Both are noted here so Part C's proposals are
built on what the corpus actually contains, not on what it was believed to contain.

---

## PART 0 — Inventory

Full-corpus `grep` for the specified term list, then direct reading of every entry with
a substantive hit (a bare word-match with zero connection to the entry's own argument —
"guild" appearing once inside a sentence about medieval masons in an entry about fired
brick, "family" meaning "a family of related alloys" — is excluded from the table below
as noise, consistent with the instruction that a passing mention doesn't count as
coverage; the noise was checked, not skipped, and confirmed to be noise in every case).

| Entry                                                                                 | Terms hit                                             | Own Mechanism or incidental | Own Hook on this topic?                          | What it actually covers                                                                                                                                                                                                                                                                                                                     |
| ------------------------------------------------------------------------------------- | ----------------------------------------------------- | --------------------------- | ------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- |
| `LAW-010` Customary Law and Pre-Written Dispute Resolution                            | wergild, feud, lineage, council                       | Own Mechanism               | Yes, but about dispute procedure                 | The compensation/oath/ordeal _procedure_ that stops a feud, using an already-existing bounded kin-group as its unexamined social substrate. Does not model what makes a kin-group bounded or who counts as "in" it.                                                                                                                         |
| `LAW-040` Marriage, Inheritance, and Personal Status Law                              | marriage, inheritance, family                         | Own Mechanism               | Yes, but about jurisdiction                      | Which legal authority (state, millet, colonial category) governs marriage/inheritance, and how that authority is administratively organized. Not the household or descent structure being governed.                                                                                                                                         |
| `LAW-060` Senate and Elite Council as a Coalition Mechanism                           | confederation, council, lineage                       | Own Mechanism               | Yes, but about executive constraint              | Council-as-check-on-a-ruler, using the Haudenosaunee Great Law of Peace and African lineage-elder councils as two of several _examples_ of the same constraint logic. Does not model how a confederation's member composition is decided, scales, or holds together.                                                                        |
| `LAW-120` City Charters and Communal Self-Government                                  | commune, guild                                        | Own Mechanism               | Yes, but about the grant                         | The ruler-to-town charter transaction (self-taxation, own courts, in exchange for revenue/service). Not the internal composition of the resulting commune's own governing council.                                                                                                                                                          |
| `LAW-130` Corporate Forms — University, City, Church, Company as Legal Persons        | clan, lineage, guild                                  | Own Mechanism               | Yes, but explicitly excludes this                | Universitas/waqf as an artificial legal person. Its own `absence` field states outright that Chinese lineage organizations hold collective property "through custom... and family/clan structures rather than through a formal legal category" — naming the exact mechanism this audit is looking for and explicitly declining to cover it. |
| `LAW-210` The Welfare State                                                           | family, kin                                           | Own Mechanism               | Yes, but about fiscal pooling                    | State risk-pooling replacing "a private, family, or charitable matter" — family/kin obligation appears only as the displaced prior system, in one clause, never characterized in its own right.                                                                                                                                             |
| `MIG-110` Steppe Confederations                                                       | confederation                                         | Own Mechanism               | Yes, but explicitly scoped away from this        | Owns the _mobility infrastructure_ precondition only. Its own opening sentence: "The political-military organization of a steppe confederation is WAR's." Checked directly: WAR has no entry that claims it (see Part A.10).                                                                                                                |
| `PHI-240` Ibn Khaldun's Muqaddimah                                                    | asabiyyah                                             | Own Mechanism               | Yes, fully                                       | The one genuine hit for a formation-and-dissolution cycle: group cohesion peaks at conquest and mechanically erodes over 3–4 generations of settled rule. Real, substantive, well-built coverage — see Part A.10 for exactly what it does and doesn't reach.                                                                                |
| `REL-010` Pre-Institutional Religion: Ancestor Cults, Shamanic Practice, Sacred Sites | ancestor cult(s)                                      | Own Mechanism               | Yes, but about religious authority's origin      | Ancestor cult as the _origin case for personal-reputation-based sacred authority_, prior to REL-020's institutionalization. Not the descent group whose ritual expression the cult actually is.                                                                                                                                             |
| `REL-200` Religious Confraternities and Lay Guilds                                    | kinship, clan, guild                                  | Own Mechanism               | Yes, fully                                       | Genuine, well-scoped coverage of _voluntary religious_ non-kin association (confraternity, Sufi tariqa) — real partial coverage of Probe 6. Its own `absence` field names East Asian "kinship and lineage-based organization (clan associations, ancestral halls)" as the alternative mechanism and explicitly doesn't cover it.            |
| `REL-140` Sacred Law and Religious Courts                                             | canon law                                             | Own Mechanism               | Yes, but about the court, not the code's content | Who staffs and legitimates a religious court, and forum-shopping between religious and secular jurisdiction. Never reaches the _substantive content_ of canon law's marriage prohibitions (cousin marriage, adoption, concubinage, widow remarriage) that Goody's thesis is actually about.                                                 |
| `REL-210` Parish Network Governance                                                   | household (registration)                              | Own Mechanism               | Yes, but about record-keeping                    | Parish-as-registry (baptism/marriage/burial records repurposed for state legibility). Not parish-as-governing-body (no vestry, churchwarden, or parish-meeting decision-making anywhere).                                                                                                                                                   |
| `HLT-010` The Forager Baseline                                                        | band (one clause)                                     | Own Mechanism (health)      | Yes, but not about group size                    | Diet/disease/activity baseline before sedentism. "Social units disperse and recombine" appears in one clause of the Social Consequence section; the entry never engages the coordination-cost or leadership-threshold question at all.                                                                                                      |
| `DIP-060` Gifts, Hostages, and Marriages                                              | marriage(s)                                           | Own Mechanism               | Yes, but elite-scoped                            | Royal/dynastic marriage as diplomatic collateral (heqin, Habsburg). Never reaches ordinary village- or lineage-level marriage-as-alliance.                                                                                                                                                                                                  |
| `AGR-140` The Pastoralist–Cultivator Conflict Cycle                                   | (land-use, not a term-list hit but directly relevant) | Own Mechanism               | Yes                                              | Land-use competition between two _already-formed_ social categories. Neither category's own internal composition is this entry's subject.                                                                                                                                                                                                   |
| `PHI-110`, `PHI-400`, `COM-010`, `FIN-010`, `WAR-030`, `LAW-110`, `HLT-230`           | kinship, prestige goods, chiefdom, baojia             | Purely incidental           | No                                               | Kinship as an ethical-philosophy comparison point (Mohism) or a structuralist-method example (Lévi-Strauss cited for _method_, not for alliance theory); "chiefdom" and "prestige goods" each used once as scale-descriptors; baojia covered twice, both times as a _policing/registration_ mechanism, never as village self-governance.    |

Zero hits, checked directly against the full corpus, for: `tribe` (the word itself
never appears anywhere), `exogamy`, `endogamy`, `brideprice`, `age set`,
`ethnogenesis`, `segmentary`, `Nuer`, `Barfield`, `Sahlins`, `Wenskus`, `Hajnal`,
`Goody`, `zadruga`, `joint family`, `stem family`, `panchayat`, `mir`/`obshchina`,
`goningumi`, `Ostrom`, `Allmend`, `Magdeburg`, `futuwwa`, `muhtasib`, `zemstvo`,
`Stolypin`, `big man`, `Traditionskern`, `comitatus`, `druzhina`, `men's house`,
`secret societ[y/ies]`, `charivari`, `youth abbey`, `moiety`/`moieties`,
`cross-cousin`, `fictive kin`, `compadrazgo`, `milk kin[ship]`, `blood brother[hood]`,
`commons`, `manorial`, `jury of presentment`, `amphictyony`, `Latin League`,
`Haudenosaunee`/`Iroquois` outside `LAW-060` and `DIP-020`, `Dunbar`, `fission-fusion`.

**Plain answer to Part 0's direct question: no.** No entry in the corpus has the
formation, scaling, or dissolution of a social group as its own primary mechanism.
`PHI-240` comes closest — asabiyyah genuinely rises and mechanically declines — but its
own Mechanism is a _political-cohesion resource_ for seizing and holding rule, not a
model of kin-group composition, membership, or segmentary scaling logic. Every other
hit is a domain's own established mechanism (dispute procedure, jurisdiction,
executive constraint, legal personhood, fiscal pooling, sacred authority, mobility
infrastructure) using a kin or corporate group as unexamined scenery.

---

## PART A — Probe List

### 1. Band and Scalar Stress — **ABSENT**

`HLT-010` mentions social-unit dispersal in one clause; nothing anywhere cites Johnson's
coordination-cost argument, Dunbar's number, or models why a group crossing a size
threshold needs a decision-maker it didn't need below it.

### 2. Descent and Corporate Kin — **ABSENT as its own mechanism**

The single most load-bearing gap found. `LAW-010` covers the _legal consequence_ of
kin-group liability without covering the kin-group's own boundary; `LAW-130`
_explicitly names and explicitly excludes_ lineage/clan corporate property-holding;
`REL-010` covers ancestor cult only as religious-authority origin. Unilineal vs.
cognatic descent, moieties, and the lineage as a bounded, property-holding, feuding
corporate body are nowhere.

### 3. Alliance and Exogamy — **ABSENT**

`DIP-060` is real but elite-scoped (royal marriage as diplomatic collateral); nothing
covers ordinary cross-cousin marriage, the incest taboo as an alliance-forcing rule,
brideprice vs. dowry, or fictive kinship (blood brotherhood, compadrazgo, adoption,
milk kinship). The prompt's own suggestion that `AGR-130` already carries Goody/Alesina
content was checked and found false — see the note above.

### 4. Household Form — **ABSENT**

Zero hits for zadruga, joint family, stem family, dvor, or ie. `LAW-040` covers legal
_jurisdiction_ over marriage/inheritance, not household composition. `HLT-300` covers
only the 1800–2020 transition. The Hajnal line and the Western European Marriage
Pattern — checked specifically at the prompt's own suggestion — are not cited anywhere
in the corpus under any name.

### 5. The Origin of the Nuclear Household (Goody's thesis) — **ABSENT**

`REL-140` covers canon law _courts_ as an institution and explicitly disclaims the
substantive law's content to `LAW-020`'s Manusmriti-adjacent territory — but neither
entry, nor any other, reaches canon law's actual marriage prohibitions (cousin
marriage, adoption, concubinage, widow remarriage) or their claimed social consequence
(kin-corporation dissolution, bequests redirected to the Church). The Schulz/Henrich
"Marriage and Family Programme" revival is absent along with the underlying Goody
thesis it revives.

### 6. Non-Kin Association — **PARTIALLY COVERED**

`REL-200` is genuine, well-built coverage of one branch: voluntary religious sodality
(confraternity, Sufi tariqa) as a household-and-neighborhood-scale organizing
principle distinct from both kinship and clergy. But age-sets and grades (Maasai,
Nuer), men's houses, initiation and secret societies (Poro, Sande), European youth
abbeys and charivari, and the war-band (comitatus, druzhina) as personal non-kin
loyalty to a leader are all absent — zero hits for every specific term.

### 7. Chiefdom and Cycling — **ABSENT**

"Chiefdom" appears exactly once in the entire corpus, as a scale-descriptor in
`WAR-030` ("a state or chiefdom"); "prestige goods" appears exactly once, describing
cacao-as-currency in `FIN-010`. Big man vs. ascribed chief, redistributive centers,
simple vs. complex chiefdoms, and chiefdom cycling (Earle, Anderson) have no coverage
anywhere.

### 8. Tribe as a Contested Category — **ABSENT, and not miscoded either**

The word "tribe" never appears anywhere in the corpus — checked directly, zero hits.
This means the corpus cannot be flagged for treating tribe as a natural evolutionary
stage, because it never uses the category at all, correctly or incorrectly. Fried's
critique, Wenskus/Pohl's Traditionskern ethnogenesis argument, and the Roman
construction of Germanic tribal identity are all absent as a result — there is no
entry to correct, only a live theoretical debate with no home.

### 9. Segmentary Opposition — **ABSENT**

Zero hits for segmentary, Nuer, Evans-Pritchard, or Sahlins. No entry models coalition
scaling by genealogical distance anywhere in the corpus.

### 10. Confederation — **PARTIALLY COVERED, and the missing piece is the one the corpus itself already promised**

`PHI-240` genuinely covers asabiyyah — the prompt's own contingency ("if not, note
that the single most mechanisable pre-modern theory of political cohesion is absent")
does not apply; it is present and well-built, covering the cyclical rise-and-decline
of a dynastic or tribal coalition's cohesion once it holds power. But this is a
different mechanism from what `MIG-110` itself explicitly promises and never receives:
checked directly against WAR's full 32-entry title list, there is no entry on steppe
decimal organization, Barfield's thesis that confederation forms specifically in
response to an adjacent state worth extracting from, or the comparative
Haudenosaunee/Powhatan/Creek/Greek-amphictyony/Latin-League material. `LAW-060` uses
the Great Law of Peace only as one example among several of "council constrains
executive," not as a case of confederation formation. This is the clearest single
finding in the whole audit: an entry (`MIG-110`) names the exact gap, names the exact
domain meant to fill it (WAR), and that domain — now fully built out to 32 entries —
never did.

### 11. The Village as Corporate Body — **ABSENT**

Zero hits for panchayat, mir/obshchina, or goningumi. Baojia appears twice, both times
as a policing/registration mechanism (`LAW-110`, `HLT-230`), never as village
self-governance. `REL-210` covers parish record-keeping, not parish decision-making.
No entry anywhere mentions a manorial court or jury of presentment. As the prompt
suspected: AGR's entries (confirmed against the domain's own recent audit) assume
communal field-rotation decisions happen without ever naming the institution that
makes them.

### 12. Commons Governance — **ABSENT**

Zero hits for "commons" anywhere in the 377-entry corpus. `AGR-100`'s own irrigation
entry poses Wittfogel's state-hydraulic-despotism thesis against a communal/local
counter-case explicitly in its own Contested section, and cites no entry for that
counter-case because none exists. Confirmed exactly as the prompt suspected.

### 13. Urban Self-Government — **PARTIALLY COVERED, comparative case missing**

`LAW-120` (charters), `COM-110` (Hanseatic League), and `COM-090` (periodic fairs and
the law merchant) give the European/commercial mechanism real, substantive coverage.
But the comparative case the prompt specifically names — Weber's claim that Islamic
cities lacked corporate municipal personality, against the qadi, muhtasib, futuwwa,
and mahalla — is entirely absent. "Qadi" appears only as a court-institution role
(`REL-140`), never as an urban-governance actor; futuwwa, muhtasib, and mahalla have
zero hits. This is exactly the kind of live, well-documented dispute the project's own
conventions prefer, and it's missing entirely rather than resolved either way.

### 14. Aggregation Upward — **PARTIALLY COVERED**

`LAW-140` (Estate Tax Treaties — Cortes, Estates, and Parliaments) genuinely covers
the top of the ladder: assemblies of local corporate bodies aggregating into a
representative body a ruler must negotiate with. But the New England town meeting, the
Russian zemstvo of 1864, Stein-Hardenberg's 1808 Prussian municipal reform, and
subsidiarity as a formal governing principle are all absent — zero hits for zemstvo,
Stolypin, town meeting, or subsidiarity.

### 15. Dissolution — **FRAGMENTED, not owned anywhere**

`AGR-200` (Enclosure) genuinely covers the destruction of one specific corporate
form (open-field customary-use rights). `LAW-210` mentions state-substitutes-for-
kin-obligation in one clause. Conscription, cadastral survey, individual land titling,
the Napoleonic Code, and collectivization each likely exist as entries about something
else (military manpower, state fiscal legibility, agrarian policy) without ever being
framed as _what ends a kin-corporate or communal form specifically_ — consistent with
the reverse-direction gap pattern the Agriculture audit already found independently in
a different context.

---

## PART B — Domain Placement

### Recommendation: (a) NEW DOMAIN — **KIN, "Kinship and Social Formation"**

Not recommended for symmetry with the other twelve. Recommended because the check the
brief itself sets — a mechanism of its own that no existing domain's boundary rule can
honestly claim — comes back positive on direct inspection of all five candidate
domains:

- **LAW**'s own rule is enforceable obligation. Segmentary opposition, band-to-chief
  scaling, and fission-fusion organization have no obligation content at all — they're
  models of _who counts as a coordinating unit_, prior to and independent of whether
  that unit's members owe each other anything enforceable. `LAW-010` and `LAW-130`
  both confirm this in their own text: both use a kin-group as an unexamined premise
  for a genuinely different, correctly-LAW-owned mechanism (dispute procedure, legal
  personhood), and `LAW-130` explicitly names the excluded material as _not_ a legal-
  personhood question.
- **REL**'s own rule is the sacred and its institutions. `REL-010` and `REL-200` both
  confirm the same pattern from the religious side: ancestor cult and confraternity
  are both genuinely REL when the entry's own claim is about _religious authority or
  devotional practice_ — but a descent group's property-holding and liability function
  isn't sacred content merely because it may carry a cult alongside it, any more than a
  chartered company's balance sheet is religious content because its founders happened
  to be pious.
- **DIP**'s own rule is inter-polity relation. A confederation's internal composition —
  which units belong, by what principle, how they scale and fission — is definitionally
  _intra_-group, not inter-polity, which is exactly why `DIP-060` stays at the
  elite/dynastic-marriage level and never reaches ordinary village-level alliance
  marriage: DIP's own boundary rule gives it no honest way to reach further down.
- **WAR**'s own rule is organized violence and doctrine. The war-band's loyalty
  structure (comitatus, druzhina) is a non-kin social bond that predates, outlasts, and
  is orthogonal to any specific campaign — `MIG-110` already tests this exact boundary
  by explicitly handing WAR the confederation's "political-military organization," and
  WAR, now at 32 entries, never claimed it. That's not a hypothetical boundary problem;
  it's a confirmed one.
- **MIG**'s own rule is movement as a process. Ethnogenesis at a frontier and
  confederation formation both often _follow_ a migration, but the group's resulting
  internal composition isn't itself a movement mechanic, which is why `MIG-110`
  correctly limits itself to mobility infrastructure and, again, hands the composition
  question elsewhere.

Every one of the five candidate domains, independently and using its own already-
written boundary logic, declines this material or hands it off. That is the positive
case for a new domain, not an assumed one.

**Proposed boundary-rules.md section:**

> ## KIN / LAW
>
> - **KIN** owns the group and its composition — who belongs, by what principle
>   (descent, marriage, oath, initiation), and how a group scales, splits, and
>   dissolves.
> - **LAW** owns enforceable obligation — once a KIN-defined group exists, what its
>   members legally owe each other and outsiders, and how that obligation is codified
>   and adjudicated.
>
> Collective liability is the clean test: which kin count as "yours" for feud and
> compensation purposes is KIN's; the compensation schedule and dispute procedure that
> actually settles a claim (`LAW-010`) is LAW's.
>
> ## KIN / REL
>
> - **KIN** owns the descent group's composition and its secular corporate functions —
>   property, liability, membership.
> - **REL** owns the sacred and its institutions — ancestor cult as ritual practice,
>   its priesthood, its calendar, stays REL's (`REL-010`'s existing scope is correct
>   and doesn't change).
>
> A sodality organized around shared devotional practice (a confraternity, a Sufi
> tariqa, `REL-200`) stays REL-canonical. A sodality organized around age, initiation,
> or oath to a leader with no devotional content is KIN's.
>
> ## KIN / DIP
>
> - **KIN** owns a confederation's internal composition — which units belong, by what
>   principle, how internal representation and decision-making work, how it scales and
>   fissions.
> - **DIP** owns the confederation's external behavior — its treaties, its recognition
>   by and relations with other polities.
>
> ## KIN / WAR
>
> - **KIN** owns the war-band's loyalty structure — a personal oath to a leader as a
>   non-kin social bond, independent of any specific campaign.
> - **WAR** owns how that band actually fights — doctrine, battlefield organization,
>   military capability.
>
> This mirrors the existing LAW/WAR precedent one level down: the same way a feudal
> land grant (LAW) is separable from the resulting knight's battlefield doctrine (WAR),
> a war-band's loyalty bond (KIN) is separable from its fighting doctrine (WAR).
>
> ## KIN / MIG
>
> - **MIG** owns movement as a process.
> - **KIN** owns the composition of a group that moves, or that forms as a consequence
>   of movement — ethnogenesis at a frontier, a confederation's internal political
>   structure once mobility (MIG's own territory) makes assembling one possible.

**LAW-080 note, not reopening the merge:** the already-merged feudalism entry is
arguably also a fictive-kinship relationship — vassalage's own historical vocabulary
("liege," homage ritual) borrows kinship's language for a non-kin bond. Add `kin` to
`LAW-080`'s `domains_touched` as a light-touch fix; no content moves, no merge
reopens.

---

## PART C — Proposed Entries

Mechanisms, not instances, per instruction — each entry below absorbs multiple probe
clusters and multiple region*variants rather than spawning one entry per ethnographic
case. Several entries below exist specifically because an \_already-written* entry
elsewhere assumes the structure without documenting it — that inbound dependency is
noted for each, per instruction, as the actual argument for writing it.

### LOAD-BEARING (6)

**1. Descent and the Corporate Lineage**

- Regions: middle-east, africa, east-asia, americas — era, roughly -6000 to 1900
- Answers: Probe 2 directly
- Mechanism: a lineage reckoned through one line of descent (patrilineal, matrilineal, or cognatic) functions as a bounded corporate body that can hold property, owe and collect collective liability, and outlive any single member — the load-bearing precondition `LAW-010`'s wergild/feud mechanic, `LAW-130`'s explicitly-excluded clan/lineage property case, and `REL-010`'s ancestor cult all already assume.
- Hook (seed): a gate — establishes "who counts as one liability- and property-holding unit" as a variable other domains' entries can check against, rather than assuming a bounded kin-group already exists.
- Prereqs: none (foundational). Enables: `LAW-010`, `LAW-130` (reciprocal fix — both currently assume this structure without citing anything for it), `REL-010` (reciprocal — ancestor cult as this entry's ritual expression).
- Contested: true — unilineal descent's near-universality in classical anthropological theory (Fortes, Evans-Pritchard) has been substantially revised by later ethnography showing cognatic and situationally-flexible descent reckoning are at least as common; model descent-group boundaries as reckoned by a mix of genealogical rule and practical/political convenience rather than a fixed unilineal default.
- Confidence: medium.

**2. Alliance, Exogamy, and Fictive Kinship**

- Regions: africa, americas, south-asia, east-asia — era, broad/recurring
- Answers: Probe 3 directly, and gives `DIP-060`'s elite marriage-diplomacy mechanic its missing ordinary-scale counterpart
- Mechanism: the incest taboo forces marriage outward, converting a potential rival group into an allied one through the resulting cross-group kinship tie (Lévi-Strauss's alliance theory) — and where a blood tie isn't available, fictive kinship (blood-brotherhood, compadrazgo godparentage, adoption, milk-kinship) manufactures the same alliance obligation deliberately.
- Hook (seed): a modifier/gate — marriage (real or fictive) converts a potential-conflict relationship into an obligated one, at a "cost" (which group's women/resources move which direction, dowry vs. brideprice) that varies by which side is judged to be gaining the alliance's greater value.
- Prereqs: `KIN-1` (Descent). See_also: `DIP-060` (explicit elite-scale contrast), `AGR-130` (correcting the record — this is where the corpus's actual Boserup content lives, distinct from this entry's Goody-adjacent territory).
- Contested: true — whether brideprice vs. dowry systematically tracks plow agriculture and property-holding patterns (the Goody/Boserup-adjacent literature the prompt named) is a live, real dispute in economic anthropology, not a settled mapping.
- Confidence: medium.

**3. Scalar Stress: Band, Segment, or Chief**

- Regions: africa (Nuer, the paradigm segmentary case), middle-east, americas — era, broad
- Answers: Probes 1 and 9 together, as a genuine branch rather than two disconnected entries
- Mechanism: above a coordination threshold (Dunbar's number is the popular anchor; Johnson's coordination-cost argument is the mechanism), face-to-face consensus stops scaling — and a society resolves the resulting stress one of two structurally different ways: segmentary lineage coalition (Evans-Pritchard's Nuer, Sahlins's predatory-expansion reading — "me against my brother, my brother and I against my cousin," scaling coalition size by genealogical distance with no permanent leader) or emergent, eventually hereditary leadership (the path toward Chiefdom, entry 4 below).
- Hook (seed): a branch — the same scalar-stress precondition resolves into a no-permanent-hierarchy segmentary coalition or an emergent-leadership path depending on prior descent-reckoning depth and practical convenience (design principle 1), not player choice.
- Prereqs: `KIN-1` (Descent). Enables: `KIN-4` (Chiefdom, as the alternate branch outcome).
- Contested: true — how mechanistically the ~150 Dunbar threshold actually operates across genuinely different subsistence bases, versus functioning as a loose heuristic later literature over-applied, is disputed; model the threshold as a real but fuzzy zone rather than a hard number.
- Confidence: medium.

**4. Chiefdom, Big Man, and Cyclical Complexity**

- Regions: americas (Mississippian and Pacific Northwest cases), oceania (used here because it's where the mechanism is best-documented, anchored back to the five focus regions via the Americas case), africa — era, broad
- Answers: Probe 7 directly
- Mechanism: an achieved "big man" (Melanesian-type, status built through repeated gift redistribution with no inherited office) and an ascribed chief (a hereditary office controlling a redistributive center and a prestige-goods economy) are two different routes to the same above-band political complexity — and chiefdoms characteristically cycle, rising and collapsing repeatedly across a region without ever consolidating into a state, distinct from both `PHI-240`'s already-state-adjacent dynastic cycle and design principle 4's depleting-pool logic applied to political rather than physical capital.
- Hook (seed): pool + event — prestige-goods accumulation funds a chief's redistributive authority as a resource that must be continuously replenished through raiding, tribute, or long-distance exchange or the chiefdom collapses back to band/segment scale, a genuine boom-bust cycle rather than one-directional state formation.
- Prereqs: `KIN-3` (as the emergent-leadership branch outcome). See_also: `PHI-240` (explicit contrast — pre-state cyclical complexity vs. already-consolidated dynastic cohesion decline).
- Contested: false (the big-man/chief distinction and chiefdom-cycling are well-established, comparatively uncontroversial anthropological findings relative to most of this audit's other proposals).
- Confidence: medium.

**5. The Steppe Confederacy: Decimal Organization and the Extraction Thesis**

- Regions: central-asia-steppe, east-asia — era, roughly -200 to 1400
- Answers: Probe 10's specific missing piece — completes the dependency `MIG-110` itself names and hands to WAR, which does not deliver it
- Mechanism: Barfield's argument that steppe confederation forms specifically as a response to an adjacent state wealthy enough to be worth extracting tribute from — not from internal steppe dynamics alone — organized internally through decimal (units of ten, drawn from Mongol and Xiongnu practice) military-administrative structure that cuts across and supersedes prior kin-based grouping specifically to concentrate command.
- Hook (seed): a branch — confederation forms and holds only while a rich, reachable settled neighbor exists to extract from; remove the neighbor (or its wealth) and the same steppe population reverts to `KIN-3`'s segmentary baseline, matching `MIG-110`'s own already-written "fast-forming, fast-dissolving" cohesion stat with an actual formation mechanism to attach it to.
- Prereqs: `MIG-110` (mobility infrastructure), `KIN-3` (segmentary baseline it supersedes). Enables: nothing further required. See_also: `PHI-240`, `DIP-060` (heqin as the settled empire's own side of the same extraction relationship).
- Contested: true — how much decimal organization specifically (versus other administrative techniques) explains steppe confederations' unusual command-and-control reach is debated among specialists in Inner Asian history; model decimal organization as one well-attested contributing mechanism rather than the sole explanation.
- Confidence: medium.

**6. Household Form and the Western European Marriage Pattern**

- Regions: europe, east-asia (Chinese joint family, zu), south-asia (Indian joint family) — era, roughly 1200–1900
- Answers: Probe 4 directly, and supplies `HLT-300`'s currently-missing pre-1800 prereq
- Mechanism: the Hajnal line marks a real, mapped boundary between two structurally different household-formation regimes — northwestern Europe's late female marriage age, high permanent celibacy rate, neolocal residence (a new couple forms an independent household rather than joining an existing one), and life-cycle service (young unmarried adults work in another household before marrying), against the joint/extended family (Chinese zu, Indian joint family, Balkan zadruga, Russian dvor) where multiple married generations or siblings share one household and property. This is a fertility brake operating through _household formation rules_, prior to and independent of `HLT-300`'s later industrial-era mortality/fertility mechanism.
- Hook (seed): a modifier — the Hajnal-line household-formation rule set caps population growth through delayed and sometimes-foregone marriage entirely, a distinct mechanism from `HLT-300`'s later economic-cost-of-children fertility brake, operating centuries earlier and through a different lever (nuptiality, not fertility-within-marriage).
- Prereqs: `KIN-2` (Alliance/marriage). Enables: `HLT-300` (reciprocal fix — HLT-300 currently has no pre-1800 prereq at all despite opening with "mortality falls first... across this entire domain's earlier entries," implying one should exist).
- Contested: true — this is the single most contested cluster this audit surveyed. Whether the Hajnal-line household pattern is a cause or a consequence of northwestern Europe's later economic divergence (the "Marriage and Family Programme" revival by Schulz and Henrich, with strong downstream psychological claims about individualism) is a live, high-stakes, actively disputed question, distinct from the underlying demographic pattern's description, which is comparatively well-established. Model the demographic pattern (late marriage, high celibacy, neolocality) as well-documented fact, and any claimed downstream psychological or economic-divergence consequence as a separate, far more contested claim layered on top.
- Confidence: medium — high confidence in the demographic pattern itself, low-to-medium in how to model its contested downstream claims responsibly.

### VALUABLE (4)

**7. Age-Sets and the War-Band: Non-Kin Male Sodality**
Regions: africa (Maasai and Nuer age-grade systems), europe (Germanic comitatus, Slavic druzhina). Era, broad. Answers Probe 6's remaining half — `REL-200` already covers the _devotional_ sodality branch; this covers the _age-graded_ and _oath-to-a-leader_ branches specifically excluded from KIN/REL's boundary above. Mechanism: an age-set moves a whole cohort through defined life stages together regardless of individual lineage, and a war-band binds young men to a leader by personal oath and shared plunder rather than by kinship or land grant — both cut _across_ lineage lines rather than extending them, a structurally distinct integration mechanism from Entry 1's descent group. Hook: a gate — age-set/war-band membership supplies political integration and military manpower a purely lineage-based system can't reach past its own kin boundary. Prereqs: `KIN-1`. See_also: `LAW-080` (the war-band's later, land-grant-based feudal successor). Contested: false. Confidence: medium.

**8. The Village as Corporate Body**
Regions: south-asia (panchayat), europe (open-field manorial court, jury of presentment), east-asia (goningumi five-family groups). Era, broad. Answers Probe 11 directly, and gives every AGR entry that assumes communal field-rotation decision-making (checked: none currently name the deciding institution) something to prereq. Mechanism: a village functions as a corporate decision-making body in its own right — allocating common land and open-field strips, enforcing by-laws, adjudicating minor disputes through its own court — independent of and prior to any state administrative apparatus reaching down to that level. Hook: civic — a village-level decision-making capacity existing as its own variable, gating whether communal-rotation agriculture (AGR's own assumed baseline) actually has a body capable of making the rotation decision. Prereqs: `KIN-1`. See_also: `AGR-100`, `AGR-200` (enclosure as this entry's own eventual dissolution case). Contested: false. Confidence: medium.

**9. Commons Governance**
Regions: europe (Swiss Allmend, Valencia's Tribunal de las Aguas), south-asia. Era, broad. Answers Probe 12 directly, and supplies the specific counter-case `AGR-100`'s own Contested section poses against Wittfogel without being able to cite. Mechanism: Ostrom's design principles (clearly defined boundaries, rules matched to local conditions, collective-choice arrangements, monitoring, graduated sanctions) describe how a commons is sustainably self-governed _without_ requiring either full privatization or centralized state control — a third option `AGR-100`'s own hydraulic-despotism-vs-communal branch currently names but has nothing to point to. Hook: branch — a commons persists sustainably when Ostrom's conditions hold and degrades (the "tragedy of the commons" case) when they don't, a conditional rather than an inevitable outcome either way. Prereqs: `KIN-8` (Village as Corporate Body). See_also: `AGR-100`. Contested: true — how universally Ostrom's principles actually predict commons success versus failure, versus being a retrospectively-fitted description of the successful cases specifically, is disputed in the design-principles literature itself. Confidence: medium.

**10. Ethnogenesis at the Frontier**
Regions: europe (Roman construction of Germanic tribal identity), central-asia-steppe. Era, broad. Answers Probe 8 directly. Mechanism: Wenskus and Pohl's Traditionskern argument holds that many "tribal" identities historians once treated as ancient and prior to contact were substantially formed _at_ and _through_ contact with an adjacent state — a small core group's claimed tradition attracting and absorbing a much larger, ethnically heterogeneous following that only becomes "one people" retrospectively — directly extending Fried's broader critique that band-tribe-chiefdom-state is a contact-produced sequence, not a natural evolutionary ladder. Hook: branch/event — a frontier "tribe" is modeled as substantially a product of the adjacent state's own administrative and military pressure, not a pre-existing unit the state merely encountered, inverting the corpus's current implicit (if never stated, since the word "tribe" is never used) default. Prereqs: `KIN-1`, `KIN-3`. See*also: `KIN-5` (the steppe case runs a comparable, though not identical, logic). Contested: true — this \_is* the contested claim; model any specific "tribal" identity's antiquity as a genuinely open, source-dependent question rather than assumed deep-time continuity. Confidence: medium.

### OPTIONAL (2)

**11. The Sworn Commune and Comparative Urban Corporate Personality**
Flavor/comparative entry on Weber's thesis that Islamic cities lacked the corporate municipal personality European sworn communes developed, tested against the qadi, muhtasib (market inspector), futuwwa (urban youth guild), and mahalla (neighborhood unit). Honest flag: `LAW-120`, `COM-110`, and `LAW-130` already substantively cover the European mechanism (charter grant, confederation, corporate legal personhood); this entry's real contribution is the comparative contested case specifically, not a new mechanism.

**12. Aggregation Upward: The Zemstvo and Municipal Reform**
Flavor/instance entry on the New England town meeting, the Russian zemstvo (1864), and Stein-Hardenberg's 1808 Prussian municipal reform. Honest flag: `LAW-140` already covers the general aggregation-of-local-bodies-into-a-representative-assembly mechanism (Cortes, Estates, parliaments); this adds specific 19th-century instances of a mechanism already established rather than a structurally new one.

---

## PART D — Verdict

**Is the hypothesis confirmed?** Yes, and more completely than hypothesized — 11 of 15
probed clusters are effectively absent rather than merely fragmented, and every
fragment that does exist is confirmed, entry by entry, to be incidental background
inside a mechanism the corpus correctly owns for some other reason.

**Recommended placement and rough entry count:** new domain, code `KIN`
("Kinship and Social Formation"), with the five-pair boundary section in Part B. 6
load-bearing + 4 valuable + 2 optional = 12 proposed entries; adopting only the
load-bearing tier still gives the domain a coherent 6-entry core covering descent,
alliance, scalar-stress branching, chiefdom cycling, steppe confederacy, and
household form — comparable in shape to Diplomacy's own starting size, which this
project's cross-domain synthesis already confirmed can justify a domain on its own.

**Single most load-bearing absence:** the steppe confederacy's political-military
organization (Part A.10) — not because it's the largest gap, but because it's the one
the corpus already caught itself on: `MIG-110` names the exact missing mechanism,
names the exact domain that was supposed to supply it, and that domain, now fully
built out, confirmed empty on direct check. Every other finding in this audit required
this session's own search to surface; this one was already flagged, in the corpus's
own words, waiting.

**Proposed for completeness rather than mechanics:** both OPTIONAL entries, named as
such — the Islamic-cities comparative case (11) and the 19th-century municipal-reform
instances (12) each add real, good history to a mechanism (corporate urban personality,
upward aggregation of local bodies) the corpus already substantively owns elsewhere.
Cutting either does not leave a mechanism unrepresented.

**Does this change `design-principles.md`?** Yes, and it should be recorded as a
standing principle, not a one-off note. Principle 8 already states five domains
independently found the same missing rung — household, workshop, crew, squad scale
beneath the institutional level. This audit is a sixth, independent instance of
essentially the same underlying bias, but one level _further_ down and in a different
register: it isn't merely that entries skip household scale, it's that the corpus's
entire vocabulary for describing a bounded group of people — who's in it, how it grows,
how it splits — doesn't exist as a first-class category anywhere, so every domain that
needs one borrows it silently from ordinary language instead of from a modeled
mechanism. Recommend appending a ninth principle along these lines: **group composition
is not scenery.** Any entry that assumes a bounded social unit (a lineage, a
confederation, a village, a guild) as background for its own actual mechanism should be
checkable against an entry that actually models how that unit forms, scales, and
dissolves — the same discipline principle 8 already applies to scale, applied one layer
further down, to the unit of analysis itself.
