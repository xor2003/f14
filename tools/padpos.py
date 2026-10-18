#!/usr/bin/env python3
# padpos.py — sweep count of 'extern int _ppN;' decls inserted right before
# a target function (keeping all existing pads), compile the module OBJ,
# and report which counts make the target's instruction stream match gold
# (normalized, via objfn). No link needed. usage:
#   padpos.py MODULE FUNCNAME [maxpad]
import re, subprocess, sys, tempfile, os

mod = sys.argv[1]
fnname = sys.argv[2]
fn = fnname if fnname.startswith('_') else '_' + fnname
fnname = fn.lstrip('_')
maxpad = int(sys.argv[3]) if len(sys.argv) > 3 else 40
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

def gold_disasm():
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
    return norm(r.stdout), gname

golden, gname = gold_disasm()
print('target %s -> gold %s %s (%d insns)' % (fn, gname,
      ':'.join('%04x' % x for x in anch[fn]), len(golden)))

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

def cand_disasm():
    r = subprocess.run(['python3', 'tools/objfn.py',
                        'build/obj/%s.OBJ' % mod, fn],
                       capture_output=True, text=True)
    return norm(r.stdout)

def aligned(cand, gold):
    if not cand or len(cand) > len(gold):
        return False
    for k in range(0, min(5, len(gold) - len(cand) + 1)):
        if gold[k:k + len(cand)] == cand:
            return True
    return False

m = re.search(r'(?m)^[\w\* ]+?\b%s\s*\([^;{}]*\)\s*\{?\s*$' % re.escape(fnname), base)
if not m:
    sys.exit('def %s not found' % fnname)
ins = m.start()
PAD = 'extern int _pp%d;\n'
hits = []
for n in range(0, maxpad + 1):
    open(path, 'w').write(base[:ins] + ''.join(PAD % k for k in range(n)) + base[ins:])
    r = compile_mod()
    if r.returncode != 0:
        print('n=%d COMPILE-FAIL' % n, flush=True)
        continue
    ok = aligned(cand_disasm(), golden)
    print('n=%d %s' % (n, 'MATCH' if ok else 'diff'), flush=True)
    if ok: hits.append(n)
open(path, 'w').write(base)
print('hits:', hits)
