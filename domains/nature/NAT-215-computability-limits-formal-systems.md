---
id: NAT-215
title: Computability and the Limits of Formal Systems
legacy_ref:
domain: nature
domains_touched: [philosophy, engineering]
tier: core
status: drafted
confidence: high
date_start: 1900
date_end: 1936
date_precision: decade
regions: [europe, americas]
region_variants:
  - region: europe
    date_start: 1900
    note: Hilbert poses the programme at Paris in 1900 and sharpens it through the 1920s — mathematics to be shown consistent, complete, and decidable, with the Entscheidungsproblem asking for a procedure that settles any mathematical statement. Godel's incompleteness theorems in 1931 answer the first two negatively from Vienna, and Turing's 1936 paper from Cambridge answers the third. The programme did not stall or run out of funding; it received rigorous answers and they were the opposite of what it asked for.
  - region: americas
    date_start: 1936
    note: Church's lambda calculus at Princeton arrives in the same year as Turing's machine and by a wholly unrelated route, and Post's formulation makes a third. The convergence is the substance of the Church-Turing thesis — several people building different formalisations of effective procedure and finding they capture exactly the same class of functions, which is the closest thing available to evidence that the notion being captured is natural rather than invented.
absence:
  - region: south-asia
    note: The constraint here is institutional and not one of mathematical capacity, and Ramanujan is the case that demonstrates it. A mathematician of the first rank working in isolation produced extraordinary results in analysis and number theory and had to travel to Cambridge to be read at all. A negative result about formal systems is not the kind of thing one person produces, because it requires a community that has agreed on a shared programme, formalised it precisely enough to be refuted, and cares about the refutation — so what is absent is the seminar, the journal, and the standing argument, not the talent.
  - region: east-asia
    note: Japanese mathematics was strong and institutionally well-established by this period, with research universities and journals of real standing, and it engaged very little with foundations because the programme that made foundations urgent was a local European argument about the security of mathematics after the set-theoretic paradoxes. A tradition can hold every institutional prerequisite and still not work on a question it has no reason to find pressing, which is a different kind of absence from lacking the means to work on it.
prereqs: [NAT-014, NAT-030]
enables: []
see_also: [NAT-240, ENG-330, PHI-390, NAT-210, NAT-050, NAT-180]
contested: true
contested_note: "Two live disputes. The Church-Turing thesis is not a theorem and cannot be one — it asserts that an informal notion, effective procedure, is exactly captured by a formal one, and its support is the convergence of independent formalisations plus the failure of anyone to produce a counterexample. Model it as extremely well-supported and formally unprovable, which is a genuinely unusual epistemic status and should not be flattened into either a proved result or a mere conjecture; the stronger physical Church-Turing thesis, that no physical process computes more than a Turing machine, is a substantially more contested claim and should be kept separate. Second, what the incompleteness theorems imply outside mathematics is disputed and very widely overstated. Lucas and later Penrose argued they show human minds are not machines; the standard reply is that the argument equivocates on which formal system the mind is being identified with. Treat the theorems as results about formal systems with no established consequence for cognition, and model claims to the contrary as a recurring philosophical argument rather than a settled finding."
stats_touched: [undecidability ceiling, intellectual authority]
tags: [computability, godel, turing, church, hilbert, entscheidungsproblem, incompleteness]
---

A proof that some questions are answerable by no procedure whatsoever —
not unanswered, but provably unanswerable. NAT-014 establishes what a
procedure is and what it can be delegated to; this establishes what no
procedure can do, and it does so before any machine exists to be limited
by it.

## Timing

Hilbert's programme from 1900, sharpened through the 1920s; Godel's
incompleteness theorems in 1931; Church's lambda calculus and Turing's
machine independently in 1936, and the Church-Turing thesis assembled from
their convergence.

## Mechanism

**The result is negative, and that is what makes it foundational.** Almost
everything else in this domain establishes that something can be known.
This establishes that certain things cannot be computed by any means, ever,
and the bound does not depend on the machine. No improvement in speed,
memory, materials, or ingenuity moves it — which is a kind of limit almost
nothing else in the corpus has, since most ceilings in this record yield to
investment eventually.

**It arrives as the failure of a research programme, and the failure is
the achievement.** Hilbert asked for proofs that mathematics is consistent,
complete, and decidable. Godel showed that any consistent formal system
strong enough to express arithmetic contains true statements it cannot
prove, and cannot prove its own consistency. Turing and Church then showed
the decision problem has no general solution. The programme was not
abandoned or defunded; it was answered rigorously in the negative, and the
answers turned out to be more valuable than the results it had requested.
This is worth modelling directly, because a research programme that fails
informatively is a different object from one that merely stops.

**The limits are established before the machines exist.** This is the real
distinction from NAT-014 and the thing that makes the entry unusual in the
corpus. Ordinarily a technology is built, its limits are discovered in
practice, and theory follows to explain them — which is the sequence
NAT-180's thermodynamics follows after the steam engine. Here the ceiling
is proved in 1936, a decade before any electronic computer runs, and every
machine ever built since has operated inside a boundary that was mapped
before the first one was switched on.

**The convergence is itself evidence.** Turing's machine, Church's lambda
calculus, and Post's formulation were built independently on different
intuitions and define exactly the same class of computable functions. That
several people reaching for effective procedure from different directions
caught the same thing is the support for the Church-Turing thesis — which
remains a thesis, since it equates a formal notion with an informal one and
so cannot be proved.

A caution the entry should carry explicitly: **Turing's machine is a proof
device, not a design.** It was constructed to be simple enough to reason
about, not to be built, and its infinite tape makes it unbuildable in
principle. Its later influence on actual machine architecture is a real but
separate question and belongs to ENG.

## Social consequence

A discipline acquires a settled account of its own boundaries before it
acquires a subject matter, which is a rare sequence. Theoretical computer
science exists as a coherent body of results with open problems for a
decade before there is a computer, and the field that later forms around
the machines inherits a foundation it did not have to build — so its
questions about what is efficiently computable are posed inside a frame
already known to be bounded.

The result also lands badly outside mathematics and keeps landing. The
incompleteness theorems are among the most misappropriated results in the
sciences, recruited to argue for the limits of reason, the impossibility of
mechanised minds, and a great deal else, and the recruitment recurs in
every generation. That an exact result about formal systems is so readily
converted into a general claim about knowledge is itself a social fact
worth carrying — the prestige of a proof transfers far more easily than its
scope.

## Regional specifics

This entry is genuinely narrow and the narrowness should be stated rather
than padded. The work happens in Gottingen, Vienna, Cambridge, and
Princeton because the question was a local one — a crisis about the
security of mathematics that followed the set-theoretic paradoxes, argued
inside a small community that had agreed on a programme precise enough to
be refuted. A negative result of this kind is not the product of individual
brilliance but of a shared formal programme somebody can disprove.

That is why Ramanujan is the right case for the South Asian absence. A
mathematician of the first rank, working outside any such community,
produced extraordinary results in other areas and had to reach Cambridge to
be read. What was missing was the seminar and the standing argument, not
the ability. Japan's case is the complement — institutions fully present,
engagement with foundations minimal, because the question that made
foundations urgent was somebody else's crisis.

## Contested

The Church-Turing thesis cannot be proved. It asserts that an informal
notion is exactly captured by a formal one, and its support is the
convergence of independent formalisations plus the absence of any
counterexample in ninety years. Model it as extremely well-supported and
formally unprovable — an unusual epistemic status that should not be
collapsed into either a theorem or a conjecture. The stronger physical
thesis, that no physical process computes more than a Turing machine, is
considerably more contested and should be kept as a separate claim.

What incompleteness implies outside mathematics is disputed and routinely
overstated. Lucas and later Penrose argued the theorems show minds are not
machines; the standard objection is that the argument equivocates about
which formal system the mind is supposed to be. Model the theorems as
results about formal systems with no established consequence for cognition,
and treat the contrary claims as a recurring philosophical argument rather
than a finding.

## Hook

An `undecidability ceiling` — a hard bound on what any procedure can
resolve, and the only limit in this record that no investment raises. Speed,
memory, materials, and cleverness all move the practical frontier and none
of them moves this, so a civilisation that has proved it knows something
permanent about every machine it will ever build. It should arrive as the
informative failure of a research programme rather than as a discovery,
which makes a well-specified programme valuable even when it is refuted —
a state that funds a precise question and gets a negative answer has bought
something real, unlike one whose question was too vague to be refuted. The
ceiling is established before any machine exists to meet it, inverting this
domain's usual order in which practice precedes theory. And it should be
gated on a specialist community rather than on individual talent, since a
negative result requires a shared formalism somebody can disprove — which
is why institutions rather than mathematicians are the binding input.
