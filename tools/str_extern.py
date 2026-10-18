#!/usr/bin/env python3
"""Rewrite C string literals -> extern char _gstrN[] references."""
import re, glob, pickle

LIT = re.compile(r'"((?:[^"\\]|\\.)*)"')

def mask_comments(t):
    out = list(t); i = 0; n = len(t)
    while i < n:
        if t[i:i+2] == '/*':
            j = t.find('*/', i+2); j = n if j < 0 else j+2
            for k in range(i, j):
                if t[k] != '\n': out[k] = ' '
            i = j; continue
        if t[i:i+2] == '//':
            j = t.find('\n', i); j = n if j < 0 else j
            for k in range(i, j): out[k] = ' '
            i = j; continue
        i += 1
    return ''.join(out)

def cbytes(s):
    out = bytearray(); i = 0
    while i < len(s):
        c = s[i]
        if c == '\\':
            i += 1
            if i >= len(s): out.append(92); break
            e = s[i]
            mp = {'n':10,'t':9,'r':13,'\\':92,'"':34,"'":39,'b':8,'f':12,'v':11,'a':7}
            if e in mp: out.append(mp[e]); i += 1
            elif e in '01234567':
                j = i
                while j < len(s) and j < i+3 and s[j] in '01234567': j += 1
                out.append(int(s[i:j], 8) & 255); i = j
            elif e == 'x':
                j = i+1
                while j < len(s) and s[j] in '0123456789abcdefABCDEF': j += 1
                out.append(int(s[i+1:j] or '0', 16) & 255); i = j
            elif e == '\n': i += 1
            else: out.append(ord(e)); i += 1
        else: out.append(ord(c) & 255); i += 1
    return bytes(out)

def in_sizeof(ct, pos):
    j = pos - 1; depth = 0
    while j >= 0:
        c = ct[j]
        if c == ')': depth += 1
        elif c == '(':
            if depth == 0:
                k = j - 1
                while k >= 0 and ct[k] in ' \t': k -= 1
                return ct[max(0,k-6):k+1].endswith('sizeof')
            depth -= 1
        elif c in ';,{}' and depth == 0:
            return False
        j -= 1
    return False

def is_array_init(ct, pos):
    """literal is entire RHS of '= \"...\"' where declarator has [] (array copy init)."""
    j = pos - 1
    while j >= 0 and ct[j] in ' \t\n': j -= 1
    if j < 0 or ct[j] != '=':
        return False
    # scan back over statement to find declarator text
    k = j - 1; depth = 0
    while k >= 0:
        c = ct[k]
        if c == ')': depth += 1
        elif c == '(':
            if depth: depth -= 1
        if depth == 0 and c in ';,{}':
            break
        k -= 1
    decl = ct[k+1:j]
    return '[' in decl and '*' not in decl

def process(fn):
    t = open(fn, errors='replace').read()
    ct = mask_comments(t)
    occ = [(m.start(), m.end(), m.group(1)) for m in LIT.finditer(ct)]
    merged = []
    k = 0
    while k < len(occ):
        s0, e0, txt = occ[k]; k += 1
        while k < len(occ) and ct[e0:occ[k][0]].strip() == '':
            txt += occ[k][2]; e0 = occ[k][1]; k += 1
        merged.append((s0, e0, txt))
    keep = []
    for s0, e0, txt in merged:
        ls = ct.rfind('\n', 0, s0) + 1
        j = ls
        while j < len(ct) and ct[j] in ' \t': j += 1
        if ct[j] == '#' and ct[j:j+7] != '#define':
            continue
        if in_sizeof(ct, s0) or is_array_init(ct, s0):
            continue
        keep.append((s0, e0, txt))
    return t, keep

def objs_files():
    """Set of src basenames that actually get compiled into OBJS."""
    import os
    objs = ''
    for l in open('build_all.sh'):
        if l.startswith('OBJS='):
            objs = l.split('"')[1] if '"' in l else l.split('=')[1].strip()
    bases = set(o.lower() for o in objs.split())
    out = set()
    for f in glob.glob('src/*.[cC]'):
        if os.path.splitext(os.path.basename(f))[0].lower() in bases:
            out.add(f)
    return out

def main():
    cset = objs_files()
    files = sorted(cset) + sorted(glob.glob('h/*.[hH]'))
    symtab = {}
    order = []
    filesyms = {}
    for fn in files:
        t, keep = process(fn)
        if not keep:
            continue
        new = []; last = 0; syms = set()
        for s0, e0, txt in keep:
            b = cbytes(txt) + b'\x00'
            if b not in symtab:
                symtab[b] = '_gstr%d' % len(symtab)
                order.append(b)
            sym = symtab[b]
            syms.add(sym)
            new.append(t[last:s0]); new.append(sym[1:]); last = e0
        new.append(t[last:])
        out = ''.join(new)
        decls = ''.join('extern char %s[];\n' % s[1:] for s in sorted(syms))
        im = None
        out = decls + '\n' + out
        open(fn, 'w').write(out)
        filesyms[fn] = syms
        print('%s: %d lits' % (fn, len(keep)))
    # only symbols referenced by a *compiled* file need GDSEG bytes
    used = set()
    for f, ss in filesyms.items():
        if f in cset or f.startswith('h/'):
            used |= ss
    pickle.dump({'symtab': symtab, 'order': order, 'used': used}, open('/tmp/strsyms.pkl', 'wb'))
    print('total unique literals:', len(symtab), 'used:', len(used))

if __name__ == '__main__':
    main()
