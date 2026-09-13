---
id: MTH-062
title: Sampling and Statistical Inference
legacy_ref:
domain: mathematics
domains_touched: [finance, health, law, commerce]
tier: core
status: drafted
confidence: medium
date_start: 1786
date_end: 1950
date_precision: decade
regions: [europe, americas, south-asia]
region_variants:
  - region: europe
    date_start: 1786
    note: Laplace estimates the population of France in the 1780s and again around 1802 by counting births for the whole country and multiplying by a ratio of population to births measured in a sample of communes — the first sample-based estimate of a national population, with an attempt to state its error. Kiaer in Norway argues from 1895 for the representative method, surveying a selected part of a population to learn about the whole, and is resisted for three decades by statisticians who held that only complete enumeration was trustworthy.
  - region: europe
    date_start: 1925
    note: Fisher's work at Rothamsted, set out in 1925, makes randomisation the basis of experimental design so that inference from an experiment has a calculable reliability, and Neyman's paper of 1934 establishes random sampling with confidence intervals as the sound method for surveys. Between them the question of how a part can speak for a whole acquires a mathematical answer, which is what finally ends the complete-enumeration objection.
  - region: americas
    date_start: 1936
    note: In the 1936 presidential election the Literary Digest polled over two million people and predicted a Landon victory, while Gallup, with a far smaller but better-designed sample, predicted Roosevelt's win. The Digest's list was drawn from telephone and car owners and skewed wealthy. Design beat size, publicly and decisively, and the United States Census adopted sampling for supplementary questions in 1940.
  - region: south-asia
    date_start: 1931
    note: P. C. Mahalanobis founds the Indian Statistical Institute in 1931, develops large-scale crop-estimation surveys in Bengal, and builds India's National Sample Survey from 1950, among the largest and most sophisticated sample survey systems in the world. It lets a very large, predominantly rural country measure consumption, employment and production continuously without the cost of complete enumeration, and it supplies the data behind the Mahalanobis planning model (MTH-084).
absence:
  - region: africa
    note: A random sample must be drawn from a sampling frame — a list or map of the units being studied — and across much of colonial and early post-independence Africa no adequate frame existed, because enumeration had so often estimated households or taxpayers rather than listing individuals (MTH-075). Without a frame there is nothing to draw a random sample from, so the method's promise of learning about a whole population from a known part could not be kept. What is absent is the frame, which is a precise and separate precondition from the statistical theory.
prereqs: [MTH-040, HLT-270]
enables: [MTH-084]
see_also: [MTH-075, FIN-020, FIN-090, LAW-110]
contested: true
contested_note: "Two disputes. First, the founders of modern statistics — Galton, Pearson and Fisher — were committed eugenicists, and much of the apparatus of correlation, regression and inference was developed in and for that programme. Historians disagree about how far the methods carry that origin and how far they are separable from it; the entry treats the entanglement as the field's founding context rather than a footnote, and models the methods as usable for any purpose while their early institutional sponsors were eugenic. Second, the complete-enumeration objection to sampling was not simply wrong: a sample is only as good as its frame and its design, and famous failures such as the 1936 Literary Digest poll show large biased samples misleading worse than small sound ones. Model sampling's reliability as conditional on frame quality and random design rather than on sample size."
stats_touched: [legibility, information sovereignty]
tags: [sampling, statistical-inference, laplace, kiaer, neyman, fisher, mahalanobis, soviet-census-1937]
---

A state can know a population without counting all of it. Inference from a
carefully chosen part to the whole is the exact counterpart of MTH-075's unit
record — that counts everyone cheaply, this counts a few and infers — and it
makes a state's knowledge of its population cheap enough to be continuous.

## Timing

From Laplace's sample-based estimate of France's population in the 1780s,
through Kiaer's representative method from 1895, Fisher's experimental design
in 1925 and Neyman's sampling theory in 1934, the 1936 polling contest, and the
Indian Statistical Institute from 1931, to India's National Sample Survey in
1950.

## Mechanism

**A known part can stand for an unknown whole, with a calculable error.** If a
sample is drawn at random from a proper list of the population, the mathematics
of probability (MTH-040) says how far its figures are likely to be from the
truth. That turns an estimate from a guess into a measurement with stated
reliability, and it means a state does not have to count every person to know
how many are employed, sick, or poor.

**Design matters more than size.** A large sample drawn from a biased list
misleads confidently; a small random one tells the truth with a known margin.
The 1936 contest between the Literary Digest's two million ballots and Gallup's
small designed sample demonstrated it in public. Randomisation is the working
part of the method, not a technicality.

**It makes `legibility` cheap enough to be continuous.** Complete enumeration is
so expensive that a census happens once a decade. A sample survey can run every
year or every month, so a state learns what is happening now rather than what
was true years ago. That is why India could measure a vast rural economy
continuously, and why sampling feeds directly into planning models (MTH-084).

## Social consequence

**The Soviet census of 1937 is the case that shows what statistical knowledge
does to a state that does not want it.** Stalin had announced in 1934 a
population near 168 million. The census of January 1937 counted about 162
million — a shortfall that recorded the dead of the 1932-33 famine and of
repression. The results were declared defective and suppressed, the census was
denounced as the work of wreckers, and its leading statisticians, including the
head of the central statistical administration, were arrested and shot. A new
census in 1939 produced an acceptable figure.

**Here `legibility` and `information sovereignty` are in direct conflict, and
the state chose to destroy its own sight.** Legibility is the state's ability to
see its population; information sovereignty is its control over what may be
known. The 1937 census gave the state an accurate picture of itself, and the
picture was politically intolerable, so the state suppressed the knowledge and
killed the people who produced it. Nothing else in the corpus carries this: not
a state unable to know its population, but a state able to know it and refusing.

**The damage outlasts the event.** After 1937, every statistician in the system
knew what an unwelcome number could cost, so figures were shaped toward what
was expected. A state that punishes accurate counts gets inaccurate ones from
then on, and it plans on them — which is the failure MTH-084 describes, a model
calibrated on data the state itself corrupted. Sampling does not cause this, but
by making statistical knowledge cheap and frequent it makes the conflict recur
more often, since there is more knowledge to find unacceptable.

## Regional specifics

Laplace and Kiaer supply the method's European origin and Fisher and Neyman its
mathematical foundation; the American 1936 contest shows design beating size in
public. **Mahalanobis and India's National Sample Survey are central, not
peripheral** — among the largest and most sophisticated survey systems anywhere,
built to measure a vast rural economy continuously and feeding the country's
planning. The Soviet 1937 census belongs to this entry's social consequence
rather than to its regional variants, because what it shows is not a regional
variation of the method but what the knowledge the method produces can do to a
state. The African absence is the sampling frame: without a list of units there
is nothing to draw a random sample from.

## Contested

The founders of the field were eugenicists and developed much of its apparatus
for that programme; the entry carries this as founding context and models the
methods as usable for any purpose. The objection to sampling was not simply
wrong, since a poor frame or design misleads worse than a complete count, so
reliability should be conditional on frame and design rather than on size.

## Hook

Lets a state know a population from a random part with a calculable error, so
`legibility` becomes cheap enough to be continuous rather than decennial. Its
reliability should depend on the sampling frame and random design, not on sample
size, so a large biased sample misleads with confidence and a state without a
frame gets nothing from the method. It should put `legibility` into direct
conflict with `information sovereignty` whenever the measured truth is politically
unacceptable, giving a state the option to suppress its own knowledge — and
exercising that option should corrupt every later figure, because the people
producing statistics learn what an unwelcome number costs.
