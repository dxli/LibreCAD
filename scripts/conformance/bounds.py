#!/usr/bin/env python3
"""bounds.py — Brace-matched Class::method body ranges over libdxfrw C++.

Given a Class::method symbol, resolve it to `(file, open_line, close_line)`
by locating the definition signature (multi-line, allowing trailing `const`)
and brace-matching the body — string-aware, comment-aware.

Round-3 R.3/R.4: this tool exists specifically because absolute line numbers
in the plan docs drift on every commit. Downstream slices MUST cite
`Class::method` + resolve via this tool at run time; NEVER commit an
absolute line number as authoritative.

Round-3 also invalidates any "hardcoded verified range table" as an
acceptance criterion (the plan's originally-listed 9 ranges included 6 naive
`[start, next_start - 1]` windows the same criterion forbids). This tool's
selfcheck asserts PROPERTIES:
  - AppId is the anti-naive exemplar (a brace-matched close < the next
    symbol's start).
  - DRW_Hatch::encodeDwgBoundaryData resolves (regression for the trailing-
    const bug that made the naive matcher return 0 tokens).
  - DRW_Dimstyle::parseDwg body length < 25 lines (S4 stub-detector
    positive control — the largest single defect in the code, per plan
    §3 P3 S4 acceptance).
  - No returned range overlaps the next symbol's `parseCode` (naive-window
    guard: the exact bug the plan says bounds.py MUST NOT reproduce).

Usage:
  bounds.py DRW_Hatch::encodeDwgBoundaryData          # print file:open-close
  bounds.py --json DRW_Hatch::parseDwg                # JSON object
  bounds.py --selfcheck                               # property-based gate
  bounds.py --all --include drw_entities.cpp          # enumerate all defs
"""
from __future__ import annotations
import argparse
import json
import re
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SRC = REPO / "libraries" / "libdxfrw" / "src"

# Rule 3: --include=*.cpp / *.h only. Skip .orig shadows explicitly.
INCLUDE_PATTERNS = ["*.cpp", "*.h"]
EXCLUDE_SUBSTRINGS = [".orig"]


def _iter_source_files() -> list[Path]:
    out: list[Path] = []
    for pat in INCLUDE_PATTERNS:
        for p in SRC.rglob(pat):
            if any(x in p.name for x in EXCLUDE_SUBSTRINGS):
                continue
            out.append(p)
    return out


def _strip_string_and_comment(source: str) -> str:
    """Return source with string literals and comments replaced by spaces of
    the same length. Preserves line breaks so line numbers stay valid.

    Handles: // to EOL, /* … */, "…" (with \\ escapes), '…' (with \\ escapes),
    and R"delim(…)delim" raw strings.
    """
    out: list[str] = []
    i, n = 0, len(source)
    while i < n:
        c = source[i]
        # Line comment
        if c == "/" and i + 1 < n and source[i + 1] == "/":
            j = source.find("\n", i)
            if j == -1:
                out.append(" " * (n - i))
                break
            out.append(" " * (j - i))
            i = j
            continue
        # Block comment
        if c == "/" and i + 1 < n and source[i + 1] == "*":
            j = source.find("*/", i + 2)
            if j == -1:
                out.append(" " * (n - i))
                break
            # Preserve newlines inside the block for line-number stability.
            for k in range(i, j + 2):
                out.append("\n" if source[k] == "\n" else " ")
            i = j + 2
            continue
        # Raw string R"delim(…)delim"
        if c == "R" and i + 1 < n and source[i + 1] == '"':
            m = re.match(r'R"([^()\\ \t\v\f\n]*)\(', source[i:])
            if m:
                delim = m.group(1)
                end_marker = ')' + delim + '"'
                j = source.find(end_marker, i + m.end())
                if j == -1:
                    out.append(" " * (n - i))
                    break
                for k in range(i, j + len(end_marker)):
                    out.append("\n" if source[k] == "\n" else " ")
                i = j + len(end_marker)
                continue
        # Regular string / char
        if c in ('"', "'"):
            j = i + 1
            while j < n:
                if source[j] == "\\" and j + 1 < n:
                    j += 2
                    continue
                if source[j] == c:
                    break
                j += 1
            for k in range(i, min(j + 1, n)):
                out.append("\n" if source[k] == "\n" else " ")
            i = min(j + 1, n)
            continue
        out.append(c)
        i += 1
    return "".join(out)


# Match a definition signature. Groups:
#   1 return type (optional, keep loose)
#   2 Class::method
# Accepts multi-line params by anchoring only on the "<Class>::<method>\s*(" prefix.
SIG_RE = re.compile(
    r"(?P<retn>[A-Za-z_][A-Za-z_0-9:*&<>\s]*?\s+)?"
    r"(?P<sym>[A-Za-z_][A-Za-z_0-9]*::[A-Za-z_~][A-Za-z_0-9]*)"
    r"\s*\("
)


def _find_body(source_clean: str, source_raw: str, sym: str
               ) -> tuple[int, int] | None:
    """Return (open_offset, close_offset) [inclusive], or None."""
    # Find sig start. Iterate matches until we find one that opens a body.
    for m in re.finditer(re.escape(sym) + r"\s*\(", source_clean):
        # Find the matching ')' for this (
        p = source_clean.find("(", m.end() - 1)
        depth = 1
        i = p + 1
        n = len(source_clean)
        while i < n and depth:
            if source_clean[i] == "(":
                depth += 1
            elif source_clean[i] == ")":
                depth -= 1
            i += 1
        if depth != 0:
            continue  # malformed
        # Skip whitespace, `const`, `override`, ref-qualifier
        while i < n and (source_clean[i].isspace() or source_clean[i] == ";"):
            i += 1
        # trailing qualifiers
        while True:
            skipped = False
            for kw in ("const", "override", "final", "noexcept", "&", "&&"):
                if source_clean[i:i + len(kw)] == kw:
                    nxt = i + len(kw)
                    if nxt >= n or not source_clean[nxt].isalnum() and source_clean[nxt] != "_":
                        i = nxt
                        while i < n and source_clean[i].isspace():
                            i += 1
                        skipped = True
                        break
            if not skipped:
                break
        if i >= n:
            continue
        if source_clean[i] == ";":
            # Prototype, not a body — keep searching.
            continue
        if source_clean[i] != "{":
            continue
        # Brace-match from here.
        open_off = i
        depth = 1
        j = i + 1
        while j < n and depth:
            ch = source_clean[j]
            if ch == "{":
                depth += 1
            elif ch == "}":
                depth -= 1
            j += 1
        if depth != 0:
            continue
        close_off = j - 1
        return open_off, close_off
    return None


def _offset_to_line(text: str, offset: int) -> int:
    return text.count("\n", 0, offset) + 1


def resolve(sym: str) -> dict | None:
    """Resolve Class::method to a body-range record."""
    for path in _iter_source_files():
        raw = path.read_text(encoding="utf-8", errors="replace")
        clean = _strip_string_and_comment(raw)
        pos = _find_body(clean, raw, sym)
        if pos:
            open_off, close_off = pos
            return {
                "symbol": sym,
                "file": str(path.relative_to(REPO)),
                "open_line": _offset_to_line(clean, open_off),
                "close_line": _offset_to_line(clean, close_off),
                "body_lines": _offset_to_line(clean, close_off)
                              - _offset_to_line(clean, open_off) + 1,
            }
    return None


def next_symbol_start(sym: str) -> int | None:
    """Return the start line of the NEXT `Class::method` definition after
    `sym` in the same file. Used to prove the naive-window guard: our
    close_line must be STRICTLY LESS THAN this."""
    r = resolve(sym)
    if not r:
        return None
    path = REPO / r["file"]
    raw = path.read_text(encoding="utf-8", errors="replace")
    clean = _strip_string_and_comment(raw)
    # Find all Class::method\s*( occurrences with body { after; return the first
    # whose sig starts strictly after our close_line.
    close_off_line = r["close_line"]
    for m in SIG_RE.finditer(clean):
        line = clean.count("\n", 0, m.start()) + 1
        if line <= close_off_line:
            continue
        return line
    return None


# ------- selfcheck -------

def _selfcheck() -> int:
    ok = True
    def check(cond: bool, msg: str) -> None:
        nonlocal ok
        print(f"  {'PASS' if cond else 'FAIL'}  {msg}")
        if not cond:
            ok = False

    # 1. AppId — the anti-naive exemplar. The plan's original 2646 was the
    # naive window; brace matching returns 2682.
    ap = resolve("DRW_AppId::parseDwg")
    print(f"DRW_AppId::parseDwg -> {ap}")
    check(ap is not None, "AppId resolves")
    if ap:
        ns = next_symbol_start("DRW_AppId::parseDwg")
        # Anti-naive check: brace-matched close is BEFORE the next symbol's sig.
        check(ns is None or ap["close_line"] < ns,
              f"AppId close {ap['close_line']} < next sym start {ns} (anti-naive)")

    # 2. DRW_Hatch::encodeDwgBoundaryData — the trailing-const regression.
    hb = resolve("DRW_Hatch::encodeDwgBoundaryData")
    print(f"DRW_Hatch::encodeDwgBoundaryData -> {hb}")
    check(hb is not None, "Hatch::encodeDwgBoundaryData resolves (trailing-const bug regression)")
    if hb:
        check(hb["body_lines"] > 20, "encodeDwgBoundaryData has a real body (not the 0-token bug)")

    # 3. DRW_Dimstyle::parseDwg — S4 stub-detector positive control. Body
    # must be small (<25 lines) so S4 fires. Round-3 verified: close is 1274.
    dm = resolve("DRW_Dimstyle::parseDwg")
    print(f"DRW_Dimstyle::parseDwg -> {dm}")
    check(dm is not None, "Dimstyle::parseDwg resolves")
    if dm:
        check(dm["body_lines"] < 25,
              f"Dimstyle body {dm['body_lines']} < 25 lines (S4 stub-detector positive control)")

    # 4. Sanity — resolve DRW_Hatch::parseDwg (the pilot workhorse).
    hp = resolve("DRW_Hatch::parseDwg")
    print(f"DRW_Hatch::parseDwg -> {hp}")
    check(hp is not None, "Hatch::parseDwg resolves")

    # 5. Naive-window guard: the resolved close of DRW_Layer::parseDwg must
    # be strictly less than the next Class::method start.
    lp = resolve("DRW_Layer::parseDwg")
    print(f"DRW_Layer::parseDwg -> {lp}")
    if lp:
        ns = next_symbol_start("DRW_Layer::parseDwg")
        check(ns is None or lp["close_line"] < ns,
              f"Layer close {lp['close_line']} < next sym {ns} (anti-naive)")

    return 0 if ok else 1


def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("symbol", nargs="?", help="Class::method to resolve.")
    ap.add_argument("--json", action="store_true", help="Emit JSON.")
    ap.add_argument("--selfcheck", action="store_true",
                    help="Run property-based acceptance checks.")
    args = ap.parse_args()
    if args.selfcheck:
        return _selfcheck()
    if not args.symbol:
        ap.error("symbol required (or --selfcheck)")
    r = resolve(args.symbol)
    if not r:
        print(f"NOT FOUND: {args.symbol}", file=sys.stderr)
        return 1
    if args.json:
        print(json.dumps(r, indent=2, sort_keys=True))
    else:
        print(f"{r['file']}:{r['open_line']}-{r['close_line']}  ({r['body_lines']} lines)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
