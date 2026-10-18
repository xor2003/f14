#!/usr/bin/env python3
"""Generate src/GDSEG.ASM — replica of retail DGROUP."""
import re, pickle

EXE = '/home/xor/games/f14gold/Fleetgold/F14GOLD/F14.EXE'
BASE = 0x5b310
IMGEND = 0x642e

def H(v):
    return '0%XH' % v

def dbline(buf):
    return '\tDB ' + ','.join('0%XH' % c for c in buf)

def main():
    gold = {}
    for l in open('analysis/gold/dseg_order.tsv'):
        p = l.rstrip('\n').split('\t')
        if len(p) >= 3 and re.match(r'^[0-9a-fA-F]+$', p[0]):
            gold[p[2].strip()] = int(p[0], 16)

    vs = pickle.load(open('/tmp/varsizes.pkl', 'rb'))
    st = pickle.load(open('/tmp/strsyms.pkl', 'rb'))
    ex = pickle.load(open('/tmp/externinv.pkl', 'rb'))
    img = open(EXE, 'rb').read()[BASE:BASE+IMGEND]

    mapped = dict(gold)
    mapped_l = {k.lower(): v for k, v in mapped.items()}
    def vsize(name):
        return max(2, vs.get(name, vs.get(name if name.startswith('_') else '_' + name, 4)))

    # fixup cells: emitted as DW instead of plain label
    fixups = {
        '_Seg3D':   '_Seg3D DW SEG _Seg3D',
        '_PalPtr3D': '_PalPtr3D DW _defaultpal',
    }

    occupied = []
    for nm, off in mapped.items():
        occupied.append((off, off + vsize(nm)))

    # string literal placement
    litpos = {}
    for b, sym in st['symtab'].items():
        poss = []
        k = img.find(b)
        while k >= 0:
            poss.append(k); k = img.find(b, k + 1)
        best = None
        for p in poss:
            if not any(s <= p < e for s, e in occupied):
                best = p; break
        if best is not None:
            litpos[sym] = best
            occupied.append((best, best + len(b)))
        else:
            litpos[sym] = None

    occupied.sort()
    END = max(e for _, e in occupied) + 0x10
    gaps = []
    cur = 0
    for s, e in occupied:
        if s > cur:
            gaps.append([cur, s])
        cur = max(cur, e)
    if cur < END:
        gaps.append([cur, END])
    spill = [END]
    def alloc(sz):
        for g in gaps:
            if g[1] - g[0] >= sz:
                p = g[0]; g[0] += sz; return p
        p = spill[0]; spill[0] += sz; return p

    items = []
    placed = {}
    emitted = set(k.lower() for k in mapped)  # case-insensitive dedup
    for nm, off in mapped.items():
        items.append((off, 'label', nm, None))

    for b, sym in st['symtab'].items():
        if sym not in st['used']:
            continue
        emitted.add(sym.lower())
        lp = litpos[sym]
        if lp is not None:
            items.append((lp, 'label', sym, None))
        else:
            items.append((alloc(len(b)), 'data', sym, b))

    # alias names that share another var's cell (inner-extern operand-order
    # aliases get their own public label at the same GDSEG offset)
    ALIASES = {'_JoyVals': '_minstick',
               '_GtimeDAC': '_gtime',
               '_FIRINGORDERCNTx': '_firingordercnt',
               '_FETZ': '_first_enemy_trace',
               '_NPLANESx': '_nplanes', '_NTRACE2x': '_ntrace2', '_NTRACEx': '_ntrace',
               '_PDLCx': '_planedatalistcnt'}

    for nm in ex['near']:
        pub = nm if nm.startswith('_') else '_' + nm
        pl = pub.lower()
        if pl in emitted or pl in {k.lower() for k in ALIASES}:
            continue
        off = alloc(vsize(nm))
        items.append((off, 'label', pub, None))
        placed[pl] = off
        emitted.add(pl)

    for extra in ['_MinStick', '_MaxStick', '_LowRange', '_HighRange',
                  '_CenterPosition', '_RawJoy', '_DeadZone']:
        el = extra.lower()
        if el not in emitted:
            placed[el] = alloc(0x20)
            items.append((placed[el], 'label', extra, None))
            emitted.add(el)
    for al, tgt in ALIASES.items():
        if al.lower() not in emitted:
            off = mapped_l.get(tgt, placed.get(tgt.lower()))
            if off is None:
                off = mapped_l.get(tgt.lstrip('_'), placed.get(tgt.lstrip('_')))
            if off is not None:
                items.append((off, 'label', al, None))
                emitted.add(al.lower())

    # emit
    pending = {}
    for off, kind, nm, data in items:
        pending.setdefault(off, []).append((kind, nm, data))

    out = ["DGROUP GROUP GDSEG",
           "GDSEG SEGMENT BYTE PUBLIC 'BEGDATA'",
           "\tEXTRN _defaultpal:BYTE"]
    pos = 0
    for off in sorted(pending):
        if off < pos:
            print('OVERLAP at', hex(off)); continue
        while pos < off and pos < IMGEND:
            n = min(16, off - pos, IMGEND - pos)
            out.append(dbline(img[pos:pos+n])); pos += n
        if pos < off:
            out.append('\tORG ' + H(off)); pos = off
        for kind, nm, data in pending[off]:
            if nm in fixups:
                out.append('\tPUBLIC ' + nm)
                out.append(fixups[nm])
                pos = off + 2
                continue
            out.append('\tPUBLIC ' + nm)
            out.append(nm + ' LABEL BYTE')
            if kind == 'data':
                for i in range(0, len(data), 16):
                    out.append(dbline(data[i:i+16]))
                pos = off + len(data)
    out.append('GDSEG ENDS')
    out.append("ZFAR SEGMENT PARA PUBLIC 'FAR_BSS'")
    for nm in sorted(ex['far']):
        pub = nm if nm.startswith('_') else '_' + nm
        pl = pub.lower()
        if pl in emitted:
            continue
        emitted.add(pl)
        out.append('\tPUBLIC ' + pub)
        out.append('%s DB %d DUP(?)' % (pub, vsize(pub)))
    out.append('ZFAR ENDS')
    out.append('END')
    open('src/GDSEG.ASM', 'w').write('\n'.join(out) + '\n')
    print('extent:', hex(spill[0]), 'labels:',
          sum(1 for i in items if i[1] == 'label'),
          'data:', sum(1 for i in items if i[1] == 'data'))

if __name__ == '__main__':
    main()
