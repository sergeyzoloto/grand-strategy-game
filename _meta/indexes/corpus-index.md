# Corpus Index

> [!note] Live view — the CSV has every field
> These Dataview queries run live against `domains/`, so nothing needs rebuilding.
> They show only a few columns. The full sixteen-column index is
> [corpus-index.csv](corpus-index.csv): id, title, domain, domains_touched, tier,
> status, confidence, dates, prereqs/enables with counts, contested, tags. It is
> **generated** and goes stale when entries change. To regenerate it, run this
> from the repo root:
>
> ```bash
> python3 _meta/scripts/build-index.py
> ```
>
> The script aborts without writing if any entry's frontmatter fails to parse.
> It also reports schema drift against [schema.md](../schema.md).

## Summary by domain

```dataview
TABLE WITHOUT ID
  domain AS "Domain",
  length(rows) AS "Entries",
  length(filter(rows.status, (s) => s = "skeleton")) AS "Skeleton",
  length(filter(rows.status, (s) => s = "drafted")) AS "Drafted",
  length(filter(rows.status, (s) => s = "reviewed")) AS "Reviewed",
  length(filter(rows.status, (s) => s = "mechanised")) AS "Mechanised",
  length(filter(rows.tier, (t) => t = "core")) AS "Core",
  length(filter(rows.tier, (t) => t = "standard")) AS "Standard",
  length(filter(rows.tier, (t) => t = "stub")) AS "Stub",
  length(filter(rows.confidence, (c) => c = "high")) AS "Conf. high",
  length(filter(rows.confidence, (c) => c = "medium")) AS "Conf. medium",
  length(filter(rows.confidence, (c) => c = "low")) AS "Conf. low",
  length(filter(rows.contested, (x) => x = true)) AS "Contested"
FROM "domains"
GROUP BY domain
SORT domain ASC
```

## Master table

```dataview
TABLE WITHOUT ID
  link(file.link, id) AS "ID",
  title AS "Title",
  domain AS "Domain",
  date_start AS "Start",
  status AS "Status",
  confidence AS "Confidence"
FROM "domains"
SORT domain ASC, date_start ASC
```

## Checks

### Orphans: `enables` is empty and no other entry lists this ID in its `prereqs`, `enables` or `see_also`

```dataviewjs
const pages = dv.pages('"domains"');
const inbound = new Set();
for (const p of pages) {
  for (const f of ["prereqs", "enables", "see_also"]) {
    for (const ref of (p[f] ?? [])) {
      if (ref && String(ref) !== String(p.id)) inbound.add(String(ref));
    }
  }
}
const orphans = pages
  .where(p => (p.enables ?? []).length === 0 && !inbound.has(String(p.id)))
  .array()
  .sort((a, b) => String(a.domain).localeCompare(String(b.domain))
    || (a.date_start ?? Infinity) - (b.date_start ?? Infinity));
dv.table(["ID", "Title", "Domain", "Start", "Status"],
  orphans.map(p => [dv.fileLink(p.file.path, false, p.id), p.title, p.domain, p.date_start, p.status]));
```

### Contested entries

```dataview
TABLE WITHOUT ID link(file.link, id) AS "ID", title AS "Title", domain AS "Domain", confidence AS "Confidence", contested_note AS "Dispute"
FROM "domains"
WHERE contested = true
SORT domain ASC, date_start ASC
```

### Confidence low or medium

```dataview
TABLE WITHOUT ID link(file.link, id) AS "ID", title AS "Title", domain AS "Domain", confidence AS "Confidence", status AS "Status"
FROM "domains"
WHERE confidence = "low" OR confidence = "medium"
SORT confidence ASC, domain ASC, date_start ASC
```

### Status not yet mechanised

```dataview
TABLE WITHOUT ID link(file.link, id) AS "ID", title AS "Title", domain AS "Domain", tier AS "Tier", status AS "Status"
FROM "domains"
WHERE status != "mechanised"
SORT domain ASC, date_start ASC
```

### `date_precision: century` where `date_end - date_start` < 150 (a recurring defect)

```dataview
TABLE WITHOUT ID link(file.link, id) AS "ID", title AS "Title", domain AS "Domain", date_start AS "Start", date_end AS "End", (date_end - date_start) AS "Span"
FROM "domains"
WHERE date_precision = "century" AND date_start != null AND date_end != null AND (date_end - date_start) < 150
SORT domain ASC, date_start ASC
```

### `stats_touched` empty

```dataview
TABLE WITHOUT ID link(file.link, id) AS "ID", title AS "Title", domain AS "Domain", status AS "Status"
FROM "domains"
WHERE !stats_touched OR length(stats_touched) = 0
SORT domain ASC, date_start ASC
```
