#!/usr/bin/env python3
# padmod2.py — iterative positional decl-pad solver for one module.
# For each function (top-down) whose normalized instruction stream differs
# from its gold extent, insert 'extern int _pqN;' pads right before its
# def; sweep count 0..MAXP; for each count compile once and measure ALL
# functions; choose the count that fixes the target and maximizes total
# module matches; apply permanently; continue to next failing function.
# usage: padmod2.py MODULE [maxpad]
import re, subprocess, sys, tempfile, os, json

mod = sys.argv[1]
maxpad = int(sys.argv[2]) if len(sys.argv) > 2 else 24
path = 'src/%s.C' % mod
base = open(path).read()
KV = os.path.expanduser('~/kvikdos/kvikdos')
ROOT = '/home/xor/games/f14src'
GOLD = '/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'

goldmap = {}
seg = {}
for l in open('analysis/gold/gold.map'):
    m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
    if m:
        seg[m.group(1)] = int(m.group(2), 16); continue
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

# order-aligned anchors override mis-paired classified entries
if os.path.exists('analysis/gold/aligned_anchors.tsv'):
    for l in open('analysis/gold/aligned_anchors.tsv'):
        f = l.rstrip().split('\t')
        if f[0] == 'name':
            continue
        m = re.match(r'([0-9a-f]{4}):([0-9a-f]{4})', f[2])
        if m:
            s, o = int(m.group(1), 16), int(m.group(2), 16)
            if (s, o) in goldmap:
                anch[f[0]] = (s, o)

myseg = {}
for l in open('build/f14_named.map'):
    m = re.match(r'(_\S+): (\S+) FAR', l)
    if m: myseg[m.group(1)] = m.group(2)
modfns = sorted(f for f, s in myseg.items()
                if s in (mod + '_TEXT', '$' + mod + '_TEXT') and f in anch)
print('module %s: %d anchored fns' % (mod, len(modfns)), flush=True)

gd = open(GOLD, 'rb').read()
ghdr = int.from_bytes(gd[8:10], 'little') * 16

def norm(text):
    out = []
    for l in text.split('\n'):
        m = re.match(r'[0-9A-F]{8}\s+[0-9A-F]+\s+(.*)', l)
        if not m: continue
        ins = m.group(1).strip()
        ins = re.sub(r'0x[0-9a-f]+:0x[0-9a-f]+', 'FAR', ins)
        ins = re.sub(r'\b(call|jmp|j[a-z]+) (short )?0x[0-9a-f]+', r'\1 T', ins)
        ins = re.sub(r'\[0x[0-9a-f]+\]', '[D]', ins)
        ins = re.sub(r'\[[a-z]{2}[+-]0x[0-9a-f]+\]', '[M]', ins)
        ins = re.sub(r'\[[a-z]{2}\]', '[M]', ins)
        ins = re.sub(r'\[[^\]]+\]', '[X]', ins)
        ins = re.sub(r'\+0x[0-9a-f]+', '+I', ins)
        ins = re.sub(r'-0x[0-9a-f]+', '-I', ins)
        ins = re.sub(r'\b0x[0-9a-f]+\b', 'I', ins)
        ins = re.sub(r'\s+', ' ', ins)
        out.append(ins)
    return out

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

golden = {fn: gold_disasm(fn) for fn in modfns}

def compile_mod(src):
    open(path, 'w').write(src)
    r = subprocess.run([KV,
        '--mount=C:%s/c6' % ROOT, '--mount=D:%s/src' % ROOT,
        '--mount=E:%s/build/obj' % ROOT, '--mount=F:%s/h' % ROOT,
        '--env=PATH=C:\\BIN;C:\\BINB', '--env=INCLUDE=F:\\;C:\\INCLUDE',
        '--env=LIB=C:\\LIB', '--env=TMP=E:\\%s' % os.environ.get('TMPD', ''),
        '--drive=E',
        'C:\\BIN\\CL.EXE', '/FPa', '/G2', '/Gs', '/AM', '/Osegcl',
        '/nologo', '/c', '/Zi', '/Zd', '-IF:\\',
        '/FoE:\\%s.OBJ' % mod, 'D:\\%s.C' % mod],
        capture_output=True, timeout=600)
    return r.returncode == 0

def cand_disasm(fn):
    r = subprocess.run(['python3', 'tools/objfn.py',
                        'build/obj/%s.OBJ' % mod, fn],
                       capture_output=True, text=True)
    return norm(r.stdout)

def aligned(cand, gold):
    if not cand or len(cand) > len(gold):
        return False
    for k in range(0, min(6, len(gold) - len(cand) + 1)):
        if gold[k:k + len(cand)] == cand:
            return True
    return False

def status_all(src):
    if not compile_mod(src):
        return None
    return {fn: aligned(cand_disasm(fn), golden[fn]) for fn in modfns}

# function defs in file order
def find_defs(src):
    order = []
    for m in re.finditer(r'(?m)^[\w\* ]*?\b(\w+)\s*\([^;{}]*\)\s*\{?\s*$', src):
        n = '_' + m.group(1)
        if n in modfns and (not order or order[-1][1] != n):
            order.append((m.start(), n))
    return order

PADN = [0]
def padblock(k):
    i = PADN[0]; PADN[0] += k
    return ''.join('extern int _pq%d;\n' % (i + j) for j in range(k))

src = base
cur = status_all(src)
if cur is None:
    sys.exit('baseline compile failed')
nfails = sum(1 for v in cur.values() if not v)
print('baseline obj-fails: %d' % nfails, flush=True)

for pos, fn in find_defs(src):
    if cur.get(fn):
        continue
    # fresh def position in current src
    m = re.search(r'(?m)^[\w\* ]*?\b%s\s*\([^;{}]*\)\s*\{?\s*$' % fn.lstrip('_'), src)
    if not m:
        continue
    ins = m.start()
    best = None
    for n in range(0, maxpad + 1):
        test = src[:ins] + padblock(n) + src[ins:]
        PADN[0] -= n
        st = status_all(test)
        if st is None:
            continue
        nmatch = sum(1 for v in st.values() if v)
        print('%s n=%d matches=%d self=%s' % (fn, n, nmatch, st[fn]), flush=True)
        if st[fn]:
            score = (nmatch, -n)
            if best is None or score > best[0]:
                best = (score, n)
    if best:
        n = best[1]
        src = src[:ins] + padblock(n) + src[ins:]
        cur = status_all(src)
        print('%s -> pad=%d applied, module matches=%d' %
              (fn, n, sum(1 for v in cur.values() if v)), flush=True)
    else:
        print('%s no pad helps, moving on' % fn, flush=True)

open(path, 'w').write(src)
open('analysis/padmod2_%s.json' % mod, 'w').write(json.dumps(
    {f: v for f, v in cur.items()}, indent=0))
print('DONE. matches=%d/%d' % (sum(1 for v in cur.values() if v), len(cur)))
