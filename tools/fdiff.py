#!/usr/bin/env python3
# fdiff.py GOLDFUN MYFUNC — side-by-side normalized disasm of one routine pair
import re,subprocess,sys,difflib
segs={l.split()[0]:int(l.split()[2],16) for l in open('build/f14_named.map') if ' CODE ' in l}
gsegs={};gfn={}
for l in open('analysis/gold/gold.map'):
    m=re.match(r'(\S+) CODE ([0-9a-f]+)',l)
    if m: cur=m.group(1);gsegs[cur]=int(m.group(2),16)
    m=re.match(r'(\S+): (\S+) FAR ([0-9a-f]+)-([0-9a-f]+)',l.strip())
    if m: gfn[m.group(1)]=(gsegs[m.group(2)],int(m.group(3),16),int(m.group(4),16))
def moff(nm):
    m=re.search(nm+r': (\S+) FAR ([0-9a-f]+)-([0-9a-f]+)',open('build/f14_named.map').read())
    return segs[m.group(1)],int(m.group(2),16),int(m.group(3),16)
def dis(exe,para,a,b,base):
    raw=open(exe,'rb').read()
    import struct
    load=struct.unpack('<H',raw[8:10])[0]*16
    off=load+para*16+a
    open('/tmp/fd.bin','wb').write(raw[off:off+b-a+1])
    return subprocess.run(['ndisasm','-b16','-o0x%x'%base,'/tmp/fd.bin'],capture_output=True,text=True).stdout
gf,mn=sys.argv[1],sys.argv[2]
gp,ga,gb=gfn[gf]; mp,ma,mb=moff(mn)
g=dis('/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE',gp,ga,gb,ga)
o=dis('build/obj/F14.EXE',mp,ma,mb,ma)
print('GOLD %s %x:%x-%x  OURS %s %x:%x-%x'%(gf,gp,ga,gb,mn,mp,ma,mb))
gg=[l.split(None,2)[2].strip() for l in g.splitlines() if l.strip()]
oo=[l.split(None,2)[2].strip() for l in o.splitlines() if l.strip()]
print('\n'.join(difflib.unified_diff(gg,oo,'GOLD','OURS',lineterm='',n=2)))
