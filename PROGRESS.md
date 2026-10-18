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
