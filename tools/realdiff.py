#!/usr/bin/env python3
# realdiff.py — for each DIFF line in a verify_build log, extract gold+ours
# routine bytes, ndisasm, normalize relocatable operands, report real diffs.
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
            ext=m.group(3).split(' ')[0]
            a,b=(int(x,16) for x in ext.split('-'))
            funs[m.group(1)]=(segs[m.group(2)],a,b)
    return funs

gold=loadmap('analysis/gold/gold.map')
mine=loadmap('build/f14_named.map')

def disasm(exe,load,para,a,b):
    off=load+para*16+a
    data=open(exe,'rb').read()[off:off+b-a+1]
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

log=sys.argv[1] if len(sys.argv)>1 else 'build/verify_gmain3.log'
# map gold-image (para:off) -> FUN name via gold load
def goldfun(para,off):
    best=None
    for n,(p,a,b) in gold.items():
        if p==para and a<=off<=b: return n
    return None

real=[]
for l in open(log):
    m=re.match(r'(\S+)\s+DIFF\s+@([0-9a-f]+):([0-9a-f]+)',l)
    if not m: continue
    nm,para,off=m.group(1),int(m.group(2),16),int(m.group(3),16)
    gf=goldfun(para,off)
    if not gf or nm not in mine: continue
    gp,ga,gb=gold[gf]; mp,ma,mine_b=mine[nm]
    g=disasm(GOLD,GLOAD,gp,ga,gb)
    o=disasm(OURS,OLOAD,mp,ma,mine_b)
    if g!=o:
        # count first few differing insns
        d=[(i,x,y) for i,(x,y) in enumerate(zip(g,o)) if x!=y]
        extra=len(g)-len(o)
        real.append((nm,gf,len(g),len(o),d[:4],extra))
print('%d real-diff routines'%len(real))
for nm,gf,lg,lo,d,extra in real:
    print(f'{nm:26} {gf:20} lens {lg}/{lo} extra {extra}')
    for i,x,y in d[:3]: print(f'    [{i}] {x}  !=  {y}')
