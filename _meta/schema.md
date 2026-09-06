# Schema

Every entry is Markdown with a YAML frontmatter block followed by body sections in
a fixed order. Frontmatter stays **engine-agnostic**: flat keys, arrays of IDs,
no nesting beyond one level, no engine vocabulary (no `trigger`, `modifier`,
`effect`, etc.). A conversion script will translate this into whatever engine is
eventually chosen.

## Frontmatter

```yaml
id: COM-011
title:
legacy_ref:            # optional, e.g. "C11"
domain: commerce       # exactly one — canonical, decides folder and ID prefix, permanent
domains_touched: []    # other domains this entry matters to, without owning it
tier: core
status: skeleton
confidence: high       # high | medium | low — MY certainty, distinct from `contested`
date_start:
date_end:
date_precision: decade
regions: []
region_variants:       # same phenomenon, different dates/character by region
  - region:
    date_start:
    note:
absence:               # where this did NOT happen and why — negative space is load-bearing
  - region:
    note:
prereqs: []            # IDs
enables: []            # IDs
see_also: []           # IDs
contested: false
contested_note: ""     # state the dispute AND how to model it as a conditional
stats_touched: []      # free text during coverage; consolidated later
tags: []
```

### Field notes

- **id** — see [conventions.md](conventions.md). Frozen forever.
- **title** — human-readable name of the entry.
- **legacy_ref** — optional bridge to the original chat-list label. See conventions.md.
- **domain** — exactly one of the 13 domain names (lowercase, matches the folder and
  the ID prefix). Canonical and permanent — see [boundary-rules.md](boundary-rules.md)
  for how to pick it when an entry has a foot in two domains.
- **domains_touched** — any number of domain names this entry matters to without
  owning it. Content is never duplicated across domains: this is a pointer, not a
  second copy. If the "touched" domain's angle grows substantial enough to need its
  own `## Mechanism`, that's a sign the entry should be split in two (see
  boundary-rules.md) rather than carried as a touch.
- **tier** — editorial weight: `core` | `standard` | `stub`.
- **status** — pipeline stage: `skeleton` | `drafted` | `reviewed` | `mechanised`.
- **confidence** — how sure *I* am the entry is factually right (`high` | `medium` |
  `low`). This is orthogonal to `contested`: a contested claim can still be one I'm
  highly confident is the best-supported reading, and a claim I'm personally unsure
  of might not be contested in the literature at all — it's just a gap in my own
  reading.
- **date_start / date_end** — negative integers for BCE. `date_end` may equal
  `date_start` for a point event, or be omitted for an ongoing/open-ended process.
- **date_precision** — `year` | `decade` | `century` | `era`.
- **regions** — flat array from the controlled vocabulary in conventions.md. This is
  the entry's general footprint.
- **region_variants** — use when the *same phenomenon* plays out differently by
  region: different date, different character, different intensity. Each item is
  `{region, date_start, note}`. This is not for unrelated regional entries — those
  are separate entries.
- **absence** — where this phenomenon did *not* occur, and why that's meaningful.
  Each item is `{region, note}`. Negative space is load-bearing (design principle 7)
  — an absence entry should explain what a region had *instead*, or what it lacked
  that made the difference, not just note a blank.
- **prereqs / enables / see_also** — arrays of other entries' IDs. `prereqs` and
  `enables` are directional (this requires / this unlocks); `see_also` is a
  non-directional cross-reference (boundary neighbor, close analogue, contrast case).
- **contested** — boolean. If true, the entry must have a `## Contested` body section.
- **contested_note** — one string covering two things: what the actual scholarly
  dispute is, *and* a sketch of how to turn that dispute into a conditional or
  probabilistic model rather than picking a side (design principle 6). Expand on
  both in the `## Contested` body section; this field is the compressed pointer.
- **stats_touched** — free text, not a controlled vocabulary, during the coverage
  pass. Every invented variable also gets logged in
  [stats-observed.md](stats-observed.md) as it's coined. Consolidation into a real
  glossary happens after coverage, not per-entry.
- **tags** — free text, loose. Not load-bearing for any index; a convenience for
  Obsidian's tag pane and ad-hoc search.

## Body sections

Fixed order, always these headings (omit `## Contested` only when `contested: false`):

```markdown
## Timing
## Mechanism
## Social consequence
## Regional specifics
## Contested
## Hook
```

- **Timing** — when, precisely as the `date_precision` allows.
- **Mechanism** — the economist's read: what cost, constraint, or incentive moves.
- **Social consequence** — the microsociologist's read: household, status,
  obligation — what changes for the people living through it, not just the ledger.
- **Regional specifics** — prose expansion of `region_variants` / `absence`.
- **Contested** — the dispute in full, and the conditional/branching model that
  represents it. Omit the heading entirely when `contested: false`.
- **Hook** — **mandatory, even at skeleton stage.** The mechanical seed: what this
  entry actually does to the game once mechanised. During coverage passes this can
  be one sentence and need not specify trigger/condition/modifier values yet — but
  it is never empty, never "TBD". An entry with no mechanic is not an entry.

## Skeleton-stage entries

An entry with `status: skeleton` carries only: full frontmatter, a one-line
description under the title, and a Hook seed. The other body sections can be
added when the entry is promoted to `drafted`.
