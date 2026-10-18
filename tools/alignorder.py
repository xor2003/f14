#!/usr/bin/env python3
# alignorder.py — re-anchor dev functions to gold routines by SEQUENCE
# alignment inside each module's paired gold segment.
#
# Model: the linker emits a module's functions in source order; gold's
# routines in the paired segment are in the same order (modulo unpaired
# statics on either side).  For every dev function we pick the gold routine
# that (a) keeps the alignment monotonic and (b) best matches on length.
#
# usage: alignorder.py > analysis/gold/aligned_anchors.tsv
import re, sys, math, collections

GOLDMAP='analysis/gold/gold.map'
MYMAP='build/f14_named.map'
CLS='analysis/gold/classified.tsv'

# ---- gold: seg -> [(off,len,name)]
seg={}; gold=collections.defaultdict(list)
for l in open(GOLDMAP):
    m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
    if m: cur=m.group(1); seg[cur]=int(m.group(2),16); continue
    m=re.match(r'(\S+): (\S+) FAR (.+)$',l.strip())
    if m:
        s=seg[m.group(2)]
        ext=m.group(3)
        a=int(ext.split(' ')[0].split('-')[0].lstrip('R'),16)
        b=int(ext.split(' ')[-1].split('-')[1],16)
        gold[s].append((a,b-a+1,m.group(1)))
for s in gold: gold[s].sort()

# ---- mine: module -> [(name,off,len)]
mine=collections.defaultdict(list)
msegs={}
for l in open(MYMAP):
    m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
    if m: msegs[m.group(1)]=int(m.group(2),16); continue
    m=re.match(r'(\S+): (\S+) FAR (.+)$',l.strip())
    if m:
        ext=m.group(3)
        a=int(ext.split(' ')[0].split('-')[0].lstrip('R'),16)
        b=int(ext.split(' ')[-1].split('-')[1],16)
        mine[m.group(2)].append((m.group(1),a,b-a+1))
for s in mine: mine[s].sort(key=lambda t:t[1])
seg2mod={v:k for k,v in msegs.items()}

# ---- classified anchors: name -> list of (seg,off) candidates
cand=collections.defaultdict(list)
hdr=None
for l in open(CLS):
    f=l.rstrip('\n').split('\t')
    if hdr is None: hdr=f; continue
    r=dict(zip(hdr,f))
    for col in ('anchor','deepaddr','bestaddr','cand'):
        v=r.get(col,'').lstrip('@')
        if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}',v):
            s,o=(int(x,16) for x in v.split(':'))
            cand[r['name']].append((s,o))

# gold segment for a module = mode of its fns' candidate segments
modgseg={}
for mod,fns in mine.items():
    c=collections.Counter()
    for n,o,l in fns:
        for s,oo in cand.get(n,[]):
            if s in gold: c[s]+=1
    if c: modgseg[mod]=c.most_common(1)[0][0]

def align(mod):
    """DP align my fn seq to gold routine seq. Returns {name:(seg,off,gname,score)}"""
    if mod not in modgseg: return {}
    gs=modgseg[mod]
    G=gold[gs]; M=mine[mod]
    n,m=len(M),len(G)
    NEG=-1e9
    # dp[i][j] = best score aligning M[:i] into G[:j]
    dp=[[NEG]*(m+1) for _ in range(n+1)]
    bk=[[None]*(m+1) for _ in range(n+1)]
    dp[0][0]=0
    for j in range(1,m+1): dp[0][j]=0  # gold routines may be skipped free (statics)
    for i in range(1,n+1):
        for j in range(1,m+1):
            # skip gold routine j (unpaired static/near in gold)
            best=dp[i][j-1]; bk[i][j]=(i,j-1,None)
            # match M[i-1] -> G[j-1]
            mn,mo,ml=M[i-1]; go,gl,gn=G[j-1]
            ls=min(ml,gl)/max(ml,gl,1)
            # prior anchor agreement bonus
            ab=0
            for s,o in cand.get(mn,[]):
                if s==gs and o==go: ab=0.5; break
                if s==gs and abs(o-go)<0x40: ab=max(ab,0.25)
            sc=ls+ab
            v=dp[i-1][j-1]+sc
            if v>best:
                best=v; bk[i][j]=(i-1,j-1,(go,gn,gl,sc))
            # skip my fn (no gold twin / merged)
            v2=dp[i-1][j]-0.05
            if v2>best:
                best=v2; bk[i][j]=(i-1,j,None)
            dp[i][j]=best
    out={}
    i,j=n,m
    while i>0 and j>0:
        step=bk[i][j]
        if step is None:
            j-=1; continue
        pi,pj,act=step
        if act is not None:
            go,gn,gl,sc=act
            out[M[i-1][0]]=(gs,go,gn,gl,sc)
        i,j=pi,pj
    return out

print('name\tmodseg\tgseg:goff\tgoldname\tgoldlen\tscore')
for mod in sorted(mine):
    if mod not in modgseg: continue
    for n,(gs,go,gn,gl,sc) in sorted(align(mod).items(), key=lambda kv:kv[1][1]):
        print('%s\t%s\t%04x:%04x\t%s\t%d\t%.2f'%(n,mod,gs,go,gn,gl,sc))
