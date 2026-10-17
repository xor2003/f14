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

## Current counts (verify_build, extents from gold.map)

- 483/748 mzdiff MATCH; `tools/realdiff.py` (normalized-disasm compare
  stripping data-relocs + far-call targets) isolates **259 routines with
  real instruction-level drift**. Heaviest: PLANES3 (31), PLANES (28),
  PLANES2 (24), AWG9 (19), COCKPIT (14), CARR (13), $3DPLANES (11).
- `/Ot` vs `/Os` tested on PLANES3 — no change. The diffs are register
  allocation + branch-polarity + operand-order spread across bodies:
  genuine SOURCE drift (snapshot predates retail), not flag selection.
- `tools/repair.py` re-pairs each DIFF against every gold anchor in
  matches.txt and reports best normalized distance — use it FIRST to
  discard mispairings (e.g. `_NDraw3DObject`,`_SetDLC`,`_InAcqList`,
  `_MissilesInAir`,`_Messages` were all wrong anchors that MATCH/differ
  under their true pairs).
- Session wins this round: `_DopplerCheck` (volatile long local + /Oa,
  ddd /Oaegcls via MODULE_FLAGS), `_NewTacView` (no return), `_doradar`
  (saturating timers), `_RsignalOne` (2nd GetRadarPtr call, reg far ptr,
  targetnum>=0 alt, (Diff+3)*10 / (8-Diff)*500), `_docannon`,
  `_firecannon` (Rtime UWORD), `_CheckBoatGunFiring` (Rtime&7 + UWORD),
  `_AIRadarRearCheck` (unsigned param, ?150:0, 0x2000 — si/ax residual).
- Rtime is UWORD (STEALTH.C def); all `extern int Rtime` in CMDS, COCKPIT,
  FLIGHT2, HSD, HUD2, MAP, PLANES2, PLANES3, PLANES, SAMS, TGTFLIR were
  drift -> now UWORD everywhere.
- Remaining artifact-class residuals (semantics verified, encoding
  differs): InitEmsFiles `neg si` vs ax-temp (compiler build quirk),
  detect es:si/es:di + one operand-order cmp, LaunchAircraft `xchg`
  fetch-and-clear idiom vs `=0` store, AIRadarRearCheck si vs ax select.
- Some sweep DIFFs are mispairings (e.g. `_Messages`->FUN_05ce_0db1,
  `_InitGraphicPages`->FUN_40b5_0470); verify pairing before editing.

## Runtime status (kvikdos + libdosbox det)

- GOLD under kvikdos reaches mode 13h + renders (kvikdos exits on its own
  after graphics). OURS exits after config.snd/JOY.F14/config.f14/
  KOR04.WLD — never opens Fonts.F14/gm-load.PIC/sp*.pic.
- Root gap: ours issues **zero int21-4B overlay execs**. Retail execs
  Mgraphic.exe + Nsound.14g from a real loader at gold `CODE_8`
  (para 0x504+, caller ip ~079d); the `int21 4B03` site itself is
  FUN_2050_0012 (gold CODE_46). Our binary only contains
  `dig_load_overlay` (OVL.C, sound variant). The MPSLIBM `LoadOverlay`
  trampoline does not reach a real implementation -> graphics driver never
  loads -> LoadPicture/LoadPicNoPal no-op -> LoadFiles' pic opens skipped.
- det-run harness: `~/inertia_player/libdosbox/run_det.sh -a AUTOEXEC
  OUTDIR MAXFRAMES` -> frames.log (byte-identical hashes). Our det run
  hangs in text mode (fixed RTC/int1a pinning may freeze wait loops —
  investigate when comparing).
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

## Session: retail-asm porting batch (continued)

Sweep: 501 → **507 MATCH / 720** pairings (matches3.txt anchors).

### Byte-exact this session
- `_CheckAISix` — retail guards the six-check with
  `bv[GetSquadLeader(cnt)].Behavior != FollowPlayer && !FriendMatch(...)`.
- `_ChangeHUDFilter` — real anchor found at 21c4:000e (retail returns early on
  HUDFilterMode 4/5, writes 0x63/0x35/0 to Palette + 3 stored palettes + Buf).
- `_BearingToText` — retail drops isawac branches, always `Speech(ZERO)` once
  per branch (residual: tail-merge granularity, compiler artifact).

### Verified artifacts (unreachable via source)
- `_All2AirHoming` — mem/mem compare operand order + ax CSE reuse.
- `_AwacsPicture` — `(Diff+1)*60` eval order; all spellings emit same code.
- `_CAPAI` — tdist/ovaldist mem/mem compare order.
- `_CheckAZELChange` — CSE reuse of compare value in ax.
- `_CheckLandingAndCrash`/`_Messages`/`_DamageAI` mispairings noted.

### Structural (deferred)
- `_AWG9` — retail drops set_acq(), adds `if (PW==18) TwsScheduler()` calling a
  ~0x490-byte TWS cluster (08de:306c+) absent from our module.
- `_DamageAI` — retail has 2nd arg (dtype), Diff<2/dtype>=0x20 → AirKill,
  switch/jump-table on dtype, `(tick+planenum)&3` damagenum.
- `_BaseBracket` — two extra calls to a scale-wrapper (2fb2:4dac) around a
  FLIGHT2 routine; needs the helper reconstructed.
- `ReadEMSPCX` index-API, `X2map`/`Y2map` inlining, overlay thunk table.

### Altitude/speed conversion cluster (499b216)
- Gold has three helpers at 2fb2:4d73/4dac/4dcd: `InvAltSpeedConv`
  (IAS->TAS: MulDiv(knots,100,TAStoIAS(100)|1) with OurAlt pinned),
  `AltSpeedConv` (TAS->IAS), `GetDAltSpeed` (retail keeps only 2 callers).
- `_InvAltSpeedConv`, `_AltSpeedConv`, `_IsVTOL` — byte-exact (edits=0).
- `_CheckTurnStuff` — full retail rewrite: `maxspeed -= GetDAltSpeed`
  after the 450 clamp, both MinSpdG arms use InvAltSpeedConv gates +
  AltSpeedConv stores, IsVTOL(ptype->PlaneEnumId) gates a 150-knot floor,
  tail clamps basespeed=maxspeed not ptype->speed. Residual: si/di
  regalloc ordering only (38 insn but all reg/slot swaps).
- PLANES2 FormationSpeedAdjustUS: `-= / += GetDAltSpeed` pair replaced by
  nested `AltSpeedConv(ps[i].alt, InvAltSpeedConv(ps[prewing].alt,
  prespeed))` gated on `type!= || alt!=`.
- PLANES.C: `spd = InvAltSpeedConv(alt, spd)` replaces dead `#if 0`
  GetDAltSpeed block (under !(status&CRASH)); `abspeed -= GetDAltSpeed`
  added after 450 clamp; `basespeed = 200` flat else-arm.
- Gold call topology (far+near): InvAlt 7, AltConv 16, GetDAlt 2.
- Remaining Conv-caller functions still DIFF: `_TakingOff`@21ce:0b7e
  (extra zero-block: ptype->f26 check, basespeed/pdest/droll stores),
  `_AddSomePlanes2`@21ce:82fd (double Conv), `_BaseBracket`@2ad2:1d6a
  (Conv vs InvAlt branches under >10 gates), `_SetUpOval1`@2ad2:2a11,
  `_StraightAttack`@21ce:3e77, FUN_2ad2_37a6 (unpaired, 2 callers).

## Session: waypoint-opcode + cover-port batch

Sweep: 507 → **520 MATCH / 746** pairings (new anchors added).

### Byte-exact (edits=0)
- `_DoOClockCall` @3fb2:64e2 — added `bv[GetSquadLeader(cnt)].Behavior !=
  FollowPlayer` gate inside the status test.
- `_RearCover` @3fb2:2089 — status-gated repick
  `target==-1 || CRASH || !ALIVE || !ACTIVE`; GetWhoCover return discarded
  (retail reads bv[].target after call).
- `_GorillaCover` @3fb2:1de6 — same repick gate plus retail's second-if:
  `target==-1` arm sets `behaveflag|=0x200`, `Behavior=Formation`,
  `horizontalsep=0x35`, `behaveflag&=~0x4000`, UpdateInterceptGroup, scans
  FollowPlayer planes for max verticlesep (+0x7d), sets
  `Behavior=FollowPlayer`, verticlesep=max, returns.

### Ported, residual = regalloc/data-placement artifacts
- `_GetWayPointChanges` @3fb2:26c4 — raw opcode switch order, near
  `struct _action_ *actptr`, byte-flag writes via char *wflag, new opcodes
  SA_DOACTION(0x23)/SA_GOALTICK(0x25)/SA_ATTACKGROUP(0x26) in SETUP.H,
  10th arg `wplevel`; call is `DoGroupAction(wplevel, 8, actionval)`.
- `_DoGroupAction` @3fb2:63b2 — added DOWPDAMAGE mcnt[0..2]=0,
  DOWPHOVER `Behavior=UpdateOurWPS, varcnt=waypt+Gtime+2`,
  actionflag==8 LoadWayPointPic, and UpdateMissionData(0,
  ptype->PlaneEnumId).
- `_LoadWayPointPic` @3fb2:6953 (new) — resource-archive pic loader:
  fseek/fread index record at WpFile, name[cnt]=0, wpPicCnt++ '%'-patch
  with right-aligned spaces, ReadEMSPCX(name,0).
- `_UpdateMissionData` @3fb2:6a38 (new) — MDStat0..2 gates, MDTable
  {count,slot[10]}x5 scan decrementing on enumid match, completion pics
  + ProcessOtherComm tail call (0x668d target still unidentified).
- New globals in PLANES3.C: `FILE *WpFile`, `wpPicCnt[128]`,
  `MDStat0/1/2`, `MDTable[220]`.

### Still open
- `_ProcessOtherComm` @2fb2:6bb5 — gold 362 insns vs ours 72: major port.
- `_Ready5` @2fb2:6b2b — actually a {count,slot[10]} group-lookup
  (FUN_6b2b != Ready5); real callers at file 0x23191/0x233c5.
- `_AIBombTarget` @21ce:6a3c — tail uses computed bombalt bands
  (farcall>>2 vs zval) — needs decode+rewrite of pdest tail.
- `_TakingOff` — 3 residual insns (dead store + lea reg choices).
- `_AirKill` @2fb2:0100 — 124 edits, behavior-ptr compares missing.
- `_ChangeGroupBehavior` @21ce:8181 — 37 edits.

Sweep: 531 → **530 MATCH / 748** (post AirKill; f14_named.map regenerated —
some extents shifted; -1 net but several stale-extent fns corrected).

### Byte-exact added
- `_AirKill` (38b6:0ebc, 215 insns) — full retail rewrite: `ourcarrierdead` flag
  on `killed==whichourcarr`; catapult release `PlaneLaunched(threathead,killed)`
  + `GetNextLaunch(killed)` when TAKEOFF+base<0+threathead∈[0,4); chute gated by
  bclass∉{CMISSILE,CIVILIAN} + killed<NPLANES + (winglevel>=Rtime&0xf || ==0xf);
  respawn `verticlesep++/varcnt` update for killed>=NPLANES; UpdateMissionData
  pair (leader==-1 → ourplanetype else PlaneEnumId); Rtime&0xf 3-arm msg select.
- New globals: `ourcarrierdead`, `ourplanetype`; bv field fix `groundt` in
  GetPlanePointXYZ (was target — offset map correction).

Sweep: 520 → **531 MATCH / 748** pairings (matches3.txt anchors, post
A2G/GetPlanePointXYZ/AddGroundTarget/AvoidMountains/ChangeGroupBehavior
batch). 217 mzdiff DIFFs remain — triaged: many are aldiff edits<=10
artifact class (operand-order, regalloc, dead-store elim, epilogue
sharing, ±0x8000 segment-wrap equivalence).

### Exact this batch
- `_Air2GroundHoming` @38b6:0229 (+statics decoyed/home/tracking,
  combined range byte-exact): tgtx==-1 → GetPlaneOrgNum→MAXPLANE(0x46)→
  GetPlanePointXYZ(&x,&y,&z) store to ssi->tgtx/tgty, else CRASH branch.
- `_AddGroundTarget` @1d56:1094: missing ST_PATRIOT enum (0x19) +
  Patriot arm (radarnum=48); NRADAR block replicates retail
  TXN(MAXGROUNDTARGET) copy-paste bug; block1 TXT vs block2 TXA enables
  MSC6 tail-merge. gold.map extent +1 (retf).
- `_GetWayXYZ` @2fb2:2ad1 exact.
- `_GetPlanePointXYZ` @2fb2:2b6e: full retail rewrite — GetPlaneOrgNum
  resolve loop, PxLng gate, aipaths[OurPath].wp0 + bv[].target waypoint
  chain accumulate via shared goto tail; 44 layout/regalloc artifacts.
- `_AvoidMountains` @21ce:1052: TAKEOFF|LANDING path = Gtime>5 gate +
  LDist2D(xL,yL)>>2+6 alt clamp (was early return). 8 operand-order
  artifacts.
- `_TIDAttkInfo` re-anchored 4d7e:0362 (was wrongly _Air2GroundHoming);
  still 131 edits — retail does fixed-position itoa+RpPrint blocks.
