#!/usr/bin/env python3
# verify_gold.py — for each named local-build routine, find the gold retail
# function it actually corresponds to.  Unlike matchgold.py (top-1 fuzzy
# pick), this tries ALL plausible gold candidates and lets mzdiff decide:
# first candidate that verifies under `mzdiff --loose --nocall` wins.
#
# usage: verify_gold.py GOLD.EXE ranges_gold.txt MY.EXE my_named.map \
#          NAMES...        (or --diffs mzdiff_file to take DIFF lines)
#
# Needs: ndisasm, ~/mzretools/build/mzdiff (with the --tmap segment-seed
# patch).

import collections
import os
import re
import subprocess
import sys
import tempfile

MZDIFF = os.path.expanduser('~/mzretools/build/mzdiff')
GHIDRA_BASE = 0x1000 << 4


def load_image(path):
    d = open(path, 'rb').read()
    hdr = int.from_bytes(d[8:10], 'little') * 16
    pages = int.from_bytes(d[4:6], 'little')
    lastp = int.from_bytes(d[2:4], 'little')
    return d[hdr:(pages - 1) * 512 + (lastp or 512)]


def load_ranges(path):
    """seg:off linA-linB ... -> {entry: [(linA,linB),...]}"""
    out = {}
    for l in open(path):
        p = l.split()
        if not p:
            continue
        seg, off = p[0].split(':')
        out[(int(seg, 16), int(off, 16))] = [
            (int(a, 16), int(b, 16))
            for a, b in (x.split('-') for x in p[1:])]
    return out


def load_mymap(path):
    """mzretools map -> {name: (segname, codepara, off_lo, off_hi)}"""
    segs, out = {}, {}
    for l in open(path):
        m = re.match(r'(\w+) CODE ([0-9a-f]+)', l)
        if m:
            segs[m.group(1)] = int(m.group(2), 16)
            continue
        m = re.match(r'(\S+): (\w+) (?:NEAR|FAR) ([0-9a-f]+)-([0-9a-f]+)',
                     l)
        if m:
            out[m.group(1)] = (m.group(2), segs[m.group(2)],
                               int(m.group(3), 16), int(m.group(4), 16))
    return out


def disasm(img, blocks, base=GHIDRA_BASE):
    data = b''.join(img[a - base:b - base] for a, b in blocks)
    if not data:
        return []
    p = subprocess.run(['ndisasm', '-b16', '-o0', '-'],
                       input=data, capture_output=True)
    mns = []
    for x in p.stdout.decode('latin1').splitlines():
        f = x.split(None, 2)
        if len(f) == 3:
            mns.append(f[2].split()[0])
    return mns


def jaccard(a, b):
    inter = sum((a & b).values())
    union = sum((a | b).values())
    return inter / union if union else 0.0


def mzdiff(gold_exe, gold_map_path, name,
           my_exe, my_map_path):
    r = subprocess.run(
        [MZDIFF, '--loose', '--nocall',
         '--map', gold_map_path, '--tmap', my_map_path,
         f'{gold_exe}:{name}', f'{my_exe}:{name}'],
        capture_output=True, timeout=120)
    out = r.stdout.decode('latin1') + r.stderr.decode('latin1')
    return 'MATCH' if re.search(r':\s*match\b', out) else out


def main():
    gold_exe, gold_rng, my_exe, my_map = sys.argv[1:5]
    rest = sys.argv[5:]

    gimg = load_image(gold_exe)
    gr = load_ranges(gold_rng)
    mim = load_image(my_exe)
    mm = load_mymap(my_map)

    names = []
    if rest and rest[0] == '--diffs':
        for l in open(rest[1]):
            if ' DIFF' in l:
                names.append(l.split()[0])
    else:
        names = rest

    print('disassembling gold functions...', file=sys.stderr)
    gfuncs = []
    for (seg, off), blocks in gr.items():
        mns = disasm(gimg, blocks)
        if mns:
            gfuncs.append({'seg': seg, 'off': off, 'blocks': blocks,
                           'h': collections.Counter(mns)})
    print(f'gold funcs with code: {len(gfuncs)}', file=sys.stderr)

    tmpdir = tempfile.mkdtemp(prefix='vg_')
    results = {}
    for name in names:
        if name not in mm:
            results[name] = 'NOMAP'
            continue
        segname, cpara, lo, hi = mm[name]
        mblocks = [(cpara * 16 + lo + GHIDRA_BASE,
                    cpara * 16 + hi + GHIDRA_BASE)]
        mh = collections.Counter(disasm(mim, mblocks))
        n = max(1, sum(mh.values()))

        cands = []
        for g in gfuncs:
            r = sum(g['h'].values()) / n
            if 0.3 <= r <= 3.0:
                cands.append((jaccard(g['h'], mh), g))
        cands.sort(key=lambda t: -t[0])
        cands = [g for j, g in cands[:8] if j >= 0.3]

        verdict = 'NOCAND'
        bestaddr = None
        for g in cands:
            seg, off, blocks = g['seg'], g['off'], g['blocks']
            end = blocks[-1][1] - seg * 16 if blocks else off + 4
            rblk = ' '.join(
                'R%04x-%04x' % (a - seg * 16, b - seg * 16)
                for a, b in blocks)
            mp = os.path.join(tmpdir, 'cand.map')
            with open(mp, 'w') as f:
                f.write('Size 6173e\n')
                f.write('CAND CODE %04x\n' % (seg - 0x1000))
                f.write('%s: CAND FAR %04x-%04x %s\n'
                        % (name, off, end, rblk))
            r = mzdiff(gold_exe, mp, name, my_exe, my_map)
            if r == 'MATCH':
                verdict = 'MATCH'
                bestaddr = '%04x:%04x' % (seg - 0x1000, off)
                break
        results[name] = verdict + ((' @' + bestaddr) if bestaddr else '')
        print('%-22s %s' % (name, results[name]))

    nm = sum(1 for v in results.values() if v.startswith('MATCH'))
    print(f'\nverified: {nm}/{len(results)}')


if __name__ == '__main__':
    main()
