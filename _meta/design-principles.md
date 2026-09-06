# Design Principles

The project's design grammar. Every entry's Hook should be checkable against these
— if a mechanic doesn't fit any of them, that's worth noticing before it's built.

1. **Exogenous shocks branch on prior institutional state, not on player choice.**
   The player doesn't pick how a famine, plague, or bullion shock resolves — what
   they built beforehand determines the branch. See `COM-011` (silver inflow
   converts to fiscal capacity *or* sovereign default depending on prior credit
   institutions, not a player decision made at the time).

2. **Habitus is acquired through architecture, crops, and work rhythm — never a
   player slider.** Cultural/behavioral traits are downstream of material
   conditions the player shapes indirectly (what gets built, what gets grown, how
   labor is scheduled), not a dial the player turns directly on a population.

3. **Authority and efficacy are separable stats.** A practice or institution can
   carry high prestige and zero actual effect for centuries (premodern medicine:
   high authority, ~zero efficacy, for two millennia) — and the reverse is also
   modelable (something quietly effective with no social standing).

4. **Depleting pools: some late-game capabilities require investment merely to
   hold steady.** Not everything is cumulative growth — some capacities decay by
   default and need standing maintenance spend just to not regress (e.g. an
   antibiotic pool that depletes with use and resistance).

5. **Asymmetry is modelled as difference, not penalty.** Two civilizations lacking
   the same thing for different reasons get different mechanical treatment, not
   the same generic malus. A gap is a distinct shape, not a negative number.

6. **Contested scholarship becomes probabilistic or branching outcomes, not
   scripted fact.** When historians disagree, the disagreement itself becomes part
   of the model — weighted causes, branching resolution, or a probability — rather
   than the entry silently picking a side.

7. **Negative space (what a region lacked) is as mechanically important as what it
   had.** This is why `absence` is a first-class frontmatter field, not an
   afterthought: the region that *didn't* get silver, *didn't* urbanize, *didn't*
   develop double-entry bookkeeping is telling you something that should show up
   in the mechanic, not just in the prose.
