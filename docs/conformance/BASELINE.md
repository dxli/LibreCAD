# Baseline — Standing test-suite failures

Recorded per plan §3 Step 1 / SP1.8 acceptance. The tree is NOT green: any
reviewer must confirm a "new" failure attribution against this allowlist
(subset check `comm -13 <(sort -u BASELINE.failures) <observed>`), never
attribute a failure to their work without proof.

## Observed on master at branch creation

Two `dwg_write_smoke_tests.cpp` failures reported by the plan / round-3
reviews at:

- `dwg_write_smoke_tests.cpp:6454` — "RS_Insert::update Block is nullptr"
- `dwg_write_smoke_tests.cpp:7161` — same signature

The plan's original headline "773 cases / 766 pass / 7 FAIL" was recorded
against a dirty 35-file working tree. This worktree is CLEAN (no dirty
files), so the observed failure set may be smaller — the two lines above
are the ones consistently attributed in the plan/addendum text.

`validate_slice.sh` uses this file as the subset allowlist (Tier B4). A
future slice touching a writer that adds a NEW failure line here is
handled by the C.2.6 attribution procedure — never by relaxing the
allowlist.

## Provenance

- Branch: `conformance/spec-review-2026-07`
- Parent commit: `f2143b879` (master tip at branch creation, 2026-07-18)
- Slice creating this file: `00-T-antiloss`

## Updating this file

Do NOT hand-edit to silence a new failure. If a genuine baseline change
is warranted (upstream fixed one of these), open a new slice, record
the SHA that changed it, and re-run the suite to derive the new set —
same discipline that guards every generated artifact in this directory.
