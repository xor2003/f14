#!/usr/bin/env python3
# triage.py — for each REAL-diff pairing, show first mismatches compactly
import re, subprocess, sys, tempfile, os, json
GOLD='/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE';OURS='build/obj/F14.EXE'
GLOAD=0x5400; OLOAD=0x5000
def loadmap(path):
    segs={}; funs={}
    for l in open(path):
        m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
        if m: segs[m.group(1)]=int(m.group(2),16); continue
        m=re.match(r'(\S+): (\S+) FAR ([0-9a-f]+)-([0-9a-f]+)',l.strip())
        if m: funs[m.group(1)]=(segs[m.group(2)],int(m.group(3),16),int(m.group(4),16))
    return funs
gold=loadmap('analysis/gold/gold.map'); mine=loadmap('build/f14_named.map')
gex=open(GOLD,'rb').read(); mex=open(OURS,'rb').read()
def dis(exe,load,p,a,b):
    d=exe[load+p*16+a:load+p*16+b+1]
    t=tempfile.NamedTemporaryFile(delete=False);t.write(d);t.close()
    o=subprocess.run(['ndisasm','-b16','-o0x%x'%a,t.name],capture_output=True,text=True).stdout;os.unlink(t.name)
    return [l.split(None,2)[2].strip() for l in o.splitlines() if len(l.split(None,2))>2]
IMM=re.compile(r'0x[0-9a-f]+')
def norm(i):
    i=re.sub(r'call 0x[0-9a-f]+:0x[0-9a-f]+','call FAR',i)
    i=re.sub(r'\b(call|jmp|j[a-z]+) 0x[0-9a-f]+',r'\1 T',i)
    return IMM.sub('N',i)
res=json.load(open('/tmp/repair3.json'))
gcache={}
for r in res:
    if r['d']==0: continue
    nm,gf=r['nm'],r['gf']
    if gf not in gold or nm not in mine: continue
    gp,ga,gb=gold[gf]; mp,ma,mb=mine[nm]
    k=(gp,ga,gb)
    if k not in gcache: gcache[k]=dis(gex,GLOAD,gp,ga,gb)
    gd=gcache[k]; md=dis(mex,OLOAD,mp,ma,mb)
    diffs=[(i,x,y) for i,(x,y) in enumerate(zip(gd,md)) if norm(x)!=norm(y)]
    nd=len(gd)-len(md)
    print('=== %s vs %s (d=%d len %d/%d extra %d)'%(nm,gf,r['d'],len(gd),len(md),nd))
    shown=0
    for i,x,y in diffs:
        if shown>=6: print('   ... +%d more'%(len(diffs)-shown)); break
        # mark pure-operand diffs
        tag='op' if norm(x)==norm(y) else '  '
        print('   %s %3d -%-30s +%s'%(tag,i,x,y)); shown+=1
