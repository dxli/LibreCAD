# Shared-Body Map — 13 edges

Plan §4 P0 item 4 + addendum A.2. Every entry below cites `Class::method` +
was resolved via `scripts/conformance/bounds.py` at authoring time. Any
absolute `.cpp` line number in the machine artifact `shared_bodies.json`
is orientation metadata only; downstream slices resolve at run time
(round-3 R.4).

## Why this map matters

- **Batching** — auditing a shared body once fans out its finding across the
  callers automatically. HATCH-1 (spline-edge tangent asymmetry) expanded to
  MPOLYGON at zero additional cost.
- **Anti-single-caller-fix** — a naive "fix this in HATCH" that ignores the
  MPOLYGON caller is exactly the fix-trap the plan §6.3 Q4 warns about.
- **Delegation guard** — `extract_code.py` (SP1.3) must recognise pure
  delegation and NOT report inheritors as asymmetric. Verified NOT FOUND
  via bounds.py this session: `DRW_DimLinear::parseDwg`,
  `DRW_Xline::parseDwg/encodeDwg`, `DRW_Table::encodeDwg` — all inherit.

## The 13 edges

Each shared body is one row; the code+scope details live in
`scripts/conformance/shared_bodies.json`.

| ID                              | Role                       | Symmetric | Callers |
|---------------------------------|----------------------------|-----------|---------|
| SB-hatch-boundary               | boundary-loop-body         | yes       | Hatch, MPolygon |
| SB-spline-body                  | spline-decode-body         | yes       | Spline, Helix |
| SB-mtext-embedded               | embedded-mtext-parser      | **no**    | MText, Attrib, Attdef (read shared / write inlined) |
| SB-ray-xline                    | shared-encoder             | yes       | Ray, Xline (inherits, no own body) |
| SB-insert                       | shared-encoder             | **no**    | Insert, MInsert, **Table (silent-INSERT bug)** |
| SB-trace-solid                  | shared-decoder-family      | **no**    | Trace, Solid (parseDwg delegates, encodeDwg does NOT) |
| SB-image-line-shadow            | shadowing-inheritance      | **no**    | Image::encodeDwg SHADOWS Line::encodeDwg |
| SB-dim-base                     | dim-shared-encoder         | yes       | all 8 typed dims |
| SB-readCommonObjectHandles      | shared-utility             | **no**    | 40 sites in drw_objects.cpp |
| SB-objControl                   | shared-control-parser      | **no**    | BLOCK CONTROL + 9 UNDOCUMENTED |
| SB-DimLinear-inherit            | inheritance-silent         | yes       | DimLinear inherits DimAligned |
| SB-Wipeout-total-delegation     | asymmetric-inheritance     | **no**    | Wipeout parseDwg = 3-line delegation; encodeDwg = 49 own lines |
| SB-Attdef-parseCode-fall        | parseCode-fallthrough      | **no**    | Attdef::parseCode → Attrib::parseCode |

## Corrections to the parent docs

### Plan §2.2 "MPOLYGON and HELIX ride free" — overstated

The **sub-bodies** (`parseDwgBoundaryData`, `parseDwgSplineBody`) do ride
free. But MPOLYGON and HELIX both have byte-parallel **top-level**
`parseDwg` bodies at `drw_entities.cpp:6491` (MPOLYGON) and `:7307` (HELIX)
that are independently maintained. A boundary-body finding transfers for
free; a top-level layout finding does NOT. Both need their own row walks
per addendum A.2.

### `readCommonObjectHandles` call-site count

Plan estimate: 28. **Actual verified count: 40** (grep-count in
`drw_objects.cpp` this session). The plan under-counted; the S5.2 sweep's
call-site enumeration must run mechanically, not from the plan estimate.

### `seekObjectHandleStream` call-site count

Plan: 44. Round-3: 36. Verified this session: raw grep of 38 hits −
1 definition (drw_objects.cpp:59 per BRIEF) − 1 comment mention = **36**.
Matches round-3.

## What the `NOT FOUND` bounds.py results teach

Three symbols the plan/subplans cite that bounds.py cannot resolve — all
are correct inheritance patterns, all are silent (no code says "inherit"):

| Symbol                             | Truth                                     |
|------------------------------------|-------------------------------------------|
| `DRW_DimLinear::parseDwg`          | inherits DRW_DimAligned::parseDwg          |
| `DRW_Xline::parseDwg` / `::encodeDwg` | inherits DRW_Ray                         |
| `DRW_Table::encodeDwg`             | inherits DRW_Insert (writes ACAD_TABLE as plain INSERT with oType=7) — the plan §5 rank 6 silent-INSERT bug |

`extract_code.py` (SP1.3) must consult `shared_bodies.json` and mark these
as `inherited-from:<base>` rather than reporting them as parseDwg-missing
or symmetry-broken — otherwise the S2 sweep's top-of-list would be full
of pure-delegation false positives.
