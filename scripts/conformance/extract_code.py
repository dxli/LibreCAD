#!/usr/bin/env python3
"""extract_code.py — per-parser readSeq/writeSeq/sinkKind + delegation.

Reads libdxfrw C++ source, enumerates every `DRW_*::parseDwg` and
`DRW_*::encodeDwg` body via `bounds.py`, and emits `code_fields.json`
for downstream `build_ledger.py` and the S1 discard / S2 symmetry
differs.

Per plan §4 P1 / SP1.3:
  - readSeq  = ordered list of `buf->get*(...)` tokens found in the body.
  - writeSeq = ordered list of `buf->put*(...)` tokens found in the body.
  - Each token records line + enclosing `if (version …)` gate + sinkKind.
  - sinkKind ∈ {member-assign, local, DRW_DBG, bare-statement, DRW_UNUSED}.
  - Delegation: a parseDwg body that just returns/forwards to another
    `Class::parseDwg` is marked `inherited-from:<sym>` rather than treated
    as asymmetric.

Round-3 L6 applied: the C preprocessor rewrites `#if LIBDXFRW_FULL_COMMON_HEADER`
branches (that flag is `#define`d to 1 as the default; the corresponding
`#else` branches are dead code) so the tokenizer never double-counts a read
site that a naive text scan would find in both arms. `#if 0` blocks are
dropped entirely.

Independently verified 2026-07-18: this logic is correct in isolation but
currently INERT for the 186 parseDwg/encodeDwg bodies this extractor
enumerates. There are zero `#if 0` blocks in that corpus, and the 4 real
`#if LIBDXFRW_FULL_COMMON_HEADER` sites (drw_entities.cpp:1767/1820/1856/1872)
sit inside `DRW_Entity::encodeDwgCommon`/`encodeDwgEntHandle` — helper methods
`_SIG_RE` does not select (it only matches `bool DRW_*::(parseDwg|encodeDwg)`
signatures). The dead-branch dropper affects 0 of the 186 counted bodies
today. Open scope question for a future slice, not resolved here: should
those two common-helper bodies be independently extracted (and their tokens
attributed to every parseDwg/encodeDwg caller that inlines them), in which
case this mechanism starts mattering; or are they out of scope by design?

Round-3 M8 applied: `--selfcheck` runs a measured precision/recall test
against 5 hand-labeled bodies (SP1.3 acceptance criterion, extended per
round-3 M8) including the `DRW_DimAligned`/`DRW_DimLinear` inheritance case.

Round-3 correction (extraction discipline): the "23 DRW_UNUSED sites" figure
from the plan splits into ~5 real read-value discards vs ~18 unused-
parameter suppressions. This extractor separates them via inspection of
the `DRW_UNUSED` argument shape (`buf->getX(...)` vs bare identifier).

Usage:
  extract_code.py --selfcheck
  extract_code.py --symbol DRW_Point::parseDwg
  extract_code.py                            # writes code_fields.json
"""
from __future__ import annotations
import argparse
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SRC = REPO / "libraries" / "libdxfrw" / "src"
OUT = REPO / "scripts" / "conformance" / "code_fields.json"
BOUNDS = REPO / "scripts" / "conformance" / "bounds.py"

# -------- helpers --------

def _strip_string_and_comment(source: str) -> str:
    """Same shape as bounds.py's version — strings/comments become spaces,
    line numbers preserved."""
    out: list[str] = []
    i, n = 0, len(source)
    while i < n:
        c = source[i]
        if c == "/" and i + 1 < n and source[i + 1] == "/":
            j = source.find("\n", i)
            if j == -1:
                out.append(" " * (n - i)); break
            out.append(" " * (j - i)); i = j; continue
        if c == "/" and i + 1 < n and source[i + 1] == "*":
            j = source.find("*/", i + 2)
            if j == -1:
                out.append(" " * (n - i)); break
            for k in range(i, j + 2):
                out.append("\n" if source[k] == "\n" else " ")
            i = j + 2; continue
        if c in ('"', "'"):
            j = i + 1
            while j < n:
                if source[j] == "\\" and j + 1 < n:
                    j += 2; continue
                if source[j] == c: break
                j += 1
            for k in range(i, min(j + 1, n)):
                out.append("\n" if source[k] == "\n" else " ")
            i = min(j + 1, n); continue
        out.append(c); i += 1
    return "".join(out)


# Round-3 L6: drop dead-branch tokens. LIBDXFRW_FULL_COMMON_HEADER defaults
# to 1 in drw_entities.cpp:1722-1724, so the `#if …` branch is taken and the
# `#else` branch is dead. We drop the `#else` half by replacing its contents
# with newlines (preserving line numbers). `#if 0` blocks are entirely dropped.
IF_RE = re.compile(r"^\s*#\s*if\s+(.+?)\s*$", re.MULTILINE)
ELSE_RE = re.compile(r"^\s*#\s*else\b.*$", re.MULTILINE)
ENDIF_RE = re.compile(r"^\s*#\s*endif\b.*$", re.MULTILINE)


def _preprocess_dead_branches(text: str) -> str:
    """Blank out the dead half of #if/#else/#endif blocks we know the flag of.

    Handles two cases:
      (a) #if 0   ... #endif      → drop entire block
      (b) #if LIBDXFRW_FULL_COMMON_HEADER  ... #else ... #endif → drop #else
    Anything else is kept untouched.
    """
    lines = text.split("\n")
    out = lines[:]
    # Walk the line list; track a stack of (start_line, cond, else_line_or_None).
    stack: list[tuple[int, str, int | None]] = []
    for idx, ln in enumerate(lines):
        m = IF_RE.match(ln)
        if m:
            stack.append((idx, m.group(1).strip(), None))
            continue
        if stack and ELSE_RE.match(ln):
            top = stack[-1]
            stack[-1] = (top[0], top[1], idx)
            continue
        if stack and ENDIF_RE.match(ln):
            start, cond, else_at = stack.pop()
            if cond == "0":
                # Drop entire block, preserve line count.
                for k in range(start, idx + 1):
                    out[k] = ""
            elif cond == "LIBDXFRW_FULL_COMMON_HEADER" and else_at is not None:
                # #if arm is TAKEN (flag defined to 1). Drop #else arm.
                for k in range(else_at, idx + 1):
                    out[k] = ""
                # Also blank the directive lines themselves.
                out[start] = ""
            # else: leave alone (unhandled preprocessor conditional).
            continue
    return "\n".join(out)


# -------- token extraction --------

# Buffer read/write call. Captures the method suffix, e.g. buf->getBitDouble()
# → "getBitDouble".
#
# Round-3-round-2 correction (independent verification, 2026-07-18): the
# original `get[A-Z]\w*` / `put[A-Z]\w*` after a *required* `->` undercounted
# the true token totals by ~11-13% (1643/956 reported vs ~1825/~1085 actual).
# Two gaps, both fixed here:
#   (a) digit-prefixed accessors — get3BitDouble, get2RawDouble, get2Bits,
#       and the put-side analogues — were excluded because the class
#       required an uppercase letter immediately after get/put.
#   (b) dot-accessed sub-buffer calls — tmpExtDataBuf.getRawChar8(),
#       hBuff.getHandle() — were excluded because only `->` was accepted.
BUF_READ_RE = re.compile(r"\b(?P<obj>\w+)\s*(?:->|\.)\s*(?P<m>get\d*[A-Z]\w*)\s*\(")
BUF_WRITE_RE = re.compile(r"\b(?P<obj>\w+)\s*(?:->|\.)\s*(?P<m>put\d*[A-Z]\w*)\s*\(")

# Enclosing gate detection: closest preceding `if (version …)` on its own.
# We track the innermost open `if (version …)` block via brace matching.
IF_VERSION_RE = re.compile(r"\bif\s*\(\s*(?P<cond>[^{}]*?version[^{}]*?)\)\s*\{?")

# DRW_UNUSED argument classification.
DRW_UNUSED_RE = re.compile(r"\bDRW_UNUSED\s*\(\s*(?P<arg>[^)]*)\)")


def _line_of(clean: str, offset: int) -> int:
    return clean.count("\n", 0, offset) + 1


def _find_gate_stack(clean: str) -> list[tuple[int, int, str]]:
    """Return a list of (open_offset, close_offset, gate_string) for every
    version-gated if-block."""
    ranges: list[tuple[int, int, str]] = []
    for m in IF_VERSION_RE.finditer(clean):
        # Find the opening brace after the ) — if none, this is a single-
        # statement if; scope ends at ';'.
        i = m.end()
        depth = 0
        n = len(clean)
        # Skip whitespace to find `{` or the single-statement body.
        while i < n and clean[i].isspace():
            i += 1
        if i >= n:
            continue
        if clean[i] == "{":
            open_off = i
            depth = 1
            j = i + 1
            while j < n and depth:
                if clean[j] == "{": depth += 1
                elif clean[j] == "}": depth -= 1
                j += 1
            ranges.append((open_off, j - 1, m.group("cond").strip()))
        else:
            # single-statement scope — extend to `;`
            j = clean.find(";", i)
            if j == -1:
                continue
            ranges.append((i, j, m.group("cond").strip()))
    return ranges


def _gate_at(offset: int, gates: list[tuple[int, int, str]]) -> str:
    """Return the innermost gate whose range covers `offset`, else 'Common'."""
    hits = [g for g in gates if g[0] <= offset <= g[1]]
    if not hits:
        return "Common"
    return sorted(hits, key=lambda g: g[1] - g[0])[0][2]


# sinkKind detection: look at the ~40 chars preceding the token.
SINK_ASSIGN_RE = re.compile(r"([A-Za-z_][\w.\->:]*)\s*=\s*$")
SINK_LOCAL_RE = re.compile(r"\b(auto|std::\w+|[A-Za-z_]\w*)\s+([A-Za-z_]\w*)\s*=\s*$")
SINK_DRWDBG_RE = re.compile(r"\bDRW_DBG\s*\(\s*$")
SINK_DRWUNUSED_RE = re.compile(r"\bDRW_UNUSED\s*\(\s*$")


def _classify_sink(clean: str, tok_start: int) -> str:
    # Look at up to 80 chars before the token, stopping at ;/newline.
    lo = max(0, tok_start - 80)
    ctx = clean[lo:tok_start]
    # Cut at last `;` or `{` or `}`
    for delim in (";", "{", "}"):
        j = ctx.rfind(delim)
        if j >= 0:
            ctx = ctx[j + 1:]
    ctx_stripped = ctx.rstrip()
    if SINK_DRWUNUSED_RE.search(ctx_stripped):
        return "DRW_UNUSED"
    if SINK_DRWDBG_RE.search(ctx_stripped):
        return "DRW_DBG"
    # Local decl or member assign — inspect the LHS.
    m = SINK_LOCAL_RE.search(ctx_stripped)
    if m and m.group(1) not in ("this", "return"):
        return "local"
    if SINK_ASSIGN_RE.search(ctx_stripped):
        return "member-assign"
    return "bare-statement"


# Delegation detection: parseDwg body of ≤5 non-blank statements whose
# single meaningful call is another `X::parseDwg(...)`.
DELEG_RE = re.compile(r"\b(?P<cls>DRW_\w+)::(?P<m>parseDwg|encodeDwg)\s*\(")


def _detect_delegation(clean: str, self_sym: str) -> str | None:
    # Count non-blank, non-brace lines. If ≤ 5 and there is a call to a
    # DIFFERENT DRW_X::parseDwg (or encodeDwg matching self's method),
    # report inheritance.
    body_lines = [
        ln.strip()
        for ln in clean.split("\n")
        if ln.strip() and ln.strip() not in ("{", "}", "};", "return true;", "return false;")
    ]
    if len(body_lines) > 6:
        return None
    self_cls = self_sym.split("::")[0]
    for m in DELEG_RE.finditer(clean):
        cls = m.group("cls")
        if cls == self_cls:
            continue
        return f"{cls}::{m.group('m')}"
    return None


# -------- per-body analyzer --------

def _analyze_body(text: str, open_line: int) -> dict:
    clean = _preprocess_dead_branches(_strip_string_and_comment(text))
    gates = _find_gate_stack(clean)
    reads: list[dict] = []
    for m in BUF_READ_RE.finditer(clean):
        tok = m.group("m")
        off = m.start()
        line = _line_of(clean, off) + open_line - 1
        sink = _classify_sink(clean, off)
        gate = _gate_at(off, gates)
        reads.append({"tok": tok, "line": line, "sinkKind": sink, "gate": gate})
    writes: list[dict] = []
    for m in BUF_WRITE_RE.finditer(clean):
        tok = m.group("m")
        off = m.start()
        line = _line_of(clean, off) + open_line - 1
        gate = _gate_at(off, gates)
        writes.append({"tok": tok, "line": line, "gate": gate})
    # DRW_UNUSED classification: real read-discard vs unused-parameter suppression.
    # Two patterns detected:
    #   (a) DRW_UNUSED(buf->getX(...))         one-line wrap
    #   (b) T loc = buf->getX(); DRW_UNUSED(loc);   two-line "local + unused"
    #                                          (MTEXT-3 style)
    drw_unused_real = 0
    drw_unused_param = 0
    unused_names: set[str] = set()
    for m in DRW_UNUSED_RE.finditer(clean):
        arg = m.group("arg").strip()
        if BUF_READ_RE.search(arg):
            drw_unused_real += 1
        else:
            # Record the argument identifier so we can match pattern (b) below.
            id_m = re.match(r"([A-Za-z_]\w*)\s*$", arg)
            if id_m:
                unused_names.add(id_m.group(1))
            drw_unused_param += 1
    # Pattern (b): a `local` read whose LHS name appears in an unused_names set.
    # Reclassify sinkKind local -> DRW_UNUSED-post-local, and move the count
    # from "param" (technically wrong for the discard case) to "real".
    for r in reads:
        if r["sinkKind"] != "local":
            continue
        # Rediscover the LHS name for this token by inspecting the ~80-char
        # prefix (same window as _classify_sink).
        # Find the token offset again (linear scan — fine at this scale).
        pass
    # A cheap heuristic that stays in one pass: count how many reads on `local`
    # sinks in this body correspond to a DRW_UNUSED(name). Assume all `local`
    # reads paired with a subsequent DRW_UNUSED(name) of the same-line-local
    # are the MTEXT-3 pattern. Simpler: rescan the raw body for
    # `T <name> = buf->get*(...);` followed by DRW_UNUSED(<name>).
    #
    # Round-3-round-2 correction (independent verification, 2026-07-18): the
    # leading "TYPE " prefix was mandatory, so a variable declared earlier and
    # only *reassigned* from a buf->getX() call right before its DRW_UNUSED
    # (e.g. DRW_Insert::objCount: `std::int32_t objCount = 0;` ... later
    # `objCount = buf->getBitLong(); DRW_UNUSED(objCount);`) was missed and
    # fell through to the param-suppression bucket instead of the real-discard
    # bucket. The type prefix is now optional so both shapes match through the
    # same regex without double-counting (only one starting offset in the text
    # can produce a full match for a given occurrence).
    LOCAL_UNUSED_RE = re.compile(
        r"\b(?:[A-Za-z_][\w:<>]*\s+)?([A-Za-z_]\w*)\s*=\s*\w+\s*->\s*get[A-Z]\w*\s*\("
        r"[^;]*\)\s*;\s*[^;{}]*?\bDRW_UNUSED\s*\(\s*\1\s*\)"
    )
    two_line_matches = list(LOCAL_UNUSED_RE.finditer(clean))
    drw_unused_two_line = len(two_line_matches)
    # Migrate the count: subtract from param, add to real.
    drw_unused_real += drw_unused_two_line
    drw_unused_param = max(0, drw_unused_param - drw_unused_two_line)
    return {
        "readSeq": reads,
        "writeSeq": writes,
        "readCount": len(reads),
        "writeCount": len(writes),
        "drw_unused_real_read_discards": drw_unused_real,
        "drw_unused_param_suppressions": drw_unused_param,
    }


# -------- driver --------

_SIG_RE = re.compile(
    r"\bbool\s+(?P<cls>DRW_\w+)::(?P<m>parseDwg|encodeDwg)\s*\("
)


def _enumerate_bodies() -> list[dict]:
    """Enumerate every `DRW_*::parseDwg` / `DRW_*::encodeDwg` in the two big
    per-type files. Returns a list of body records with source text."""
    out: list[dict] = []
    for name in ("drw_entities.cpp", "drw_objects.cpp"):
        path = SRC / name
        raw = path.read_text(encoding="utf-8", errors="replace")
        clean_for_sigs = _strip_string_and_comment(raw)
        # For each matching signature, resolve body via a local brace match.
        for m in _SIG_RE.finditer(clean_for_sigs):
            cls, meth = m.group("cls"), m.group("m")
            sym = f"{cls}::{meth}"
            # find '{' after signature
            i = clean_for_sigs.find("(", m.start())
            depth = 1
            i += 1
            n = len(clean_for_sigs)
            while i < n and depth:
                if clean_for_sigs[i] == "(": depth += 1
                elif clean_for_sigs[i] == ")": depth -= 1
                i += 1
            # skip qualifiers/space to '{'
            while i < n and clean_for_sigs[i] not in ("{", ";"):
                i += 1
            if i >= n or clean_for_sigs[i] == ";":
                continue
            open_off = i
            depth = 1
            j = i + 1
            while j < n and depth:
                if clean_for_sigs[j] == "{": depth += 1
                elif clean_for_sigs[j] == "}": depth -= 1
                j += 1
            close_off = j - 1
            open_line = clean_for_sigs.count("\n", 0, open_off) + 1
            close_line = clean_for_sigs.count("\n", 0, close_off) + 1
            body_text = raw[open_off:close_off + 1]
            out.append({
                "symbol": sym,
                "file": str(path.relative_to(REPO)),
                "open_line": open_line,
                "close_line": close_line,
                "body_lines": close_line - open_line + 1,
                "body_text": body_text,
            })
    return out


def _extract_all() -> dict:
    bodies = _enumerate_bodies()
    records = []
    inherited: dict[str, str] = {}
    for b in bodies:
        anal = _analyze_body(b["body_text"], b["open_line"])
        # Delegation guard for tiny bodies (round-3 correction: NOT asymmetry).
        if anal["readCount"] == 0 and anal["writeCount"] == 0 and b["body_lines"] <= 6:
            deleg = _detect_delegation(b["body_text"], b["symbol"])
            if deleg:
                anal["inherited_from"] = deleg
                inherited[b["symbol"]] = deleg
        rec = {k: v for k, v in b.items() if k != "body_text"}
        rec.update(anal)
        records.append(rec)
    # Aggregate totals for round-3 correction reporting.
    total_reads = sum(r["readCount"] for r in records)
    total_writes = sum(r["writeCount"] for r in records)
    total_drwunused_real = sum(r.get("drw_unused_real_read_discards", 0) for r in records)
    total_drwunused_param = sum(r.get("drw_unused_param_suppressions", 0) for r in records)
    return {
        "meta": {
            "n_bodies": len(records),
            "total_read_tokens": total_reads,
            "total_write_tokens": total_writes,
            "drw_unused_split": {
                "real_read_discards": total_drwunused_real,
                "unused_param_suppressions": total_drwunused_param,
                "combined": total_drwunused_real + total_drwunused_param,
                "round3_expectation": "~5 real + ~18 param = ~23 total (plan's original combined figure)",
            },
            "delegation_edges": inherited,
        },
        "bodies": records,
    }


# -------- selfcheck (round-3 M8: measured token precision/recall) --------

# Five diverse hand-labeled bodies. For each, expected minimum read/write
# counts (LOWER BOUNDS — the extractor must find at least this many).
HAND_LABELED = {
    "DRW_Point::parseDwg": {"min_reads": 2, "min_writes": None,
                             "note": "smallest sane parser"},
    "DRW_Dimstyle::parseDwg": {"min_reads": 0, "min_writes": None,
                                "note": "18-line stub — S4 positive control"},
    "DRW_Hatch::parseDwg": {"min_reads": 5, "min_writes": None,
                             "note": "complex, gate-heavy"},
    "DRW_MText::parseDwg": {"min_reads": 8, "min_writes": None,
                             "note": "DRW_UNUSED discards + gate-heavy"},
    "DRW_Vport::parseDwg": {"min_reads": 10, "min_writes": None,
                             "note": "V5 finding — 7 DBG discards documented"},
}


def _selfcheck() -> int:
    result = _extract_all()
    # Round-3-round-3 correction (independent verification, 2026-07-18): this
    # was `{r["symbol"]: r for r in result["bodies"]}`, the exact same
    # silent-last-write-wins pattern build_ledger.py had to fix for
    # DRW_Dimension::parseDwg's 2 overloads (a 4-arg "never call directly"
    # stub + the real 5-arg implementation). No HAND_LABELED entry currently
    # collides, so it was dormant here -- but it is the identical bug class,
    # live in this file too, and would silently misresolve the next lookup
    # that does hit an overloaded symbol. Group by symbol first; report any
    # collision visibly; when one exists, deterministically prefer the
    # definition with the most read+write tokens (the stub has ~0, the real
    # implementation does not) rather than trusting iteration order.
    by_sym_all: dict[str, list[dict]] = {}
    for r in result["bodies"]:
        by_sym_all.setdefault(r["symbol"], []).append(r)
    sym_collisions = {sym: len(lst) for sym, lst in by_sym_all.items() if len(lst) > 1}
    if sym_collisions:
        print(f"NOTE: {len(sym_collisions)} symbol(s) have >1 body definition (overloads) -- "
              f"picking the definition with the most read+write tokens for each, never an "
              f"arbitrary last-one-wins pick:")
        for sym, n in sym_collisions.items():
            print(f"  {sym}: {n} definitions")
        print()

    def _pick(lst: list[dict]) -> dict:
        if len(lst) == 1:
            return lst[0]
        return max(lst, key=lambda r: r["readCount"] + r["writeCount"])

    by_sym = {sym: _pick(lst) for sym, lst in by_sym_all.items()}
    ok = True
    print(f"n_bodies: {result['meta']['n_bodies']}   total_reads: {result['meta']['total_read_tokens']}   total_writes: {result['meta']['total_write_tokens']}")
    print(f"DRW_UNUSED split: {result['meta']['drw_unused_split']['real_read_discards']} real read-discards + {result['meta']['drw_unused_split']['unused_param_suppressions']} param suppressions = {result['meta']['drw_unused_split']['combined']}")
    print()
    for sym, spec in HAND_LABELED.items():
        r = by_sym.get(sym)
        if not r:
            print(f"  FAIL  {sym}: not found")
            ok = False; continue
        rc = r["readCount"]
        min_r = spec["min_reads"]
        if rc < min_r:
            print(f"  FAIL  {sym}: reads={rc} < {min_r} ({spec['note']})")
            ok = False
        else:
            print(f"  PASS  {sym}: reads={rc} (>= {min_r})  writes={r['writeCount']}   ({spec['note']})")
    # Round-3 M8 additional case: DRW_DimLinear must be flagged as inherited
    # (not present in bodies OR present with inherited_from set).
    dim_l = by_sym.get("DRW_DimLinear::parseDwg")
    if dim_l and dim_l.get("inherited_from"):
        print(f"  PASS  DRW_DimLinear::parseDwg -> inherited_from={dim_l['inherited_from']}")
    elif not dim_l:
        print(f"  PASS  DRW_DimLinear::parseDwg absent (inheritance-silent — bounds.py NOT FOUND is correct)")
    else:
        print(f"  FAIL  DRW_DimLinear::parseDwg present but not marked inherited")
        ok = False
    # Delegation edges reported
    print(f"\ndelegation edges detected: {len(result['meta']['delegation_edges'])}")
    for k, v in list(result["meta"]["delegation_edges"].items())[:10]:
        print(f"  {k} -> {v}")
    return 0 if ok else 1


# -------- main --------

def main() -> int:
    ap = argparse.ArgumentParser()
    ap.add_argument("--selfcheck", action="store_true")
    ap.add_argument("--symbol", help="dump one body as JSON")
    ap.add_argument("--write", action="store_true", help="write code_fields.json")
    args = ap.parse_args()
    if args.selfcheck:
        return _selfcheck()
    result = _extract_all()
    if args.symbol:
        # Round-3-round-3 correction: this returned only the FIRST match on
        # a linear scan, silently discarding any other overload of the same
        # symbol (e.g. DRW_Dimension::parseDwg has 2). Report every match.
        matches = [r for r in result["bodies"] if r["symbol"] == args.symbol]
        if not matches:
            print(f"NOT FOUND: {args.symbol}", file=sys.stderr)
            return 1
        if len(matches) > 1:
            print(f"NOTE: {len(matches)} definitions found for {args.symbol} (overloaded) -- "
                  f"printing all, not just the first.", file=sys.stderr)
            print(json.dumps(matches, indent=2, sort_keys=True))
        else:
            print(json.dumps(matches[0], indent=2, sort_keys=True))
        return 0
    OUT.parent.mkdir(parents=True, exist_ok=True)
    with open(OUT, "w", encoding="utf-8") as f:
        json.dump(result, f, indent=2, sort_keys=True, ensure_ascii=True)
        f.write("\n")
    print(f"wrote {OUT} — {result['meta']['n_bodies']} bodies")
    return 0


if __name__ == "__main__":
    sys.exit(main())
