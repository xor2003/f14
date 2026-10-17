#!/usr/bin/env python3
# repair.py — for each realdiff-listed routine, test ALL gold candidates
# from matches.txt and report the best instruction-match anchor.
import re, subprocess, tempfile, os, sys

GOLD='/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'
OURS='build/obj/F14.EXE'
GLOAD=0x5400; OLOAD=0x5000

def loadmap(path):
    segs={}; funs={}
    for l in open(path):
        m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
        if m: segs[m.group(1)]=int(m.group(2),16); continue
        m=re.match(r'(\S+): (\S+) FAR (.+)$',l.strip())
        if m and m.group(2) in segs:
            a,b=(int(x,16) for x in m.group(3).split(' ')[0].split('-'))
            funs[m.group(1)]=(segs[m.group(2)],a,b)
    return funs

gold=loadmap('analysis/gold/gold.map')
mine=loadmap('build/f14_named.map')
gex=open(GOLD,'rb').read(); mex=open(OURS,'rb').read()

def disasm(exe,load,para,a,b):
    data=exe[load+para*16+a:load+para*16+b+1]
    tf=tempfile.NamedTemporaryFile(delete=False,suffix='.bin')
    tf.write(data); tf.close()
    out=subprocess.run(['ndisasm','-b16','-o0x%x'%a,tf.name],capture_output=True,text=True).stdout
    os.unlink(tf.name)
    res=[]
    for l in out.splitlines():
        p=l.split(None,2)
        if len(p)<3: continue
        ins=p[2].strip()
        ins=re.sub(r'call 0x[0-9a-f]+:0x[0-9a-f]+','call FAR',ins)
        ins=re.sub(r'\[0x[0-9a-f]+\]','[MEM]',ins)
        ins=re.sub(r'word 0x[0-9a-f]+','word IMM',ins)
        ins=re.sub(r'\b0x[0-9a-f]+\b','IMMV',ins)
        res.append(ins)
    return res

def ndiff(a,b):
    n=max(len(a),len(b)); d=0
    for i in range(n):
        x=a[i] if i<len(a) else None; y=b[i] if i<len(b) else None
        if x!=y: d+=1
    return d

# name -> candidate gold (para,off) anchors from matches.txt
cand={}
for l in open('analysis/gold/matches.txt'):
    m=re.match(r'^1([0-9a-f]+):([0-9a-f]+) n=\s*\d+ -> (\S+)',l)
    if m:
        para=int(m.group(1),16); off=int(m.group(2),16)
        cand.setdefault(m.group(3),[]).append((para,off))

out=[]
names=sys.argv[1:]
if not names:
    names=[l.split()[0] for l in open('/tmp/difflist.txt') if l.strip()]
for nm in names:
    if nm not in mine or nm not in cand: continue
    mp,ma,mb=mine[nm]
    md=disasm(mex,OLOAD,mp,ma,mb)
    best=None
    for para,off in cand[nm]:
        gf=None
        for gn,(gp,ga,gb) in gold.items():
            if gp==para and ga<=off<=gb: gf=(gn,ga,gb); break
        if not gf: continue
        gd=disasm(gex,GLOAD,para,gf[1],gf[2])
        d=ndiff(gd,md)
        if best is None or d<best[0]:
            best=(d,'%04x:%04x'%(para,gf[1]),gf[0],len(gd),len(md))
    if best: out.append((best[0],nm,best))
out.sort()
for d,nm,b in out: print('%-30s best %-9s %s d=%d len=%d/%d'%(nm,b[1],b[2],d,b[3],b[4]))
print('%d routines evaluated'%len(out), file=sys.stderr)
