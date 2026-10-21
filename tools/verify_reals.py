#!/usr/bin/env python3
import struct, subprocess, re, sys, os

def objparse(d):
    d = open(d, 'rb').read()
    i = 0
    segs = {}
    pubs = {}
    while i + 3 <= len(d):
        t = d[i]
        ln = d[i + 1] | d[i + 2] << 8
        rec = d[i + 3:i + 3 + ln]
        if t == 0xa0 or t == 0xa2:  # LEDATA/LEDATA32
            sidx = rec[0]
            off = struct.unpack('<H', rec[1:3])[0]
            segs.setdefault(sidx, bytearray())
            body = rec[3:]
            if len(segs[sidx]) < off + len(body):
                segs[sidx].extend(b'\0' * (off + len(body) - len(segs[sidx])))
            segs[sidx][off:off + len(body)] = body
        elif t == 0x90:  # PUBDEF
            sidx = rec[1]
            p = 2
            if sidx == 0:
                p += 2
            while p + 4 <= len(rec):
                nl = rec[p]; p += 1
                nm = rec[p:p + nl].decode('latin1', 'replace'); p += nl
                if p + 3 > len(rec):
                    break
                o = rec[p] | rec[p + 1] << 8; p += 2
                ti = rec[p]; p += 1
                if ti & 0x80:
                    p += 1
                if sidx:
                    pubs[nm] = (sidx, o)
        i += 3 + ln
    return segs, pubs

GEX = open('/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE', 'rb').read()
LOAD = struct.unpack('<H', GEX[8:10])[0] * 16
IMG = GEX[LOAD:]

def rawlines(out):
    L = []
    for l in out.splitlines():
        m = re.match(r'[0-9A-F]{8}\s+([0-9A-F]+)\s+(.*)', l)
        if m:
            L.append((m.group(1), m.group(2).strip()))
    return L

GIDX = {}
def build_gidx():
    for l in open('/tmp/gall.asm'):
        m = re.match(r'([0-9A-F]{8})\s+([0-9A-F]+)\s+(.*)', l)
        if m:
            GIDX[int(m.group(1), 16)] = (m.group(2), m.group(3).strip())

def gold_at(addr):
    out = []
    a = addr
    while a in GIDX and len(out) < 2000:
        out.append(GIDX[a])
        a += len(bytes.fromhex(out[-1][0]))
    return out

def norm(s):
    return re.sub(r'0x[0-9a-f]+', '?', s)

OBJC = {}
def obj_fn(mod, fn):
    if mod not in OBJC:
        path = 'build/obj/%s.OBJ' % mod
        OBJC[mod] = objparse(path) if os.path.exists(path) else ({}, {})
    segs, pubs = OBJC[mod]
    if fn not in pubs:
        return []
    sidx, off = pubs[fn]
    code = bytes(segs.get(sidx, b''))
    nxt = min([v[1] for k, v in pubs.items() if v[0] == sidx and v[1] > off]
              or [len(code)])
    blob = code[off:nxt]
    open('/tmp/of.bin', 'wb').write(blob)
    o = subprocess.run(['ndisasm', '-b16', '-o', '0x%x' % off, '/tmp/of.bin'],
                       capture_output=True, text=True).stdout
    return rawlines(o)

def main():
    build_gidx()
    for l in open('/tmp/fn2obj.txt'):
        fn, mod = l.split()
        if mod == '?':
            print('%-32s NOOBJ' % fn)
            continue
        C = obj_fn(mod, fn)
        if not C:
            print('%-32s NODIS %s' % (fn, mod))
            continue
        e = C[0]
        if e[1].startswith('enter '):
            try:
                fs = int(e[1].split()[1].rstrip(','), 16)
            except ValueError:
                fs = -1
            if 0 <= fs <= 0xffff:
                sig = b'\xc8' + struct.pack('<H', fs) + b'\x00'
            else:
                sig = b''.join(bytes.fromhex(h) for h, _ in C[:3])
        else:
            sig = b''.join(bytes.fromhex(h) for h, _ in C[:3])
        hits = []
        st = 0
        while len(hits) <= 1500:
            st = IMG.find(sig, st)
            if st < 0:
                break
            hits.append(st)
            st += 1
        if not hits:
            print('%-32s NOHIT %s' % (fn, mod))
            continue
        best = None
        for h in hits:
            G = gold_at(h)
            if not G:
                continue
            nd = 0
            for (gh, gi), (ch, ci) in zip(G, C):
                if norm(gi) != norm(ci):
                    nd += 1
                    if nd > 60:
                        break
            if best is None or (nd, len(C)) < best[0]:
                best = ((nd, len(C)), h)
        print('%-32s %-12s g@0x%x nd=%d len=%d' %
              (fn, mod, best[1], best[0][0], best[0][1]))

main()
