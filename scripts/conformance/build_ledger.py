#!/usr/bin/env python3
"""build_ledger.py — Regenerable coverage-of-record ledger.

Joins:
  scripts/conformance/spec_fields.json    (104 units, 1345 data rows, 143 xrefs)
  scripts/conformance/code_fields.json    (186 bodies, readSeq/writeSeq, delegation edges)
  scripts/conformance/shared_bodies.json  (13 edges, delegation guard)
  scripts/conformance/GATE_BRANCHES.json  (gate-string -> version-branch subset)
  (optional) docs/conformance/corpus_hits.json   (populated by 00-O-fieldhit)

Emits:
  scripts/conformance/spec_ledger.json — one record per (unit, row_ordinal, version_branch).
                                          Every §20.4.N row × its applicable branches.
Plus named-untabled records for the §20.3-named / §20.4-untabled family (A.1)
and unknowable records for the 9 UNDOCUMENTED control-objects (round-3 M5).

Row identity per A.16: (unit, row_ordinal, version_branch).

Verdict enum (round-3 M5 restores `unknowable`):
  unaudited | walked-clean | defective | sweep-only | named-untabled |
  crossing-asymmetric | unexercised | unvalidatable | unknowable

Modes:
  --write            regenerate spec_ledger.json
  --check            re-derive to a buffer and diff against committed
  --stats            print aggregate counts (independently reproducible)
  --explain <unit>   dump ledger rows for one unit
"""
from __future__ import annotations
import argparse
import json
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SPEC_FIELDS = REPO / "scripts" / "conformance" / "spec_fields.json"
CODE_FIELDS = REPO / "scripts" / "conformance" / "code_fields.json"
SHARED_BODIES = REPO / "scripts" / "conformance" / "shared_bodies.json"
GATE_BRANCHES = REPO / "scripts" / "conformance" / "GATE_BRANCHES.json"
CORPUS_HITS = REPO / "docs" / "conformance" / "corpus_hits.json"
LEDGER = REPO / "scripts" / "conformance" / "spec_ledger.json"

# Round-3 M5: restore `unknowable` (dropped in A.16). Downstream sweeps
# must include it in their verdict-enum guards.
VERDICT_ENUM = [
    "unaudited",
    "walked-clean",
    "defective",
    "sweep-only",
    "named-untabled",
    "crossing-asymmetric",
    "unexercised",
    "unvalidatable",
    "unknowable",
]

# Addendum A.1 — §20.3-named / §20.4-untabled family. Records get
# verdict=named-untabled at seed (no §20.4 row to join to). Reviewers may
# promote these to walked-clean / defective as they walk the parseDwg site.
#
# Round-3-round-3 correction (independent verification, 2026-07-18):
#   (a) parse_symbol values were bare "<TYPE>::parseDwg" placeholders, not
#       the real C++ symbols — grep-verified against code_fields.json and
#       corrected below (DRW_ prefix; WipeoutVariables is plural in source;
#       PLACEHOLDER's real class is DRW_AcDbPlaceholder, not DRW_Placeholder).
#   (b) OLEFRAME has NO implementation anywhere in the tree — the only trace
#       is a commented-out enum entry (drw_entities.h:69, `//ackaged OLEFRAME,`).
#       parse_symbol is None, not a fabricated symbol that looks resolved.
#   (c) every row shared the identical (unit=None, row_ordinal=None,
#       version_branch=None) key, so any downstream consumer indexing by the
#       ledger's own documented row identity collapsed 7 rows into 1 and
#       silently dropped 6. Each row now gets a distinct synthetic key in a
#       dedicated namespace that cannot collide with a real "20.4.N" unit.
NAMED_UNTABLED = [
    {"dwg_type": "MATERIAL",        "parse_symbol": "DRW_Material::parseDwg",
     "note": "prefix-only per §20.3; parseDwg lives in drw_objects.cpp — see plan §5 Deferred"},
    {"dwg_type": "VISUALSTYLE",     "parse_symbol": "DRW_VisualStyle::parseDwg",
     "note": "R2010+ bit-layout historically mis-modelled as BB+B (now 3xB); grep-verified"},
    {"dwg_type": "PLOTSETTINGS",    "parse_symbol": "DRW_PlotSettings::parseDwg",
     "note": "§20.3 non-fixed"},
    {"dwg_type": "DBCOLOR",         "parse_symbol": "DRW_DbColor::parseDwg",
     "note": "ENC 0x40 → handle case; verified by 2026-05 fix"},
    {"dwg_type": "WIPEOUTVARIABLE", "parse_symbol": "DRW_WipeoutVariables::parseDwg",
     "note": "§20.3 non-fixed; note the real class name is plural (WipeoutVariables)"},
    {"dwg_type": "PLACEHOLDER",     "parse_symbol": "DRW_AcDbPlaceholder::parseDwg",
     "note": "ACDBPLACEHOLDER (0x50); both fixed and non-fixed in §20.3; real class is DRW_AcDbPlaceholder"},
    {"dwg_type": "OLEFRAME",        "parse_symbol": None,
     "note": "§20.3 fixed (0x2B) but UNIMPLEMENTED — grep-verified 2026-07-18: no DRW_OleFrame class, "
             "no dispatch case, only a commented-out enum entry at drw_entities.h:69. "
             "parse_symbol is deliberately None; do not fabricate a resolved-looking symbol here."},
]

# Round-3 M5 — the 9 UNDOCUMENTED control-objects seed with verdict=unknowable.
UNDOCUMENTED_CONTROLS = [
    ("20.4.53", "(UNDOCUMENTED §20.4.53)"),
    ("20.4.55", "(UNDOCUMENTED §20.4.55)"),
    ("20.4.57", "(UNDOCUMENTED §20.4.57)"),
    ("20.4.59", "(UNDOCUMENTED §20.4.59)"),
    ("20.4.61", "(UNDOCUMENTED §20.4.61)"),
    ("20.4.63", "(UNDOCUMENTED §20.4.63)"),
    ("20.4.65", "(UNDOCUMENTED §20.4.65)"),
    ("20.4.67", "(UNDOCUMENTED §20.4.67)"),
    ("20.4.69", "(UNDOCUMENTED §20.4.69)"),
]


def _load(path: Path) -> dict:
    if not path.exists():
        return {}
    return json.loads(path.read_text())


def _resolve_symbol_for_unit(unit_num: str, code_bodies: list[dict]) -> tuple[str | None, str | None]:
    """Best-effort join: pick DRW_<X>::parseDwg (reader) and DRW_<X>::encodeDwg (writer).

    Naming heuristic: the unit's title starts with a type label (e.g. `HATCH (varies)`).
    We look for `DRW_<Titlecased>::parseDwg` and `::encodeDwg`. If no exact match,
    return (None, None) and let the sweep phase's stronger matcher pick it up.
    """
    # Cheap heuristic: try mapping the unit -> DRW_<capitalized single-word from title>.
    # For units like 'HATCH (varies)' → 'DRW_Hatch'.
    # For 'DIMSTYLE' → 'DRW_Dimstyle'.
    # This misses everything with a suffix parenthesis or multi-word title;
    # matches ~half of parseDwg-parsed units. That's OK for the seed; the
    # remaining rows carry `reader_symbol=null` and downstream slices resolve.
    return (None, None)  # seed slice keeps this null; extract_code has the true set


def _build_ledger(strict: bool = False) -> dict:
    spec = _load(SPEC_FIELDS)
    code = _load(CODE_FIELDS)
    shared = _load(SHARED_BODIES)
    gates = _load(GATE_BRANCHES)
    corpus = _load(CORPUS_HITS)  # {} if fieldhit not landed yet

    if not spec or not code or not gates:
        raise SystemExit("build_ledger: missing prerequisite artifact — check exists and non-empty")

    # Build symbol -> body index for readSeq/writeSeq lookup.
    # NOTE: DRW_Dimension::parseDwg has 2 overloads (round-3 M8 test case) —
    # the second silently overwrites the first in a plain dict. We preserve
    # BOTH by keying the index on (symbol, open_line) and separately record
    # the overload-collision count so aggregates stay reproducible.
    bodies_all = code.get("bodies", [])
    body_by_key: dict[tuple[str, int], dict] = {
        (b["symbol"], b["open_line"]): b for b in bodies_all
    }
    body_by_sym: dict[str, list[dict]] = {}
    for b in bodies_all:
        body_by_sym.setdefault(b["symbol"], []).append(b)
    overload_collisions = sum(1 for lst in body_by_sym.values() if len(lst) > 1)

    # For each spec row, expand into (unit, row_ordinal, version_branch) cells.
    gate_map = gates.get("gates", {})
    rows: list[dict] = []
    for u in spec["units"]:
        unit_num = u["num"]
        for r in u["rows"]:
            g = r.get("gate") or "Common"
            branches = gate_map.get(g)
            if branches is None:
                # Fall back to Common (all 7 branches) for gates that are
                # conditional-only or missing — never silently drop.
                branches = ["R13-R14", "R2000", "R2004", "R2007", "R2010", "R2013", "R2018"]
            for branch in branches:
                rows.append({
                    "unit": unit_num,
                    "row_ordinal": r["row_ordinal"],
                    "version_branch": branch,
                    "spec_line": r["spec_line"],
                    "gate": g,
                    "name": r["name"],
                    "type": r["type"],
                    "dxf_code": r.get("dxf_code"),
                    "descText_sha256": r.get("descText_sha256"),
                    "reader_symbol": None,   # populated by future extract_code join pass
                    "writer_symbol": None,
                    "shared_body": None,     # future join
                    "corpus_hits": None,     # future 00-O-fieldhit
                    "verdict": "unaudited",
                    "finding_id": None,
                })

    # Named-untabled family (A.1) — one seed row per type. No §20.4 unit to
    # join to, so each row is keyed in a dedicated synthetic namespace
    # ("__named-untabled__/<TYPE>") that cannot collide with a real "20.4.N"
    # unit string, giving every row a distinct (unit, row_ordinal,
    # version_branch) triple as meta.row_identity promises.
    for e in NAMED_UNTABLED:
        rows.append({
            "unit": f"__named-untabled__/{e['dwg_type']}",
            "row_ordinal": 0,
            "version_branch": "Common",
            "spec_line": None,
            "gate": None,
            "name": e["dwg_type"],
            "type": "STRUCTURAL",
            "dxf_code": None,
            "descText_sha256": None,
            "reader_symbol": e["parse_symbol"],
            "writer_symbol": None,
            "shared_body": None,
            "corpus_hits": None,
            "verdict": "named-untabled",
            "finding_id": None,
            "note": e["note"],
        })

    # 9 UNDOCUMENTED control-objects — seed unknowable.
    for unit, label in UNDOCUMENTED_CONTROLS:
        rows.append({
            "unit": unit,
            "row_ordinal": 0,
            "version_branch": "Common",
            "spec_line": None,
            "gate": "Common",
            "name": label,
            "type": "STRUCTURAL",
            "dxf_code": None,
            "descText_sha256": None,
            "reader_symbol": None,
            "writer_symbol": None,
            "shared_body": None,
            "corpus_hits": None,
            "verdict": "unknowable",
            "finding_id": None,
            "note": (
                "9 UNDOCUMENTED control-objects — round-3 M5 restores `unknowable` verdict. "
                "SCOPE (independent verification, 2026-07-18): this marker covers SEMANTICS "
                "only. The unit's real field rows (~10, verdict=unaudited, keyed by their own "
                "row_ordinal/version_branch — see addendum A.1-seam4) ARE structurally walkable "
                "and are NOT covered by this marker; they carry no literal key collision with "
                "it (this row's version_branch is the literal string \"Common\", which never "
                "appears as a real branch value). Per §2.2: field rows get walked, semantics do "
                "not — this row records only the latter."
            ),
        })

    # Aggregate stats — independently reproducible from the row list.
    n_spec_rows = sum(len(u["rows"]) for u in spec["units"])
    n_units = len(spec["units"])
    n_branch_cells = sum(1 for r in rows if r["verdict"] == "unaudited")
    n_named_untabled = sum(1 for r in rows if r["verdict"] == "named-untabled")
    n_unknowable = sum(1 for r in rows if r["verdict"] == "unknowable")
    n_bodies_total = len(bodies_all)             # every parseDwg/encodeDwg definition
    n_bodies_distinct_sym = len(body_by_sym)      # deduped by symbol name
    total_reads = code.get("meta", {}).get("total_read_tokens", 0)
    total_writes = code.get("meta", {}).get("total_write_tokens", 0)
    delegation_edges = len(code.get("meta", {}).get("delegation_edges", {}))
    verdict_counts: dict[str, int] = {}
    for r in rows:
        verdict_counts[r["verdict"]] = verdict_counts.get(r["verdict"], 0) + 1

    meta = {
        "row_identity": "(unit, row_ordinal, version_branch)",
        "verdict_enum": VERDICT_ENUM,
        "spec_units": n_units,
        "spec_data_rows": n_spec_rows,
        "code_bodies_total": n_bodies_total,           # every parseDwg/encodeDwg def
        "code_bodies_distinct_symbol": n_bodies_distinct_sym,  # after de-dup by symbol
        "code_overload_collisions": overload_collisions,  # round-3 M8 case
        "code_total_reads": total_reads,
        "code_total_writes": total_writes,
        "delegation_edges": delegation_edges,
        "branch_cells_unaudited": n_branch_cells,
        "named_untabled_seeded": n_named_untabled,
        "unknowable_seeded": n_unknowable,
        "total_rows": len(rows),
        "verdict_counts": verdict_counts,
        "corpus_hits_populated": bool(corpus),
    }
    return {"meta": meta, "rows": rows}


def _write() -> int:
    obj = _build_ledger()
    LEDGER.parent.mkdir(parents=True, exist_ok=True)
    with open(LEDGER, "w", encoding="utf-8") as f:
        json.dump(obj, f, indent=2, sort_keys=True, ensure_ascii=True)
        f.write("\n")
    print(f"wrote {LEDGER} — {obj['meta']['total_rows']} rows")
    return 0


def _check() -> int:
    if not LEDGER.exists():
        print("FAIL: spec_ledger.json missing — run --write first", file=sys.stderr)
        return 1
    fresh = _build_ledger()
    committed = json.loads(LEDGER.read_text())
    fresh_bytes = json.dumps(fresh, indent=2, sort_keys=True, ensure_ascii=True) + "\n"
    committed_bytes = LEDGER.read_text()
    if fresh_bytes != committed_bytes:
        # Show a short diff summary.
        print("FAIL: spec_ledger.json is stale — run --write to regenerate", file=sys.stderr)
        print(f"  fresh rows={fresh['meta']['total_rows']}  committed rows={committed['meta']['total_rows']}", file=sys.stderr)
        return 1
    print(f"PASS: spec_ledger.json byte-identical to fresh regen ({fresh['meta']['total_rows']} rows)")
    return 0


def _stats() -> int:
    obj = _build_ledger()
    m = obj["meta"]
    print("build_ledger stats (independently reproducible):")
    for k, v in m.items():
        if isinstance(v, dict):
            print(f"  {k}:")
            for k2, v2 in sorted(v.items()):
                print(f"      {k2}: {v2}")
        else:
            print(f"  {k}: {v}")
    # Independent aggregate check the coordinator specifically asked for:
    # verdict-counts sum equals total_rows (no row unaccounted for).
    vc_sum = sum(m["verdict_counts"].values())
    assert vc_sum == m["total_rows"], f"verdict-count sum {vc_sum} != total_rows {m['total_rows']}"
    print(f"\n  invariant OK: sum(verdict_counts) == total_rows ({vc_sum})")
    return 0


def _explain(unit: str) -> int:
    obj = _build_ledger()
    matched = [r for r in obj["rows"] if r.get("unit") == unit]
    if not matched:
        print(f"no rows for unit {unit}", file=sys.stderr)
        return 1
    print(json.dumps(matched, indent=2, sort_keys=True))
    return 0


def main() -> int:
    ap = argparse.ArgumentParser()
    g = ap.add_mutually_exclusive_group(required=True)
    g.add_argument("--write", action="store_true")
    g.add_argument("--check", action="store_true")
    g.add_argument("--stats", action="store_true")
    g.add_argument("--explain", metavar="UNIT")
    args = ap.parse_args()
    if args.write:
        return _write()
    if args.check:
        return _check()
    if args.stats:
        return _stats()
    if args.explain:
        return _explain(args.explain)
    return 2


if __name__ == "__main__":
    sys.exit(main())
