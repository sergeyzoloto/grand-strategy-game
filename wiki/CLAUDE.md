# Design wiki: instructions for Claude

This folder is a design-reference wiki (Markdown + YAML frontmatter, read in Obsidian).
It is documentation, not code. The game engine is not chosen; keep frontmatter flat and
engine-agnostic (arrays of IDs, no engine vocabulary such as `trigger`, `modifier`, `effect`).

## Read first

Before creating or editing an entry, read in this order:

1. `_meta/conventions.md`: IDs, filenames, controlled vocabularies.
2. `_meta/schema.md`: every frontmatter field and the body section order.
3. `_meta/entry-template.md`: the starting point for a new entry.
4. `_meta/design-principles.md` and `_meta/boundary-rules.md`: design grammar and domain ownership.

`README.md` still says 13 domains. `_meta/conventions.md` lists 16 codes (including MED and
MTH), and `domains/` has 16 folders. Treat the conventions table as authoritative.

## Entries

- Path: `domains/<domain>/{ID}-{kebab-title}.md`, for example `domains/commerce/COM-011-silver-shock.md`.
- ID: `DOM-###`, with the domain code from the conventions table. Numbers step by 10 within a
  domain, in chronological order of `date_start`. Take a free slot at the right position.
- Never reuse a retired ID (`_meta/retired-ids.md`). Reusing a burned `former_id` needs the
  disambiguation row described in `_meta/conventions.md`.
- Start a new entry from `_meta/entry-template.md`, with `status: skeleton`.
- Frontmatter values:
  - `regions` and every region field use only the controlled vocabulary in `conventions.md`.
    No free text.
  - Dates are integers, negative for BCE (`-9500`). Never strings and never ranges packed into one field.
  - `date_precision` must match the real confidence (`year` | `decade` | `century` | `era`).
  - `contested: true` requires a `## Contested` body section and a fuller source pointer.
  - Any entry whose mechanic rests on a specific number needs a fuller source pointer too.
- Body sections, in this fixed order: `## Timing`, `## Mechanism`, `## Social consequence`,
  `## Regional specifics`, `## Contested` (only when `contested: true`), `## Hook`.
- `## Hook` is mandatory, even at skeleton stage. It is never empty and never "TBD".
  Say what the entry does to the game once mechanised.
- Citation check: if the body names another ID (`\b[A-Z]{2,4}-\d{3}\b`), that ID must also appear
  in `prereqs`, `enables` or `see_also`. Run this check before promoting an entry past `drafted`.
- Each invented game variable gets a line in `_meta/stats-observed.md` when it is coined.

## Do not edit by hand

- `indexes/`: Dataview queries over `domains/`. Never hand-maintain them.
- `_meta/id-migrations.md`: append-only. Log every renumbering run here.
- `_meta/retired-ids.md`: only add to it, when an entry is cut.
- `_meta/scripts/renumber.py`: run `--dry-run` first, and only `--execute` after review.
- `_meta/audits/`: historical audit output.
- `.obsidian/`: Obsidian settings.

## Writing

- Entries are written in the language the author uses for the wiki; the existing entries
  are in English, so write new ones in English unless told otherwise.
- Keep entries focused on the mechanic. An entry exists because it earns a mechanic, not
  because it is interesting history.
- Do not duplicate content across domains. Use `domains_touched` as a pointer.
- Do not commit. Commits are made by the user unless they ask otherwise.
