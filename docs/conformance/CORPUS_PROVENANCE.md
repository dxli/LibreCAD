# CORPUS_PROVENANCE.md — round-3 M9 prerequisite for 00-O-corpus-wire

Machine artifact: `docs/conformance/corpus_provenance.json` (full per-file tags).

Regenerate with `scripts/conformance/tag_corpus_provenance.py --write`.

## Why this exists (round-3 M9)

The plan's honesty claim "corpus expands ~54 → ~750" is inflated by:
- **Non-DWG-magic files masquerading as `.dwg`** (DXF ASCII exports, empty containers).
- **ODA/Teigha rewrites**, which are byte-identical to what the ODA spec _says_ AutoCAD does, so they're a weak physics check when the spec is what's being questioned.

This file tags every corpus DWG so downstream sweeps can filter and the §6.5 physics tiebreaker can prefer genuine-AutoCAD files.

## Summary

- Total files scanned: **670**
- Genuine DWG binary (AC1xxx/AC2xxx magic): **641**
- Non-DWG magic (filtered from sweeps): **29**

### By corpus root
- `~/doc/dwg5`: 280
- `~/doc/dwg6`: 390

### By DWG magic version
- `AC1021`: 102
- `AC1024`: 99
- `AC1015`: 87
- `AC1027`: 85
- `AC1032`: 82
- `AC1018`: 71
- `AC1009`: 67
- `AC1014`: 33
- `999
dx`: 23
- `AC1012`: 5
- `AC2.10`: 4
- `AC1003`: 4
- `AC1006`: 4
- `AC1.40`: 2
- `AC1004`: 2

### By producer family
- **unknown**: 622
- **genuine-AutoCAD**: 32
- **ODA-Teigha**: 16

## Downstream discipline (round-3 M9)

The `00-O-corpus-wire` slice, when it lands, MUST:
1. Read `corpus_provenance.json` and filter `is_dwg_binary == True` before feeding files to sweeps.
2. For §6.5 physics tiebreak on ODA-spec-vs-corpus contests, prefer `producer_family == 'genuine-AutoCAD'` files; downgrade ODA-Teigha-only support to OBSERVATION.
3. Report the `producer_family` breakdown alongside every sweep's recall/FP numbers so the honesty story isn't a raw file count.

## Signature-detection limits

- Signatures are sniffed from the first 32KB of each file (`SNIFF_BYTES`). ODA Teigha's producer string in AppInfo is typically well within that window, but a DWG whose AppInfo is R2004+-LZ77-compressed and lands past 32KB is silently `producer_family="unknown"`. Verified true for the corpus at hand; escalate to full-decoding if a future sweep's honesty gate demands it.
- **The 32KB/compression explanation is not the only reason a file lands in `unknown`.** Independently verified 2026-07-18: several pre-R2004 files in the corpus (AC1015/AC1009, sampled) are ALSO `unknown`, and for those the compression story doesn't apply at all — AppInfo compression is an R2004+ mechanism, so a pre-R2004 file's true reason is simply "no vendor signature string was ever embedded in the file to begin with," not "it's there but past the sniff window." Both explanations are real and distinct; do not assume every `unknown` file is a compression-window miss.
- Signature ordering: Teigha → ODA → AutoCAD → third-party → libdxfrw. A DWG that Teigha REWROTE from an AutoCAD original will match Teigha first (correct — the last writer to touch it is what determines the byte-level fingerprint).

## Enumeration semantics — case-insensitive `.dwg` suffix

This script matches `f.suffix.lower() == '.dwg'`, so `dtm_2023.Dwg` (uppercase `D`) is counted. A shell `ls *.dwg` or a `find -name '*.dwg'` will NOT match that file — one such file exists in `~/doc/dwg6`. Downstream tools that cross-check counts against shell recount must use `find -iname '*.dwg'` for the same case-insensitive semantics or their assertions will be off by 1.

