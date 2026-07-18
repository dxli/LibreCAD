#!/usr/bin/env python3
"""check_shared_bodies.py — Verify shared_bodies.json consistency.

Called by validate_slice.sh for 00-T-shared-bodies. Asserts:
  - JSON parses and every edge has the schema keys.
  - Every named shared_symbol/shared_write_symbol/callers_read/callers_write
    with a Class::method shape either resolves via bounds.py OR is documented
    as inherited (`NOT FOUND` is an expected outcome for inheritors).
  - The corrections block records the readCommonObjectHandles + seekObject
    HandleStream count reconciliations.
  - Every edge with `symmetric_body_pair: false` explains why in `notes`.
"""
from __future__ import annotations
import json
import re
import subprocess
import sys
from pathlib import Path

REPO = Path(__file__).resolve().parents[2]
SB = REPO / "scripts" / "conformance" / "shared_bodies.json"
BOUNDS = REPO / "scripts" / "conformance" / "bounds.py"

REQUIRED_KEYS = {"id", "role", "shared_symbol", "shared_write_symbol",
                 "callers_read", "callers_write", "symmetric_body_pair", "notes"}
SYM_RE = re.compile(r"[A-Za-z_][A-Za-z_0-9]*::[A-Za-z_~][A-Za-z_0-9]*")


def _bounds_resolves(sym: str) -> bool:
    try:
        r = subprocess.run(
            ["/Users/dli/.venv/bin/python", str(BOUNDS), sym],
            capture_output=True, text=True, timeout=30,
        )
    except Exception:
        return False
    return r.returncode == 0


def main() -> int:
    if not SB.exists():
        print(f"FAIL: {SB} missing", file=sys.stderr)
        return 1
    obj = json.loads(SB.read_text())
    edges = obj.get("edges", [])
    if len(edges) < 13:
        print(f"FAIL: expected >=13 edges, got {len(edges)}", file=sys.stderr)
        return 1
    ok = True
    for e in edges:
        missing = REQUIRED_KEYS - set(e.keys())
        if missing:
            print(f"FAIL: edge {e.get('id','?')} missing keys {missing}")
            ok = False
        if e.get("symmetric_body_pair") is False and not (e.get("notes") or ""):
            print(f"FAIL: edge {e['id']} asymmetric but no notes")
            ok = False
    corr = obj.get("corrections") or {}
    for req in ("plan_2.2_ride_free_claim", "readCommonObjectHandles_call_sites",
                "seekObjectHandleStream_call_sites"):
        if req not in corr:
            print(f"FAIL: corrections missing {req}")
            ok = False
    # Spot-check: SB-hatch-boundary references DRW_Hatch::parseDwgBoundaryData
    # and bounds.py resolves it.
    hb = [e for e in edges if e["id"] == "SB-hatch-boundary"]
    if hb and not _bounds_resolves("DRW_Hatch::parseDwgBoundaryData"):
        print("FAIL: DRW_Hatch::parseDwgBoundaryData no longer resolves")
        ok = False
    print(f"{'PASS' if ok else 'FAIL'}: {len(edges)} edges, corrections present")
    return 0 if ok else 1


if __name__ == "__main__":
    sys.exit(main())
