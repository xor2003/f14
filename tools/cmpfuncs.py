#!/usr/bin/env python3
# cmpfuncs.py — per-function byte comparison of two MZ executables,
# relocation-aware. Pairs functions by symbol name: original side comes
# from an mzretools map (Ghidra-derived extents with R-blocks), local side
# from the LINK "Publics by Name" table.
#
# usage: cmpfuncs.py ORIG.EXE ORIG.map LOCAL.EXE LOCAL.MAP
#
# Relocation sites are masked on both sides before comparing, so far-call
# segment fields and data-segment references that legitimately differ
# between link layouts do not count as diffs. On mismatch both slices are
# disassembled (ndisasm -b16) and compared instruction-for-instruction;
# if the instruction mnemonics match exactly the pair is reported NEAR.
# Empty disassembly is an error, never a match.

import re
import struct
import subprocess
import sys


def load_exe(path):
    d = open(path, 'rb').read()
    hdr = int.from_bytes(d[8:10], 'little') * 16
    pages = int.from_bytes(d[4:6], 'little')
    lastp = int.from_bytes(d[2:4], 'little')
    imend = (pages - 1) * 512 + (lastp or 512)
    rel = set()
    nrel = int.from_bytes(d[6:8], 'little')
    base = int.from_bytes(d[24:26], 'little') * 16
    for i in range(nrel):
        off, seg = struct.unpack('<HH', d[base + 4 * i:base + 4 * i + 4])
        rel.add(seg * 16 + off)          # image offset of relocated word
    return d[hdr:imend], rel


def load_map(path):
    """mzretools map -> [(name, image_off, [(start,end),...])]. Segment
    declarations carry paragraph bases; R-blocks are seg-relative."""
    segs, rout = {}, []
    for l in open(path, errors='replace'):
        m = re.match(r'(\w+) CODE ([0-9a-f]+)', l)
        if m:
            segs[m.group(1)] = int(m.group(2), 16) * 16
            continue
        m = re.match(r'(\S+): (\w+) (?:NEAR|FAR) [0-9a-f]+-[0-9a-f]+'
                     r'((?: R[0-9a-f]+-[0-9a-f]+)+)', l)
        if m:
            rout.append((m.group(1), segs[m.group(2)],
                         [(int(a, 16), int(b, 16)) for a, b in
                          re.findall(r'R([0-9a-f]+)-([0-9a-f]+)', l)]))
    return rout


def load_pubs(path):
    """LINK .map 'Publics by Name' -> {name: image_off} (para*16+off)."""
    pubs, inpub = {}, False
    for l in open(path, errors='replace'):
        if 'Publics by Name' in l:
            inpub = True
            continue
        if 'Publics by Value' in l:
            break
        m = re.match(r'\s*([0-9A-F]{4}):([0-9A-F]{4})\s+(\S+)', l)
        if inpub and m:
            pubs[m.group(3)] = int(m.group(1), 16) * 16 + int(m.group(2), 16)
    return pubs


def slice_masked(img, rel, img_off, blocks):
    b = bytearray()
    for a, e in blocks:
        b += img[img_off + a:img_off + e + 1]
    for i in range(len(b)):
        acc = 0
        io = -1
        for a, e in blocks:
            if i < acc + (e - a + 1):
                io = img_off + a + (i - acc)
                break
            acc += e - a + 1
        if io in rel or io + 1 in rel:
            b[i] = 0
    return bytes(b)


def mask_flat(img, rel, img_off, size):
    b = bytearray(img[img_off:img_off + size])
    for i in range(len(b)):
        if img_off + i in rel or img_off + i + 1 in rel:
            b[i] = 0
    return bytes(b)


def disasm(bts):
    p = subprocess.run(['ndisasm', '-b16', '-o0', '-'],
                       input=bts, capture_output=True)
    out = [x.split(None, 2)[-1].strip()
           for x in p.stdout.decode('latin1').splitlines() if x.strip()]
    if not out:
        raise RuntimeError('ndisasm produced no output')
    return out


def main():
    imgO, relO = load_exe(sys.argv[1])
    rout = load_map(sys.argv[2])
    imgM, relM = load_exe(sys.argv[3])
    pubs = load_pubs(sys.argv[4])

    n_exact = n_near = n_diff = 0
    for name, img_off, blocks in rout:
        if name not in pubs:
            continue
        om = slice_masked(imgO, relO, img_off, blocks)
        mo = pubs[name]
        tot = sum(e - a + 1 for a, e in blocks)
        mm = mask_flat(imgM, relM, mo, tot)
        if om == mm:
            n_exact += 1
            continue
        try:
            if disasm(om) == disasm(mm):
                n_near += 1
                print(f'{name} NEAR {tot}')
                continue
        except RuntimeError:
            pass
        n_diff += 1
        print(f'{name} DIFF {tot}')
    print(f'EXACT={n_exact} NEAR={n_near} DIFF={n_diff}')


if __name__ == '__main__':
    main()
