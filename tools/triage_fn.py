#!/usr/bin/env python3
# triage_fn.py — for each named function, run single-routine mzdiff and print
# the first '!=' mismatch lines (compact: gold vs cand), plus the source site
# hint.  usage: triage_fn.py NAME...
import os
import re
import subprocess
import sys
import tempfile

MZDIFF = os.path.expanduser('~/mzretools/build/mzdiff')
GOLD = '/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'
GOLDMAP = 'analysis/gold/gold.map'
CAND = 'build/obj/F14.EXE'
CANDMAP = 'build/f14_named.map'

sys.path.insert(0, 'tools')
from verify_build import load_goldmap

gold = load_goldmap(GOLDMAP)
my_names = {l.split(':')[0] for l in open(CANDMAP)
            if re.match(r'^\S+:\s+\S+\s+FAR\s', l)}
cseg_of = {}
for l in open(CANDMAP):
    m = re.match(r'(\S+): (\S+) FAR', l)
    if m:
        cseg_of[m.group(1)] = m.group(2)

for name in sys.argv[1:]:
    if name not in my_names:
        print('%s: NOT-IN-CANDMAP' % name)
        continue
    # find gold anchor: search gold.map for routine of this name? gold map has
    # FUN names; instead use matches/classified like verify does — quick path:
    # scan all gold entries whose extent we can try. Cheap: use the pairing TSV.
    hit = None
    hdr = None
    for l in open('analysis/gold/classified.tsv'):
        f = l.rstrip('\n').split('\t')
        if hdr is None:
            hdr = f
            continue
        r = dict(zip(hdr, f))
        if r.get('name') != name:
            continue
        for col in ('cand', 'bestaddr', 'deepaddr', 'anchor'):
            v = r.get(col, '').lstrip('@')
            if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}', v):
                s, o = (int(x, 16) for x in v.split(':'))
                if (s, o) in gold:
                    hit = (s, o)
                    break
        break
    if not hit:
        for l in open('analysis/gold/matches.txt'):
            m = re.match(r'([0-9a-f]{4}):([0-9a-f]{4})\s+n=\s*\d+\s+->\s+(\S+)', l)
            if m and m.group(3) == name:
                hit = (int(m.group(1), 16) - 0x1000, int(m.group(2), 16))
                break
    if not hit:
        print('%s: NO-ANCHOR' % name)
        continue
    para, off = hit
    ent = gold.get((para, off))
    if not ent:
        print('%s: NO-GOLD-EXTENT @%04x:%04x' % (name, para, off))
        continue
    cseg, ext = ent
    mp = tempfile.mktemp(prefix='tr_', suffix='.map')
    open(mp, 'w').write('Size 6173e\nDGROUP DATA 50b1\n%s CODE %04x\n'
                        '%s: %s FAR %s\n' % (cseg, para, name, cseg, ext))
    r = subprocess.run([MZDIFF, '--loose', '--nocall', '--map', mp,
                        '--tmap', CANDMAP, '%s:%s' % (GOLD, name),
                        '%s:%s' % (CAND, name)],
                       capture_output=True, timeout=120)
    os.unlink(mp)
    out = r.stdout.decode('latin1')
    neq = [l for l in out.split('\n') if '!=' in l]
    mtc = 'MATCH' if re.search(r':\s*match\b', out) else 'DIFF'
    print('=== %s %s @%04x:%04x' % (name, mtc, para, off))
    for l in neq[:6]:
        print('   %s' % l.strip()[:110])
