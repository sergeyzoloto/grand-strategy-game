IDs renumbered 2026-09-11; see `_meta/id-migrations.md`.

# Philosophy (PHI) Domain Audit — 2026-09-08

Hostile adversarial review. 30 entries read in full (PHI-030 through PHI-410), plus
`_meta/schema.md`, `_meta/conventions.md`, `_meta/design-principles.md`,
`_meta/boundary-rules.md`, and cross-checks against NAT, LAW, REL, HLT, DIP, FIN
for prereq validity and boundary violations.

**The pattern, stated up front:** this domain has one dominant, systematic blind
spot, not a scatter of random gaps. The Middle East is a co-equal focus region by
project mandate, and it appears in this domain's `regions` field only five times
out of thirty entries — and in every single one of those five appearances, it is
the _contrast case_, the _absence note_, or a _region_variant_ borrowed to
illuminate someone else's story (Zeno of Citium's Levantine ancestry inside a
Hellenistic-Greek entry; the Mu'tazila inside a Latin natural-law entry; the
madrasa inside a European-university entry; Tanzimat reform inside a
Weber-secularization entry). Not one of the domain's 30 entries has the Middle
East as its own primary civilizational story. Confirmed independently: three
_other_ domains (NAT-020, NAT-040, HLT-150) already cite Avicenna, Averroes, and
Al-Razi by name as supporting detail in _their_ narratives — meaning this
domain's own gap is visible even from outside it. Everything else below is real,
but this is the throughline.

═══ PART A — DIAGNOSTIC SWEEP ═══

## 1. EMERGENCE GAP — FAIL

The earliest entry is PHI-030 (-800, "Emergence of Schools of Thought"), and its
own Mechanism section states its job explicitly: converting an individual
thinker's insight into a transmissible institution. It never asks what
philosophical activity looked like _before_ there was anything for a school to
organize — proverbial wisdom literature, priestly cosmological speculation,
maxim-collections. PHI-006 already half-argues for the gap itself, in its own
`absence` note: Jaspers's original Axial framework explicitly excludes Egypt and
Mesopotamia despite the Instructions of Ptahhotep predating the proposed axial
window "by well over a millennium" — a boundary-drawing choice the entry flags as
contested but never resolves into its own entry. The domain begins at organized
competitive argument and never shows the unorganized reflection schools arise to
discipline.

## 2. SUBSTRATE GAP — mostly PASS, one real hole

The transmission substrate is unusually well-modeled for a "systematic argument"
domain: PHI-030 explicitly owns disciple-training capacity as the mechanism that
lets a doctrine outlive its founder, and NAT-050 (translation movements) is
correctly cross-referenced as the raw-material supply line into PHI-220
(Scholasticism). This is better substrate coverage than most domains manage.
The one real hole: PHI-220's own Mechanism claims scholasticism converts "the
recovered and translated Aristotelian corpus (NAT-050, NAT-030)" into
disputatio material — but the actual transmission chain ran substantially
_through_ Arabic Peripatetic commentary (Averroes was called "The Commentator"
specifically because Aquinas read Aristotle by way of him), and that link is
present nowhere in the domain. The substrate PHI-220 assumes is real but
mis-specified — it silently skips a step. See Part B, Falsafa.

## 3. SCALE GAP — FAIL at one level

Sorting the 30 entries: civilizational/comparative (PHI-006, PHI-280, PHI-410),
state/institutional (PHI-130, PHI-220, PHI-210), and overwhelmingly
individual-thinker/school (roughly 22 of 30). There is no popular/household/folk
level anywhere — philosophy in this domain is uniformly elite, textual, and
school-produced. This is _partly_ appropriate: REL already owns
institution-and-community (REL-210's parish network, REL-090's religious
education), and how an illiterate villager actually encountered simplified
ethical teaching is arguably REL's job, not PHI's, under the boundary rule (REL
owns practice and community; PHI owns the argument). But even granting that
division, there is a genuine missing rung between "elite school" and "REL's
lay institutions": no entry anywhere models philosophy that never had a school
at all — practical/folk ethical reasoning that competes with, rather than feeds
into, organized argument. PHI-030's own `absence` note for the Yoruba Ifa corpus
gestures at exactly this and then drops it.

## 4. REVERSE-DIRECTION GAP — FAIL, sharply

Of 30 entries, only one comes close to modeling loss rather than growth:
PHI-390, where the verification principle's self-application failure redirects
(not destroys) a tradition. Real, catastrophic capability-loss events that this
domain's own entries directly set up and then never cash in are missing
entirely: the Qin's burning of books and burying of scholars (PHI-130's own
Legalist state is the one that does this, immediately after the point where the
entry's Timing section stops), and the closure of Plato's Academy by Justinian
in 529 CE (ending roughly 900 years of continuous operation — this record's
longest-lived named philosophical institution, terminated by a single
imperial decree, with no entry marking it). Design principle 4 (depleting
pools) has zero instances anywhere in this domain — not "weakly represented,"
_zero_. A domain this old and this rich in named institutions should have at
least one entry where accumulated intellectual authority is actually destroyed,
not merely superseded by the next school.

## 5. PREREQ ORPHANS — no dead links, but a systemic under-linking pattern

No entry points at an uncatalogued ID — every `prereqs`/`enables`/`see_also`
target checked against the live wiki resolves. That's a clean PASS on the
narrow question. But a wider automated sweep (every `DOM-###` string mentioned
in a PHI entry's body, checked against that entry's own linking fields) turns up
thirteen entries that name another entry's ID in prose and never register the
relationship in frontmatter: PHI-030↔REL-060, PHI-100↔PHI-070/PHI-090,
PHI-140↔PHI-006, PHI-006↔PHI-030/PHI-110/PHI-070, PHI-060↔PHI-100/PHI-070,
PHI-190↔PHI-030, PHI-270↔NAT-090, PHI-280↔PHI-160/REL-220, PHI-300↔DIP-100/
PHI-290, PHI-180↔PHI-030/PHI-080/PHI-150/PHI-290. The clearest single instance:
PHI-006's own `contested_note` names five "labeled axial entries" —
PHI-100, PHI-080, PHI-110, PHI-070, PHI-090 — and its `see_also` lists three of
them, silently dropping PHI-110 and PHI-070. (Cross-domain linking, by
contrast, is clean — every PHI↔LAW pair I spot-checked, PHI-140↔LAW-050,
PHI-130↔LAW-020, PHI-160/PHI-270↔LAW-230, PHI-210↔LAW-100, turned out to be
correctly reciprocated via `see_also` on both sides. The under-linking is an
_internal_ PHI problem, not a boundary problem.)

## 6. MECHANISM DIVERSITY — PASS on branch, FAIL on pool

Rough Hook classification across the 30: modifier ≈13, branch ≈8, gate ≈7,
event ≈1 (PHI-390), pool = **0**, civic ≈1-2 (PHI-220's corporate
self-governance, PHI-110's organized defense corps, both partial). The branch
count is a genuine strength — 24 of 30 entries are `contested: true`, and
unusually, most of those contested notes actually cash out as a modeled
conditional rather than a restated dispute (PHI-040's underdetermined corpus
resolved differently by rival schools; PHI-080's li-as-control-vs-li-as-
cultivation split; PHI-380/PHI-400's two-directions-from-one-pressure
structure). This is the most consistently branch-literate domain audited this
cycle. But the total absence of a pool mechanic is a real gap, and it's the
same gap probe 4 identifies from a different angle — closing the
reverse-direction gap with a genuine capability-_destruction_ entry would give
this domain its first pool in the same move.

## 7. BOUNDARY INTEGRITY — PASS on the REL/PHI split in general, one concrete stale duplication

The REL/PHI institution-vs-argument split (per boundary-rules.md) is
well-observed: PHI-060 explicitly defers Daoist religious institution to REL
("not yet written"), PHI-090 cleanly hands the Sangha to REL-060, PHI-190 hands
Shankara's mathas to REL ("not yet written," and it's still true — no such REL
entry exists). But one instance has gone stale: PHI-290 ("Shinto and
Kokutai — the Sacralization of Sovereign Power") says Shinto's "institutional
and ritual practice... is REL's fuller territory, not yet written." REL-260
("State Shinto and the Modern Invention of a National Religion") was written
in this project's most recent REL implementation pass, covering the same
1868–1945 Meiji-to-defeat window and the same underlying phenomenon from the
institutional side — and neither entry cross-references the other. This isn't
a boundary violation (the institution/argument split is exactly right) but it
is a live duplication risk with a factually outdated sentence sitting in
PHI-290's body right now. See Part C.

═══ PART B — PROPOSED ADDITIONS ═══

## LOAD-BEARING

**1. Pre-Axial Wisdom Literature (Egyptian, Mesopotamian, and Vedic-Brahmanic Maxim Traditions)**

- Dates: -2400 to -900, `era`. Regions: africa (Egypt), middle-east
  (Mesopotamia), south-asia (early Vedic/Brahmana material).
- Answers: Probe 1 (Emergence Gap).
- Mechanism: Ethical and cosmological reflection is transmitted through fixed
  maxim-collections curated by an existing priestly or scribal institution,
  not through a competing lineage founded to out-argue rivals — authority
  rests on the corpus's antiquity and the interpreter's institutional standing,
  not on demonstrated dialectical victory, which is precisely the mechanism
  PHI-030's schools displace and compete against once they appear.
- Hook: A gate — establishes pre-competitive, corpus-authority ethical
  reasoning as this domain's actual starting condition, with PHI-030's
  school-formation mechanic explicitly readable as a _response_ to and
  departure from it rather than an unexplained beginning.
- Prereqs: none. Enables: PHI-030.
- Contested: true — how much genuine continuity exists between named
  wisdom-literature maxims and later systematized ethics (Egyptian Ma'at and
  Greek natural-law-adjacent reasoning, in particular) is disputed; model any
  specific claimed influence line as a separate, weaker, contested sub-claim,
  not an established transmission.
- Confidence: medium.

**2. Hebrew Prophetic Ethics — Covenant, Critique of Power, and Social Justice**

- Dates: -750 to -500, `era`. Regions: middle-east.
- Answers: Probe 5 (this is the orphan reference sitting inside PHI-006's own
  core text and contested_note right now, named but uncatalogued) and the
  Middle East regional gap.
- Mechanism: The prophetic tradition (Amos, Isaiah, Jeremiah) argues ethical
  obligation to the poor and the stranger is covenant-binding on the ruler and
  the community alike, and licenses the prophet to publicly indict a king's or
  a people's conduct against that standard regardless of the indicted party's
  political power — a distinct move from Greek civic-virtue argument (PHI-140)
  or Chinese role-based ritual ethics (PHI-080): moral authority here derives
  from claimed direct divine commission, addressed _at_ power rather than
  _through_ participation in it.
- Hook: A gate — supplies the fourth Axial breakthrough PHI-006 already names
  but can't currently point to, and a distinct authority-model (commissioned
  external critique of the ruler, not citizen participation or role-correct
  performance) other entries can build a "prophetic dissent" branch on.
- Prereqs: none. Enables: none currently (terminal, matching PHI-070/PHI-110's
  own pattern of major standalone traditions with no downstream PHI entry yet).
  See_also: REL (no existing REL entry owns this — flag as a REL-side gap,
  out of scope for this audit).
- Contested: true — how much of the prophetic corpus reflects the stated
  pre-exilic dating versus later post-exilic redaction and retrospective
  systematization is a live text-critical dispute; model the ethical content
  as reliably datable to the broad era while treating precise authorship and
  composition dates as uncertain, the same handling PHI-060 already gives the
  Daodejing.
- Confidence: medium.

**3. Falsafa — Islamic Peripatetic Philosophy and the Reason-Revelation Synthesis**

- Dates: 850 to 1198, `era`. Regions: middle-east, south-asia (secondary,
  transmission).
- Answers: Probe 2 (substrate — supplies the actual Aristotle-to-Aquinas
  transmission chain PHI-220 currently elides) and the Middle East regional
  gap directly.
- Mechanism: Al-Farabi, Avicenna, and Averroes build a systematic program
  reconciling Aristotelian metaphysics and logic with revealed monotheism,
  producing original argument (Avicenna's essence/existence distinction,
  Averroes's account of intellect) rather than merely preserving Greek text —
  and Averroes's commentaries specifically become the vehicle through which
  much of the Latin West re-encounters Aristotle, predating and materially
  feeding PHI-220's disputatio culture. Al-Ghazali's _Incoherence of the
  Philosophers_ then attacks the whole falsafa program on causation grounds
  (nothing guarantees a cause must produce its effect; only God's habitual
  will does), provoking Averroes's direct rebuttal, _The Incoherence of the
  Incoherence_ — a live, symmetric, named philosophical rivalry exactly on
  the model PHI-320/PHI-110 already use.
- Hook: A gate and a branch — supplies the actual substrate PHI-220 assumes
  (Aristotelian corpus reaches Latin Europe substantially _through_ this
  tradition's commentary, not around it), and the Ghazali/Averroes causation
  dispute is a clean branch point (occasionalism vs. necessary causation) with
  real downstream stakes for how "natural law" (PHI-160) style
  reason-discovers-order claims get treated in an Islamic theological context.
- Prereqs: NAT-050 (translation movements — the Baghdad-centered Greek-to-
  Arabic translation effort that supplies raw material here predates and
  feeds this). Enables: PHI-220.
- Contested: true — see Mechanism; also whether Averroes's rebuttal
  "succeeds" against Ghazali is itself a live position-dependent judgment
  within the tradition, not a settled outcome — model as an open branch, not
  Averroes's victory.
- Confidence: high.

**4. Historical Materialism (Marx)**

- Dates: 1845 to 1883, `era`. Regions: europe.
- Answers: no single numbered probe cleanly, but this is as close to a hard
  prereq orphan as prose-only citation gets: two _other_ domains already
  presuppose this entry by name without it existing anywhere in the 350-entry
  wiki. FIN-200 ("institutional responses to capitalism") explicitly carves
  out "the systematic theoretical argument against capitalism (Marx and the
  ...)" as something it is _not_ covering, implying it lives elsewhere; PHI-410
  explicitly says the Frankfurt School is "extending Marx's critique of
  commodification directly into the realm of culture" as unexplained
  background. Nobody owns him.
- Mechanism: Historical materialism argues a society's mode of production
  (who controls productive property, and the resulting class relation)
  determines its legal, political, and ideological superstructure, not the
  reverse — Enlightenment natural-rights and consent theory (PHI-270) reads,
  on this account, not as timeless discovered truth but as the ideological
  self-justification a rising bourgeois class produces to legitimate its own
  property claims, converting PHI-270's own claimed universality into
  something this entry treats as a contestable class position rather than
  a neutral finding.
- Hook: A branch and a gate — reframes every prior "systematic argument"
  entry in the domain as potentially ideological superstructure rather than
  disinterested reasoning (a genuinely different, symmetric-rivalry-worthy
  register than anything else here), and is the explicit, currently-missing
  prereq for both PHI-410 and FIN-200.
- Prereqs: PHI-270 (the consent/natural-rights theory this directly argues
  against). Enables: PHI-410. domains_touched: [finance] (FIN-200).
- Contested: true — whether historical materialism is best read as a strict
  determinist claim (base mechanically determines superstructure) or a
  looser "in the last instance" influence claim is disputed within Marxist
  scholarship itself (a dispute later Frankfurt-School critical theory,
  PHI-410, is itself substantially a response to); model the determinism
  strength as the open variable, not a fixed reading.
- Confidence: high.

**5. Suppression and Closure of Philosophical Institutions**

- Dates: -213 to 529, `era`. Regions: east-asia (Qin book-burning),
  mediterranean (closure of the Academy).
- Answers: Probe 4 (reverse-direction) and Probe 6 (pool — this is the
  domain's first genuine depleting-pool mechanic).
- Mechanism: A sufficiently motivated and capable state can destroy
  accumulated intellectual-transmission capacity (PHI-030's own
  "intellectual authority") in a single decree — the Qin's -213 burning of
  non-Legalist texts and execution of scholars (the immediate, unstated
  aftermath of PHI-130's own Legalist state), and Justinian's 529 closure of
  Plato's Academy, ending roughly nine centuries of continuous operation —
  converting a school's accumulated transmission capacity from something
  that erodes gradually through competitive failure (PHI-030's normal
  mechanic) into something that can be zeroed out immediately by an external
  political decision the school itself has no say in.
- Hook: A pool-drain event — `intellectual authority` (PHI-030's stat) is
  not just built and lost through competitive failure but is directly,
  catastrophically destructible by state action, with no guarantee of
  recovery on any timescale the school's own disciples control.
- Prereqs: PHI-030, PHI-130 (region_variant: east-asia). Enables: none.
- Contested: false (both events are well-documented; the Qin case's precise
  scale is debated by historians but the event itself is not, and that
  scale-uncertainty belongs in a `confidence` note rather than a `contested`
  branch).
- Confidence: high.

**6. Ibn Khaldun's Muqaddimah — Cyclical Theory of Civilizational Rise and Decline**

- Dates: 1377, `year` (composition date; the theory's own scope is
  civilizational-longue-durée). Regions: middle-east, africa (Maghreb).
- Answers: Probe 4 (reverse-direction, distinct mechanism from proposal 5 —
  a _theory_ of why civilizations cycle, not an event of capability
  destruction) and the Middle East gap.
- Mechanism: Ibn Khaldun argues asabiyyah (group solidarity/cohesion) is what
  lets a tribal or dynastic group seize and hold power, and that the same
  settled, urbanized comfort that successful rule produces systematically
  erodes the asabiyyah that produced it within roughly three to four
  generations, at which point the group becomes vulnerable to conquest by a
  new, higher-cohesion outside group — a genuinely cyclical model,
  structurally opposed to every other entry in this domain's implicit
  progress-narrative, where decline is not contingent failure but the
  predictable, mechanical consequence of success itself.
- Hook: A branch/pool — models political-cultural cohesion as a resource that
  peaks and then predictably depletes as a direct function of the very
  success (urbanization, luxury, settled administration) that accumulated it,
  giving any state-level entry elsewhere in the wiki a genuine decline
  clock to check against, not just growth.
- Prereqs: none. Enables: none currently (a standalone theoretical framework
  other domains' state-decline entries could retroactively cite).
- Contested: true — whether asabiyyah cyclicality is a serious general
  social-scientific model or a period-bound observation about specific
  Maghrebi dynastic patterns Ibn Khaldun overgeneralized from is disputed;
  model it as a contestable general mechanism available to any qualifying
  state, not a confirmed universal law.
- Confidence: medium.

## VALUABLE

**7. Utilitarianism (Bentham and Mill)**

- Dates: 1789 to 1863, `century`. Regions: europe.
- Answers: Probe 6 (distinct branch partner for PHI-320's Kant) — and closes
  a citation PHI-110's own contested_note already makes and disclaims
  ("comparable to modern Western utilitarianism... anachronistically
  imports categories... foreign to... Benthamite utilitarianism
  specifically") without the disclaimed entry ever existing.
- Mechanism: Bentham's felicific calculus argues right action is whatever
  maximizes aggregate pleasure over pain, quantified and compared directly
  across persons with no principled ceiling on what can be traded off against
  what; Mill's revision (higher versus lower pleasures, the harm principle)
  reintroduces qualitative distinction and individual-liberty side-constraints
  specifically to block the aggregation logic from licensing a tyrannized
  minority's suffering whenever it maximizes majority welfare.
- Hook: A branch — direct, symmetric rivalry with PHI-320's categorical
  imperative (outcome-aggregation vs. universalizable duty regardless of
  outcome) on the same "genuine, load-bearing fork" pattern PHI-320's own
  Hook already uses for its Mohism comparison, now with the actual named
  Western tradition that comparison was reaching for.
- Prereqs: PHI-270 (empiricism's consequence-oriented reasoning). Enables:
  none. See_also: PHI-110, PHI-320.
- Contested: true — the aggregation-versus-rights-side-constraint tension
  between act- and rule-utilitarianism (does maximizing aggregate welfare
  ever license instrumentalizing an individual) is a live, unresolved
  internal dispute, not a settled Millian patch; model act-utilitarian and
  rule-utilitarian resolutions as a branch rather than treating Mill's
  liberty principle as a clean fix.
- Confidence: high.

**8. Nietzschean Genealogy and the Critique of Morality as Power**

- Dates: 1878 to 1900, `century`. Regions: europe.
- Answers: Probe 6 (a genuinely distinct mechanism — unmasking a moral claim
  as a power-relation in disguise, rather than arguing for or against a
  normative position on its own terms, which is what every other ethical
  entry in the domain does) and closes a real substrate gap: PHI-400
  explicitly cites Foucault's genealogical method reaching "a peak influence"
  without ever crediting the method's actual origin.
- Mechanism: Nietzsche's genealogical method asks not whether a moral claim
  is true but what historical power-relation produced it and whose interest
  it serves — his master/slave morality argument reads conventional
  "good/evil" ethics (Christian humility, equality, pity) as originating in
  a historically real inversion, where the powerless recode the powerful's
  own virtues as vices out of resentment (ressentiment) — a method that
  treats every other entry's normative argument (Kant's, the utilitarians',
  natural law's) as a further object of historical-psychological
  investigation rather than a rival first-order claim to evaluate on its
  own terms.
- Hook: A branch — supplies a meta-level "unmask the power interest behind
  this claim" move usable against any other ethical-theory entry in the
  domain, and is the explicit, currently-uncredited method PHI-400's own
  Foucault citation depends on.
- Prereqs: PHI-250, PHI-270 (the Enlightenment rational-universalist target
  Nietzsche's critique is aimed at). Enables: PHI-400.
- Contested: true — whether Nietzschean genealogy is itself self-undermining
  (if all moral claims reduce to power interest, does the genealogical claim
  itself escape that reduction) is a standard, unresolved objection; model
  genealogy as a tool with real explanatory reach whose own foundational
  status remains contested, not a neutral analytic method above the fray.
- Confidence: high.

**9. Zoroastrian Moral Dualism**

- Dates: -1200 to -500, `era` (compositional dating of the Gathas is
  genuinely uncertain and disputed by a wide margin). Regions: middle-east.
- Answers: Probe 6 (a metaphysically distinct mechanism — cosmic good-versus-
  evil dualism with real moral stakes and linear eschatological time, as
  opposed to every other metaphysics in the domain, which is either monist
  (Vedanta), impersonal-process (Daoism), or silent on cosmic structure
  entirely) and the Middle East gap.
- Mechanism: Zoroastrianism argues the cosmos is the real battleground
  between two independent, opposed principles (Ahura Mazda and Angra Mainyu),
  with every individual's free moral choice actually mattering to which side
  accumulates strength toward an eventual, linear, one-time cosmic
  resolution — a structurally different time-model from Vedanta's or
  Buddhism's cyclical framing (PHI-040, PHI-090), and a structurally
  different agency-model from Confucian role-cultivation (PHI-080): moral
  choice here is cosmically consequential in itself, not primarily
  self-cultivating or role-fulfilling.
- Hook: A modifier/branch — linear-eschatological moral dualism as a
  distinct cosmology-of-ethics option, contrasted directly against PHI-040's
  monism and PHI-090's cyclical anatta framework as three genuinely different,
  non-reducible metaphysical starting points for grounding ethical stakes.
- Prereqs: none. Enables: none. See_also: PHI-040, PHI-090.
- Contested: true — the compositional dating of the Gathas (anywhere from
  roughly -1700 to -600 depending on linguistic-dating method) is one of the
  most unsettled chronological disputes in the history-of-religion
  literature; model date_precision as `era` and treat any more specific
  claimed date as contested rather than resolved.
- Confidence: medium — this is the softest VALUABLE call in this set; the
  historical uncertainty is real and the entry rests on comparatively thin
  textual ground relative to the other proposals.

## OPTIONAL

**10. Kierkegaard and the Leap of Faith**

- Dates: 1841 to 1855, `century`. Regions: europe.
- Answers: nothing not already covered — PHI-400 already models the
  individual-subjective-existence-over-system mechanic Kierkegaard would
  instantiate. Proposed mostly for completeness: existentialism's actual
  19th-century founder is currently unmodeled, and PHI-400's own Timing
  section starts cold at Heidegger (1927) with no acknowledgment anything
  existentialist happened before it. Good history, thin distinct mechanic.
- Hook: A gate feeding PHI-400, mostly flavor.
- Prereqs: PHI-320 (the systematic rationalism Kierkegaard reacts against).
  Enables: PHI-400.
- Contested: false. Confidence: high.

**11. Enlightenment Sociability — Salons, Coffeehouses, and the Public Sphere**

- Dates: 1650 to 1800, `century`. Regions: europe.
- Answers: nothing directly — this is social-history texture for where
  PHI-250/PHI-270/PHI-300-era argument actually got debated informally
  outside the school/university, not a new argument or a new mechanism.
  Weak mechanic, genuinely good flavor: names the physical/social venue (the
  salon, the coffeehouse, the periodical) that made Enlightenment argument a
  circulating public conversation rather than a purely written scholarly
  exchange.
- Hook: A modifier at best — mostly connective tissue between existing
  entries, not a lever anything else pulls.
- Prereqs: PHI-250. Enables: none.
- Contested: false. Confidence: medium.

═══ PART C — PROPOSED MODIFICATIONS ═══

## SPLIT

**PHI-150 (Collapse of the Polis and Hellenistic Ethics)** — Cynicism doesn't
belong in this entry's causal story and the entry says so itself: its own
Timing section admits "Cynicism, associated earliest with Diogenes, predates
this window but is absorbed into the same broader Hellenistic ethical
landscape." Diogenes is active in a still-functioning, still-autonomous
Athenian democratic context (c. 412–323 BCE) — Cynicism is not a response to
the polis's political collapse the way Stoicism, Epicureanism, and Skepticism
demonstrably are (all three schools found within decades of Alexander's death
specifically). The entry's own Mechanism section explains all four schools as
relocating the good life "from something requiring favorable external
political circumstance... to something achievable through the individual's
own internal disposition regardless of circumstance" — true of the three
post-collapse schools, not true of Cynicism's actual timeline.
Resulting entries: **PHI-150** retained, scoped to Stoicism/Epicureanism/
Skepticism only (the true polis-collapse-caused schools), prereqs unchanged
(PHI-140); a new standalone **Cynicism and the Rejection of Convention** entry,
prereqs [PHI-030] (school-formation generally, not polis-collapse
specifically), see_also PHI-060 (the wu-wei anti-conventionalism comparison
PHI-060 already draws stays valid and should be reciprocated).

## MERGE

None found. This domain's existing splits are unusually well-judged — PHI-040/
PHI-190 (Upanishadic corpus vs. Shankara's systematic defense of one reading of
it) and PHI-380/HLT-320 (philosophical standing vs. clinical efficacy of
psychoanalysis) are both textbook-correct applications of the
one-entry-one-Mechanism rule, not accidental duplication. I looked for a
Rationalism/Empiricism (PHI-250/PHI-270) or Pragmatism/Positivism
(PHI-370/PHI-390) merge case and didn't find one — in both pairs the rivalry
_is_ the mechanic, and collapsing either pair would delete the branch, not
simplify it.

## RE-DOMAIN

- **PHI-290 ↔ REL-260**: both entries. PHI-290's body still says Shinto's
  institutional and ritual side "is REL's fuller territory, not yet written"
  — it has been written (REL-260, "State Shinto and the Modern Invention of a
  National Religion," same 1868–1945 window, same phenomenon, institutional
  angle). Add REL-260 to PHI-290's `see_also` and PHI-290 to REL-260's
  `see_also`; correct the stale "not yet written" sentence in PHI-290's body.
- **PHI-006's own internal linking**: add PHI-110 and PHI-070 to its
  `see_also` — both are named in its own `contested_note` as two of the five
  "labeled axial entries" and neither is currently linked. (If Part D's
  thread-page conversion is accepted, this becomes moot — a thread page is
  links-only by definition and would include all five automatically.)
- Domain-wide: the thirteen-item under-linking list from Probe 5 is real but
  I'm not proposing thirteen separate fixes here — that's implementation-pass
  work, not an audit finding. Flagging the pattern and the two clearest,
  highest-consequence instances above is the audit's job.

## RETIER

**PHI-310 (Romanticism): core → standard.** Single region (europe only),
`enables: []`, and its entire mechanical contribution is supplying vocabulary
that DIP-160 converts into a recognition claim — the same narrow,
single-region, terminal-node shape this project has retiered before (MAR-250,
FIN-170, REL-170, all core→standard for the identical structural reason: real
and well-documented, but structurally a feeder, not a load-bearing hub).

Borderline, not proposed: **PHI-230 (Renaissance Humanism)** has a similarly
modest Hook ("reshaping which intellectual skillset a court or city actually
seeks out") and `enables: []`, but it's a broader, better-known pivot with
real if uncatalogued downstream civilizational weight (proto-Enlightenment,
vernacular culture) — leaving it core, flagging the doubt rather than acting
on it.

## CUT

**PHI-006 (The Axial Age) — convert to a thread page, don't keep as a PHI
entry.** Its own Mechanism section does no independent mechanical work: it
restates that five _other_ entries (PHI-100, PHI-080, PHI-110, PHI-070,
PHI-090) share no direct transmission link, and its Hook is explicitly a
non-claim — "a pattern worth flagging... without committing to any one of the
proposed causal mechanisms as confirmed." No cost moves, no constraint binds,
no branch resolves anything; it's meta-commentary about other entries'
clustering in time, which is precisely what `boundary-rules.md` defines a
thread page for: "topics that span many entries... get a thread page in
`_meta/threads/` instead of being stitched together via frontmatter... links
only, no prose duplicating what's already in the linked entries." Converting
it also resolves Probe 5's clearest internal-linking gap for free — a thread
page just lists PHI-100/004/009/011/012 (and, if Part B #2 is accepted, the
new Hebrew-prophetic entry) with no reciprocity obligation. This is the one
clear cut in the domain; I did not find a second entry with no mechanic or a
duplicated one — everything else genuinely does something.

═══ PART D — VERDICT ═══

**Worst structural problem:** the Middle East is present in this domain only
as other civilizations' supporting cast — never the subject of its own core
entry — despite being a co-equal focus region by explicit project mandate, and
the gap is bad enough that three _other_ domains have already independently
cited Avicenna, Averroes, and Al-Razi as someone else's footnote.

**Entry count:** 30 before. Proposed: +11 (6 load-bearing, 3 valuable, 2
optional), −1 if the PHI-006 thread-page conversion is accepted → **40**
PHI entries (41 if PHI-006 is kept as a standard entry instead of converted).

**Proposed out of symmetry rather than mechanical need:** Kierkegaard
(Optional #10) — named honestly in its own writeup. PHI-400 already covers
the mechanic he'd instantiate; he's there because existentialism's founder
being entirely absent reads like an oversight once you notice it, not because
the domain needs a new lever. Zoroastrian Dualism (Valuable #9) is the softest
non-optional call — real distinct mechanism, but resting on genuinely
disputed dating and thinner textual ground than the other eight proposals.

**Does this domain justify separate existence?** Yes, clearly. The REL/PHI
split (institution/practice vs. systematic argument) is the best-observed
boundary rule of any domain audited this cycle — PHI-060, PHI-090, and PHI-190
all correctly defer institutional content to REL rather than duplicating it,
and PHI-290/REL-260's overlap is a linking failure, not a boundary failure
(both entries are canonically where they should be). This domain has a real,
distinct methodological identity — argument as argument, independent of its
institutional carrier — that no other domain in this wiki attempts to own, and
30 entries deep it is still finding load-bearing gaps rather than running out
of things to say.
