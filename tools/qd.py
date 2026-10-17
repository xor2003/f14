#!/usr/bin/env python3
# qd.py NAME [GFUN] — compact normalized diff vs gold anchor
import re,subprocess,sys,tempfile,os
GOLD='/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE';OURS='build/obj/F14.EXE'
def loadmap(path):
    segs={};funs={}
    for l in open(path):
        m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
        if m: segs[m.group(1)]=int(m.group(2),16); continue
        m=re.match(r'(\S+): (\S+) FAR ([0-9a-f]+)-([0-9a-f]+)',l.strip())
        if m: funs[m.group(1)]=(segs[m.group(2)],int(m.group(3),16),int(m.group(4),16))
    return funs
gold=loadmap('analysis/gold/gold_named.map'); gold.update(loadmap('analysis/gold/gold.map'));mine=loadmap('build/f14_named.map')
def dis(exe,load,p,a,b):
    import struct
    raw=open(exe,'rb').read()
    load=struct.unpack('<H',raw[8:10])[0]*16
    d=raw[load+p*16+a:load+p*16+b+1]
    t=tempfile.NamedTemporaryFile(delete=False);t.write(d);t.close()
    o=subprocess.run(['ndisasm','-b16','-o0x%x'%a,t.name],capture_output=True,text=True).stdout;os.unlink(t.name)
    return [l.split(None,2)[2].strip() for l in o.splitlines() if len(l.split(None,2))>2]
nm=sys.argv[1]
gf=sys.argv[2] if len(sys.argv)>2 else None
if not gf:
    import json
    for r in json.load(open('/tmp/repair3.json')):
        if r['nm']==nm: gf=r['gf']; break
    if not gf or gf not in gold: gf=nm
gp,ga,gb=gold[gf]; mp,ma,mb=mine[nm]
gd=dis(GOLD,0x5400,gp,ga,gb); md=dis(OURS,0x5000,mp,ma,mb)
print('%s %x:%x-%x  vs  %s %x:%x-%x'%(gf,gp,ga,gb,nm,mp,ma,mb))
for i in range(max(len(gd),len(md))):
    g=gd[i] if i<len(gd) else '-'; m=md[i] if i<len(md) else '-'
    flag='  ' if g==m else '!!'
    if flag=='!!': print('%3d %s %-28s | %s'%(i,flag,g,m))
