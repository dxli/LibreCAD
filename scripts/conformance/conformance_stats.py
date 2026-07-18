"""conformance_stats.py — the single derivation function feeding both
gen_progress and gen_readme (subplans Part C.4.1). Every count, every state,
every aggregate flows through compute_progress() so no two generators can
disagree.
"""
from __future__ import annotations
import json
import subprocess
from pathlib import Path
from typing import Any

REPO = Path(__file__).resolve().parents[2]
SLICES = REPO / "scripts" / "conformance" / "slices.json"
GATE_RESULTS = REPO / "docs" / "conformance" / "gate_results"
FINDINGS = REPO / "docs" / "conformance" / "findings.json"
ERRATA = REPO / "docs" / "conformance" / "errata.json"
LEDGER = REPO / "scripts" / "conformance" / "spec_ledger.json"
CODE_FIELDS = REPO / "scripts" / "conformance" / "code_fields.json"
SPEC_FIELDS = REPO / "scripts" / "conformance" / "spec_fields.json"
BRANCH = "conformance/spec-review-2026-07"
REMOTE_TRACK = "dli"

# State derivation table per subplans C.0.4.
STATES = [
    "planned", "in-progress", "validated", "committed", "pushed",
    "recorded", "verified", "landed", "deferred",
]


def _load(p: Path) -> Any:
    if not p.exists():
        return None
    try:
        return json.loads(p.read_text())
    except Exception:
        return None


def _git(*args: str) -> str:
    r = subprocess.run(["git", *args], capture_output=True, text=True, cwd=REPO)
    return r.stdout.strip()


def _slice_commit_sha(slice_id: str) -> str | None:
    """Find the commit that introduced this slice's gate_result shard."""
    shard = f"docs/conformance/gate_results/{slice_id}.json"
    if not (GATE_RESULTS / f"{slice_id}.json").exists():
        return None
    sha = _git("log", "--diff-filter=A", "--format=%H", "-n", "1", "--", shard)
    return sha or None


def _reachable_from_origin(sha: str, branch: str = BRANCH,
                          remote: str = REMOTE_TRACK) -> bool:
    if not sha:
        return False
    r = subprocess.run(
        ["git", "merge-base", "--is-ancestor", sha, f"{remote}/{branch}"],
        capture_output=True, cwd=REPO,
    )
    return r.returncode == 0


def _on_master(sha: str) -> bool:
    if not sha:
        return False
    r = subprocess.run(
        ["git", "merge-base", "--is-ancestor", sha, f"{REMOTE_TRACK}/master"],
        capture_output=True, cwd=REPO,
    )
    return r.returncode == 0


def _derive_state(slice_row: dict, gate_shard: dict | None,
                  commit_sha: str | None,
                  reachable: bool, on_master: bool) -> str:
    """Derive the highest state the evidence supports (never latch)."""
    if not gate_shard or not gate_shard.get("passed"):
        return "planned"
    # Validated at minimum.
    if not commit_sha:
        return "validated"
    if not reachable:
        return "committed"
    if on_master:
        return "landed"
    return "pushed"


def compute_progress() -> dict:
    slices = _load(SLICES) or []
    findings = _load(FINDINGS) or []
    errata = _load(ERRATA) or []
    ledger = _load(LEDGER)
    code = _load(CODE_FIELDS)
    spec = _load(SPEC_FIELDS)

    per_slice: list[dict] = []
    state_counts: dict[str, int] = {s: 0 for s in STATES}
    for row in slices:
        sid = row["id"]
        shard = _load(GATE_RESULTS / f"{sid}.json")
        sha = _slice_commit_sha(sid)
        reachable = _reachable_from_origin(sha) if sha else False
        on_master = _on_master(sha) if sha else False
        state = _derive_state(row, shard, sha, reachable, on_master)
        state_counts[state] = state_counts.get(state, 0) + 1
        per_slice.append({
            "id": sid,
            "kind": row["kind"],
            "phase": row["phase"],
            "phase_ord": row["phase_ord"],
            "title": row["title"],
            "state": state,
            "commit_sha": sha,
            "reachable_from_origin": reachable,
            "on_master": on_master,
            "gate_passed": bool(shard and shard.get("passed")),
            "input_hash": (shard or {}).get("input_hash"),
        })

    ledger_meta = (ledger or {}).get("meta", {}) if ledger else {}
    code_meta = (code or {}).get("meta", {}) if code else {}
    spec_meta = (spec or {}).get("meta", {}) if spec else {}

    # Aggregate scorecard — every number derived from an in-repo artifact.
    # Deliberately NOT embedding `current_head` or `commit_count_ahead_of_master`:
    # both change on every commit including PROGRESS.md's own commit, producing
    # a chase-your-tail dynamic where --check fails immediately after --write.
    # Transient git state is queried live at CI time, never persisted here.
    aggregate = {
        "branch": BRANCH,
        "remote": REMOTE_TRACK,
        "spec_units": spec_meta.get("units_count"),
        "spec_data_rows": sum(
            u.get("row_count", 0) for u in (spec or {}).get("units", [])
        ) if spec else None,
        "code_bodies_total": code_meta.get("n_bodies"),
        "code_total_reads": code_meta.get("total_read_tokens"),
        "code_total_writes": code_meta.get("total_write_tokens"),
        "code_delegation_edges": len(code_meta.get("delegation_edges") or {}),
        "code_drw_unused_real": (
            code_meta.get("drw_unused_split", {}).get("real_read_discards")
        ),
        "code_drw_unused_param": (
            code_meta.get("drw_unused_split", {}).get("unused_param_suppressions")
        ),
        "ledger_total_rows": ledger_meta.get("total_rows"),
        "ledger_branch_cells_unaudited": ledger_meta.get("branch_cells_unaudited"),
        "ledger_named_untabled_seeded": ledger_meta.get("named_untabled_seeded"),
        "ledger_unknowable_seeded": ledger_meta.get("unknowable_seeded"),
        "ledger_verdict_counts": ledger_meta.get("verdict_counts"),
        "findings_count": len(findings) if isinstance(findings, list) else None,
        "errata_count": len(errata) if isinstance(errata, list) else None,
    }
    return {
        "aggregate": aggregate,
        "per_slice": per_slice,
        "state_counts": state_counts,
    }
