# F-14 Fleet Defender — retail-matching source rebuild progress

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
