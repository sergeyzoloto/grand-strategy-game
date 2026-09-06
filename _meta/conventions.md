# Conventions

## IDs

`DOM-###` — domain code, hyphen, zero-padded sequential number in creation order.

IDs are **frozen once assigned**. Never reassign, never renumber, never reuse a
number after deleting an entry. If an entry is cut, its number stays retired.

### Domain codes

| Code | Domain      |
| ---- | ----------- |
| AGR  | agriculture |
| HLT  | health      |
| COM  | commerce    |
| LAW  | law         |
| REL  | religion    |
| WAR  | war         |
| FIN  | finance     |
| ENG  | engineering |
| NAT  | nature      |
| PHI  | philosophy  |
| DIP  | diplomacy   |
| MIG  | migration   |

## Filenames

`{ID}-{kebab-title}.md`

Example: `COM-011-silver-shock.md`

The ID prefix is authoritative; the kebab-title exists for human readability when
browsing the folder. If a title changes later, the filename may be updated to
match — the ID inside the frontmatter is what everything actually links against.

## legacy_ref

Optional frontmatter field that preserves the original label an entry had in the
source chat lists (e.g. `C11`, `D5`, `P9`, `E2`), before domain codes and frozen
IDs existed. Keeps earlier conversations navigable by letting you map "that thing
we called D5" back to its current ID. Omit for entries created directly in this
scheme with no prior chat label.

## Controlled region vocabulary

No free text in `regions` or any region field. Use exactly these values:

```text
europe
mediterranean
middle-east
south-asia
east-asia
southeast-asia
central-asia-steppe
africa
americas
oceania
global
```

`global` means genuinely worldwide or region-indifferent — not "I haven't decided
yet." If an entry is region-indifferent because it hasn't been researched, that's
a gap to close before promoting it past `status: skeleton`.

## date_precision

One of: `year` | `decade` | `century` | `era`

Matches the actual confidence in the date, not the granularity you'd like to have.
A Bronze Age institutional shift gets `era`; don't fake `year` precision you don't
have.

## status

One of: `skeleton` | `drafted` | `reviewed` | `mechanised`

Progression is one-directional in normal use: `skeleton` → `drafted` → `reviewed`
→ `mechanised`. An entry can be sent backward deliberately (e.g. a `reviewed`
entry turns out to need a rewrite), but that's a conscious edit, not a routine
transition.

## tier

One of: `core` | `standard` | `stub`

Editorial weight, independent of `status`. A `stub` can be `reviewed` (i.e. "yes,
this minor entry is correct and complete as a stub"). `core` entries are the ones
that must eventually reach `mechanised`; `stub` entries may never need to.

## Dates

Negative integers for BCE, e.g. `-9500`. Never strings, never `"9500 BCE"`, never
a range packed into one field — use `date_start` / `date_end`.

## Sources

Informal author-surname pointers are the default and are sufficient for most
entries — e.g. `Greif, Maghribi traders`. This is a private design document, not
a publication; full citations are overhead it doesn't need.

**Exception:** any entry with `contested: true`, or any entry that rests its
mechanic on a specific number (a date, a percentage, a population figure), gets a
fuller pointer — enough to relocate the actual claim later (author, work, and
roughly where in it) if the number gets challenged.
