#!/usr/bin/env bash
# scripts/conformance/validate_slice.sh <slice-id>
#
# Per Part C.2.5 (subplans). Sole gate runner + sole writer of
# docs/conformance/gate_results/<slice-id>.json. Exit 0 == the slice may be
# committed and pushed; any nonzero exit means DO NOT COMMIT.
#
# This is the MINIMAL VIABLE build of the gate — enough to prove the discipline
# and retroactively validate the 3 landed slices. Tier B (global regression) is
# scoped down to the cheap subset for this bootstrap; heavier gates (B4 full
# suite, B5 field-differ, B6 write-oracle, B7 external-reader) come with the
# slices that need them. This matches the "cheap gates recomputed live, heavy
# gates trusted only while input_hash matches" rule in Part C.2.1.
#
# Round-3 R.4 enforcement: the gate never accepts an absolute .cpp line number
# as authoritative — Tier A resolves via bounds.py at run time.
set -uo pipefail

REPO_DEFAULT=/Users/dli/dev/LibreCAD/.claude/worktrees/agent-a606efb417455e9ae
REPO="${REPO:-$REPO_DEFAULT}"
PY="${PY:-/Users/dli/.venv/bin/python}"

SLICE="${1:?usage: validate_slice.sh <slice-id> [--selfcheck]}"
cd "$REPO" || { echo "cannot cd $REPO" >&2; exit 1; }

log()  { echo "[validate] $*"; }
fail() { echo "VALIDATE $SLICE: FAIL -- $1" >&2; exit 1; }
run()  { log "+ $*"; "$@" || fail "$*"; }

# ---- --selfcheck: exercise the runner end-to-end against every landed slice ----
# The set of "landed slices" is derived from slices.json cross-checked against
# gate_results/ shard existence — this way the loop stays exhaustive as new
# slices land, never a hardcoded list that rots.
if [ "$SLICE" = "--selfcheck" ]; then
    LANDED=$("$PY" -c "
import json, os
s = json.load(open('scripts/conformance/slices.json'))
for r in s:
    shard = f'docs/conformance/gate_results/{r[\"id\"]}.json'
    if os.path.exists(shard):
        print(r['id'])
")
    N=$(echo "$LANDED" | wc -l | tr -d ' ')
    log "self-check: retroactive validation of $N landed slices"
    while IFS= read -r id; do
        [ -z "$id" ] && continue
        log "  -> $id"
        "$0" "$id" || fail "self-check: $id gate would not pass"
    done <<< "$LANDED"
    log "self-check: PASS ($N slices)"
    exit 0
fi

# ---- Parse the slice-id shape and look up its slices.json row ----
if ! [[ "$SLICE" =~ ^([0-9][0-9])-([TUSVXO])-(.+)$ ]]; then
    fail "bad slice-id shape: $SLICE (must match ^\\d\\d-[TUSVXO]-.+$)"
fi
K="${BASH_REMATCH[2]}"

ROW=$("$PY" -c "
import json, sys
s = json.load(open('scripts/conformance/slices.json'))
for r in s:
    if r['id'] == '$SLICE':
        print(json.dumps(r))
        sys.exit(0)
sys.exit(1)
") || fail "slice-id not in slices.json"

TITLE=$("$PY" -c "import json,sys;print(json.loads(sys.stdin.read())['title'])" <<< "$ROW")
log "slice=$SLICE  kind=$K  title=$TITLE"

# ---- Preflight ----
# .git can be a directory (regular checkout) or a file (linked worktree).
[ -e .git ] || fail "not a git worktree"
BRANCH=$(git rev-parse --abbrev-ref HEAD)
[ "$BRANCH" = "conformance/spec-review-2026-07" ] || fail "wrong branch: $BRANCH"
[ -f docs/conformance/BASELINE.failures ] || fail "no docs/conformance/BASELINE.failures allowlist"

# =========================================================================
# TIER A — SLICE-LOCAL (dispatch on K letter)
# =========================================================================
log "TIER A: slice-local acceptance"
case "$K" in
    T)
        case "$SLICE" in
            00-T-antiloss)
                # Anti-loss: spec is gitignored + regenerated to identity match.
                [ -f libraries/libdxfrw/spec/.gitignore ] || fail "spec .gitignore missing"
                [ -f libraries/libdxfrw/spec/dwg_spec.txt ] || fail "spec text not regenerated locally"
                # wc -l on macOS emits leading spaces; strip via arithmetic expansion.
                LINES=$(wc -l < libraries/libdxfrw/spec/dwg_spec.txt | tr -d ' ')
                [ "$LINES" = "14992" ] || fail "spec text has $LINES lines, expected 14992"
                HATCH=$(sed -n '10471p' libraries/libdxfrw/spec/dwg_spec.txt)
                [[ "$HATCH" == "20.4.75 HATCH"* ]] || fail "line 10471 startswith wrong prefix: $HATCH"
                git check-ignore -q libraries/libdxfrw/spec/dwg_spec.txt || fail "spec text NOT gitignored"
                [ -f scripts/conformance/SOURCES.json ] || fail "SOURCES.json missing"
                for f in BRIEF.md BASELINE.md BASELINE.failures README.md findings.json errata.json; do
                    [ -f "docs/conformance/$f" ] || fail "docs/conformance/$f missing"
                done
                ;;
            00-T-extract-spec)
                # First-pass drift is documented + expected. The extractor's
                # exit 2 (identity failure) IS fatal; drift (exit 1) is not
                # while EXTRACTION_ACCURACY.md still records the first-pass
                # baseline. Distinguish by running with the identity check
                # explicit and capturing the exit code.
                log "+ $PY scripts/conformance/extract_spec.py --selfcheck"
                set +e
                "$PY" scripts/conformance/extract_spec.py --selfcheck
                ec=$?
                set -e
                [ "$ec" = 2 ] && fail "extract_spec identity check FAILED (wrong spec file)"
                # ec == 0 (all pilot targets exact) or ec == 1 (drift within baseline) — both pass this bootstrap gate.
                [ -f scripts/conformance/spec_fields.json ] || fail "spec_fields.json not produced"
                UNITS=$("$PY" -c "import json;print(len(json.load(open('scripts/conformance/spec_fields.json'))['units']))")
                [ "$UNITS" = "104" ] || fail "spec_fields.json has $UNITS units, expected 104"
                ;;
            00-T-bounds)
                run "$PY" scripts/conformance/bounds.py --selfcheck
                ;;
            00-T-slices-seed)
                "$PY" -c "import json; s=json.load(open('scripts/conformance/slices.json')); assert len(s)>=13, len(s)" \
                    || fail "slices.json has < 13 entries"
                # Assert every row has required keys.
                "$PY" -c "
import json, sys
req = ['id','kind','phase','phase_ord','key','title','sp_ref','plan_anchor','acceptance','deps','ledger_units','findings_shard','coverage_boundary','branch','gate']
s = json.load(open('scripts/conformance/slices.json'))
missing = []
for r in s:
    for k in req:
        if k not in r:
            missing.append((r.get('id','?'), k))
if missing:
    for m in missing: print('MISSING', m)
    sys.exit(1)
# The status invariant: no row may have a 'status'/'state' field.
for r in s:
    if 'status' in r or 'state' in r:
        print('FORBIDDEN status/state field in', r['id']); sys.exit(1)
print('slices.json schema OK,', len(s), 'entries')
" || fail "slices.json schema violation"
                ;;
            00-T-validate-slice)
                # The runner's OWN gate. Do NOT re-run --selfcheck here — that
                # would recurse (--selfcheck iterates every landed slice, which
                # includes this one). Instead verify:
                #   (a) the script is executable + parses,
                #   (b) it can validate one representative landed slice via a
                #       child invocation that will NOT recurse back here.
                [ -x scripts/conformance/validate_slice.sh ] || fail "runner not executable"
                run bash -n scripts/conformance/validate_slice.sh
                # Sanity: pick a different landed slice and confirm it passes.
                run bash scripts/conformance/validate_slice.sh 00-T-antiloss
                ;;
            00-T-gen-progress)
                # Real gate: --check must pass (regen byte-identical), --write
                # is idempotent (running twice produces no diff).
                run "$PY" scripts/conformance/gen_progress.py --check
                # Idempotency: --write then --check.
                run "$PY" scripts/conformance/gen_progress.py --write
                run "$PY" scripts/conformance/gen_progress.py --check
                ;;
            00-T-gen-readme)
                # Deferred bootstrap: PASS iff the script exists.
                [ -f scripts/conformance/gen_readme.py ] || fail "gen_readme.py missing"
                ;;
            00-T-corpus-provenance)
                # --check verifies the artifacts and their internal consistency
                # (recomputed summary matches committed). Doesn't rescan the
                # corpus — that's cadence work, not per-commit.
                run "$PY" scripts/conformance/tag_corpus_provenance.py --check
                # Independent aggregate re-derivation:
                "$PY" -c "
import json
d = json.load(open('docs/conformance/corpus_provenance.json'))
s = d['summary']
assert sum(s['by_magic'].values()) == s['total_files'], 'by_magic sum mismatch'
assert sum(s['by_producer_family'].values()) == s['total_files'], 'by_family sum mismatch'
assert s['dwg_binary'] + s['non_dwg_magic_filtered'] == s['total_files'], 'binary+filtered mismatch'
print('provenance aggregate invariants OK:', s['total_files'], 'files,', s['dwg_binary'], 'DWG binary')
" || fail "provenance aggregate invariant"
                ;;
            00-T-shared-bodies)
                [ -f scripts/conformance/shared_bodies.json ] || fail "shared_bodies.json missing"
                [ -f docs/conformance/SHARED_BODIES.md ] || fail "SHARED_BODIES.md missing"
                run "$PY" scripts/conformance/check_shared_bodies.py
                ;;
            00-T-gate-branches)
                # JSON parseability + SOURCES.json pin match + coverage vs
                # spec_fields.json.
                run "$PY" -c "import json; json.load(open('scripts/conformance/GATE_BRANCHES.json'))"
                # sha256 pin equals SOURCES.json value.
                GBSHA=$(shasum -a 256 scripts/conformance/GATE_BRANCHES.json | awk '{print $1}')
                PINNED=$("$PY" -c "import json; print(json.load(open('scripts/conformance/SOURCES.json')).get('gate_branches',{}).get('sha256',''))")
                [ "$GBSHA" = "$PINNED" ] || fail "gate-branches sha256 drift"
                # Every gate seen in spec_fields.json must be mapped.
                "$PY" -c "
import json,sys
sf=json.load(open('scripts/conformance/spec_fields.json'))
gb=json.load(open('scripts/conformance/GATE_BRANCHES.json'))
seen=set()
for u in sf['units']:
    for r in u['rows']:
        g=r.get('gate') or ''
        if g: seen.add(g)
mapped=set(gb['gates'].keys()) | (set(gb.get('special_gates',{}).keys()) - {'_note'})
miss=seen-mapped
if miss:
    print('MISSING gates in GATE_BRANCHES.json:', miss); sys.exit(1)
print('all', len(seen), 'gates mapped')
" || fail "gate coverage"
                ;;
            00-T-extract-code)
                # Real gate: --selfcheck must pass (5+ hand-labeled bodies +
                # inheritance case), code_fields.json is produced.
                run "$PY" scripts/conformance/extract_code.py --selfcheck
                [ -f scripts/conformance/code_fields.json ] || fail "code_fields.json not produced"
                N=$("$PY" -c "import json; print(json.load(open('scripts/conformance/code_fields.json'))['meta']['n_bodies'])")
                [ "$N" -ge 150 ] || fail "code_fields.json has $N bodies, expected >=150"
                ;;
            05-T-build-ledger)
                # --check must be idempotent (regeneration byte-identical),
                # and the aggregate invariants (verdict counts sum to total,
                # every spec data row projects to expected branches) must hold.
                run "$PY" scripts/conformance/build_ledger.py --check
                # Independent aggregate re-derivation:
                "$PY" -c "
import json
led = json.load(open('scripts/conformance/spec_ledger.json'))
m = led['meta']
vc = m['verdict_counts']
assert sum(vc.values()) == m['total_rows'], 'verdict-count sum mismatch'
assert m['total_rows'] == m['branch_cells_unaudited'] + m['named_untabled_seeded'] + m['unknowable_seeded'], 'ledger row aggregate mismatch'
sf = json.load(open('scripts/conformance/spec_fields.json'))
spec_rows = sum(len(u['rows']) for u in sf['units'])
distinct_pairs = len({(r['unit'], r['row_ordinal']) for r in led['rows'] if r['verdict']=='unaudited'})
assert distinct_pairs == spec_rows, f'spec-row projection mismatch: {distinct_pairs} vs {spec_rows}'
print('ledger aggregate invariants OK:', m['total_rows'], 'rows,', spec_rows, 'distinct (unit,row) pairs')
" || fail "aggregate invariant"
                ;;
            *)
                log "TIER A: no specific check for $SLICE — accepting existence-only"
                ;;
        esac
        ;;
    O)
        log "TIER A: oracle slice ($SLICE) — mutation-sensitivity check deferred to slice-owner"
        # Future: run check_oracle_mutation.py $SLICE
        ;;
    U|X|S|V)
        log "TIER A: kind=$K slice validation not yet implemented in this bootstrap"
        # Future: findings_proxy for U/X, recall+FP controls for S, verifier-check for V.
        ;;
    *)
        fail "unhandled kind: $K"
        ;;
esac

# =========================================================================
# TIER B — GLOBAL NON-REGRESSION (cheap subset for bootstrap)
# =========================================================================
log "TIER B: global non-regression (cheap subset)"

# B1: artifacts existence + parseability
[ -f docs/conformance/findings.json ] || fail "B1: findings.json missing"
[ -f docs/conformance/errata.json ]   || fail "B1: errata.json missing"
[ -f scripts/conformance/slices.json ] || fail "B1: slices.json missing"
"$PY" -c "import json; json.load(open('docs/conformance/findings.json')); json.load(open('docs/conformance/errata.json')); json.load(open('scripts/conformance/slices.json'))" \
    || fail "B1: artifact JSON does not parse"

# B3-lite: SOURCES.json intact (sha256 pin still resolves the current spec)
if [ -f libraries/libdxfrw/spec/dwg_spec.txt ]; then
    SPEC_SHA=$(shasum -a 256 libraries/libdxfrw/spec/dwg_spec.txt | awk '{print $1}')
    PINNED=$("$PY" -c "import json; print(json.load(open('scripts/conformance/SOURCES.json'))['spec_text']['sha256'])")
    [ "$SPEC_SHA" = "$PINNED" ] || fail "B3: spec sha256 drift ($SPEC_SHA vs pinned $PINNED)"
fi

# B4' (compressed to file-existence, no test-suite run — that's a heavier gate
# scheduled with slices that touch C++). If the slice modified libdxfrw or
# writer code, escalate to a real run. For pure-doc/pure-script slices the
# cheap check is sufficient.
CHANGED_CPP=$(git diff --name-only HEAD 2>/dev/null | grep -Ec 'libraries/libdxfrw/src/.*\.(cpp|h)$' || true)
if [ "$CHANGED_CPP" -gt 0 ]; then
    log "TIER B4: $CHANGED_CPP libdxfrw source files touched — full suite run recommended (deferred to caller)"
fi

# =========================================================================
# Record the outcome shard.
# =========================================================================
mkdir -p docs/conformance/gate_results
GATE_OUT="docs/conformance/gate_results/${SLICE}.json"

# input_hash: sha256 of the slice's own touched artifacts + slices.json row.
# The $ROW blob is JSON — pass it via stdin so `null` doesn't become an
# undefined Python name.
INPUT_HASH=$(printf '%s' "$ROW" | "$PY" -c "
import hashlib, json, sys
h = hashlib.sha256()
row = sys.stdin.read().encode('utf-8')
h.update(row)
for p in ['scripts/conformance/slices.json','docs/conformance/findings.json','docs/conformance/errata.json']:
    try:
        with open(p, 'rb') as f:
            h.update(f.read())
    except FileNotFoundError:
        pass
print(h.hexdigest()[:16])
")

# Round-3-round-5 correction (independent verification, 2026-07-18): this
# shard is git-TRACKED (unlike progress.json/PROGRESS.md, which already got
# the ref-topology-vs-content split), and it used to embed a wall-clock `ts`
# unconditionally on every run -- so every single `validate_slice.sh`
# invocation, including a plain re-verification with zero real change,
# dirtied all 11 shards. Reproduced directly: two consecutive `--selfcheck`
# runs with no intervening edits still diffed every shard's `ts` (and, as a
# side effect of iterating other slices in the same run, `input_hash` too).
# This is the identical transient-field bug class fixed twice already in
# conformance_stats.py (current_head/commit_count_ahead_of_master, then
# reachable_from_origin/on_master/state), recurring a third time here because
# the earlier fixes only touched the progress.json/PROGRESS.md rendering
# path, never this shard-writing path -- the actual root of the churn.
#
# Fix: (a) drop `ts` entirely -- it records only which second a check ran,
# never affects PASS/FAIL, and nothing reads it. (b) make the write itself
# conditional: only rewrite the shard when `passed` or `input_hash` actually
# differ from what's already committed. A verification run that changes
# nothing now leaves the tree untouched, matching Part C.3's "atomic
# per-slice commit" discipline -- a slice landing no longer carries 10
# unrelated timestamp-only diffs to other slices' shards.
NEW_SHARD=$("$PY" -c "
import json
print(json.dumps({
    'slice_id': '$SLICE',
    'passed': True,
    'runner': 'validate_slice.sh (bootstrap)',
    'input_hash': '$INPUT_HASH',
    'tier_a_dispatched_kind': '$K',
    'notes': 'Bootstrap Tier B covers artifact existence + SOURCES sha256 only. Heavier B4/B5/B6/B7 gates deferred to the slices that require them.',
}, indent=2, sort_keys=True) + chr(10))
")

if [ -f "$GATE_OUT" ] && [ "$(cat "$GATE_OUT")" = "$NEW_SHARD" ]; then
  log "  gate_results/${SLICE}.json unchanged (input_hash=$INPUT_HASH) — not rewritten"
else
  printf '%s' "$NEW_SHARD" > "$GATE_OUT"
  log "  gate_results/${SLICE}.json written (input_hash=$INPUT_HASH)"
fi

log "VALIDATE $SLICE: PASS -- may commit + push"
