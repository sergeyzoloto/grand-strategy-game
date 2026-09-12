# Telecommunications Chain — Coverage Check

**Date:** 2026-09-12
**Scope:** the chain running from electromagnetic theory through telegraph,
telephone, wireless, broadcast, radar, spectrum law, and radio navigation.
**Method:** corpus-wide grep for `radio`, `wireless`, `broadcast`, `spectrum`,
`Marconi`, `Hertz`, `Maxwell`, `radar`, `telephone`, `television`, `satellite`,
`GPS`, plus a date-banded sweep of every entry with `date_start` in 1840-1945.
**No entries written.** Proposals only.

---

## Headline

The chain is two entries long and then stops.

`NAT-170` (Electromagnetism) and `ENG-230` (The Telegraph) exist. **Nothing
between the telegraph and `ENG-330` Semiconductors (1947) covers any part of
electrical communication.** Every item on the check list below is absent —
telephone, wireless telegraphy, broadcast radio, radar, spectrum regulation,
radio navigation, satellite positioning, television. Eight gaps, zero partial
coverage, in a 110-year window the corpus otherwise documents densely (78
entries have `date_start` between 1840 and 1945).

This is not a thin patch. It is a structural hole with a specific shape: the
corpus documents **the wire** and then skips to **the chip**, omitting the
entire wireless era in between — which is precisely the era in which
communication stops being infrastructure a state builds and starts being a
medium a state must contest.

### The hole is already load-bearing

Four existing entries depend on technologies no entry documents:

| Entry | Depends on | Status |
| --- | --- | --- |
| `WAR-310` Mechanization and Combined Arms | radio as the coordinating comms layer — named explicitly in the body | undocumented |
| `WAR-280` Psychological Operations and Propaganda | radio as mass media | undocumented |
| `PHI-410` Mass Culture and the Culture Industry | radio as a culture-industry channel, named twice | undocumented |
| `MAR-340` Convoy Escort | radar as a cause of the 1943 turn in the Atlantic | undocumented |

This is the project's recurring failure mode (citation-gap, `conventions.md`
§3.3) in its strongest form. The usual case is an entry naming an ID in prose
without carrying it in a link field. Here the referenced mechanism **has no ID
to carry** — four entries rest on a prerequisite that does not exist.

`NAT-240` (Information Theory, 1948) compounds it from the other end. Shannon's
channel-capacity theorem came out of telegraph and telephone transmission
engineering at Bell Labs, and `NAT-240` carries no link to `ENG-230` or to
anything else in the chain it abstracts. Its `see_also` reaches sideways to
`NAT-060`, `NAT-180`, `NAT-210`, `NAT-220` and `ENG-330` — every direction
except the one it actually came from.

---

## Item-by-item

### 1. Electromagnetic theory (NAT) — EXISTS, with one gap

`NAT-170` Electromagnetism, 1820-1873, `prereqs: [NAT-040]`,
`enables: [ENG-230, ENG-300, NAT-220]`. Confirmed contents: Oersted's 1820
current-and-compass discovery, Faraday's 1830s induction work, Maxwell's
equations 1861-1865 published in final form 1873. It explicitly states the
wave prediction — "implying that electromagnetic waves could carry information
or energy across distance without a physical wire, a prediction later confirmed
and exploited by radio technology."

**Gap: the confirmation is not in the corpus.** Hertz generated and detected
electromagnetic waves in 1886-1888 — the experiment that turned a mathematical
prediction into a physical fact and made Marconi's work conceivable. `NAT-170`
ends in 1873, thirteen years short of it. `Hertz` returns zero hits corpus-wide.

**Recommendation — the cheap fix, not a new entry.** Extend `NAT-170`'s
`date_end` to 1888 and add Hertz to its `## Timing` and `## Mechanism`. A
separate entry is not warranted: the experiment has no independent mechanism,
it closes `NAT-170`'s own loop. The one argument for splitting it out is
`NAT-170`'s theory-before-practice Hook, which Hertz sharpens — but that is a
sentence, not an entry. If a separate entry is preferred anyway, **`NAT-205`**
is free (between `NAT-200` 1869 and `NAT-210` 1900).

### 2. Telephone (ENG) — ABSENT

Zero hits for `telephone` anywhere in the corpus.

Distinct from the telegraph on exactly the axis the brief names, and the
distinction is mechanical rather than technical. The telegraph is a
**trained-operator relay**: a message is composed, handed to a specialist,
encoded, transmitted, decoded by a second specialist, and delivered — which
means every telegram passes through at least two people who are not the sender
or the recipient, and the network's capacity is bounded by the supply of
trained operators. The telephone removes the intermediary and the training
requirement together. Consequences that do not follow from `ENG-230`:
conversation rather than dispatch (negotiation and interrogative
back-and-forth, not one-way notification); no written record by default, which
matters for both commercial contracting and for what a state can later
reconstruct; the switchboard as a labour institution and one of the first
large-scale feminised clerical workforces; and the shift of the network's
terminal point from the telegraph office to the premises, which is what makes
the firm rather than the city the unit of connection.

**Proposed ID: `ENG-295`** (1876-1915). Sorts after `ENG-290` Internal
Combustion Engine (1876..1913) on the `date_end` tiebreak and before `ENG-300`
Electrification (1882). Slots `ENG-291`-`ENG-299` are free.

### 3. Wireless telegraphy (ENG/MAR) — ABSENT

Zero hits for `wireless` or `Marconi`.

Marconi's 1895 first transmissions through to the 1901 transatlantic signal;
ship-to-shore as the first application with no wired alternative at all;
*Titanic* 1912 as the demonstrating disaster — and specifically the fact that
the nearby *Californian*'s operator was off duty, which is the detail that
produced the regulation; the 1914 SOLAS convention's continuous-watch
requirement as the resulting institution.

This is the entry that carries the **wire constraint being broken** — and it
matters that it is first broken at sea, because the sea is the one surface
where wire was never an option and where the marginal value of the first
wireless link is therefore infinite rather than incremental.

**Proposed ID: `ENG-315`** (1895-1914). Between `ENG-310` Steel-Frame
Construction (1885) and `ENG-320` Agrochemicals (1909). Free.

**Boundary call — ENG, not MAR, but it is genuinely close.** `boundary-rules.md`
gives MAR "navigation and shipbuilding technique" and the sea as a theatre;
wireless telegraphy is neither navigation nor shipbuilding, it is a general
communications technique whose first killer application happened to be
maritime. ENG owns the technique. **`domains_touched: [maritime, law,
diplomacy, war]`**, with maritime doing heavy work. The test for revisiting
this: if the SOLAS distress-watch regime — continuous listening watch,
distress-frequency silence periods, operator licensing — grows a `## Mechanism`
of its own about maritime safety regulation as an institution, that half splits
out as a MAR entry and this one keeps the technology. **`MAR-335`** is free
(between `MAR-330` Dreadnought 1906 and `MAR-340` Convoy Escort 1917) if that
happens.

### 4. Broadcast radio (ENG) — ABSENT. Full proposal below.

### 5. Radar (WAR/ENG) — ABSENT

Two `radar` hits, neither real: `MAR-340` uses it as an undocumented cause,
and `FIN-020` says "off a ruler's radar" as a metaphor.

**Proposed ID: `WAR-315`** (1935-1945), between `WAR-310` Mechanization (1917)
and `WAR-320` Special Forces (1940). Free.

**Boundary call — WAR, not ENG.** `boundary-rules.md` is explicit: ENG owns
technique, WAR owns doctrine and its social organisation, and "gunpowder
metallurgy is ENG, the infantry doctrine it enables is WAR." Radar splits the
same way, and the half worth writing first is the doctrine. The mechanism is
not the cavity magnetron; it is that **detection inverts the defender's
problem**. Without early warning a defender must cover area, holding standing
patrols everywhere an attack might arrive, and cost scales with the perimeter.
With it the defender covers time instead, holding aircraft on the ground and
launching against a known track — the Dowding system, which is the entry's
core, is a command-and-reporting architecture with radar as one input, and it
is the architecture rather than the sensor that wins. This also makes radar a
clean instance of design principle 1: the same sensor produces a decisive
advantage or none at all depending on whether a reporting and interception
institution was built beforehand.

Write the ENG artifact entry separately only if the technique grows its own
Mechanism (magnetron power output, wavelength versus resolution, the
airborne-set weight constraint). **`ENG-326`** is free if so.

### 6. Spectrum as regulated property (LAW/DIP) — ABSENT

Zero substantive hits (`spectrum` appears in `HLT-020` and `ENG-310` in
unrelated senses).

**Proposed ID: `LAW-225`** (1906-1934), between `LAW-220` Universal Suffrage
(1893) and `LAW-230` Human Rights Protection (1948). Free.

**Boundary call — LAW, not DIP.** `boundary-rules.md` says a treaty is always
DIP and the inter-polity axis wins, which points at the 1906 Berlin and 1927
Washington radiotelegraph conventions. But the entry's own claim is not about
inter-polity relation; it is that a **new legal object** gets manufactured —
excludable property rights in a physically non-excludable medium. Nothing about
the electromagnetic spectrum is rivalrous in the way land or water is; two
transmitters on one frequency do not divide a resource, they destroy each
other's signal, which means the property right is created to solve
interference, not scarcity of a thing. That is a LAW mechanism, and it is a
genuinely novel one rather than an extension of land tenure or chattel
property, which is the reason it earns its own entry rather than a paragraph in
an existing property entry. `domains_touched: [diplomacy, engineering,
commerce, war]`. The ITU coordination story goes in `region_variants` and
`domains_touched` unless it grows its own Mechanism, at which point **`DIP-185`**
is free (between `DIP-180` 1920 and `DIP-190` 1935).

### 7. Radio navigation forward to GPS (MAR/ENG) — ABSENT

Zero hits for `satellite` or `GPS`.

Two entries, not one, because the mechanisms differ:

- **Radio navigation** — direction finding, then hyperbolic systems (Decca,
  LORAN). Mechanism: position fixing stops depending on seeing the sky, which
  removes the weather and daylight constraint that celestial navigation could
  never remove, and shifts the skill from the navigator to the shore
  infrastructure. **Canonically MAR** — `boundary-rules.md` assigns navigation
  technique to MAR explicitly. **Proposed `MAR-335`** (1910-1970), free —
  *but note this collides with the fallback slot in item 3; only one of the two
  can take it, and radio navigation has the better claim.*
- **Satellite positioning / GPS** — **canonically ENG, not MAR**, and this is
  the boundary call worth arguing. MAR's carve-out exists because maritime
  instances are "numerous and coherent enough to deserve one home." GPS is the
  opposite: it is position fixing for ships, aircraft, vehicles, artillery,
  container logistics, and eventually individuals, and putting it in MAR would
  make the sea the owner of a capability whose maritime share is small. The
  mechanism is also different in kind from radio navigation — a single
  operator, initially military, provides a global public good with a
  selectively degradable civilian signal, which makes precision a lever one
  state holds over everyone else's economy. **Proposed `ENG-340`**
  (1973-2000), free above `ENG-330` Semiconductors (1947).

---

## Broadcast radio — full proposal

**Proposed ID: `ENG-325`** (1920-1970). Between `ENG-320` Agrochemicals (1909)
and `ENG-330` Semiconductors (1947). Free.
**`domains_touched: [war, diplomacy, philosophy, law, religion]`**

**This is not a variant of the telegraph and must not be folded into a general
communications entry.** Telegraph and telephone are both point-to-point,
addressed, and private; broadcast is one-to-many, unaddressed, and public. The
economics invert — a telegraph's cost scales with the distance wired and its
revenue with messages sent, while a broadcast's marginal cost per additional
listener is zero and its revenue has no natural relationship to reach at all,
which is why broadcasting has to be funded by something other than usage
(licence fee, advertising, or the state) and why the funding model determines
the institution. A single entry covering "communications" would have to carry
both mechanisms and would fail the one-entry-one-Mechanism rule outright.

### Mechanism 1 — the wire constraint is removed

`ENG-230`'s corrected `absence` field now establishes the constraint precisely:
a network built from continuous physical line cannot reach anything it cannot
be strung across, and the cost that binds is not distance but holding an
unbroken line through ground the builder does not continuously control. Radio
deletes that term. **Reach becomes a function of transmitter power and terrain,
not of capital laid across ground** — and, critically, reach no longer requires
*owning or controlling the intervening territory*. One transmitter covers
everything within its footprint whether or not the operator holds a single mile
of the ground between.

This is what makes radio the first communications technology that is cheap for
the poor and the peripheral rather than the rich and the central. Wiring a
territory is an imperial-scale capital project; a transmitter is a purchase.
The asymmetry that `ENG-230` encodes — network geography recording who built it
and why — does not survive the transition.

### Mechanism 2 — direct channel, no literacy, simultaneous arrival

One-to-many with **no literacy requirement** and **simultaneous arrival**,
which together give a ruler a direct channel to an entire population and
bypass the intermediaries print empowered — editors, local notables, clergy,
schoolteachers. Print requires a reader and therefore a school system, and it
requires a distributor and therefore someone local who decides what circulates
and frames it on arrival. Radio needs neither. The population hears the ruler's
actual voice, at the same moment, unmediated and unedited.

The mechanic is the collapse of an intermediary layer that every prior mass
medium *created*. A state that could previously reach its population only
through notables who could bargain over the message now reaches past them —
which strengthens the centre against provincial elites in exactly the way
`ENG-230`'s Ottoman variant describes for administration, but applied to the
population rather than the bureaucracy.

**State-subsidised receivers with deliberately limited range are the clearest
instance** and should be modelled explicitly: the receiver is subsidised
because the state wants the channel open, and its range is limited because the
state wants the channel exclusive. Those are two separate design decisions and
the entry should keep them separate — subsidy raises `broadcast reach`,
range-limiting defends `information sovereignty`.

### Mechanism 3 — broadcasts cross borders without transit

A signal does not pass through a border post, and no transit permission is
sought or needed. **Information sovereignty becomes contestable from outside**
for the first time — a state can control every printing press, school, and
telegraph office inside its territory and still lose the argument to a
transmitter it cannot reach. Jamming and external broadcasting both become
instruments of statecraft, and jamming is the more interesting mechanic of the
two because it is a pure standing cost with no positive output: the jammer buys
nothing but denial, must keep paying for it continuously, and pays more as the
adversary adds transmitters. A textbook depleting-pool inversion (design
principle 4) — the expenditure that merely holds position.

### Mechanism 4 — spectrum as a new legal object

Covered by the `LAW-225` proposal above; `ENG-325` should `enables` it and
carry `law` in `domains_touched` rather than duplicating the argument.

### Regional coverage

All five focus regions carry a distinct mechanism, and **the strongest cases for
mechanisms 2 and 3 are outside Europe and the Americas.** Written as proposed
the entry runs roughly a third non-Western at minimum, and the two examples
doing the most analytical work — the Japanese surrender broadcast and Fanon on
Algeria — are both non-European.

- **East Asia — the purest case of mechanism 2 anywhere.** The
  *Gyokuon-hōsō* of 15 August 1945: the first time most Japanese had ever heard
  the emperor's voice, announcing surrender directly to the population,
  bypassing an army that had spent the previous days attempting to prevent
  exactly that transmission. Ruler-to-population with every intermediary
  removed, at the one moment when every intermediary would have blocked it.
  NHK from 1926 supplies the institutional build-up. Paired with this, Chinese
  wired-loudspeaker networks (*youxian guangbo*) are the **inversion worth
  modelling**: a state that deliberately re-imposes the wire, accepting the
  cost and the coverage limit, precisely because a wired loudspeaker cannot
  receive anything from outside. Mechanism 1 traded away to buy mechanism 3.
- **Middle East — the definitive case of mechanism 3.** *Sawt al-Arab* (Voice
  of the Arabs) from Cairo, 1953 onward, broadcasting pan-Arab nationalism in
  Arabic to populations over the heads of their own governments across the
  whole region, and implicated directly in the politics of states Egypt had no
  other means of reaching. No European external-broadcasting case demonstrates
  contested information sovereignty as cleanly, because none of them had a
  shared language community spanning that many polities.
- **Africa — the case that shows the mechanism running from below.** Fanon's
  *This Is the Voice of Algeria* (1959) on Radio Algérie and the FLN's Voice of
  Fighting Algeria: Algerians bought receivers *en masse* specifically to hear
  broadcasts the French authorities jammed, and the jamming itself became
  evidence the broadcast mattered. Radio as an instrument of anticolonial
  mobilisation rather than of state control, which is the necessary
  counterweight to the state-subsidised-receiver case and prevents the entry
  reading as a one-directional propaganda mechanic.
- **South Asia — the case that shows why literacy independence is decisive.**
  The Indian State Broadcasting Service (1930) and All India Radio in a society
  where print reached a small literate minority and radio reached anyone within
  earshot of one set — the gap between the two audiences is larger here than
  anywhere in Europe, which is the whole point. Congress Radio's underground
  transmissions during Quit India (1942) supply the from-below variant, and
  post-1947 AIR supplies language standardisation as nation-building.
- **Europe — the subsidised-limited-range case, and the institutional
  contrast.** The *Volksempfänger* VE301 (1933), priced to be bought and
  engineered to struggle with foreign stations, is the clearest single instance
  of mechanism 2's design logic. Against it, the BBC's licence-fee
  public-service model shows the same technology under a funding structure that
  produces a different institution — worth carrying because it demonstrates the
  funding model, not the technology, determines the political result.
- **Americas — the commercial model and the spectrum precedent.** US
  advertising-funded broadcasting as the third funding structure, with the
  Radio Act of 1927 and the FCC (1934) as the concrete origin of the `LAW-225`
  object. Fireside chats (1933) for mechanism 2 in a democratic register, and
  Perón's use of radio in Argentina for the populist one.

### Absence

At least: **central-asia-steppe and interior regions generally** — the mirror
of `ENG-230`'s corrected absence, and the load-bearing half. Radio reaches
places wire never did, so the absence here is not about coverage but about
**receivers**: a broadcast with no sets in the footprint is inaudible, which
makes receiver distribution rather than transmitter siting the binding
constraint, and makes radio the first medium where the state must put hardware
in households rather than merely build infrastructure. **Oceania** is the
sharpest contrast with `ENG-230` — the region wire could not reach at all is
the region radio serves best, one transmitter covering what no cable ever
justified.

### Proposed stats

| stat | definition sketch |
| --- | --- |
| `broadcast reach` | population within receivable range, a function of transmitter power, terrain, and receiver density in the footprint — explicitly **not** a function of territory controlled, which is what separates it from `ENG-230`'s wire network. |
| `information sovereignty` | degree to which a state controls what its own population hears, newly contestable from outside the border and defensible only by continuous expenditure (jamming, receiver range-limiting, set licensing) that produces no output of its own. |
| `intermediary dependence` | a ruler's reliance on editors, notables, and clergy to reach the population, which print raised and broadcast collapses — the mechanism behind radio's centralising effect. |

### Links

`prereqs: [ENG-315, NAT-170]` (wireless telegraphy, electromagnetism).
`enables: [LAW-225, WAR-280, PHI-410]`.
`see_also: [ENG-230, WAR-310, MAR-340, ENG-300]` — electrification because
household receivers need household power, a real gating dependency that
explains part of the regional adoption pattern.

**Writing `ENG-325` retires three of the four dangling dependencies in the
headline table.** `MAR-340`'s radar dependency needs `WAR-315`.

---

## ID slot summary

No renumbering required. Every proposed slot is free. `renumber.py` not run.

| Proposal | Domain | ID | Between | Status |
| --- | --- | --- | --- | --- |
| Hertz confirmation | NAT | *extend `NAT-170`* | — | preferred over a new entry; `NAT-205` free if split |
| Telephone | ENG | `ENG-295` | ENG-290 (1876..1913) / ENG-300 (1882) | free |
| Wireless telegraphy | ENG | `ENG-315` | ENG-310 (1885) / ENG-320 (1909) | free |
| Broadcast radio | ENG | `ENG-325` | ENG-320 (1909) / ENG-330 (1947) | free |
| Radar (doctrine) | WAR | `WAR-315` | WAR-310 (1917) / WAR-320 (1940) | free |
| Spectrum as property | LAW | `LAW-225` | LAW-220 (1893) / LAW-230 (1948) | free |
| Radio navigation | MAR | `MAR-335` | MAR-330 (1906) / MAR-340 (1917) | free |
| Satellite positioning | ENG | `ENG-340` | above ENG-330 (1947) | free |
| *(fallbacks)* | | `ENG-326`, `DIP-185` | | free |

One collision to note: `MAR-335` is both the radio-navigation slot and the
fallback slot for a split-out SOLAS entry. Radio navigation has the better
claim; a SOLAS split would take `MAR-337`.

---

## Priority

1. **`ENG-325` broadcast radio** — clears three dangling dependencies, carries
   the most distinct mechanisms, and is the entry the corpus is most visibly
   missing.
2. **`ENG-315` wireless telegraphy** — `ENG-325`'s prereq; the chain does not
   connect without it.
3. **`WAR-315` radar** — clears `MAR-340`; strong design-principle-1 instance.
4. **`LAW-225` spectrum** — a genuinely novel legal object, not an extension.
5. **`NAT-170` extension for Hertz** — cheapest item on the list, a `date_end`
   change and two sentences.
6. **`ENG-295` telephone** — real and distinct, but nothing currently depends
   on it.
7. **`MAR-335` / `ENG-340` navigation and GPS** — furthest from the existing
   corpus and the least load-bearing.

## Secondary gaps noted, not proposed

- **Television** — absent apart from one passing mention in `HLT-340`. Needs a
  decision on whether it is a distinct mechanism from broadcast radio or a
  `region_variants`-and-`date_end` extension of `ENG-325`. Leaning: extension,
  since mechanisms 1-4 all apply unchanged, with the literacy-independence
  point if anything stronger.
- **`NAT-240` Information Theory is orphaned from its own origins.** Shannon's
  channel-capacity work came out of telegraph and telephone transmission
  engineering; `NAT-240` links to neither. At minimum add `ENG-230` to its
  `see_also`; a `prereqs` link becomes defensible once `ENG-295` exists.
- **No COM information-speed entry.** Flagged in full below.

## Flagged for TASK 1(b), not written

`ENG-230`'s Mechanism now carries the information-asymmetry argument and the
convergence-by-price rather than convergence-by-goods shift. It was to be
cross-linked to a COM entry on information speed. **No such entry exists.** All
23 COM titles were enumerated; the nearest are `COM-220` (Trademark, Brand, and
Trust at a Distance, 1860) and `COM-200` (Railways) — neither covers market
integration through information. The `information speed` stat is used by six
entries (`WAR-110`, `WAR-280`, `DIP-030`, `DIP-080`, `HLT-180`, `ENG-230`) and
`stats-observed.md` records its source as `—`, meaning no entry has ever
defined it.

**Gap flagged, not filled**, per instruction. Two candidate resolutions: a COM
entry on price convergence and market integration as an information
phenomenon, or promotion of `ENG-230` to the defining source for `information
speed` in `stats-observed.md`. The second is a one-line change and probably
correct regardless of whether the first is written.

A related finding from the same sweep: **`COM-200` Railways contains no
signalling or traffic-control material at all.** Its Mechanism covers route
design, financing structure, and labour; block signalling — the reason
telegraph and railway networks were built along the same rights of way, and
what actually governs the "track capacity" term `COM-200` names — is absent.
This is why `ENG-230` links to `COM-200` by `see_also` rather than `enables`:
the dependency is real in history but the target entry does not carry it.
Either `COM-200` gains a signalling paragraph and the link becomes directional,
or signalling earns its own ENG entry.
