#!/usr/bin/env python3
# gstr_inner.py — move file-scope `extern char gstrN[];` decls into the
# functions that use them (after the local decl block, in use order).
# Rationale: MS C 6 leaf/symbol ordering is fixed per-function at compile
# time; hoisted externs perturb it vs retail, where string literals bound
# at their use site inside the function. Inner externs restore that order.

import re
import sys

DECL_KW = re.compile(
    r'^\s*(int|char|long|unsigned|signed|short|float|double|void|struct|'
    r'union|enum|static|extern|auto|register|volatile|const|far|near|'
    r'huge|pascal|cdecl|TARGET|TIDTarg|DTARGET|PLANESTATUS|PLANE|PLANEREC|'
    r'PHOENIX|SBox|WEAPON|LAUNCH|BOX|POINT|varient|BYTE|WORD|DWORD|BOOL|'
    r'PSB|CELLTYP|MENUTYP|FONTPTR|WEAPS|AmmoType|SYSSTATE|MFUNC|'
    r'asignal|setupparms|StoreData|ORDNANCE|RAY|WingCmds|MAPCORNER|'
    r'Target3D|SORTABLE|DISPLAY)\b')
GSTR_TOP = re.compile(r'^extern char gstr\d+\[\];')
GSTR_USE = re.compile(r'gstr\d+')

def process(path):
    lines = open(path).read().split('\n')
    # strip every col-0 gstr extern (top or anywhere at file scope)
    out, removed = [], 0
    i = 0
    n = len(lines)
    while i < n:
        if GSTR_TOP.match(lines[i]):
            removed += 1
            i += 1
            continue
        out.append(lines[i])
        i += 1
    # second pass: insert inner decls
    res = []
    i = 0
    n = len(out)
    inserted = 0
    while i < n:
        res.append(out[i])
        # function open: '{' at col0 preceded by ')' header or K&R param decl,
        # or inline 'name(args) {'
        hdr = bool(re.match(r'^\w[\w \t\*]*\([^;]*\)\s*\{', out[i]))
        if not hdr and re.match(r'^\{\s*(//.*)?$', out[i]):
            j = len(res) - 2
            while j >= 0 and (res[j] == '' or res[j].startswith('//')):
                j -= 1
            if j >= 0:
                p = res[j].rstrip()
                hdr = (p.endswith(')') or
                       (p.endswith(';') and not p.endswith('=') and
                        re.match(r'^\s*(int|char|long|short|unsigned|'
                                 r'far|near|struct|TARGET|PLANE|'
                                 r'register)\b', p)))
        if hdr:
            # function open — collect decl lines
            i += 1
            have = set()
            while i < n and DECL_KW.match(out[i]):
                res.append(out[i])
                for m in GSTR_USE.finditer(out[i]):
                    have.add(m.group(0))
                # consume continuation lines of multi-line initializers
                while not out[i].rstrip().rstrip('\\').endswith(';'):
                    i += 1
                    if i >= n:
                        break
                    res.append(out[i])
                    for m in GSTR_USE.finditer(out[i]):
                        have.add(m.group(0))
                i += 1
            # scan function body (until col-0 '}')
            j = i
            uses = []
            while j < n:
                if out[j] == '}':
                    break
                for m in GSTR_USE.finditer(out[j]):
                    if m.group(0) not in uses:
                        uses.append(m.group(0))
                j += 1
            uses = [u for u in uses if u not in have]
            for u in uses:
                res.append('\textern char %s[];' % u)
                inserted += 1
            continue
        i += 1
    open(path, 'w').write('\n'.join(res))
    print('%s: removed %d top decls, inserted %d inner decls' %
          (path, removed, inserted))

for p in sys.argv[1:]:
    process(p)
