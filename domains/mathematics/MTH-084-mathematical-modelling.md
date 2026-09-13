---
id: MTH-084
title: Mathematical Modelling as a Method
legacy_ref:
domain: mathematics
domains_touched: [war, finance, commerce, law, maritime]
tier: core
status: drafted
confidence: medium
date_start: 1939
date_end: 2000
date_precision: decade
regions: [europe, south-asia, americas]
region_variants:
  - region: europe
    date_start: 1939
    note: Leonid Kantorovich in Leningrad formulates linear programming in 1939 while solving a plywood trust's production problem, showing how to allocate resources optimally under constraints. The work was neglected for two decades under Soviet planning, partly because its optimal solutions generate shadow prices — implicit valuations that looked uncomfortably like market prices and exposed where planned prices were wrong. He shared the Nobel prize in economics in 1975 for work his own state had set aside.
  - region: europe
    date_start: 1940
    note: British operational research teams from 1940, associated with Patrick Blackett, apply modelling to radar deployment, bombing and anti-submarine warfare. Their best-known result is that larger convoys lose a smaller proportion of their ships, because the escort perimeter grows more slowly than the area it protects (MAR-340) — a counterintuitive policy derived from a model rather than from experience, and adopted because the derivation was convincing.
  - region: americas
    date_start: 1944
    note: Von Neumann and Morgenstern's Theory of Games of 1944, Dantzig's simplex method for linear programming in 1947, and the Monte Carlo method developed at Los Alamos from 1946 give modelling its general tools, and the RAND Corporation from 1948 applies them to strategy, most consequentially to nuclear deterrence. Modelling becomes an institution in its own right — a professional class paid to derive consequences for governments.
  - region: south-asia
    date_start: 1955
    note: P. C. Mahalanobis builds the model behind India's Second Five-Year Plan from 1955-56, deriving the share of investment to direct into heavy industry from a formal growth model, and the Indian Statistical Institute supplies both the model and the survey data to calibrate it. It is the clearest case of a newly independent state choosing its development path by derivation, and one of the most consequential uses of a mathematical model in any economy.
absence:
  - region: africa
    note: A model substitutes for a system only if the system has been measured well enough to calibrate the model, and across much of post-independence Africa the inherited statistical base was thin — colonial enumeration had often counted households or taxpayers by estimate rather than individuals (MTH-075). Planning models were frequently supplied by external consultants and calibrated on data that did not exist at the needed resolution, so the method's central promise, deriving consequences reliably, could not be kept. What was absent is not modelling expertise but the measured system a model needs.
prereqs: [MTH-040, MTH-035, MTH-062]
enables: [WAR-330]
see_also: [MAR-340, FIN-200, MTH-087, WAR-290, MTH-075]
contested: true
contested_note: "Three disputes. First, whether a model can substitute for prices in allocating an economy is the socialist calculation debate — Mises and Hayek argued central planners could not obtain the information prices convey, and optimal-planning advocates argued models could compute it — and the Soviet experience, including the neglect of Kantorovich, is read both ways. Model modelling as improving allocation conditional on measurement quality and on whether the planner will accept results that contradict policy. Second, the effect of the Mahalanobis model on Indian growth is disputed between those who credit it with building an industrial base and those who blame it for decades of slow growth; model the outcome as contested rather than settled. Third, models used to manage war by metrics — body counts and kill ratios in Vietnam — show that a model substituting for a system can substitute badly when the measured quantity becomes the goal; model degradation when the modelled variable is also the target that officials are rewarded on."
stats_touched: [legibility, intellectual authority]
tags: [operations-research, linear-programming, game-theory, monte-carlo, kantorovich, mahalanobis, rand]
---

A formal model substitutes for the system it represents, so consequences are
derived rather than observed — and a policy can be tested before it is enacted.
This is where mathematics becomes an instrument of administration rather than
of science.

## Timing

From Kantorovich's linear programming in 1939 and British operational research
from 1940, through game theory in 1944, Monte Carlo methods from 1946, the
simplex method in 1947 and RAND from 1948, to the Mahalanobis model of 1955-56
and the spread of modelling through planning, military strategy and economic
policy to about 2000.

## Mechanism

**The model replaces the experiment.** A system too large, too costly or too
dangerous to try things on — a national economy, a convoy system, a nuclear
standoff — can be represented formally, and the consequences of a decision
derived from the representation. Linear programming finds the best allocation
under constraints; game theory derives strategy against an adversary who is
also optimising; Monte Carlo simulation estimates outcomes too complicated to
calculate exactly. In each case the answer comes from the model before anyone
acts in the world.

**It extends `legibility` from counting a system to predicting it.** MTH-075
let a state sort what it had recorded; a model lets it see what a decision
would do. Administration gains a new kind of sight, and a new kind of error:
the state sees the model, not the system, and the two diverge wherever
measurement is poor or the model omits what matters.

**Derivation can overrule experience, and that is both the value and the
danger.** The convoy result — larger convoys lose proportionally fewer ships —
was counterintuitive to experienced officers and correct, adopted because the
derivation was convincing. The same authority lets a model's output override
judgement when the model is wrong. When the modelled quantity becomes the
target officials are rewarded on, the model stops describing the system and
starts distorting it.

## Social consequence

A professional class forms whose authority rests on deriving consequences for
governments — operational research teams, RAND analysts, planning-commission
modellers. `intellectual authority` shifts toward people who can build and
interpret models, and decisions move from those with experience of a system to
those with a formal representation of it.

A model can also produce answers its sponsor does not want. Kantorovich's
optimal solutions generated shadow prices that exposed where Soviet planned
prices were wrong, and his work was set aside for two decades because its
output contradicted policy. The method's power to derive consequences is also a
power to derive unwelcome ones, so the value of modelling to a state depends on
whether the state will accept being told it is mistaken.

## Regional specifics

The method is not an American invention with foreign applications. Kantorovich
formulated linear programming in Leningrad before anyone in the West and was
ignored by his own state; British operational research made modelling a
wartime institution; the United States generalised the tools and built RAND;
and India under Mahalanobis made a formal growth model the basis of a national
development plan, one of the most consequential single uses of a model
anywhere. The African absence records the precondition as a negative: a model is
only as good as the measured system beneath it, and a thin inherited statistical
base meant externally supplied models had too little to calibrate against.

## Contested

Whether models can substitute for prices is the socialist calculation debate,
read both ways on the Soviet record. The Mahalanobis model's effect on Indian
growth is disputed. Metric-driven war management shows models degrading when
the measured quantity becomes the goal. Model the method's value as conditional
on measurement quality, on the planner's willingness to accept contrary results,
and on the modelled variable not being the target people are rewarded on.

## Hook

Lets consequences be derived before a decision is taken, extending `legibility`
from counting a system to predicting it. Its value should be conditional on how
well the underlying system is measured, so a state with a thin statistical base
gains little however skilled its modellers. Derivation should be able to overrule
experience, producing correct counterintuitive policy when the model is sound and
confident error when it is not, and it should degrade whenever the modelled
quantity becomes the target officials are rewarded on. It should transfer
`intellectual authority` to modellers. And it should be able to produce results
its sponsor rejects, so the method pays only for a state willing to be told it is
wrong.
