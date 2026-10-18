#!/usr/bin/env python3
# bandsolve.py — from bands_<MOD>.json (match-status vs uniform top-pad n),
# compute a nondecreasing cumulative shift S(fn) so every diffing fn lands
# in its band and matching fns stay in theirs.  Emits per-gap pad counts.
#
# Model: shift S at a function = total extern decls added above it in TU.
# Band_f = {n: status(f, uniform-shift n)=MATCH}.  Assume status depends
# only on total shift (verified empirically on CAPAI/LGetTargADP).
#
# usage: bandsolve.py MODULE
import json, re, sys

mod = sys.argv[1]
bands = json.load(open('analysis/bands_%s.json' % mod))
base = open('src/%s.C' % mod).read()

# function order in source
forder = []
seen = set()
for m in re.finditer(r'(?m)^([\w\* ]+?)\b(\w+)\s*\([^;]*\)\s*$', base):
    n = '_' + m.group(2)
    if n in bands and n not in seen:
        forder.append((m.start(1), n))
        seen.add(n)
forder.sort()

MAXS = max(max(v) for v in bands.values() if v) + 40
S = 0
plan = []
for pos, fn in forder:
    band = set(bands[fn])
    if not band:
        continue                      # unknown — leave
    if S in band:
        plan.append((pos, fn, S, 'ok' if 0 in band else 'fixed'))
        continue
    # raise S to smallest value >= S in band
    cand = [s for s in band if s >= S]
    if not cand:
        # need a later period; bands only sampled to maxpad — extend search
        cand = [s for s in range(S, MAXS) if (s % 17) in {b % 17 for b in band}]
        if not cand:
            plan.append((pos, fn, S, 'STUCK'))
            continue
    S2 = min(cand)
    plan.append((pos, fn, S2, 'pad+%d' % (S2 - S)))
    S = S2

for pos, fn, s, tag in plan:
    print('%-30s S=%-3d %s' % (fn, s, tag))
