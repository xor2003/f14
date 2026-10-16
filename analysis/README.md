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
`mzdiff_mybuild_loose.txt`: **85 MATCH / 43 DIFF** verified by
`mzdiff --loose` — these .C source functions compile to the same code as
the retail binary modulo relocation/literal fields. `gold_to_source.tsv`
is the consolidated gold_addr -> name -> source_module -> score table.

DIFF includes Ghidra boundary artifacts (function split differently at
entry), so DIFF means "unverified", not proven different.
