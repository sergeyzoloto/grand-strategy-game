# Boundary Rules

## Multi-domain

Every entry has exactly one canonical `domain` — it decides the file's folder
and its ID prefix, permanently. Nothing about that is up for negotiation per
entry.

Relevance to other domains is handled separately, by `domains_touched`: any
number of domain names, for domains this entry matters to without owning it.
Content is **never duplicated across domains** — `domains_touched` is a pointer,
not a second copy of the entry living under another domain's roof.

If an entry seems to genuinely need two `## Mechanism` sections — one COM read,
one FIN read, say, each substantial in its own right — that's not a
multi-domain entry, it's **two entries**. Split it, give each half its own
canonical domain and ID, and link them with `prereqs`/`enables` (or `see_also`
if the link is non-directional). One entry, one Mechanism, one owning domain is
the rule; a topic that resists that is telling you it's actually two.

Topics that span many entries — not just two — get a thread page in
`_meta/threads/` instead of being stitched together via frontmatter. A thread
page contains **links only**: no prose duplicating what's already in the linked
entries' Mechanism/Social consequence/Hook sections. Its job is to be the map,
not another copy of the territory.

These are the domain pairs that most often tempt a double-home for a single
entry — useful for deciding canonical `domain` before deciding whether
`domains_touched`, a split, or a thread page is the right way to handle the
rest.

## COM / FIN

- **COM (commerce)** owns goods in motion and the institutions that move them:
  trade routes, merchant networks, market integration, logistics.
- **FIN (finance)** owns value abstracted across time: credit, banking, fiscal
  machinery, money itself.

A trade route is COM. The bank that finances the trade route is FIN. Silver as a
commodity flowing through a market is COM; silver as the base of a monetary
system is FIN — an entry can legitimately be canonically COM for its market
effects while carrying `finance` in `domains_touched` for its monetary ones (see
`COM-011`). If the FIN read grows a Mechanism section of its own, split it out.

## WAR / ENG

- **ENG (engineering)** owns technique — the artifact, the method of making it.
- **WAR (war)** owns doctrine and its social organization — how the technique
  gets used, organized, and who it puts in charge.

Gunpowder metallurgy is ENG. The infantry doctrine and standing-army financing it
enables is WAR.

## NAT / ENG

- **NAT (natural sciences)** owns the knowledge-generating method and institution — how
  a society comes to know things, and the institutions that produce that
  knowledge.
- **ENG (engineering)** owns applied capability — turning known principles into
  working artifacts and processes.

The scientific method / experimental societies are NAT. The steam engine built
on that knowledge is ENG.

## REL / PHI

- **REL (religion)** owns institution, practice, community, and endowment — the
  church, the temple, the ritual calendar, the property it holds.
- **PHI (philosophy)** owns systematic argument — the doctrine as a structure of
  reasoning, independent of its institutional carrier.

Monasticism as a landholding, labor-organizing institution is REL. Scholastic
argument about universals is PHI.

## HLT / (AGR, ENG, REL, PHI)

**HLT (health)** owns the body, disease, and their institutional and habitual
management as its own read — not the underlying activity that produces a
health effect as a side consequence:

- Production and diet content are AGR's; the body's and population's health
  consequence of that diet is HLT's. A crop's yield is AGR; the stature and
  dental penalty a cereal-dependent diet writes into a population's skeleton
  is HLT.
- The physical engineering of infrastructure is ENG's; what that
  infrastructure does to a settlement's disease dynamics, and what its
  absence or collapse does, is HLT's.
- A practice's institution, endowment, and theological rationale are REL's;
  its disease-vector or body-management function — who may touch a corpse
  and how fast, what may be eaten and by whom — is HLT's, cross-referencing
  REL for the fuller institutional and doctrinal account rather than
  duplicating it.
- A systematic theory of the body, mind, or disease causation is canonically
  HLT when the entry's own claim is about the theory's institutional
  authority and its (in)dependence from actual efficacy; PHI is the closer
  fit only where the entry's own claim is about the argument's structure
  independent of any institutional or clinical carrier at all.

Ritual burial's institution and theology are REL; the same practice's
corpse-handling disease-management function is HLT. Aqueduct and sewer
construction technique is ENG; the resulting settlement mortality rate,
and its decay when maintenance lapses, is HLT.

## LAW / WAR

- **LAW (law)** owns the land-tenure and inheritance bond itself — who
  holds land under what obligation, how that obligation is legally
  codified and passed down, and how it compares across jurisdictions as an
  institutional form.
- **WAR (war)** owns the resulting military capability and doctrine —
  how an army raised this way fights, how its social culture (honor
  codes, hereditary warrior class identity) differs from a salaried
  force, and how that capability compares to alternative manpower
  models.

A feudal land grant in exchange for service is LAW. The knight or samurai
class's own battlefield doctrine and honor-bound military culture built
on top of that grant is WAR. Where an entry's own comparative material
(which regions, which specific institutions, which historical dates) is
identical to another entry's under this split, that's a sign the two
should be one entry with two `domains_touched`, not two independently-
written entries that happen to agree.

## LAW / DIP

- **LAW (law)** owns internal enforceable obligation — contracts, property,
  courts, within a polity.
- **DIP (diplomacy)** owns inter-polity relation — treaties, alliances,
  recognition, envoys.

A treaty is always DIP, even though it's technically a legal instrument — the
inter-polity axis wins.

## MIG / all

**MIG (migration)** owns movement as a process with its own mechanics: what
triggers it, how it propagates, what it costs, what it carries. A _specific_
migration event that's mostly interesting for its effect on one domain stays in
that domain (e.g. a labor migration that mainly matters for its commercial
effect stays COM) and cross-references MIG for the generic movement mechanic it
instantiates.

## MAR / (WAR, ENG, COM, FIN)

**MAR (maritime)** owns the sea as a theatre with its own mechanics, the same
way MIG owns movement — carved out of whichever domain would otherwise claim
it, specifically because the maritime instance is numerous and coherent enough
to deserve one home rather than being scattered across four:

- Naval warfare — warship classes, naval doctrine and strategy, naval
  institutions (Admiralty, Marines) — is MAR, not WAR. WAR keeps land and air
  warfare doctrine.
- Navigation and shipbuilding technique (celestial/ocean navigation, the
  chronometer, sail rig) is MAR, not ENG. ENG keeps land-based and general
  technique.
- Maritime trade infrastructure and practice (ports, shipping, sea lanes,
  containerization) is MAR, not COM. COM keeps land-based and general
  commerce.
- Maritime-specific financial instruments (marine insurance) are MAR, not
  FIN. FIN keeps general financial instruments.

This is a deliberate exception to the WAR/ENG, SCI-adjacent, and COM/FIN
splits above: a warship's doctrine and its hull technique both live in the
same MAR entry rather than being pulled apart the way gunpowder metallurgy
(ENG) and infantry doctrine (WAR) are, because on land those are two
substantial, independently-developing stories and at sea, for this project's
purposes, they aren't. `domains_touched` still points to WAR/ENG/COM/FIN
where a specific entry's non-maritime angle is substantial (a war fought
partly at sea touches WAR; a chronometer's horology touches ENG), but doesn't
require a split the way a genuinely two-Mechanism entry would.
