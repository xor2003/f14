#!/usr/bin/env python3
# fdiff.py NAME... — per-function mzdiff gold-vs-build using the same
# pairing logic as verify_build.py (aligned_anchors > classified > matches).
import os, re, subprocess, sys, tempfile

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
GOLD = '/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'
GOLDMAP = ROOT + '/analysis/gold/gold.map'
CAND = ROOT + '/build/obj/F14.EXE'
CANDMAP = ROOT + '/build/f14_named.map'
MZDIFF = os.path.expanduser('~/mzretools/build/mzdiff')


def load_goldmap(path):
    seg, fun = {}, {}
    for l in open(path):
        m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
        if m:
            seg[m.group(1)] = int(m.group(2), 16)
            cur = m.group(1)
            continue
        m = re.match(r'(\S+): (\S+) FAR (.+)$', l.strip())
        if m:
            start = int(m.group(3).split(' ')[0].split('-')[0], 16)
            fun[(seg[m.group(2)], start)] = (m.group(2), m.group(3))
    return fun


def main():
    gold = load_goldmap(GOLDMAP)
    gstarts = {}
    for (para, off) in gold:
        gstarts.setdefault(para, []).append(off)
    for p in gstarts:
        gstarts[p].sort()
    names = {l.split(':')[0] for l in open(CANDMAP)
             if re.match(r'^\S+:\s+\S+\s+FAR\s', l)}

    jobs = {}

    def add(mname, para, off, src):
        if mname not in names or mname in jobs:
            return
        ent = gold.get((para, off))
        if ent:
            jobs[mname] = (para, off, ent[0], ent[1], src)
            return
        nxt = next((o for o in gstarts.get(para, []) if o > off), None)
        if nxt is None or nxt - off > 0x8000:
            return
        jobs[mname] = (para, off, 'CAND',
                       '%04x-%04x R%04x-%04x' % (off, nxt, off, nxt), src)

    for l in open(ROOT + '/analysis/gold/aligned_anchors.tsv'):
        f = l.rstrip('\n').split('\t')
        if len(f) < 3:
            continue
        v = f[2].lstrip('@')
        if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}', v):
            s, o = (int(x, 16) for x in v.split(':'))
            add(f[0], s, o, 'aln')
    hdr = None
    for l in open(ROOT + '/analysis/gold/classified.tsv'):
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
    for l in open(ROOT + '/analysis/gold/matches.txt'):
        m = re.match(r'([0-9a-f]{4}):([0-9a-f]{4})\s+n=\s*\d+\s+->\s+(\S+)', l)
        if m:
            add(m.group(3), int(m.group(1), 16) - 0x1000,
                int(m.group(2), 16), 'mch')

    tmp = tempfile.mkdtemp(prefix='fd_')
    for name in sys.argv[1:]:
        if name not in jobs:
            print('%s: no pairing' % name)
            continue
        para, off, cseg, ext, src = jobs[name]
        mp = os.path.join(tmp, 'c_%s.map' % re.sub(r'\W', '_', name))
        open(mp, 'w').write(
            'Size 6173e\nDGROUP DATA 50b1\n%s CODE %04x\n%s: %s FAR %s\n'
            % (cseg, para, name, cseg, ext))
        r = subprocess.run(
            [MZDIFF, '--loose', '--nocall', '--map', mp, '--tmap', CANDMAP,
             '%s:%s' % (GOLD, name), '%s:%s' % (CAND, name)],
            capture_output=True, timeout=120)
        out = r.stdout.decode('latin1') + r.stderr.decode('latin1')
        verdict = 'MATCH' if re.search(r':\s*match\b', out) else 'DIFF'
        print('##### %s %s @%04x:%04x via %s' % (name, verdict, para, off, src))
        for l in out.splitlines():
            if '!=' in l or 'ERROR' in l or 'Context' in l:
                print(l)
        print()


if __name__ == '__main__':
    main()
