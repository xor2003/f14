#!/usr/bin/env python3
# repair2.py — for each DIFF routine, score our disasm against EVERY gold
# routine in the same gold segment (para), report best anchor + distance.
import re, subprocess, sys, tempfile, os

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
bypara={}
for n,(p,a,b) in gold.items(): bypara.setdefault(p,[]).append((n,a,b))

def disasm(exe,para,a,b):
    data=exe[bload+para*16+a:bload+para*16+b+1]
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

# DIFF pairings from the verify log: name @para:off
pairs=[]
for l in open(sys.argv[1] if len(sys.argv)>1 else 'build/verify_full.log'):
    m=re.match(r'(\S+)\s+DIFF\s+@([0-9a-f]+):([0-9a-f]+)',l)
    if m: pairs.append((m.group(1),int(m.group(2),16),int(m.group(3),16)))

# disasm cache
cache={}
def gdis(p,a,b):
    k=(p,a,b)
    if k not in cache:
        global bload; bload=GLOAD; cache[k]=disasm(gex,p,a,b)
    return cache[k]

out=[]
for nm,gpara,goff in pairs:
    if nm not in mine: continue
    mp,ma,mb=mine[nm]
    bload=OLOAD; md=disasm(mex,mp,ma,mb)
    cands=bypara.get(gpara,[])
    if not cands: continue
    best=None
    for gn,ga,gb in cands:
        d=ndiff(gdis(gpara,ga,gb),md)
        if best is None or d<best[0]: best=(d,'%04x:%04x'%(gpara,ga),gn,len(cache[(gpara,ga,gb)]),len(md))
    out.append((best[0],nm,best))
out.sort()
for d,nm,b in out: print('%-30s %-9s %-22s d=%d len=%d/%d'%(nm,b[1],b[2],d,b[3],b[4]))
print('%d evaluated'%len(out), file=sys.stderr)
