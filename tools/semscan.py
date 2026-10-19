#!/usr/bin/env python3
# semscan.py — bulk semantic-diff classifier over verify_build pairings.
# Aligns gold-vs-cand mnemonic streams with difflib (survives insert/delete),
# then classifies each differing aligned pair:
#   REGALLOC  same shape modulo register names
#   ORDSWAP   commutative operand order
#   POL       jcc polarity flip (same target)
#   FRAME     prologue/epilogue placement
#   LAYOUT    identical after imm/addr normalization (data-relocs, targets)
#   REGVS     reg-in-one/mem-in-other residency choice
#   REAL      different mnemonic/constant class or extra-missing insns
# Usage: semscan.py [--verbose NAME] [--names f,f,f]
import re, subprocess, sys, tempfile, os, difflib

GOLD='/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'
OURS='build/obj/F14.EXE'
ROOT=os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def load_goldmap(path):
    seg={}; fun={}
    for l in open(path):
        m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
        if m: seg[m.group(1)]=int(m.group(2),16); continue
        m=re.match(r'(\S+): (\S+) FAR (.+)$',l.strip())
        if m:
            st=int(m.group(3).split(' ')[0].split('-')[0],16)
            fun[(seg[m.group(2)],st)]=(m.group(2),m.group(3))
    return fun

def load_names(path):
    out={}
    for l in open(path):
        m=re.match(r'^(\S+):\s+\S+\s+FAR\s+([0-9a-f]+)-([0-9a-f]+)',l)
        if m: out[m.group(1)]=(int(m.group(2),16),int(m.group(3),16))
    return out

def load_candseg(path):
    # name -> image para of its segment in MY exe
    segs={}; out={}
    for l in open(path):
        m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
        if m: segs[m.group(1)]=int(m.group(2),16); continue
        m=re.match(r'(\S+): (\S+) FAR ([0-9a-f]+)-([0-9a-f]+)',l)
        if m: out[m.group(1)]=(segs[m.group(2)],int(m.group(3),16),int(m.group(4),16))
    return out

def build_jobs():
    """replicate verify_build pairing: aligned > classified > matches"""
    gold=load_goldmap(ROOT+'/analysis/gold/gold.map')
    gstarts={}
    for (p,o) in gold: gstarts.setdefault(p,[]).append(o)
    for p in gstarts: gstarts[p].sort()
    names=set(load_names(ROOT+'/build/f14_named.map'))
    jobs={}
    def add(nm,p,o,src):
        if nm not in names or nm in jobs: return
        ent=gold.get((p,o))
        if ent:
            ext=ent[1].split(' ')[0].split('-')
            jobs[nm]=(p,int(ext[0],16),int(ext[1],16)); return
        nxt=next((x for x in gstarts.get(p,[]) if x>o),None)
        if nxt is None or nxt-o>0x8000: return
        jobs[nm]=(p,o,nxt)
    cand=ROOT+'/analysis/gold/aligned_anchors.tsv'
    if os.path.exists(cand):
        for l in open(cand):
            f=l.rstrip('\n').split('\t')
            if len(f)<3: continue
            v=f[2].lstrip('@')
            if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}',v):
                s,o=(int(x,16) for x in v.split(':')); add(f[0],s,o,'aln')
    hdr=None
    for l in open(ROOT+'/analysis/gold/classified.tsv'):
        f=l.rstrip('\n').split('\t')
        if hdr is None: hdr=f; continue
        r=dict(zip(hdr,f))
        for col in ('anchor','deepaddr','bestaddr','cand'):
            v=r.get(col,'').lstrip('@')
            if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}',v):
                s,o=(int(x,16) for x in v.split(':')); add(r['name'],s,o,'cls')
                break
    for fn in ('matches2.txt','matches_my.txt'):
        for l in open(ROOT+'/analysis/gold/'+fn):
            m=re.match(r'([0-9a-f]{4}):([0-9a-f]{4})\s+n=\s*\d+\s+->\s+(\S+)',l)
            if m: add(m.group(3),int(m.group(1),16)-0x1000,int(m.group(2),16),'mch')
    return jobs

def exe_img(exe):
    import struct
    return exe[struct.unpack('<H',exe[8:10])[0]*16:]

_tmp=[]
def dis(img,p,a,b):
    d=img[p*16+a:p*16+b+1]
    t=tempfile.NamedTemporaryFile(delete=False);t.write(d);t.close();_tmp.append(t.name)
    o=subprocess.run(['ndisasm','-b16','-o0x%x'%a,t.name],capture_output=True,text=True).stdout
    return [l.split(None,2)[2].strip() for l in o.splitlines() if len(l.split(None,2))>2]

IMM=re.compile(r'(?<![0-9a-f])-?0x[0-9a-f]+')
REGS={'ax','bx','cx','dx','si','di'}
POL={'jg':'jl','jng':'jnl','jl':'jg','jnl':'jng','jge':'jle','jle':'jge',
     'ja':'jb','jb':'ja','jae':'jbe','jbe':'jae','jc':'jb','jnc':'jae',
     'jo':'jno','jno':'jo','js':'jns','jns':'js','jp':'jnp','jnp':'jp','jz':'jnz','jnz':'jz','je':'jne','jne':'je'}

def ops(i):
    p=i.split(None,1)
    return (p[0],[o.strip() for o in p[1].split(',')] if len(p)>1 else [])

def shape(i):
    m,o=ops(i)
    return (m,tuple('R' if x in REGS else re.sub(r'\b(ax|bx|cx|dx|si|di)\b','R',x) for x in o))

def mkey(i):
    m,o=ops(i)
    o=[re.sub(r'\b(ax|bx|cx|dx|si|di)\b','R',x) for x in o]
    o=[re.sub(r'\[.*?\]','M',x) for x in o]
    return (m,tuple(o))

def norm(i):
    i=IMM.sub('N',i)
    i=re.sub(r'\b(call|jmp|j[a-z]+) (?:0x[0-9a-f]+:)?N',r'\1 T',i)
    return i

def classify(g,m):
    gm,go=ops(g); mm,mo=ops(m)
    if g==m: return 'EQ'
    if {g,m}=={'pop bp','leave'} or {g,m}=={'leave','pop bp'}: return 'FRAME'
    if gm==mm and len(go)==2 and len(mo)==2 and go[0]==mo[1] and go[1]==mo[0]: return 'ORDSWAP'
    if gm in POL and gm==POL.get(mm,mm): return 'POL'
    if shape(g)==shape(m): return 'REGALLOC'
    if norm(g)==norm(m): return 'LAYOUT'
    if mkey(g)==mkey(m): return 'REGVS'
    return 'REAL'

def diff_func(gd,md):
    gmn=[ops(i)[0] for i in gd]; mmn=[ops(i)[0] for i in md]
    sm=difflib.SequenceMatcher(a=gmn,b=mmn,autojunk=False)
    counts={}; real=[]
    for tag,i1,i2,j1,j2 in sm.get_opcodes():
        if tag=='equal':
            for gi,mi in zip(range(i1,i2),range(j1,j2)):
                k=classify(gd[gi],md[mi])
                counts[k]=counts.get(k,0)+1
                if k=='REAL': real.append((gi,mi,gd[gi],md[mi]))
        else:
            n=max(i2-i1,j2-j1)
            counts['HUNK']=counts.get('HUNK',0)+n
            for gi in range(i1,min(i2,i1+2)): real.append((gi,-1,gd[gi],'<->'))
            for mi in range(j1,min(j2,j1+2)): real.append((-1,mi,'<->',md[mi]))
    return counts,real

def main():
    verbose=None; only=None
    args=sys.argv[1:]
    if '--verbose' in args: verbose=args[args.index('--verbose')+1]
    if '--names' in args: only=set(args[args.index('--names')+1].split(','))
    jobs=build_jobs()
    mine=load_candseg(ROOT+'/build/f14_named.map')
    gi=exe_img(open(GOLD,'rb').read()); mi=exe_img(open(OURS,'rb').read())
    from collections import Counter
    tally=Counter(); rows=[]
    for nm in sorted(jobs):
        if only and nm not in only: continue
        gp,ga,ge=jobs[nm]
        if nm not in mine: continue
        mp,ma,mb=mine[nm]
        if ge-ga>0x4000 or mb-ma>0x4000: continue
        try:
            gd=dis(gi,gp,ga,ge); md=dis(mi,mp,ma,mb)
        except Exception: continue
        c,real=diff_func(gd,md)
        nr=c.get('REAL',0); nh=c.get('HUNK',0)
        v='MATCH' if sum(c.values())==0 else ('REAL' if nr or nh>4 else 'ART')
        tally[v]+=1; rows.append((v,nm,c))
        if verbose==nm:
            for a,b,g,m in real[:50]: print('%3d/%-3d -%s +%s'%(a,b,g,m))
    for v,nm,c in rows:
        print('%-6s %-32s %s'%(v,nm,' '.join('%s=%d'%(k,c[k]) for k in sorted(c))))
    print(tally)
    for t in _tmp: os.unlink(t)

if __name__=='__main__': main()
