# Grand Strategy Game — Design Wiki

A private design-reference wiki documenting ~300 historical events and technologies
across 12 domains, each one terminating in a game mechanic. This is a **design
document with historical justification**, not an encyclopedia — every entry exists
because it earns a mechanic, not because it's historically interesting on its own.

Read in [Obsidian](https://obsidian.md) with the [Dataview](https://blacksmithgn.github.io/obsidian-dataview/)
plugin. Storage is plain Markdown + YAML frontmatter, versioned in git. The game
engine is undecided — frontmatter stays flat and engine-agnostic (arrays of IDs,
no engine vocabulary) so a conversion script can target whatever engine is chosen
later.

## Layout

```text
/_meta/          Conventions, schema, templates, design grammar — read these first.
/domains/        One folder per domain, one file per entry.
/indexes/        Dataview queries over /domains — never hand-maintained.
```

## Domains

`agriculture` `health` `commerce` `law` `religion` `war` `finance` `engineering`
`sciences` `philosophy` `diplomacy` `migration`

## Start here

1. [_meta/conventions.md](_meta/conventions.md) — IDs, filenames, controlled vocabularies.
2. [_meta/schema.md](_meta/schema.md) — the frontmatter schema, field by field.
3. [_meta/entry-template.md](_meta/entry-template.md) — copy this to start a new entry.
4. [_meta/design-principles.md](_meta/design-principles.md) — the design grammar every entry should obey.
5. [_meta/boundary-rules.md](_meta/boundary-rules.md) — which domain owns what, when two could claim an entry.
6. [domains/commerce/COM-011-silver-shock.md](domains/commerce/COM-011-silver-shock.md) — a full worked example.

## Status

Three domains — agriculture, health, commerce — exist as prior chat output and will
be retrofitted into this structure next. Everything else starts empty.
