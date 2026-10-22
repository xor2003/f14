## F-14 Fleet Defender Pre-Release
*Source*: https://archive.org/details/f-14-src  

### Build Environment
- DOSBox-X  
- Microsoft Macro Assembler (MASM) 6.11  
- Microsoft C/C++ 6.0  
- HX DOS Extender (DPMI) 2.16

Installing this software should correctly set environment variables to your `AUTOEXEC.BAT`. If you attempt to run without DPMI, you will get an error message:    
```
run-time error R6901
- DOSX32 : This is a protected-mode application that requires DPMI (DOS Protected Mode Interface) services. Examples of hosts that provide these services include:  
 - a DOS session under Windows 3.x in enhanced mode  
 - Qualitas 386MAX or BlueMAX version 6.x  
```  
Initialially I installed Windows 3.x and ran with enhanced mode `WIN /3` but was unsucessful. HXRT216 seems to work just fine, so add it to your `AUTOEXEC.BAT` (or environment script): `HXRT216\BIN\HDPMI32.EXE`.  An issue with running this DPMI is an error message during compilation which states the following:  
```
Phar Lap fatal err 10049: Ran out of stack buffers
```
In my environment script (or `AUTOEXEC.BAT`) you can try adding `set GOTNT=-NOPAGE` prior to executing `HDPMI32.EXE`. This helps a little bit (I think), but it has required me to still reboot and continue attempts to recompile further.

The target we are interested in compiling is `f14.exe` under `src`.

### Changelog
There are various issues with building the source as it stands, some of which are fixed. The first issue was that various C files required headers, and only some C files referenced `INCDIR`. I've simply added `-I$(INCDIR)` to the `CFLAGS` in the `MAKEFILE`. Another issue was that it appeared `PASM.ASM` was using reserved keywords for procedure argument names (namely `ADDR`); I've simply renamed these.   

### Current Issues
The biggest blocker right now is missing `MPSLIBM.lib`. I see that the `MAKEFILE` references a `..\lib\mpslibm.lib`, which would presumably have these missing functions such as `_DrawDot`, `_MouseON`, etc. This static library is not a target in the `MAKEFILE` and does not appear to be included, so this may require people going on a difficult scavenger hunt to find it.

---

## Dev-toolchain guide (`zip/`)

The leaked archive contains the game **plus the MicroProse dev toolchain**.
All game tables are authored as `.TXT` and compiled to packed `.DAT`
records the game loads at startup — the game never reads text.

```
*.TXT tables   --compiler-->  *.DAT records  -->  F14.EXE / F.EXE
*.MIS missions --MISCOMP---->  MISSION.F14     -->  campaign
```

Layout in `zip/`:
`tools/bin/` = prebuilt `.EXE` tools · `tools/src/` = their `.C` sources ·
`tools/missions/` = `.MIS`/`.TXT` mission & table sources ·
`tools/data/` = compiled `.DAT` + `BASEDEF`/`ENUMS.H`/`MISSION.H` specs ·
`h/` = headers · `game/` = full runnable retail install · `backups/` +
`docs/` = superseded files & notes · `archive/` = pristine ARJ volumes.

### Verified working (run under `kvikdos`)

`kvikdos --root=<dir-with-dat+txt> TOOL.EXE` — console tools stream output.

| tool | result |
|------|--------|
| `READIT` | ✅ dumps `F14PD01.DAT` plane records — names (`An-22a`,`MiG-25c`), speed, `.3DZ` model refs |
| `MISLCOMP` | ✅ `F14MD01.TXT`→missile table (`Crotale`,`CADS-N-1` + stats) |
| `RADCOMP` | ✅ `F14RD01.TXT`→radar table (`SA-2` + range/model fields) |
| `MISREAD` | ✅ mission→readable text (`[world] NorthCape`,`[time]`,`[loadout]`,`[briefing]`). **Needs `BASEDEF`** (field-definition file) in CWD or it errors `Error opening Input File basedef`. Usage: `misread <misName>` |
| `MISCOMP` | ✅ parses+validates missions. Usage: `miscomp misName /ddefName /ooutName /eEnums.h` (out default `MISSION.F14`). Strict — flags undefined objects (`bad entry carrier`,`awacs` vs object DB) so it needs the right `basedef`/enum context |

### Non-trivial / caveats (tested)

- **`PLANCOMP` produced a 507 MB `F14PD01.DAT`** under kvikdos —
  apparently writes far more than the ~7 KB record table (a seek/extend
  bug or expects a specific environment). Use the shipped `.DAT`, treat
  its recompile with suspicion until the write behaviour is understood.
- **`MISREAD`/`MISCOMP` are not standalone** — both consult `BASEDEF`
  (object names) and `ENUMS.H`/`MISSION.H`; running them outside the
  data dir gives `bad entry`/`Unexpected end of input` errors.
- `LOGCHK`, `3DZVIEW`, `JOYREAD`, `VALREAD`, `OBJCHECK`, `PLANREAD`,
  `READMIS`, `MILCOMP`, `READFILE` — **source-only, no prebuilt `.EXE`**
  in the archive. (`READFILE` is a shared helper `.OBJ` the compilers
  link, not a standalone tool.)

### Building the tools

Sources use **only standard DOS C headers** (`stdio.h`,`io.h`,`fcntl.h`,
`dos.h`,`sys/*.h`,`stdlib.h`) — no exotic libs — so they build under any
period DOS C compiler. Borland C (`BCC.EXE`) under kvikdos currently
fails on A20/DPMI setup; use the project's MSC path (`C700/BIN/CL.EXE`)
or a real DOS box — a straightforward `cl MISLCOMP.C` / `bcc MISLCOMP.C`
single-file compile should suffice for the standalone tools, while the
mission tools additionally link `READFILE.C`/`READMIS.C` + `MISSION.H`.

### F-15 / F-19 applicability

The tool **format** is F-14-specific — `F14*01.DAT` record layouts and
`mission.f14` won't load F-15/F-19 data (different structures/filenames).
But the **toolchain pattern** (text table → compiler → packed `.DAT`,
mission compiler → `mission.*`, `BASEDEF`/`ENUMS.H` object DB) is the
same MicroProse scheme — the F-15/F-19 leaks use analogous tools, and
`zip/h/F15DEFS.H` shows the shared lineage. So these tools are the
reference implementation for understanding F-15/F-19's equivalent
utilities, not drop-in replacements.
