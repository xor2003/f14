#!/usr/bin/env python3
# padsweep.py — sweep decl-pad counts at top of a module; for each count,
# relink and triage ALL the module's diffing functions.  Reports the
# count that maximizes matches.  usage: padsweep.py MODULE [maxpad] [--pos FUNCNAME]
import re, subprocess, sys, os

mod = sys.argv[1]
maxpad = 40
posfn = None
for a in sys.argv[2:]:
    if a.isdigit(): maxpad = int(a)
    elif a == '--pos': pass
args = sys.argv[2:]
if '--pos' in args:
    posfn = args[args.index('--pos') + 1].lstrip('_')
    args.remove('--pos'); args.remove(posfn)
    maxpad = int(args[0]) if args and args[0].isdigit() else maxpad

path = 'src/%s.C' % mod
base = open(path).read()

seg = {}
for l in open('build/f14_named.map'):
    m = re.match(r'(_\S+): (\S+) FAR', l)
    if m: seg[m.group(1)] = m.group(2)
fns = [re.match(r'=== (_\S+)', l).group(1)
       for l in open('analysis/firstdiffs.txt') if l.startswith('=== ')]
fns = [f for f in fns if seg.get(f) == mod + '_TEXT']
if not fns:
    sys.exit('no diffing fns mapped to %s_TEXT' % mod)
print('module %s: %d diffs: %s' % (mod, len(fns), ' '.join(fns)))

if posfn:
    m = re.search(r'(^|\n)[\w\s\*]*\b%s\s*\(' % re.escape(posfn), base)
    ins = m.start() + (1 if m.group(0).startswith('\n') else 0)
else:
    mi = [m.end() for m in re.finditer(r'#include[^\n]*\n', base)]
    ins = mi[-1] if mi else 0

PAD = 'extern int _pd%d;\n'
status0 = {}
for n in range(0, maxpad + 1):
    open(path, 'w').write(base[:ins] + ''.join(PAD % k for k in range(n)) + base[ins:])
    subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
    res = {}
    for fn in fns:
        t = subprocess.run(['python3', 'tools/triage_fn.py', fn],
                           capture_output=True, text=True).stdout
        m = re.search(r'=== \S+ (MATCH|DIFF)', t)
        res[fn] = m.group(1) if m else 'ERR'
    if n == 0: status0 = dict(res)
    nm = sum(1 for v in res.values() if v == 'MATCH')
    gained = [f for f, v in res.items() if v == 'MATCH' and status0.get(f) == 'DIFF']
    lost = [f for f, v in res.items() if v == 'DIFF' and status0.get(f) == 'MATCH']
    print('pad=%2d  match=%2d/%d  +%s  -%s' % (n, nm, len(fns), ','.join(gained), ','.join(lost)), flush=True)
open(path, 'w').write(base)
subprocess.run(['bash', 'tools/relink_check.sh', mod], capture_output=True)
