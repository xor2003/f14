#!/usr/bin/env python3
# padmod.py — sequential pad-fix for a module.  For each diffing function
# (file order), sweep extern-pad counts at its position; among counts that
# make it MATCH, pick the one that maximizes total module matches (so
# already-matching functions below aren't regressed).  Apply and continue.
# usage: padmod.py MODULE [maxpad]
import re, subprocess, sys, tempfile, os
import concurrent.futures as cf

mod = sys.argv[1]
maxpad = int(sys.argv[2]) if len(sys.argv) > 2 else 45
path = 'src/%s.C' % mod
base = open(path).read()
MZDIFF = os.path.expanduser('~/mzretools/build/mzdiff')
GOLD = '/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'
CAND = 'build/obj/F14.EXE'
CANDMAP = 'build/f14_named.map'

goldmap = {}
seg = {}
for l in open('analysis/gold/gold.map'):
    m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
    if m:
        cur = m.group(1); seg[cur] = int(m.group(2), 16); continue
    m = re.match(r'(\S+): (\S+) FAR (.+)$', l.strip())
    if m:
        start = int(m.group(3).split(' ')[0].split('-')[0], 16)
        goldmap[(seg[m.group(2)], start)] = (m.group(2), m.group(3))

anch = {}
hdr = None
for l in open('analysis/gold/classified.tsv'):
    f = l.rstrip('\n').split('\t')
    if hdr is None:
        hdr = f; continue
    r = dict(zip(hdr, f))
    for col in ('cand', 'bestaddr', 'deepaddr', 'anchor'):
        v = r.get(col, '').lstrip('@')
        if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}', v):
            s, o = (int(x, 16) for x in v.split(':'))
            if (s, o) in goldmap:
                anch[r['name']] = (s, o); break

myseg = {}
for l in open(CANDMAP):
    m = re.match(r'(_\S+): (\S+) FAR', l)
    if m: myseg[m.group(1)] = m.group(2)
modfns = sorted(f for f, s in myseg.items() if s == mod + '_TEXT' and f in anch)

maps = {}
for fn in modfns:
    s, o = anch[fn]
    cseg, ext = goldmap[(s, o)]
    mp = tempfile.mktemp(prefix='pm_', suffix='.map')
    open(mp, 'w').write('Size 6173e\nDGROUP DATA 50b1\n%s CODE %04x\n%s: %s FAR %s\n'
                        % (cseg, s, fn, cseg, ext))
    maps[fn] = mp

ex = cf.ThreadPoolExecutor(16)

def verdict(fn):
    r = subprocess.run([MZDIFF, '--loose', '--nocall', '--map', maps[fn],
                        '--tmap', CANDMAP, '%s:%s' % (GOLD, fn),
                        '%s:%s' % (CAND, fn)], capture_output=True, timeout=120)
    return fn, ('MATCH' if re.search(rb':\s*match\b', r.stdout) else 'DIFF')

def module_status():
    return dict(ex.map(verdict, modfns))

def relink():
    subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)

cur = base
def find_def(nm):
    m = re.search(r'(?m)^[\w\s\*]*\b%s\s*\(' % re.escape(nm), cur)
    return m.start() if m else -1

# initial diff set for this module
relink()
st0 = module_status()
_diffs = [f for f, v in st0.items() if v != 'MATCH']
def _pos(f):
    p = find_def(f.lstrip('_'))
    return p if p >= 0 else 1 << 30
diffs = sorted(_diffs, key=_pos)
print('baseline: %d/%d match; diffs: %s' % (sum(1 for v in st0.values() if v == 'MATCH'), len(modfns), ' '.join(diffs)), flush=True)

PAD = 'extern int _pd%d;\n'
applied = []
for target in diffs:
    tname = target.lstrip('_')
    pos = find_def(tname)
    if pos < 0:
        print('%s: def not found, skip' % target); continue
    # current status snapshot for regression scoring
    st_before = module_status()
    best = None
    for n in range(0, maxpad + 1):
        open(path, 'w').write(cur[:pos] + ''.join(PAD % k for k in range(n)) + cur[pos:])
        relink()
        r = subprocess.run([MZDIFF, '--loose', '--nocall', '--map', maps[target],
                            '--tmap', CANDMAP, '%s:%s' % (GOLD, target),
                            '%s:%s' % (CAND, target)], capture_output=True, timeout=120)
        ok = bool(re.search(rb':\s*match\b', r.stdout))
        if not ok:
            continue
        # target matches — score module
        st = module_status()
        score = sum(1 for v in st.values() if v == 'MATCH')
        lost = [f for f, v in st.items() if v != 'MATCH' and st_before.get(f) == 'MATCH']
        print('  %s pad=%d MATCH score=%d lost=%s' % (target, n, score, ','.join(lost)), flush=True)
        if best is None or (score, -n) > best[0]:
            best = ((score, -n), n)
    open(path, 'w').write(cur)
    if best is None:
        print('%s: no pad fixes (non-pad class)' % target, flush=True)
        relink()
        continue
    _, n = best
    cur = cur[:pos] + ''.join(PAD % k for k in range(n)) + cur[pos:]
    open(path, 'w').write(cur)
    relink()
    applied.append((target, n))
    print('%s: applied pad=%d' % (target, n), flush=True)

st = module_status()
print('final: %d/%d match' % (sum(1 for v in st.values() if v == 'MATCH'), len(modfns)))
print('applied:', applied)
open('analysis/padmod_%s.log' % mod, 'w').write(repr(applied) + '\n' + repr(st))
for mp in maps.values():
    os.unlink(mp)
