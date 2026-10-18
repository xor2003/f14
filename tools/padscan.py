#!/usr/bin/env python3
# padscan.py — sweep file-scope extern decl-pad counts before a function,
# report which counts flip the first-diff.  The MS C 6 leaf-order pick
# oscillates with symbol-table load; decl pads shift it.
#
# usage: padscan.py MODULE FUNCTION [maxpad] [--top]
#   MODULE e.g. RIOHEAD, FUNCTION e.g. _HeadProject (or int HeadProject src name)
#   --top: pad at top of file (after includes) instead of before the function
import re, subprocess, sys, os

mod, fn = sys.argv[1], sys.argv[2]
maxpad = int(sys.argv[3]) if len(sys.argv) > 3 and sys.argv[3].isdigit() else 40
attop = '--top' in sys.argv
path = 'src/%s.C' % mod
base = open(path).read()

fname = fn.lstrip('_')
# find insertion point
if attop:
    m = re.search(r'\n(?!#)', base)
    ins = m.end() if m else 0
else:
    # before the function's return-type line
    m = re.search(r'(^|\n)[\w\s\*]*\b%s\s*\(' % re.escape(fname), base)
    if not m:
        sys.exit('function %s not found in %s' % (fname, path))
    ins = base.rindex('\n', 0, m.end()) + 1 if m.group(0).startswith('\n') else 0
    ins = m.start() + (1 if m.group(0).startswith('\n') else 0)

PADLINE = 'extern int _pd%d;\n'

def restore():
    open(path, 'w').write(base)

results = {}
for n in list(range(0, maxpad + 1)):
    pad = ''.join(PADLINE % k for k in range(n))
    open(path, 'w').write(base[:ins] + pad + base[ins:])
    r = subprocess.run(['bash', 'tools/relink_check.sh', mod],
                       capture_output=True, text=True)
    t = subprocess.run(['python3', 'tools/triage_fn.py', fn],
                       capture_output=True, text=True).stdout
    m = re.search(r'=== \S+ (MATCH|DIFF)', t)
    st = m.group(1) if m else 'ERR'
    # first mismatch file-offset for depth tracking
    d = re.search(r'at \S+/([0-9a-f]+):', t)
    depth = int(d.group(1), 16) if d else -1
    results[n] = (st, depth)
    print('%3d %s depth=%x' % (n, st, depth), flush=True)
    if st == 'MATCH':
        break
restore()
