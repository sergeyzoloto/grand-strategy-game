#!/usr/bin/env python3
"""Build the corpus-wide index of every entry.

Walks domains/**/*.md, parses each entry's YAML frontmatter, and writes
_meta/indexes/corpus-index.csv (UTF-8 with BOM, sorted by domain then
date_start). Also reports schema drift: frontmatter fields present in the
corpus but not in _meta/schema.md, and schema fields absent from every entry.

Python stdlib only. The frontmatter parser handles the YAML subset the corpus
uses (plain/quoted scalars, flow sequences that may span lines, block
sequences of one-level mappings) and raises on anything else. Any entry that
fails to parse aborts the whole run before anything is written — a missing
row in an index is worse than an error.

Idempotent; safe to re-run.

Usage:
    python3 _meta/scripts/build-index.py
"""

import csv
import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
DOMAINS_DIR = REPO_ROOT / "domains"
SCHEMA_PATH = REPO_ROOT / "_meta" / "schema.md"
CSV_PATH = REPO_ROOT / "_meta" / "indexes" / "corpus-index.csv"

FM_DELIM = "---"

COLUMNS = [
    "id", "title", "domain", "domains_touched", "tier", "status", "confidence",
    "date_start", "date_end", "date_precision",
    "prereq_count", "prereqs", "enables_count", "enables",
    "contested", "tags",
]
ARRAY_FIELDS = {"domains_touched", "prereqs", "enables", "tags"}
INT_FIELDS = {"date_start", "date_end"}
BOOL_FIELDS = {"contested"}
COUNT_OF = {"prereq_count": "prereqs", "enables_count": "enables"}


class ParseError(Exception):
    pass


# --------------------------------------------------------------------------
# Minimal YAML-subset parser
# --------------------------------------------------------------------------

KEY_RE = re.compile(r"^([A-Za-z_][A-Za-z0-9_-]*):(?:\s+(.*)|)$")


def strip_comment(text):
    """Remove a trailing ' #' comment outside quotes."""
    in_dq = in_sq = False
    prev = " "
    for i, ch in enumerate(text):
        if ch == '"' and not in_sq and (i == 0 or text[i - 1] != "\\"):
            in_dq = not in_dq
        elif ch == "'" and not in_dq:
            in_sq = not in_sq
        elif ch == "#" and not in_dq and not in_sq and prev.isspace():
            return text[:i].rstrip()
        prev = ch
    return text.rstrip()


def parse_quoted(text):
    """Parse a whole quoted scalar; raise if anything trails the closing quote."""
    q = text[0]
    out = []
    i = 1
    while i < len(text):
        ch = text[i]
        if q == '"' and ch == "\\":
            if i + 1 >= len(text):
                break
            nxt = text[i + 1]
            out.append({"n": "\n", "t": "\t", '"': '"', "\\": "\\", "/": "/"}.get(nxt, "\\" + nxt))
            i += 2
            continue
        if ch == q:
            if q == "'" and i + 1 < len(text) and text[i + 1] == "'":
                out.append("'")
                i += 2
                continue
            if text[i + 1:].strip():
                raise ParseError(f"unexpected text after closing quote: {text!r}")
            return "".join(out)
        out.append(ch)
        i += 1
    raise ParseError(f"unterminated quoted string: {text!r}")


def parse_scalar(text):
    text = text.strip()
    if text == "" or text in ("~", "null"):
        return None
    if text[0] in "\"'":
        return parse_quoted(text)
    if text[0] in "[{>|&*!%@`":
        raise ParseError(f"unsupported YAML construct: {text!r}")
    if text in ("true", "false"):
        return text == "true"
    if re.fullmatch(r"-?\d+", text):
        return int(text)
    return text


def split_flow(inner):
    """Split the inside of a flow sequence on top-level commas."""
    items, buf = [], []
    in_dq = in_sq = False
    for i, ch in enumerate(inner):
        if ch == '"' and not in_sq and (i == 0 or inner[i - 1] != "\\"):
            in_dq = not in_dq
        elif ch == "'" and not in_dq:
            in_sq = not in_sq
        elif ch in "[]{}" and not in_dq and not in_sq:
            raise ParseError(f"nested flow collections are not supported: [{inner}]")
        if ch == "," and not in_dq and not in_sq:
            items.append("".join(buf))
            buf = []
        else:
            buf.append(ch)
    if in_dq or in_sq:
        raise ParseError(f"unterminated quote in flow sequence: [{inner}]")
    items.append("".join(buf))
    # Allow a trailing comma; reject empty items elsewhere.
    if items and items[-1].strip() == "":
        items.pop()
    if any(it.strip() == "" for it in items):
        raise ParseError(f"empty item in flow sequence: [{inner}]")
    return [parse_scalar(it) for it in items]


def parse_flow(text):
    text = text.strip()
    if not (text.startswith("[") and text.endswith("]")):
        raise ParseError(f"malformed flow sequence: {text!r}")
    return split_flow(text[1:-1])


def parse_frontmatter(fm_lines):
    """Parse frontmatter lines into a dict. Raises ParseError with a line number."""
    data = {}
    i = 0
    n = len(fm_lines)

    def err(msg, idx):
        raise ParseError(f"frontmatter line {idx + 2}: {msg}")

    while i < n:
        raw = fm_lines[i]
        line = strip_comment(raw)
        if not line.strip():
            i += 1
            continue
        if raw[0].isspace() or raw.startswith("-"):
            err(f"unexpected indented or sequence line at top level: {raw!r}", i)
        m = KEY_RE.match(line)
        if not m:
            err(f"not a 'key: value' line: {raw!r}", i)
        key, value = m.group(1), (m.group(2) or "").strip()
        if key in data:
            err(f"duplicate key {key!r}", i)
        start = i
        i += 1

        if value.startswith("["):
            # Flow sequence, possibly spanning lines.
            buf = value
            while not strip_comment(buf).rstrip().endswith("]"):
                if i >= n:
                    err(f"unterminated flow sequence for {key!r}", start)
                buf += " " + strip_comment(fm_lines[i]).strip()
                i += 1
            try:
                data[key] = parse_flow(buf)
            except ParseError as e:
                err(f"{key}: {e}", start)
            continue

        if value != "":
            try:
                data[key] = parse_scalar(value)
            except ParseError as e:
                err(f"{key}: {e}", start)
            continue

        # Empty value: either null, or a block that follows on indented lines.
        block = []
        while i < n and (not fm_lines[i].strip() or fm_lines[i][0].isspace()):
            block.append((i, fm_lines[i]))
            i += 1
        while block and not block[-1][1].strip():
            block.pop()
        if not block:
            data[key] = None
            continue

        first_idx, first = block[0]
        stripped = strip_comment(first).strip()
        if stripped.startswith("["):
            buf = " ".join(strip_comment(l).strip() for _, l in block)
            try:
                data[key] = parse_flow(buf)
            except ParseError as e:
                err(f"{key}: {e}", first_idx)
        elif stripped.startswith("- ") or stripped == "-":
            data[key] = parse_block_sequence(block, key, err)
        else:
            err(f"{key}: nested mappings are not supported: {first!r}", first_idx)
    return data


def parse_block_sequence(block, key, err):
    items = []
    item_indent = None
    current = None
    field_indent = None
    for idx, raw in block:
        line = strip_comment(raw)
        if not line.strip():
            continue
        indent = len(line) - len(line.lstrip())
        body = line.strip()
        if body.startswith("- ") or body == "-":
            if item_indent is None:
                item_indent = indent
            if indent != item_indent:
                err(f"{key}: inconsistent sequence indentation", idx)
            rest = body[1:].strip()
            m = KEY_RE.match(rest)
            if m:
                current = {}
                items.append(current)
                field_indent = indent + 2
                try:
                    current[m.group(1)] = parse_scalar(m.group(2) or "")
                except ParseError as e:
                    err(f"{key}: {e}", idx)
            else:
                current = None
                try:
                    items.append(parse_scalar(rest))
                except ParseError as e:
                    err(f"{key}: {e}", idx)
        else:
            if current is None or indent != field_indent:
                err(f"{key}: unexpected line in sequence: {raw!r}", idx)
            m = KEY_RE.match(body)
            if not m:
                err(f"{key}: not a 'key: value' line: {raw!r}", idx)
            if m.group(1) in current:
                err(f"{key}: duplicate key {m.group(1)!r} in item", idx)
            try:
                current[m.group(1)] = parse_scalar(m.group(2) or "")
            except ParseError as e:
                err(f"{key}: {e}", idx)
    return items


def read_entry(path):
    text = path.read_text(encoding="utf-8")
    if text.startswith("﻿"):
        text = text[1:]
    lines = text.split("\n")
    if not lines or lines[0].rstrip("\r") != FM_DELIM:
        raise ParseError("missing opening '---' frontmatter delimiter")
    for end in range(1, len(lines)):
        if lines[end].rstrip("\r") == FM_DELIM:
            break
    else:
        raise ParseError("missing closing '---' frontmatter delimiter")
    fm = parse_frontmatter([l.rstrip("\r") for l in lines[1:end]])
    if not fm:
        raise ParseError("empty frontmatter")
    return fm


# --------------------------------------------------------------------------
# Row building
# --------------------------------------------------------------------------

def validate_types(fm):
    """Return type errors for fields whose shape the index depends on."""
    problems = []
    for f in ARRAY_FIELDS:
        if f in fm and fm[f] is not None and not isinstance(fm[f], list):
            problems.append(f"{f} must be an array, got {fm[f]!r}")
    for f in INT_FIELDS:
        v = fm.get(f)
        if v is not None and (isinstance(v, bool) or not isinstance(v, int)):
            problems.append(f"{f} must be an integer, got {v!r}")
    for f in BOOL_FIELDS:
        v = fm.get(f)
        if v is not None and not isinstance(v, bool):
            problems.append(f"{f} must be true/false, got {v!r}")
    for f in ("id", "domain"):
        if not fm.get(f):
            problems.append(f"{f} is missing or empty (needed to key and sort the index)")
    return problems


def cell(value):
    if value is None:
        return ""
    if isinstance(value, bool):
        return "TRUE" if value else "FALSE"
    if isinstance(value, list):
        return "; ".join("" if v is None else cell(v) for v in value)
    return str(value)


def build_row(fm):
    row = {}
    for col in COLUMNS:
        if col in COUNT_OF:
            arr = fm.get(COUNT_OF[col])
            row[col] = len(arr) if isinstance(arr, list) else ""
        else:
            row[col] = cell(fm.get(col))
    return row


def sort_key(fm):
    ds = fm.get("date_start")
    return (fm["domain"], ds is None, ds if ds is not None else 0, fm["id"])


# --------------------------------------------------------------------------
# Schema drift
# --------------------------------------------------------------------------

def schema_fields():
    """Top-level and nested field names from the yaml block in schema.md."""
    text = SCHEMA_PATH.read_text(encoding="utf-8")
    m = re.search(r"## Frontmatter\s+```yaml\n(.*?)\n```", text, re.S)
    if not m:
        raise ParseError(f"{SCHEMA_PATH}: no ```yaml block under '## Frontmatter'")
    top, nested = set(), {}
    parent = None
    for line in m.group(1).split("\n"):
        mt = re.match(r"^([A-Za-z_][\w-]*):", line)
        if mt:
            parent = mt.group(1)
            top.add(parent)
            continue
        mn = re.match(r"^\s+(?:-\s+)?([A-Za-z_][\w-]*):", line)
        if mn and parent:
            nested.setdefault(parent, set()).add(mn.group(1))
    return top, nested


def corpus_fields(entries):
    top, nested = {}, {}
    for _, fm in entries:
        for k, v in fm.items():
            top[k] = top.get(k, 0) + 1
            if isinstance(v, list):
                for item in v:
                    if isinstance(item, dict):
                        for sub in item:
                            nested.setdefault(k, {}).setdefault(sub, 0)
                            nested[k][sub] += 1
    return top, nested


# --------------------------------------------------------------------------

def main():
    paths = sorted(DOMAINS_DIR.rglob("*.md"))
    if not paths:
        print(f"ERROR: no entries found under {DOMAINS_DIR}", file=sys.stderr)
        return 1

    entries, failures = [], []
    for path in paths:
        rel = path.relative_to(REPO_ROOT)
        try:
            fm = read_entry(path)
        except (ParseError, UnicodeDecodeError) as e:
            failures.append((rel, str(e)))
            continue
        problems = validate_types(fm)
        if problems:
            failures.extend((rel, p) for p in problems)
            continue
        entries.append((rel, fm))

    if failures:
        print(f"ERROR: {len(failures)} problem(s) parsing entries; nothing written.", file=sys.stderr)
        for rel, msg in failures:
            print(f"  {rel}: {msg}", file=sys.stderr)
        return 1

    entries.sort(key=lambda e: sort_key(e[1]))

    CSV_PATH.parent.mkdir(parents=True, exist_ok=True)
    tmp = CSV_PATH.with_suffix(".csv.tmp")
    with tmp.open("w", encoding="utf-8-sig", newline="") as fh:
        writer = csv.DictWriter(fh, fieldnames=COLUMNS, lineterminator="\r\n")
        writer.writeheader()
        for _, fm in entries:
            writer.writerow(build_row(fm))
    tmp.replace(CSV_PATH)

    # ---- report ----
    print(f"Wrote {CSV_PATH.relative_to(REPO_ROOT)}")
    print(f"\nTotal entries: {len(entries)}")
    per_domain = {}
    for _, fm in entries:
        per_domain[fm["domain"]] = per_domain.get(fm["domain"], 0) + 1
    for d in sorted(per_domain):
        print(f"  {d:<16} {per_domain[d]}")

    print("\nParse failures: none")

    ids = {}
    for rel, fm in entries:
        ids.setdefault(fm["id"], []).append(str(rel))
    dupes = {k: v for k, v in ids.items() if len(v) > 1}
    if dupes:
        print("\nWARNING: duplicate ids:")
        for k, v in sorted(dupes.items()):
            print(f"  {k}: {', '.join(v)}")

    s_top, s_nested = schema_fields()
    c_top, c_nested = corpus_fields(entries)
    print("\nSchema drift")
    extra = sorted(set(c_top) - s_top)
    print("  Fields in corpus but not in schema.md:")
    print("    top-level: " + (", ".join(f"{k} ({c_top[k]} entries)" for k in extra) if extra else "none"))
    for parent in sorted(c_nested):
        extra_sub = sorted(set(c_nested[parent]) - s_nested.get(parent, set()))
        if extra_sub:
            where = sorted({str(rel) for rel, fm in entries
                            if any(isinstance(it, dict) and set(it) & set(extra_sub)
                                   for it in (fm.get(parent) or []))})
            print(f"    nested under {parent}: " + ", ".join(f"{k} ({c_nested[parent][k]} items)" for k in extra_sub)
                  + " — in " + ", ".join(where))
    absent = sorted(s_top - set(c_top))
    print("  Schema fields absent from every entry:")
    print("    top-level: " + (", ".join(absent) if absent else "none"))
    for parent in sorted(s_nested):
        if parent in c_nested:
            missing_sub = sorted(s_nested[parent] - set(c_nested[parent]))
            if missing_sub:
                print(f"    nested under {parent}: " + ", ".join(missing_sub))
    partial = sorted((k, c) for k, c in c_top.items() if k in s_top and c < len(entries))
    if partial:
        print("  Schema fields present in only some entries: " + ", ".join(f"{k} ({c}/{len(entries)})" for k, c in partial))
    return 0


if __name__ == "__main__":
    sys.exit(main())
