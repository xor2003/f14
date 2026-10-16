#!/usr/bin/env python3
# callpair_gold.py — propagate verified gold<->dev function pairings through
# the call graph.
#
# For each verified MATCH pair (dev name <-> gold addr), the two functions are
# known-identical twins, so their *ordered call sites* correspond one-to-one:
# the i-th call target on the dev side pairs with the i-th call target on the
# gold side.  This finds gold twins for functions whose mnemonic histogram is
# too drifted for Jaccard pairing (e.g. __output, reached via _sprintf's call).
#
# usage: callpair_gold.py GOLD.EXE ranges_gold.txt MY.EXE my_linkmap.map \
#          verify_sweep.txt > callpairs.tsv
#
# Output: dev_name <TAB> gold_seg:off <TAB> via <caller> <TAB> callsite#
#
# Only FAR calls with absolute seg:ofs targets and NEAR calls within the same
# segment are used (indirect calls carry no static target).  Call *order* is
# preserved by every MSC codegen level, which is what makes positional pairing
# work.  Pairs are deduped by dev name; conflicts are reported, not merged.

import collections
import re
import subprocess
import sys

GHIDRA_BASE = 0x1000 << 4


def load_image(path):
    d = open(path, 'rb').read()
    hdr = int.from_bytes(d[8:10], 'little') * 16
    pages = int.from_bytes(d[4:6], 'little')
    lastp = int.from_bytes(d[2:4], 'little')
    return d[hdr:(pages - 1) * 512 + (lastp or 512)]


def load_ranges(path):
    out = {}
    for l in open(path):
        p = l.split()
        if not p:
            continue
        seg, off = p[0].split(':')
        out[(int(seg, 16), int(off, 16))] = [
            (int(a, 16), int(b, 16))
            for a, b in (x.split('-') for x in p[1:])]
    return out


def load_mymap(path):
    """mzretools map -> segs dict + routines + per-seg sorted public list."""
    segs = {}
    pubs = collections.defaultdict(list)   # segname -> [(off, name)]
    funcs = {}
    for l in open(path):
        m = re.match(r'(\S+) CODE ([0-9a-f]+)', l)
        if m:
            segs[m.group(1)] = int(m.group(2), 16)
            continue
        m = re.match(r'(\S+): (\S+) (?:NEAR|FAR) ([0-9a-f]+)-([0-9a-f]+)', l)
        if m:
            name, seg = m.group(1), m.group(2)
            lo, hi = int(m.group(3), 16), int(m.group(4), 16)
            funcs[name] = (seg, segs[seg], lo, hi)
            pubs[seg].append((lo, name))
    for seg in pubs:
        pubs[seg].sort()
    return segs, funcs, pubs


def call_targets(img, blocks, base, is_far_seg=None):
    """ordered list of call targets: ('far', seg, ofs) or ('near', ofs).

    blocks: [(linear_lo, linear_hi)] in `base`-space bytes of `img`.
    Returns list preserving program order; each entry tagged with the byte
    offset of the call site for context.
    """
    data = b''.join(img[a - base:b - base] for a, b in blocks)
    p = subprocess.run(['ndisasm', '-b16', '-o0', '-'],
                       input=data, capture_output=True)
    out = []
    for x in p.stdout.decode('latin1').splitlines():
        f = x.split(None, 2)
        if len(f) != 3:
            continue
        text = f[2].strip()
        parts = text.split()
        mn = parts[0]
        if mn not in ('call', 'callf') and not mn.startswith('call'):
            continue
        arg = parts[-1]
        m = re.match(r'0x([0-9a-f]+):0x([0-9a-f]+)$', arg)
        if m:
            out.append(('far', int(m.group(1), 16), int(m.group(2), 16)))
            continue
        m = re.match(r'0x([0-9a-f]+)$', arg)
        if m:
            out.append(('near', int(m.group(1), 16)))
    return out


def seg_to_img_off(segpara, off):
    return segpara * 16 + off


def resolve_dev_callee(segname, segs, pubs, kind, tseg, toff, caller_lo):
    """dev call target -> owning public name (containing-or-before)."""
    if kind == 'far':
        # map relocated seg value back to a named segment: match any seg whose
        # image para equals tseg
        for sn, para in segs.items():
            if para == tseg:
                best = None
                for po, pn in pubs.get(sn, []):
                    if po <= toff:
                        best = pn
                    else:
                        break
                return best, sn
        return None, None
    # near: same segment as caller
    best = None
    for po, pn in pubs.get(segname, []):
        if po <= toff:
            best = pn
        else:
            break
    return best, segname


def gold_func_at(gr, tseg_abs, toff):
    """ranges_gold (ghidra seg) -> (seg,off) of the function containing target.

    Gold call targets are ghidra linear addresses: seg*16+off in ghidra space
    (== image para+0x1000 space).  Caller supplies absolute target.
    """
    key = (tseg_abs, toff)
    if key in gr:
        return key
    # find containing function start in same seg
    for (s, o), blocks in gr.items():
        if s == tseg_abs and o <= toff:
            return (s, o)
    return None


def main():
    gold_exe, gold_rng, my_exe, my_map, sweep = sys.argv[1:6]
    extras = [a.split('=', 1)[1] for a in sys.argv[6:] if a.startswith('--extra=')]
    equal_only = '--equal' in sys.argv[6:]
    gimg = load_image(gold_exe)
    gr = load_ranges(gold_rng)
    mim = load_image(my_exe)
    segs, funcs, pubs = load_mymap(my_map)

    matches = {}   # name -> gold (seg,off) in ghidra space
    for l in open(sweep):
        p = l.split()
        if len(p) >= 3 and p[1] == 'MATCH':
            s, o = p[2].lstrip('@').split(':')
            matches[p[0]] = (int(s, 16) + 0x1000, int(o, 16))
    nseed = len(matches)
    for ex in extras:
        for l in open(ex):
            p = l.split('\t')
            if len(p) >= 2 and re.match(r'[0-9a-f]+:[0-9a-f]+$', p[1].strip()):
                s, o = p[1].strip().split(':')
                matches.setdefault(p[0], (int(s, 16) + 0x1000, int(o, 16)))

    # gold: function start (ghidra seg,off) -> sorted for containment lookup
    gstarts = sorted(gr.keys())

    def gcontainer(tseg, toff):
        """function whose start is the greatest <= target in same seg."""
        best = None
        for s, o in gstarts:
            if s == tseg and o <= toff:
                best = (s, o)
            if s > tseg:
                break
        return best

    pairs = {}       # dev callee name -> [(gold seg,off, caller, idx)]
    for name, (gseg, goff) in sorted(matches.items()):
        if name not in funcs:
            continue
        segname, cpara, lo, hi = funcs[name]
        mblocks = [(cpara * 16 + lo + GHIDRA_BASE,
                    cpara * 16 + hi + GHIDRA_BASE)]
        mc = call_targets(mim, mblocks, GHIDRA_BASE)
        gkey = (gseg, goff)
        if gkey not in gr:
            continue
        gc = call_targets(gimg, gr[gkey], GHIDRA_BASE)
        if equal_only and len(mc) != len(gc):
            continue
        # positional pairing: same call-site index
        for i, (mk, mv) in enumerate(zip(mc, gc)):
            # dev target -> name
            if mk[0] == 'far':
                dn, dseg = resolve_dev_callee(segname, segs, pubs,
                                              'far', mk[1], mk[2], lo)
            else:
                dn, dseg = resolve_dev_callee(segname, segs, pubs,
                                              'near', None, mk[1], lo)
            if not dn:
                continue
            # gold target -> containing function start
            if mv[0] == 'far':
                gabs = mv[1]          # relocated para of target seg
                gofs = mv[2]
                # convert para to ghidra seg: ghidra seg = para+0x1000 only if
                # the image was loaded at 0x1000<<4 — ghidra segs are exactly
                # the loaded paragraphs, so ghidra_seg = para + 0x1000
                gkey2 = gcontainer(gabs + 0x1000, gofs)
            else:
                gkey2 = gcontainer(gseg, mv[1])
            if not gkey2:
                continue
            pairs.setdefault(dn, []).append((gkey2, name, i))

    out = 0
    for dn, lst in sorted(pairs.items()):
        addrs = collections.Counter(x[0] for x in lst)
        (gkey, cnt) = addrs.most_common(1)[0]
        tag = '' if len(addrs) == 1 else (' CONFLICT(%s)' %
                                          ','.join('%04x:%04x' % a
                                                   for a in addrs))
        print('%s\t%04x:%04x\tvia %s x%d%s' % (dn, gkey[0] - 0x1000, gkey[1],
                                              lst[0][1], cnt, tag))
        out += 1
    print('# %d call pairs from %d+%d seeds' % (out, nseed,
                                              len(matches) - nseed),
          file=sys.stderr)


if __name__ == '__main__':
    main()
