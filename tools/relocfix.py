#!/usr/bin/env python3
# relocfix.py — restore MZ relocation entries that OMF fixups cannot express.
#
# usage: relocfix.py ORIG.EXE NEW.EXE OUT.EXE
#
# A word fixup needs both bytes inside one segment; a reloc site that
# straddles a real segment boundary (the last byte of one segment and the
# first of the next) is emitted literally by ada_segfix.py to keep the
# image byte-exact.  This pass re-adds the missing entries to the linked
# exe's reloc table, absorbing the +4 bytes in the post-table padding so
# the image stays at the same file offset.

import re
import struct
import sys


def relocs(d):
    nr = struct.unpack_from('<H', d, 6)[0]
    ro = struct.unpack_from('<H', d, 0x18)[0]
    return nr, ro, [(struct.unpack_from('<H', d, ro + i * 4 + 2)[0],
                     struct.unpack_from('<H', d, ro + i * 4)[0])
                    for i in range(nr)]


def main():
    orig_p, new_p, out_p = sys.argv[1:4]
    orig = open(orig_p, 'rb').read()
    new = bytearray(open(new_p, 'rb').read())

    # region bases (image-relative para) for seg:off decomposition —
    # entries store the site's segment paragraph, not site>>4
    bounds = []
    if len(sys.argv) > 4:
        for l in open(sys.argv[4]):
            m = re.match(r'\S+\s+([0-9a-f]+):([0-9a-f]+)', l)
            if m:
                lo = (int(m.group(1), 16) * 16 + int(m.group(2), 16)
                      - 0x10000)
                bounds.append(lo & ~0xf)  # segment starts at para floor

    onr, oro, oents = relocs(orig)
    nnr, nro, nents = relocs(new)
    osites = {s * 16 + o for s, o in oents}
    nsites = {s * 16 + o for s, o in nents}
    missing = sorted(osites - nsites)
    if not missing:
        open(out_p, 'wb').write(new)
        return

    hsize = struct.unpack_from('<H', new, 8)[0] * 16
    # room between table end and image start for the extra entries
    assert nro + (nnr + len(missing)) * 4 <= hsize

    # original table order locally follows image position: insert each
    # missing site after the last entry at a lower site
    for site in missing:
        ents = [(s * 16 + o, o, s) for s, o in
                ((struct.unpack_from('<H', new, nro + i * 4 + 2)[0],
                  struct.unpack_from('<H', new, nro + i * 4)[0])
                 for i in range(nnr))]
        pos = 0
        for i, (lin, o, s) in enumerate(ents):
            if lin <= site:
                pos = i + 1
        base = max((b for b in bounds if b <= site), default=site & ~0xf)
        entry = struct.pack('<HH', site - base, base // 16)
        new = (new[:nro + pos * 4] + entry +
               new[nro + pos * 4:hsize - 4] + new[hsize:])
        nnr += 1
    struct.pack_into('<H', new, 6, nnr)
    open(out_p, 'wb').write(new)
    print('relocfix: added %d entries (%s)' % (
        len(missing), ' '.join('%x' % s for s in missing)))


if __name__ == '__main__':
    main()
