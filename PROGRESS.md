# F-14 Fleet Defender — retail-matching source rebuild progress

## Turn: F14RUN runtime — overlay loader works end-to-end under libdosbox

Two independent defects fixed; F14RUN now runs past both early overlay
loads (verified live under libdosbox gdbserver, port 14777):

1. `MPSREAL.ASM` encodings (real reconstruction bug, affects both build
   paths): `mov di,offset g601f` (3B) → `lea di,[g601f]` (4B) at retail
   `g6435`, and `mov ax,offset g6d66` → `lea ax,[g6d66]` at `g6bcc`.
   Retail `F14.EXE`/`F14S.EXE` use 4-byte `lea` there; the shorter `mov`
   shrank `InitOverlay` by one byte, so the raw `db 0E8h,008h,001h`
   int21-wrapper call at `g6361` landed on `push si` instead of the
   balanced `push bp` entry — `pop bp` ate the near return address,
   `ret` jumped to `3fb8:0000` (mid-`farfread`), and control wandered
   into DGROUP strings (`0x59fb6`). Fix verified: call now lands on
   `push bp`, wrapper returns cleanly to the continuation, `ax=0`.
   NOTE: `F14R.EXE` in the game dir is a copy of `F14RUN.EXE`, not
   retail — the true runtime reference is packed `F14.EXE`/`F14g.EXE`.
   Lesson for lifted asm: prefer `lea reg,[sym]` over `mov reg,offset`
   — preserves the retail operand-size encoding so raw `db` call sites
   stay aligned.

2. `GDSEGR.ASM` (runtime compact DGROUP, run-build only): `_SoundFile`
   and `_SpeechDriver` are *initialized* filename templates in gold
   (`"Psound.14g"`/`"Digital.f14"` at `0xa556`/`0xa562`) but the
   ORG-free replica declared them as bare `LABEL BYTE` inside a
   ~520-symbol BSS cluster that all collapses to one address
   (`ds:642e`). Any write through a cluster var (startup memset/strcpy)
   zeroed the tail, so `LoadOverlay` saw `"N"` → "Filename not found"
   → exit 0x99. Fix: moved both labels to just before the cluster
   start (after the `DB 574 DUP(0)` pad) and emitted the real strings
   (`0xc`/`0x28` gold sizes); cluster now lands at `0x6462`, forward
   writes can't reach the strings. Verified: `LoadOverlay` receives
   `"Nsound.14g"` (SOUND.C patches `[0]` per music_type), both mode-03
   EXECs (`Mgraphic.exe`, `Nsound.14g`) complete, game continues into a
   BIOS idle wait (menu keyboard loop) — no wild execution.

Caveat: the ~520-symbol collapsed BSS cluster is still a latent hazard
for deeper runtime (every var aliases `0x6462`); if later init code
needs distinct vars, emit gold ORG spacing for the used subset.
F14RUN layout intentionally differs from retail — not for byte-match.

## Turn: _LoadScottMission near-match + DECOMPILATION.md methodology written

_LoadScottMission: three-layer fix —
- fopen#1 keeps global `MisFile` (gold `mov [0xcba4],ax`), fopen#2 uses
  local `FILE *File` at `[bp-0xa]` (gold pushes `[bp-0xa]` at fread sites —
  the earlier `File`→`MisFile` replacement was wrong, reverted).
- GDSEG: labeled the second embedded `"rb"/"mission.f14"/"No Mission
  Data"` copy inside the `_KeyJoyY` blob as `gstr2b`/`gstr302b`/`gstr303b`;
  second fopen/Message now push them — gold's two error paths use
  distinct string objects (offsets `0x32a5` vs `0x32c4`), which also
  reproduces gold's tail-merged `push;push;jmp short back` shape.
- Result: `0x32b5`/`0x32b8`/`0x32c4` pushes + `jnz` polarity now
  byte-match gold; residuals = `enter 0x240 vs 0x242` (one extra word
  local), local-slot permutation, zero-store ordering — pinned class.

Methodology now documented in `DECOMPILATION.md`: toolchain, re-anchoring,
diff taxonomy (stale-anchor/dead/lib/layout/regalloc/operand-order/
store-group/asm-encoding vs real), source-shape recipes, MSC6 codegen
facts, gotchas. `tools/pairdiff.py` preserved (was /tmp).

## Turn: _DDDSwitches MATCH — file-scope statics before externs = gold decl order

_DDDSwitches fully reconstructed: gold's version is the PRE-CHECKBOX
iteration — no RadarOn/DrawLine borders, two independent `if(AWG9_SNIFF)` /
`if(!(AWG9_SNIFF))` ChangeColor pairs at (227,122,14,5), mode case order
TWSA,TWSM,SRCH,RWS,PDSTT, w=17 rects, RWS y=157.

Key mechanism fix for the != operand-order artifacts: `RadarMode`/`PrevRadar`
moved from FUNCTION-scope statics to FILE-scope statics declared BEFORE the
`extern int RadarRange;` line — binding order then makes the statics
earlier-bound → ax slot, externs later-bound → mem-left `cmp` operand.
Emission became byte-identical to gold INCLUDING gold's extra
`mov ax,[RadarRange]` reload (function now 0x3f4 = gold length; only the 2
jump-table data regions differ). The decl-order rule is TU-global, not
per-function: gold's COCKPIT.C had `static int RadarMode,PrevRadar` near
the top before the RadarRange extern.

COCKPIT sweep after the change: most fns hunk:0, no operand-order
regressions. Remaining real mismatches in module: _DrawRadarDials(32),
_Flip(21), _DisplayLeftStuff(18), _DDDSwitchRefresh(16),
_DisplayTIDSwitches(14), _MasterArmSwitch(13), _DoCRT(10).

## Turn: GetWeaponCount cmp-order root cause — the `/O` letter `e`

Flag sweep on the in-context `*p==type` compare: `/Osc /Oc /Os /Oscl /Ogsc
/Osgcl` emit gold order (`mov ax,[bp+6];cmp [bx],ax`); every flag set
containing `e` emits the flipped order. So the `e` pass (expression
opt) does the commutative-operand canonicalization AND is required for
the dx/bx register allocation + `enter 2` frame (`/Osgcl` alone →
`enter 6`, all-stack locals). Gold has regalloc AND param-first order
→ retail's `e` pass ordered leaves differently than our C 6.00-alpha.
~80 source constructs (volatile, hoisted temps, casts, folded exprs,
param types, loop forms, decl ordering) all flip in-context. **Pinned
as compiler-release artifact** — same class as _ChangeHUDFilter and
the other mem/mem cmp-order diffs documented below.

Reference oracle: `/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE` (retail).
`src/F14.EXE` is used ONLY for CodeView names/module order, never behavior.

## Turn: 605 -> 607 — cmp operand-order mechanism cracked

NEW MSC6 rule (verified): for `cmp` between two memory operands (local/param
vs global), the LATER-BOUND operand emits as the mem-left operand and the
earlier-bound goes to ax. To make a GLOBAL the mem-left operand (retail's
shape, e.g. `cmp NPLANES,ax`), bind it AFTER the locals:

1. If the global is defined in the same .C: move the `int G=0;` def to FILE
   END and declare `extern int G;` inside the function AFTER the locals.
   (EMSPagesGrabbed in EMSLOAD.C — GetEmsMem+ResizeEmsMem now MATCH.)
2. If the global comes via header extern: `extern int G;` inside the fn
   after locals SOMETIMES flips it (CheckDataLink, ReArmWingman MATCH) —
   worked for NPLANES in some fns, not others (mechanism partly unclear).
3. Loop-bound spelling `for(i=0;GLOBAL>i;i++)` emits literal
   `cmp GLOBAL,i;jg` when the global is late-bound; `i<GLOBAL` emits
   `cmp i,GLOBAL` when early-bound. `>` is NOT always normalized.

_detect: signal/dist decl swap (signal before dist) + `dist<signal`
spelling flipped the compare to gold's `cmp dist,ax;jb`.

Unsigned compares emit literal jump sense (ja/jb) — source `>` gives ja,
`<` gives jb; which operand is mem-left follows the binding rule above.

Remaining classes unchanged: si/di/bx regalloc (use-count driven),
far-ptr es save/reload, retail-only blocks (WING_SCHED in CheckDataLink's
extent is now passed — function body itself matches).

## Turn: 645 -> UpdateInterceptGroup MATCH; operand-order is MODULE-STATE chaotic

KEY finding on the "sometimes works" operand-order mechanism: the
late-bound-vs-early-bound mem-left choice is NOT purely per-function.
Adding `int isheli` to UpdateInterceptGroup (line ~389, early in
PLANES3.C) perturbed module codegen state and FLIPPED the `cmp` operand
order in 4 later siblings — _DoGroupAction, _GetPlaneLanding,
_GetWhichElevator, _ReArmWingman went MATCH->DIFF though their source is
unchanged and correctly paired (same gold addr). Verified by revert:
removing isheli restores _GetWhichElevator MATCH. So the 4 are NOT source
bugs — gold emits `cmp NPLANES,cnt` (bound-left) and the in-fn
`extern int NPLANES` late-bind trick is unreliable for header-COMDEF
globals (NPLANES is `int NPLANES;` COMDEF in WORLD.H — can't move to
file-end, option 1 doesn't apply). Inert levers tried: respelling
`NPLANES>cnt` <-> `cnt<NPLANES`, repositioning in-fn extern to truly-last,
merging decls, renaming isheli->cnt, adding unused pad local. The 4 will
likely flip back as future edits perturb the module.

Also landed: DrawRioMapInfo case4/5 loop bound LASTPLANE->NPLANES (real
wrong-global fix, 4->1 diff); SetUpLaunch helicopter check
behaveflag&ISHELICOPTER -> GetPlanePtr(type)->bclass==HELICOPTER_OBJCLASS
(real reconstruction fix, still a deeper local-order diff).

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

### Damage system batch (post-AirKill)
- `_DamageAI` @2ad2:22b3 — 2-arg retail form: `DamageAI(planenum, damage)`
  with dense 32-case switch (jump table) mapping damage codes to severity
  0..4; DAMAGELITE[0]∈{0,1} or damage>=0x20 -> AirKill; behaveflag damage
  bits, damage smoke, GetAIBase/PromoteToLeader/WingDamageMsg calls.
  All callers (AAAHIT, BULLETS, PLANES3, SAMS) pass retail codes.
- `_ShowAIStatus` @21ce:5d73 — 3-arg form `(ainum, otherai, effects)`
  emitting "I'm Hit, ..." strings per effect bit; _ShowAIWeaponStatus
  unpaired (no gold counterpart — its sites call other code in retail).
- `_PromoteToLeader` @2fb2:4a51 — promotes BOTH wingman(+6) and
  nextsec(+0xa): Formation||GorillaBomb gate, Behavior/groundt copy +
  DoNextWayPt (skipped when TAKEOFF), DOWPTROOPDROP leader unlink, and
  surviving-wingman `behaveflag = (x & ~0xf) + 2` position fixup.
- `_VectorIntercept` — byte-exact after merging fragmented gold.map
  extents (0467-0850).
- `_GorillaBomb` @2ad2:44b2 — byte-exact (new gold.map FUN entry; the
  old 2b89:02d4 entry was pitchbar2's anchor).
- `_DESIGNATED_TrackingAI` @21ce:6f8e — retail blocks recovered:
  first-entry xloc/yloc init from AIBOMBX/Y; abs>0x800 one-shot
  LowBombTarget (Behavior!= guard) + >0x2000 forced switch;
  UpdateMissionData on DOWPGOAL; groundt +=2 on aiwp.x==-2L else ++;
  GetWayXYZ -> xloc/yloc before DoNextWayPt/RestoreCover. 7 regalloc diffs.

Sweep anchors: `_DamageAI`->3ad2:22b3, `_ShowAIStatus`->31ce:5d73,
`_TakingOff`->31ce:0b7e, `_DESIGNATED_TrackingAI`->31ce:6f8e,
`_GorillaBomb`->3ad2:44b2, `_ShowAIWeaponStatus` unpaired.

Next big ports (largest residual insn counts): `_TakingOff` @21ce:0b7e
(gold 387 vs ours 210 — Gtime>5 gate, Behavior indirect-call path,
bclass==0x28 split, GetWayPointChanges+LogIt tail), `_FlyWayPoint`
@2ad2:37a6 (520 vs 464), `_SetUpWingman` @2fb2:58d1 (gold init tables),
`_pitchbar2` @1b89:02d4, `_TIDAttkInfo` @3d7e:0362.

### TakingOff decode notes (gold 21ce:0b7e-1051, NOT yet ported)
Retail algorithm (replaces ours entirely):
- `if(Gtime < 5) return`
- Catapult-release: `if(ps[i].base<0 && bv[i].threathead<0 && bv[i].groundt!=-1
  && aiwaypoints[groundt].x <= -1L)` -> `bv[i].basespeed=0; ps[i].alt=0;
  if(bv[i].threathead > -5) PlaneLaunched? ...` then at 0xe62 path:
  `if(0<=threathead<4){ PlaneLaunched(threathead,i); GetNextLaunch(i); }`
- `if(bv[i].Behavior == 2fb2:2d78) { basespeed=0; ps[i].alt=0; return; }`
- bclass==0x28 (ptype->+0xa? actually cmp [es:bx+0x26],0x28 -> ptype field 0x26
  = bclass): `if(bv[i].varcnt > Gtime)` -> zero [0x61a4]+0x1a(speed), +0x14(roll),
  [0xd656], [0x66a6](droll?), bv basespeed; return.
  else `if(bv[planecnt].basespeed < 0x64)` -> `bv[i].varcnt=0;
  if(basespeed<=0x1e){ save bv+0x14 in si/di; call far bv[i].Behavior();
  restore } bv[planecnt].basespeed+=3; ps[planecnt].alt+=3; ...`
  else `bv[planecnt].basespeed = ptype->speed(-0x14?? +0xa) - (x>>2)`;
  GetWayPointChanges(groundt,&x9, -1); if(bp-8 != -1)
  bv[i].basespeed = call 0x2fb2:0x4dac(ps[i].x?, bp-8);
  ps[i].status &= ~0x80; LogIt(0x20, i, xL,yL,alt, -999,-999, 0,0,0);
  tail: [0xd656] = ps[?].speed<0xc8 ? 0x2000 : 0.
- else (bclass!=0x28): `call 0x2fb2:0x2d90(*ps[0x61a4]?, &4 outs)` -> ret in
  [bp-0x20]; if nonzero: `si = GetSquadLeader(i)`; if si>=0 squad walk:
  di=0; for(bx=si; bx>=0; bx=bv[bx].wingman) { if(bx==i){if(!di)di=1;}
  else if(ps[bx].status & (TAKEOFF|ACTIVE|ALIVE) && !di) di=-1; }
  [bp-6]=di. if(di == -1) { ps[our].? =0; [0x66a6]=0; [0xd656]=0; return }
  else main launch physics.
- Launch physics (0xe17+): `if(bv[i].varcnt > Gtime) jmp 0xc3e(shared)`;
  `if(bv[i].varcnt != 0){ bv[i].basespeed=0xa; varcnt=0; ps[our].speed=0xa;
  if(ps[i].base<0 && 0<=threathead<4){ PlaneLaunched(threathead,i);
  GetNextLaunch(i); } }`
  `[0x66a6]=0; si=bv[i].threathead; if(bv[i].basespeed <= 0x1e){
  di=bv[i].basespeed; call far bv[i].Behavior(); bv[i].basespeed=di;
  ps[our].?+0x14=0; [0x66a6]=0; } bv[i].threathead=si;
  if(ps[our].speed < 0xb2){ ps[our].pitch?=0; roll?=0; [0x66a6]=0 }
  else if(ps[our].pitch < 0x400){ ps[our].pitch += 0x10000/[0x503a](Fticks?);
  if(ps[our].roll < 0x1000) [0x66a6](droll)=0x200; }
  [0xd656] = ps[our].pitch;
  if(ptype->speed > ps[our].speed && ps[our].speed < 0xdc){
    if(bp-0x20==0) speed += 0x140000/[0x503a] (>=1);
    else speed += (speed<0x96 ? 0x4b : 7); bv[i].basespeed += same }
  else { ax = (call 0x342:0x113b(ps[our].+8..+0xe fields))>>2 + 0x50;
    if(ax < ps[our].alt){ bv[planecnt].basespeed = ptype->speed - (x>>2);
    GetWayPointChanges(groundt,&9outs,-1); if(bp-8!=-1)
    bv[i].basespeed = 0x2fb2:0x4dac(ps[i].x?, bp-8);
    ps[i].status &= ~0x80;
    LogIt(0x20, i, xL, yL, alt, -999, -999, 0, 0, 0); return } }
  tail: if(bv[i].basespeed > ptype->speed) bv[i].basespeed = ptype->speed.
- Globals: [0x61a4] = &ps[planecnt] (our-plane ptr; +0x1a speed, +0x12 pitch,
  +0x14 roll, +0x6 alt, +8/+0xc xL/yL); [0x906c]=planecnt; [0x619a]=i;
  [0x669a]=Gtime; [0x66a6]=droll; [0xd656]=thrust-readout?; [0x503a]=Fticks;
  [0xc56e]=NPLANES; [0x7e4c]=ptype far ptr (+0xa = speed/maneuver);
  callees: 0x106a:0x108=PlaneLaunched(cat,plane); 0x2fb2:0x446d=GetNextLaunch;
  0x2fb2:0x26c4=GetWayPointChanges(9 outs); 0x2fb2:0x2d90=5-out resolver;
  0x2fb2:0x4dac=speed helper; 0x2fb2:0x2d78=a Behavior ptr; 0x3b61:0x504=LogIt;
  0x342:0x113b=4-arg dist/wave-height helper; 0x1fc6:9=Message; 0x2ad2:0x6ec=
  GetSquadLeader.

## Session N — SetUpWingman + anchor corrections (cont.)

- `_SetUpWingman` ported to retail `2ad2:0004` (was mispaired to `2fb2:58d1`
  which is an unported separation helper). Recovered: ISHELICOPTER head flag,
  startval<NPLANES / endval>LASTPLANE clamps, unconditional mcnt[0..2] copy,
  winglevel copy, behaveflag slot `+= numflight + (leaderflag&0xf ?: 1)`,
  `numflight > numadd` loop exit, `&= ~ENEMYNOFIRE` bug fix. 4 artifact edits.
- `_InterceptControl` @`2fb2:0004` confirmed (Gtime<7 + GetSquadLeader head).
- `2ad2:0004` is SetUpWingman; `2fb2:0004` is InterceptControl (same offset,
  different segments - keep distinct).
- Retail `2fb2:6bb5` (FlyWayPoint attack==0xc9 branch): group-attack helper,
  writes `behavecnt=Gtime` at 3 sites, calls GetPlaneOrgNum/GetSquadLeader/
  GetWayPointChanges/DoGroupAction/CheckPlaneBlast/GetWayXYZ/DoNextWayPt.
  NO our-source counterpart - unported function, InterceptControl used as
  placeholder call.
- Retail `2fb2:0fa7` = LaunchCruise (positional + NPLANES log-split head).
- Remaining big ports: _pitchbar2 (gold has 9 Rotate2D/2 print vs our 13/4 -
  different draw structure), _TIDAttkInfo @3d7e:0362, _AddSomePlanes2,
  _DoGroupAction @2fb2:63b2, FUN_2fb2_6bb5 (needs new source fn).
- Small-edit DIFFs surveyed (_AIGetBombDist, _AISanitizeCheck,
  _AIRadarRearCheck, _AIBombTarget): all artifact-class (jump-form polarity,
  global-offset, push-eq-zero, imul-via-global-i vs si).

## Session N+1 — DoAltHover / CruiseDistruct / CheckForMobileR5 / DoGroupAction

- `_CruiseDistruct` BYTE-EXACT: retail gate is GetQHeight(xL,yL)>>2 + 300 > alt
- `_CheckForMobileR5` semantic match: faction mask = Rdrs[base].status &
  (FRIENDLY|NEUTRAL) (isfriend param dead), FriendMatchType positive sense,
  dead Dist2D call kept, first Ready5 leader wins, yval steps +2
- `_DoAltHover` semantic match: i=planecnt head store, aiwp (-2,-1) pair test
  (int casts on long fields), basespeed=InvAltSpeedConv(alt,speedval),
  dual ChangeGroupBehavior(planecnt,0) around DoNextWayPt
- `_DoGroupAction` operand order Gtime+waypt (4 regalloc edits)
- `_SetUpWingman` semantic match @2ad2:0004 (see prev entry)
- `_CheckPlaneBlast` remaining: ~53 edits, needs shared-tail 12-arg call-site
  decode (gold pushes 0,0,0,[bp+0x12]... jmp shared call at 0x3e0e)
- `_pitchbar2` remaining: gold draws bracket with 9 Rotate2D/2 prints vs our
  13/4 - different structure, needs visual decode
- `_CheckTurnStuff` 38 edits: si/di regalloc + probably real diffs deeper

## Session — CheckPlaneBlast byte-exact + anchor hygiene
- `_CheckPlaneBlast` **BYTE-EXACT** (394=394). Retail extras recovered:
  * `missiletype==0x39` → "TORPEDO" arm (SLG codes 49/50, beyond our enum — raw ints)
  * boat side (cnt>NPLANES) has 3-way msg select incl. torpedo
  * `LogIt` is varargs-ish: `..., 0L, logtype)` tails; per-arm `if(doKill)` dup with shared LogIt tails
  * strings in DGROUP: "Damaged By "/"TORPEDO"/"BOMB"/" Hit By "/"Hit by "
- `_AddSomePlanes2`: 7 artifact edits (regalloc: cnt spilled to [bp-0xc] vs ax; jg/jl forms)
- `_BaseBracket`: 4 edits — relangle arms semantically equal, MSC6 commutes `a-b+c` → global-first
- Anchor fixes: matches.txt `_GorillaBomb` -> 3ad2:44b2 (mch had wrong 3ad2:1236)
- `_TIDAttkInfo` pairing `4d7e:0362` is BOGUS (4d7e = CRT segment: _vsprintf/__ftbuf live there). Needs real anchor — TID fns live in CODE_30-ish region (matches3 uses seg+0x1000 encodings vs gold.map paras)
- IMPORTANT: matches.txt `mch` anchors OVERRIDE classified.tsv cls anchors in verify_build.py (processed last). Stale auto-pairings can flip MATCH->DIFF between runs.
- ps[] base in gold = 0x7e64 (field 0x7e74 = head, +0x10); bv[] base 0x69c0 stride 0x30;
  [0x61aa] = `i` (file-static plane idx), [0x906c] = planecnt, [0x247a] = OurHead, [0x61a4]=psi ptr
- BIG REMAINING: _pitchbar2 (218 vs 355 — helper-based draw loop differs structurally),
  _TIDAttkInfo (needs anchor), retail-only fns 2fb2:6bb5 (group-attack helper, writes
  behavecnt=Gtime, callees GetPlaneOrgNum/GetSquadLeader/GetWayPointChanges/DoGroupAction/
  CheckPlaneBlast/GetWayXYZ/DoNextWayPt) and 2fb2:58d1 (formation separation helper)

## HSD.C (pitchbar family) — 2026-04-15
- `_pitchnum` @1b89:01c9 — **BYTE-EXACT** (102=102)
- `_pitchbar` @1b89:0128 — artifact-class (63v66, 10 edits): gold emits `local+arg`
  (`mov ax,[bp-8]; add ax,[bp+0xa]`); ours `arg+local`. MSC6 canonicalizes
  `x+ox`/`ox+x` to identical output — spelling cannot flip leaf order.
  Same for live-ax reuse after scaley. Semantic-identical.
- `_pitchbar2` @1b89:02d4 — artifact-class (355=355!, 7 edits): gold.map extent
  was truncated (02d4-04f9 → real 02d4-063f). Whole body byte-identical except
  one CSE cluster: ours hoists `ox-t` into dx→spill [bp-0x1a] (enter 0x1a),
  gold recomputes in swapped di (enter 0x18) via si↔di swap after print1.
  No spelling variant (ox-t+c, flags /Osegl//Oaegcls) changes the CSE choice.
- gold.map: fixed FUN_1b89_02d4 extent 04f9→063f (was truncated mid-function).

## LoadScottMission retail-format decode (2026-04-??, uncommitted work-in-progress)
- Verify status: **540/748** after anchor fixes (_LoadFiles@18d5:0212,
  _NDraw3DObject@0058:0b45, _SetDLC@0dc3:2288, _show_range@3cb5:057c).
- `_LoadScottMission` gold = `FUN_1d56_0000` (seg 1d56); FUN_1d56_1094 is
  exactly 716B = our `_AddGroundTarget` -> seg identity proven.
  Gold fn ~1298 insns vs ours ~992. Frame `enter 0x240` (ours 0x150).
- Retail format diffs:
  * mission header fread is **0x102 (258B)** at [bp-0x194]; ours 0x9e=158B.
    Retail ints time@+116(+0x74), count fields cluster +114..+134,
    long loc fields +138..(+157): objectLoc~+0x8a, mEscLoc~+0xb6,
    mDieLoc~+0xba, mLiveLoc~+0xbe, pathLoc+0x92, wpLoc+0x96, actLoc+0x9a.
    SETUP.H has expanded _missionHeader_ (desc[64]); MISSION.H still 158B.
  * `_object_` is **0x64 (100B)** in retail vs ours 82B.
  * TWO fopen("mission.f14","rb"): first -> GLOBAL FILE* [0xcba4] (MisFile,
    used by reader at gold img 0x36473 fread 0x102 = mission-display),
    NULL -> Message("No Mission Data",1)+ret 0; second -> local File.
  * Init: LASTPLANE=70, warstatus=2, DOINVISIBLE=0, planelanding=-999,
    7 zeroed globals (ours 6), + 10-iter loop word[0x6684+i]=1.
  * bounds clamps numpaths>=40, numwps>=85, numactions>=130 (we have).
  * sections: paths@0xb5e4 x8B, wpts@0xb132 x14B, actions@0x675c x4B,
    wp-z repair loop (imul 0xe).
  * THREE missing must-X loaders, same shape each:
    fseek(loc); for(k<n): flag[g]=1; fread(rec,0x68,1); type=byte@rec+0x10
    cbw -> typeArr[k*0x16]; for(i<10){id=word@rec+0x14+2i -> idArr[k*0xb+i];
    if(id<0)break}.  live: flag 0xc560, arrs 0x651e/0x6520;
    die: flag 0x6670, arrs 0x658c/0x658e; esc: flag 0xc566, arrs 0x65fa/0x65fc.
    src decls for these arrays not yet identified.
  * objects loop: byte global [0xaea2]=cnt; fread(obj,0x64); numjump local
    [bp-0x224] skip; `obj.object==0x9f(SO_GT) || obj.type in
    {0xc,0x13,0xf,0x10,0x11,0x12,0x14,0x15}` -> push cnt + copy obj to
    stack (sub sp,0x64; rep movsw cx=0x32) + call AddGroundTarget —
    arg order (obj,cnt) matches ours. Then `test byte obj[+0x50],0xf` =
    our `(number&0xf)==0` continue. Rest of loop tail undecoded.
- MAP.C cluster (_X2map/_Y2map/_mplot/_MapLoc): all callers #ifdef YEP'd
  dead code; anchors bogus; retail inlines `cl=7-Mscale; sar ax,cl;
  +41/+28` math. Defer (structural item, not a real port).
- Anchors fixed: _LoadFiles, _NDraw3DObject, _SetDLC, _show_range,
  _DrawDecoy->072e:0515, _Nline3D->1017:02f9 (gold.map entry added).
- pitchnum2: remaining 4 edits proven regalloc-context artifact
  (isolated probe reproduces gold ax-reuse; itoa/pitchbar2 tail kills it).

## LoadScottMission PORTED (decode -> source-complete)

- `_missionHeader_` rewritten to retail 258B layout (SETUP.H): field order
  world@+0x72, time@+0x74, weather@+0x76, ceiling@+0x78, loadout@+0x7a,
  minTimeAloft@+0x7c, counts +0x7e-0x88, objectLoc+0x8a, gtLoc+0x8e,
  mLiveLoc+0xba, mDieLoc+0xbe, mEscLoc..., pathLoc+0x92...
- `_object_` now retail 100B: name[32]@+6, callSign[16]@+0x26,
  squadMember+0x36, flight pad+0x38, quality(long)@+0x3a, side@+0x3e,
  x@+0x40, number@+0x50, path@+0x56, size 0x64 (sub sp,0x64/cx=0x32 verified)
- MustX[3][5] {int type; int ids[10]} retail globals at 0x651e/0x658c/0x65fa
  (22B/row); 0x68-byte mrec records fread per entry; flag globals
  DoMustLive/Die/Esc zeroed separately AFTER fseek (sub ax,ax not ret-val),
  =1 inside each loop; inner id copy loop `while(id>=0)` (jnl, sentinel<0)
- GetEscortNum(objid) added to PLANES3.C (gold 2fb2:6b2b): scans MustX[2][i]
  type>0 rows, ids[0..9] match -> row index else -1; MustX extern'd
- escnt[5] per-escort-group counts (local), escnt[g]++ on ST_PLAYER
- carrier remap = switch(ConfigData.Carrier){default:0x76 NIMITZA;1:0x77
  KHAWKA;2:0x78 FORSTALA} on objects==NIMITZA/KHAWKA/FORSTALA -> dec-chain
- wingman switch(OurLoadOut) jump-table 6 cases -> bigplaneid (byte-exact)
- ST_WINGMAN sets ourplanetype=one_object.object (global 0xc972)
- else-branch guards are `Message(...);continue` (cross-jump shared call
  tail at gold 0x962 confirmed reproduced)
- altval/speedval default conditions use `launch <= 0` not `!launch`
  (jg/jng = signed), altval<25 -> 2500, speedval<=0&&(launch<=0||altval>50)
  -> 300
- `lastalt = aiwaypoints[cnt].z = altval` chained store (jmp merges into
  lastalt=ax inside the other arm)
- object loop: lastadded(UBYTE)=cnt at top; fread(obj,0x64); numjump>0
  ->numjump--,continue
- tail: GetPlanePtr(ps[x].type)->bclass==CARRIER_OBJCLASS(0x3c)/0x46 checks
  (not IsBoat) in the whichourcarr/othercarr blocks; UpdateOurCarrier pushed
  whichourcarr / otherplane2; order verticlesep-- ;fclose;MissileAdd;Speech
- _AddGroundTarget: 258/258 insns normalized-equal (MATCH modulo epilogue)
- LOADMISS.C emits `enter 0x242` vs gold 0x240 (+2 = one extra int local)

REMAINING DIFFS ARE PLACEMENT/ARTIFACT CLASS: zfix-loop arm hoisting
(gold emits the two small arms between `jmp test` and inc, ours inline),
cmp operand-order polarity (jnl/jng), BSS absolute offsets (MustX at
0x651e vs ours 0x95d8 etc.), `mov si,bx` regalloc, jump-table data bytes.

RIOMAP.C + CMDS.C (Gold RIO map screen) — retail port COMPLETE
==============================================================
- Rebuilt module mirrors CODE_53 layout: BigRioDot(triangle 3-line),
  RioWpMark(plane validate + route draw), DrawRioWayPoints(aipaths
  sentinel loop), RioCircle(parametric ring + label), DrawRioTrack
  (UpdateOurWPS(-999,1) + wps[] trail + range rings 29/58 "100"/"200"),
  GetToken, TextInTheBox (do..while(T[l]!='\n'), no SetAPen), 
  DrawRioMapInfo 5-mode renderer (1=carrier 2=player+rings+trail
  3=wingman 4=friendly wps 5=non-F14 bclass sprite switch),
  RioLegend (MOVE/WINGMAN column fixup), RioMap (mission goal fread
  0x102 hdr, goalLen clamp >0xfe + S[254]=0 dead store, status strings
  Escort/Intercept/Survive-fail/MinTime, 6-case world map dispatch,
  RPS9 init {1,0,0,0x13f,0xc7,2,2,0,2}, key dec-chain
  ESC/W/R/O/P/F/C/B/M/SLASH, TRAINING(0x1000)-gated MKEY reposition)
- CheckOurAlt added to CMDS.C (gold 1081:1230-1273): register int q
  ->si + volatile int GroundH ->[bp-2] reproduces enter-2 + lea ax,
  [si+0x1f4] spill pattern exactly
- MSC6 local ordering: aggregates sorted by SIZE not decl order;
  register/volatile mix controls memory-vs-reg residence
- WayPoint {x,y,xL,yL} 12B stride matches retail wps[] base 0x4fde
- _path_ reordered {wp0@0,numWPs@2,name@4} (name field unused)

FOCUSED RESULT: 9/12 byte-match; 3 residual = pure codegen artifacts
- _RioWpMark: tp far-ptr home split di+[bp-2] + pcid [bp-1e] dead
  stores (volatile pcid reproduces frame; regalloc order differs)
- _DrawRioMapInfo: case-4 loop bound cmp operand-order (i-N vs N-i)
- _DrawRioTrack: same operand-order class at trail loop

Gold BSS: wps[] 0x4fde, NextWayPoint 0x500e, numplanes 0xc56e,
AIWingMan 0x3a0c, whichourcarr 0xc558, RioWingOK 0x4740,
Rp1 0x43ea, Rp2 0x43fe, Rp3 0x4412, MouseSprite 0xc562, Button 0x5a7e
DoMustLive->escort strs 0xc560, DoMustDie->intercept 0x6670,
DoMustEsc->survive-fail 0xc566, MinTimeSet->aloft 0x64c6

=== HSD pitch cluster (commit 2a97b7d, 546/756) ===
- Removed vestigial #include <stdlib.h> + "armt.h" (F-15 III header) from
  HSD.C: drops visible-decl count below MSC6's optimizer-tracking
  threshold -> arg-eval emits local-first adds + live-reg ax reuse as in
  retail. _pitchbar, _pitchnum2 now byte-MATCH. _pitchnum, _pitch_ladder,
  _scaler, _circlepoints, _drawcircle also MATCH.
- _pitchbar2: only residual is MSC6 CSE of shared (ox-t) between the two
  RpPrint calls (spills si-t to [bp-1a], reloads as cx) vs retail
  recompute on di after si<->di swap. No expr ordering /O flag /optimize
  pragma reproduces retail; volatile int t kills the CSE and restores the
  0x18 frame leaving 2-site load-vs-memop artifact. Semantic match.
- _fourpoints: pre-existing artifact (screen+x+y operand order + es:bx
  spill-vs-reload through cx:dx).
- tools/fdiff.py: read MZ hdrparas for image base (ours 0x5200, gold
  0x5400) instead of hardcoded 0x5000/0x5400.

=== Header-threshold sweep (557/756) ===
MSC6's optimizer tracking is bounded by visible-declaration count per
module. Removing genuinely-unused system headers drops modules under the
threshold and flips compare-direction / operand-order / CSE codegen to
retail shape. Applied:
- AWG9.C:   -bios.h -dos.h      (+4, -0)
- PLANES.C: -bios.h -dos.h      (+7, -5 artifact regressions)
- PLANES2.C:-bios.h -dos.h      (+2, -0)
- PLANES3.C:-bios.h -dos.h      (+3, -3 artifact regressions)
- BULLETS.C:-bios.h -dos.h      (+2, -0)
- BOATS.C:  -bios.h -dos.h      (+1, -1 artifact regression)
- CARR.C:   -bios.h -dos.h
- CMDS.C:   -dos.h (bios needed for _bios_keybrd)
Net: +20 MATCH, -9 artifact-class DIFFs. 546 -> 557/756.
Regressions are same artifact class (operand-order flips); recoverable
with per-function source adjustments later if needed.
Gold-only scheduler port pending: AWG9() tail calls FUN_08de_306c
(counter-gated: [0x12d0]++ >0xa -> call 0x3431 + reset; then calls
0x30d8, 0x31b2, 0x308e) gated by [0xae9c]==0x12. Gold lacks our
SearchForTargets() call in AWG9() head.

=== AWG9 tail scheduler decode (for next port pass) ===
Gold AWG9() head has NO set_acq() call (removed from ours — head now
byte-matches through DetermineShootParams). Gold tail before Aim9Sound:
    cmp word [0xae9c], 0x12   ; behavecommands == OTHERWINGAI (18, SHIFT-F8)
    jnz skip; call 0x306c
Mapping evidence: gold [0xae9c]==0x12 appears 3x (AWG9 + 2 wingman-cmd
sites); ours cmp [0x9d04]=_behavecommands ==0x12 3x. [0x1052] pairs to
our _DataLink@0xf08.

GOLD-ONLY helpers to port (AWG9_TEXT, before/after ~0x306c):
- FUN_306c (22B): ctr[0x12d0]++; if(ctr>10){F3431();ctr=0;} F30d8();F31b2();F308e();
- FUN_3431 (1fB): [0x104e]=0xbb2e; [0xbe30]=0; for(i<numplanes)w[0x76ec+i]=0
- FUN_308e (4aB): flag fixup — [0x906e]=0; if([0xc564]==0 && [0x1054]!=0){
    if([0x6972]){[0x906e]=1;[0x102c]=[0x66a2];ret}
    if([0x102c]==-1){[0x906e]=0;[0x102c]=-1;ret} [0x906e]=1; ret}
    else if([0x102c]!=-1)[0x906e]=1;
- FUN_30d8 (daB): TARGFILE filler — for(i<numplanes){ bx=rec(0x7e7c+0x28i);
    if(!(bx->f0&2)||bx->f2==0||TBL[0x76ec+i]!=0)continue;
    F325a(i,&T@bp-34); if(T.f@-28<0)continue;
    lobound=(T.f@-34==0?0xee:0x1dc0); hibound=(…?0xa:0x1d6);
    skip if (dx:ax)<lobound or >hibound on [bp-20] (32-bit cmp);
    x=math1e8c:5f([bp-28],[bp-2c]); y=math1e8c:5f([bp-28],[bp-2a]);
    az=angle18a7:19e(x); if(az>[0x103a])continue;
    el=angle18a7:19e(-y);if(el>[0x1036])continue;
    F3404(&T); TBL[i]++; }
- FUN_31b2 (a8B): sel fixup — if([0xbe30]==0){[0x102c]=-1;[0x906e]=0;ret}
    if([0x1054]){ if([0x102c]==-1)goto adopt; scan recs@si=[0x104e]
      stride0x32 dx<[0xbe30]: rec->f1a=[0x102c]; if(rec->f1a!=0){cx=1;break}
      if(!cx)adopt: [0x102c]=[0x104e]->f1a }
    if([0x102c]==-1)ret; AWG9_project(5 pushes rec@0x7e6a+sel*0x28);
    if(ax==-1){[0x102c]=-1;ret}
    [0x7df8]=[0x7e52]-[0x2f3a]; [0x7e0a]=[0x8960]-[0x2f3c]; [0x906e]=1;
- FUN_325a (178B) called by 30d8: fill TARGET(si,&T) — likely pairs our
  _SetupNewTarget (ours 0xec) or gold-only larger variant — CHECK.
- FUN_3404 (2dB) called by 30d8 tail: add-to-TARGFILE-ish.
Global identity work needed: [0xbe30]=?count(AGEDTARGFILECNT?),
[0x104e]=?ptr->0x32-stride recs (DESIG_TARG? sizeof check),
[0x102c]=?selected(SelectedTarg=ours@0x9c96, region differs — verify),
[0x906e]=?flag, [0x1054]=?(_InRangeFlash@0xf10 by -0x144),
[0x76ec]=int-per-plane tbl, [0x7e7c]=ps[]-ish 0x28-stride recs,
[0x12d0]=?junktime-adjacent counter(ours 0x118c region),
[0x66a2],[0x6972],[0xc564],[0x103a],[0x1036],[0x7e52],[0x8960],
[0x2f3a],[0x2f3c],[0x7df8],[0x7e0a],[0x104e]->0xbb2e = TBD.

Confirmed gold<->ours global map (via matched-fn operand pairing):
  gold 0x1008=_AWG9_REALITY_LEVEL, 0x100a=_RIO_ASSIST, 0x100c=_AWG9_MODE,
  0x1052=_DataLink, 0xc56e=NPLANES(_9f86), 0xc564=_AWG9_SNIFF(_9f7e),
  0x6672=_DESIGNATED(_5974), 0xae9c=_behavecommands(_9d04),
  0x7e7c=ps[] recs (0x28 stride; ours 0x701e+), 0xc97c=_DETECTED_TARG(_ab66)
Still TBD: 0x12d0 ctr (junktime region ~our 0x118c), 0x76ec per-plane
  word tbl parallel to ps (TARGFILE-occupancy flags?), 0x104e=ptr to
  0x32-stride recs (DESIG_TARG candidate), 0x102c selected idx
  (SelectedTarg sem.), 0x906e flag, 0x1054, 0x6972, 0x66a2, 0x103a,
  0x1036, 0x7e52, 0x8960, 0x2f3a, 0x2f3c, 0x7df8, 0x7e0a, ptr 0xbb2e.
SearchForTargets diff (real): gold `for(i=0;i<NPLANES;i++)` emits
  `mov ax,[i]; cmp [NPLANES],ax; jg` + `cmp [DESIGNATED],ax` +
  dual ptr locals [bp-0xa]=DETECTED_TARG, [bp-8]=ps — ours has
  operand-order flipped loop-head (compare-form source differs).

Additional decode notes:
- FUN_30d8 walks a 0x28-stride record array parallel to an int-per-plane
  array at 0x76ec (di+=2, bound by numplanes). If 0x7e7c is gold ps[],
  its stride 0x28(40) differs from our struct Planes=38B — check whether
  gold's Planes has 2 extra bytes (field we lack) or 0x7e7c is a
  different array. Field offsets used: [bx]&0x2 (status ACTIVE),
  [bx+2]!=0 (speed/alive). [bp-34..] = TARGET local filled by F325a;
  [bp-28]=dist-ish, [bp-20]=long, [bp-34]=mode flag (0->0xee/0x1dc0,
  0xa/0x1d6 bounds), calls 1e8c:5f (hypot/range), 18a7:19e (angle),
  then F3404(&T) adds to file + TBL[i]++.
- FUN_31b2 recs at [0x104e]->0xbb2e have 0x32(50) stride, field +0x1a
  holds "selected idx". Not DTARGET(8B)/TARGET(20B). Candidates:
  AGEDTARGFILE elems? a 50B tracking struct.
- _AWG9 now diffs ONLY at tail: missing `if(behavecommands==OTHERWINGAI)
  WingmanAISched()` + the whole helper family.

=== Session update: wingman scheduler family ported (563/756) ===
- _AWG9 tail ported: `if (behavecommands==OTHERWINGAI) WingmanAISched()`
  reproduces retail's `cmp [0xae9c],0x12; jnz; call 306c` — _AWG9 MATCH.
- Helper family appended at EOF of AWG9.C (decoded from retail asm):
  WingmanAISched(F306c, +cnt>10 reset), WingmanListReset(F3431:
  WING_PTR=WING_TARGS, cnt=0, clear DETECTED_TARG[]), WingmanAdd(F3404:
  TARGET*WING_PTR + cnt, rep movsw copy, cap 10), WingmanScan(F30d8:
  ps[] scan — status&ACTIVE, speed, !DETECTED_TARG — SetupNewTarget,
  rz>=0, long-range gate `(conv)?238L:7616L` max / `?10L:470L` min,
  az=ARCTAN(rx,rz), el=-ARCTAN(ry,rz), |az|>Angles[DEG20].wdeg,
  |el|>Angles[DEG40].wdeg gates, WingmanAdd+DETECTED_TARG[i]++),
  WingmanSelect(F31b2: revalidate LOCKSHOOT vs list; AWG9_project
  ps[LOCKSHOOT].xL/yL/alt==-1 -> LOCKSHOOT=-1; else update wing tx/ty),
  WingmanFixup(F308e: WING_HAVETARG flag mgmt + LOCKSHOOT=LOCKED when
  !AWG9_SNIFF && WING_SELOK && WING_LOCKOK).
  New statics: WING_CNT(gold be30) WING_SCHED(12d0) WING_SELOK(1054)
  WING_LOCKOK(6972) WING_HAVETARG(906e) WING_TARGS[10](bb2e)
  WING_PTR(104e) WING_TX/WING_TY(7df8/7e0a).
  Gold global map confirmed: 0x1006=SELECTED_AZ, 0x1004=SEL_BAR_NUM,
  0x102c=LOCKSHOOT, 0x66a2=LOCKED, 0x1030=Angles[] base
  (Angles[1].wdeg=0x1036=DEG40, Angles[2].wdeg=0x103a=DEG20).
- _CheckAZELChange: prevaz/prevbar moved to file-scope statics.
  Remaining DIFF is pure artifact: gold emits `mov ax,[SEL_AZ];
  cmp [prevaz],ax` vs ours `mov ax,[prevaz]; cmp [SEL_AZ],ax`.
  Probe .LST proves MSC6 emits gold form for identical source in a
  fresh module — the flip is module-decl-context regalloc, unfixable
  by source order/scope/volatile (all probed). Same class as
  _CheckDataLink tail (`cmp [NPLANES],ax` vs `cmp [bp-2],ax`) and
  _SearchForTargets loop head — banked as known artifact family.
- _doaaa diff also artifact-class: gold walks ps via bx=+0x12(pitch)
  anchor, ours +6(alt) — strength-reduction bias difference, identical
  field reads. struct Planes is 40B (comment "38 bytes" is stale;
  stride matches gold 0x28 — NO missing field).

## Session — wingman scheduler byte-match + firemissile (cont.)

- AWG9() tail: `if (behavecommands == OTHERWINGAI) WingmanAISched();`
  before Aim9Sound — _AWG9 MATCH.
- Wingman helper family (gold 08de:306c/308e/30d8/31b2/3404/3431) ported to
  EOF of AWG9.C: WingmanAISched/Fixup/Scan/Select/ListReset all
  instruction-identical; WingmanAdd differs by ONE dead-store
  (`mov [bp-2],ax` — MSC6 eliminates the dead local store, gold kept it).
  WingmanSelect needed `found=0` pre-init + `return (WING_HAVETARG=0)` /
  `return(0)` tails (gold materializes ax=0 on early exits).
- Gold global resolutions via matched-fn pairings: 0x6972==SHOOT,
  0x66a2==LOCKED, 0x102c==LOCKSHOOT; 0x1054/0x906e/0xbe30/0xbb2e/0x12d0 are
  scheduler-private (written only inside the family) -> statics.
- _firemissile MATCH: gold's arg9 = `weapon==W_AIM_9 ? LOCKSHOOT :
  (SHOOT?LOCKED:-1)` — Sidewinders take the scheduler lock target.
- AWG9 residual diffs now all artifact/pairing class: _AddAcqList
  (t in di vs bx reload), _CheckAZELChange (data-offset map collision),
  _CheckDataLink/_SearchForTargets (loop-head cmp operand order),
  _LockDistanceCheck (`int radarsig[1]` reproduces enter 0xa + dec ax form;
  residual = `cmp [bp-a],5` vs `mov ax,[bp-a];cmp ax,5` on the last test),
  _show_az_carat (gold routine is a different function — scaled-point
  helper, needs real port), _Message/_MissilesInAir (mch anchors point at
  wrong gold routines).
- count: **564/756 MATCH**.

- _show_jamming MATCH: gold's body is
  `if (JAMMER[0]) { if (JAMMER_ACTIVE && !off) RpPrint(..,"JAMMER");
  else if (!JAMMER_ACTIVE) RpPrint(..,"JAMMER"); }` — not
  "JAMMER ACTIVE". Global map: gold 0x527a=JAMMER_ACTIVE,
  0x527c=JAMMER[0], 0x5294=ticks, 0x5296=off (byte xor toggle).
- TEWS APen setup: show_radar/show_plane set RpCRT->APen=0x56,
  show_jamming=0x54 at head.
- _show_radar/_show_plane residual = instruction-scheduling order
  around the RpPrint call (same ops reordered, regalloc class).
- count: **573/763 MATCH** (DDD cluster clean: _DoDDD/_show_az_carat/
  _DDDWeaponRanges/_DopplerCheck/_convert_xy/_show_jamming all MATCH;
  _DDDTargetDraw colors 84/88 fixed, frame/regalloc residual remains).
- Triage of all 190 DIFFs: dominant classes are (a) compare operand-order
  (mov ax,[local];cmp [global],ax vs reversed) — contextual MSC6 pick,
  source-insensitive; (b) si/di regalloc; (c) index vs pointer array
  addressing (bx=i*stride +[bx+base+off] vs bx=&arr[i]+[bx+off]);
  (d) eval-order of commutative operands (param-first always chosen).
- doaaa: real fix — struct GroundTarget reordered to {x,y,orgnum,gflags}
  (gold reads x@si-0xa via +0xe-biased ptr, orgnum@+8 confirmed by
  CheckAAAHit). Residual = loop-tail operand order only.
- Still-real diffs noted: _AwacsPicture (eval order (Diff+1)*60 vs
  Gtime-LastCall — MSC6 picks param/sub first regardless of source),
  _Do3DObject (ViewY+OY order — all 4 source forms emit param-first),
  _DoPlayerOnTheCat (gold switches on different var [0x28c8]),
  _AISanitizeCheck (block-layout + cnt-in-ax regalloc),
  _DESIGNATED_TrackingAI (index-form vs pointer-form bv[] access).
- mzretools note: /Fc writes .COD only with bare filename (/Fcname.cod),
  /Fl with drive path (/FlD:\X.LST); MZ hdrsize gold=0x5400 ours=0x5200.
- count: **573/763 MATCH**.
- CARR.C big win (576/763): retail DID compile the `#ifdef YEP` heave
  block in DoPlayerOnTheCat — but with different constants than the
  dead source: switch(StormLevel){case0/1:WS=WH=0;case2:WH=3,WS=110},
  plus `if(StormLevel<2)HeavAngle=Heaving=0` (chain stores L-to-R).
  Also retail: CATVEL+=20*Ft (not 30) + `if(CATVEL>0xec4)CATVEL=0xec4`
  clamp, AirSpeed before ONTHECAT+=, `if(ONTHECAT>0x2710)`=10000,
  `return 1` (int-returning, not void).
- DisplayBall: retail calls SlopeCheck(X,Y,Z,&HD,&PTCH,&DELHT,&DST)
  NOT GlideScopeCheck; PTCH+=637 then MeatBall zones {<=455:4,
  456-544:3, 545-727:2, 728-818:1, >=819:0}. Locals PTCH@bp-2.
- Carrier3D calls DisplayBall() between dz compute and abs(dz)
  (near call inside CODE_20 — retail-only interposition).
- KeepPlaneOnCarrier: `if(planenum==-1)return 1` is an early return
  (dead `if(!WireCaught)` block is genuinely absent in gold —
  reconstruction artifact confirmed). Residual: basenum cmp order.
- Dominant residual class proven by batch triage (batchdiff.py):
  `local<global` loop-tail compares — gold `mov ax,[cnt];cmp [NG],ax`
  vs ours `mov ax,[NG];cmp[cnt],ax`. ~25+ functions. MSC6 operand pick
  is context-sensitive (ax-liveness from prior store); standalone
  probes emit gold's form, module context flips it.
- Other classes: si/di regalloc swap (~15), index-form `[bx+base+off]`
  vs pointer `[bx+off]` for arr[i].field (~10), eval-order for
  commutative exprs (param-first always).
- GetCloseAirBase MATCH: retail has `ainum==-1` player-coord branch
  (xydist(Px-R.x, Py-R.y)) ours lacked; mask is AIRBASE+DESTROYED (0x81)
  NOT +NEUTRAL; `volatile COORD d` + decl-order swap (`tdist` before `d`)
  reproduces gold's stack layout + jnb.
- EasyLanding: `if(Night||MW==0||landpt<3)--l; elif(l<5)l=2; else l-=4`
  was written as goto+separate-if; semantics now exact but MSC6 still
  emits dec out-of-line last vs gold inline-first — block-order heuristic
  not driven by braces/goto/elif-nesting (all probe variants identical).
- MSC6 long-add rule proven: `global ± param` ALWAYS loads param into
  ax:dx first (probes: all source forms identical). Gold's
  `mov ax,[ViewY];add ax,[OY]` therefore comes from a different tree —
  Do3DObject dy remains uncracked (eval-order class).
- Verify corrected: classified.tsv is the second input (not classes.tsv);
  bogus matches.txt anchors for _DoPlayerOnTheCat removed — it now
  MATCHes at cls anchor 0dc3:1c10. Current: 532 MATCH / 763 pairings
  (mch 440/644, cls 92/119). Prior 576 figure used different pairing set.
- DrawTexPlane: gold re-reads es:[bx]->PlaneEnumId per compare (no CSE)
  and orders the bv[ID]/AnimVals statements differently — 352-insn
  function needs deeper reconstruction pass.
- DrawSBox MATCH (576/763): retail MapInBank arg is an if/else on
  `BITID>0x7f` — `(BITID-0x80)/16&15 +8` stamped-arm vs `STMPBANK+
  (BITID/16&15)` else. KEY FORM: `if(c) MapInBank(A); else MapInBank(B)`
  emits ax-resident + merged `push ax;call` tail; ternary or temp-var
  forms stage through si/di (probe-proven). Applied to all 3 call sites.
- DoBox: added missing `GRNDDETAIL==2 && (BITID&0x7f)==0x7f` early-out
  (gold 0342:09ff) and `SZ2=SZ` (register copy) with SZ2 throughout the
  bounds test — both match. Sole residual: `VIS1|VIS2` leaf-or order —
  gold `mov al,[+7];or al,[+6]`, ours forced to +6-first by MSC6's
  offset-sorted | canonicalization (30+ probe variants: no source shape
  reproduces descending order). Then dataMap[6]=7 collision aborts at
  the byte-identical `test es:[bx+6]` — tool-side cascade of the real
  order diff. Semantically equivalent; flagged as canonicalization-class.
- CheckWingmanSix loop tail: gold `mov ax,[cnt];cmp [NPLANES],ax;jle`
  vs ours `mov ax,[NPLANES];cmp [cnt],ax;jge` — identical context incl.
  `inc [bp-2]`; operand pick is descriptor-state-driven, not source
  direction (NPLANES>cnt flip emits same). Probe: register-resident
  counter gives gold's `cmp [NG],reg` shape — memory counter doesn't.
- AddAcqList: gold pins `t` into di before the AWG9==EASY test and
  reuses it for ps[t->i].type (imul 0x28) + t->conv/d tests; ours
  reloads bx per site. Semantics confirmed identical (EASY gate present
  in gold at 0x28b3, MODERATE at [0x1008]==1, AUTHENTIC at ==2 with
  4988/159616 and 1900/60800,4513/144416,... sig table). Residual:
  di-vs-bx pointer pin + preload hoist — regalloc class.
- verify_full.log: 576 -> 577 MATCH / 763 pairings (AIGetBombDist MATCH)
- MSC6 compare rule (probe lp8): unsigned<unsigned emits
  `mov ax,[RHS]; cmp [LHS],ax; jnb` ; signed mixes emit LHS-load.
  &&-context with trailing abs() calls forces LHS-load regardless —
  CoreCheckGunFiring keeps residual mov ax,[tdistl] vs gold's
  mov ax,[tdistfire] under identical source.
- ChooseBehavior/AISanitizeCheck/CheckDataLink: the local-vs-global
  `cmp` operand pick + eval-order is a symbol-table/descriptor tie-break
  — identical source emits different order under different register
  pressure. Also feeds the `push ax` vs `push [bp-X]` arg diff (ax=cnt
  live iff tail loads cnt first).
- Mispairings found (verify DIFF but not real routines):
  _UpdateLights 2fb2:1759 = 4-arg position query; _ChangeColor/
  _ClipSprite 4140:10c4 = 6-byte thunk stubs.
- verify_curr.log: 577 -> 581 MATCH / 763 pairings
  - _CheckLandingAndCrash: Gtime<2 mission-start grace term; !ejected guard on crash LogIts;
    RadarAlt+=10 before OurAlt+=10; REALFLIGHT fixup block unconditional — MATCH
  - _End3D: sweep state (bounds -15/45, +12/-5, PlaneAnimVals[1]=SweepVar*DEG, [2]=*-DEG),
    GrndAnimVals[16/17]->PlaneAnimVals copies, beacon ternary, UpdateCarrierAnim() call — MATCH
  - _UpdateCarrierAnim: launch-strobe preamble + CarrierAnimVals[28]=Night||!LightsOn,
    else-if cat branch structure — MATCH
  - _FireColors: no direct Palette[] writes, bounds 3*9/3*6, MCGAColors(ORANGEFLAME,2,Buf) — MATCH
  - _AIGetBombDist: flipped compare arms (pdest=0x2000 uses >, pdest=0xD000 uses <) — MATCH
  - _CoreCheckGunFiring: behaveflag mask is ENEMYNOFIRE|ISHELICOPTER (0x82 byte test) — MATCH
  - Parked classes: mem-reg compare operand order (binder pick), si/di/ax regalloc,
    MSC6 |/== leaf canonicalization (DoBox VIS1|VIS2, DoOClockCall equality),
    mispairings (TacBox, MBox, UpdateLights anchor)
  - FLIGHT still large structural diff; ejection rng bounds fixed to +-64/FrameRate
- verify_curr.log: 581 -> 589 MATCH / 763 pairings
  - _DrawSettings: retail drops "Vaughn Thomas"/"Version: 1.18" RpPrints — MATCH
  - _DrawBullets: color ternary -> local, then trs[i].firedby!=-1 -> GetPlanePtr
    -> PlaneEnumId==0xa7 -> col=0x92 recolor block, sx/ssx guard last — MATCH
  - _GetTargADP + _StraightAttack: fixed via inner-extern binding — MATCH
  - KEY MECHANISM: MSC6 evaluates the LATER-BOUND operand first into the register.
    Params bind at function entry; globals declared file-top bind early. An
    `extern` declared INSIDE the function body binds late -> global-first emit.
    Retail F14 style appears to use function-scope externs; the port hoisted them.
    Removing file-top externs perturbs module symtab -> can flip unrelated
    operand picks (regressed _AddSomePlanes2's whichourcarr!=cnt compare).
  - Parked: Boats3D pointer-walk + bv[i].field0&0x800 test; FlyTo preamble
    (gold has bounds-check+exit, different frame); EasyLanding then/else layout.
- verify_sweep_new.log: 585 MATCH / 763 (verified current; earlier "589" likely
  included transient probe state — committed tree reproduces 585)
  - _InitCarrier: retail CAT0Z {165,190,174}, CAT1Z {...,136}, CAT2Z {-221,-168,-205},
    CAT3Z {-221,-185,-205} extracted from image (committed). Still DIFF: retail
    frame 0x10c vs ours 0x10a, scrambled local-array order, extra live local.
  - _GenDsp: PalPkt -> static (retail routine is frameless). Frame gone; residual
    diffs are branch-layout/data-offset. Committed.
  - _InitSound: gold is a pure dec-ax switch cascade on [0xae94] with case values
    {0,1,2,3,4,0x46-0x48,0xca-0xcd,0xce,0xd0-0xd1} writing 0x8b/0x96/'n' bytes to
    [0x1c86] — does NOT match our music_type switch on C_* consts (0x10..0x38).
    Either retail-only logic or a bad pairing; needs identification.
  - _NewLockView: ours' candidate anchor (3b2e:0aaa) starts mid-instruction —
    mispairing, needs map/pairing regen.
  - Parked class inventory (first-diff scan, /tmp/cls.tsv):
    ~33 SAMEOP:mov — dominant sub-class: ours reads local [bp-X] copy where gold
      reads global directly (~12 fns: GetWhichElevator, CheckDataLink,
      CheckWingmanSix, ReArmWingman, SearchForTargets, UpdateAIsOnCarrier, doaaa,
      GetEmsMem, ResizeEmsMem, SetMatsPtrsOrder, lproject, HeadProject) —
      local-vs-global residency / eval-order, not per-line fixable.
    ~30 SAMEOP:push — arg-eval-order + si/di prologue-save picks.
    mem-reg cmp flips + jge/jle polarity: symtab-order class (All2AirHoming,
      AwacsPicture, CheckProximityHit probes confirmed not source-spellable).
    si-zero-cache regalloc (InitSpeech): gold keeps si=0 live for two zero-uses.
  - _DoCockpitDisplays: retail-only helper FUN_120f_08e1 call in DOWN block.
- Re-anchoring round (verify_sweep_reanch2.log): 595/763 (+10, zero regressions)
  - mnemonic-prefix scan over gold routine starts found 62 changed anchors;
    10 became MATCH (InitGraphicPages, JetisonSwitch, MBox, MyMessage,
    RedrawJoyBox, TacBox, UpdateCloseBase, ZoomDisplay, diamondbox,
    draw_shoot_cue) — they were correct code all along, wrongly paired.
- Byte-scan reanchor (/tmp/bytescan2.py — masked-operand needle over whole
  gold image, not just Ghidra starts): +6 more MATCH:
  GetBehaviorLeader 2ad2:0718, InitSound 4a6f:0008(eff 3a6f:0008),
  LaunchAircraft, MissilesInAir, ReadEMSPCX, __dos_read.
  ~45 DIFF names get NO byte-scan hit => genuinely different head bytes
  (real source diffs or merged/!present retail routines).
- _MBox: retail fill color is 5 not 17 (DrawLine 18 ok) — committed.
- Discovery: verify_build.py reads matches.txt first-row-per-name wins;
  matches.txt dev@ offsets are stale but unused; only gold seg:off consumed.
- Discovery: '=~' (INS_MATCH_DIFFOP2) counts as DIFF — memory operands that
  can't resolve to matching var names. Feeding DATA-segment publics into
  the candidate map may convert many of these to MATCH (unexploited).
- verify_sweep_600.log: 601 MATCH / 763 (+16 via re-anchoring)
- Refinements: '=~' is a dataMap offset-collision (per-compare OffsetMap,
  offset-keyed not name-keyed) => feeding var symbols won't fix it; treat
  as real/inconsistent operand mapping. dataMatch allows maxData=#DATA segs.
- ChangeColor/ClipSprite/DrawDot all anchored to same 4140:10c4 (cls
  cascade) + byte-scan nohit => genuinely different head bytes.
- IsLand correct anchor, real mid-func diff (extra far call in retail).
- verify_reanchor2.log: 603 MATCH / 763 — _CheckProximityHit re-anchored
  to FUN_38b6_0b6e (48b6:0b6e); injected false-positive 3ad2:0728 row was
  shadowing it. Swept all 89 alternate candidates of remaining DIFFs:
  none match — remaining first-rows are already optimal.
- _CheckDetection MATCH via local decl reorder (returnval after
  tdist/tdist1 kills an ax spill). Same lever as _CheckE2C.
- si/di/bx register-residency class (_CheckPlaneLand startval,
  _GetQHeight param, _CheckTurnStuff) is NOT driven by decl order and
  NOT overridden by 'register' keyword — MSC6 cost model follows actual
  use pattern => likely real source/usage differences, parked.
- _CheckForMobileR5, _GetWhichElevator: '=~'/'~~'-only — DGROUP offset
  collisions; need gold-vs-ours variable-layout table to confirm equal.
- _CheckTurnStuff/_DisplayRioHead/_GetWayPointChanges/_GetWhichElevator
  remain anchored at shared bogus 1017:02f9; byte-scan nohit for all —
  real head-byte differences (or absent/merged retail routines).
- _EasyLanding: anchor 0dc3:22c1 correct; bodies identical through the
  weather/landpt chain (all data-off diffs normalize). REAL TAIL DIFF:
  retail has an extra epilogue block ours lacks — near call FUN_0dc3_2293,
  push [0x7a78]; call FUN_1604_0dda (CRT28, sprintf-class), [0x25c0]=ax,
  [0x186a]=0, call FUN_1604_0c69, then re-tests the 0x5038/0xca1e/0x145a
  trio. Likely LSO-string/lastlso bookkeeping (Coursestr cluster sits at
  file scope nearby). Needs identification of the two CODE_28 callees and
  the three globals before source reconstruction.
- Mass classification (tools replica of verify pipeline): of 160 DIFFs,
  82 are =~/~~-only (zero '!=' insns). =~ collisions arise when the SAME
  gold offset pairs to different ours offsets inside one routine — real
  access-order/codegen differences, NOT a bulk-fixable class.
- _EasyLanding extent fixed (R22c1-24a0 continuous): bodies identical
  through tail; ONLY remaining diff is then/else placement of the
  `--landpt` vs else-chain at 0x246f — resistant to polarity swap AND
  else-if flatten AND negated-block swap (MSC6 block-order invariant).
- FUN_1604_0dda==_Get1GTrim, FUN_1604_0c69==_InitScottFlight (byte-twins
  confirmed via mzdiff). EasyLanding tail calls match in source already.
- _GetEmsMem/_ResizeEmsMem: gold `cmp [EMS],ax(local i)` vs ours
  `mov ax,[EMS]; cmp [i],ax` — operand choice resists decl reorder AND
  source comparison flip (flip changed frame 6->0xa, worse). Parked.
- First-diff table for all 160 DIFFs captured (build/ + /tmp): biggest
  cluster is 82 =~-only (data layout), rest are per-function codegen.
- NQFLY.C fixes committed: SBox struct VIS2@6/VIS1@7 (retail layout —
  proven by DoBox emitting +7-first under MSC6's offset-ascending leaf
  canonicalization while DrawSBox tests VIS1 at +6); DrawSBox use-sites
  retargeted (VIS2@184/211, VIS1@242); MakeBoxes `t>5/t=5` (was 6).
- _DrawSBox stays MATCH after retarget; _DoBox residual = es save/reload
  codegen for far-ptr field copies (parked); _MakeBoxes residual =
  regalloc order (parked).
- MSC6 leaf-order rules so far: '|' leaves emit by ascending field offset;
  cmp/arith operand pick = later-bound symbol first; block then/else
  placement invariant to source spelling; register residency (si/di/bx)
  driven by use-count, not decl order or 'register' keyword.
- _SearchForTargets/_CheckDataLink/_UpdateAIsOnCarrier: ours reads global
  [0xa476] where gold reads local — likely file-scope var that retail
  made function-local (needs deeper context diff).

## Turn: 607 probe results — operand-order mechanism boundaries

- `cmp` direction for `==` and `+`/`add` operand pick are NOT controlled by
  the late-binding rule: _GetRadarGTNum (`cmp [bx+off],ax` vs ax-left),
  _CheckProximityHit (`cmp losing,F5;jl` vs `cmp F5,losing;jg`),
  _CheckTailAspect (`si=y.lo+[PyLng]` vs `si=[PyLng]+y.lo`),
  _TrooperDrop (`al=Rtime/cl=prewing` vs `al=prewing/cl=Rtime`) — all
  spelling-invariant (tried `>`/`<` swaps, `mem==expr` order, inner extern,
  `y+PyLng-0x100000` and `y-0x100000+PyLng` flattenings, UWORD type change).
  Isolated probes DO emit gold shape (near-ptr field cmp mem-left), so the
  pick is driven by in-function register/liveness context, not decl order.
- _UpdateAIsOnCarrier keeps `cmp [bp-2],ax(NPLANES)` despite extern+`>` —
  inner-extern rebind worked in AWG9.C (CheckDataLink) but not BOATS.C;
  both bind NPLAS via the same WORLD.H extern (def lives in STEALTH.C
  under `_include_data`). Unexplained; parked.
- Verified probe pipeline: kvikdos CL.EXE -> OBJ -> LEDATA extract ->
  objdump -m i8086 works for codegen experiments (see /tmp/probe/cmpp*.c).

## Turn: DGROUP layout analysis — the =~-only class (82 fns) decoded

Harvested 7591 gold<->ours data-operand pairs across all 761 pairings
(tools/dseg_pairs.py -> analysis/gold/dseg_pairs.tsv), built per-variable
displacement map (analysis/gold/dseg_order.tsv, 482 vars).

Findings:
- Retail's DGROUP order is a PER-VARIABLE reordering of ours, not module
  blocks. e.g. _psi: ours 0xa4a2 -> gold 0x61a4 (next to _OBJ);
  _TARGFILE: ours 0x6170 -> gold 0x104a (very early); _NPLANES/_FPARAMS
  ours 0xa476+ -> gold 0xc56e+ (+0x20f8).
- Dominant deltas: +0x79e (346 pairs, post-_edata BSS region), +0x816
  (981 pairs, _OBJ/_bv region ours 0x6180-0x6600), +0x96e (416, _ps
  region 0x74c0+), +0x1f34-0x1f5e (~300, ours 0x8ec0-0xa440 region),
  +0x38a (~300, ours 0x4c54-0x51b0 PlaneDataList..PadLockee block).
- Early region (ours <0x2000) has small +deltas (0xec block = GRPAGE/
  GXYLO/OurHead etc. at ours 0x1d70-0x24f2 -> gold 0x1e5c-0x2498).
- Negative deltas exist too (_psi -0x42fe, _Mapy -0x2e, _TARGFILE -0x5126)
  = vars retail placed EARLIER than ours.

To fix: reorder data declarations so each var lands at its gold offset —
vars may need to move BETWEEN files (module granularity) AND within files.
Retail code/data boundaries: gold _edata = ours 0x572c+0x79e = 0x5eca;
gold end-of-BSS ~0xd658+ vs ours ~0xbbe0 — retail has ~0x1a00 more data.

No CodeView in retail exe (checked NB09-NB11/$$SYMBOLS — stripped).

## Turn: COMDEF allocation mechanism — the =~ root cause identified

Empirical discovery via controlled kvikdos LINK experiments (/tmp/comtest):
- Uninitialized globals become OMF COMDEF records; LINK 5.31 allocates them
  into the shared `c_common` BSS-class segment in an order that is NOT
  decl order, NOT module order, NOT extdef/ref order.
- Evidence: `int a1..a10`(M1)+`d1`(M2)+`e1`(M3)+`f1`(M4) emits as
  `a1,a2,d1,e1,a3,f1,a4,a5,a6,a7,a10,a8,a9` — modules interleave freely.
  Identical decl-order reversal yields IDENTICAL layout (name-driven, not
  order-driven). `qa..qz` in one module emits rotated `qh..qz,qa..qg`.
- Hypothesis: LINK emits comdefs in internal symbol-hash-table order
  (hash includes ALL publics+comdefs+extdefs — 3000+ symbols, so fitting
  against comdefs-only fails). Brute-force over rotate/poly/sum hashes
  with chained + linear-probe models found no universal fit yet.
- MASM `var dw ?` in a BSS segment is a real alternative: ordering fully
  controllable (decl order), but gold interleaves comdefs with library
  data and segment members — would need multiple data stubs placed at
  multiple link positions, and gap contents are unknown vars.

Data for next turn: analysis/gold/dseg_order.tsv (482 named gold offsets),
our_comdef_order.txt (our emission order). Retail has ~0x1a00 more data
(gold _end ~0xd658 vs ours 0xbbe0) — missing vars both shift hash order
and explain gold-side gaps.

Pending decision: crack LINK's exact comdef order algorithm vs rebuild
layout by direct ASM data blocks vs per-symbol hashing simulation.

## 2026-10-18 — GDSEG replica approach: mechanism works, hits 64K wall

Attempted the explicit-layout fix for the ~82 `=~` functions (identical insn
streams, only DGROUP operand offsets differ):

- `src/GDSEG.ASM` (generated): `GDSEG SEGMENT BYTE PUBLIC 'BEGDATA'` linked
  first in OBJS — `ORG` + `PUBLIC _x LABEL BYTE` for all 480 mapped gold vars
  at their retail offsets; raw retail image bytes emitted 0..0x642e; real
  fixups for `_PalPtr3D` (DW _defaultpal) and `_Seg3D` (DW SEG _Seg3D).
- `tools/extern_defs.py` converted ~187 C globals to `extern` (all modules
  compile clean; idempotent; comment/string-masked parsing).
- ASM externs added in EGRAPH/EMS/JOYSTX/KEYJOY/PASM/HORZASM/_rp
  (`.MODEL MEDIUM,C` auto-underscores PUBLIC/EXTRN — `GXLO` IS `_GXLO`).
- Verified `_psi` landed exactly at gold 0x61a4 — ORG control works.

**L2041 wall**: GDSEG spans 0..0xd658 (gold's full data extent) + leftover
real data (strings/anonymous ~0x4745 _DATA + _BSS 0x632 + CONST + lib data +
103 unmapped comdefs) + stack > 64K. mzretools `OffsetMap::dataMatch` uses
maxData=1 (bijective per-function offset remap), so unmapped vars need only
unique offsets — but the leftover bytes physically occupy DGROUP space
regardless of matching, and ~0x4300 over the 64K cap.

Gold's image itself contains its strings inside 0..0x642e — our strings are
DUPLICATED content. To fit, the anonymous literal/anonymous _DATA must move
into GDSEG at gold offsets (labels + content-match search), or the whole
approach must switch to reproducing gold's COMDEF hash-order by matching its
exact comdef multiset (blocked: names of gold's unmapped vars are unknown).

Next candidate: string-literal-to-extern-`char[]` conversion emitting each
literal's bytes at its gold offset (content-matched), OR shrink GDSEG to
only vars referenced by `=~` functions if the high region isn't needed.

Tools: tools/dseg_pairs.py (harvested 7591 operand pairs ->
analysis/gold/dseg_pairs.tsv, dseg_order.tsv: 482 vars with gold offsets).

## Session 2025-10-18 (cont): gstr leaf-order restoration — 579/760

**Mechanism unlocked**: MS C 6 operand pick for mem-vs-mem compares
(`mov ax,[A]; cmp [B],ax`) is driven by **leaf-table ordering fixed at
function compile time** — the set/order of symbols bound so far in the TU.
The DGROUP extern + hoisted `gstr` passes perturbed that state vs retail,
where string literals bound as anonymous symbols AT THEIR USE SITE inside
each function.

**Fix**: tools/gstr_inner.py — moved every file-scope
`extern char gstrN[];` into the functions that use them (inner externs
after local decls, in use order). Replicates retail's bind-at-use leaf
positions. Result: **570 -> 579** (+21 fixed: _AvoidLand _AWG9_IFF
_AWG9_project _BulletDist _CheckAZELChange _CheckTailAspect _DoFuel
_DrawASmoke _GetEnemyTarget _GetPlaneLanding _GetWhichElevator _GorillaBomb
_GorillaCover _Ladder2 _Langle _MapEmsFile _pitchnum2
_ProcessAICommunications _Sound _UpdateTWSSelected _Wall; -12 regressed).

Also: `_UpdateAIsOnCarrier` restored via inner `extern int NPLANESx`
alias (late-bound global -> local wins ax); `_NPLANESx`/`_LOCKEDx` GDSEG
alias labels at target ORGs. WORLD.H `NPLANES` now tentative-def in both
include branches (neutral vs sweep, more faithful K&R common semantics).

**Proven per-function levers** for operand-order flips:
- inner `extern int <ALIAS>x;` AFTER local decls + alias label at target
  DGROUP offset (GDSEG.ASM `LABEL BYTE` at same ORG)
- source direction swap `<`/`>` — works only when leaf orders allow
- declare the induction local LAST among ints
- distinct anonymous string literals prevent over-merged tails
- `==` canonicalizes global->ax regardless of direction/binding — needs
  TU table-state tuning, not source tweaks

**Table-state tuning**: leaf order flips at symtable size boundaries —
e.g. 60 dummy decls before CheckDataLink flips its pick to gold; decls at
EOF don't reach earlier functions. Retail's TU state differs from ours by
the accumulated pass artifacts; per-function padding or decl placement is
the remaining lever for stubborn sites (_CheckDataLink, _CheckDetection).

**Regression set** (12, need individual table-state or shape work):
_AIBombTarget _AISanitizeCheck _CheckAISix _CheckDetection
_DesignateTIDTarg _DoPlayerOnTheCat _FriendlyChooseBehavior _horiz_line
_SetUpWingman _ShowAIWeaponStatus _StraightAttack _UpdateAIsOnCarrierFIXED

Diff set now 181 functions.

## 594/760 — EMS far-call encoding batch

- `call far ptr X` in EMS.ASM forces `9A` encoding (UASM/MASM smartcall
  otherwise emits `push cs;call near` for same-segment far procs).
  Fixed all 9 EMS functions (_EMSinit.._EMSver). Same lever applies to
  any .ASM intra-segment far call where gold kept `9A`.
- `tools/objfn.py`: per-function OMF OBJ disasm — fast emit checks
  (~2s) without relinking. PUBDEF parse handles 2-byte type indices.
- Link note: `build_all.sh link` reports ~92 L2025 "defined more than
  once" (GDSEG CRT symbols vs MLIBCA members) — non-fatal; linker keeps
  GDSEG's replica defs and writes the EXE. relink_check.sh silences
  this; F14.EXE/F14.MAP timestamps confirm output stays fresh.
- `_DoCockpitDisplays`: callee segment-home mismatch — gold had
  `_GDisplay` same-segment + `_DrawVVGauge` external; mine is reversed
  (both in COCKPIT.C). Needs function relocation between TUs, not
  codegen tweaks.
- `_CheckDataLink` still DIFF: first-loop `i<NPLANES` emits ax=global
  in this TU regardless of direction/alias/leaf — extern-class and
  tentative both lose. Residual TU symbol-table state issue.

## Remaining ~166 diff inventory (first-mismatch opcode histogram)

xor ax,ax ×8 · push di ×8 · cmp word ×8 · mov word[L] ×7 · mov ax,[] ×7
+ long tail: regalloc si↔di swaps, stack-slot order, polarity flips,
structural (block-layout over-merge/out-of-line arms), DGROUP placement
(_CheckAAAHit Dist2Obj@0x60d2 class), byte-scan nohits, retail-only
helpers.

## 595/760 — session notes: pairings audit + lever refinement

- `_CheckE2C` MATCH via decl reorder (`signal` after `dist`) — commit
  e29809f. Decl order still a working lever for slot+leaf alignment.
- **Pairing audit**: of the ~165 remaining diffs, classified.tsv marks
  118 NOCAND-UNVERIFIED + 9 NOCAND-CLAIMED + 1 NOCAND-WEAK. These pairings
  were similarity-chosen, never anchor-verified. Verified with repair3.py
  same-segment re-anchor scan: best anchor already chosen for nearly all —
  the diffs are REAL codegen diffs, not mis-anchors (exceptions: the
  FUN_4140_0c2d stub cluster — _ClipSprite/_DrawDot/_DrawLine/_DrawPoly/
  _MouseSHOW/_SndSounds/_ChangeColor/_RpPrint all fuzzy-match the same
  gold stub; true homes unknown).
- **Dead ends ruled out this session**:
  - `extern` vs tentative-def symbol class: identical emit in minimal
    probe — NOT the operand-pick lever.
  - `register` keyword on params/locals: no effect on MSC6's pick here.
  - Local param-copy (`TARGET *p = t`): copy lands on stack, not di.
  - `/Oa` module flag: no change for PLANES2.
  - `==`/`add` generators: unconditionally canonicalize global→ax — not
    source-order-sensitive. Sites needing local→ax must come from a
    different surrounding structure (e.g. more work inside the arm).
- Confirmed lever set: inner-extern alias decls (NPLANESx/_y, Altx),
  decl order (drives both stack slot AND leaf order), `call far ptr`,
  dropping unused headers (symtable load), hoisted-vs-inner gstr decls.
- triage_fn.py pairs via classified.tsv `cand` column first; verify_build
  order is anchor→deepaddr→bestaddr→cand. Column choice explains some
  reported-vs-actual anchor drift when re-checking manually.

## Probe results (minimal TU, fp*.c): si always = first int param

- For `*(ptr+a±b)` forms, MSC6 deterministically puts the FIRST int param
  into si and the second into di — operand order, `register`, local
  copies, subscript vs deref syntax, /Os variants: all emit identically.
  `_fourpoints` gold has si=bp+c (second param) — implies a different
  source structure than any tested shape; parked.
- `_X2map`: gold mutates through a pointer (`sar [bx],cl`) — different
  API shape than mine (returns shifted int). NOCAND pairing; parked.
- `_AddAcqList`: gold keeps param `t` in di across the function; MSC6
  allocates di only via leaf order — locals i/j claim si/di first here.
  No source lever found (decl order, register hints, local-copy all fail).
- `_DamageAI`/`_AIBombTarget`: gold derefs a global pointer + does
  `imul word [g]` in-place scaling — structurally different eval order;
  real TU-state/codegen diffs.

## Standing count: 595/760 (78.3%) — committed through e29809f

## Decl-pad / symtable-state matching (breakthrough, this session)

MS C 6 codegen (operand-order leaf picks, si/di regalloc, polarity) depends
on the EXACT ordered decl sequence before each function — not just count.
Pads = `extern int _pdN;` / `_pqN;` lines placed right before a function
def. Bands (matching pad counts) repeat ~17-22; uniform-top pads and
distributed pads produce DIFFERENT states at the same cumulative count
(insertion order into hash chains matters).

- tools/padmod2.py MOD — iterative positional solver: for each obj-failing
  fn top-down, sweep pad count 0..24 right before its def; one compile per
  count measures ALL module fns (objfn + normalized compare vs gold R-block
  extent). Applies count maximizing module matches. ~90s/compile.
- obj-match ≠ mzdiff-match: wrong anchors and data/call-target issues mean
  obj-win can still mzdiff-DIFF. Harvest needs relink + verify_build.
- TMPD env var selects E:\tmp subdir so lanes can run parallel (concurrent
  CL.EXE in same TMP deadlocks!).
- _Wall proved positional: baseline MATCH, uniform-top-21 MATCH,
  positional-21 DIFF. Baseline matchers broken by upstream pads need their
  own positional pads.

Pending work queues:
- lane queue scripts /tmp/q_lane{1..4}.sh run modules serially.
- MPSLIBM_TEXT: gold has ~45 real routines (4140:xxxx) vs stub lib — needs
  real implementation reconstruction (big separate task).
- _TEXT RTL (setargv/vsprintf/amsg_exit/aFNaldiv): custom lib or mis-anchored;
  C700/SOURCE/STARTUP has runtime source.
- PASM_TEXT asm fns: byte-level asm review needed.
- _DrawASurf (422 insn, no gold routine matches), _ShowAIStatus (0.26 jac):
  source-level reconstruction differs fundamentally.

## Session state (compact): 603/760 verified committed (69e9411)

- aligned_anchors.tsv = order-aligned gold anchors for DIFF fns only
  (DP seq-alignment of dev pubsym order vs gold segment routine order).
  objsweep.py/padpos.py/padmod2.py all read it and override classified
  anchors. Rerun padmod2 on a module uses corrected anchors.
- ~10 fns have IDENTICAL normalized streams; only address immediates
  differ -> resolve for free once DGROUP+segment layout converges.
- GetWeaponCount: gold twin located at 08de:00c6 (mis-paired before).
  Leaf-pick (elem vs param in ax) resists pads 0-24, inner externs,
  aliases, operand flips, pointer-loop. Only probe-TU emits gold form.
  Parked: needs deeper symtable-state lever or alternate source shape.
- SetUpWingman: NPLANESx/LASTPLANEx inner externs fixed 2 of 3 leaf
  compares (jnl/cmp[bp],ax); residual regalloc diff.
- lanes: padmod2 sweeps continue on PLANES3,N3D,SAMS,MESSAGE,MAP,HSD
  with corrected anchors; /tmp/pm2b_*.log; queues in /tmp/q_lane5,6.sh.
- Orphaned kvikdos/C2.EXE procs killed (old t5 deadlock leftovers).

## Session state: ~606/760 (46c958f, df61d19)

Verified/committed this session:
- TakingOff moved PLANES2.C->PLANES.C (retail seg 21ce). Function
  symbols perturb symtable for all fns below insertion; downstream
  pads must be re-swept after any move. di2=0 hoisted to fn top
  (retail emits sub di,di before Gtime check; still needs di bind).
- MESSAGE.C: Messages() merged from MyMessages + ejected/MSGDETAIL/
  MsgEMS guards; Message() mprintfs removed (_Message MATCH at
  1fc6:0009); _LoadMsgEMS MATCH at 1fc6:00ef; MsgBox uses DrawLine
  (0bdc) not RectFill (07cc) for borders — 1 operand-order diff left.
- DrawSweepDisplay: static timer throttle + RectFill bars — codegen
  identical; residual diff = MPS-lib stub call target (layout class).
- WingmanAdd: WING_PTR[WING_CNT++]=*t -> MATCH at 08de:3404.
- fourpoints: 4 pads before def -> byte-identical (si=y,di=x bind).
- Gold inlines View==PADLOCK/PILOT checks inside big MyMessages;
  no standalone Messages wrapper exists in retail.

In-flight: padmod2 lanes PLANES, PLANES2, PLANES3, MESSAGE, HUD2
(resweeping TakingOff-move regressions: _Wall, _CheckTailAspect,
_CoreCheckGunFiring, _GiveSanitizeMessage, _GetTargADP, _LGetTargADP).

## Late-session notes
- __amsg_exit (RTL): instruction-identical, only DGROUP/call offsets
  differ -> resolves at data-layout convergence, same as ~10 others.
- _FLIGHT: first ~50 insns match; diverges mid-body where mine has a
  guard structure gold lacks (if x==0 skip vs direct imul).
- _MsgBox: 1 residual = operand-order in Len+StartX (regalloc pick);
  invariant to pads/locals/param-order probes. Parked.
- Module diff census (snap6): PLANES15 CMDS14 PLANES2 12 PLANES3 10
  COCKPIT10 BOATS9 GMAIN7 3DPLANES7 TEWS5 + long tail.
- Lanes running: padmod2 on PLANES,PLANES2,PLANES3,COCKPIT,GMAIN,
  3DPLANES,TEWS,AWG9 + serial queues /tmp/pm_queue{1,2}.log
  (CARR,3DLOADER,MAP,CARRAI,BULLETS,AAAHIT,3DSCENE,NPOLYS,RIOMAP,
  FLIGHT2 | NQFLY,LOADMISS,TAC,WORLD,SPRITES,MONOPRIN,RADARS,SAMS,
  N3D,TID). Finished-clean: MESSAGE,HUD2,CMDS,BOATS (no new pads).
- PLANES2 lane applied: _SetUpWingman p=10, _CheckWingmanSix p=5.
- DGROUP: gold i@0x619a vs mine 0xf45c -> my DGROUP much smaller;
  gstr/global ordering needs retail-order reconstruction in GDSEG.

## 614/761
- Inner-extern global alias = THE leaf-order lever for param-vs-global:
  declaring `extern UWORD Altx;` INSIDE the fn binds the global AFTER
  params -> deltalt evaluated first = gold order (LGetTargADP ndiff 0).
- objfn.py unreliable on big objs (pub/extent) - use linked exe + map.
- _DoHover: aln anchor 2ad2:40e4 wrong - my 9-insn stub needs byte-search.
- pitchnum/MsgBox: param>local and param>param leaf ranking immovable
  so far (source order, pads, decl order all inert).
- 13 fns codegen-identical modulo relocs (DGROUP/seg-layout convergence
  will auto-fix): CheckAAAHit DetermineShootParams DoOClockCall
  DoPlayerOnTheCat DrawSweepDisplay FriendlyChooseBehavior NDDot NDLine
  NPFLine Perspect3D ShowOurWPS TrackDisplayPosition UpdateOurPlaneAnims.

## Turn: 615 -> 620 verified (snap7 race-free rebuild)

Race-free snapshot link (all lanes frozen into /tmp/snap7) verified 620/761.
New MATCHES this turn: _show_plane, _show_jamming (TEWS retail recon),
_AICoreRadarCheck, _AISanitizeCheck, _AWG9_project + lane pads.

TEWS reconstructions (committed cf3a569): show_plane x=(len-1)/2+2 CRT_Y-3;
show_jamming else-branch prints gstr758; show_radar x=(len-1)/2+1 CRT_Y-3.
horiz_line: retail draws ONE DrawLine pts[0]->pts[1] (second call removed).
Residual _show_radar/_horiz_line diffs: param/local leaf-order + relocs.

Open leaf-order classes (source-order-invariant in MS C6):
- param > local wins ax (pitchnum ox+x-1, MsgBox arg) — parked
- param > param symtable ordering (MsgBox Len+StartX) — parked
- _LGetTargADP global-first leaf rank despite inner-extern — parked
- _CheckTailAspect: gold reads distinct retail global 0xae26 (not PyLng_lo)
- _MapLoc: thunk-call vs far-call structure — needs real reconstruction

## Stream-identical DIFF census (reloc/DGROUP-only residual)

These emit byte-shape-identical instruction streams; verify DIFFs come
only from relocated operands (DGROUP offsets, far-call segment targets).
They auto-converge when data/segment layout converges:

  _NDDot _Perspect3D _DetermineShootParams _NDLine _NPFLine
  _CheckAAAHit _DrawSweepDisplay _ShowOurWPS _DoOClockCall
  _DoPlayerOnTheCat _UpdateOurPlaneAnims _FriendlyChooseBehavior
  _ChangeGroupBehavior (fixed: zero-init stores as statements)
  _show_radar _pitchnum (+12 previously found = ~27 total)

_Closest real diffs: _GetRadarGTNum nd1 (cmp dir), _GiveSanitizeMessage
nd3 (global-vs-local cmp leaf), _AwacsPicture nd5 (expr difference),
_AvoidMountains/_FormationSpeedAdjustBoat/_MakeBoxes/_EasyLanding nd6-16
(block-placement + si/di + regalloc classes)._

## Anchor-scan wins (622/761)

Byte-pattern twin-scan over all DIFFs found correct anchors:
MATCH: _farfread @40af:0004, _GetInterceptPlane @3037:0001.
Closer: _MovePlane (nd 40->9), _VectorIntercept (nd 291->2),
_TIDxy/_TrackDisplayPosition (stream-identical, reloc residual).
Note: gold contains duplicated function bodies in multiple segments;
pattern-scan finds the closest twin but relocs still gate MATCH.

## Leaf-order census (all levers exhausted, parked)

cmp-dir (GetRadarGTNum, VectorIntercept, GiveSanitizeMessage),
param-vs-global (CheckTailAspect si=PyLng+y, GetTargADP, LGetTargADP),
param-vs-param (MsgBox Len+StartX), eval-order (AwacsPicture).
Inner-extern works ONLY for specific contexts (Altx succeeded once);
CheckTailAspect inner-extern PyLng: no effect. Source compare-flips,
decl reorders, regroupings: all emit-identical. Needs real leaf-rank
mechanism (probably symtable slot ordering or a different /O flag).

## DGROUP layout (blocks the ~27 stream-identical fns)

NG*32 vars: gold DGROUP+0x010a (early BEGDATA), mine +0xde86 (end of
_DATA after 56KB GDSEG). Retail's PASM data was BEGDATA-class or in
GDSEG. dseg_pairs.tsv has 7591 gold<->mine offset pairs; layout is
block-scrambled (+0x300/-0x5126 runs), not a uniform shift.

## DGROUP convergence — model solved, PASM block verified (623/761)

Retail DGROUP ordering decoded from gold image bytes + code refs:

    [NULL 0-0x41][ _DATA 0x42-0x649 ][ GDSEG-region 0x64a+ ][ _BSS ]

_DATA run contents (gold):
  0x42-0x109  RTL crt0dat + early C vars (banner is NULL-seg; 'r+b'@0x82,
              far-ptrs@0x70, mode dims@0x8e, 'rb'@0x106, CLIPZ dd@0x46=0x20000)
  0x10a-0x283 PASM .DATA block: NGXLO32@0x10a, NGYLO32@0x10e, NGXHI32@0x112,
              NGYHI32@0x116, NGXMID dw@0x11a, NGYMID dw@0x11c, FILLYLO@0x11e,
              FILLYHI@0x122, identity-mat@0x126, zoomfactor@0x138, NOASPECT@0x13a,
              CNT@0x13c, DELZ@0x140, TOPTOBOT@0x144, gap vars@0x146-0x153,
              NPX..NPYTADD dd's @0x154-0x16b, VECT@0x16c, LVECT@0x172,
              OFlags@0x17e, MWID@0x180, TransTable@0x182, NTRANSGREYPTRS@0x282
  0x284-0x2bf EMM.ASM .DATA: dw 0184h@0x284, emminstalled@0x286,
              emmname 'EMMXXXX0'@0x288, emmpagemap@0x290, emmmpbuf@0x298
  0x2be-0x649 gstr0-49 + interleaved small vars (dw 5@0x2bc, dw -1@0x2be,
              'rb'@0x2e9 between gstr0 and gstr1, word vars 3,3,2,52,44,24@0x330)

GDSEG-region (gold 0x64a+): gstr50 'SARATOW.3DZ'@0x64a onward + game vars
  (_TRGTB@0x532c confirmed via dseg_order; _ps@0x7e64; NG 16-bit word vars
  _NGXHI/_NGXLO/_NGXMID/_NGYHI/_NGYLO/_NGYMID at ORG'd 0x7b62-0x7b6c —
  SEPARATE uninit vars, NOT aliases of the dd's).

Mechanics:
- GDSEG.ASM class BEGDATA->BSS makes LINK order the big segment after the
  _DATA run (class-order, not obj-order). Bytes still emit.
- PASM .DATA vars split by scope: 16-bit NG words -> EXTRN (GDSEG labels
  own _NGXMID etc for C code at 0x7b62 set); NGXMID/NGYMID dw's in the
  PASM block are LOCAL (asm refs bind 0x11a/0x11c; gold Perspect3D reads
  [0x11a]/[0x1 1c]).
- CLIPZ defined in PASM .DATA @0x46 (dd 020000h); GDSEG's sequential
  _CLIPZ label removed. _GRTBLSEG DGROUP label removed (gold's is the
  far _GRTBL seg @50ea:0000).

Verified byte-identical after rebuild: _NDDot 0/92, _NDLine 0/326,
_Perspect3D 0/146. verify_build snapshot: 623 MATCH / 761 pairings.

TODO to finish DGROUP:
1. _DATA must end at 0x649 (mine 0x12BA): move mid-run content out —
   TrgTbl (TRIG.ASM; gold has _TRGTB@0x532c in GDSEG region), MCGA G-vars,
   a0-fill table, _DEFAULTPAL (EXTRN'd in GDSEG already), C literals, RTL tail.
2. gstr0-49 need a 'DATA' segment emission ordered after EMM (#8): either
   new 'gdata' obj between emm/nqfly in OBJS, or appended inside EMM.ASM
   .DATA tail. Interleave per gold order (rb@0x2e9, word vars @0x330-0x33d).
3. Rebase GDSEG ORGs by -0x64a once GDSEG base lands at 0x64a; sort the
   sequential head-block (1183 labels incl ~770 gstrs) into gold order:
   542 content-matched in gold image, +174 via dseg_order.tsv; remaining
   ~467 mostly small strings/uninit vars need neighbor-inference.
4. Fix wrong string CONTENTS: gold F14L09C/F14R09C/F14SMLO3/F14SMC02 vs
   mine F14L09B/F14R09B/F14SMLO2/F14SMC01 (version-suffix diffs).
5. _RioLeftShould@0x150 sits inside PASM block gap vars (0x146-0x153).

## GDSEG full migration — DONE (39eca83), 623/761 held

GDSEG.ASM is now a byte-image replica of gold's DGROUP: BEGDATA at
DGROUP+0, 1206 labels at verified gold offsets, emitted bytes = gold
image (extent 0x642e), uninit tail = ORG'd labels only. PASM.ASM and
EMM.ASM .DATA sections removed — all vars EXTRN to GDSEG labels.

Verify invocation (complete arg set — aligned_anchors MUST be first):
  python3 tools/verify_build.py GOLD analysis/gold/gold.map \
    build/obj/F14.EXE build/f14_named.map \
    analysis/gold/aligned_anchors.tsv analysis/gold/classified.tsv \
    analysis/gold/matches.txt
  -> 623 MATCH / 761 pairings

Pitfall learned: unplaced vars MUST NOT alias one address — mzdiff
maps gold-off->my-off per ref; N gold offs -> same my-off = collision
DIFF. The 558 unresolved vars now get distinct synthetic offsets at
0xde40+ (past _BEND, no emitted bytes).

Naming collisions decoded (gold has BOTH a C-visible var and a same-
named PASM-block var): NGXMID->NGXMIDL@0x11a, NGYMID->NGYMIDL@0x11c
(C vars _NGXMID@0x7b66 etc. stay separate). PASM code refs renamed.

Relocatable data defs preserved: _PalPtr3D DW _defaultpal @0x2b3a,
_Seg3D DW SEG _Seg3D @0x2d3c (DW-offset relocs survive as baked image
bytes; SEG relocs need real defs for runtime correctness).

Still TODO for DGROUP:
- 558 vars at synthetic offsets (unresolved gold position) — biggest
  residual source. Resolve via gold code-operand analysis: disasm each
  gold fn's data refs, map my fn's corresponding operands by position.
- ~30 lib data syms (crt0dat family) are GDSEG labels -> L2025 dups
  (harmless, first-def wins) but change lib module pull — watch _TEXT
  ordering diffs if lib code offsets shift.
- wrong string contents in image where my data differed (now emits
  gold bytes verbatim — resolved).

## Update: DGROUP harvest + near-fn reconstruction (verify 622/761)

- +23 unplaced GDSEG vars resolved to real gold offsets (dseg_pairs + mzdiff); 538 remain at synthetic offsets.
- `_X2map`/`_Y2map` made `near` (f14.pat signatures show `ret c3`, `[bp+4]` params); mplot/MapLoc now emit near calls. Net: +`_ChangeGroupBehavior` MATCH; −2 cls:bestaddr pairings (anchor drift from MAP_TEXT shrink, not code regressions).
- Gold has NO standalone X2map/Y2map bodies with the `1+Mscale` idiom — three `mov cl,7;sub cl,[var]` sites exist inside bigger functions (inlined x/y shift pairs). The `7-var` there is likely a different var (0x908e) — needs var-identification.
- `src/f14.pat` (790 patterns) + `f14my.pat` are FLIRT-style signatures with named symbols and `^off _Sym` reloc refs — per-function shape oracle for reconstruction.
- Leaf-order class still parked: `_horiz_line` source-form flip emitted identical bytes (verified).

## Update: DGROUP gap closure + enregistered-var reconstruction (verify ~640/760)

- `_CheckProximityHit` MATCH: embedded `"Aircraft "/"We're "/"Hit by "`
  labels inside gstr722 + TXT() calls; local `tempmissile` far ptr for
  DamageAI; `(UBYTE)status & CRASH` byte test; single LogIt in AirKill arm.
- `_All2AirHoming` MATCH: `_Gtime`@0x669a, `_lastCounter`@0xca18,
  `_GtimeDAC`@0xca1a (were wrongly colocated); file-scope extern hoisted
  before includes flips compare operand order (leaf-insertion lever).
- `_ntmp` placed at DGROUP 0xc0de (was ZFAR 0) — fixes all TXN pushes.
- Missing 20-byte record after `_InXPLO`/`f14.pic` (01 00 00 00 26 00
  2d 00 15 00 11 00 00 00 73 00 2c 00): fixes iplanes@0x619a and all
  refs in 0x5610-0x6200 (was third +0x12 region after _O and XPLO).
- PLANES3 `static int i` = gold var at 0x61ac: now `extern int i3` +
  `#define i i3`. Collateral MATCHes: _DoOClockCall, _FollowPlayer,
  _GetLastPosition, _GorillaCover.
- `_ailoc` moved to 0xbe4a (22B BSS gap after _WORLDXTRAZ).
- `_MovePlane` MATCH: missing tail `ps[].alt=(GetQHeight(xL,yL)>>2)+4`;
  once the tail existed, planenum->si enregistration came for free.
- `_AICoreRadarCheck` MATCH: `COORD tdist` split to its own decl AFTER
  tdist1 — last-declared wins ax (same F117 leaf-insertion rule).
- `_GetRadarGTNum` 1 real diff parked: `cmp ax,[f]` vs `cmp [f],ax`
  opcode-direction — `==`-form emits 0x39, `!=`-form pins 0x3B in mine;
  `!(a==b)`, `&&`-fusion, operand-swap all inert.
- `_TakingOff` di2 = real tri-state flag (0/1/-1) enregistered in di
  for the squadron list walk + zero-operand compares. My source has the
  walk but MSC constant-folds di2->0 (imm compares) and won't enregister
  (only 3 visible uses); needs the remaining di2 uses reconstructed.
- `register` keyword is fully ignored by this MSC 6.00 build.
- si/di-vs-bx temp selection and param-vs-local leaf order remain the
  dominant parked classes (~15 fns).
- verify runs MUST be serialized with rebuilds (see prior races).

## Session: missing-block reconstructions (642 -> 644+)

- `_UpdateLights` MATCH: missing unconditional tail — MouseHIDE +
  MapInEMSSprite + TransRectCopy (col 111 vs 134 by JetisonSW) +
  RectCopy(88,102,20x20) + MouseShow. Same sprite imagery as
  JetisonSwitch() but no View gate / no state update. Restored.
- `_LetNextPlaneLand` MATCH: missing early-exit guard —
  `if(whichourcarr != -999 && bv[whichourcarr].verticlesep != 0 &&
  bv[whichourcarr].varcnt > Gtime) return;` (carrier landing-slot
  timeout live). bv field offsets: varcnt@0x18, verticlesep@0x1c,
  threathead@0x28 (stride 0x30, far Behavior fnptr@0).
- `_NewLockView` MATCH (earlier): gold's dual padlock-cursor search —
  plane scan gated by SamLockee==-1 (no PadLockee wrap), plus a
  ss[NSAM](28x44) SAM-padlock fallback via an int* target-field cursor;
  new adjacent global SamLockee(0x5536). Loop exits use i>=N/j>=NSAM.
- `_HsdCrt` +2 fix: Knots label is (Length1/2)-2 (dec ax;dec ax), not
  +2 — label was 4px low. Label block still diffs deeper (gold uses a
  sprite-scale-gated path, enter 0x62 vs 0x3a — different local set).
- `_MoveAIToElevator` resolved to MATCH (earlier frame work).
- `_GetWhoCover` partial: `int gnum=bv[planenum].groundt` hoist killed
  an in-place `imul word[groundt],0xe` that corrupted groundt before
  the second aiwaypoints[g].y read. Still DIFF: gold spills
  ps[cnt].status to [bp-0xa] (embedded-assign form) + caches ps/bv
  base ptrs in [bp-0x2]/[bp-0x4].
- `_EasyLanding` parked: landpt decrement logic is semantically
  identical, but gold places the shared `dec` BEFORE the else-block
  (jnl->else); MSC6 won't reorder (goto/!/&& forms all inert).
- `_AICarrierLand` big divergence: gold enter 0x62 vs mine 0x3a —
  MSC6 dead-eliminated ~20 local words; body needs fuller rewrite.
- Confirmed the "missing-block" pattern is the highest-yield class:
  functions ending early (retf) while gold continues.

## Session: BACKTOBASE gates + operand-order reconstructions -> 645/761

New MATCH:
- `_Jink2`: gold's early-exit is `CheckDetection(i)==-999 &&
  !(ps[i].status & BACKTOBASE)` — a returning-to-base plane skips the
  StraightAttack handoff and falls through to enemynum processing.
  Added the missing conjunct -> byte-exact MATCH.

Real-condition fix (still DIFF deeper):
- `_Jink`: outer maneuver gate is `tdist<105 && !(ps[i].status &
  BACKTOBASE)` — same returning-plane idiom folded into the outer if.
  Remaining diff is a pinned winglevel CSE spill ([bp-0x1a] temp, not a
  named local).

Recurring finding — `ps[i].status & BACKTOBASE` (bit 0x4) is a real
missing gate in the combat AI routines; several funcs test it via
`test byte[bx+0x7e7c],0x4` where my source ran the block unconditionally.
`_UpdateSpeedAndPosition` has another at behaveflag&ENEMYNOFIRE scope
(needs full reconstruction — gold does `si=func(i); if(flg&2){if(st&4)si++}`).

Parked (regalloc/eval-order, not source bugs):
- `_NearWingTarget`: gold iterates ps[] via si cursor + cnt at [bp-0x2]
  (memory); mine enregisters cnt. `volatile` experiment added reload
  artifacts (1->10 diffs), reverted.
- `_GetRadarGTNum`: `cmp [field],ax` vs `cmp ax,[field]` — MSC6 puts the
  computed (gt.x>>HTOV) result in ax as cmp-dest regardless of == order.
- `_AvoidMountains`: `cmp ax,[bx+6]` vs `cmp[bx+6],ax` — heightval lives
  in si (strength-reduced), cmp operand order is downstream of that
  regalloc; > vs < spelling is canonicalized inert.
- `_ShowAIStatus`, `_FlyWayPoint`: gold clears 2-3 locals via shared
  `sub ax,ax`; mine enregisters one + extra `push di`. Local-enregister
  class, pinned.
- `_GDisplay`: gold computes `si=([0xae94]>=1?0x70:0)` via cmc/sbb/and
  bool idiom before struct stores — a mode-dependent offset my source
  lacks; needs field-level reconstruction.

## Session: size-gap scan + DGROUP symbol map (645/761)

New tool: `analysis/gold/dseg_order.tsv` maps GOLD DGROUP offsets -> names
(gold_off, our_off, name, delta, conf). Resolves e.g. 0x2b00=_ABOVECLOUDS,
0x1e9e=_GRPAGE0, 0x61a4=_psi, 0xd656=_pdest, 0x619a=_i (labelled
`_friendfire`, low conf — it is really the global index `i`; `imul
word[0x619a]` = i*stride). Use this to decode `mov ax,[goff]` / `cmp
[goff]` operand references when reconstructing.

Routine-size scan (gold size - my size) flags real missing-code gaps.
Largest genuine (excluding tiny mis-paired stubs):
_DisplayRioHead +414, _UpdateHelicopterHPR_SandP +258, _DrawASurf +209,
_Messages +182 (mostly DGROUP-offset collision, not code), _JoyRecal +144,
_LocationPicture +142, _GetInterceptPlane +122, _LoadScottMission +113,
_TID_draw_target +110, _UpdateSpeedAndPosition +106, _NHorz +82,
_SteerAndMoveMissile +74, _NDraw3D +66.

_NDraw3D analysis (3DSCENE.C): gold prologue does `si=di=0; if([0xd5e]!=0)
{si=ABOVECLOUDS; di=[0xd654]}` before Bgn3D; the [0xd5e] flag also selects
between two renderer far-calls (0x3a9a:0x9d9 vs 0xa32) mid-body. Land block
matches: `if(!ABOVECLOUDS){ND.seg=GRPAGE0[1](0x1ea0); NE.seg=[0x76];
DrawNLand; Carrier3D; Boats3D}`. si/di are register vars consumed between
Bgn3D and the land test; [0xd5e]/[0xd654] are unnamed in the dseg map —
blocked pending a data-symbol resolution.

_TakingOff (PLANES.C:724): already reconstructed — `di2` register-zero
(`sub di,di`, reused as `cmp[..],di` operand) + `if(Gtime<5)return 0` +
`ps[i].base<di2 && bv[i].threathead<di2` all present. Residual is
local-vs-register init layout, pinned.

_AIBombTarget (PLANES.C:5045): gold caches `&bv[i]` (imul bx,[i],0x30 ->
[bp-0x1a]) before `&bv[wingman]` (imul bx,si,0x30 -> [bp-0x1c]); mine
reverses. Semantics identical — eval-order, pinned.

_SteerAndMoveMissile (SAMS.C:460): gold clamps the third speed-bump
branch to a minimum of 1: `mov ax,50; cwd; idiv [FrameRate]; mov bx,ax;
or bx,bx; jnz L; mov bx,1; L: add [ssi+0x12],bx` => `ssi->speed += (50/
FrameRate ? 50/FrameRate : 1)` (or a min-1 div macro). Real semantic
guard mine lacks. HOWEVER: writing it as a `?:` operand reshapes the
else-if chain so an EARLIER divergence surfaces (`jmp short` vs inline
`cwd;idiv` at +0x73b) — the chain's arm layout itself also differs.
Needs full chain-layout reconstruction, not just the clamp. DEFERRED.

== ANCHOR MIS-PAIRING (not source bugs) ==
Several DIFFs were wrong retail twins, not source errors. Fixed via:
- size-collision scan: two funcs claiming same gold addr (awk uniq -d on
  aligned_anchors.tsv col3) -> _Boats3D/_X2map@072e:00d1, _CheckMouseInput/
  _GetJoystickInputs@1081:008c.
- classified.tsv 'bestaddr' (byte-similarity) often holds the TRUE twin when
  alignorder's sequence-DP mis-paired (fn-count drift). For _AddAcqList,
  bestaddr=08de:28aa was the real insertion-loop twin; old anchor 08de:13c9
  is actually _LockDistanceCheck (a Message() predicate).
- _CheckMouseInput->1081:01f0 => byte-exact MATCH (+1). _AddAcqList->28aa
  => correct twin, residual is t-in-bx-vs-di regalloc only.

REMAINING BLOCKER CLASSES (verified, all steer-proof or systemic):
1. PINNED codegen: si/di/bx regalloc, cmp-operand-order, branch-polarity,
   dead-store, shared-zero 'sub ax,ax' frames. register-kw does NOT steer.
2. DGROUP data-layout: far-ptr segment-selector globals at wrong near
   offsets (ChangeHUDFilter reads [0x5fce] gold vs [0xf35c] ours). dseg_pairs
   shows ~916 divergent offsets - wholesale data reorder, large task.
3. Near-fn anchors: _X2map/_Y2map/_mplot/_MapLoc (MAP_TEXT). Gold has NO
   'sar ax,cl' map-transform near-fn - X2map likely inlined into callers or
   differently formulated. True twins not yet found.
4. _UpdatePalBuf anchor 2060:005e is WRONG (gold is a 2734B 'enter 0x30c'
   packet/filename handler, not the 75B palette copy). Real twin unfound.

## Turn: NQFLY DoBox/GetQHeight/IsLand — real source diffs found + fixed

- _IsLand DIFF->MATCH: `int XB,ZB` -> `unsigned int XB,ZB` reproduces
  gold's byte-pack (`mov bl,[slot]; mov bh,al`) AND the extra `enter 0xa`
  frame (dead XOFF/ZOFF locals retained, matching gold's slot count).
  Gold evaluates X-div first, Z-div second, reads `[es:bx+si]`, `cbw`.
- _GetQHeight: 132-insn stream is instruction-identical to gold after
  removing `MikeHeight=H1` (no global store in gold; freed ax for H2-4
  reads, H1 shifts in di: `mov di,ax; shl di,4`), direct `<<4` per-read
  expressions, and early-return tail `if(H2/2>CLOUDHEIGHT)return(CH*2)`.
  Sole residual: `cmp si,ax;jge` vs `cmp ax,si;jle` — same pinned
  operand-order class (steer-proof: decl order, register kw, pad locals,
  in-cond assign, ternary, expr operands all inert; module-state bound).
- _DoBox REAL source differences reconstructed from gold (previously
  catalogued as "es save/reload codegen"):
  * `if (B->VIS2&0x80) B->VIS1|=0x80` — was `VIS1`->`VIS2` (fields
    swapped vs old source; gold tests +6=VIS2, sets +7=VIS1).
  * field-copy blocks emit VIS2(+6) before VIS1(+7) => source writes
    `TBXBS->VIS2=` before `TBXBS->VIS1=` in both copy sites.
  * quadrant blocks use the OPPOSITE guard: `if(B->VIS2)` gates the
    TYPE1/CLR1/PTR+1 body; `if(B->VIS1)` gates TYPE2/CLR2/PTR+8 body.
    Chained assigns are `VIS2=VIS1=` (VIS1 stored first).
  * all 4 else-branches guard the BITID copy: `if(LEV!=2)
    TBXBS->BITID=B->BITID;` (gold has 8 `cmp LEV,2` sites vs our 4;
    when LEV==2 the earlier `*(PTR)` BITID value survives).
  Residual: 1 insn — `VIS1|VIS2` head emits +7-load-first in gold,
  +6-first here (offset-ascending leaf order; spelling/`!`/cast/`+0`
  all inert). Plus MAXLEV global at different DGROUP offset (data).
- _DrawLandObjects MATCH->DIFF after module perturbation: `(Y+yt)`
  emits param-first in gold, local-first here — same pinned
  leaf-eval-order class (2 sites, `/-scl` and `*scl` arms).
- _MakeBoxes residual confirmed same class: `*(PTR+256*Zbyte+Xbyte)`
  index emits ZBASE+i first in gold, XBASE+j first here (the -8 on Z
  folds into `[es:bx-0x800]` disp in BOTH). Positional/subscript/
  double-mask/explicit-disp respells all inert.
- PATTERN across all 4 NQFLY residuals: gold evaluates canonical-LEFT
  operand first, this build evaluates canonical-RIGHT first. Suspect
  systemic leaf-eval-order difference (compiler version or module
  state) — every respell inert because tree canonicalization precedes
  leaf pick. NQFLY now 6/10 MATCH; 4 DIFFs = 1-2 insns each, all this
  class, bodies otherwise byte-identical.

## Turn: post-NQFLY sweep — sampled diffs all resolve to parked classes

- _ReArmWingman (PLANES3): residual is commuted `NPLANESx > cnt` loop
  test (gold `cmp [NPLANESx],cnt;jg`, ours `cmp cnt,ax;jl`) — operand-
  order class; source already respelled backwards, still commutes.
- _EndStack3D (NHORZ): REAL addressing diff, but blocker class 2 —
  gold reads STACK3D[i].field via `mov es,[0x5fca]; es:[bx+off]` (far
  ptr/array) while ours uses near `ds:[bx+off]` (_STACK3D @DGROUP
  0xa142). Retail STACK3D sat behind a far pointer/far segment; fix
  requires the DGROUP data-layout task, not a source tweak.
- _CheckForMobileR5: dead-store class (`mov [bp-6],ax` where
  otherplane's slot is never read back); temp-free/assign-in-cond/
  for(;;) respells all perturb or dead-store identically. Parked.
- _GetWhoCover: gold caches `ps[cnt].status` into a reused slot
  ([bp-0xa]) then tests ALIVE/ACTIVE from the copy; every temp-spelling
  (tdist/gnum assign-in-cond, nested if) perturbs prologue regalloc.
  Reverted to baseline; parked pending a lever that shifts only the
  test site.
- _AIBombTarget: `bv[i]==bv[wingman]` eval-order (i*0x30 first gold,
  wingman first ours) — pinned class, `==`-swap inert.
- _ShowOurWPS, _CheckPlaneLand, _CheckTurnStuff, _DoLandWait,
  _DoAltHover, _DoGroupAction, _GetRadarGTNum: all previously
  catalogued pinned regalloc/operand-order instances.
- _AIGoHome/_AIGoHome2 (PLANES2): prologue slot-assignment drift
  (gold enter 0xa + di/si pushes; ours wider frame) — local residency
  regalloc, no evident source difference.
- padmod2 sweep complete (CARR..3DSCENE lanes): _Splatted best 9/12
  pad n, no further flips. Standing census /tmp/census.txt = 154 DIFF
  names, dominated by the three parked classes above.

## Turn: F19-trick application + _GetWhoCover near-solve + flag analysis

- F19 DECOMPILATION.md levers tried on pinned diffs: subtraction-compare,
  register param/local, volatile, name-hash, nested blocks, casts, /Oa.
- `_GetWhoCover` reconstructed: nested `if` + `stat` temp (renamed from
  dead-store `isfriend`) + inlined `bv[planenum].groundt` now emit gold's
  `mov ax;mov [slot];test al;test [slot]` status-cache + correct else-if
  topology. Residual: gold spills `groundt` to `[bp-2]` (named-local
  `mov ax,load;mov slot,ax;imul bx,ax` pattern) — every named-gnum spelling
  steals `di` from `planenum` (MSC6 prefers dense early-use locals); CSE
  form emits `imul mem` instead of the spill. ~15 variants; pinned.
- `/Oe` investigation: gold calls `_aFlshr` helper 175×, never inlines
  `>>5`. Initial probe suggested dropping /Oe fixed it, but module sweeps
  showed identical match counts under /Osegcl vs /Osgcl — the earlier
  "fix" was stale-binary state, not flags. PLANES2 override reverted.
- tools/relink_check.sh now honors per-module MODULE_FLAGS (was hardcoded
  /Osegcl, silently ignoring build_all.sh overrides).
- PLANES3 unchanged-count; no sibling regressions.

## Turn: TID reconstruction + mission-data call sites + final sweep triage

- TID.C fully reconstructed from gold: new static helper `TargInPhxRange`
  (enter 0x32, SetupNewTarget+orient_to_player+`(t.conv)?t.d:t.d>>HTOV` then
  `BIT_2_NM(rng*10) <= ORDNANCE[W_AIM_54].rmax` — matched by writing
  `if (cond>max) return 0; return 1;` to get gold's jl-first block order).
  `_DoTID` gained `if (DisplayFrame==0) InRangeFlash^=1` toggle, the missing
  4-waypoint marker loop (`for(i=0,wp=wps;wp<wps+NWAYPOINTS;wp++,i++)`,
  carrier glyph for wps[0], caret markers wps[1-3], color 89/85 gated by
  NextWayPoint), `TIDTargetCnt=0` moved after the loop, final
  `draw_circle(...,84,0)` with no trailing return. `_TID_draw_target` gained
  `int drawtail=1`, `drawtail=InRangeFlash` when `TargInPhxRange(tc->i)`,
  tail switch gated by `if(drawtail)`, and the PDSTT caret pair
  `DrawLine(tempx-2,tempy,tempx,tempy+2);DrawLine(tempx,tempy+2,tempx+2,tempy)`.
  All three functions now hdiff `hunks: 0` vs gold (DoTID@0x3d7e4,
  helper@0x3e014, TID_draw_target@0x3e072).
- PLANES.C mission-data block recovered: gold's `alt<0` crash arm in both
  `UpdateSpeedAndPosition` and `UpdateHelicopterHPR_SandP` calls
  `IndirectWPCorrection(i)` + `LogIt(...)` + `UpdateMissionData(1/2, id)`
  — the USP site is `bv[i].leader==-1 ? ourplanetype : ptype->PlaneEnumId`
  conditional, the heli site unconditional. Both now match modulo
  cmp-canonicalization. Same block added to PLSTRIP.C twin (unlinked).
- `_UpdateOurWPS` (PLANES3): void->int; new `nWPs` local snapshots NextNum
  per filled wps[i]; `if(nWPs>3)nWPs=3; return nWPs;` tail + bare `return;`
  in the `numwps==0` arm (matches gold's garbage-ax early return).
  RIOMAP.C:217 caller + PROTO.H:171 confirmed the int return.
- `_firemissile2` (SAMS): added gold's helicopter-aggro preamble
  `if(bv[gtarg].behaveflag&ISHELICOPTER){behaveflag&=~ISHELICOPTER;
  ChangeGroupBehavior(gtarg,0);}` before CheckAIBombs — now clean.
- `_SteerAndMoveMissile` (SAMS): `idiv`-site shared tail verified; MAXPLANE
  smoke branch uses `ASinB(-50)`, `-ACosB(-50)`, two dead `ASinB(-25)`/
  `ACosB(-25)` calls, StartASmoke cnt 15; else-branch has
  `(sst->MissileEnumId==0x3b)?5:EXHAUST` type test; homer test is 3-case
  switch on ML_FFBOMB/ML_LGBOMB/ML_RTBOMB. Down to 1 eval-order artifact.
- `_OutSideHeading`: `if(Status&TRAINING) strcat(N," TRNG")` + gstr781.
- `_DoCRT`: removed FlirCopy(VdiMask) from VdiMode branch.
- `_TIDShowRange`: already matched — `d = MsgOnScreen ? 7 : 0` present.
- Census re-run: all located functions verified at linked level; remaining
  diffs are reloc-placeholder, eval-order, regalloc (si<->di), block-order,
  jmp-table-bytes, or next-fn-boundary artifacts. New masked-prefix locator
  placed 30 previously-NOHIT functions; newly verified clean:
  _AWG9_DEC_RANGE@0x9d0b _ChangeModerateScan@0x9a9a
  _CheckLandingAndCrash@0x3b72e _EMSinit@0x15e68 _InitSound@0x3a6f8
  _InitTWSData@0x8e4c _MissilesInAir@0x3a376 _ProcessCoolyCmds@0x11e50
  _UpdateCarrierAnim@0xedbe _show_az_carat@0x1530e _CheckImHit _DistOf
  _RioTop _restore_video_state _UpdateLights _DrawASmoke _DrawRioMapInfo
  _RadarThreats _CheckTurnStuff _ChooseBehavior _FormationBoat
  _NearWingTarget _DrawRadarRange.
- EMS.ASM wrappers: gold keeps `call far ptr EMSinit` untranslated
  (`9A` bytes) while our link's /FARCALLTRANSLATION rewrites it to
  `push cs;call near`+nop; gold also carries the classic MASM `jmp`+`nop`
  fill after `jmp Exit` in the EMSint macro. Fix needs a module split or
  unresolvable far ref — parked (asm, not C).
- `_ChangeHUDFilter`(FLIR) @0x21c4e — semantically identical after ~14
  source variants; residuals are pinned compiler-version codegen
  differences: (a) `if(View==4)goto out; if(View==5)goto out` — gold emits
  independent per-site jcc (`jnz+jmp` far / `jz` short), our /Osegcl fuses
  them into a shared-arm `||`-form; (b) gold's 3 `Palette[]` stores are a
  separate group (al reloaded per element since overwritten), ours CSEs
  the constants across statements into value-groups. Same frame (enter 4),
  same stores, same call. No source construct found reproduces gold's
  schedule — likely different MSC version for this module.
- Remaining REAL work (source-level reconstruction needed):
  * MAP.C `_X2map/_Y2map/_mplot/_MapLoc` — absent from gold's image in
    current form; likely `#ifdef YEP`-gated dead code in the retail build
    (their only callers are inside `#ifdef YEP`).
  * `_UpdatePalBuf`(FLIR) — gold has no 'M'/'0' header-magic compare;
    packet parse differs.
  * `_InitSpeech`(SOUND), `_DrawXploSprite`(SPRITES) — different codegen,
    unlocated in gold.
  * `_InitOptions`(OPTIONS) — VERIFIED exact @0x204f8 (hunks:0); earlier
    7-vs-20-insn note was a mispairing.
  * `_JoyConvert`(JOYSTX.ASM) — hand-asm `proc near`, NOT a C task (same
    class as EMS wrappers; parked).
  * `_DoAltHover`(PLANES3), `_FlyTo`(PLANES2), `_GetJoystickInputs`(CMDS),
    `_LoadScottMission`(LOADMISS),
    `_ScottFlight`(FLIGHT2), `_Cmds`(CMDS) — all located only to
    wrong/mispaired candidates; need per-function relocation via
    mid-body idiom search.
- `_GetJoystickInputs`(CMDS) @gold 0x1089c — semantically
  reconstructed; several real source fixes recovered from gold:
  (a) ThrustStick: `if(j.z>0)j.z=0; ThrustChange=abs(j.z)` — NOT
      `128-j.z` (abs = FUN_18a7_019e sat.abs call);
  (b) CoolyStick: `if(j.z>0)j.z=0; CoolyVal=(abs(j.z)+16)/32;
      if(CoolyVal && KEYPRINT==0) KEYPRINT=CoolyKeys[CoolyVal]` —
      the KEYPRINT==0 gate was missing;
  (c) Trigger data: `Trigs=0; for(i=3;i>=0;i--)
      Trigs=(Trigs<<1)|Trigger2(i)` — 4 individual Trigger2 polls
      (CODE_35+0x3c), NOT `~inp(0x201)>>4)&15`;
  (d) CH-keys dispatch `StickType==7 || StickType==6` — my header
      only defines J_xxx 0-5; retail has types 6/7 (probably newer
      CH/Thrustmaster variants);
  (e) RESIDUALS (artifact): ReadJoy2 or-chain order (source
      J|R|C|T vs our canonicalized C|T|J|R — pinned), CoolyKeys/
      CHKeys table disp16 sign (DSEG layout), trigger-loop regalloc
      (cx temp vs di accumulate), boundary tail.
- `_FlyTo`(PLANES2) @gold 0x2e1ec — semantically reconstructed;
  the real fix was DELETING the `#if 1` debug block
  (`tempvar && bv[i].Behavior==AIGoHome` + TXT/TXN/Message) — that
  block is absent from retail's FlyTo entirely (function ends at
  `return(0)`). Body now matches: GetTargADP 8-arg push order,
  TurnAIPlaneBy(i,relangle), alttemp/altdim+50/pdest chain,
  tdist<20&&!ignoredist return. RESIDUALS (artifact): gold keeps
  `alttemp` in `di` (push di+si, 4-insn epilogue worth merging →
  `mov ax,1;jmp short`-shared); mine reuses `si` (push si only,
  cheap epilogue → inlined two retf). Same pinned regalloc class
  as _ChangeHUDFilter + tdist/tangle slot-hash swap.
- `_ScottFlight`(FLIGHT2) @gold 0x17049 — semantically
  reconstructed; the real recovery: gold's rudder-slew formula is
  `((RudderTrim+RUDDERVALS)*15*D)/200` and
  `((RudderTrim+RUDDERVALS)*15*(1000-D))/800` — RudderTrim (the
  trim-pool global, extern int in GDSEG right after RUDDERVALS) is
  ADDED to pedal input; my source had `RUDDERVALS` alone. Verified
  by gold's `mov ax,[0x186c]; add ax,[0x186a]; imul ax,ax,15`.
  RESIDUALS (artifact): enter 0xfc vs 0x104 — the +8 bytes are a
  compiler long-arith SPILL temp at [bp-0xfc..0x104] in the
  `OLDSLEW+((D-OLDSLEW)*SFticks/60)*RELWND/200` chain (gold caches
  in si:di; MSC6 spills + reorders the LMUL/LDIV calls); ~9x
  `[bx+N]`/`[bx-N]` ps[]/bv[] struct-offset disp16; VVI at gold
  DSEG 0x898e vs mine 0x8992 (cell layout); `mov si,ax`/`push
  word[bp-N]` scheduling.
- `_LoadScottMission`(LOADMISS) @gold 0x1d560 — semantically
  reconstructed; aligned regions fully agree: dual
  fopen("mission.f14","rb") calls, shared Message("No Mission
  Data")+return0 error tail (gold merges via backward jz/jmp —
  retail had TWO copies of each string literal, no pooling),
  the object-read loops, pilot/RIO setup. RESIDUALS (artifact):
  enter 0x240 vs 0x242 (+1 int slot, temp/decl layout); loop-
  rotation polarity (`jnz`/`jz` + `cmp i,lim`/`cmp lim,i`
  operand-order canon); ~6x `[bx+N]`/`[bx-N]` struct offsets;
  `mov si,bx` CSE vs recompute imul; string-literal duplication.
- `_UpdatePalBuf`(FLIR) — ABSENT in retail F14GOLD: the function's
  whole body is `PalPkt[0]=='M' && PalPkt[1]=='0'` + palette copy;
  there is NO `0x4d` immediate compare (byte/word, any reg/mem form,
  near/far) anywhere in the gold image, and no `LoadPalToPal`→
  PalBuf call pair in gold's menu code. The f14.pat signature
  (push bp/sub sp,4/near ret form) came from a different build.
  Classification: dead source for this target — cannot pair.
- `_NHorz`(NHORZ) @gold 0x192e4 — semantically reconstructed; residuals
  all pinned operand-canonicalization + boundary:
  (a) `enter 0x316` matched via added `int DUMMY` — gold has one extra
      dead stack-int slot (never referenced) around [bp-0x40];
  (b) MISSING logic recovered: `else { if (SAVEBW!=BADWEATHER)
      GRY=MDiv(10,abs(M),ARADD/2); else GRY=((-M)<12?0xf0+(-M&15):0xfc); }`
      — nested `abs()` inside MDiv's arg eval (FUN_18a7_019e = sat.abs,
      callee2 = _MDiv CODE_40+0x668);
  (c) GDSEG.ASM: `_CLOUDAMNT` was aliased onto `_SKYDETAIL`'s cell —
      split into its own word (gold has separate cells);
  (d) `if (SAVEBW!=BADWEATHER)` polarity for correct MDiv/ternary layout;
  (e) RESIDUALS (pinned): `CHT<CLOUDTHICK` and `MYWID*i` operand order —
      our MSC6 canonicalizes the GLOBAL operand into ax for cmp/imul
      regardless of written order (tried both written forms); gold's
      compiler loads whichever operand is the local/LHS first. Same
      for `SAVEBW!=BADWEATHER` (== operand order). ~7 single-insn
      canonicalization artifacts + boundary tail remain.
- `_JoyRecal`(NJOYCAL) — EXACT, hunks:0 @gold 0x1cb37 (0x15c bytes).
  Retail is a hybrid: NJOYCAL skeleton (LoadPicNoPal +
  `NWaitShowJoy(...)==-1 → return`) PLUS JOYCAL-era calibration
  semantics inline. Source reconstruction vs previous source:
  (a) `CoolyStick` block added before ThrustStick:
      `InitJoy2(CoolyStick); NWaitShowJoy(gstr349+18,128);
      InitJoy2(CoolyStick);` + axis-3 neutralization stores +
      `NWaitShowJoy(gstr349+36,64)`;
  (b) ThrustStick: `InitJoy2` calls bracketing each message AND
      args swapped to (gstr350,128)=Max[3], (gstr351,64)=Min[3]
      (vals mask bit2i=min,bit2i+1=max in NWaitShowJoy);
  (c) inline neutralization between the InitJoy2 and second msg:
      `MinStick[3]=RawJoy[3]; LowRange[3]=CenterPosition[3]-MinStick[3];
      MaxStick[3]=MinStick[3]; HighRange[3]=MaxStick[3]-CenterPosition[3]`
      — gold byte-identical in both stick blocks;
  (d) hat strings live inside gstr349's data (offsets +18/+36 —
      "Center Hat Switch"/"Move Hat Switch Up", matching gold's
      contiguous string layout byte-for-byte incl. double
      "Then press Trigger" and JOY.F14/rb/wb strings).
- `_NShowJoy`(NJOYCAL) — semantic match; ~6 single-insn residuals all
  regalloc/scheduling (gold keeps `x` in si vs our stack slot; cbw/idiv
  ordering inside x/=4 and the (x*x)/i ternary; push si vs push di).
- `_InitSpeech`(SOUND) — located @gold 0x3a7d5; reconstructed to 1
  pinned eval-order hunk (AvailSysMem call vs pages division order).
  Added `<io.h>` for `long filelength` (kills spurious cwd); `flag`
  local (si-cached 0; set iff dig_type==0x22; passed as 3rd arg to
  LoadBigEMSFile); `_dos_close` before pages calc.
- `_DrawXploSprite`(SPRITES) — located @gold 0x3ae62; instruction
  stream identical, diffs are SPRTVLS DSEG offsets only.
- `_TD_box`(HUD2) — EXACT, hunks:0 @gold 0x1b69e. Real reconstruction:
  (a) `y = TB_CTRY + PDT_PROJ_Y - TB_WINY` (operand order, not
  `PDT_PROJ_Y + TB_CTRY`); (b) direct clamps — the `int l` + range-check
  `if(...)l++; if(l){...}` wrapper did not exist in retail; (c) window
  defs differed: `TB_WINL 108` (not 119) and `TB_WINW 68` (not 73),
  emitting `cmp si,0x68`/`mov si,0x68` and `cmp cx,0x3f`/`mov cx,0x3f`
  same-constant compare+clamp pairs. _steering_dot _diamondbox _break_x
  _HUD_nav also verified hunks:0.
- CARR module sweep (MSC6, module stays /Osegcl): retail compiled
  `_InitCarrier` with `#pragma optimize("el",off)` — restoring that pragma
  gives enter 0x10c, whichcarr at [bp-0x9a] + si-cached index, `i` at
  [bp-0x98], index-form terminal loops — EXACT, hunks:0 @0xdc32.
  Module-wide rescan (gold = mine+0x14ec in CODE_20): _InitCarrier
  _InitCarrierObjects _InitPlaneOnCarrier _GetCatHeading _Carrier3D
  _DrawCarrier _InBox _InBoxLng _GlideScopeCheck all hunks:0.
  _OnCarrier _DisplayBall _CarrierLanding _ils _SetPosOnCat
  _DoPlayerOnTheCat _DoPlayerOnTheHook _SetGear _SetDLC _TurnTo
  _UpdateCarrierAnim = boundary/extent artifacts only (gold fn extents
  sit ~7-9 bytes earlier mid-module). Small real residuals: _vdiils(2),
  _OnTheDeck(2), _SlopeCheck(1: [bp]=dx vs push dx), _SetHook(1),
  _EasyLanding(1), _MoveAIToCat(2), _KeepPlaneOnCarrier(1 tail),
  _DoLSO(6: incl two push [bx+si+N] vs [bx+si-N] displacement-sign
  diffs).
  * NODIS asm-module entries (PASM/MATH/_MPSLIBM): not C, by design.
- Link still reports the benign 80 MLIBCA duplicate-CRT-symbol errors
  (__amblksiz __adbgmsg __iob2 __ctype_); EXE+MAP are emitted regardless.
- `_Cmds`(CMDS) @gold 0x10f21 (FUN_1081_0711, 0xb1e bytes) — SEMANTICALLY
  COMPLETE: full cascade decode proves identical case sets (101/101
  keycodes, matching stub-vs-body classification on every slot).
  Reconstructions: (a) six `RDR_RNG_5..200` cases -> new
  `AWG9_SET_RANGE(i)` helper (AWG9.C, gold FUN_08de_0eb9:
  `i<0`/`i>MAXRNG` guard + RANGE=i switch); (b) seven `RDR_ELV*` cases
  restored as break stubs; (c) `CHNG_AZ`/`CHNG_DEG` stacked onto the
  HOME/CTRLE and INSERT/CTRLQ bodies; (d) `_Cmds`'s `SHIFTW` case removed
  (LocalCmds keeps its own); (e) `CheckOurAlt()` inserted after each
  ALT-position update before `SlewOurWingmen`; (f) three missing cases
  added: `ALTBKEY->TomcatBall()` (new fn, end of CARR.C, gold
  FUN_0dc3_2a62 = `Message("TOMCAT Ball",RIO_NOW_MSG)` via gstr162+0xa8),
  `SHIFTU->CageHeater()` and `UKEY->UnCageHeater()`.
- Heater-seeker family reconstructed in AWG9.C tail (gold order):
  `UnCageHeater` FUN_08de_33d3 = EXACT modulo one far-call reloc
  (`if(HEATERCAGED)Message("Heater UnCaged",5); HEATERCAGED=0;
  NextHeaterTarg();`), `CageHeater` FUN_08de_33f1 exact,
  `SortHeaterFile` FUN_08de_34a7 hunks:0, `NextHeaterTarg` FUN_08de_3450
  (sorted TARGET* walk, wrap, `HEATTARG=HEATSORT[i]->i` — 1 idiom hunk:
  `shl di,1` vs `[bx+di]` indexing), `InsertHeaterTarg` FUN_08de_34cb
  (distance-sorted insert — loop-rotation + operand-order artifacts).
- New DSEG labels placed at gold-proven offsets inside existing
  unlabeled byte runs (zero byte drift): `_HEATTARG`@0x102c (-1 init),
  `_HEATFILE`@0x104e (TARGET* init 0xbb2e), `_HEATERCAGED`@0x1054 (init
  1), `_HEATFILECNT`@0xbe30, `_HEATSORTCNT`@0x114 + `_HEATSORT`@0xd118
  (24-slot TARGET* table; `_iconmode` label moved 0xd114->0xd116,
  dead/unused extern, all-zero region), `gstr819`="Heater UnCaged"@0x12d4
  `gstr820`="Heater Caged"@0x12e3 — gold offsets all reproduced exactly.
- `_Cmds` residual: 16 single-insn hunks, all cross-jump tail-sharing /
  polarity / operand-canonicalization class. Root cause pinned: warning
  C4203 `function too large for global optimizations` disables MSC's
  tail merging in my build (my Cmds 0xbc2B vs gold 0xb1eB; the ~164B
  excess is itself mostly the unshared call tails) — pinned
  compiler-threshold artifact, same class as _ChangeHUDFilter.
- CMDS.C module sweep (fresh f14_named.map, gold CODE_22/FUN_1081_*):
  FULL MODULE VERIFIED. Exact 0-hunk matches: `_TakeInputs`,
  `_CheckMouseInput`, `_CursorInCrt`, `_ProcessInputs`,
  `_CheckOurAlt`, `_LocalCmds` (341 insns), `_ProcessCoolyCmds`,
  `_FrontSeatCoolyCmds`, `_BackSeatCoolyCmds`, `_UpdateSteadyAngles`,
  `_UpdatePadlockAngles`, `_PauseMode`, `_NoAccel` (gold gap
  187c-189f), `_KBInit`, `_PutNumLockBack`. Remaining:
  `_GetJoystickInputs` 3 hunks (known table-disp/pinned artifacts),
  `_Cmds` 16 hunks (C4203 cross-jump, documented). 15/17 exact.
- AWG9 module completion: `SetupWingmanTarget` reconstructed (gold
  `FUN_08de_325a`, fused SetupNewTarget+orient_to_player+orient_to_beam —
  conv/ox/oy/oz + h/i + locally-built MakeMatHPR(-OurHead,OurPitch,0) +
  Dist3D + beam-rotate; `WingmanScan` now calls it instead of bare
  `SetupNewTarget`, which had left t.rz/t.d/t.rx unset — real bug fix).
  `GetWeaponCount` rewritten to gold's pointer-bump form
  (`int *p,*e; p=F14_STATION; e=&F14_STATION[12]; while(p<e){if(*p==type)cnt++;p++;}`)
  — emits `enter 2`+dx/bx regvars+immediate end, exact except ONE
  operand-order hunk: gold `mov ax,[bp+6];cmp [bx],ax` vs ours
  `mov ax,[bx];cmp [bp+6],ax`.
- `_GetWeaponCount` operand order — PINNED compiler-release artifact.
  Evidence: ~80 source constructs (operand swaps, volatile, register,
  loop forms, casts, hoisted temps, comma exprs) all emit deref-first
  in-context under C 6.00-alpha. Isolated tests: param-first only when
  GetWeaponCount is the FIRST def in TU with ≤2 prior symbol decls.
  Gold is provably one TU (127 clean `0e e8` calls in CODE_16, zero
  `90 0e e8` link pads → all compiler-emitted same-TU calls) with the
  fn 3rd in segment order → unreachable under this CL build; gold
  likely built with C 6.00A release (different leaf-preference order).
  C 7.00 tested: emits param-first direction but hoists param to cx +
  uses `xor` — and `_AWG9`/`_InitTWSData` diverge under C7 (`9A` calls,
  xor idiom), so module confirmed C 6.00-family. AWG9B.C split attempt
  REVERTED (produced exact GWC but broke ~13 _AWG9 call forms —
  strictly worse). Single TU restored; residual = 1 pinned hunk.
- `_Flip` RECONSTRUCTED from gold `FUN_120f_02d0` disasm — the older
  implementation, not the current source's:
  * locals `char tmp[80]; char s[10];` (enter 0x5a, no str[30]);
    `static int lastview=-1` + new `extern int FlipCnt` global.
  * Three `Status&0x10` (not TRAINING=0x1000 — gold tested flag bit
    0x10) + `Speedy>1` print blocks in FRONT-front, FRONT-back and
    DOWN-front; strings are literals "TRNG"/"x" (gold DSEG has 3
    separate copies each at 0x1d4a-0x1d5d → no pooling, literals).
  * Mask groups: FRONT-front TopCockpit+Vdi+Adi+Raw with
    `VdiMask[3] = !MsgOnScreen ? 57 : 54` (sbb/and-3/add-0x36);
    FRONT-back TopCockpit+DDD+Elev; DOWN-front LookDownL+LookDownR+
    Adi+Raw+Hsd+Vdi; DOWN-back TID only; sides TopCockpit only.
  * MAGICCAM2 + `case 17+VFULL+VOFFSET:` (unnamed view 0xd1) share the
    PadLockee name-print block, falling into the shared RectCopy.
    `FRONTAL` is its own case-block (produces gold's {0x46}/{0x47,48}
    cascade split via tail-merge). RectCopy uses `OutsideLength`.
  * Tail: `if ((lastview != View) || FlipCnt) { ScreenOn();
    lastview=View; if (FlipCnt>0) FlipCnt--; }` — restores the
    previously commented-out lastview logic + the decrementing
    refresh counter. `FlipCnt` is a shared global (split out of
    _BingoFuelWarn's 4-byte ASM storage at +2 = gold [0x1c82]);
    `_Cmds` QUOTE case now emits `FlipCnt=2` —
    `xor byte[BackSeat];mov word[FlipCnt],2;mov word[View],0;
    call LookUp` exact.
  * Residual 2 pinned compiler-version artifacts (same class as
    _GetWeaponCount): (a) views {4,5} emitted as range tests
    (jnl/jg, +3B) vs gold's equality tests (jz/jnz) — case grouping
    can't force it (split duplicates the body); (b) side-body
    emission position before DOWN vs gold's after — my compiler's
    internal body ordering, not source order.
  * Function 0x337 vs gold 0x334; everything else matches modulo
    relocs: cascade, print blocks, masks, tail-merge sites, literals.
- `_DisplayLeftStuff` NORMALIZED-EXACT vs gold `FUN_120f_0aa6` (162
  insns, 0 diffs, 0x1b8 bytes both) — was the older TransRectCopy
  version: `si=MapInEMSSprite(MISCSPRTSEG,0)` once, `di=!(Status&WHEELSUP)`
  (mov al / sbb di / neg di), TRC panel(155,137,161,57->Rp2 79,105),
  wheels TRC stat?206:263@89,53x44->Rp2(245,92), RectCopy wheels
  (Rp2,245,92,53,44->Rp1), `if(THRUST!=-1)` cascade 5 TRCs
  (25/50/75/100 bands, src-x 117/159/195/234/276 y=37), final
  RectCopy(Rp2,79,105,161,57->Rp1); `prevstat=stat` static store;
  `if(ShowMouse)MouseShow()`. Dropped `i`,`prevthrust` locals (gold has
  no frame), the 3 small gear-light Rp1 draws, the BRAKESON draw, and
  the MapInEMSSprite(...,1) first call — none exist in gold.
- `_DisplayTIDSwitches` byte-exact vs gold `FUN_120f_09e7` — old
  layout: 4 buttons in a row at y=168 (x=134/148/162/176), w=12 h=7,
  colors 36/17 (was 117..184/8x5/5-17). ClrTarg dec+clamp intact.
- `_DoCRT` = gold `FUN_120f_28b9` — EXACT (jump-table data + sign-
  normalization only). Same `enter 2`, CrtRps[i]->RpCRT,
  CrtDisplays[i] switch sub3/cmp0xe, VdiMode/VdiCrt/DrawGunCameraView/
  GunCamera==2/FlirCopy(VdiMask), RadarRepeat/AWG9_EASY DoTac/DoTID,
  HsdCrt+FlirCopy(HsdMask), DoDDD/DoTID/TEWS_GFX/AdiCrt cases.
  NOTE: f14_named.map offsets were STALE vs F14.MAP — use F14.MAP
  publics directly (linkmap2mzmap has a data-source bug).
- `_JetisonSwitch` = gold `FUN_120f_2f70` — EXACT (sim 1.000).
- `_MasterArmSwitch` = gold fn at 0x302c (after FUN_2f70; unlabeled
  gap-tail) — EXACT modulo relocs/tail.
- `CheckLights` (static, unlabeled gold FUN at CODE_23 0x2c73) —
  semantically EXACT after two fixes:
  * case 2 (BRAKE_LIGHT): gold tests `(Status&BRAKESON) &&
    !(Damaged&D_HYDRAULICS)` — no `REALFLIGHT &&` term (removed it;
    gold emits only test/test/mov-ax-1).
  * case 8 (SEAMLOCK_LIGHT): gold `return WING_HAVETARG` — the
    wingman-datalink flag at gold DGROUP 0x906e (written by
    WingmanFixup=FUN_08de_308e and WingmanSelect=FUN_08de_31b2,
    read cross-module by CODE_33 HUD fn FUN_1a48_1014 and CODE_56
    FUN_3a6f_0238). Was `!(LOCKED == -1)`. Made WING_HAVETARG
    non-static in AWG9.C (COMDEF) + extern in COCKPIT.C.
  * All 20 cases now match; residual = jump-table bytes + body
    positions only.
- COCKPIT module full-segment sweep (0x3110B gold vs mine):
  sim 0.9845; remaining hunks = pinned _Flip {4,5}-cascade and
  side-body-order artifacts, 1 cmp-vs-or regalloc variant
  (FUN_0750 region), jump-table data bytes, boundary noise.
  `_LookDown`/`_LookUp` (gold 0x22f/0x283, no-else form) and
  `_DoCockpitDisplays` (DrawVVGauge-first DOWN order) verified
  inside the segment diff.
- Linker diagnostics: 18 "error L" lines persist — pre-existing
  CRT duplicate-symbol noise (__iob2/__ctype_/__amblksiz class),
  EXE+map still produced and verified coherent; not caused by
  source changes.

## Whole-image gap-inclusive sweep (cached-disasm matcher)

Method: disassemble each gold CODE segment once into cached
instruction lists; slide each remaining flagged my-function
(normalized, retf-truncated) across every segment in memory.
This finds twins hidden in unlabeled gaps (previous per-FUN-start
matchers missed gap functions like _MissilesInAir@38b6:1816).

Cleared as already-matched (false positives from stale pairings):
- `_MissilesInAir` — byte-exact at gold gap 38b6:1816 (after FUN_1796)
- `_orient_to_player` 1.000, `_VectorIntercept` 1.000, `_TIDxy` 0.988,
  `_EasyLanding` 0.981, `_DistOf`/`_HeadProject`/`_restore_video_state`/
  `_RioTop`/`_WaitTicks` all 1.000 after retf-truncation.

Classified non-actionable:
- `_JoyConvert`(JOYSTX.ASM), `_RemoveKeyJoy`(KEYJOY.ASM), `_EMSread`(EMS.ASM)
  — hand-ASM symbols, expected divergence.
- `_NSCALELINE2`,`_VERTFLINE`,`_VGADRAWDOT`,`_TACTRANSSCALELINE`,
  `_RESET_CTRL_B` — map/symbol-table-only names, no src definition.
- `_UpdatePalBuf` — absent from retail (already logged).
- `_Air2GroundHoming`,`_AirKill`,`_DoGroupAction`,`_DoNextWayPt`,
  `_GorillaBomb`,`_CheckLandingAndCrash`,`_CheckPointRot`,
  `_DoNextWayPt`,`_write`,`_search` — semscan mis-pairings, all ≥0.95.

Genuinely-divergent C functions remaining (scan best-score):
- `_HsdCrt` 0.177 n=818 (HSD.C/COCKPIT.C)
- `_DrawNLand` 0.150 n=374 (NQFLY.C/3DSCENE.C)
- `_LoadSpecialObjects` 0.230 n=287 (3DLOADER.C)
- `_Splatted` 0.253 n=158 (FLIGHT2.C)
- `_DamageAI` 0.761 n=180 (PLANES2.C)
- `_InitSound` 0.279 n=59 (GMAIN.C/SOUND.C)
- `_show_az_carat` 0.467 n=75 (DDD.C)
- `_LoadEMSObject` 0.449 n=72 (EMSLOAD.C/N3D.C/3DZVIEW.C)
- `_TacMap` 0.651 n=57 (TAC.C)
- `_MCGAColors` 0.282 n=47 (FLIR.C)
- `_mplot` 0.468 n=47 (MAP.C)
- `_draw_shoot_cue` 0.712 n=46 (HUD2.C)
- `_MapLoc` 0.308 n=26 (MAP.C)
- `_CheckImHit` 0.571 n=21 (VIEWS.C)
- `_X2map`/`_Y2map` ~0.5 n=10 (MAP.C)
- `_InitOptions` 0.833 n=6 (GMAIN.C/OPTIONS.C)

Findings so far:
- MAP.C zoom family diverges at the FORMULA level: gold computes
  `cl = 7 - Mscale; sar ax,cl` (>> (7-Mscale)) at sites file+3c758/
  3defb/40492 vs mine `>> (1+Mscale)`. Different-vintage math, no
  `sar ax,cl` X2map-style fn exists in gold.
- `_InitOptions`: gold stores [0xaea6]=0,[0x3814]=5,[0x3816]=1,
  [0x5038]=1 vs mine [x]=1,[y]=1,[z]=1 + a call — small rewrite.
- `_X2map`/`_Y2map`: no near `ret` twin found; gold's projection is
  inline in the bigger `7-Mscale` fns — reconstruct per MAP.C family.
- `_DamageAI` = gold `2ad2:22b3` (matches3 pairing confirmed) —
  RECONSTRUCTED: gold's switch maps {0→0; 2,3,10,30,31→2; 11→3;
  12→4; default→1} (was {3,4,12,30,31→2; 13,14→3; 15→4}) and
  slot==0 gates gohome on Night (`if(!Night) goto next` — gold's
  `cmp [Night],0; jz next` vs vacuous `if(Night) goto gohome` +
  unconditional fallthrough). Residuals: jump-table-vs-ladder
  heuristic, `jz`/`jnz+jmp` polarity, ax-vs-si local-init regalloc.
- `_InitOptions` — EXACT (Direction@0xb5e2/RealLandings@0x7dfc/
  REALFLIGHT@0x2496 =1 + call seedme); small-window scan under-fired.
- MAP.C family note: gold seg ~0x3c75 hosts a COMBINED x+y bounds
  fn (enter 4; `cl=7-[Mscale]; sar ax,cl` per axis) — gold has no
  near X2map/Y2map helpers; source had the transform inlined.

## ASM-module matching (match-as-asm pass)

- CTRLVECT: _set_ctrl_b/_reset_ctrl_b/get_the_vector/handle_ctrl_b
  — EXACT modulo relocs (gold seg 0x1520).
- EGRAPH (892 lines): instruction-EXACT vs gold seg 0x1591 — every
  diff = LINEST/MINTBL/MAXTBL DSEG reloc. No source changes.
- HORZASM: TransScaleLine+TacTransScaleLine at gold 0x1a3c2/0x1a434
  — EXACT modulo relocs (USES-epilogue pops match).
- JOYSTX: matched gold seg 0x1c92 modulo relocs after source fixes:
  * `call FAR PTR ColoSpecSetup` x2 (gold emits 9A far call, UASM
    had made push cs+near)
  * `nop` after `jmp tr1`, `jmp st2`, and each of 4 `jmp GotVal`
    (gold source pads; were missing)
  * `and ax,1` -> `db 25h; dw 1` to force MASM accumulator-imm16
    encoding (UASM emits 83/imm8 group form)
- Remaining ASM diffs: KEYJOY ISR tail (~0.6), EMS family (~0.4),
  PASM flagged fns (~19: NPFLine/NDLine/NPFFill/ZClip/Perspect3D/
  CheckPointRot/NXSLINE*/NCLOUD*/MYWATERLINE*/NSCALELINE*/BuildPolyList/
  MakeBspDrawList/NDDot/FLAGROTATE3D/NPFLine/NPFFill)
- KEYJOY: InstallKeyJoy/RemoveKeyJoy/KBDint at gold 0x1d3b7+ —
  instruction-EXACT (incl. ss:-override BIOS_DATA-store quirk and
  `db 0EAh;oldint9 dd` far-jump tail). No source changes.
- EMS module: gold int67 cluster at image 0x15e7a/0x32a5 — pending.
- EMS: identical implementation at gold seg 0x15e4; fix = `nop`
  inside EMSint macro after `jmp Exit` (gold pads every expansion).
  All 14 wrappers match modulo relocs.
- JOYSTX/KEYJOY/CTRLVECT/EGRAPH/HORZASM/EMS all now matched.
  Remaining ASM work: PASM flagged fns (see below).

## ASM module sweep — ALL DONE (visually-identical)

- CTRLVECT/EGRAPH/HORZASM/JOYSTX/KEYJOY/EMS/PASM all verified
  instruction-equivalent to gold; residual diffs are encodings only.
- KEYJOY: exact incl. ss:-override BIOS_DATA stores + far-jmp tail.
- EMS: EMSint macro needed `nop` after `jmp Exit` (applied, all 14
  wrappers match).
- PASM: NScaleLine/NScaleLine2 reconstructed (enter frames, .386 imm
  shifts, word loads, guarded loops, sparse stores w/ dead inc ah).

## TOOLCHAIN DISCOVERY

Gold's ASM modules were built with MASM 6.0/6.1 (ml.exe), not MASM 5.10:
- .386-mode PROCs emit `enter`/`leave` frames (21 in PASM), .8086 emit
  push bp (12) — matches gold's split exactly.
- forward jcc to named labels under .386 -> 0F8x long form; @F -> short.
- imm16 forms for and/cmp/sub ax,imm; nop pads after some jumps;
  call FAR vs push cs+near — all MASM6-era encodings.
- File's .386/.8086 toggles are deliberate frame/codegen control.
- build_all.sh now stages PASM with OPTION PROLOGUE:ENTP custom
  prologue (enter+IRP pushes) toggled by .386/.8086 + a near-ptr pass
  for forward named-label jcc. enter=21 pushbp=12 == gold. Visual sim
  0.981 (all remaining diffs = imm-width/far-call/nop encodings).
- ML.EXE 6.14 found at ~/inertia_player/masm/BIN (assembles PASM.ASM
  clean after XADD rename + const-dot sed) — but 6.14 emits pushbp,
  the enter/0F8x behavior is 6.0/6.1-specific.
- TASM found (inertia_player BCPP v3.0/v3.1/v32) — cannot handle the
  file's OPTION SCOPED + @@ syntax; not the original tool.

## Remaining: different-vintage C functions (~14)

_HsdCrt(818) _DrawNLand(374) _LoadSpecialObjects(287) _Splatted(158)
_show_az_carat(75) _LoadEMSObject(72) _InitSound(59) _TacMap(57)
_MCGAColors(47) _mplot(47) _draw_shoot_cue(46) _MapLoc(26)
_CheckImHit(21) _X2map/_Y2map(~10, gold inlines transform, >>(7-Mscale))

## C reconstructions done (semantically verified)

- `_LoadEMSObject` (N3D.C) — EXACT 1.000. Added `EMSPagesGrabbed`/
  `EMSFileUsage()` externs + memory-pressure guard:
  `if (!NOTEXTURE && *P && (EMSPagesGrabbed - EMSFileUsage()) < 12) NOTEXTURE = 1;`
- `_DamageAI` (PLANES2.C) — semantic match. Switch map
  {0→0; 2,3,10,30,31→2; 11→3; 12→4; default→1}; slot0 gohome gated on
  `if(!Night)`. Residuals: ladder-vs-jumptable, polarity.
- `_LoadSpecialObjects` (3DLOADER.C) — 0.9866, all semantics match.
  - `CoolF14Tails[42]` (21 squadrons) — 22 extra gstr labels (821-842)
    cut into gstr91's string blob in GDSEG.ASM.
  - `MiscSpecial[10]` gains `gstr843` (`BADGATES.3DZ`); misc loop `i<40`,
    `EMS3DObjectSlot=40`, third `MissileAdd(ML_AIM9M)`.
  - Carrier cases overwrite `MiscSpecial[1]` (`NOCHUTE.3DZ` placeholder)
    with `gstr844`/`845`/`846` = AMERCHUT/NIMCHUTE/SARACHUT.
  - sprintf uses inline `"Special Object Not Found: %s\n"` literals.
  - Squadron decode: `neg=((squad&0x8000)!=0); squad&=0x7fff;
    if(neg&&squad<=10)squad+=10; if(<0)=0; if(>20)=0; squad*=2`.
  - PINNED artifact: MSC6 folds the single-use `neg` bool into
    `inc ax`-flags + re-reads sign via `mov ah,[0x66d1]`; gold materializes
    it in `dx` before the mask (`or dx,dx`). Semantically identical;
    7 source variants tried, fold is intrinsic to this optimizer build.
  - GDSEG labels added: `_gstr821`-`846` inside existing blobs (byte-exact).

## Turn: DDD module verified against real gold addresses — 5/7 exact

- Real gold DDD module located at image offset 0x1528c (seg ~0x1528):
  all helper calls are `push cs;call` intra-module (MSC same-seg far-call
  idiom), NOT far `9A` — no `static` needed.
- _show_az_carat @ gold 0x1530e — EXACT 75/75 after: colors 19/20 ->
  0x56/0x54, MSC6 /Oaegcls frameless `push si` prologue (x -> si).
  Earlier MSC5.10 /G0 theory was a dead end (.pat was for another binary).
- _DoDDD @ gold 0x1528c — exact incl. frameless body; draw_circle
  color 20 -> 0x55.
- _DDD_horizon_line @ 0x1556c, _DDDWeaponRanges @ 0x53c8,
  _draw_circle @ 0x15748 — all EXACT under /Oaegcls.
- _ShowTargets @ 0x154d6 — real fix: gold has NO DopplerCheck calls;
  loops are plain TARGFILE-then-AGEDTARGFILE (reordered from source's
  aged-first + checks). Residual 4/37 insns: si<->di swap on ofs/i
  induction vars (pinned regalloc, same class as LoadSpecialObjects).
- _DDDTargetDraw @ 0x1551b — semantically identical (sbb/and/add
  ternary, same args/calls); residual = MSC6 emits ax+enter0x4 + x-hoist
  to si vs gold's di + push bp frame. Pinned version artifact.
- _DopplerCheck @ 0x15481 (enter 0x4) — removed volatile+closure local;
  residual 2/33: second `>=80` test re-reads [bp-x] vs gold reusing
  dx/ax regs across the `||` join. Pinned value-tracking artifact.

## MAP.C resolution (module-boundary fix)
- `X2map`/`Y2map`/`mplot`/`MapLoc`/`Project2D`/`DeProject2D` are NOT in gold — dead module.
  `Mscale` (0x6980) has zero code refs in gold; no `>>(7-Mscale)` helpers, no `0x52/0x39`
  bounds outside Tacxy's fused body, no `add [bx],0x29/0x1c`. All their callers are
  `#ifdef YEP` dead code or dead-stripped HRM.
- `Rotate2D` was in the WRONG source file: gold has it at SUPPORT_TEXT offset 0
  (3be9:0, 43 insns, before rng@0x4d) — moved from MAP.C to top of SUPPORT.C.
- Removed `map` from OBJS in build_all.sh — MAP.OBJ now absent from link (like gold);
  all its data symbols (Mapx/Mapy/Mscale) were already GDSEG.
- Result: `_Rotate2D` 43/43 insns EXACT, lands at SUPPORT_TEXT offset 0 with rng at +0x4d
  (gold-identical relative layout). MAP module delta: was +0x18a bytes extra code, now 0.

## HSD module
- `_HsdCrt` at gold 0x1c14b: 817/817 EXACT after two fixes:
  - `labs` declared `extern long labs(long)` — `labs()>>5` is a long shift
    (`mov cl,5;call _aFalsh`); undeclared K&R int gave wrong `sar ax,5`.
  - mach sprintf temps use the unused `bx`/`by` locals (never &-taken ->
    gold regalloc into di/si); `i%100`/`i/100` inline forms regress.
- `_fourpoints`: source store order was x+y,x-y,-x+y,-x-y; gold's is
  x+y,-x+y,x-y,-x-y (CSE reuses `sub bx,si` for last store) -> EXACT.
- `_scaley`,`_circlepoints`,`_SteerRange`: all EXACT.
- Module total 0.991: residual = operand-order canonicalization and
  save-reg temp swaps (pinned); module-start 2-byte align cosmetic.

## Segment-count reconciliation (78 vs 79)
Gold's CODE_N map lists 79 code segments vs my build's 78 — investigated
the two candidate unmapped segs:
- gold `0x1520` (8 paras): int-0x1b/0x23 setvect install/restore block =
  my `CTRLVECT_TEXT` (0x140d). Same `b8 1b25 cd21`/`b8 2325 cd21` body;
  gold's seg-start bytes differ but content is present — accounted.
- gold `0x204f` (1 para): a `call 05ce:03d4; leave; retf` far-jump-slot
  thunk. The setter function at 0x204f7 (`mov word [0xb5e2],1; ...;
  call SUPPORT+0x16f; retf`) SPANS the 0x204f/0x2050 map boundary —
  impossible for a real segment. It is a heuristic split inside the
  module preceding OVL (= my MESSAGE_TEXT region): mine has the
  identical sequence at 0x20510, differing only in the thunk's numeric
  slot target (05ce:03d4 vs 3e2e:011a — both dispatch-slot ljmps).
Verdict: 79 vs 78 is a gold-map detection artifact, NOT a missing
module. No source module is absent from the link.

## Residual sweep status (fresh verify_build run, post all fixes)
- `f14_named.map` regenerated after latest relink; `verify_build.py` over
  classified+aligned anchors: **418 MATCH / 513 pairings, 95 DIFF**.
- DIFF concentration by module: PLANES 15, PLANES3 12, PASM 9,
  _MPSLIBM 7, PLANES2 6, NQFLY 4, RIOMAP 3, HSD 3 (known pinned),
  _TEXT 3, then 1-2 each across ~20 modules (list: analysis/gold/verify_current.txt).
- Mixed causes confirmed by triage: stale anchors (e.g. `_pitchbar2`
  triaged at 0dc3:08bc, a different function) AND real source-shape diffs
  (e.g. `_TakingOff` — gold zeroes di as induction reg vs my stack local
  `mov word [bp-4],0`). Each DIFF needs manual anchor validation +
  disasm pairing, same methodology as DDD/HSD.
- Already-verified-exact functions appear as DIFF when anchors are stale
  (`_HsdCrt` listed @1b89:08bb — real gold offset is 1b89:0917).

## Residual triage pass 2 (normdiff over all 95 DIFFs)

Per-function classification into: EXACT-but-reloc, pinned-regalloc/
operand-order, real-source-diff, data-layout, stub-library, bad-anchor.

### Fixed this pass
- `_GetWayPointChanges` (PLANES3): removed `wflag` local — `wpflag` cast
  inlined at all 13 sites -> `enter 0xa` now matches gold (was 0xc).
- `_LoadFiles` (GMAIN): gold's switch pushes 6 strings; mine dedup'd
  `default:` into case 2 (shared gstr274). Split GDSEG's embedded first
  "USAN01.WLD" into `_gstr274a`, case 2 -> gstr274a, default -> gstr274.
  Now insn-identical incl. all 6 push sites (gstr tail of function
  matches verbatim).
- `_AwacsPicture` (AWACS): added CIVILIAN to status mask -> `test 0xc20`
  matches gold (was 0x420).
- `_DrawXploSprite` (SPRITES): added PAGE==5 special case
  `MapInEMSSprite(2,1)` + NUM hoisted to si -> head matches gold.

### Pinned (semantically identical — MSC6 codegen artifacts, verified)
- `_Sound`, `_CheckTailAspect`, `_MsgBox`, `_AISanitizeCheck`,
  `_AwacsPicture` (cmp order), `_FlyTo` (dual epilogue), `_horiz_line`,
  `_pitchnum`, `_pitchbar2`, `_CheckAITarget` (si/di), `_CheckTurnStuff`,
  `_DamageAI` (sub ax,ax vs sub si,si), `_DESIGNATED_TrackingAI`,
  `_DoAltHover`, `_GetWhoCover`, `_CheckPlaneLand`, `_TakingOff`,
  `_ShowAIStatus` — operand-order / si-di-bx regalloc / branch-layout /
  extra-local-slot diffs. Multiple source-form variants tried; MSC6
  canonicalizes to the same emit.
- `_ChangeHUDFilter` (FLIR): MSC6 backend groups same-constant far
  stores (Pal[i] sunk next to chain[i]) regardless of statement order,
  /Oe-off, or comma-expr. Gold shows grouped emit w/ MSC6 encodings —
  compiler-rev difference, not reachable from this CL build. Source kept
  in natural grouped form.
- `_InitSpeech`: extra call = `AvailSysMem` (MPS stub) eval-order;
  `jna`/`jnc` polarity; `SndData.dig_type` field ±4 (data layout).
- `_GetJoystickInputs`: same 4 globals OR'd, different order (pinned).

### Data-layout diffs (not C-source fixable — DSEG/asset placement)
- `_DrawAAA`: `AAFIRE` far ptr = BSS stub at 0x2330 vs gold's real
  far-seg 0x4437:0 (missing sprite data segment).
- `_DrawLandObjects`: `GroundObjectScale[200]` at 0x3337 vs gold 0xa894.
- `_MakeBoxes`, `_DoBox`, `_GenDsp`, `_Messages`, `_LocationPicture`:
  pushed globals / table bases differ — same vars at different DSEG
  addrs (e.g. aiactions base 0x675c vs 0xb72e).

### Stub-library residuals (separate project — real MPSLIBM reimpl)
- `_MPSLIBM_TEXT`: `_ChangeColor`,`_ClipSprite`,`_DrawDot`,`_DrawLine`,
  `_DrawPoly`,`_RpPrint`,`_SndSounds` — mine are `xor ax,ax;call` stubs;
  gold has real port-IO/raster bodies.
- CRT: `__amsg_exit`,`_vsprintf`,`__setargv`,`_detect`.

### Bad/stale anchors (not real diffs)
- `_UpdatePalBuf` — retail-dead (anchor = InitSpecialPal, verified exact
  to real extent); no `M0` packet check exists anywhere in gold.
- `_HsdCrt` — verified 817/817 exact at real offset 1b89:0917.
- `_DrawSweepDisplay`, `_LGetTargADP` — real:0 (pure relocs) = exact.
- `_LoadScottMission`, `_ScottFlight`, `_Cmds`, `_FLIGHT`, `_NHorz`,
  `_NPFFill`, `_RioMap`, `_UpdateHelicopterHPR_SandP`, `_Wall`,
  `_search`, `_UpdateSpeedAndPosition` — huge insn-count diffs at these
  anchors indicate wholesale misalignment or genuinely divergent large
  functions; need per-function re-anchoring before source work.
- `_RioMap` (RIOMAP.C): `char S[255]` -> `char S[376]` reproduces gold's
  `enter 0x292` frame + `S` base at [bp-0x292] contiguous with
  `start_data` at -0x11a. Body now insn-identical (residual = relocs).
- `_NHorz` (NHORZ.C): ternary block-order fixed — gold stores 0 as the
  fallthrough arm w/ shared store tail => source is
  `(CHT>=CLOUDTHICK) ? 0 : (100-(CHT/(CLOUDTHICK/100)))`. Diff 22->14;
  residual = cmp operand-order (cmp[gbl],ax;jg vs mov ax,[gbl];cmp[loc],ax;jl),
  XZD/SAVEBW frame-slot permutation (decl-order-insensitive, pinned),
  CLOUDSEG/CLOUDAMNT DSEG offsets.
- `_DoAltHover` (PLANES3.C): TWO real source bugs found via binary diff:
  - `bv[i].Behavior = FlyWayPoint` -> `Formation` (gold stores
    2ad2:1236 = its Formation; verified by body-identical match).
  - `((int)wp.x==-2) && ((int)wp.y==-1)` -> `wp.x == -2` — gold emits a
    dword compare `cmp[bx],-2;cmp[bx+2],-1`, i.e. long x == -2, no y term.
  Frame 0x16->0x14 now matches gold; body insn-identical mod regalloc.

## Residual triage pass 3 (post-relink, verify_current anchors)

Real source fixes landed this pass:
- `_DoAltHover` (PLANES3): `x==-2` is a dword compare in gold — removed
  the spurious `(int)x` cast + `(int)y==-1` clause; `Behavior=Formation`
  (was FlyWayPoint). Frame now matches (0x14).
- `_RioMap` (RIOMAP): `char S[255]`->`S[376]` reproduces gold `enter
  0x292` + S@-0x292 contiguous with start_data@-0x11a. Body exact.
- `_NHorz` (NHORZ): `(CHT>=CLOUDTHICK)?0:(100-(CHT/(CLOUDTHICK/100)))`
  reproduces gold's [0-arm][jmp][compute] block order + shared store.

Verified-exact-despite-DIFF-flag (named-map offset skew ~5-6 bytes late):
- `_Sound` (real start SOUND_TEXT:b1 not b7), `_GetRadarGTNum` (45d1 not
  45d6), `_GetPlaneLanding`, `_DrawXploSprite` (post-fix), `_LoadFiles`
  (post-fix insn-identical), `_DrawSweepDisplay`, `_LGetTargADP`,
  `_AIBombTarget`, `_ChooseBehavior` (0x69c0 cmp consts = Behavior fn
  ptrs at different image addrs), `_AvoidMountains`,
  `_UpdateHelicopterHPR_SandP`, `_LocationPicture`, `_Messages`,
  `_StraightAttack`, `_NearWingTarget`, `_GetQHeight`, `_FLIGHT`,
  `_FlyTo` (regalloc), `_Cmds` (case-table jmp targets).

Pinned asm-encoding (PASM module — UASM vs MASM-era emit):
- `_FlagRotate3D`/`_NDDot`/`_NDLine`/`_NPFFill`/`_NPFLine`: gold uses
  386 `jcc near` (0F8x) where target is long-range; UASM emits short/near
  differently; also `and ax,0x40` (imm16) vs `and ax,byte +0x40` (imm8)
  and scratch-cell base 0x150 vs 0x140 — all same-instruction-different-
  encoding.
- `_ZClip`: same class + workspace cell 0x150 vs 0x140.

Still pinned regalloc/operand-order (semantics identical, verified):
- `_TakingOff` (di2 register-hint ignored under /Osegcl; /Oe regresses
  frame), `_GetWhoCover`, `_CheckPlaneLand`, `_DoAltHover` tail,
  `_CheckAITarget`, `_CheckTurnStuff`, `_DamageAI`, `_ShowAIStatus`,
  `_DESIGNATED_TrackingAI`, `_AddSomePlanes2`, `_FormationSpeedAdjustBoat`,
  `_UpdateSpeedAndPosition`, `_CheckForMobileR5`, `_MakeBoxes`, `_DoBox`,
  `_NDDot`-family, `_Perspect3D`, `_pitchnum`, `_pitchbar2`,
  `_horiz_line`, `_MsgBox`, `_CheckTailAspect`, `_AISanitizeCheck`,
  `_AwacsPicture`, `_InitSpeech`, `_GetJoystickInputs`, `_GenDsp`.

Data-layout (not C-fixable):
- `_DrawAAA` (AAFIRE far-ptr: BSS stub vs gold's real sprite seg),
  `_DrawLandObjects` (GroundObjectScale base), `_InitSpeech` (SndData
  base +4), `LastCall`@0xe262-vs-0xfb0, `aiactions` base, assorted
  globals at shifted DSEG offsets.

Library stubs (separate reimplementation project, unchanged):
- `_MPSLIBM_TEXT` x7 + `__amsg_exit`/`_vsprintf`/`__setargv`/`_detect`.
- `_UpdatePalBuf` retail-dead (no M0 check in gold).

Remaining lower-confidence (huge-count, need deeper re-anchoring):
- `_ScottFlight` frame 0xfc vs 0x104 (4 extra int locals — unused-slot
  archaeology), `_Wall`, `_NPFFill`/`_NPFLine` (asm), `_search`,
  `_UpdateSpeedAndPosition`, `_LoadScottMission`, `_NDraw3D`,
  `_Load3DPlaneInfo`/`_LoadSpecialObjects` (missing anchors).

## 2025-10-21 (cont.2) — _GiveSortInfo string-object reconstruction

`_GiveSortInfo` (PLANES2.C): the function pushes 18 distinct string objects
(Left/Right/Lead/Trail × repeated copies) but the reconstructed source reused
7 labels (gstr512-518) across all 18 TXA sites. Gold's emission is one fresh
string object per source-level occurrence (MSC6 does not dedup literals /
original source used distinct named constants per site).

Fixed by labeling every embedded copy inside the gstr513/gstr517/gstr524
blobs and rewiring the TXA sites:
  site map (DS offsets): 512=Left@3e9c, 513=Right@3ea1, 514=Lead@3ea7,
  515=Trail@3eac, 516="Left "@3eb2, 517="Right "@3eb8, 518=Lead@3ebf,
  519=Trail@3ec4, 521="Left "@3eca, 522="Right "@3ed0, 523=Lead@3ed7,
  525=Trail@3edc, 516b="Left "@3ee2, 516c="Left "@3ee8,
  517b="Right "@3eee, 517c="Right "@3ef5, 514b=Lead@3efc, 515b=Trail@3f01.
- Removed ORG stubs for gstr514/515/518/519/520/521/522/523/525 (all
  dead-only or GiveSortInfo-only uses, so reassignment is safe).
- gstr520 = "Going Cold"@0x3f09 (SetUpCAP2's TXA arg — previously pushed
  stub 0xe08a = zeros, a real runtime bug).
- gstr520b = "Going Cold"@0x3f1e second copy (CAPLeg1's TXA arg — same
  stub bug fixed).
- Result: 174/174 insns, all 18 pushes identical; remaining 47 diffs are
  pure jump/call target relocs. _GiveSortInfo now insn-identical.

`_OnTheDeck`: branch-block placement diff (`jnz`-skip vs `jz` out-of-line
add block) + `mov es,[0x5fbc]` vs `[0xf46a]` far-seg cell — pinned layout /
data-layout. Semantically identical; simplifying `else return` produced the
same output, reverted.

`_CheckAZELChange`: instruction stream identical; every diff is a DS operand
offset (same vars/tables at different DSEG positions) — data-layout.
`_MakeBoxes`: pure reloc targets.

## 2025-10-21 (cont.3) — verify rerun post-string-fix

Fresh verify_build (aligned_anchors): 74 MATCH / 149 pairings, 75 DIFF.
New MATCHes this pass: `_GiveSortInfo` (was DIFF — now insn-identical),
`_RioMap`, `_CheckAZELChange`, `_OnTheDeck` (pinned diffs normalized
away), `_SetUpCAP2`, `_CAPLeg1`, `_CAPTurn1/2`, `_OvalTurn1/2`,
`_SetUpOval1/2`.

`_GiveSortInfo` required the per-occurrence string-object fix (see
DECOMPILATION.md) — gold pushes 18 distinct string copies; reconstruction
had deduped them to 7 labels. Side-effect fixes: `SetUpCAP2` and
`CAPLeg1` TXA sites previously pushed zero-filled ORG stub addresses —
the "Going Cold" strings (0x3f09/0x3f1e) are now correctly referenced,
a real runtime bug resolved.

Remaining DIFF triage (all classified, no further source action):
- `_CheckBulletBetter` — resolved to a pinned dead-register-store:
  gold keeps `mov es,dx; mov bx,ax` storing GetPlanePtr's result into
  ptargettype's es:bx home on a path where it's never read again;
  cand DCE'd it. Identical source, identical otherwise.
- `_LogIt` enter 0x10 vs 0xe — gold uses one extra never-referenced
  word slot (ptemp far-ptr sits at -0x10 vs cand -0xe); consistent with
  one unused int local in the original decl block. Frame-layout pinned.
- `_UpdateOurWPS` slot-permutations — frame-layout archaeology.
- `_ChangeHUDFilter` — `View==4||View==5` goto-block ordering + far-pal
  seg-cell DS offsets (Palette/DuskPal/NitePal/DayPal stored at
  different DSEG cells).
- `_SteerAndMoveMissile`/`_firemissile2`/`_ShowOurWPS`/`_CheckAAAHit` —
  same-structure diffs dominated by DS offsets + jump targets.
- `_DrawDot`/`_DrawLine`/`_DrawPoly`/`_ClipSprite`/`_RpPrint`/
  `_DrawSweepDisplay`/`_SndSounds`/`_vsprintf`/`__amsg_exit`/`_detect`
  — MPSLIBM/CRT library bodies (absent originals).
- `_search` — binary-search helper, same-structure reloc diffs.
