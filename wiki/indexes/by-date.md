# Index — By Date

Chronological sort across all domains. Remember `date_start` is a negative
integer for BCE, so ascending order runs earliest-first correctly without any
special-casing.

```dataview
TABLE id AS "ID", title AS "Title", domain AS "Domain", date_start AS "Start", date_end AS "End", date_precision AS "Precision"
FROM "domains"
SORT date_start ASC
```
