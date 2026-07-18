# DECISION — field-hit counter caller attribution (round-3 H2)

**Status:** DECIDED — `__builtin_return_address(0)` + build-time symbolization.
**Slice this unblocks:** `00-O-fieldhit` (SP1.4).
**Recorded:** 2026-07-18, on branch `conformance/spec-review-2026-07`.

## The problem

Round-3 finding H2: SP1.4 originally proposed an inline hook in
`dwgbuffer` primitives (getBitDouble/BitShort/BitLong/BitExtrusion/…) that
records `(file:line, version) → count`, with the promise that this feeds
the ledger's `corpus_hits` column keyed by each parser's `reader_line`.

That promise cannot be kept as written: an inline hook inside the callee
records the *primitive's* `file:line` (e.g. `dwgbuffer.cpp:172`), NOT the
caller's `reader_line`. libdxfrw is C++17, so `std::source_location`
(C++20) is unavailable; `DRW_DBG(a)` was cited as a model, but it is a
call-site macro that captures its argument at the caller — it captures
nothing when moved into the callee.

Three real deliverables depend on getting real caller attribution:

1. **The unexercised-row list** (plan §3 Step 12, P6) — must know which
   parser lines never fired.
2. **The ledger `corpus_hits` column** (plan §8.2 / A.16) — keyed by
   `reader_line`.
3. **The "measured not guessed" honesty claim** (plan §9.5) —
   ad-hoc `dwgread -O JSON` cannot substitute per round-3.

## The two options

### Option A — `__builtin_return_address(0)` + build-time symbolization

At each primitive, call `__builtin_return_address(0)` to get the return
address. Emit `(return_addr, version)` per call. At exit, symbolize with
`atos` (macOS) or `addr2line` (Linux) against the linked binary to
recover `(reader_file, reader_line)`.

- **Pros:** true caller attribution; no source edits to parsers; matches
  the "no parser edits" acceptance criterion exactly.
- **Cons:**
  - Requires `-g` (debug info) at compile time — already the default in
    dev builds.
  - Inlining can move the return address up the stack; some parsers
    inline through helpers. Round-3 flagged this hazard explicitly.
    Mitigation: mark the primitives `__attribute__((noinline))` under
    `-DDRW_FIELD_COVERAGE`, so inlining does not perturb attribution.
    (Production builds without the flag remain byte-identical — the
    attribute is inside the ifdef.)
  - Requires `atos`/`addr2line` at post-processing time. Both are
    standard developer tools; documented in the merge script.

### Option B — accept per-primitive granularity

Log the primitive's own `file:line`. Rewrite the 3 dependent deliverables
to key on `dwgbuffer:line + version + parser_hint`. The parser_hint would
have to be a stack-context marker set at parser entry (e.g. thread-local),
requiring parser-side edits — which reintroduces the exact "no parser
edits" cost the counter was supposed to avoid.

- **Pros:** simple, no linker/symbolizer hazards.
- **Cons:** breaks the SP1.4 acceptance criterion, and either forces
  parser-side thread-local markers (defeating "no parser edits") or
  degrades the unexercised-row list to unexercised-primitive-call-site
  which is a much weaker signal.

## Decision

**Adopt Option A** — `__builtin_return_address(0)` + build-time
symbolization under `-DDRW_FIELD_COVERAGE`, with primitives marked
`__attribute__((noinline))` under the same flag guard.

The inlining hazard is the only real risk, and `__attribute__((noinline))`
under the ifdef guard eliminates it deterministically. The alternative
(Option B) sacrifices SP1.4's central promise. Since the counter's
compile-time-gated design already accepts a modest overhead in the
instrumented build, the noinline attribute is a proportionate cost.

## Concrete implementation contract (for the future 00-O-fieldhit slice)

Inside `intern/dwgbuffer.h` (and matching .cpp definitions), under
`#ifdef DRW_FIELD_COVERAGE`:

1. Each read primitive gets `__attribute__((noinline))`.
2. First statement: `void *pc = __builtin_return_address(0);`.
3. Emit `record(pc, version)` — record function is a thread-local
   append-only vector (round-3 field-hit reference to `DRW_dbg`'s
   `s_enabled` gate applies here for enabled/disabled toggle only, not
   as the record-target model).
4. At program exit or `dumpCoverage()`, iterate the recorded vector,
   deduplicate, and pipe addresses through `atos` (macOS) or
   `addr2line` (Linux) resolved against `argv[0]` binary + `.dSYM` bundle.
5. Output `docs/conformance/corpus_hits.json` schema:
   `{"reader_file:reader_line": {"total": N, "by_version": {"AC1015": n, "AC1018": n, ...}}}`.
6. Production build (without `-DDRW_FIELD_COVERAGE`) MUST be byte-
   identical on `[entity-encode]` — the mutation-sensitivity check
   (Option A introduces attributes only inside the ifdef, so this holds
   automatically).

## What this leaves open for the future 00-O-fieldhit slice

- Choose the record-buffer model (thread-local vs global-with-mutex).
  Recommendation: thread-local for lockless append, merged at
  `dumpCoverage()`.
- Confirm `atos` availability on this macOS. Fallback: link against
  `libbfd` and symbolize in-process (heavier build dependency; only
  needed if `atos` proves flaky).
- Format the `.dSYM` bundle path in `SOURCES.json` for reproducibility.

## Reference

Plan §4 P0 item 7 (originally "optional") → addendum A.10 (elevated to
REQUIRED) → subplans SP1.4 (attribution mechanism unstated) → round-3
H2 (blocker on the attribution mechanism). This decision closes H2.
