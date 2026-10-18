#!/usr/bin/env python3
import re, subprocess, sys
def load_image(p):
    d=open(p,'rb').read(); return d[int.from_bytes(d[8:10],'little')*16:]
def seq_at(img, lin, n=10):
    out=subprocess.run(['ndisasm','-b16','-o','0x0','/dev/stdin'],
        input=img[lin:lin+0x200],capture_output=True).stdout.decode().splitlines()
    r=[]
    for l in out:
        p=l.split()
        if len(p)>=3 and re.fullmatch(r'[0-9a-fA-F]+',p[1]):
            r.append((bytes.fromhex(p[1]),p[2]))
        if len(r)>=n: break
    return r
msegs={}; mine={}
for l in open('build/f14_named.map'):
    m=re.match(r'(\S+)\s+(?:CODE|DATA)\s+([0-9a-f]+)',l)
    if m: msegs[l.split()[0]]=int(m.group(2),16); continue
    m=re.match(r'(\S+):\s+(\S+)\s+FAR\s+([0-9a-f]+)-([0-9a-f]+)',l)
    if m: mine[m.group(1)]=(msegs.get(m.group(2),0),int(m.group(3),16),int(m.group(4),16))
pubs={}
for l in open('build/obj/F14.MAP'):
    m=re.search(r'\s([0-9A-F]{4}):([0-9A-F]{4})\s+(\S+)',l)
    if m: pubs.setdefault(m.group(3),(int(m.group(1),16),int(m.group(2),16)))
our=load_image('build/obj/F14.EXE'); gold=load_image('/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE')
MODRM_OPS={0xff,0xfe,0xf6,0xf7,0x8f,0x80,0x81,0x82,0x83,0xc6,0xc7,
           0x00,0x01,0x02,0x03,0x08,0x09,0x0a,0x0b,0x10,0x11,0x12,0x13,
           0x18,0x19,0x1a,0x1b,0x20,0x21,0x22,0x23,0x28,0x29,0x2a,0x2b,
           0x30,0x31,0x32,0x33,0x38,0x39,0x3a,0x3b,0x84,0x85,0x86,0x87,
           0x88,0x89,0x8a,0x8b,0x8d,0x62,0x63,0x69,0x6b,0x8c,0x8e,0xc4,0xc5,
           0xf6,0xf7,0xd0,0xd1,0xd2,0xd3,0x26,0x2e,0x36,0x3e,0x64,0x65}
def needle_regex(insns):
    out=b''
    for b,_ in insns:
        if not b: continue
        out+=re.escape(b[:1])
        if len(b)>1 and b[0] in MODRM_OPS:
            out+=re.escape(b[1:2]); out+=b'.'*(len(b)-2)
        else:
            out+=b'.'*(len(b)-1)
    return out
for name in sys.argv[1:]:
    key=name if name.startswith('_') else '_'+name
    if key in mine: sb,off,end=mine[key]; lin0=sb*16+off
    elif key in pubs: lin0=pubs[key][0]*16+pubs[key][1]
    else: print(name,'NOMAP'); continue
    ins=seq_at(our,lin0)
    idx=0
    for i,(b,m) in enumerate(ins):
        if b[0] in (0x55,0x8b,0x57,0x56,0x50,0xc8,0x53,0x54): idx=i+1
        else: break
    body=ins
    nre=needle_regex(body[idx:idx+4])
    if len(nre)<4: nre=needle_regex(body[:4])
    hits=[m.start() for m in re.finditer(nre,gold)][:400]
    best=None
    for h in hits:
        for back in range(0,6):
            st=h-back
            g=seq_at(gold,st)
            gm=[m for _,m in g]
            om=[m for _,m in body]
            s=0
            for a,b in zip(om,gm):
                if a==b: s+=1
                else: break
            if best is None or s>best[0]: best=(s,st)
    if best and best[0]>=6: print('%-28s @%05x prefix=%d'%(name,best[1],best[0]))
    else: print('%-28s nohit hits=%d best=%d'%(name,len(hits),best[0] if best else 0))
