# Energy and Fuel Supply — Coverage Check

**Date:** 2026-09-12
**Scope:** energy sources and fuel supply across the corpus, from the organic
economy through coal, whale oil, petroleum, refining, and oil geopolitics.
**Method:** corpus-wide grep for coal, petroleum, oil, kerosene, gasoline,
petrol, diesel, refining, distillation, cracking, whale oil, whaling, charcoal,
peat, firewood, fossil, drilling, pipeline, OPEC, energy — plus timber,
deforestation, and steam; followed by inspection of every energy-converting
entry and a date-banded sort check of ENG, COM, WAR, MAR, LAW and DIP.
**No entries written.** Proposals only.

---

## PART 1 — INVENTORY

### The headline answer

**No entry in this corpus has energy supply as its own mechanism.** Not one.

The corpus contains energy *converters* (ENG-170 steam engine, ENG-120
windmill and watermill, ENG-290 internal combustion engine), an energy
*distributor* (ENG-300 electrification), and an energy-consuming industrial
chain of roughly forty entries. It contains nothing on where the energy comes
from, what it costs, whether it is a flow or a stock, or what happens when it
runs out or is replaced. The chain has converters at every stage and no source
at any stage.

### Hit-by-hit

| Term | Entries | Own Mechanism, or incidental? |
| --- | --- | --- |
| `coal` | ENG-170 | **Incidental.** Four words — "the chemical energy in fuel (initially coal)" — in a parenthesis. Nothing else. |
| `charcoal` | ENG-010, ENG-140 | **Incidental** in both. ENG-010 says smelting "requires a fuel supply chain (charcoal production...)" and stops there; ENG-140 means the gunpowder ingredient, not a fuel. |
| `petroleum` | MAR-100 | **Incidental**, but load-bearing in an unexpected way — see below. One clause. |
| `oil` | MAR-100 (whale oil); FIN-250, FIN-270, MIG-200, MIG-270, HLT-350 (Gulf oil states) | **Incidental everywhere.** The FIN/MIG/HLT cluster treats oil wealth as an exogenous given — six entries build mechanisms on oil revenue without any entry explaining where oil revenue comes from. |
| `whale` / `whaling` | MAR-100 | **Own Mechanism** — the only genuine hit in the inventory. See Part 2(c). |
| `gasoline` | AGR-260 | **Incidental.** The gasoline tractor as a displacer of draft animals; the fuel is a modifier on a farm-mechanisation entry. |
| `diesel` | ENG-290 | **Incidental.** A tag and one clause naming Rudolf Diesel. |
| `refining` | ENG-100, ENG-140, NAT-040, NAT-160 | **False positives, all four.** Paper refining, saltpetre refining, "refining what has worked before." Zero petroleum refining anywhere. |
| `fossil` | NAT-100, NAT-230 | **False positives.** Palaeontological fossils in geology and plate tectonics. No fossil fuel. |
| `energy` | ENG-170, ENG-120, ENG-080, ENG-300, ENG-320, NAT-210, NAT-220, WAR-330 | **Incidental.** Used in the physics sense or as a synonym for effort. No entry treats an energy budget as a constraint. |
| `timber` | MAR-010, ENG-200 | **Incidental.** Shipbuilding material and a bridge material. Never a fuel, never a scarce resource. |
| `kerosene`, `distillation`, `cracking`, `peat`, `firewood`, `drilling`, `pipeline`, `OPEC`, `deforestation` | — | **Zero hits.** |

### What the grep also turned up

- **`coal` is almost entirely "coalition".** Of the entries the first pass
  flagged, LAW-060, LAW-160 and PHI-240 are false positives, leaving exactly
  two real occurrences in the whole corpus.
- **MAR-100's `date_end` is 1859, and the reason given in its own body is
  "petroleum displaces whale oil as a lighting fuel by 1859."** The corpus
  therefore dates the end of an entire industry to a substitution event whose
  other half does not exist. `enables: []` — MAR-100 is a dead end. The
  stranding mechanism is present, in the sense that the incumbent's death is
  recorded, and the thing that killed it is not in the corpus.
- **Six entries build on oil revenue as an unexplained given** — FIN-250,
  FIN-270, MIG-200, MIG-270, HLT-350, and DIP-190 adjacent. This is the same
  structural pattern the telecommunications audit found with radio: entries
  resting on a prerequisite that has no ID to carry.

---

## PART 2 — CHAIN CHECK

### a. The organic energy economy and its ceiling — ABSENT

Nothing. No entry on firewood, charcoal as a fuel supply, peat, or the
timber-scarcity constraint. Wrigley's organic-to-mineral distinction does not
appear and neither does any equivalent framing.

What exists is scattered and unconnected: ENG-120 covers water and wind as
converters with `stats_touched: []`; ENG-010 names a charcoal supply chain in
passing; AGR-110 has the single best piece of organic-energy material in the
corpus — South Asian dung as a contested dual use between fertiliser and fuel —
sitting inside a soil-nutrient entry with no energy framing at all. Human and
animal muscle as an energy source appears nowhere as such, though MIG-050,
AGR-130 and AGR-260 all depend on it.

**This is the deepest gap in the audit.** It is also the one that gates the
most: no entry in the corpus can currently explain why industrialisation
happened where and when it did, because the constraint that fossil fuel
relieved is not modelled.

### b. Coal — ABSENT, and yes, it is the same bug one level down

**The corpus has the steam engine without the coal economy under it.**
ENG-170 The Steam Engine: `prereqs: [NAT-040]` — mechanics, nothing else.
`stats_touched: []`. Coal appears once, in a parenthesis. ENG-170 `enables`
seven entries including COM-170 (the factory), COM-200 (railways) and ENG-300
(electrification), so the entire industrial chain descends from an entry whose
own fuel is a parenthetical aside.

ENG-170 additionally carries **the same defects this audit was called to fix in
ENG-290**, which suggests a systematic rather than isolated problem:

| Defect | ENG-170 | ENG-290 | ENG-300 |
| --- | --- | --- | --- |
| `date_precision: century` on a short span | yes (118 yr) | yes (37 yr) | yes (53 yr) |
| `stats_touched: []` | yes | yes | yes |
| No fuel or energy prereq | yes | yes | yes |
| `regions` too narrow | `[europe]` only | omits east-asia | — |

ENG-170's `absence` field is, to its credit, used correctly — east-asia, no
independent steam development. That one is fine.

### c. Whale oil and the sperm whale fishery — EXISTS, and is good

MAR-100 Fishing and Whaling (1000-1859, `stats_touched: [fishery stock]`) is
the only entry in the inventory with energy material as its own mechanism, and
it is well built: Basque Biscay whaling, Nantucket and New Bedford deep-ocean
sperm whaling as multi-year blue-water industry, Japanese coastal cooperative
whaling as a genuine organisational contrast, and a South Asian absence
explained by the monsoon system's better return on the same coastal capital.
`fishery stock` is a proper depleting pool under design principle 4.

**Recommendation: do not split it.** The illuminant-industry material belongs
where it is. What is missing is not an entry but the *other side of the
substitution* — MAR-100 records a death with no killer. Fix by giving MAR-100
`enables: [ENG-265]` once petroleum exists, which also retires its dead-end
`enables: []`.

### d. Petroleum extraction — ABSENT

Zero hits for drilling, pipeline, or any extraction material. Petroleum appears
once in the corpus, in MAR-100's clause about what displaced whale oil.

### e. Refining — ABSENT

Zero hits for distillation or cracking. All four `refining` hits are false
positives (paper, saltpetre, "refining what has worked before"). The fixed-ratio
problem, thermal cracking (1913) and catalytic cracking (1937) are entirely
absent.

### f. Oil as strategic dependency — ABSENT

No 1911 Royal Navy conversion, no Anglo-Persian concession, no Baku, no
Ploiesti, no Japanese oil position in 1941, no OPEC, no 1973. Checked
specifically: **DIP-190 Sanctions does not carry the 1973 embargo** — its
embargo material is Cuba and Iran, and its range (1935-1994) spans 1973 without
touching it.

**Important correction to the brief's premise.** The brief suggests
cross-linking the tin mechanic in the bronze entry rather than re-deriving
concentration-as-asymmetry. **COM-020 Metallurgy as Craft and Commodity
contains zero occurrences of "tin" or "cassiterite."** Its Mechanism is about
skill scarcity driving guild formation — "converting raw ore availability into
a value-added tradeable commodity whose price reflects skilled labor and fuel
input as much as raw material cost." Input geography is explicitly *not* its
subject. The nearest thing anywhere is ENG-010's africa `region_variant`, which
notes that copper and tin ores are less accessible than iron and that sub-Saharan
smelting therefore skipped bronze — one sentence, framed as a technological
sequence rather than a strategic dependency.

So the concentration mechanic has **no prior home to cross-link to**. A proposal
here originates it rather than reusing it, and a secondary recommendation
follows: COM-020 or ENG-010 should gain the tin material, since bronze-age tin
is the cleanest pre-modern instance of the same mechanic and the corpus
currently drops it.

### g. Vertical integration and antitrust — ABSENT, both halves

No Standard Oil, no Sherman Act, no antitrust or competition law anywhere. The
`monopol` hits are unrelated — COM-150's chartered-company monopoly, LAW-225's
spectrum licence, MAR-150's smuggling. FIN-070 Joint-Stock Companies is the
nearest structural neighbour and covers the corporate form, not integration or
its legal limits.

### h. Electrification's fuel source — NOT STATED

**ENG-300 does not say what generates the electricity.** It covers distribution,
the grid, AC versus DC, and rural access. `prereqs: [ENG-170, NAT-170]` implies
coal through the steam engine, but the implication is never drawn and the word
coal does not appear. `stats_touched: []`. The entry converts "energy access
from something each user had to generate locally into a purchased utility" —
which is precisely a statement about supply — without naming a single source.

---

## PART 3 — PROPOSALS

All seven slots below were verified free by sort-position check. No renumbering
required. `renumber.py` not run.

### LOAD-BEARING

#### 1. `ENG-025` — The Organic Energy Economy and Its Ceiling

**Placement: ENG.** Contestable, and worth arguing. The mechanism is land
allocation, which points at AGR — but AGR owns production and diet, and this
entry's own claim is about usable energy supply and its aggregate bound. The
decisive argument is chain position: it must gate ENG-010 (charcoal smelting),
ENG-120 (water and wind conversion) and ENG-170 (the fossil escape), and ENG
already de facto owns energy conversion through ENG-120. Housing it in AGR
would sever it from the entire industrial chain it exists to gate and would
misdescribe watermills and charcoal burning as agriculture.
`domains_touched: [agriculture, commerce, nature, health]`. Boundary neighbour
to watch: AGR-090 Population Pressure and Agricultural Intensification, which
handles land pressure from the food side — the split is that AGR-090 asks how
much food an acre yields and this asks what else that acre could have been.

**Dates: -6000 to 1800**, `date_precision: era`. Not earlier — see the ID
warning below.

**Mechanism — the organic ceiling.** A pre-fossil economy is bounded by the
annual photosynthetic yield of the land it controls. Every joule comes from
current sunlight captured this year, which means **energy competes directly
with food, timber and fodder for the same acreage** — charcoal for a furnace is
woodland not cleared for grain, fodder for draft animals is arable not growing
human calories, and the competition is zero-sum within a fixed land area.
Growth in any one use must come out of another, which is why organic economies
run into walls rather than curves.

Fossil fuel removes the competition entirely by drawing on stored rather than
current flow — a stock accumulated over geological time, released at whatever
rate extraction allows, with no acreage cost at all. This is Wrigley's
organic-to-mineral distinction, it is the deepest available explanation for why
industrialisation happened where it did, and **it should gate the industrial
chain rather than sit beside it.**

**Regional coverage, and it is strongly non-European.** Tokugawa Japan is the
best case in the entry and should lead: a state that hit the timber ceiling
hard in the seventeenth century and solved it by regulation and silviculture
rather than by fossil substitution — Totman's account — reaching a sustainable
organic equilibrium instead of escaping the constraint. That is design
principle 5 exactly: a different shape, not a deficit. Song and Ming China
supply the Jiangnan timber crisis and the early coal substitution that makes
the Great Divergence question live. South Asia supplies dung as fuel, already
present in AGR-110 as a documented fertiliser-versus-fuel tension and needing
only to be linked. The Middle East supplies chronic timber scarcity — cedar
imported from Lebanon, mudbrick construction as the substitute (ENG-040), an
organic economy running permanently below the ceiling other regions had to grow
into. Africa supplies charcoal-based iron smelting at scale (ENG-010). Europe
supplies the English case and is one instance among six, not the frame.

**Absence (genuine).** Americas: no draft animals, no watermills or windmills
(ENG-120's own absence), so the pre-contact energy economy is human muscle plus
fire and nothing else — which forecloses not just industry but the entire
water-powered milling and bellows chain, and is why Andean metallurgy stayed
ornamental (ENG-010). Oceania: no landmass large enough to accumulate a timber
surplus against a ceiling in the first place.

**New stat: `energy budget`.** Reuse `yield ceiling` (AGR-020) for the land
competition and `toxic burden` (HLT) for fuel smoke.

> **ID WARNING.** `ENG-025` works only if `date_start` is -6000 or later. It
> sorts after ENG-020 (-6000..-250) on the `date_end` tiebreak and before
> ENG-030 (-4500). **If the entry is dated to the origin of settled agriculture
> (-8000 or earlier) instead, no slot exists** — ENG's floor is exactly ENG-010
> and there is nothing below it. Dating it -6000 is defensible on its own terms
> (the horizon at which charcoal smelting makes the energy budget a binding
> allocation problem, matching ENG-010's own date) and avoids a renumbering run.
> If an earlier date is preferred, that is a renumber decision, not a slot.

#### 2. `ENG-165` — The Coal Economy

**Placement: ENG.** COM has a claim — coal is a bulk commodity and COM owns
goods in motion — but the entry's mechanism is extraction and its physical
constraints (drainage, haulage, pit depth), which is applied capability. The
decisive point is the feedback loop: the steam engine was invented *to pump
water out of coal mines*, so coal and ENG-170 are one technical system and
splitting them across domains would break the causal loop that makes both
interesting. `domains_touched: [commerce, health, war, nature]`.

**Dates: 1550 to 1900**, `date_precision: decade`. Song-dynasty coal goes in a
`region_variant` dated 1000 rather than setting `date_start` — which is exactly
what `region_variants` are for, and it keeps the slot clean.

**Mechanism.** Coal converts energy from a flow into a **stock** — the single
most consequential property change in this audit. A woodland yields a
sustainable annual increment forever; a coal seam yields whatever you can lift
and then is gone. Three consequences: extraction is bounded by engineering
(drainage and haulage) rather than by biology, so investment raises output in a
way replanting never could; the resource is **geographically fixed**, so
industry relocates to the coal rather than the reverse, which reorganises
settlement around pits; and depth imposes its own escalating cost, with water
ingress the binding constraint that calls the steam engine into existence.

**Contested: true — and this is the entry's best feature.** Why China did not
industrialise on its coal despite Song Kaifeng's coal-fired iron industry is a
live dispute. Pomeranz argues geography — China's coal was in the northwest,
far from the Yangtze delta's commercial core, while England's sat next to its
population and water transport. Critics argue the constraint was demand,
institutions, or that the comparison unit is wrong. **Model it as a conditional
on coal-to-market distance and water-transport access rather than picking a
side**, per design principle 6. This is a much better conditional than most in
the corpus because the variable is concrete and measurable.

**Regional coverage.** China (Song coal-iron, and the divergence question),
Britain, Germany (Ruhr), Japan (Chikuho, and the state's role in developing
it), South Asia (Raniganj from 1774, colonial development for railway fuel),
southern Africa (Witwatersrand coal underpinning the gold industry).

**Absence (genuine).** The Middle East has essentially no accessible coal — a
real foreclosure that explains late industrialisation despite proximity to
Europe and Ottoman administrative capacity, and sets up the region's later
leapfrog directly to oil without a coal stage. This is the single best absence
available in the whole energy chain because the same region later becomes the
energy centre of the world through a different fuel.

**Stats:** `energy budget`, `toxic burden` (reuse — coal smoke is the entry's
health consequence), new `extraction depth cost`.

#### 3. `ENG-265` — Petroleum Extraction

**Placement: ENG.** Extraction technique. `domains_touched: [commerce, war,
maritime, finance]`. **Dates: 1859 to 1911**, `date_precision: decade`.

**Mechanism — substitution and stranding, which is the real content.** An
energy transition does not add a source; **it strands the incumbent's capital.**
Kerosene destroyed the whale fishery in roughly twenty years, and what died was
not a resource but an investment — purpose-built ships useless for anything
else, port infrastructure at New Bedford and Nantucket, multi-year voyage
financing, and a body of skill with no transferable application. The
displacement should be modelled, not just the adoption: the incumbent's assets
go to near-zero *while still physically functional*, which is a different and
crueller mechanic than depletion. MAR-100 already models the depletion side
through `fishery stock`; this supplies the half that actually ended the
industry.

**Regional coverage — and the brief's warning is correct and important.**
Middle East oil is late (Persia 1908) and must not be the frame. The early
industry is elsewhere: **Baku** is the essential case — Russian Azerbaijan
produced roughly half the world's oil around 1900, with Nobel and Rothschild
capital, and it belongs at the centre of the entry rather than as a footnote to
Pennsylvania. **Burma's Yenangyaung** is the case that breaks the Drake framing
altogether, with indigenous hand-dug wells and a hereditary extraction right
operating at commercial scale *before* 1859. Sumatra (Royal Dutch), Mexico and
Venezuela, then Persia. The Americas supply Drake and Spindletop. That
distribution makes the entry genuinely co-equal without reaching for it.

**Absence (genuine).** Japan — negligible domestic petroleum, which is the
direct cause of the 1941 strategic position and should be planted here for
WAR-275 to collect. Sub-Saharan Africa before the Nigerian finds of the 1950s.
Germany — coal-rich and oil-poor, the specific asymmetry that produced
synthetic-fuel programmes.

**New stat: `stranded capital`.** Reuse `fishery stock`.

### VALUABLE

#### 4. `ENG-275` — Refining and the Fixed-Ratio Problem

**Placement: ENG.** **Dates: 1863 to 1937**, `date_precision: decade`. Sorts
after ENG-270 (1863..1927) on the `date_end` tiebreak.

**Mechanism.** Crude yields fractions in proportions the refiner does not
choose. A barrel gives what it gives, so a refiner optimising for one product
is stuck with the others — **early refiners wanted kerosene and dumped gasoline
as a hazardous waste**, and then the automobile inverted the demand and the
waste became the product. That inversion is the mechanic: a fixed output ratio
against a shifting demand ratio, with the mismatch appearing as either glut or
shortage in every fraction simultaneously.

Cracking is what lets the ratio be shifted — thermal (Burton, 1913) then
catalytic (Houdry, 1937) — converting a fixed joint-product problem into a
controllable one. **Catalytic cracking is a genuine WAR dependency and not a
chemistry footnote**: it is what made 100-octane aviation fuel available at
scale, which is a real and quantifiable performance difference in piston
aircraft, and it belongs in the corpus for that reason alone.

**Regional coverage.** United States, Russia (Baku refining), Germany
(Bergius hydrogenation and Fischer-Tropsch synthesis — the absence-driven
alternative, making liquid fuel from coal because there was no oil), Japan
(refining capacity as the binding constraint behind the 1941 calculation),
Romania (Ploiesti as Europe's refining centre). Germany's synthetic-fuel case
is the strongest regional content here because it shows the constraint being
engineered around at ruinous cost rather than simply suffered.

**New stat: `refining flexibility`.**

#### 5. `WAR-275` — Oil as Strategic Dependency

**Placement: WAR.** DIP has a real claim — the Anglo-Persian concession and the
1973 embargo are inter-polity relations, and boundary-rules says the
inter-polity axis wins for treaties. But the entry's own claim is about
**military capability gated on an input a state cannot source**, which is WAR's
side of the WAR/ENG and LAW/WAR splits alike. Churchill's 1911 conversion of
the Royal Navy from Welsh coal to oil is a doctrine decision — more speed and
range, bought by exchanging a domestic fuel for a foreign one — and everything
downstream follows from that trade. `domains_touched: [diplomacy, maritime,
commerce, finance]`. **Dates: 1911 to 1973**, `date_precision: decade`.

**One entry with region_variants, not a series of national cases**, per the
brief. Variants: Britain and Persia (1911 conversion and the concession that
followed), Germany (Ploiesti, the drive for Baku, synthetic fuel as the
substitute for access), Japan (the 1941 position — embargo against a state with
no domestic oil and limited refining, and the decision it forced), the United
States (the only major power that was a net exporter, and what that permitted),
the Middle East and OPEC (1973, the commodity weapon).

**Mechanism — concentration as strategic asymmetry.** Coal is widespread; oil
is not. A militarily essential input that most powers cannot source
domestically produces a dependency that is qualitatively unlike any other
logistics constraint, because **it cannot be solved by producing more of it at
home at any price** — only by securing access to someone else's territory,
which converts an industrial question into a territorial one.

**Note on the tin cross-link: it is not available.** As established in Part
2(f), COM-020 carries no tin content whatsoever. This entry would **originate**
the concentration mechanic rather than reuse it. Secondary recommendation:
COM-020 or ENG-010 should gain the bronze-age tin material, at which point the
two cross-link properly and the mechanic gains a 3,000-year precedent — which
would be considerably stronger than presenting it as a twentieth-century
novelty.

**Absence (genuine).** Landlocked or oil-less powers whose strategic options
narrowed regardless of industrial capacity — and the useful inversion, states
with oil but no refining or tanker capacity, for whom possession did not equal
access.

**New stat: `strategic input dependency`.** Verified against
`stats-observed.md`: nothing comparable exists.

#### 6. `LAW-215` — Antitrust and the Limits of Combination

**Placement: LAW.** **Dates: 1890 to 1911**, `date_precision: decade`. A new
legal object in the same sense LAW-225 is — the state asserting a power to
break up a lawfully-assembled private structure that has broken no other law.
`domains_touched: [commerce, finance, engineering]`.

### OPTIONAL

#### 7. `COM-225` — Vertical Integration and the Standard Oil Model

**Placement: COM.** **Dates: 1870 to 1911**, `date_precision: decade`.

**Split rationale.** The brief treats vertical integration and antitrust as one
item. Boundary-rules is explicit that two substantial mechanisms are two
entries, and these are genuinely distinct — the integrated corporation is a
commercial form (control the pipeline, the refinery, the tank car and the
distribution to escape dependence on any counterparty) while antitrust is a
legal instrument. **However**, they share dates, region, and case material
almost exactly, and boundary-rules also warns that identical comparative
material across two entries is a sign they should be one entry with two
`domains_touched`. That tension is why this is ranked OPTIONAL rather than
VALUABLE: write LAW-215 first, and write COM-225 only if the commercial
mechanism turns out to need its own Mechanism section rather than a paragraph.

### Not recommended

- **Splitting MAR-100.** The whale-oil material is well covered where it is.
  Fix the dead-end `enables` instead.
- **A standalone "energy transition" entry.** The substitution-and-stranding
  mechanic is better carried concretely by ENG-265 than abstractly on its own.

### ID slot summary

| Proposal | Domain | ID | Sorts between | Status |
| --- | --- | --- | --- | --- |
| Organic energy ceiling | ENG | `ENG-025` | ENG-020 (-6000..-250) / ENG-030 (-4500) | free — **only if dated -6000 or later** |
| Coal economy | ENG | `ENG-165` | ENG-160 (1092) / ENG-170 (1712) | free |
| Petroleum extraction | ENG | `ENG-265` | ENG-260 (1856) / ENG-270 (1863) | free |
| Refining | ENG | `ENG-275` | ENG-270 (1863..1927) / ENG-280 (1866) | free |
| Oil as strategic dependency | WAR | `WAR-275` | WAR-270 (1911..1945) / WAR-280 (1914) | free |
| Antitrust | LAW | `LAW-215` | LAW-210 (1883) / LAW-220 (1893) | free |
| Vertical integration | COM | `COM-225` | COM-220 (1860) / COM-230 (1876) | free |

### Build order

`ENG-025` → `ENG-165` → `ENG-265` → `ENG-275` → `WAR-275` → `LAW-215` →
`COM-225`. Each depends on the one before it for a prereq.

---

## ENG-290 — fixes specified

To apply when the chain is built, not before, since two of them need entries
that do not yet exist.

| # | Defect | Fix |
| --- | --- | --- |
| 1 | `date_precision: century` on a 37-year span | → `decade` |
| 2 | `absence` holds late Japanese presence | Delete. **The material is already duplicated verbatim in Regional specifics** — the paragraph beginning "Japanese automotive capability developing only from the 1920s-1930s" covers it, so nothing is lost. Replace with genuine absence: regions with no refining or fuel-distribution access, where the engine was available and the fuel was not. An internal combustion engine without a fuel distribution network is an ornament, and that is the real absence — the constraint is downstream of the artifact. |
| 3 | `stats_touched: []` | → `[energy budget, stranded capital]`, plus `refining flexibility` once ENG-275 exists. The ICE is what inverted the fixed-ratio problem, turning gasoline from waste into product, so it should touch that stat rather than merely cite it. |
| 4 | No fuel prereq | → add `ENG-275` (refining), **not** `ENG-265`. The engine needs gasoline, not crude; the refining step is the actual dependency and the distinction is mechanically real. |
| 5 | `regions` omits east-asia | → add `east-asia`, and convert the deleted `absence` note into a proper `region_variant` dated 1920. |

### Same defects found elsewhere, not in the brief's scope

**ENG-170 The Steam Engine** carries all five: `century` precision on 118
years, empty `stats_touched`, no fuel prereq, `regions: [europe]` only. Given
that it `enables` seven entries including the factory, railways and
electrification, it is arguably a higher-value fix than ENG-290. Its `absence`
field is correctly used.

**ENG-300 Electrification** carries three: `century` precision on 53 years,
empty `stats_touched`, and no statement of what generates the electricity
despite a Mechanism explicitly about converting local generation into purchased
supply. Needs `ENG-165` as a prereq once coal exists.

**ENG-120 Windmill and Watermill** has empty `stats_touched` and no energy
framing; it should touch `energy budget` and become a prereq sibling under
ENG-025.

---

## Summary

The corpus documents an industrial revolution with no fuel, a steam engine with
no coal, an internal combustion engine with no petroleum, an electrical grid
with no generation, and six entries resting on oil revenue that no entry
explains. The single genuine piece of energy-supply content in 389 entries is
the whale fishery in MAR-100 — which ends, by its own date and its own body
text, at the arrival of a substitute the corpus does not contain.
