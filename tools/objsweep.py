#!/usr/bin/env python3
# objsweep.py — measure every module function's codegen-match vs uniform
# decl-pad shift, WITHOUT linking: compile module OBJ once per pad count,
# disasm each function via objfn, compare normalized instruction text vs
# the paired gold routine's disasm.
#
# usage: objsweep.py MODULE maxpad
import re, subprocess, sys, json, tempfile, os

mod = sys.argv[1]
maxpad = int(sys.argv[2]) if len(sys.argv) > 2 else 30
path = 'src/%s.C' % mod
base = open(path).read()
KV = os.path.expanduser('~/kvikdos/kvikdos')
ROOT = '/home/xor/games/f14src'
GOLD = '/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'

# ---- gold extents ----
goldmap = {}
seg = {}
for l in open('analysis/gold/gold.map'):
    m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
    if m:
        cur = m.group(1); seg[cur] = int(m.group(2), 16); continue
    m = re.match(r'(\S+): (\S+) FAR (.+)$', l.strip())
    if m:
        st = int(m.group(3).split(' ')[0].split('-')[0], 16)
        goldmap[(seg[m.group(2)], st)] = (m.group(2), m.group(3), m.group(1))

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
for l in open('build/f14_named.map'):
    m = re.match(r'(_\S+): (\S+) FAR', l)
    if m: myseg[m.group(1)] = m.group(2)
modfns = sorted(f for f, s in myseg.items() if s == mod + '_TEXT' and f in anch)
print('module %s: %d anchored fns' % (mod, len(modfns)), flush=True)

# ---- gold disasm (normalized) ----
gd = open(GOLD, 'rb').read()
ghdr = int.from_bytes(gd[8:10], 'little') * 16

def gold_disasm(fn):
    s, o = anch[fn]
    cseg, ext, gname = goldmap[(s, o)]
    chunks = [p for p in ext.split(' ') if p.startswith('R')]
    chunks = [(int(p[1:].split('-')[0], 16), int(p[1:].split('-')[1], 16))
              for p in chunks]
    if not chunks:
        a, b = ext.split(' ')[0].split('-')
        chunks = [(int(a, 16), int(b, 16))]
    img = s * 16
    blob = b''
    for a, b in chunks:
        blob += gd[ghdr + img + a:ghdr + img + b + 1]
    f = tempfile.mktemp(suffix='.bin')
    open(f, 'wb').write(blob)
    r = subprocess.run(['ndisasm', '-b16', f], capture_output=True, text=True)
    os.unlink(f)
    return norm(r.stdout)

def norm(text):
    out = []
    for l in text.split('\n'):
        m = re.match(r'[0-9A-F]{8}\s+[0-9A-F]+\s+(.*)', l)
        if not m:
            continue
        ins = m.group(1).strip()
        ins = re.sub(r'0x[0-9a-f]+:0x[0-9a-f]+', 'FAR', ins)
        ins = re.sub(r'\b(call|jmp|j[a-z]+) 0x[0-9a-f]+', r'\1 T', ins)
        ins = re.sub(r'\b(call|jmp|j[a-z]+) short 0x[0-9a-f]+', r'\1 T', ins)
        ins = re.sub(r'\[0x[0-9a-f]+\]', '[D]', ins)
        ins = re.sub(r'\[[a-z]{2}\+0x[0-9a-f]+\]', '[M]', ins)
        ins = re.sub(r'\[[a-z]{2}-0x[0-9a-f]+\]', '[M]', ins)
        ins = re.sub(r'\[[a-z]{2}\]', '[M]', ins)
        ins = re.sub(r'\[[^\]]+\]', '[X]', ins)
        ins = re.sub(r'\+0x[0-9a-f]+', '+I', ins)
        ins = re.sub(r'-0x[0-9a-f]+', '-I', ins)
        ins = re.sub(r'\b0x[0-9a-f]+\b', 'I', ins)
        ins = re.sub(r'\bbyte I\b', 'I', ins)
        ins = re.sub(r'\s+', ' ', ins)
        out.append(ins)
    return out

golden = {fn: gold_disasm(fn) for fn in modfns}

def compile_mod():
    return subprocess.run([KV,
        '--mount=C:%s/c6' % ROOT, '--mount=D:%s/src' % ROOT,
        '--mount=E:%s/build/obj' % ROOT, '--mount=F:%s/h' % ROOT,
        '--env=PATH=C:\\BIN;C:\\BINB', '--env=INCLUDE=F:\\;C:\\INCLUDE',
        '--env=LIB=C:\\LIB', '--env=TMP=E:\\', '--drive=E',
        'C:\\BIN\\CL.EXE', '/FPa', '/G2', '/Gs', '/AM', '/Osegcl',
        '/nologo', '/c', '/Zi', '/Zd', '-IF:\\',
        '/FoE:\\%s.OBJ' % mod, 'D:\\%s.C' % mod],
        capture_output=True, timeout=600)

def cand_disasm(fn):
    r = subprocess.run(['python3', 'tools/objfn.py',
                        'build/obj/%s.OBJ' % mod, fn],
                       capture_output=True, text=True)
    return norm(r.stdout)

def aligned(cand, gold):
    # obj pubdef can point inside a shared prologue: candidate must appear
    # as a contiguous slice of gold (gold may lead by a few instructions)
    if not cand or len(cand) > len(gold):
        return False
    for k in range(0, min(5, len(gold) - len(cand) + 1)):
        if gold[k:k + len(cand)] == cand:
            return True
    return False

mi = [m.end() for m in re.finditer(r'#include[^\n]*\n', base)]
ins = mi[-1] if mi else 0
PAD = 'extern int _pd%d;\n'
bands = {f: [] for f in modfns}
for n in range(0, maxpad + 1):
    open(path, 'w').write(base[:ins] + ''.join(PAD % k for k in range(n)) + base[ins:])
    r = compile_mod()
    if r.returncode != 0:
        print('n=%d COMPILE-FAIL' % n, flush=True)
        continue
    hits = []
    for fn in modfns:
        if aligned(cand_disasm(fn), golden[fn]):
            bands[fn].append(n)
            hits.append(fn)
    print('n=%2d  %2d obj-match: %s' % (n, len(hits), ' '.join(hits[:8])), flush=True)
open(path, 'w').write(base)
json.dump(bands, open('analysis/bands_%s.json' % mod, 'w'), indent=0)
