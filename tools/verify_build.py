#!/usr/bin/env python3
# verify_build.py — per-function mzdiff of a source-built F14.EXE vs retail.
# Each pairing gets a single-routine cand map (mzdiff seeds the whole map,
# so one routine per map).  Extents for the gold side come from gold.map
# (Ghidra routine extents — reliable); the union file's next-start split
# truncated routines and produced false DIFFs.
#
#   classified.tsv cols  anchor/deepaddr/bestaddr/cand : IMAGE para:off
#   matches.txt          gold addrs                    : GHIDRA seg:off
#   gold.map             CODE_n paras                  : IMAGE para
#
# usage: verify_build.py GOLD.EXE gold.map MY.EXE my_named.map
#          classified.tsv matches.txt [-j N]

import concurrent.futures as cf
import os
import re
import subprocess
import sys
import tempfile

MZDIFF = os.path.expanduser('~/mzretools/build/mzdiff')


def load_goldmap(path):
    """(para, off) -> (segname, extent-line 's-e Ra-b Rc-d')."""
    seg = {}
    fun = {}
    for l in open(path):
        m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
        if m:
            cur = m.group(1)
            seg[cur] = int(m.group(2), 16)
            continue
        m = re.match(r'(\S+): (\S+) FAR (.+)$', l.strip())
        if m:
            start = int(m.group(3).split(' ')[0].split('-')[0], 16)
            fun[(seg[m.group(2)], start)] = (m.group(2), m.group(3))
    return fun


def load_names(path):
    return {l.split(':')[0] for l in open(path)
            if re.match(r'^\S+:\s+\S+\s+FAR\s', l)}


def mzdiff_one(gold_exe, cand_map, name, my_exe, my_map):
    r = subprocess.run(
        [MZDIFF, '--loose', '--nocall',
         '--map', cand_map, '--tmap', my_map,
         f'{gold_exe}:{name}', f'{my_exe}:{name}'],
        capture_output=True, timeout=120)
    out = r.stdout.decode('latin1') + r.stderr.decode('latin1')
    return 'MATCH' if re.search(r':\s*match\b', out) else 'DIFF'


def main():
    gold_exe, goldmap_path, my_exe, my_map = sys.argv[1:5]
    tsvs = [a for a in sys.argv[5:] if not a.startswith('-')]
    gold = load_goldmap(goldmap_path)
    # next-start map for fallback extents within a gold segment
    gstarts = {}
    for (para, off) in gold:
        gstarts.setdefault(para, []).append(off)
    for para in gstarts:
        gstarts[para].sort()

    my_names = load_names(my_map)
    tmpdir = tempfile.mkdtemp(prefix='vb_')

    jobs = {}   # mname -> (para, off, extent, src)

    def add(mname, para, off, src):
        if mname not in my_names or mname in jobs:
            return
        ent = gold.get((para, off))
        if ent:
            jobs[mname] = (para, off, ent[0], ent[1], src)
            return
        # fallback: extent to next gold routine start in same segment
        nxt = None
        for o in gstarts.get(para, []):
            if o > off:
                nxt = o
                break
        if nxt is None or nxt - off > 0x8000:
            return
        jobs[mname] = (para, off, 'CAND', '%04x-%04x R%04x-%04x'
                       % (off, nxt, off, nxt), src)

    # aligned_anchors.tsv holds curated extents and wins over the fuzzy
    # classified/matches rows; load it implicitly if it lives beside the map.
    for cand_aln in (os.path.join(os.path.dirname(goldmap_path),
                                  'aligned_anchors.tsv'),):
        if os.path.exists(cand_aln) and cand_aln not in tsvs:
            tsvs.insert(0, cand_aln)

    for path in tsvs:
        if 'aligned' in path:
            for l in open(path):
                f = l.rstrip('\n').split('\t')
                if len(f) < 3:
                    continue
                v = f[2].lstrip('@')
                if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}', v):
                    s, o = (int(x, 16) for x in v.split(':'))
                    add(f[0], s, o, 'aln')
        elif 'classified' in path:
            hdr = None
            for l in open(path):
                f = l.rstrip('\n').split('\t')
                if hdr is None:
                    hdr = f
                    continue
                r = dict(zip(hdr, f))
                for col in ('anchor', 'deepaddr', 'bestaddr', 'cand'):
                    v = r.get(col, '').lstrip('@')
                    if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}', v):
                        s, o = (int(x, 16) for x in v.split(':'))
                        add(r['name'], s, o, 'cls:' + col)
                        break
        elif 'matches' in path:
            for l in open(path):
                m = re.match(r'([0-9a-f]{4}):([0-9a-f]{4})\s+n=\s*\d+'
                             r'\s+->\s+(\S+)', l)
                if m:
                    s, o = int(m.group(1), 16), int(m.group(2), 16)
                    add(m.group(3), s - 0x1000, o, 'mch')

    print('%d pairings' % len(jobs), file=sys.stderr)

    res = {}
    with cf.ThreadPoolExecutor(8) as ex:
        futs = {}
        for mname, (para, off, cseg, ext, src) in jobs.items():
            mp = os.path.join(tmpdir, 'c_%s.map' % re.sub(r'\W', '_', mname))
            with open(mp, 'w') as f:
                f.write('Size 6173e\nDGROUP DATA 50b1\n%s CODE %04x\n'
                        '%s: %s FAR %s\n'
                        % (cseg, para, mname, cseg, ext))
            futs[ex.submit(mzdiff_one, gold_exe, mp, mname,
                           my_exe, my_map)] = (mname, para, off, src)
        for fu in cf.as_completed(futs):
            mname, para, off, src = futs[fu]
            try:
                res[mname] = (para, off, src, fu.result())
            except Exception as e:
                res[mname] = (para, off, src, 'ERR:' + str(e)[:40])

    nm = sum(1 for v in res.values() if v[3] == 'MATCH')
    for mname, (para, off, src, v) in sorted(res.items()):
        print('%-26s %-7s @%04x:%04x via %s' % (mname, v, para, off, src))
    print('\n%d MATCH / %d pairings' % (nm, len(res)))


if __name__ == '__main__':
    main()
