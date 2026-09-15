# Index — By Domain

## Rollup

```dataview
TABLE WITHOUT ID domain AS "Domain", length(rows) AS "Count"
FROM "domains"
GROUP BY domain
SORT domain ASC
```

## Full listing

```dataview
TABLE id AS "ID", title AS "Title", tier AS "Tier", status AS "Status"
FROM "domains"
SORT domain ASC, id ASC
```
