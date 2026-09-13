---
id: NAT-260
title: Simulation as a Third Mode of Inquiry
legacy_ref:
domain: nature
domains_touched: [mathematics, engineering, war, health]
tier: core
status: drafted
confidence: medium
date_start: 1945
date_end: 2000
date_precision: decade
regions: [americas, europe, east-asia, south-asia]
region_variants:
  - region: americas
    date_start: 1946
    note: Monte Carlo methods at Los Alamos from 1946 and the ENIAC weather forecast of 1950 by Charney, Fjørtoft and von Neumann begin the mode — the latter running the numerical forecast Lewis Fry Richardson had attempted by hand in 1922 and failed. Norman Phillips's 1956 general circulation model and Manabe and Wetherald's 1967 climate model make the atmosphere a system studied by simulation. After the United States stopped nuclear testing in 1992, the Stockpile Stewardship Program from 1995 moved weapons physics almost entirely into simulation, driving the Accelerated Strategic Computing Initiative and the first teraflop machine in 1997.
  - region: europe
    date_start: 1953
    note: Soviet computational mathematics grows from the applied mathematics department Mstislav Keldysh built in 1953, later the Keldysh Institute of Applied Mathematics, which computed for the atomic and space programmes. Working on machines that lagged Western hardware, Soviet mathematicians produced numerical methods of lasting importance — Godunov's 1959 scheme for gas dynamics, Tikhonov's regularisation of ill-posed problems, Samarskii's difference schemes — so a hardware deficit was partly converted into methodological strength.
  - region: europe
    date_start: 1975
    note: The European Centre for Medium-Range Weather Forecasts, founded by a consortium of European states in 1975 and issuing operational forecasts from 1979, is the simulation-era counterpart of CERN — a shared computing facility and model no member could sustain alone, which became among the most accurate global forecasting centres in the world.
  - region: east-asia
    date_start: 1993
    note: Japanese supercomputing leads through vector machines built for scientific simulation — Fujitsu's Numerical Wind Tunnel from 1993 and then the NEC Earth Simulator, operated by JAMSTEC from 2002 and the fastest computer in the world until 2004. The Earth Simulator was built for climate and solid-earth modelling specifically, and its arrival at the top of the rankings was read in the United States as a strategic shock, which shows simulation capacity already counted as national capability.
  - region: east-asia
    date_start: 1983
    note: China's Yinhe machine at the National University of Defense Technology in 1983 begins a domestic supercomputing line that reaches the top of the world rankings with Tianhe-1A in 2010. After the United States barred exports of Intel processors to Chinese supercomputing centres in 2015, the Sunway TaihuLight took first place in 2016 on domestically designed processors — an export control converted into indigenous capacity by the following year.
  - region: south-asia
    date_start: 1988
    note: India founds the Centre for Development of Advanced Computing in 1988 after United States export restrictions blocked the supercomputer it sought for monsoon forecasting, and C-DAC's PARAM 8000 of 1991 is a parallel machine built domestically in response. The case is the clearest instance of computing capability treated as a dual-use strategic input and restricted accordingly, and of a sanctioned state responding by building the capability rather than doing without.
absence:
  - region: africa
    note: Simulation of African climate and disease through this window is produced almost entirely outside the continent, and it is weakest exactly where it is most needed, because validation requires observations and sub-Saharan Africa had among the sparsest weather-station and surveillance networks in the world. No regional supercomputing centre existed to run models locally, and the models run elsewhere could not be benchmarked against a record that did not exist — so what is absent is not only the machine but the observational base that makes a simulation's output checkable at all.
prereqs: [NAT-250, MTH-087]
enables: []
see_also: [MTH-084, MTH-062, WAR-275, NAT-090, NAT-150]
contested: true
contested_note: "The epistemic status of simulation is disputed. Oreskes, Shrader-Frechette and Belitz ('Verification, Validation, and Confirmation of Numerical Models in the Earth Sciences', Science, 1994) argue models of open natural systems can never be verified or validated in a strict sense, only confirmed to some degree, so simulation results are weaker than experiment in kind. Others argue simulation can function as a form of measurement or experiment — Morrison ('Models, measurement and computer simulation', Philosophical Studies, 2009) and Parker ('Does matter really matter?', Synthese, 2009) — and Winsberg (Science in the Age of Computer Simulation, 2010) treats it as a distinct practice with its own sanctioning methods. Galison ('Computer Simulations and the Trading Zone', 1996) shows Monte Carlo practitioners themselves treating it as neither theory nor experiment. Model simulation's credibility as conditional rather than fixed: higher where benchmarks against known cases exist, where independent models are compared and ensembles report their spread, and where the simulated regime lies close to observed ones; lower where the model is extrapolated far beyond any observation, and never scored simply as equivalent or inferior to experiment."
stats_touched: [intellectual authority, legibility, strategic input dependency, instrument threshold]
tags: [simulation, general-circulation-model, climate-model, monte-carlo, stockpile-stewardship, casp, model-intercomparison, keldysh, earth-simulator, c-dac, export-controls]
---

Simulation becomes a third way of knowing, alongside theory and experiment. It
reaches systems that can neither be solved analytically nor built and tested: you
cannot solve the equations and you cannot run the experiment, but you can run the
model. The claim is epistemic. It is not that computers made science faster.

## Timing

From Monte Carlo methods at Los Alamos in 1946 and the first numerical weather
forecast in 1950, through general circulation models from 1956, Soviet
computational mathematics from 1953, and ECMWF from 1975, to model
intercomparison projects and protein-structure prediction contests in the 1990s
and the move of nuclear weapons physics into simulation after testing ended in the
mid-1990s. Japan's, China's and India's national simulation capacities run through
the 1980s-2000s.

## Mechanism

**Why this entry is NAT and not MTH.** Boundary rules give MTH procedure and
computation, and ENG the machine. MTH-087 owns the stored-program architecture
that makes simulation possible. MTH-084 owns modelling as a method for deriving
consequences for administrative decisions, such as convoys, plans and strategy.
This entry's claim is different. It is about how knowledge of natural systems is
produced and justified when neither theory nor experiment is available. Its
central claims are refutable by observation, since a climate or galaxy simulation
makes predictions the world can contradict, and that is the MTH refutation test
failing, which places the entry in NAT. It links MTH-084 and MTH-087 rather than
re-deriving them.

**The only available mode.** In several fields simulation is not a faster
alternative to experiment. It is the only means of investigation:

- **Climate.** There is one atmosphere and no second Earth to use as a control.
- **Galaxy formation and cosmic structure.** The processes take billions of
  years.
- **Protein folding.** It happens faster and smaller than most instruments can
  follow.
- **Nuclear weapons physics after test bans.** Experiment became prohibited by
  treaty, and the entire enterprise moved into computation.
- **Epidemic projection.** The epidemic cannot be rerun under different
  policies.
- **Materials design.** The candidate compounds do not exist yet.

In each case the system is too large, too slow, too dangerous, too fast or
unavailable to experiment on.

**Consequence one: the validation problem.** An experiment's result can be checked
against the world because the world produced it. A simulation's output is checked
against the world only where observations exist, and simulation is most needed
exactly where they do not. So a new practice of justification had to be invented,
and it is a real methodological innovation:

- **Benchmarking against known cases.** Run the model on a past period or a
  solved problem and see whether it reproduces what happened.
- **Model intercomparison.** Independent groups run different models of the
  same system under common protocols, as in climate intercomparison from the
  1990s, and the spread between them becomes a measure of uncertainty.
- **Blind prediction contests.** Protein-structure prediction from 1994 asked
  groups to predict structures that had been solved but not yet published.
- **Ensembles and sensitivity analysis.** Many runs with varied parameters show
  which conclusions survive the modellers' choices.

None of these is observation. Together they build credibility, and whether that
credibility equals an experiment's is disputed.

**Consequence two: data volume and automated analysis.** Big Science instruments
(NAT-250) produce more data than any team can inspect. Particle-physics triggers
discard almost all collisions before anything is stored, and sky surveys image
more objects than anyone could look at. Reduction therefore has to be automated
and filtered statistically. The scientist stops examining observations and starts
examining the output of a pipeline they did not personally write. That puts a new
kind of instrument between the investigator and the evidence: software that
encodes decisions about what counts as signal. The inference that pipeline
performs rests on MTH-062's methods, and in climate science even the observational
record is filled in and made consistent by models. Edwards calls this the mutual
dependence of models and data (_A Vast Machine_, 2010). The line between measurement and simulation blurs.

**Consequence three: the reproducibility inversion.** Experimental science checks
a result by having others repeat the experiment. NAT-090 names that expectation.
The publishing and replicating machinery that would enforce it is not yet carried
in NAT-150, which covers only the society as a reviewing body. A simulation
reverses the norm. Run the same code on the same inputs and you get the same
answer every time, and that repeatability says nothing about whether the answer is
right. Meanwhile, results that should be reproducible often cannot be reproduced.
The code may be unavailable. Outputs depend on software versions, compilers and
hardware. Two groups modelling the same system make different, equally defensible
choices and get different answers. Reproducibility and validity, which experiment
had tied together, come apart. The response, arguments for publishing code and
data with every computational result from the 1990s onward, is a new norm being
built to cover what the old one no longer did.

**Capability becomes a strategic input.** Computing power large enough for
weapons, cryptanalysis and climate modelling was restricted for export because it
was a military and scientific capability at once. That makes simulation capacity a
strategic input with the structure of WAR-275's oil dependency, with one
difference that matters. A geological absence cannot be engineered away. A
computing embargo can be, and India in 1991 and China in 2016 both answered denial
by building the machine. The dependency is real, and it is breakable by domestic
investment in a way that oil never was. Supercomputers also sit above an
`instrument threshold` of their own, which is why the European forecasting centre
was a consortium like CERN.

## Social consequence

At the scale of a working life, a large share of science becomes coding. A
climate scientist, an astrophysicist or a weapons physicist from the 1970s onward
spends much of the day writing, debugging and running model code, and their
expertise includes knowing which numerical choices are defensible. Credit
questions follow. The people who build and maintain the model are often not the
authors of the papers it enables, and scientific software is poorly recognised in
the reward structure NAT-150 describes.

`intellectual authority` shifts toward institutions that hold the largest models
and machines, and the public increasingly meets science as projection: a
temperature range for 2100, an epidemic curve under different policies, a
forecast cone. Those outputs carry authority in policy that depends on the
validation practices above, which the public cannot inspect. When projections are
contested politically, the dispute often lands on exactly what simulation cannot
supply, a direct observation of the future. The Stockpile Stewardship case adds a
sharper version. A state's confidence that its deterrent works rests on models no
one is permitted to test against an explosion.

## Regional specifics

This entry defaults to an American story unless the other cases are carried at
full weight, and they belong in it on their merits.

- **Soviet computational mathematics** under Keldysh produced foundational
  numerical methods on weaker hardware, turning a machine deficit into a
  methods strength.
- **ECMWF** is the European consortium answer, a shared model and machine no
  member could run alone.
- **Japan** built the world's fastest computer specifically for Earth-system
  simulation, and its arrival was treated as a strategic event.
- **China** answered a processor export ban by reaching first place on domestic
  chips.
- **India** founded C-DAC because a supercomputer for monsoon forecasting was
  denied, and built a parallel machine within three years.

The last two are the export-control story. Computing was restricted as a strategic
input and was rebuilt domestically where the state could afford to.

The United States supplies the method's origin in weapons work, the first
numerical forecast, the general circulation model, and the move of an entire
weapons programme into simulation after test bans. Sub-Saharan Africa is the
genuine absence. Models of its climate and diseases were run elsewhere and could
not be checked against a sparse observational record, so the region lacked both
the machine and the observations that make a simulation's output testable.

## Contested

Whether simulation produces knowledge of the same standing as experiment is
unresolved. One position holds that models of open natural systems can only be
confirmed, never verified, and are weaker in kind. Another treats simulation as a
form of measurement or experiment with its own methods of justification, and
historians of Monte Carlo show its practitioners regarded it as neither theory nor
experiment.

Model credibility as conditional rather than fixed. It should be higher where
benchmarks, independent model comparisons and ensembles exist and where the
simulated regime lies near observed ones. It should be lower where the model is
extrapolated far past any observation. Never score simulation simply as equal or
inferior to experiment.

## Hook

A third mode of inquiry, gated on `instrument threshold` computing capacity. It
unlocks investigation of systems that neither theory nor experiment can reach:
climate, cosmic structure, protein folding, untestable weapons, epidemics and
undiscovered materials.

**Credibility is conditional.** The weight of a simulation's output in
`intellectual authority` and policy should depend on validation infrastructure:
benchmark cases, model intercomparison and ensembles, and above all observations to
check against. Extrapolation beyond observed regimes should lower it, so a state
with strong machines and a thin observational record gets confident but
unverifiable answers.

**Instruments now produce data faster than people can inspect it.** Automated
pipelines become a new instrument between scientists and evidence, and they extend
`legibility` over natural systems while embedding unexamined analytic choices.

**Reproducibility decouples from validity.** Identical reruns guarantee nothing, and
different defensible models disagree, so trust depends on new norms of publishing
code and data rather than on repetition.

**Computing capacity is a strategic input.** It is restricted by export control like
any `strategic input dependency`, but unlike a geological absence it can be broken
by domestic investment, at a cost a state must choose to pay.
