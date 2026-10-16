#!/usr/bin/env python3
# deeppair_gold.py — open-ended gold-candidate sweep for unpaired dev names.
#
# matchgold.py/verify_gold.py only try the top-8 Jaccard candidates.  For
# large functions the true twin's token histogram can drift far enough to fall
# outside that window while its *mnemonic stream* is still near-identical.
# This script scores every size-plausible gold function (rebuilt extents,
# length within ~2.5x) by difflib ratio on mnemonic sequences and reports the
# best pairing per name.
#
# usage: deeppair_gold.py classified.tsv gold_named.map ranges_gold_full.txt \
#          my_linkmap.map F14GOLD.EXE MY.EXE [names...] > deeppair.tsv
#          (optional env DEEPPAIR_MEMMAP=memmap_gold.txt caps extents at
#           segment ends — recommended, else last-in-seg extents run away)
#
# With no names argument, all names appearing in classified.tsv are swept.
# Output: name <TAB> best_nseq <TAB> @seg:off <TAB> gold_insns <TAB> my_insns

import difflib
import re
import subprocess
import sys

GHIDRA_BASE = 0x10000


def load_image(path):
    """Load module bytes (everything past the MZ header).  Linear address 0 =
    first load byte; gold/Ghidra linear addresses need -GHIDRA_BASE."""
    d = open(path, 'rb').read()
    hdr = int.from_bytes(d[8:10], 'little') * 16
    return d[hdr:]


def segs(mapfile):
    segs = []
    segre = re.compile(r'\s*([0-9a-fA-F]+)H?\s+([0-9a-fA-F]+)H?\s+[0-9a-fA-F]+H?\s+(?:\S+\s+)?(\S+)')
    for l in open(mapfile):
        m = segre.match(l)
        if m:
            segs.append((m.group(3), int(m.group(1), 16), int(m.group(2), 16)))
    return segs


def load_mymap(mapfile):
    rout = {}
    segstarts = {}
    for l in open(mapfile):
        m = re.match(r'(\S+)\s+CODE\s+([0-9a-f]+)\s*$', l)
        if m:
            segstarts[m.group(1)] = int(m.group(2), 16)
            continue
        m = re.match(r'\s*(\S+):\s*(\S+)\s+FAR\s+([0-9a-f]+)-([0-9a-f]+)\s*(.*)', l)
        if m:
            nm, seg, a, b, r = m.groups()
            if seg in segstarts:
                rout[nm] = (seg, int(a, 16), int(b, 16),
                            [(int(x[0], 16), int(x[1], 16))
                             for x in re.findall(r'R([0-9a-f]+)-([0-9a-f]+)', r)])
    return rout, segstarts


def load_gold(mapfile):
    """Returns {name: (segpara, offset)} — segpara in map convention; add
    GHIDRA_BASE>>4 to key into ranges_gold_full.txt / load the image."""
    segtab = {}
    g = {}
    for l in open(mapfile):
        m = re.match(r'(\S+)\s+CODE\s+([0-9a-f]+)\s*$', l)
        if m:
            segtab[m.group(1)] = int(m.group(2), 16)
            continue
        m = re.match(r'\s*(\S+):\s*(\S+)\s+FAR\s+([0-9a-f]+)-([0-9a-f]+)', l)
        if m:
            nm, sg, a, b = m.groups()
            if sg in segtab:
                g[nm] = (segtab[sg], int(a, 16))
    return g


def load_segends(memmapfile):
    """Ghidra memmap: NAME seg:off seg:end size=N -> {ghidra_para: end_off}."""
    ends = {}
    for l in open(memmapfile):
        m = re.match(r'(\S+)\s+([0-9a-f]+):[0-9a-f]+\s+([0-9a-f]+):([0-9a-f]+)', l)
        if m:
            ends[int(m.group(2), 16)] = int(m.group(4), 16)
    return ends


def rebuild_ranges(rangefile, segends=None):
    """gold extents: start -> next function start in segment (LINK-map style).
    Last-in-segment functions cap at the memmap segment end when given."""
    starts = {}
    for l in open(rangefile):
        p = l.split()
        s, o = p[0].split(':')
        s, o = int(s, 16), int(o, 16)
        first = p[1].split('-')
        st = int(first[0], 16)  # ranges are absolute linear addresses
        en = int(first[1], 16)
        starts.setdefault(s, []).append((o, st, en))
    out = {}
    for s, es in starts.items():
        es.sort()
        segcap = s * 16 + segends[s] + 1 if segends and s in segends else None
        for i, (o, st, en) in enumerate(es):
            nxt = es[i + 1][1] if i + 1 < len(es) else st + 0x10000
            hi = max(en, nxt)
            if segcap is not None:
                hi = min(hi, segcap)
            out[(s, o)] = (st, hi)
    return out


def load_cls(path):
    out = []
    for l in open(path):
        f = l.split('\t')
        if f[0] == 'name':
            continue
        out.append(f[0])
    return out


def disasm(img, imgbase, org, start, end, mns):
    """img: bytes of the load image (linear address imgbase .. ).
    org: value for ndisasm -o (linear addr shown in listing; unused beyond).
    start/end: linear addresses of the byte range."""
    p = subprocess.run(['ndisasm', '-b16', '-o0x%x' % (org & 0xffff), '-e0', '-'],
                       input=img[start - imgbase:end - imgbase],
                       capture_output=True)
    for l in p.stdout.decode('latin1').splitlines():
        f = l.split(None, 2)
        if len(f) > 2:
            mns.append(f[2].split()[0])


def main():
    cls_file, gmap, granges, mmap, gold_exe, my_exe = sys.argv[1:7]
    names = sys.argv[7:] or load_cls(cls_file)

    img = load_image(gold_exe)
    myimg = load_image(my_exe)
    gfuncs = load_gold(gmap)
    import os
    segends = (load_segends(os.environ['DEEPPAIR_MEMMAP'])
               if os.environ.get('DEEPPAIR_MEMMAP') else None)
    grebuilt = rebuild_ranges(granges, segends)
    myfuncs, myseg = load_mymap(mmap)

    print('disassembling gold...', file=sys.stderr)
    gmns = {}
    for nm, (segpara, off) in gfuncs.items():
        gpara = segpara + (GHIDRA_BASE >> 4)
        st, en = grebuilt.get((gpara, off),
                            (gpara * 16 + off - GHIDRA_BASE,
                             gpara * 16 + off - GHIDRA_BASE + 0x40))
        mns = []
        disasm(img, GHIDRA_BASE, st, st, min(en, st + 0x10000), mns)
        gmns[nm] = ('@%04x:%04x' % (segpara, off), mns)
    print('gold funcs=%d' % len(gmns), file=sys.stderr)

    for name in names:
        if name not in myfuncs:
            continue
        seg, a, b, blocks = myfuncs[name]
        s0 = myseg.get(seg)
        mm = []
        for x, y in blocks:
            disasm(myimg, 0, s0 * 16 + x, s0 * 16 + x, s0 * 16 + y, mm)
        if not mm:
            continue
        best = (0, None, None)
        for nm, (key, gmn) in gmns.items():
            gl = len(gmn)
            if gl < 3 or not (len(mm) * 0.4 <= gl <= len(mm) * 2.5):
                continue
            r = difflib.SequenceMatcher(None, gmn, mm).ratio()
            if r > best[0]:
                best = (r, key, gl)
        if best[1]:
            print('%-26s nseq=%.2f %s (g=%d m=%d)' % (name, best[0], best[1], best[2], len(mm)))


if __name__ == '__main__':
    main()
