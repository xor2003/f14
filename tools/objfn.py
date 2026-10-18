#!/usr/bin/env python3
# objfn.py — extract a function's code bytes from an MSC OMF .OBJ and
# disassemble them.  usage: objfn.py OBJ [FUNCNAME]
# FUNCNAME without leading underscore accepted. Prints ndisasm output.

import re
import subprocess
import sys


def parse(path):
    d = open(path, 'rb').read()
    i = 0
    segs = {}          # segidx -> bytes
    segorder = []      # SEGDEF order -> attr
    names = {}         # nameidx -> str
    pubs = {}          # name -> (segidx, off)
    cur_seg = None
    while i < len(d) - 3:
        t = d[i]
        ln = d[i + 1] | d[i + 2] << 8
        rec = d[i + 3:i + 3 + ln]
        if t == 0x96:   # LNAMES: len-prefixed names
            p = 0
            while p < len(rec):
                nl = rec[p]; p += 1
                names[len(names) + 1] = rec[p:p + nl].decode('latin1')
                p += nl
        elif t == 0xa0 or t == 0xa2:  # LEDATA/LEDATA32
            sidx = rec[0]
            off = rec[1] | rec[2] << 8
            segs.setdefault(sidx, bytearray())
            body = rec[3:]
            if len(segs[sidx]) < off + len(body):
                segs[sidx].extend(b'\0' * (off + len(body) - len(segs[sidx])))
            segs[sidx][off:off + len(body)] = body
        elif t == 0x90:  # PUBDEF
            grp = rec[0]; sidx = rec[1]
            p = 2
            if sidx == 0:
                p += 2
            while p + 4 <= len(rec):
                nl = rec[p]; p += 1
                nm = rec[p:p + nl].decode('latin1', 'replace'); p += nl
                if p + 3 > len(rec):
                    break
                o = rec[p] | rec[p + 1] << 8; p += 2
                ti = rec[p]; p += 1          # type index
                if ti & 0x80:
                    p += 1                    # 2-byte index continuation
                if sidx:
                    pubs[nm] = (sidx, o)
        i += 3 + ln
    return segs, pubs


def main():
    d = sys.argv[1]
    want = sys.argv[2] if len(sys.argv) > 2 else None
    segs, pubs = parse(d)
    if want is None:
        for k, v in sorted(pubs.items(), key=lambda x: x[1]):
            print('%s seg%d %04x' % (k, v[0], v[1]))
        return
    if want not in pubs:
        print('NOT FOUND', want)
        return
    sidx, off = pubs[want]
    code = bytes(segs.get(sidx, b''))
    # next pub in same seg bounds the extent
    nxt = min([v[1] for k, v in pubs.items() if v[0] == sidx and v[1] > off]
              or [len(code)])
    blob = code[off:nxt]
    open('/tmp/objfn.bin', 'wb').write(blob)
    print(subprocess.run(['ndisasm', '-b16', '-o', '0x%x' % off,
                          '/tmp/objfn.bin'], capture_output=True,
                         text=True).stdout)

main()
