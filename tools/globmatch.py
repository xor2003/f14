#!/usr/bin/env python3
# globmatch.py — global best-anchor search for REAL-diff routines
import re, subprocess, sys, tempfile, os, json
GOLD='/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE';OURS='build/obj/F14.EXE'
def loadmap(path):
    segs={}; funs={}
    for l in open(path):
        m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
        if m: segs[m.group(1)]=int(m.group(2),16); continue
        m=re.match(r'(\S+): (\S+) FAR ([0-9a-f]+)-([0-9a-f]+)',l.strip())
        if m: funs[m.group(1)]=(segs[m.group(2)],int(m.group(3),16),int(m.group(4),16))
    return funs
gold=loadmap('analysis/gold/gold_named.map'); gold.update(loadmap('analysis/gold/gold.map'))
mine=loadmap('build/f14_named.map')
gex=open(GOLD,'rb').read(); mex=open(OURS,'rb').read()
IMM=re.compile(r'-?0x[0-9a-f]+')
def dis(exe,load,p,a,b):
    d=exe[load+p*16+a:load+p*16+b+1]
    t=tempfile.NamedTemporaryFile(delete=False);t.write(d);t.close()
    o=subprocess.run(['ndisasm','-b16','-o0x%x'%a,t.name],capture_output=True,text=True).stdout;os.unlink(t.name)
    return [l.split(None,2)[2].strip() for l in o.splitlines() if len(l.split(None,2))>2]
def norm(i):
    i=re.sub(r'call 0x[0-9a-f]+:0x[0-9a-f]+','call FAR',i)
    i=re.sub(r'\b(call|jmp|j[a-z]+) 0x[0-9a-f]+',r'\1 T',i)
    i=IMM.sub('N',i); i=re.sub(r'[+-]N(?=\])','N',i)
    return i
def ndiff(gd,md):
    gn=[norm(x) for x in gd]; mn=[norm(x) for x in md]
    n=max(len(gn),len(mn)); d=0
    for i in range(n):
        x=gn[i] if i<len(gn) else None; y=mn[i] if i<len(mn) else None
        if x!=y: d+=1
    return d
# all gold disasms cached
gdis={n:(p,a,b,None) for n,(p,a,b) in gold.items()}
gc={}
def getg(n):
    if n not in gc:
        p,a,b=gold[n]; gc[n]=dis(gex,0x5400,p,a,b)
    return gc[n]
real=json.load(open('/tmp/realdiffs.json'))
out=[]
for nm,gf,lg,lm,dd in real:
    if nm not in mine: continue
    mp,ma,mb=mine[nm]; md=dis(mex,0x5000,mp,ma,mb)
    mlen=len(md)
    best=[]
    for gn,(gp,ga,gb) in gold.items():
        glen=gb-ga
        if abs(glen-(mb-ma))> max(0x60,(mb-ma)//2): continue
        gd=getg(gn)
        d=ndiff(gd,md)
        best.append((d,gn))
    best.sort()
    out.append((nm,gf,best[:3]))
    print('%-28s cur=%-20s best=%s'%(nm,gf,best[:3]))
json.dump(out,open('/tmp/globmatch.json','w'))
