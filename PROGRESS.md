# F-14 Fleet Defender — retail-matching source rebuild progress

Reference oracle: `/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE` (retail).
`src/F14.EXE` is used ONLY for CodeView names/module order, never behavior.

## Toolchain (build_all.sh)

- C: MSC 6.00 `c6/BIN/CL.EXE` under kvikdos, flags `/FPa /G2 /Gs /AM /Osegcl /Zi /Zd`
  (`/Gs` stack-check-off is required — without it every C routine emits a
  spurious `chkstk` prologue vs retail).
- ASM: real **MASM 5.10** (`kvikdos/borrow/msdos4/TOOLS/MASM.EXE`) via
  `build/masmstag/` staging. Retail MASM encodings differ from UASM:
  accumulator-form `05 xx yy` imm16, `9A` far-call relocs + `nop` pads.
  UASM stays only for `_rp.asm` (MASM 6-era syntax).
- Link: LINK 5.31.009 `/MAP /SEG:256 /ST:3500 /CODEVIEW`.

## Verified matches (mzdiff, extents from analysis/gold/gold.map)

- **GMAIN (CODE_30 @18d5): 12/13 routines byte-exact** — `_main`,
  `_WaitScreen`, `_InitAbort`, `_InitWeather`, `_DisplayErrorMessages`,
  `_LoadFiles`, `_InitDamnPalettes`, `_LoadConfig`, `_LoadSoundConfig`,
  `_MainGameLoop`, `_InitGraphicPages`, `_NoOp1`.
  - `_InitEmsFiles`: identical except gold `neg si` vs ours
    `mov ax,si; neg ax; mov si,ax` — unary-minus ax-temp is MSC6.00 codegen;
    suspected retail compiler build (6.00a?) difference. Semantically equal.
- **EMSLOAD**: `_NewEmsFunction`, `_LoadPlanesEms`, `_LoadTheWorld`,
  `_LoadGroundObjects`, `_EMSFileUsage` byte-exact; `_GetEmsMem`,
  `_ResizeEmsMem` instruction-identical modulo embedded DGROUP addrs.
- **EMM.ASM**: `EMSReallocPages` (int67 fn 51h, returns -1/0) byte-exact vs
  gold `CODE_5:0091`.

## Retail facts recovered this round

- `NUMBANKS` retail = **13** (was 19): gold `GetEmsMem` uses `i+0x34`,
  `LoadTheWorld` loop bound `cmp si,0xd`.
- Retail `NewEmsFunction` grabs ALL free EMS pages (no `100+NUMBANKS*4`
  clamp): `grabbed=free; alloc(grabbed); grabbed -= 4*NUMBANKS`.
- `InitWeather`: `case 0` and `default` share the 9150/500 cloud block;
  weather 1|2 → WeatherType=-1, 8150/600.
- Retail GMAIN routine order (14) implemented; extra `LoadMsgEMS`
  (`MakeEMSFileBlock("MSG.EMS",320)` when `MsgEMS==0`), `InitThreatTbl`
  (AWG9.C tail), `EMSReallocPages` (EMM.ASM), `LoadPlanesEms` (EMSLOAD.C).

## Remaining

- 267 routines with real instruction-level drift (see `tools/realdiff.py`
  output; false-DIFF filtering strips data-relocs). AWG9, SAMS, PLANES*,
  RIOMAP, TID are the heavy modules.
- mzdiff needs `DGROUP DATA <para>` in maps to resolve DS refs: gold DGROUP
  image para `50b1`, ours `5340` (auto-emitted by linkmap2mzmap.py).
- Runtime verification under DOSBox/kvikdos pending for this build.

## Verify commands

```
./build_all.sh all && ./build_all.sh link
python3 tools/linkmap2mzmap.py build/obj/F14.MAP 6173e > build/f14_named.map
python3 tools/verify_build.py GOLD.EXE analysis/gold/gold.map \
    build/obj/F14.EXE build/f14_named.map \
    analysis/gold/classified.tsv analysis/gold/matches.txt
python3 tools/realdiff.py build/verify_<n>.log   # real-vs-reloc diffs
```
