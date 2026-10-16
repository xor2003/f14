# F-14 function map / comparison artifacts

Regenerated from `src/F14.EXE` (dev snapshot with compacted CodeView NB02)
and the local `build/obj/F14.EXE` + `F14.MAP`. See f19ru
`DECOMPILATION.md` §16 for the method.

- `cvpubs.txt` — CodeView public symbols `para:off name` (4465 records)
- `funcs.txt` — Ghidra function inventory `seg:off FUN_x_y size` (1018)
- `ranges.txt` — Ghidra per-function instruction R-blocks
- `f14_ghidra.map` — mzretools-format map of the original exe (932 named)
- `funcmap.txt` — module | local addr | name | cmp status (1022 pubs)
- `cmp_rel.txt` — per-function verdicts (EXACT/NEAR/DIFF)
- `rename.txt` — Ghidra `seg:off name` pairs used by RenameFuncs.java
- `DumpFuncs.java` / `RenameFuncs.java` — Ghidra headless postScripts

Compare: `python3 tools/cmpfuncs.py src/F14.EXE analysis/f14_ghidra.map build/obj/F14.EXE build/obj/F14.MAP`

## Gold (retail) F14GOLD.EXE matching

`analysis/gold/` — the shipped 399KB binary analyzed the same way:

- `funcs_gold.txt` / `ranges.txt` / `memmap.txt` — Ghidra dump (1068 funcs)
- `matches.txt` — fuzzy match gold func -> dev func name (mnemonic-multiset
  Jaccard + difflib LCS; `tools/matchgold.py`)
- `gold_named.map` — mzretools map with matched names applied (274 funcs)
- `mzdiff_results.txt` / `mzdiff_loose.txt` — per-function mzdiff verdicts:
  strict: 43 MATCH / 188 DIFF; `--loose` (reloc-tolerant): **132 MATCH / 99 DIFF**

Regenerate: `python3 tools/matchgold.py F14GOLD.EXE ranges.txt src/F14.EXE \
  dev_ranges.txt cvpubs.txt` then `tools/ghidra2mzmap.py memmap.txt ranges.txt \
  6173e rename_gold.txt > gold_named.map` and run mzdiff per-pair:

  mzdiff --loose --map <single-routine dev map> --tmap gold_named.map --nocall \
    src/F14.EXE:NAME F14GOLD.EXE:NAME

NOTE: needs the mzretools patch seeding target segments from --tmap
(mzretools commit — loadExe registers map segs on the target).

### Gold vs local build (the .C sources compiled)

`matches_my.txt` — gold funcs matched against the LOCAL build's symbols
(F14.MAP publics, i.e. the .C functions): 715 matched, 188 at LCS>=0.9.
`gold_mybuild_named.map` + `my_named.map` pair routines by name;
`mzdiff_mybuild_loose.txt` + `verify_sweep.txt`: **96 MATCH** unique names
verified by `mzdiff --loose --nocall` — these .C/.ASM source functions
compile to the same code as the retail binary modulo relocation/literal
fields. The remaining ~37 unresolved names are nearly all `.ASM` module
functions (PASM/EMS/3D/JOYSTX/MATH/MCGAMASK/EMM) — real source drift
(e.g. `EMSfree` near-call -> far-call to EMSinit), NOT UASM-vs-MASM
encoding (pristine EMS.ASM assembled with the bundled MASM 6.14 under
Wine produces byte-identical code to the UASM build). `gold_to_source.tsv`
is the consolidated gold_addr -> name -> source_module -> score table.

DIFF includes Ghidra boundary artifacts (function split differently at
entry), so DIFF means "unverified", not proven different.

### Compiler codegen notes (dev vs gold vs rebuild)

Opcode-density scan of the load images and prologue inspection
(`_mono_scroll` at dev image 0x28):

- **Gold retail** — 2602 `push imm16` (0x68), `enter`/`leave` prologues:
  286 codegen (`/G2`) + optimize — same ISA family as the makefile's
  `/Osegcl /G2`. This is why the local `/Osegcl /G2` build verifies
  85+ functions byte-modulo-reloc identical to gold.
- **Dev snapshot (src/F14.EXE)** — only ~326 `push imm16`; `_mono_scroll`
  uses `push bp; mov bp,sp; sub sp,0xa`, locals kept in memory,
  `mov ax,imm; push ax` argument pushes: a **debug-style build**
  (8086/186 codegen + no register promotion), consistent with the
  embedded `/Zi` CodeView info. It is NOT a codegen oracle for retail;
  its function names/module layout are still valid for gold matching.
- Local `c6/` CL.EXE is **MSC 6.00 Alpha** (Feb 15 1990): under `/Od`
  it emits `81 ec` word-imm frames + unconditional `push si/di`,
  whereas the dev binary shows `83 ec` byte-imm + si/di saved only
  when used — most likely retail MSC 6.00 `/Od`-family codegen.
  Closest local reproduction of dev's prologue shape:
  `/FPa /G0 /AM /Os /Gs` (gets `83 ec` + selective si/di, though the
  optimizer still elides the dead local stores dev keeps).

### Multi-candidate verification (verify_gold.py)

The top-1 fuzzy pairing sometimes picks the wrong gold function
(e.g. `_EMSfree` paired to mid-function junk). `tools/verify_gold.py`
scores every gold function against a named local routine (mnemonic
Jaccard + size window), builds a single-routine map per candidate, and
runs `mzdiff --loose --nocall` until one verifies. `--order=nseq`
re-ranks candidates by difflib mnemonic-sequence ratio instead of
Jaccard (helps when codegen drift skews the histogram). Corrected
verdicts for the original 43 DIFFs: `_EMSReleasePages`->0328:00b8,
`_LDivI`, `_ReadJoy2`, `_SetWorkPage`, `_int86x`, `_labs` all MATCH at
other gold addresses; remaining NOCANDs are genuine drift or boundary
splits.

### LINK-map extents (linkmap2mzmap.py)

Ghidra's per-function R-blocks truncate at `call far` sites — dev-build
extents derived from them produced ~129 nine-byte stubs (e.g.
`_DrawMissile` 05cd-05d7). `tools/linkmap2mzmap.py` rebuilds the dev
map from `build/obj/F14.MAP` publics instead: per-segment sorted
publics, extent = public -> next public, same-offset aliases deduped
(optional preferred-names file). `gold/my_linkmap.map` gives
`_DrawMissile` as 05cd-0698 and raises usable candidates from 57 to
511 of 560 NOCANDs.

### NOCAND classification (classify_gold.py / classified.tsv)

`tools/classify_gold.py ... --seq` scores each remaining NOCAND:
- `nseq` — difflib ratio on mnemonic streams (relocation/literals
  invisible by construction; captures structural/codegen identity)
- `best`  — best nseq over all 8 Jaccard candidates (mispair check)
- `cseq`  — difflib ratio on callee-identity patterns (call targets
  mapped to first-seen indices; survives codegen, breaks on source
  edits)

Result on verify_sweep.txt (85 MATCH / 560 NOCAND), full table in
`gold/classified.tsv`:

- NOCAND-CLAIMED 12 / WEAK 2 / EMPTY 1 — extent artifacts.
- 545 UNVERIFIED, split:
  - **432 nseq>=0.80** — same source, mechanical codegen drift
    (dev is /G0-ish + no-optimize codegen vs gold /G2 186; operand
    commutation, `enter` vs bp-frame, offset/jump-target shifts only).
  - **113 nseq<0.80** — refined further by callpair/deeppair below.
- Re-running the 32 worst top-1-vs-best candidates under
  `--order=nseq` recovered 0 further MATCHes — mzdiff's stream-level
  strictness already rejects what nseq merely re-ranks.

### Call-graph propagation (callpair_gold.py)

`tools/callpair_gold.py` pairs callees positionally through verified
MATCH pairs' ordered call sites — relocation-independent and immune to
histogram drift. `--extra=SEEDS.tsv` folds earlier anchor rounds back in
as callers (fixpoint); `--equal` gates callers to equal call counts.
Two rounds produce 43 pairs (`callpairs.tsv`), incl. `__output` ->
`4140:14be` via `_fread`, `_main`->`18d5:000e`, `_fread`->`4140:039a`,
`_fwrite`->`4140:047e`, `__nullcheck`->`4140:101c`. Conflicts are
resolved by nseq at each alternative (`anchors_nseq.tsv`): majority
picks confirmed for `__cinit`/`_strcpy`; `__setargv`/`__setenvp`/
`__searchseg` anchors hit degenerate Ghidra extents and are discarded
as real startup-CRT drift.

### Open-ended sweep (deeppair_gold.py)

`tools/deeppair_gold.py` scores **every** size-plausible gold function
(not just the Jaccard top-8) by difflib mnemonic-sequence ratio, using
LINK-map-style rebuilt gold extents (start -> next function start, from
sorted `ranges_gold_full.txt` keys — Ghidra R-blocks truncate large
functions at `call far`, e.g. `__output` was a 44-insn stub).

Result on the 117 low-nseq suspects (classified.tsv `deepnseq`/`deepaddr`):

- **28 recovered to nseq>=0.80** — pairing artifacts, not drift:
  `__output` 1.00 @4140:14be (mzdiff-**MATCH**, verified — 86th
  verification), `_pitchbar2`/`_diamondbox`/`_MBox`/`_TacBox`/
  `_AISanitizeGroupCheck`/`_FlyCruise` 0.97, `_MsgBox` 0.96,
  `_horiz_line`/`_TIDDisplayHorz` shared seg, `_DrawAltGauge` 0.98, ...
- **26 stay <0.50** — the hardened drift list: `_HsdCrt` 0.04,
  `_DrawRioMapInfo`/`_RioMap` 0.05/0.08, `__setargv` 0.05, `_DoLSO` 0.07,
  `_Ladder` 0.08, `_LocationPicture` 0.11, `_Splatted` 0.12,
  `_LaunchCruise`/`_GorillaBomb` 0.13, `_SetupView` 0.17, `__setenvp` 0.45
  — concentrated in RIOMAP/HSD displays, AI/PLANES, LSO/carrier, and the
  `_TEXT` startup CRT (`__setargv`/`__setenvp`/`__myalloc` genuinely
  differ from dev's link).
- mzdiff re-verified all 28 recoveries: only `__output` (nseq=1.00)
  MATCHes — the rest remain codegen-DIFF as expected (mnemonic stream
  conserved, operand selection differs).
- Confirmed mechanism: dev `PASM.ASM`/`MONOPRIN.ASM` are `.386` code
  (`_LongDiv` = `cdq;idiv dword;shld` — retail can't run 386). The
  "different CRT rev" claim from the first sweep is retracted for
  `__output` (proved identical at the call-graph anchor) but holds for
  the `__astart`/`__setargv`/`__setenvp` startup cluster.
