#!/usr/bin/env python3
# mztest.py NAME GPARA GEXT — mzdiff gold routine (at gpara:gext) vs ours NAME
import re, subprocess, sys, os
GOLD='/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'
OURS='build/obj/F14.EXE'
MZ=os.path.expanduser('~/mzretools/build/mzdiff')
name,gp,gext=sys.argv[1:4]
gm='/tmp/mz_g.map'
open(gm,'w').write('Size 6173e\nDGROUP DATA 50b1\nSEGX CODE %s\n%s: SEGX FAR %s\n'%(gp,name,gext))
r=subprocess.run([MZ,'--loose','--nocall','--map',gm,'--tmap','build/f14_named.map','%s:%s'%(GOLD,name),'%s:%s'%(OURS,name)],capture_output=True,timeout=120)
print((r.stdout+r.stderr).decode('latin1'))
