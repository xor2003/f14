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
