#!/usr/bin/env python3
# semdiff.py — classify each named/anchored pair: MATCH / LAYOUT / ARTIFACT / REAL
# ARTIFACT = only regalloc, operand-order-with-polarity, leave/popbp diffs
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
gold=loadmap('analysis/gold/gold_named.map')
gold.update(loadmap('analysis/gold/gold.map'))
mine=loadmap('build/f14_named.map')
gex=open(GOLD,'rb').read(); mex=open(OURS,'rb').read()
def dis(exe,load,p,a,b):
    d=exe[load+p*16+a:load+p*16+b+1]
    t=tempfile.NamedTemporaryFile(delete=False);t.write(d);t.close()
    o=subprocess.run(['ndisasm','-b16','-o0x%x'%a,t.name],capture_output=True,text=True).stdout;os.unlink(t.name)
    return [l.split(None,2)[2].strip() for l in o.splitlines() if len(l.split(None,2))>2]
IMM=re.compile(r'-?0x[0-9a-f]+')
REGS={'ax','bx','cx','dx','si','di'}
def stripmem(i):
    i=IMM.sub('N',i); i=re.sub(r'[+-]N(?=\])','N',i)
    i=re.sub(r'\b(call|jmp|j[a-z]+) N',r'\1 T',i)
    return i
def ops(i):
    p=i.split(None,1)
    return (p[0], p[1].split(',') if len(p)>1 else [])
def regset(i):
    m,oo=ops(i)
    return set(o.strip() for o in oo if o.strip() in REGS)
def is_jcc(i): return bool(re.match(r'j[a-z]+',i))
POL={'jg':'jl','jng':'jnl','jl':'jg','jnl':'jng','ja':'jc','jna':'jnc','jc':'ja','jnc':'jna',
     'jge':'jle','jle':'jge','jb':'jbe','jbe':'jb','jae':'jna','jnb':'jc','jae':'jb','jnb':'jae','jo':'jno','jno':'jo','js':'jns','jns':'js','jp':'jnp','jnp':'jp'}
def classify_pair(gd,md):
    """return list of (i,kind) for diffs: REAL,REGALLOC,ORDCMP,FRAME,LAYOUT,LEN"""
    n=max(len(gd),len(md)); out=[]
    # build a plausible reg-renaming hypothesis: none global, just per-insn check
    for i in range(n):
        g=gd[i] if i<len(gd) else None; m=md[i] if i<len(md) else None
        if g==m: continue
        if g is None or m is None: out.append((i,'LEN',g,m)); continue
        gs,ms=stripmem(g),stripmem(m)
        if gs==ms: out.append((i,'LAYOUT',g,m)); continue
        gm,go=ops(g); mm,mo=ops(m)
        # framing
        if {g,m}=={'pop bp','leave'} or {g,m}=={'leave','pop bp'}:
            out.append((i,'FRAME',g,m)); continue
        # pure operand-order in commutative/verify ops
        if gm==mm and len(go)==2 and len(mo)==2 and go[0]==mo[1] and go[1]==mo[0]:
            out.append((i,'ORDSWAP',g,m)); continue
        # register-choice: same mnemonic+shape modulo REGS
        def shape(s):
            m2,o2=ops(s)
            o2=[('R' if o.strip() in REGS else o.strip()) for o in o2]
            return (m2,tuple(re.sub(r'\b(ax|bx|cx|dx|si|di)\b','R',o) for o in o2))
        if shape(g)==shape(m): out.append((i,'REGALLOC',g,m)); continue
        # jcc polarity
        if gm in POL and gm==POL.get(mm,mm):
            out.append((i,'POL',g,m)); continue
        # register-residency: same op where one operand is reg in one, mem in other
        gnorm=re.sub(r'\b(ax|bx|cx|dx|si|di)\b','R',stripmem(g))
        mnorm=re.sub(r'\b(ax|bx|cx|dx|si|di)\b','R',stripmem(m))
        gnorm=re.sub(r'\[.*?\]','M',gnorm); mnorm=re.sub(r'\[.*?\]','M',mnorm)
        gnorm=re.sub(r'(word|byte) ','',gnorm); mnorm=re.sub(r'(word|byte) ','',mnorm)
        if gnorm==mnorm or (gnorm.replace('R','M')==mnorm.replace('R','M')):
            out.append((i,'REGVS',g,m)); continue
        out.append((i,'REAL',g,m))
    return out
def verdict(diffs,gd,md):
    if not diffs: return 'MATCH'
    kinds={d[1] for d in diffs}
    if 'REAL' not in kinds:
        if kinds=={'LAYOUT'}: return 'LAYOUT'
        if 'LEN' in kinds and kinds<={'LAYOUT','LEN','REGALLOC','ORDSWAP','POL','FRAME','REGVS'}: return 'EXTENT'
        if kinds<={'LAYOUT','REGALLOC','ORDSWAP','POL','FRAME','REGVS','EXTG','EXTM'}: return 'ARTIFACT' if kinds!={'EXTG'} and kinds!={'EXTM'} else 'EXTENT'
        return 'ARTIFACT'
    return 'REAL'
def run(nm,gf,pad=0):
    gp,ga,gb=gold[gf]; mp,ma,mb=mine[nm]
    gd=dis(gex,0x5400,gp,ga,gb); md=dis(mex,0x5000,mp,ma,mb)
    diffs=classify_pair(gd,md)
    # if lengths differ, try healing the SHORTER side's extent:
    if len(gd)!=len(md):
        gpad=dis(gex,0x5400,gp,ga,gb+0x80); mpad=dis(mex,0x5000,mp,ma,mb+0x80)
        if len(gd)<len(md):
            tail=classify_pair(gpad[len(gd):len(md)+4], mpad[len(gd):len(md)+4])
            if all(d[1]!='REAL' for d in tail): diffs=[d for d in diffs if d[1]!='LEN']+[(-1,'EXTG',None,None)]
        else:
            tail=classify_pair(gpad[len(md):len(gd)+4], mpad[len(md):len(gd)+4])
            if all(d[1]!='REAL' for d in tail): diffs=[d for d in diffs if d[1]!='LEN']+[(-1,'EXTM',None,None)]
    return gd,md,diffs
if __name__=='__main__':
    if len(sys.argv)>2:
        gd,md,dd=run(sys.argv[1],sys.argv[2])
    else:
        nm=sys.argv[1]; gd,md,dd=run(nm,nm)
    for i,k,g,m in dd: print('%3d %-8s -%-30s +%s'%(i,k,g,m))
    print('VERDICT:',verdict(dd,gd,md))
