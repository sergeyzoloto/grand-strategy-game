# Index — By Status

## Counts

```dataview
TABLE WITHOUT ID status AS "Status", length(rows) AS "Count"
FROM "domains"
GROUP BY status
```

## Full listing

```dataview
TABLE id AS "ID", title AS "Title", domain AS "Domain", tier AS "Tier"
FROM "domains"
SORT status ASC, id ASC
```

## Orphan check

Entries nothing points to (not present in any other entry's `prereqs`, `enables`,
or `see_also`) **and** that don't point anywhere themselves (`enables` is empty).
A true dead end — worth a look before calling it `reviewed`.

Relations here are plain ID strings in frontmatter arrays, not Obsidian
`[[wikilinks]]`, so this needs DataviewJS rather than a plain query to check
inbound references across all pages.

```dataviewjs
const pages = dv.pages('"domains"').array();
const referenced = new Set();
for (const p of pages) {
  for (const field of ["prereqs", "enables", "see_also"]) {
    for (const id of (p[field] || [])) referenced.add(id);
  }
}
const orphans = pages.filter(p => !referenced.has(p.id) && (p.enables || []).length === 0);
dv.table(
  ["ID", "Title", "Domain", "Status"],
  orphans.map(p => [p.id, p.title, p.domain, p.status])
);
```
