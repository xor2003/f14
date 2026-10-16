#!/usr/bin/env python3
# classify_gold.py — classify verify_gold.py NOCAND results.
#
# For each NOCAND name, recompute the gold-candidate histograms and report:
#   NOCAND-EMPTY      no gold candidate within jaccard>=0.3 / size window
#   NOCAND-CLAIMED    best candidate's gold addr already MATCH-verified for
#                     a different name (extent/boundary artifact or dup code)
#   NOCAND-WEAK       candidates exist but best jaccard < 0.5
#   NOCAND-UNVERIFIED had plausible candidates (j>=0.5) that mzdiff rejected
#                     -> real drift/codegen territory; report best cand addr
#
# usage: classify_gold.py GOLD.EXE ranges_gold.txt MY.EXE my_named.map \
#          verify_sweep.txt > classify.tsv
#
# Only the histogram pass runs here (no mzdiff) so it is fast.

import collections
import os
import re
import subprocess
import sys

GHIDRA_BASE = 0x1000 << 4


def load_image(path):
    d = open(path, 'rb').read()
    hdr = int.from_bytes(d[8:10], 'little') * 16
    pages = int.from_bytes(d[4:6], 'little')
    lastp = int.from_bytes(d[2:4], 'little')
    return d[hdr:(pages - 1) * 512 + (lastp or 512)]


def load_ranges(path):
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
    segs, out = {}, {}
    for l in open(path):
        m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
        if m:
            segs[m.group(1)] = int(m.group(2), 16)
            continue
        m = re.match(r'(\S+): (\S+) (?:NEAR|FAR) ([0-9a-f]+)-([0-9a-f]+)',
                     l)
        if m:
            out[m.group(1)] = (m.group(2), segs[m.group(2)],
                               int(m.group(3), 16), int(m.group(4), 16))
    return out


def disasm(img, blocks, base=GHIDRA_BASE):
    data = b''.join(img[a - base:b - base] for a, b in blocks)
    if not data:
        return [], []
    p = subprocess.run(['ndisasm', '-b16', '-o0', '-'],
                       input=data, capture_output=True)
    mns = []
    calls = []
    seen = {}
    for x in p.stdout.decode('latin1').splitlines():
        f = x.split(None, 2)
        if len(f) == 3:
            text = f[2].strip()
            mn = text.split()[0]
            mns.append(mn)
            if mn == 'call' or mn.startswith('callf'):
                t = text.split()[-1]
                if t not in seen:
                    seen[t] = len(seen)
                calls.append(seen[t])
    return mns, calls


def jaccard(a, b):
    inter = sum((a & b).values())
    union = sum((a | b).values())
    return inter / union if union else 0.0


MZDIFF = os.path.expanduser('~/mzretools/build/mzdiff')


def norm_insn(text):
    """mnemonic + operand shape: numbers->N, seg:ofs->far, keeps regs/[] """
    t = text.strip()
    if not t:
        return t
    mn, _, ops = t.partition(' ')
    ops = re.sub(r'0x[0-9a-f]+:[0-9a-f]+', 'FAR', ops)
    ops = re.sub(r'0x[0-9a-f]+|\b\d+\b', 'N', ops)
    ops = re.sub(r'\s+', '', ops)
    return mn + ' ' + ops


def mzdiff_stats(gold_exe, g, name, my_exe, my_map, tmpdir):
    """One mzdiff against the best candidate; report diff density."""
    seg, off, blocks = g['seg'], g['off'], g['blocks']
    end = blocks[-1][1] - seg * 16 if blocks else off + 4
    rblk = ' '.join('R%04x-%04x' % (a - seg * 16, b - seg * 16)
                    for a, b in blocks)
    mp = os.path.join(tmpdir, 'cand.map')
    with open(mp, 'w') as f:
        f.write('Size 6173e\n')
        f.write('CAND CODE %04x\n' % (seg - 0x1000))
        f.write('%s: CAND FAR %04x-%04x %s\n' % (name, off, end, rblk))
    try:
        r = subprocess.run(
            [MZDIFF, '--loose', '--nocall',
             '--map', mp, '--tmap', my_map,
             f'{gold_exe}:{name}', f'{my_exe}:{name}'],
            capture_output=True, timeout=120)
        out = r.stdout.decode('latin1') + r.stderr.decode('latin1')
    except subprocess.TimeoutExpired:
        return 'mzdiff=TIMEOUT'
    if re.search(r':\s*match\b', out):
        return 'mzdiff=MATCH'
    tot = len([l for l in out.splitlines() if ' == ' in l or ' != ' in l])
    ndiff = len([l for l in out.splitlines() if ' != ' in l])
    return 'mzdiff=DIFF(%d/%d)' % (ndiff, tot)


def main():
    gold_exe, gold_rng, my_exe, my_map, sweep = sys.argv[1:6]
    verify = len(sys.argv) > 6 and sys.argv[6] == '--seq'

    nocands, claimed = [], {}
    for l in open(sweep):
        p = l.split()
        if len(p) < 2:
            continue
        if p[1] == 'NOCAND':
            nocands.append(p[0])
        elif p[1] == 'MATCH' and len(p) >= 3:
            claimed[p[2]] = p[0]          # '@seg:off' -> verified name

    gimg = load_image(gold_exe)
    gr = load_ranges(gold_rng)
    mim = load_image(my_exe)
    mm = load_mymap(my_map)

    print('disassembling gold functions...', file=sys.stderr)
    gfuncs = []
    for (seg, off), blocks in gr.items():
        mns, calls = disasm(gimg, blocks)
        if mns:
            gfuncs.append({'seg': seg, 'off': off, 'blocks': blocks,
                           'h': collections.Counter(mns), 'mns': mns,
                           'calls': calls})
    print(f'gold funcs with code: {len(gfuncs)}', file=sys.stderr)

    hist = collections.Counter()
    for name in nocands:
        if name not in mm:
            print('%-24s NOMAP' % name)
            hist['NOMAP'] += 1
            continue
        segname, cpara, lo, hi = mm[name]
        mblocks = [(cpara * 16 + lo + GHIDRA_BASE,
                    cpara * 16 + hi + GHIDRA_BASE)]
        mns_my, calls_my = disasm(mim, mblocks)
        mh = collections.Counter(mns_my)
        n = max(1, sum(mh.values()))

        cands = []
        for g in gfuncs:
            r = sum(g['h'].values()) / n
            if 0.3 <= r <= 3.0:
                cands.append((jaccard(g['h'], mh), g))
        cands.sort(key=lambda t: -t[0])
        cands = [(j, g) for j, g in cands if j >= 0.3][:8]

        if not cands:
            print('%-24s NOCAND-EMPTY myinsns=%d' % (name, n))
            hist['NOCAND-EMPTY'] += 1
            continue
        j0, g0 = cands[0]
        addr = '@%04x:%04x' % (g0['seg'] - 0x1000, g0['off'])
        extra = ''
        if verify:
            import difflib
            mseq = mns_my
            r = difflib.SequenceMatcher(None, g0['mns'], mseq,
                                        autojunk=False)
            # normalized pass: strip all literals/offsets -> exposes
            # structural drift vs mechanical (relocation/codegen) drift
            gn = [norm_insn(x) for x in g0['mns']]
            mn = [norm_insn(x) for x in mseq]
            r2 = difflib.SequenceMatcher(None, gn, mn, autojunk=False)
            extra = ' seq=%.2f nseq=%.2f glen=%d mlen=%d' % (
                r.ratio(), r2.ratio(), len(g0['mns']), len(mseq))
            # best structural match across all candidates: if a lower-
            # jaccard candidate aligns better, top-1 was mispaired
            bj, bg, bs = j0, g0, r2.ratio()
            for jx, gx in cands[1:]:
                rx = difflib.SequenceMatcher(
                    None, gx['mns'], mseq, autojunk=False).ratio()
                if rx > bs:
                    bj, bg, bs = jx, gx, rx
            baddr = '@%04x:%04x' % (bg['seg'] - 0x1000, bg['off'])
            extra += ' best=%.2f@%s' % (bs, baddr)
            # call-graph shape: callee-identity pattern survives codegen
            # drift, breaks on source edits -> real drift vs /Od noise
            gc, mc = bg['calls'], calls_my
            if gc or mc:
                rc = difflib.SequenceMatcher(
                    None, gc, mc, autojunk=False).ratio()
                extra += ' cseq=%.2f gcall=%d mcall=%d' % (
                    rc, len(gc), len(mc))
        if addr in claimed and claimed[addr] != name:
            print('%-24s NOCAND-CLAIMED j=%.2f %s by=%s%s' %
                  (name, j0, addr, claimed[addr], extra))
            hist['NOCAND-CLAIMED'] += 1
        elif j0 < 0.5:
            print('%-24s NOCAND-WEAK j=%.2f %s ncand=%d%s' %
                  (name, j0, addr, len(cands), extra))
            hist['NOCAND-WEAK'] += 1
        else:
            print('%-24s NOCAND-UNVERIFIED j=%.2f %s ncand=%d%s' %
                  (name, j0, addr, len(cands), extra))
            hist['NOCAND-UNVERIFIED'] += 1

    print('\nsummary:', file=sys.stderr)
    for k, v in hist.most_common():
        print('  %-20s %d' % (k, v), file=sys.stderr)


if __name__ == '__main__':
    main()
