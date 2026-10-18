#!/usr/bin/env python3
# dseg_pairs.py — harvest gold<->ours DGROUP absolute-operand pairs.
# Runs verbose mzdiff over every pairing (same job setup as verify_build.py)
# and parses aligned instruction lines for [0xNNNN] absolute data refs.
#
# Output: analysis/gold/dseg_pairs.tsv  gold_off \t our_off \t func \t insn
#
# usage: dseg_pairs.py GOLD.EXE gold.map MY.EXE my_named.map classified.tsv
#        matches.txt [-j N] [--diffs-only LOG]
import concurrent.futures as cf
import os, re, subprocess, sys, tempfile

MZDIFF = os.path.expanduser('~/mzretools/build/mzdiff')

def load_goldmap(path):
    seg = {}; fun = {}
    for l in open(path):
        m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
        if m: seg[m.group(1)] = int(m.group(2), 16); continue
        m = re.match(r'(\S+): (\S+) FAR (.+)$', l.strip())
        if m:
            start = int(m.group(3).split(' ')[0].split('-')[0], 16)
            fun[(seg[m.group(2)], start)] = (m.group(2), m.group(3))
    return fun

def load_names(path):
    return {l.split(':')[0] for l in open(path)
            if re.match(r'^\S+:\s+\S+\s+FAR\s', l)}

ABSREF = re.compile(r'\[0x([0-9a-f]{4})\]|\+0x([0-9a-f]{4})\]')
PAIR = re.compile(
    r'([0-9a-f]{4}:[0-9a-f]{4})/[0-9a-f]+: (.+?)\s+([~=!]+)\s+'
    r'[0-9a-f]{4}:[0-9a-f]{4}/[0-9a-f]+: (.+)$')

def run_one(gold_exe, cand_map, name, my_exe, my_map):
    r = subprocess.run(
        [MZDIFF, '--loose', '--nocall', '--verbose', '--ctx', '200',
         '--map', cand_map, '--tmap', my_map,
         f'{gold_exe}:{name}', f'{my_exe}:{name}'],
        capture_output=True, timeout=120)
    out = r.stdout.decode('latin1') + r.stderr.decode('latin1')
    pairs = []
    for l in out.splitlines():
        m = PAIR.search(l)
        if not m:
            continue
        g_abs = [a or b for a, b in ABSREF.findall(m.group(2))]
        o_abs = [a or b for a, b in ABSREF.findall(m.group(4))]
        for ga, oa in zip(g_abs, o_abs):
            pairs.append((int(ga, 16), int(oa, 16), name,
                          m.group(2).strip()[:40]))
    return pairs

def main():
    gold_exe, goldmap_path, my_exe, my_map = sys.argv[1:5]
    tsvs = [a for a in sys.argv[5:] if not a.startswith('-')]
    jobs = {}
    gold = load_goldmap(goldmap_path)
    my_names = load_names(my_map)

    def add(mname, para, off, src):
        if mname not in my_names or mname in jobs:
            return
        ent = gold.get((para, off))
        if not ent:
            return
        jobs[mname] = (para, off, ent[0], ent[1], src)

    for path in tsvs:
        if 'classified' in path:
            hdr = None
            for l in open(path):
                f = l.rstrip('\n').split('\t')
                if hdr is None:
                    hdr = f; continue
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

    tmpdir = tempfile.mkdtemp(prefix='dp_')
    allpairs = []
    with cf.ThreadPoolExecutor(8) as ex:
        futs = {}
        for mname, (para, off, cseg, ext, src) in jobs.items():
            mp = os.path.join(tmpdir, 'c_%s.map' % re.sub(r'\W', '_', mname))
            with open(mp, 'w') as f:
                f.write('Size 6173e\nDGROUP DATA 50b1\n%s CODE %04x\n'
                        '%s: %s FAR %s\n' % (cseg, para, mname, cseg, ext))
            futs[ex.submit(run_one, gold_exe, mp, mname, my_exe, my_map)] = mname
        for fu in cf.as_completed(futs):
            try:
                allpairs.extend(fu.result())
            except Exception:
                pass

    with open('analysis/gold/dseg_pairs.tsv', 'w') as f:
        f.write('gold_off\tour_off\tfunc\tinsn\n')
        for ga, oa, nm, ins in sorted(allpairs):
            f.write('%04x\t%04x\t%s\t%s\n' % (ga, oa, nm, ins))
    print('%d pairings, %d operand pairs' % (len(jobs), len(allpairs)))

if __name__ == '__main__':
    main()
