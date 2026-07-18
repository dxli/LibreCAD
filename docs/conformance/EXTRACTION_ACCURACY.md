# Extraction Accuracy — extract_spec.py measured drift

Plan §4 P0 acceptance requires reproducing pilot row counts exactly. This
first-pass extractor gets close but does not yet hit them exactly. This
document publishes the measured deltas so downstream slices (build_ledger,
sweeps, ledger --check) know what to expect.

## Measured this run

Recorded at slice `00-T-extract-spec` first landing:

| Unit  | Name    | Pilot target | Observed | Delta | Status                              |
|-------|---------|--------------|----------|-------|-------------------------------------|
| 20.4.40 | SPLINE  | 20 data + 3 xref | 19 + 3 | −1 | close — row-count drift only         |
| 20.4.64 | VPORT   | 58            | 57 + 1 | −1 | close                               |
| 20.4.75 | HATCH   | 68            | 67 + 3 | −1 | close                               |
| 20.4.46 | MTEXT   | 35 data + 3 xref | 37 + 1 | +2 | over — likely a two-line fold FP    |

Aggregate: **104 units enumerated (exact), 1345 data rows extracted, 143
xref rows extracted**. Plan reference "measured 1466" from the recon.
Total delta: −121 rows (~8.3%). All 104 unit boundaries `(start, fend)` are
correctly detected.

## Known extractor limitations documented here

These are surfaced by the current pass and are candidates for the P0 hardening
follow-up slice. They do NOT block P1 (ledger build), but the ledger must
disposition their rows with the appropriate `unaudited-extractor-gap` verdict
so `--check` does not silently pass.

1. **§20.4.2 Common Entity Handle Data** — 0 data rows extracted; expected 12.
   The section uses a group-code-leading layout (`8  LAYER (hard pointer)`)
   without a DWG type token per row. Requires a §20.4.2-specific parser branch
   or a hand-authored `spec_fields_overrides.json`.
2. **§20.4.7 ENDBLK, §20.4.8 SEQEND, §20.4.45 DICTIONARYWDFLT** — 0 rows;
   each expected 1. Their normative content is a single Handle row in an
   unusual layout. Same fix as above.
3. **§20.4.101 TABLESTYLE, §20.4.96 TABLE** — 0 rows. Both deferred per plan
   §5 Deferred; no immediate action needed.
4. **§20.4.13/14 VERTEX (MESH)/(PFACE), §20.4.43 XLINE** — 0 rows expected
   (cross-references only) — correctly extracted.
5. **descText coverage** — 68.3% of data rows have a description hash
   populated. Plan target is ≥95%. The gap is real: many spec rows are
   bare (`View target 3BD 17`) with no prose description column. This is
   a **spec artifact, not an extractor bug**.
6. **Two-line wrapping** — a `name-only-on-line-N` + `<TYPE> <code> <desc>-on-line-N+1`
   fold is implemented but appears to over-match on ~2 MTEXT rows.
   Candidate for tightening.

## descText handling (round-3 R.1 licensing)

Per round-3 B1 licensing blocker: the ODA spec text is copyrighted with no
redistribution grant. `spec_fields.json` stores `descText_sha256` (16-hex
prefix of sha256) instead of verbatim prose. The plan's phrase "descText
non-empty ≥95%" is satisfied only by the hash being populated; downstream
sweeps that need the verbatim text (the crossing-grep in P4.5, S5.3 prose
patterns) MUST read the developer-local `libraries/libdxfrw/spec/dwg_spec.txt`
at run time and never persist prose to git.

## Gate whitelist coverage

`extract_spec.py` runs with `--strict-gates=False` by default. Under strict
mode it hard-fails on any unknown gate string, per plan §4 P0 item 2. All
104 units currently parse under both modes.

## Regeneration

```
pdftotext -layout /Users/dli/doc/dwg/dwg.pdf libraries/libdxfrw/spec/dwg_spec.txt
/Users/dli/.venv/bin/python scripts/conformance/extract_spec.py --selfcheck
```

Any drift in the observed numbers above is either an extractor tightening
(fix here) or a spec-text upgrade (bump SOURCES.json's sha256).
