#!/usr/bin/env python3
# padcheck.py — for a module, try inner-extern pad counts in one function;
# report target match AND downstream regression count.
# usage: padcheck.py MODULE FUNCTION maxpad
import re, subprocess, sys

mod, fn, maxpad = sys.argv[1], sys.argv[2].lstrip('_'), int(sys.argv[3])
path = 'src/%s.C' % mod
base = open(path).read()

seg = {}
for l in open('build/f14_named.map'):
    m = re.match(r'(_\S+): (\S+) FAR', l)
    if m: seg[m.group(1)] = m.group(2)
modfns = [f for f, s in seg.items() if s == mod + '_TEXT']

# function order in module by map offset? use source order instead
forder = []
for m in re.finditer(r'(?m)^[\w\s\*]*\b(\w+)\s*\([^;]*$', base):
    n = '_' + m.group(1)
    if n in modfns: forder.append(n)
below = forder[forder.index('_' + fn) + 1:] if '_' + fn in forder else []

m = re.search(r'(?m)^([\w\s\*]*\b%s\s*\([^)]*\)\s*\n\{)' % re.escape(fn), base)
if not m:
    sys.exit('fn header not found')
ins = m.end(1)

def triage(f):
    t = subprocess.run(['python3', 'tools/triage_fn.py', f],
                       capture_output=True, text=True).stdout
    mm = re.search(r'=== \S+ (MATCH|DIFF)', t)
    return mm.group(1) if mm else 'ERR'

for n in range(0, maxpad + 1):
    pad = ''.join('\textern int _pd%d;\n' % k for k in range(n))
    open(path, 'w').write(base[:ins] + pad + base[ins:])
    subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
    tstat = triage('_' + fn)
    regr = [f for f in below if triage(f) != 'MATCH']
    print('pad=%2d  target=%s  regress=%d %s' % (n, tstat, len(regr), ' '.join(regr[:8])), flush=True)
open(path, 'w').write(base)
subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
