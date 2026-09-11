#!/usr/bin/env python3
"""
Renumber all domain entries into chronological order within each domain.

Two modes:
  --dry-run (default)  Compute the migration map and write
                        _meta/id-migrations-PROPOSED.md. Touches nothing else.
  --execute            Apply the map: rename files, rewrite frontmatter and
                        body ID references, update _meta/, append to
                        _meta/id-migrations.md, and print a verification report.

Python stdlib only — frontmatter is parsed/edited as text, not via a YAML
library, so exact formatting (flow lists, folded scalars, comments) survives
untouched except for the fields this script is explicitly allowed to change.

See _meta/conventions.md, _meta/schema.md, _meta/boundary-rules.md.
"""

import argparse
import datetime
import re
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
DOMAINS_DIR = REPO_ROOT / "domains"
META_DIR = REPO_ROOT / "_meta"

ID_TOKEN_RE = re.compile(r"\b[A-Z]{2,4}-\d{3}\b")
FILENAME_ID_RE = re.compile(r"^([A-Z]{2,4}-\d{3})-(.+)\.md$")
FM_DELIM = "---"

# Domain code order, for stable report/table ordering. Kept in sync with
# conventions.md's domain code table; not load-bearing for correctness.
DOMAIN_CODE_ORDER = [
    "AGR", "HLT", "COM", "LAW", "REL", "WAR", "FIN", "ENG",
    "NAT", "PHI", "DIP", "MIG", "MAR",
]

# _meta files/dirs whose ID references get rewritten in --execute (step 3f).
META_REWRITE_TARGETS = [
    META_DIR / "threads",
    META_DIR / "audits",
    META_DIR / "contested-register.md",
    META_DIR / "stats-observed.md",
    META_DIR / "indexes",
]

# Append-only historical ledgers: NEVER rewritten by the substitution pass.
META_EXCLUDE = {
    META_DIR / "id-migrations.md",
    META_DIR / "id-migrations-PROPOSED.md",
}


class AbortError(Exception):
    pass


class Entry:
    def __init__(self, path, old_id, domain_code, date_start, date_end, title):
        self.path = path
        self.old_id = old_id
        self.domain_code = domain_code
        self.date_start = date_start
        self.date_end = date_end
        self.title = title
        self.new_id = None


def read_frontmatter_bounds(lines):
    """Return (fm_start, fm_end) line indices such that lines[fm_start] and
    lines[fm_end] are the '---' delimiters and lines[fm_start+1:fm_end] is
    the frontmatter body."""
    if not lines or lines[0].rstrip("\n") != FM_DELIM:
        raise AbortError("missing opening '---' frontmatter delimiter")
    for i in range(1, len(lines)):
        if lines[i].rstrip("\n") == FM_DELIM:
            return 0, i
    raise AbortError("missing closing '---' frontmatter delimiter")


def get_field_line(fm_lines, key):
    prefix = f"{key}:"
    for i, line in enumerate(fm_lines):
        if line.startswith(prefix):
            return i, line[len(prefix):].strip()
    return None, None


def parse_int_field(fm_lines, key, path, missing, bad_type):
    idx, value = get_field_line(fm_lines, key)
    if idx is None or value == "":
        missing.append((path, key))
        return None
    if re.fullmatch(r"-?\d+", value):
        return int(value)
    bad_type.append((path, key, value))
    return None


def collect_entries():
    paths = sorted(DOMAINS_DIR.rglob("*.md"))
    entries = []
    missing = []
    bad_type = []

    for path in paths:
        text = path.read_text(encoding="utf-8")
        lines = text.split("\n")
        try:
            fm_start, fm_end = read_frontmatter_bounds(lines)
        except AbortError as e:
            raise AbortError(f"{path}: {e}")
        fm_lines = lines[fm_start + 1:fm_end]

        idx, old_id = get_field_line(fm_lines, "id")
        if idx is None or not old_id:
            raise AbortError(f"{path}: missing 'id' field")
        m = re.fullmatch(r"([A-Z]{2,4})-(\d{3})", old_id.strip())
        if not m:
            raise AbortError(f"{path}: id '{old_id}' does not match DOM-### pattern")
        domain_code = m.group(1)

        date_start = parse_int_field(fm_lines, "date_start", path, missing, bad_type)
        date_end = parse_int_field(fm_lines, "date_end", path, missing, bad_type)

        _, title = get_field_line(fm_lines, "title")

        entries.append(Entry(path, old_id.strip(), domain_code, date_start, date_end, title))

    if missing or bad_type:
        msg_lines = ["Aborting: date fields are not usable for sorting.\n"]
        if missing:
            msg_lines.append("Missing (or empty) required field:")
            for path, key in missing:
                msg_lines.append(f"  {path.relative_to(REPO_ROOT)}: {key}")
        if bad_type:
            msg_lines.append("Non-integer value (string, not a bare int):")
            for path, key, value in bad_type:
                msg_lines.append(f"  {path.relative_to(REPO_ROOT)}: {key} = {value!r}")
        raise AbortError("\n".join(msg_lines))

    return entries


def build_migration_map(entries):
    by_domain = {}
    for e in entries:
        by_domain.setdefault(e.domain_code, []).append(e)

    for code, group in by_domain.items():
        def sort_key(e):
            end = e.date_end if e.date_end is not None else float("inf")
            return (e.date_start, end, e.old_id)
        group.sort(key=sort_key)
        for i, e in enumerate(group):
            e.new_id = f"{code}-{(i + 1) * 10:03d}"

    all_new_ids = [e.new_id for e in entries]
    if len(all_new_ids) != len(set(all_new_ids)):
        seen = set()
        dupes = set()
        for nid in all_new_ids:
            if nid in seen:
                dupes.add(nid)
            seen.add(nid)
        raise AbortError(f"Aborting: ID collision in proposed output: {sorted(dupes)}")

    return by_domain


def kebab_title_from_filename(path):
    m = FILENAME_ID_RE.match(path.name)
    if not m:
        raise AbortError(f"{path}: filename does not match '{{ID}}-{{kebab-title}}.md'")
    return m.group(2)


def write_proposed_report(by_domain, out_path):
    lines = ["# Proposed ID Renumbering\n"]
    lines.append(
        "Dry run only. Nothing has been changed. Review, then re-run with "
        "`--execute` to apply.\n"
    )

    total_before = sum(len(g) for g in by_domain.values())
    total_after = total_before

    ordered_codes = [c for c in DOMAIN_CODE_ORDER if c in by_domain]
    ordered_codes += [c for c in sorted(by_domain) if c not in ordered_codes]

    for code in ordered_codes:
        group = by_domain[code]
        lines.append(f"\n## {code}\n")
        lines.append("| old ID | new ID | date_start | date_end | title |")
        lines.append("|---|---|---|---|---|")
        for e in group:
            title = (e.title or "").strip()
            lines.append(
                f"| {e.old_id} | {e.new_id} | {e.date_start} | {e.date_end} | {title} |"
            )

    lines.append("\n## Summary\n")
    lines.append("| domain | entries before | entries after |")
    lines.append("|---|---|---|")
    for code in ordered_codes:
        n = len(by_domain[code])
        lines.append(f"| {code} | {n} | {n} |")
    lines.append(f"\nTotal before: {total_before}. Total after: {total_after}. Match: "
                  f"{'yes' if total_before == total_after else 'NO'}.")

    out_path.write_text("\n".join(lines) + "\n", encoding="utf-8")


def substitute_ids_in_text(text, id_map):
    return ID_TOKEN_RE.sub(lambda m: id_map.get(m.group(0), m.group(0)), text)


def substitute_ids_in_frontmatter(fm_lines, id_map):
    out = []
    for line in fm_lines:
        if line.startswith("legacy_ref:") or line.startswith("former_ids:"):
            out.append(line)
        else:
            out.append(substitute_ids_in_text(line, id_map))
    return out


def add_former_ids(fm_lines, old_id):
    id_idx, _ = get_field_line(fm_lines, "id")
    if id_idx is None:
        raise AbortError("missing 'id' field while adding former_ids")

    fi_idx, fi_value = get_field_line(fm_lines, "former_ids")
    if fi_idx is not None:
        inner = fi_value.strip()
        if inner.startswith("[") and inner.endswith("]"):
            inner = inner[1:-1]
        items = [x.strip() for x in inner.split(",") if x.strip()]
        if old_id not in items:
            items.append(old_id)
        fm_lines[fi_idx] = f"former_ids: [{', '.join(items)}]"
    else:
        fm_lines.insert(id_idx + 1, f"former_ids: [{old_id}]")
    return fm_lines


def process_entry_file(entry, id_map):
    text = entry.path.read_text(encoding="utf-8")
    entry.old_text = text
    lines = text.split("\n")
    fm_start, fm_end = read_frontmatter_bounds(lines)
    fm_lines = lines[fm_start + 1:fm_end]
    body_lines = lines[fm_end + 1:]

    fm_lines = substitute_ids_in_frontmatter(fm_lines, id_map)
    if entry.old_id != entry.new_id:
        fm_lines = add_former_ids(fm_lines, entry.old_id)

    body_text = substitute_ids_in_text("\n".join(body_lines), id_map)

    new_text = "\n".join([FM_DELIM] + fm_lines + [FM_DELIM]) + "\n" + body_text
    if not new_text.endswith("\n"):
        new_text += "\n"

    kebab = kebab_title_from_filename(entry.path)
    new_filename = f"{entry.new_id}-{kebab}.md"
    new_path = entry.path.parent / new_filename

    new_path.write_text(new_text, encoding="utf-8")
    if new_path != entry.path:
        entry.path.unlink()

    entry.new_path = new_path
    return new_path


def iter_meta_rewrite_files():
    for target in META_REWRITE_TARGETS:
        if target in META_EXCLUDE:
            continue
        if not target.exists():
            continue
        if target.is_file():
            yield target
        else:
            for p in sorted(target.rglob("*.md")):
                if p not in META_EXCLUDE:
                    yield p


def rewrite_meta_files(id_map, run_date):
    banner = f"IDs renumbered {run_date}; see `_meta/id-migrations.md`.\n\n"
    audits_dir = META_DIR / "audits"
    for path in iter_meta_rewrite_files():
        text = path.read_text(encoding="utf-8")
        new_text = substitute_ids_in_text(text, id_map)
        try:
            path.relative_to(audits_dir)
            is_audit = True
        except ValueError:
            is_audit = False
        if is_audit:
            new_text = banner + new_text
        if new_text != text:
            path.write_text(new_text, encoding="utf-8")


def append_migration_map(by_domain, run_date):
    path = META_DIR / "id-migrations.md"
    header = (
        "# ID Migrations\n\n"
        "Append-only log of every chronological renumbering run. This file is "
        "the canonical resolver for a stale ID reference from a prior "
        "conversation, an old audit, or an external note: look up the old ID "
        "here to find what it became, following through multiple runs if "
        "necessary. Never pruned, never rewritten — earlier runs stay exactly "
        "as recorded even after a later run moves the same entry again.\n"
    )

    if path.exists():
        existing = path.read_text(encoding="utf-8")
        run_numbers = [int(m) for m in re.findall(r"^## Run (\d+)", existing, re.MULTILINE)]
        next_run = max(run_numbers) + 1 if run_numbers else 1
    else:
        existing = header
        next_run = 1

    ordered_codes = [c for c in DOMAIN_CODE_ORDER if c in by_domain]
    ordered_codes += [c for c in sorted(by_domain) if c not in ordered_codes]

    section = [f"\n## Run {next_run} — {run_date} — chronological renumber\n"]
    for code in ordered_codes:
        group = by_domain[code]
        section.append(f"\n### {code}\n")
        section.append("| old ID | new ID | title |")
        section.append("|---|---|---|")
        for e in group:
            title = (e.title or "").strip()
            section.append(f"| {e.old_id} | {e.new_id} | {title} |")

    path.write_text(existing.rstrip("\n") + "\n" + "\n".join(section) + "\n", encoding="utf-8")


def verify(id_map, by_domain):
    report = []

    all_entries = [e for group in by_domain.values() for e in group]
    before_counts = {code: len(group) for code, group in by_domain.items()}
    after_paths = sorted(DOMAINS_DIR.rglob("*.md"))
    after_counts = {}
    for p in after_paths:
        m = FILENAME_ID_RE.match(p.name)
        if m:
            code = re.match(r"[A-Z]{2,4}", m.group(1)).group(0)
            after_counts[code] = after_counts.get(code, 0) + 1
    report.append("## Entry counts per domain (before -> after)")
    for code in sorted(before_counts):
        b = before_counts[code]
        a = after_counts.get(code, 0)
        flag = "" if b == a else "  <-- MISMATCH"
        report.append(f"  {code}: {b} -> {a}{flag}")

    valid_ids = {e.new_id for e in all_entries}
    dangling = []
    scan_paths = list(after_paths) + [
        p for p in META_DIR.rglob("*.md") if p not in META_EXCLUDE
    ]
    for p in scan_paths:
        text = p.read_text(encoding="utf-8")
        lines = text.split("\n")
        try:
            fm_start, fm_end = read_frontmatter_bounds(lines)
            _, fi_value = get_field_line(lines[fm_start + 1:fm_end], "former_ids")
            scan_text = text
            if fi_value:
                scan_text = scan_text.replace(f"former_ids: {fi_value}", "former_ids:")
        except AbortError:
            scan_text = text
        for tok in ID_TOKEN_RE.findall(scan_text):
            if tok not in valid_ids:
                dangling.append((p, tok))
    report.append("\n## Dangling ID references (resolve to no current entry)")
    if dangling:
        for p, tok in sorted(set(dangling)):
            report.append(f"  {p.relative_to(REPO_ROOT)}: {tok}")
    else:
        report.append("  none")

    # A blanket "does this old ID string appear anywhere" scan is unusable here:
    # every domain restarts its own numbering at 010, 020, 030..., so an old ID
    # like AGR-020 is, after this very run, almost certainly also some other
    # entry's brand-new, perfectly valid ID. String reuse across domains'
    # sequences is expected, not a leftover. The only precise check is: for
    # each renumbered entry, does its own pre-image's ID-token sequence map
    # 1:1 onto its post-image's ID-token sequence under id_map? That catches
    # a real missed substitution without flagging legitimate reuse.
    def strip_former_ids_field(text):
        return re.sub(r"^former_ids:.*$", "former_ids:", text, flags=re.MULTILINE)

    substitution_errors = []
    for e in all_entries:
        old_text = getattr(e, "old_text", None)
        new_path = getattr(e, "new_path", None)
        if old_text is None or new_path is None:
            continue
        new_text = new_path.read_text(encoding="utf-8")
        old_tokens = ID_TOKEN_RE.findall(strip_former_ids_field(old_text))
        expected = [id_map.get(t, t) for t in old_tokens]
        new_tokens = ID_TOKEN_RE.findall(strip_former_ids_field(new_text))
        if expected != new_tokens:
            substitution_errors.append((e.path, expected, new_tokens))
    report.append("\n## Substitution correctness (per-entry ID-token sequence, pre vs. post)")
    if substitution_errors:
        for p, expected, new_tokens in substitution_errors:
            report.append(f"  {p}: expected {expected} got {new_tokens}")
    else:
        report.append(f"  all {len(all_entries)} entries verified exact")

    mismatches = []
    for p in DOMAINS_DIR.rglob("*.md"):
        m = FILENAME_ID_RE.match(p.name)
        fname_id = m.group(1) if m else None
        text = p.read_text(encoding="utf-8")
        lines = text.split("\n")
        fm_start, fm_end = read_frontmatter_bounds(lines)
        fm_lines = lines[fm_start + 1:fm_end]
        _, fm_id = get_field_line(fm_lines, "id")
        if fname_id != (fm_id.strip() if fm_id else None):
            mismatches.append((p, fname_id, fm_id))
    report.append("\n## Filename / frontmatter id disagreements")
    if mismatches:
        for p, fname_id, fm_id in mismatches:
            report.append(f"  {p.relative_to(REPO_ROOT)}: filename={fname_id} frontmatter={fm_id}")
    else:
        report.append("  none")

    report.append("\n## Prereq/enables reciprocity")
    report.append(
        "  (checked structurally: this run only renumbers IDs and rewrites "
        "references 1:1, it never adds/removes a prereqs/enables entry, so "
        "reciprocity is preserved by construction — see script logic, no "
        "prereqs/enables list is ever mutated in content, only ID tokens "
        "inside them are substituted.)"
    )

    report.append("\n## Prereqs pointing to a later ID within the same domain")
    id_lookup = {e.new_id: e for e in all_entries}
    flagged = []
    for p in DOMAINS_DIR.rglob("*.md"):
        text = p.read_text(encoding="utf-8")
        lines = text.split("\n")
        fm_start, fm_end = read_frontmatter_bounds(lines)
        fm_lines = lines[fm_start + 1:fm_end]
        _, self_id = get_field_line(fm_lines, "id")
        self_id = self_id.strip() if self_id else None
        _, prereqs_val = get_field_line(fm_lines, "prereqs")
        if not prereqs_val or not self_id:
            continue
        inner = prereqs_val.strip().strip("[]")
        prereq_ids = [x.strip() for x in inner.split(",") if x.strip()]
        self_domain = re.match(r"[A-Z]{2,4}", self_id).group(0)
        self_num = int(self_id.split("-")[1])
        for pid in prereq_ids:
            pd_match = re.match(r"[A-Z]{2,4}", pid)
            if not pd_match or pd_match.group(0) != self_domain:
                continue
            p_num = int(pid.split("-")[1])
            if p_num > self_num:
                flagged.append((p, self_id, pid))
    if flagged:
        for p, self_id, pid in flagged:
            report.append(f"  {self_id} (in {p.relative_to(REPO_ROOT)}) lists prereq {pid}, which is numbered later")
    else:
        report.append("  none")

    return "\n".join(report)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    mode = parser.add_mutually_exclusive_group()
    mode.add_argument("--dry-run", action="store_true", help="default; compute and report only")
    mode.add_argument("--execute", action="store_true", help="apply the renumbering")
    args = parser.parse_args()

    try:
        entries = collect_entries()
        by_domain = build_migration_map(entries)
    except AbortError as e:
        print(str(e), file=sys.stderr)
        sys.exit(1)

    if not args.execute:
        out_path = META_DIR / "id-migrations-PROPOSED.md"
        write_proposed_report(by_domain, out_path)
        print(f"Dry run complete. Nothing changed. See {out_path.relative_to(REPO_ROOT)}")
        return

    id_map = {e.old_id: e.new_id for group in by_domain.values() for e in group}
    run_date = datetime.date.today().isoformat()

    for group in by_domain.values():
        for e in group:
            process_entry_file(e, id_map)

    rewrite_meta_files(id_map, run_date)
    append_migration_map(by_domain, run_date)

    proposed_path = META_DIR / "id-migrations-PROPOSED.md"
    if proposed_path.exists():
        proposed_path.unlink()

    print(f"Executed renumbering run for {run_date}.\n")
    print(verify(id_map, by_domain))


if __name__ == "__main__":
    main()
