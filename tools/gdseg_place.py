#!/usr/bin/env python3
# Place PUBLIC labels at exact offsets inside GDSEG.ASM.
# - removes existing PUBLIC/LABEL pairs for the named symbols
# - splits `DB n DUP(x)` runs to insert labels at exact byte positions
# - labels never emit bytes; DW/DD multi-byte items can't be split (warns)
# Usage: gdseg_place.py name=hexoff [name=hexoff ...]
import re, sys

path = 'src/GDSEG.ASM'

def main():
    want = []
    for a in sys.argv[1:]:
        n, o = a.split('=')
        want.append((n.strip(), int(o, 16)))
    want.sort(key=lambda x: x[1])
    names = set(n for n, _ in want)
    lines = open(path).read().split('\n')

    # pass 1: strip existing PUBLIC _n / _n LABEL lines for the names
    out = []
    i = 0
    while i < len(lines):
        l = lines[i]
        m = re.match(r'\s*PUBLIC\s+_(\w+)\s*$', l)
        if m and m.group(1) in names and i + 1 < len(lines) and re.match(r'_' + re.escape(m.group(1)) + r'\s+LABEL', lines[i + 1]):
            i += 2
            continue
        m = re.match(r'_(\w+)\s+LABEL', l)
        if m and m.group(1) in names:
            i += 1
            continue
        out.append(l)
        i += 1
    lines = out

    # pass 2: walk emission position, insert labels
    pos = 0
    out = []
    wi = 0
    for l in lines:
        m = re.match(r'\s*ORG\s+0?([0-9a-fA-F]+)H', l)
        if m:
            pos = int(m.group(1), 16)
            out.append(l)
            continue
        mm = re.match(r'(\s*)(DB|DW|DD)\s+(.*)', l)
        if not mm:
            out.append(l)
            continue
        ind, op, body = mm.groups()
        usz = {'DB': 1, 'DW': 2, 'DD': 4}[op]
        items = [p.strip() for p in body.split(',')]
        # total size
        tot = 0
        spans = []
        for p in items:
            d = re.match(r'(\d+)\s+DUP\s*\((.*)\)\s*$', p)
            c = int(d.group(1)) * usz if d else usz
            spans.append((p, c, bool(d)))
            tot += c
        # labels before this line
        while wi < len(want) and want[wi][1] < pos:
            out += ['\tPUBLIC _' + want[wi][0], '_' + want[wi][0] + ' LABEL BYTE']
            wi += 1
        if not any(pos <= o < pos + tot for _, o in want[wi:]):
            out.append(l)
            pos += tot
            continue
        # rebuild the line with labels inserted; only DB DUP-runs are splittable
        cur = pos
        buf = []
        def flush():
            if not buf: return
            if all(re.match(r'1 DUP\((.*)\)$', b) for b in buf):
                fills = set(re.match(r'1 DUP\((.*)\)$', b).group(1) for b in buf)
                if len(fills) == 1:
                    out.append(ind + 'DB %d DUP(%s)' % (len(buf), fills.pop()))
                    buf.clear(); return
            for b in buf:
                out.append(ind + 'DB ' + b if re.match(r'1 DUP', b) or ',' not in b else ind + 'DB ' + b)
            buf.clear()
        for txt, c, isdup in spans:
            if isdup and usz == 1:
                fill = re.match(r'\d+\s+DUP\s*\((.*)\)', txt).group(1)
                for _k in range(c):
                    while wi < len(want) and want[wi][1] == cur:
                        flush()
                        out += ['\tPUBLIC _' + want[wi][0], '_' + want[wi][0] + ' LABEL BYTE']
                        wi += 1
                    buf.append('1 DUP(%s)' % fill)
                    cur += 1
            else:
                while wi < len(want) and want[wi][1] == cur:
                    flush()
                    out += ['\tPUBLIC _' + want[wi][0], '_' + want[wi][0] + ' LABEL BYTE']
                    wi += 1
                if any(cur < o < cur + c for _, o in want):
                    sys.stderr.write('WARN: label wanted mid-item at %x (%s)\n' % (cur, txt))
                buf.append(txt if isdup else txt)
                cur += c
        flush()
        pos += tot
    while wi < len(want):
        out += ['\tPUBLIC _' + want[wi][0], '_' + want[wi][0] + ' LABEL BYTE']
        wi += 1
    open(path, 'w').write('\n'.join(out))
    print('placed:', ', '.join('%s@%x' % (n, o) for n, o in want))

if __name__ == '__main__':
    main()
