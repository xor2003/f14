#!/usr/bin/env python3
# bandsweep.py — measure every module function's match-status vs uniform
# decl-pad shift at top of file.  Output: bands JSON {fn: [n where MATCH]}.
# usage: bandsweep.py MODULE maxpad
import re, subprocess, sys, json

mod = sys.argv[1]
maxpad = int(sys.argv[2]) if len(sys.argv) > 2 else 60
path = 'src/%s.C' % mod
base = open(path).read()

seg = {}
for l in open('build/f14_named.map'):
    m = re.match(r'(_\S+): (\S+) FAR', l)
    if m: seg[m.group(1)] = m.group(2)
modfns = sorted(f for f, s in seg.items() if s == mod + '_TEXT')

mi = [m.end() for m in re.finditer(r'#include[^\n]*\n', base)]
ins = mi[-1] if mi else 0
PAD = 'extern int _pd%d;\n'

bands = {f: [] for f in modfns}
for n in range(0, maxpad + 1):
    open(path, 'w').write(base[:ins] + ''.join(PAD % k for k in range(n)) + base[ins:])
    subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
    hits = []
    for fn in modfns:
        t = subprocess.run(['python3', 'tools/triage_fn.py', fn],
                           capture_output=True, text=True).stdout
        if re.search(r'=== \S+ MATCH', t):
            bands[fn].append(n)
            hits.append(fn)
    print('n=%2d  %2d match' % (n, len(hits)), flush=True)
open(path, 'w').write(base)
subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
json.dump(bands, open('analysis/bands_%s.json' % mod, 'w'), indent=0)
