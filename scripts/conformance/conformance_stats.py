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


# Round-3-round-4 correction (independent verification, 2026-07-18): the
# original compute_progress() persisted reachable_from_origin/on_master/state
# (and the state_counts derived from them) straight into progress.json and
# PROGRESS.md, both of which gen_progress.py --check byte-diffs against a
# fresh regen. Those three fields are NOT a function of repo tree content --
# they are live facts about whether a commit SHA is an ancestor of a
# *remote-tracking ref*, which moves independently of repo content every time
# this actively-pushed branch is pushed. Reproduced concretely: pushing with
# zero content change (simulated via `git update-ref refs/remotes/dli/<branch>
# HEAD`, then fully reverted) flips reachable_from_origin from false to true
# for every previously-committed slice, which changes the byte-diffed file
# without any new commit -- the exact "chase-your-tail" pathology this slice
# was built to eliminate, just recurring at push-granularity instead of
# commit-granularity. This is the identical bug class as the current_head /
# commit_count_ahead_of_master fields removed at the prior landing; those two
# specific fields being gone did not close the bug CLASS, only two instances
# of it.
#
# Fix: split the derivation into CONTENT (pure function of repo tree state;
# safe to byte-diff, and diffed strictly by --check with no exceptions) and
# LIVE (a function of the moving remote-tracking ref; computed fresh on
# demand via --status, never persisted into a --check-gated file). This is
# the same "developer-local, not committed" pattern already used for the
# spec text (B1/B2) and the SOURCES.json sha256 pins.
LIVE_ONLY_FIELDS = ("reachable_from_origin", "on_master", "state")


def compute_progress() -> dict:
    """CONTENT-ONLY derivation: a pure function of repo tree state, safe for
    gen_progress.py --check's strict byte-identity contract. Never includes
    reachable_from_origin/on_master/state -- those are LIVE (ref-topology)
    facts computed only by compute_live_status(), which is never persisted
    into a --check-gated file. See LIVE_ONLY_FIELDS above."""
    slices = _load(SLICES) or []
    findings = _load(FINDINGS) or []
    errata = _load(ERRATA) or []
    ledger = _load(LEDGER)
    code = _load(CODE_FIELDS)
    spec = _load(SPEC_FIELDS)

    per_slice: list[dict] = []
    for row in slices:
        sid = row["id"]
        shard = _load(GATE_RESULTS / f"{sid}.json")
        sha = _slice_commit_sha(sid)
        per_slice.append({
            "id": sid,
            "kind": row["kind"],
            "phase": row["phase"],
            "phase_ord": row["phase_ord"],
            "title": row["title"],
            "commit_sha": sha,
            # Purely content-derived: does a passing gate shard + a landing
            # commit exist. No ref-topology query, so this never drifts from
            # an external push -- unlike "pushed"/"landed", it only changes
            # when THIS slice's own evidence changes.
            "committed": bool(sha and shard and shard.get("passed")),
            "gate_passed": bool(shard and shard.get("passed")),
            # input_hash and ts are DELIBERATELY NOT persisted here. See
            # HANDOFF: input_hash currently hashes the full slices.json
            # (validate_slice.sh scoping bug), so any slice-registry edit
            # invalidates every hash. Persisting it into --check-gated files
            # would churn PROGRESS.md/progress.json on every unrelated commit.
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
    # Content-only summary: "committed" is purely evidence-of-this-slice
    # (no ref-topology query), so this count is stable across a push, unlike
    # the old state_counts (which mixed in reachable/on_master and therefore
    # moved every time the branch was pushed).
    committed_count = sum(1 for r in per_slice if r["committed"])
    return {
        "aggregate": aggregate,
        "per_slice": per_slice,
        "committed_count": committed_count,
        "planned_count": len(per_slice) - committed_count,
    }


def compute_live_status() -> dict:
    """LIVE ref-topology derivation: reachable_from_origin/on_master/state
    per slice, computed fresh from the CURRENT remote-tracking refs. Never
    written to a --check-gated file (see LIVE_ONLY_FIELDS above) -- call this
    on demand via `gen_progress.py --status` when you actually want to know
    push/landed status. It legitimately changes on every push with zero
    content change; that is expected, not staleness."""
    slices = _load(SLICES) or []
    live: list[dict] = []
    state_counts: dict[str, int] = {s: 0 for s in STATES}
    for row in slices:
        sid = row["id"]
        shard = _load(GATE_RESULTS / f"{sid}.json")
        sha = _slice_commit_sha(sid)
        reachable = _reachable_from_origin(sha) if sha else False
        on_master = _on_master(sha) if sha else False
        state = _derive_state(row, shard, sha, reachable, on_master)
        state_counts[state] = state_counts.get(state, 0) + 1
        live.append({
            "id": sid,
            "commit_sha": sha,
            "reachable_from_origin": reachable,
            "on_master": on_master,
            "state": state,
        })
    return {"per_slice": live, "state_counts": state_counts}
