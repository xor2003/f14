#!/usr/bin/env python3
# repair3.py — for each DIFF pairing, compare against every gold routine in
# the same segment. Report best anchor, normalized distance, and classify:
#   LAYOUT  all diffs are [imm]/mem operands shifting by one constant delta
#   CALL    diffs only in call targets
#   REAL    instruction-mnemonic/structure drift
import re, subprocess, sys, tempfile, os, json

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

def disasm(exe,load,para,a,b):
    data=exe[load+para*16+a:load+para*16+b+1]
    tf=tempfile.NamedTemporaryFile(delete=False,suffix='.bin')
    tf.write(data); tf.close()
    out=subprocess.run(['ndisasm','-b16','-o0x%x'%a,tf.name],capture_output=True,text=True).stdout
    os.unlink(tf.name)
    return [l.split(None,2)[2].strip() for l in out.splitlines() if len(l.split(None,2))>2]

IMMP=re.compile(r'0x[0-9a-f]+')
def norm(ins):
    ins=re.sub(r'call 0x[0-9a-f]+:0x[0-9a-f]+','call FAR',ins)
    ins=re.sub(r'\b(call|jmp|j[a-z]+) 0x[0-9a-f]+',r'\1 TGT',ins)
    ins=IMMP.sub('N',ins)
    return ins
def mnem(ins): return ins.split(' ',1)[0] if ins else ins

pairs=[]
for l in open(sys.argv[1] if len(sys.argv)>1 else 'build/verify_full.log'):
    m=re.match(r'(\S+)\s+DIFF\s+@([0-9a-f]+):([0-9a-f]+)',l)
    if m: pairs.append((m.group(1),int(m.group(2),16),int(m.group(3),16)))

gcache={}
def gdis(p,a,b):
    k=(p,a,b)
    if k not in gcache: gcache[k]=disasm(gex,GLOAD,p,a,b)
    return gcache[k]

res=[]
for nm,gpara,goff in pairs:
    if nm not in mine: continue
    mp,ma,mb=mine[nm]
    md=disasm(mex,OLOAD,mp,ma,mb)
    best=None
    for gn,ga,gb in bypara.get(gpara,[]):
        gd=gdis(gpara,ga,gb)
        d=sum(1 for x,y in zip(gd,md) if norm(x)!=norm(y))+abs(len(gd)-len(md))
        if best is None or d<best[0]: best=(d,gn,ga,gb,gd)
    if not best: continue
    d,gn,ga,gb,gd=best
    # classify operand-level detail at this anchor
    diffs=[(i,x,y) for i,(x,y) in enumerate(zip(gd,md)) if x!=y]
    if d==0:
        deltas=set(); allimm=True; allcall=True
        for i,x,y in diffs:
            if mnem(x)!=mnem(y): allimm=allcall=False; break
            xi=IMMP.findall(x); yi=IMMP.findall(y)
            if len(xi)==len(yi) and len(xi)>=1:
                for a,b in zip(xi,yi): deltas.add(int(a,16)-int(b,16))
            else: allimm=False
            if mnem(x) not in ('call','jmp'): allcall=False
        if allcall: cls='CALL'
        elif allimm and len(deltas)<=3: cls='LAYOUT d=%s'%sorted(deltas)
        elif allimm: cls='IMM d=%s'%sorted(deltas)[:40]
        else: cls='IMM-MIX'
    else:
        # partial: any diffs that are pure memref deltas?
        cls='REAL'
    res.append(dict(nm=nm,gf=gn,g='%04x:%04x'%(gpara,ga),d=d,cls=cls,
                    lg=len(gd),lm=len(md),ndiff=len(diffs)))
res.sort(key=lambda r:r['d'])
json.dump(res,open('/tmp/repair3.json','w'),indent=1)
from collections import Counter
print(Counter(r['cls'].split()[0] for r in res))
for r in res:
    if r['d']==0 or r['d']<=6:
        print('%-28s %-20s %-9s d=%d n=%d len=%d/%d'%(r['nm'],r['gf'],r['cls'],r['d'],r['ndiff'],r['lg'],r['lm']))
