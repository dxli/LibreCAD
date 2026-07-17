# Shared Reviewer Brief — read before every unit

Plan §3 Step 0. These eight rules exist because the pilot re-derived them
104 times and got at least one wrong each time. **Do not skip this file.**

Every citation below MUST be re-verified against current master at run
time — this file is a checklist, not an index. Round-3 R.4 rule:
`Class::method` + semantic anchor is authoritative; **no absolute `.cpp`
line number is authoritative** (the file drifts; every dwgwriter15.cpp
line number the plan quoted has already gone stale). Anchors are given
below as `Class::method` — resolve via `bounds.py` or grep.

---

## 1. `DRW_DBG(a)` ALWAYS EVALUATES ITS ARGUMENT.

`intern/drw_dbg.h` — `#define DRW_DBG(a) DRW_dbg::dbg(a)` routes to
`DRW_dbg::dbg(T&&)`. `DRW_DBG(buf->getRawShort16())` **advances the
buffer**. Call sites rely on the side effect. An auditor who reads it
as a no-op files ~8 phantom "field never read → desync" criticals on
VPORT alone. Correct reading: **bits ARE consumed, values ARE
discarded** — the field is a legitimate value-only defect candidate,
not a desync.

## 2. Desync escape rule — the severity calibrator.

`DRW_Entity::parseDwgEntHandle` defaults `resetHandleStream=true` and
absolutely seeks to `objSize` for `version > AC1018`. Therefore an
R2007+ intra-entity desync **cannot cascade** to later entities.
`objSize` is a **BIT** count with version-dependent provenance: RL for
AC1015..AC1021, **computed** as `ms*8 - bs` for `>AC1021`, read LATE
for `<AC1015`. `seekObjectHandleStream` is a **no-op for ≤AC1018** —
any object parser that under-decodes its body AND relies on it reads
garbage handles on R2000/R2004 (~36 call sites per round-3 correction,
not 44).

## 3. `.orig` files shadow the real source.

There are ~8 `.orig` files in `libraries/libdxfrw/src/` (`drw_entities.cpp.orig`
is 213,941 bytes vs the live 412,869; also drw_header.cpp.orig, libdwgr.cpp.orig,
intern/dwgbufferw.{cpp,h}.orig, intern/dwgwriter{.h,15.cpp,15.h}.orig).
**All greps MUST use `--include=*.cpp --include=*.h`.**

## 4. Exact bounds, never a fixed window.

A 160-line window from `DRW_Layer::parseDwg` bleeds into `DRW_Block_Record`
and reports Layer as *having* the reactor loop. A naive `[start, next_start-1]`
range still gives false clears (round-3 verified: this is the exact error
in 6 of 9 originally-cited bounds ranges — Dimstyle真close is at 1274 not
1325). **Only true brace-matching resolves it** — always use
`scripts/conformance/bounds.py`, never a hand-count.

## 5. Two dwgread binaries disagree.

`/opt/homebrew/bin/dwgread` (released 0.13.3) vs
`~/dev/libredwg/programs/dwgread` (a libtool wrapper script,
`0.13.3.553_6d6a_dirty`). The `[.dwg_readback]` gate masks this by
defaulting to the dirty one. **Every oracle claim MUST name its
binary.** Pin `/opt/homebrew/bin/dwgread` for read-side reachability.

## 6. Code spec-annotations are NOT an index.

§20.4.95 is annotated for three different things (proxy graphics in
`drw_entities.cpp`, SPATIAL_INDEX and VISUALSTYLE in `drw_objects.cpp`).
§20.4.46 for both MTEXT and DICTIONARYVAR. §20.4.93 for both
SORTENTSTABLE and SCALE — while SCALE is also annotated §20.4.92.
Fewer than 40 of ~110 parsers carry any reference. **Resolve units from
`spec_fields.json` (produced by `extract_spec.py`), never from code
comments.**

## 7. "We have no storage for X" comments are PRESUMED STALE.

Three of HATCH's five findings trace to two such comments that were
simply false — `patternLines` and `pixelSize` both exist in
`DRW_Hatch` and are both wired to the DXF path. Grep the claimed-absent
member against the header before accepting any such comment (S3 stale-
comment lint enforces this mechanically).

## 8. Severity taxonomy.

- **DESYNC** — a field not consumed → cascades → *critical*.
- **VALUE-ONLY** — consumed-but-discarded, or transposed, or mutated →
  silent, survives round-trip → *medium*.

They look identical at a glance and need different fixes. **Empirical
prior: 12 of 16 pilot findings were value-only.** The realistic failure
mode in a mature reader is silent value loss, not catastrophic desync.
Set expectations accordingly.

---

## Authority order

For every conflict between the four planning documents:

**round3 > subplans §B0 > addendum > plan.**

Also, per round-3 R.4: `.cpp` absolute line numbers are advisory only;
resolve every citation at run time via `bounds.py` (Class::method +
semantic anchor).

## Additional round-3 rules

- **Licensing (B1).** The ODA spec text/PDF is developer-local and
  gitignored (`libraries/libdxfrw/spec/.gitignore`). The only committed
  reference is the sha256 pin in `scripts/conformance/SOURCES.json`.
  Do **not** commit verbatim `descText` prose from the spec into
  `spec_fields.json`/`spec_ledger.json`/`findings.json`; hash or
  paraphrase per R.1 option (a) or (b).
- **Substrate (B2).** Spec text is generated with
  `pdftotext -layout /Users/dli/doc/dwg/dwg.pdf …`. Identity check:
  14992 lines; `sed -n '10471p'` startswith `20.4.75 HATCH`. The
  30k-line `dwgTs/doc/dwg_spec.txt` is a **different** rendering — do
  not use it.
- **Method reframe (round-3 L1).** The uniquely oracle-invisible
  "symmetric-but-wrong" class is empirically ~2-3 of the pilot's 16
  findings, not all 16; the rest are un-canonicalized read-side
  divergences reachable by extending `dwgts_json_diff.py`'s
  `TYPE_TO_CANON`. Promote extended-canon dwgTs diff to a primary
  discovery method.
- **Corrections table (round-3).** These stale claims in the plan/
  subplans are superseded and MUST NOT be recopied into new work:
  - "23 DRW_UNUSED" → **~5** read-value discards + ~18 unused-parameter
    suppressions (split the figure).
  - "44 seekObjectHandleStream call sites" → **36** actual calls.
  - "26 crossing-asymmetric classes" → **24** (derive; don't
    hardcode).
  - "31,865-line spec" → **14,992** lines (the 31,865 file is the C1-
    forbidden dwgTs rendering).
  - "26 crossing-asymmetric" → 24. Derive at run time.
