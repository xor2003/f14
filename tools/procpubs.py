#!/usr/bin/env python3
# emit PUBLIC decls for PROC names defined at unconditional assembly depth.
# if 0 / IFDEF/IFNDEF blocks are treated as non-assembling (safe: only drops
# exports for conditionally-compiled procs, which callers can't rely on anyway).
import re
import sys

depth = []  # stack of booleans
names = []
seen = set()
proc_re = re.compile(r'^\s*([A-Za-z_][A-Za-z0-9_]*)\s+PROC\b', re.I)
cond_re = re.compile(r'^\s*(if|ifdef|ifndef|else|endif)\b(.*)', re.I)

for line in open(sys.argv[1], errors='replace'):
    m = cond_re.match(line)
    if m:
        kw = m.group(1).lower()
        arg = m.group(2).strip()
        if kw in ('if', 'ifdef', 'ifndef'):
            if kw == 'if':
                try:
                    val = bool(int(arg.split()[0], 0))
                except (ValueError, IndexError):
                    val = False  # unknown expr -> treat dead for export purposes
            else:
                val = False  # ifdef/ifndef unknown
            depth.append(val)
        elif kw == 'else':
            if depth:
                depth[-1] = not depth[-1]
        elif kw == 'endif':
            if depth:
                depth.pop()
        continue
    if all(depth):
        m = proc_re.match(line)
        if m and m.group(1) not in seen:
            seen.add(m.group(1))
            names.append(m.group(1))

for n in names:
    print('PUBLIC ' + n)
