#!/usr/bin/env python3
# namecheck.py — pair gold_named.map names to f14_named.map, normalized compare
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
gold=loadmap('analysis/gold/gold_named.map'); mine=loadmap('build/f14_named.map')
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
common=sorted(set(gold)&set(mine))
res=[]
for nm in common:
    if nm.startswith('FUN_'): continue
    gp,ga,gb=gold[nm]; mp,ma,mb=mine[nm]
    gd=dis(gex,0x5400,gp,ga,gb); md=dis(mex,0x5000,mp,ma,mb)
    d=sum(1 for x,y in zip(gd,md) if norm(x)!=norm(y))+abs(len(gd)-len(md))
    res.append((d,nm,len(gd),len(md),gp,ga,gb,mp,ma,mb))
res.sort()
same=sum(1 for r in res if r[0]==0)
print('common named:',len(res),'identical-insn:',same)
json.dump(res,open('/tmp/namecheck.json','w'))
for r in res:
    if r[0]>0 and r[0]<200:
        print('%-4d %-28s %d/%d  g=%04x:%04x-%04x m=%04x:%04x-%04x'%(r[0],r[1],r[2],r[3],r[4],r[5],r[6],r[7],r[8],r[9]))
