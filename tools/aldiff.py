#!/usr/bin/env python3
# aldiff.py — LCS-aligned semantic diff between a gold routine and ours.
# usage: aldiff.py OURNAME [GOLDNAME|para:off]   prints aligned edit list.
import re, subprocess, sys, tempfile, os
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
def loadof(ex): return int.from_bytes(ex[8:10],'little')*16
def dis(exe,load,p,a,b):
    d=exe[load+p*16+a:load+p*16+b+1]
    t=tempfile.NamedTemporaryFile(delete=False);t.write(d);t.close()
    o=subprocess.run(['ndisasm','-b16','-o0x%x'%a,t.name],capture_output=True,text=True).stdout;os.unlink(t.name)
    out=[]
    for l in o.splitlines():
        p2=l.split(None,2)
        if len(p2)>2: out.append((p2[0].lstrip('0'),p2[2].strip()))
    return out
IMM=re.compile(r'-?0x[0-9a-f]+\b')
def norm(i):
    i=IMM.sub('N',i)
    i=re.sub(r'call N:N','call FAR',i)
    i=re.sub(r'\b(jmp|j[a-z]+|call|loop\w*) N',r'\1 T',i)
    i=re.sub(r'short T','T',i)
    return i
def lcs_align(gd,md):
    n,m=len(gd),len(md)
    G=[norm(x[1]) for x in gd]; M=[norm(x[1]) for x in md]
    dp=[[0]*(m+1) for _ in range(n+1)]
    for i in range(1,n+1):
        gi=G[i-1]; row=dp[i]; prow=dp[i-1]
        for j in range(1,m+1):
            row[j]=prow[j-1]+1 if gi==M[j-1] else (prow[j] if prow[j]>=row[j-1] else row[j-1])
    edits=[]; i,j=n,m
    while i>0 or j>0:
        if i>0 and j>0 and G[i-1]==M[j-1]: i-=1; j-=1; continue
        if i>0 and j>0 and dp[i-1][j-1]==dp[i][j]:
            edits.append(('SUB',i-1,j-1)); i-=1; j-=1; continue
        if i>0 and dp[i-1][j]==dp[i][j]:
            edits.append(('DEL',i-1,j)); i-=1; continue
        if j>0 and dp[i][j-1]==dp[i][j]:
            edits.append(('INS',i,j-1)); j-=1; continue
        edits.append(('SUB',i-1,j-1)); i-=1; j-=1
    edits.reverse()
    return edits
if __name__=='__main__':
    nm=sys.argv[1]; garg=sys.argv[2] if len(sys.argv)>2 else nm
    if garg in gold: gf=garg
    elif ':' in garg:
        gp,go=(int(x,16) for x in garg.split(':'))
        gf=None
        for gn,(p,a,b) in gold.items():
            if p==gp and a<=go<=b: gf=gn; break
    else: gf=garg
    gp,ga,gb=gold[gf]; mp,ma,mb=mine[nm]
    gd=dis(gex,loadof(gex),gp,ga,gb); md=dis(mex,loadof(mex),mp,ma,mb)
    edits=lcs_align(gd,md)
    print('GOLD %s @%04x:%04x-%04x  OUR %s %04x-%04x  gd=%d md=%d edits=%d'
          %(gf,gp,ga,gb,nm,ma,mb,len(gd),len(md),len(edits)))
    for e in edits[:120]:
        if e[0]=='SUB':
            print('  g%-5s -%-34s | m%-5s +%s'%(gd[e[1]][0],gd[e[1]][1],md[e[2]][0],md[e[2]][1]))
        elif e[0]=='DEL':
            print('  g%-5s -%-34s | +++'%(gd[e[1]][0],gd[e[1]][1]))
        else:
            print('  g----- ---- | m%-5s +%s'%(md[e[2]][0],md[e[2]][1]))
