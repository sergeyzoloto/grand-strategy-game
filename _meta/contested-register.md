# Contested Register

Every entry with `contested: true`, pulled live. This is the working list for
"which mechanics still need a branching/probabilistic model instead of a scripted
fact" (design principle 6) — check it before mechanising an entry, not after.

```dataview
TABLE domain AS "Domain", date_start AS "Date", confidence AS "My confidence", contested_note AS "Dispute / conditional sketch"
FROM "domains"
WHERE contested = true
SORT date_start ASC
```

Manual notes on individual disputes (cross-entry patterns, sources worth chasing,
disagreements between entries) can go below — this section is not query-generated
and won't be overwritten by re-running Dataview.

## Notes
