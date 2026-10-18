#!/usr/bin/env python3
# padfn.py — sweep extern-pad counts inserted right before FUNCTION's
# definition (file scope); report which counts make it MATCH.
# usage: padfn.py MODULE FUNCNAME maxpad
import re, subprocess, sys, tempfile, os

mod, fname = sys.argv[1], sys.argv[2].lstrip('_')
maxpad = int(sys.argv[3]) if len(sys.argv) > 3 else 50
fn = '_' + fname
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
anch = None
hdr = None
for l in open('analysis/gold/classified.tsv'):
    f = l.rstrip('\n').split('\t')
    if hdr is None:
        hdr = f; continue
    r = dict(zip(hdr, f))
    if r['name'] != fn:
        continue
    for col in ('cand', 'bestaddr', 'deepaddr', 'anchor'):
        v = r.get(col, '').lstrip('@')
        if re.fullmatch(r'[0-9a-f]{4}:[0-9a-f]{4}', v):
            s, o = (int(x, 16) for x in v.split(':'))
            if (s, o) in goldmap:
                anch = (s, o); break
    break
if not anch:
    sys.exit('no anchor for ' + fn)
cseg, ext = goldmap[anch]
mp = tempfile.mktemp(prefix='pf_', suffix='.map')
open(mp, 'w').write('Size 6173e\nDGROUP DATA 50b1\n%s CODE %04x\n%s: %s FAR %s\n'
                    % (cseg, anch[0], fn, cseg, ext))

m = re.search(r'(?m)^[\w\s\*]*\b%s\s*\(' % re.escape(fname), base)
if not m:
    sys.exit('fn def not found')
ins = m.start()

PAD = 'extern int _pd%d;\n'
good = []
for n in range(0, maxpad + 1):
    open(path, 'w').write(base[:ins] + ''.join(PAD % k for k in range(n)) + base[ins:])
    subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
    r = subprocess.run([MZDIFF, '--loose', '--nocall', '--map', mp,
                        '--tmap', CANDMAP, '%s:%s' % (GOLD, fn),
                        '%s:%s' % (CAND, fn)], capture_output=True, timeout=120)
    ok = bool(re.search(rb':\s*match\b', r.stdout))
    if ok:
        good.append(n)
    print('%2d %s' % (n, 'MATCH' if ok else '.'), flush=True)
open(path, 'w').write(base)
subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
os.unlink(mp)
print('GOOD:', good)
