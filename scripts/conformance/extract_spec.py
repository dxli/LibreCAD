#!/usr/bin/env python3
"""extract_spec.py — Extract normative field-table rows from the ODA §20.4 spec.

Reads libraries/libdxfrw/spec/dwg_spec.txt (developer-local, gitignored,
sha256-pinned in SOURCES.json) and emits scripts/conformance/spec_fields.json
containing per-unit row lists.

Round-3 R.1 licensing: verbatim `descText` prose is committed only as a
sha256 hash, never as prose (see --descText-mode). The paraphrase mode is a
placeholder for the eventual maintainer-signed-off inclusion.

Round-3 B2 substrate check: hard-fails on the wrong 14992-vs-31865-line
rendering. Identity check: line count == 14992 and line 10471 startswith
'20.4.75 HATCH'.

Row identity: `(unit, row_ordinal)` per plan §8.2. Structural cross-refs
(Common Entity Data / Common Entity Handle Data / CRC) emit as
`{kind: 'xref'}` records so downstream ledger builds can join them.

Usage:
    extract_spec.py                             # writes spec_fields.json
    extract_spec.py --unit 20.4.75              # dump one unit as JSON
    extract_spec.py --selfcheck                 # pilot count assertions
"""
from __future__ import annotations
import argparse
import hashlib
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SPEC_TEXT = REPO / "libraries" / "libdxfrw" / "spec" / "dwg_spec.txt"
OUT_JSON = REPO / "scripts" / "conformance" / "spec_fields.json"

# Round-3 B2 identity assertions.
EXPECTED_LINE_COUNT = 14992
EXPECTED_HATCH_LINE = 10471
EXPECTED_HATCH_PREFIX = "20.4.75 HATCH"

# Unit heading, e.g. "20.4.75 HATCH (varies)" or "20.4.53 (UNDOCUMENTED)".
UNIT_RE = re.compile(r"^\s*20\.4\.(\d+)\b\s*(.*)$")
# Sub-heading, e.g. "20.4.75.1 Example:" - marks fend (field-end).
SUBHEAD_RE = re.compile(r"^\s*20\.4\.\d+\.\d+\b")

# Version/context gate lines. Round-3 correction: keep '&' in class, and allow
# multi-word constructs like 'R13 & R14 Only', 'Until R14', 'R2000 & later'.
GATE_RE = re.compile(
    r"^\s*(R[\d][\dA-Za-z+\-.& ]*(?: Only)?|Common|Until .*|If .*)\s*:\s*$"
)

# DWG type enum from ODA §2.x. Order matters for the alternation.
DWG_TYPES = [
    "3DPOINT", "3BD", "3RD",
    "2DPOINT", "2BD", "2RD",
    "BLL", "BLd", "BSd",  # signed / long variants
    "BB", "BS", "BL", "BD",
    "CMC", "ENC",
    "MC", "MS",
    "OT",
    "RC", "RD", "RL", "RS",
    "TV", "TU", "T",
    "H",
    "B",
    "X",
]
TYPE_ALT = "|".join(DWG_TYPES)

# Field row shape:  <name> <TYPE> [CODE] [DESC]  where CODE is 1-4 digits or a
# small compound like 63,421 or 43/44 or -3.
#
# Detection strategy:
#   (a) MULTIWORD_RE — multi-word name + 2+ spaces to TYPE (the strict form,
#       does not risk absorbing "Start" as a bogus type).
#   (b) SINGLEWORD_RE — single-word name (no internal spaces) + 1+ spaces
#       to TYPE (catches "scaleorspacing BD 41").
#   (c) _token_split_field() — rightmost-TYPE-token fallback that handles the
#       "multi-word name + 1 space to TYPE" case ("Default lighting type RC 282").
#       This runs only if (a) and (b) both miss.
#
# All three allow indent==0 (some HATCH lines start at column 0 in the layout).
FIELD_RE_MULTIWORD = re.compile(
    rf"^(?P<indent>\s*)(?P<name>[A-Za-z0-9#()\-][A-Za-z0-9#().'/ +\-]*?)\s{{2,}}(?P<type>{TYPE_ALT})\b(?:\s+(?P<code>-?\d[\d,/]*))?(?:\s+(?P<desc>.*))?$"
)
FIELD_RE_SINGLEWORD = re.compile(
    rf"^(?P<indent>\s*)(?P<name>[A-Za-z0-9#()][A-Za-z0-9#().'/+\-]*)\s+(?P<type>{TYPE_ALT})\b(?:\s+(?P<code>-?\d[\d,/]*))?(?:\s+(?P<desc>.*))?$"
)
_TYPE_SET = set(DWG_TYPES)
_CODE_RE = re.compile(r"^-?\d[\d,/]*$")


def _token_split_field(line: str) -> dict | None:
    """Rightmost-TYPE-token fallback.

    Given a stripped line, find the RIGHTMOST whitespace-separated token that
    is exactly a DWG type. Everything left = name, right of it = [code, desc].
    Returns None if no such token exists, or if the name would be empty, or
    if what follows the type isn't plausibly a code+desc.
    """
    stripped = line.lstrip()
    indent = line[: len(line) - len(stripped)]
    tokens = stripped.split()
    if len(tokens) < 2:
        return None
    idx = None
    for i in range(len(tokens) - 1, 0, -1):  # skip index 0 (the first token can't be the type alone)
        if tokens[i] in _TYPE_SET:
            idx = i
            break
    if idx is None:
        return None
    name = " ".join(tokens[:idx]).strip()
    if not name:
        return None
    # Guard: name must not itself contain a bare TYPE token — that would mean
    # the true type is one we already found; we picked the rightmost, so OK.
    typ = tokens[idx]
    code = None
    desc = None
    tail = tokens[idx + 1:]
    if tail and _CODE_RE.match(tail[0]):
        code = tail[0]
        if len(tail) > 1:
            desc = " ".join(tail[1:])
    elif tail:
        desc = " ".join(tail)
    return {"indent": indent, "name": name, "type": typ, "code": code, "desc": desc}

# Structural cross-reference blocks.
XREF_RE = re.compile(
    r"^\s+(Common Entity Data|Common Entity Handle Data|Common Non Entity Data|Common non entity data)\s*$"
)
CRC_RE = re.compile(r"^\s+CRC\s+X\s+---.*$")

# Noise: pdftotext -layout page-break headers/footers.
PAGE_HEADER_RE = re.compile(r"^\s*Open Design Specification for \.dwg files")

# Control-flow line prefixes (NOT field rows). Matched with a whole-line
# regex — never against the first word alone, because field names may
# legitimately start with "Start" / "End" (SPLINE, HATCH: "Start tangent",
# "End tangent"). Round-3 lesson: the pilot's naive first-word skip drops
# 3 HATCH rows silently.
CTRL_LINE_RE = re.compile(
    r"^\s*("
    r"Repeat\b.*times\s*:?"
    r"|Begin\s+repeat\b.*times\s*:?"
    r"|End\s+repeat\b"
    r"|End\s+of\s+repeat\b"
    r"|else\s+if\b"
    r"|else\s*\{?"
    r"|if\s*\(.*\)\s*\{?"
    r"|if!\("
    r"|otherwise\b"
    r"|endif\b"
    r"|then\b"
    r")\s*$", re.IGNORECASE)

# Gate whitelist — sanctioned members. Any observed gate NOT in this set that
# also doesn't match a numbered variant fires a hard-fail. Round-3 M2/gate
# discipline.
KNOWN_GATE_HEADS = {
    "Common", "R13", "R14", "R2000+", "R2004+", "R2007+", "R2010+", "R2013+",
    "R2018+", "R24", "R21", "R27", "R11", "R12",
    "R13-R14", "R13 - R14", "R13 & R14",
    "R2000 & later", "R2004 & later", "R2007 & later",
    "R2007 & prior", "R2013 & later",
    "R2000 only", "R2004 only", "R2007 only", "R2010 only",
    "Until R14", "Until R2000", "Until R2004",
}

SKIP_COLON = {  # Known noise patterns that end in ':' but are not gates.
    "DXF", "Wireframe == true", "Note",
}


def _read_spec() -> list[str]:
    if not SPEC_TEXT.exists():
        sys.stderr.write(
            f"error: spec text not found at {SPEC_TEXT}\n"
            f"       regenerate with: pdftotext -layout /Users/dli/doc/dwg/dwg.pdf {SPEC_TEXT}\n"
        )
        sys.exit(2)
    # Read raw bytes so we count newlines the same way as wc -l (matches the
    # committed identity assertions). The file ends with a form-feed (\x0c)
    # and pdftotext -layout also emits \x0c mid-file at page breaks (~279 of
    # them). splitlines() would split on those too and shift line numbers;
    # we split ONLY on \n so line-number arithmetic matches sed/wc.
    raw = SPEC_TEXT.read_bytes()
    newline_count = raw.count(b"\n")
    text = raw.decode("utf-8")
    # split("\n") on text ending in "\n" produces a trailing empty element
    # that is NOT a line; drop it so len(lines) == newline_count.
    parts = text.split("\n")
    if parts and parts[-1] == "":
        parts = parts[:-1]
    lines = parts

    # Round-3 B2 identity check — hard-fail on the wrong rendering.
    if newline_count != EXPECTED_LINE_COUNT:
        sys.stderr.write(
            f"error: spec text has {newline_count} newline-lines, expected {EXPECTED_LINE_COUNT}\n"
            f"       this looks like a different rendering (dwgTs/doc/dwg_spec.txt is 31865 lines and WRONG per round-3 B2)\n"
            f"       regenerate with: pdftotext -layout /Users/dli/doc/dwg/dwg.pdf {SPEC_TEXT}\n"
        )
        sys.exit(2)
    hatch_line = lines[EXPECTED_HATCH_LINE - 1].strip()
    if not hatch_line.startswith(EXPECTED_HATCH_PREFIX):
        sys.stderr.write(
            f"error: line {EXPECTED_HATCH_LINE} = {hatch_line!r}, expected startswith {EXPECTED_HATCH_PREFIX!r}\n"
        )
        sys.exit(2)
    return lines


def _units_index(lines: list[str]) -> list[dict]:
    """Locate every §20.4.N heading and its fend (next §20.4.N.M sub-heading)."""
    starts: list[tuple[int, str, str]] = []  # (1-based line, num, title)
    fends: dict[str, int] = {}
    for i, ln in enumerate(lines, start=1):
        m = UNIT_RE.match(ln)
        if m and not SUBHEAD_RE.match(ln):
            starts.append((i, f"20.4.{m.group(1)}", m.group(2).strip()))
        elif SUBHEAD_RE.match(ln):
            head = re.match(r"^\s*(20\.4\.\d+)\.\d+", ln).group(1)
            fends.setdefault(head, i)
    units = []
    for idx, (start, num, title) in enumerate(starts):
        nxt = starts[idx + 1][0] if idx + 1 < len(starts) else len(lines) + 1
        fend = fends.get(num, nxt)
        # If no sub-heading exists (e.g. UNDOCUMENTED units) fend == nxt.
        units.append({
            "num": num,
            "title": title,
            "start": start,
            "fend": fend,
            "end": nxt,
        })
    return units


def _hash_desc(text: str) -> str:
    return "sha256:" + hashlib.sha256(text.strip().encode("utf-8")).hexdigest()[:16]


def _canonicalize_gate(head: str) -> str:
    head = head.strip().rstrip(":").strip()
    head = re.sub(r"\s+", " ", head)
    return head


def _classify_gate(head: str, strict: bool) -> str | None:
    """Return the canonical gate string, or None if the line is skip-noise.

    If `strict=True`, an unknown gate raises SystemExit(2) — the gate whitelist
    hard-fail rule.
    """
    canon = _canonicalize_gate(head)
    # Skip patterns
    for skip in SKIP_COLON:
        if canon.lower().startswith(skip.lower()):
            return None
    # Repeat / control keywords ending in ':' are not gates.
    low = canon.lower()
    for kw in ("repeat", "begin repeat", "end repeat", "if (", "if!(",
               "else if", "else"):
        if low.startswith(kw):
            return None
    # Bit-diagram like "76543210" is not a gate.
    if re.match(r"^[0-7]{1,8}$", canon):
        return None
    # Known heads pass through unchanged.
    for known in KNOWN_GATE_HEADS:
        if canon == known or canon.startswith(known + " "):
            return canon
    # Common suffix "Only" or "+"
    if re.match(r"^R[\d][\dA-Za-z+\-.& ]*(?: Only)?$", canon):
        return canon
    if strict:
        # Hard-fail per plan §4 P0 item 2.
        raise SystemExit(f"error: unknown gate string {canon!r} — add to KNOWN_GATE_HEADS or SKIP_COLON")
    return canon


def _line_starts_with_type_only(line: str) -> bool:
    """Return True if the (stripped) line starts with a DWG type token and
    a non-type second token — i.e. it looks like the '<TYPE> <code> <desc>'
    tail of a two-line wrapped field row (the name is on the previous line)."""
    s = line.strip()
    if not s:
        return False
    toks = s.split()
    if not toks:
        return False
    if toks[0] not in _TYPE_SET:
        return False
    # If a second token exists, it should look like a code or free text, not
    # itself be a type (otherwise this is a bit-diagram or similar).
    return True


def _extract_unit(lines: list[str], unit: dict, desc_mode: str,
                  strict_gates: bool) -> dict:
    rows = []
    xrefs = []
    ordinal = 0
    gate_stack = ["Common"]  # default context
    span = lines[unit["start"] - 1: unit["fend"] - 1]
    # First pass: fold two-line wrapped field rows into one virtual line so
    # the downstream field regex sees "<name> <TYPE> <code> <desc>". Names
    # that are alone on a line, followed by a line starting with a type,
    # are the classic MTEXT case ("Background scale factor" / "BL  45 …").
    folded: list[tuple[int, str]] = []
    skip_next = False
    for offset, raw in enumerate(span):
        if skip_next:
            skip_next = False
            continue
        lineno = unit["start"] + offset
        line = raw.rstrip("\n")
        if offset + 1 < len(span):
            nxt = span[offset + 1].rstrip("\n")
            s = line.strip()
            if (s
                    and not GATE_RE.match(line)
                    and not XREF_RE.match(line)
                    and not CRC_RE.match(line)
                    and not CTRL_LINE_RE.match(line)
                    and not PAGE_HEADER_RE.match(line)
                    and s not in ("{", "}", "};")
                    and not any(x in s for x in ("*/", "/*"))
                    and _line_starts_with_type_only(nxt)):
                # Only fold if `line` looks like a bare name (no type tokens
                # in it) so we do not glue two real rows.
                if not any(t in _TYPE_SET for t in s.split()):
                    joined = line.rstrip() + "  " + nxt.strip()
                    folded.append((lineno, joined))
                    skip_next = True
                    continue
        folded.append((lineno, line))
    for lineno, line in folded:
        if not line.strip():
            continue
        if PAGE_HEADER_RE.match(line):
            continue
        # Sub-heading — should not appear (we stopped at fend), but guard.
        if SUBHEAD_RE.match(line):
            break
        # Structural xref
        xm = XREF_RE.match(line)
        if xm:
            xrefs.append({
                "kind": "xref",
                "name": xm.group(1).strip(),
                "spec_line": lineno,
            })
            continue
        if CRC_RE.match(line):
            xrefs.append({
                "kind": "xref",
                "name": "CRC",
                "spec_line": lineno,
            })
            continue
        # Gate line (ends in ':' by itself). Do NOT strip inside — the whole
        # trimmed line must match the gate shape.
        gm = GATE_RE.match(line)
        if gm:
            g = _classify_gate(gm.group(1), strict_gates)
            if g:
                gate_stack = [g]
            continue
        # Skip control flow lines.
        s = line.strip()
        if s.startswith("/*") or s.endswith("*/") or s.startswith("//"):
            continue
        if s in ("{", "}", "};"):
            continue
        # Control-flow lines are skipped by whole-line match — never by
        # first-word alone, or fields named "Start tangent" / "End tangent"
        # (HATCH R24 arm) get silently dropped.
        if CTRL_LINE_RE.match(line):
            continue
        # Bare `if (…) {` or `else if (…) {` with a trailing brace on its own
        # is control flow (the trailing `{` version is caught above; this
        # catches lines where the '{' is on the next line).
        if re.match(r"^\s*(if|else\s+if)\s*\(", line, re.IGNORECASE) and line.rstrip().endswith("{"):
            continue
        # Field row. Try multi-word (2+ space separator) first; fall back to
        # single-word (1+ space); finally, token-based rightmost-TYPE for the
        # "multi-word name + 1 space to type" case ("Default lighting type RC 282").
        fm = FIELD_RE_MULTIWORD.match(line)
        tok = None
        if not fm:
            fm = FIELD_RE_SINGLEWORD.match(line)
        if not fm:
            tok = _token_split_field(line)
        if fm or tok:
            ordinal += 1
            if fm:
                name_v = fm.group("name").strip()
                type_v = fm.group("type")
                code_v = fm.group("code")
                desc = (fm.group("desc") or "").strip()
            else:
                name_v = tok["name"]
                type_v = tok["type"]
                code_v = tok["code"]
                desc = (tok["desc"] or "").strip()
            rec = {
                "row_ordinal": ordinal,
                "unit": unit["num"],
                "gate": gate_stack[-1] if gate_stack else "Common",
                "name": name_v,
                "type": type_v,
                "dxf_code": code_v,
                "spec_line": lineno,
                "kind": "field",
            }
            # Round-3 R.1 — descText goes in as a hash by default. --descText-mode=verbatim
            # is available for developer-local reproduction only; committed files must be
            # 'hash' or 'paraphrase'.
            if desc:
                if desc_mode == "hash":
                    rec["descText_sha256"] = _hash_desc(desc)
                    rec["descText"] = None
                elif desc_mode == "verbatim":
                    rec["descText"] = desc
                    rec["descText_sha256"] = _hash_desc(desc)
                else:  # paraphrase — placeholder, TBD by maintainer
                    rec["descText"] = None
                    rec["descText_sha256"] = _hash_desc(desc)
            else:
                rec["descText"] = None
                rec["descText_sha256"] = None
            rows.append(rec)
    return {
        "num": unit["num"],
        "title": unit["title"],
        "start": unit["start"],
        "fend": unit["fend"],
        "rows": rows,
        "xrefs": xrefs,
        "row_count": len(rows),
        "xref_count": len(xrefs),
    }


def _selfcheck(units: dict) -> int:
    """Assert pilot targets; exit non-zero on drift.

    Round-3 note: the exact counts (SPLINE 20+3, VPORT 58, HATCH 68, MTEXT 35+3)
    are the plan's acceptance criteria. This function reports the observed
    deltas; adjustment of the extractor to hit them exactly is iterative
    hardening (SP1.1 acceptance, may take multiple slice-passes).
    """
    targets = {
        "20.4.40": ("SPLINE", 20),  # data rows only, +3 xrefs
        "20.4.64": ("VPORT",  58),
        "20.4.75": ("HATCH",  68),
        "20.4.46": ("MTEXT",  35),  # data rows only, +3 xrefs
    }
    total_data = sum(u["row_count"] for u in units.values())
    total_xref = sum(u["xref_count"] for u in units.values())
    print(f"units: {len(units)}   data rows: {total_data}   xref rows: {total_xref}")
    ok = True
    for num, (name, expected) in targets.items():
        u = units.get(num)
        if not u:
            print(f"  MISSING {num} ({name})")
            ok = False
            continue
        got = u["row_count"]
        mark = "OK" if got == expected else "DRIFT"
        if got != expected:
            ok = False
        print(f"  {mark:6s}  {num:8s} {name:8s}  expected {expected:3d}  got {got:3d}  (xrefs={u['xref_count']})")
    return 0 if ok else 1


def _write_json(units_records: list[dict]) -> None:
    obj = {
        "meta": {
            "spec_lines": EXPECTED_LINE_COUNT,
            "spec_identity_line": EXPECTED_HATCH_LINE,
            "spec_identity_prefix": EXPECTED_HATCH_PREFIX,
            "units_count": len(units_records),
            "descText_policy": "sha256_hash_only (round-3 R.1 licensing)",
        },
        "units": units_records,
    }
    OUT_JSON.parent.mkdir(parents=True, exist_ok=True)
    with open(OUT_JSON, "w", encoding="utf-8") as f:
        json.dump(obj, f, indent=2, sort_keys=True, ensure_ascii=True)
        f.write("\n")


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--unit", help="Extract one unit (e.g. 20.4.75) and print JSON.")
    ap.add_argument("--selfcheck", action="store_true",
                    help="Assert pilot targets; exit non-zero on drift.")
    ap.add_argument("--descText-mode", choices=["hash", "paraphrase", "verbatim"],
                    default="hash", help="round-3 R.1: only 'hash' or 'paraphrase' may be committed.")
    ap.add_argument("--strict-gates", action="store_true",
                    help="Hard-fail on any unknown gate string (plan §4 P0 item 2).")
    args = ap.parse_args()

    lines = _read_spec()
    idx = _units_index(lines)
    units_records = [_extract_unit(lines, u, args.descText_mode, args.strict_gates) for u in idx]
    units = {u["num"]: u for u in units_records}

    if args.unit:
        u = units.get(args.unit)
        if not u:
            print(f"unit {args.unit} not found", file=sys.stderr)
            return 2
        print(json.dumps(u, indent=2, sort_keys=True, ensure_ascii=True))
        return 0

    _write_json(units_records)
    if args.selfcheck:
        return _selfcheck(units)
    print(f"wrote {OUT_JSON} — {len(units_records)} units")
    return 0


if __name__ == "__main__":
    sys.exit(main())
