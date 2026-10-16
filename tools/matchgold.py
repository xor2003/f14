#!/usr/bin/env python3
# matchgold.py — match functions in the retail gold F14.EXE (no symbols)
# against the named dev snapshot (src/F14.EXE CodeView publics) by
# instruction-sequence similarity.
#
# Both sides were analyzed by Ghidra; ranges.txt gives per-function
# instruction extents (linear addresses, Ghidra base para 0x1000).
# Each function is disassembled (ndisasm -b16) on its instruction ranges
# only (embedded data excluded). Similarity = mnemonic-multiset Jaccard,
# confirmed by difflib LCS ratio on the mnemonic stream.
#
# usage: matchgold.py GOLD.EXE gold_ranges.txt DEV.EXE dev_ranges.txt cvpubs.txt

import re
import subprocess
import sys
import difflib
import collections


def load_image(path):
    d = open(path, 'rb').read()
    hdr = int.from_bytes(d[8:10], 'little') * 16
    pages = int.from_bytes(d[4:6], 'little')
    lastp = int.from_bytes(d[2:4], 'little')
    return d[hdr:(pages - 1) * 512 + (lastp or 512)]


def load_ranges(path):
    """seg:off linA-linB ... -> [(entry_seg, entry_off, [(linA,linB)...])]"""
    out = []
    for l in open(path):
        p = l.split()
        if not p:
            continue
        seg, off = p[0].split(':')
        out.append((int(seg, 16), int(off, 16),
                    [(int(a, 16), int(b, 16)) for a, b in
                     (x.split('-') for x in p[1:])]))
    return out


def load_pubs(path):
    pubs = {}
    for l in open(path):
        m = re.match(r'([0-9a-f]{4}):([0-9a-f]{4}) (\S+)', l)
        if m:
            pubs[(int(m.group(1), 16) + 0x1000, int(m.group(2), 16))] = \
                m.group(3)
    return pubs


GHIDRA_BASE = 0x1000 << 4          # para base as linear


def instrs(img, entry, blocks):
    """disassemble the function's instruction ranges -> [mnemonics]"""
    data = b''
    for a, b in blocks:
        data += img[a - GHIDRA_BASE:b - GHIDRA_BASE]
    p = subprocess.run(['ndisasm', '-b16', '-o0', '-'],
                       input=data, capture_output=True)
    mns = []
    for x in p.stdout.decode('latin1').splitlines():
        f = x.split(None, 2)
        if len(f) == 3:
            mns.append(f[2].split()[0])
    return mns


def hist(mns):
    return collections.Counter(mns)


def jaccard(a, b):
    inter = sum((a & b).values())
    union = sum((a | b).values())
    return inter / union if union else 0.0


def main():
    imgG = load_image(sys.argv[1])
    rg = load_ranges(sys.argv[2])
    imgD = load_image(sys.argv[3])
    rd = load_ranges(sys.argv[4])
    pubs = load_pubs(sys.argv[5])

    dev = []
    for seg, off, blocks in rd:
        m = instrs(imgD, off, blocks)
        if not m:
            continue
        dev.append({'key': (seg, off), 'name': pubs.get((seg, off)),
                    'mns': m, 'h': hist(m)})
    gold = []
    for seg, off, blocks in rg:
        m = instrs(imgG, off, blocks)
        if not m:
            continue
        gold.append({'key': (seg, off), 'mns': m, 'h': hist(m)})
    sys.stderr.write(f'dev={len(dev)} gold={len(gold)}\n')

    named = [d for d in dev if d['name']]
    results = []
    for g in gold:
        n = len(g['mns'])
        best = None
        bestj = 0.0
        for d in dev:
            r = len(d['mns']) / n
            if r < 0.4 or r > 2.5:
                continue
            j = jaccard(g['h'], d['h'])
            if j > bestj:
                bestj = j
                best = d
        if best is None or bestj < 0.45:
            results.append((g, None, 0.0, 0.0))
            continue
        # LCS-confirm against the few top candidates
        cands = sorted(
            (d for d in dev
             if 0.4 <= len(d['mns']) / n <= 2.5 and
             jaccard(g['h'], d['h']) >= bestj * 0.9),
            key=lambda d: -jaccard(g['h'], d['h']))[:6]
        bl, bs = None, 0.0
        for d in cands:
            r = difflib.SequenceMatcher(None, g['mns'], d['mns'],
                                        autojunk=False).ratio()
            if r > bs:
                bs, bl = r, d
        results.append((g, bl, jaccard(g['h'], bl['h']) if bl else 0.0, bs))

    n_named = n_exact = 0
    out = open('/tmp/f14gold/matches.txt', 'w')
    for g, d, j, r in sorted(results, key=lambda t: -t[3]):
        gs, go = g['key']
        line = f'{gs:04x}:{go:04x} n={len(g["mns"]):4} '
        if d:
            ds, do = d['key']
            nm = d['name'] or 'FUN_%04x_%04x' % (ds, do)
            line += f'-> {nm:35} dev@{ds - 0x1000:04x}:{do:04x} jac={j:.2f} lcs={r:.2f}'
            if d['name']:
                n_named += 1
                if r > 0.97:
                    n_exact += 1
        else:
            line += '-> (none)'
        out.write(line + '\n')
    out.close()
    print(f'gold funcs={len(gold)} matched_named={n_named} strong(>.97)={n_exact}')


if __name__ == '__main__':
    main()
