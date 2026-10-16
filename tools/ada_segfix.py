#!/usr/bin/env python3
# ada_segfix.py — re-segment an ada_script .asm along real segment boundaries.
#
# tools.ada_script flattens the image into seg000_N fragments, which breaks
# `call near ptr X` for callees in other fragments.  The original exe's real
# segments (from Ghidra memmap / LINK map) never exceed 64KB and near calls
# stay inside them.
#
# usage: ada_segfix.py F14GOLD.asm memmap_gold.txt IMAGE.EXE > out.asm
#
# Every emitted asm line carries "@<imgoff>:<len>" — we track the emission
# address and split segments at real-segment boundaries.  Two hazards:
#  - `name equ $+K` labels (mid-instruction entry points): the following
#    instruction is emitted as raw db bytes so the label can sit inside it.
#  - segment switches are deferred while inside a proc (block nesting).

import re
import sys

ADDR = re.compile(r'@([0-9A-Fa-f]+):([0-9A-Fa-f]+)\b')
SEGOPEN = re.compile(r'^\s*(\S+)\s+segment\b', re.I)
SEGEND = re.compile(r'^\s*(\S+)\s+ends\b', re.I)
EQU = re.compile(r'^(\S+)\s+equ\s+\$\+([0-9A-Fa-f]+)\s*$')
PROC = re.compile(r'^\s*(\S+)\s+proc\s+(near|far)', re.I)
ENDP = re.compile(r'^\s*(\S+)\s+endp\b', re.I)
LBL = re.compile(r'^(?:sub|loc|byte|word|dword|off|unk|FUN)_?([0-9A-Fa-f]{4,5})\b')


def load_bounds(path):
    regs = []
    for l in open(path):
        m = re.match(r'(CODE\S*)\s+([0-9a-f]+):([0-9a-f]+)\s+([0-9a-f]+):([0-9a-f]+)', l)
        if m:
            lo = int(m.group(2), 16) * 16 + int(m.group(3), 16) - 0x10000
            hi = int(m.group(4), 16) * 16 + int(m.group(5), 16) - 0x10000
            regs.append((lo, hi, m.group(1)))
    regs.sort()
    return regs


def region_of(regs, addr):
    for lo, hi, nm in regs:
        if lo <= addr <= hi:
            return nm
    return None


def main():
    asm_path, memmap_path, exe_path = sys.argv[1:4]
    regs = load_bounds(memmap_path)
    raw = open(exe_path, 'rb').read()
    img = raw[int.from_bytes(raw[8:10], 'little') * 16:]
    # original MZ relocation sites (image-relative offsets of seg words)
    relset = set()
    nrel = int.from_bytes(raw[6:8], 'little')
    roff = int.from_bytes(raw[0x18:0x1a], 'little')
    for r in range(nrel):
        ro, rs = int.from_bytes(raw[roff + r * 4:roff + r * 4 + 2], 'little'), \
            int.from_bytes(raw[roff + r * 4 + 2:roff + r * 4 + 4], 'little')
        relset.add(rs * 16 + ro)

    # original stack window: ss:sp -> [ss*16, ss*16 + sp) must be emitted as
    # a `stack`-combine segment so the linker writes a real SS:SP header
    stack_base = int.from_bytes(raw[0xe:0x10], 'little') * 16
    stack_top = stack_base + int.from_bytes(raw[0x10:0x12], 'little')

    lines = open(asm_path).read().splitlines()

    # pass 1: ada fragment name -> base addr (first @addr after its header)
    frag_base = {}
    cur_frag = None
    for l in lines:
        m = SEGOPEN.match(l)
        if m:
            cur_frag = m.group(1)
            continue
        if cur_frag and not frag_base.get(cur_frag):
            a = ADDR.search(l)
            if a:
                frag_base[cur_frag] = int(a.group(1), 16)

    def want_seg(addr):
        reg = region_of(regs, addr)
        if reg:
            return reg
        if stack_base <= addr < stack_top:
            return 'stack_seg'
        # a gap window revisited after stack_seg must not reuse the
        # earlier name — same-named segments concatenate in the obj and
        # would place the tail before the stack
        suffix = 'b' if addr >= stack_top else ''
        return 'gap_%05x%s' % ((addr & ~0xffff), suffix)

    def bound_after(addr):
        # next segment-relevant boundary above addr: 64K window edge,
        # a real region start, or a stack-window edge
        cands = [(addr | 0xffff) + 1, stack_base, stack_top]
        cands += [lo for lo, hi, nm in regs]
        return min(x for x in cands if x > addr)

    def newname(old, a_addr=None, off_word=None):
        """ada fragment name -> region whose para LINK should emit.
        Prefer the region whose paragraph == original word (byte-exact
        value); else decode the far pointer target; else fragment base."""
        if a_addr is not None:
            ow = int.from_bytes(img[a_addr:a_addr + 2], 'little')
            for lo, hi, nm in regs:
                if lo // 16 == ow:
                    return nm
            if off_word is not None:
                tgt = ow * 16 + off_word
                reg = region_of(regs, tgt)
                if reg:
                    return reg
            return None
        a = frag_base.get(old)
        if a is None:
            return old
        reg = region_of(regs, a)
        return reg or ('gap_%05x' % (a & ~0xffff))

    # pass 1b: referenced but undefined code labels (dseg-region routines are
    # emitted as db bytes with no label).  name encodes linear+0x10000.
    defined = set()
    wanted = set()
    for l in lines:
        lm = re.match(r'^([a-zA-Z_]\w*)\s*(?:::|:|label|proc|equ)', l)
        if lm:
            defined.add(lm.group(1))
        for m in re.finditer(r'\b(?:call|jmp\w*)\s+(?:\w+\s+ptr\s+)?(sub_\w+|loc_\w+)', l):
            wanted.add(m.group(1))
    needed = {w: int(w.split('_')[1], 16) - 0x10000
              for w in wanted - defined}

    out = []
    cur = None
    in_seg = False
    seg_off = {}          # emitted bytes per segment name
    seg_base = {}         # region base addr per segment name
    proc_depth = 0
    cur_proc = None

    def emit_db(bs):
        bs = list(bs)
        j = 0
        while j < len(bs):
            run = 1
            while j + run < len(bs) and bs[j + run] == bs[j]:
                run += 1
            if run >= 8:
                out.append('\tdb 0%xh dup(0%02xh)' % (run, bs[j]))
                j += run
            else:
                chunk = []
                while j < len(bs) and len(chunk) < 48:
                    run = 1
                    while j + run < len(bs) and bs[j + run] == bs[j]:
                        run += 1
                    if run >= 8:
                        break
                    chunk.extend(bs[j:j + run])
                    j += run
                if chunk:
                    out.append('\tdb %s' % ','.join(
                        '0%02xh' % b for b in chunk))
    pending_boundary = None   # seg name deferred until proc closes
    skip_to = 0               # image pos already covered by an emitted dw
    pend_off = 0              # bytes emitted by unaddressed continuation lines

    def est_bytes(line):
        cp = line.split(';')[0]
        m = re.match(r'\s*(dw|dd|db)\b(.*)', cp)
        if not m:
            return 0
        sz = 2 if m.group(1) == 'dw' else 4 if m.group(1) == 'dd' else 1
        dm = re.search(r'(\d+)\s*dup\s*\(', m.group(2))
        if dm:
            return int(dm.group(1)) * sz
        rest = m.group(2).strip()
        return (rest.count(',') + (1 if rest else 0)) * sz
    early_closed = set()      # procs closed early by a straddling label

    def open_seg(name):
        nonlocal cur, in_seg
        if name == 'stack_seg':
            out.append('%s\tsegment byte stack \'STACK\' use16' % name)
        else:
            out.append('%s\tsegment byte public \'CODE\' use16' % name)
        out.append('\tassume cs:%s' % name)
        out.append('\tassume ds:nothing, es:nothing, ss:nothing')
        cur = name
        in_seg = True

    def close_seg():
        nonlocal in_seg
        if in_seg:
            out.append('%s\tends' % cur)
            in_seg = False

    def base_of(name):
        if name == 'stack_seg':
            return stack_base
        if name.startswith('gap_'):
            return int(name[4:].rstrip('b'), 16)
        for lo, hi, nm in regs:
            if nm == name:
                return lo
        return 0

    def org_to(addr):
        # position the location counter: ada emits the data/BSS region
        # non-monotonically, so `org` is needed when @addr jumps.
        # gap segments anchor at their first emitted address — their name
        # is nominal; basing at the 64K window start would pad the segment
        # with uninitialized length and push following segments late.
        base = seg_base.setdefault(
            cur, addr if cur.startswith('gap_') else base_of(cur))
        off = addr - base
        if off != seg_off.get(cur, 0):
            out.append('\torg 0%xh ; @%x' % (off, addr))
        seg_off[cur] = off

    i = 0
    while i < len(lines):
        l = lines[i]
        if SEGOPEN.match(l) or SEGEND.match(l):
            i += 1
            continue
        if re.match(r'^\s*assume\b', l, re.I):
            i += 1
            continue
        if re.match(r'^\s*end\b', l, re.I):
            close_seg()
            out.append(l)
            i += 1
            continue
        a = ADDR.search(l)
        if a:
            addr0 = int(a.group(1), 16)
            if addr0 < skip_to:
                # already emitted as part of a straddling reloc word
                i += 1
                continue
            for w, wa in needed.items():
                if wa == addr0:
                    want_w = want_seg(wa)
                    if want_w != cur:
                        close_seg()
                        open_seg(want_w)
                    out.append('%s label near ; added: undefined ref' % w)
        if not a and not proc_depth:
            # label/proc names encode linear+0x10000
            lm = LBL.match(l)
            if lm:
                addr = int(lm.group(1), 16) - 0x10000
                want = want_seg(addr)
                if want != cur:
                    close_seg()
                    open_seg(want)
        if a:
            addr = int(a.group(1), 16)
            alen = int(a.group(2))
            want = want_seg(addr)
            if want != cur:
                if proc_depth:
                    # ada leaves some procs unterminated into the data
                    # region — close it so the boundary can open
                    out.append('%s endp ; auto-close at seg boundary'
                               % cur_proc)
                    early_closed.add(cur_proc)
                    proc_depth = 0
                close_seg()
                open_seg(want)
            org_to(addr)
            seg_off[cur] = addr - seg_base.setdefault(
                cur, base_of(cur)) + alen
            pend_off = 0
            # a `dup(?)` line spanning the segment's 64K window: split it
            # across successive gap segments (uninitialized — no content)
            reg_end = region_of(regs, addr + alen - 1)
            want_end = reg_end or want_seg(addr + alen - 1)
            if want_end != cur and re.search(r'dup\s*\(\s*\?\s*\)', l):
                first = bound_after(addr) - addr
                out.append('\tdb 0%xh dup(?) ; split @%x' % (first, addr))
                seg_off[cur] = addr - seg_base[cur] + first
                pend_off = 0
                lines.insert(i + 1, '\tdb 0%xh dup(?) ; @%x:%d'
                             % (alen - first, addr + first, alen - first))
                i += 1
                continue
            if reg_end and reg_end != want and not proc_depth:
                # line straddles a segment boundary — emit bytes per side
                bound = base_of(reg_end)
                out.append('; %s ; split at seg boundary' % l.strip())
                emit_db(img[addr:bound])
                close_seg()
                open_seg(reg_end)
                emit_db(img[bound:addr + alen])
                seg_off[cur] = addr + alen - base_of(cur)
                pend_off = 0
                i += 1
                continue
        em = EQU.match(l)
        if em:
            # `name equ $+K` is a plain label K bytes ahead — verbatim is
            # correct MASM unless the label lands across a segment boundary
            # while we are inside a proc (boundary bytes must move segs).
            name = em.group(1)
            lm = LBL.match(name)
            addr = int(lm.group(1), 16) - 0x10000 if lm else None
            want = want_seg(addr) if addr is not None else None
            if want and want != cur and proc_depth:
                # close proc+seg early, split the straddling instruction
                out.append('; %s' % l.strip())
                na = ADDR.search(lines[i + 1]) if i + 1 < len(lines) \
                    else None
                a0 = int(na.group(1), 16) if na else None
                aln = int(na.group(2)) if na else None
                if a0 is not None and a0 <= addr < a0 + aln:
                    # the boundary itself may be mid-instruction: bytes
                    # [a0, bound) stay in the old seg, bytes [bound, addr)
                    # open the new seg ahead of the label
                    bound = base_of(want)
                    pre_old = bound - a0
                    if pre_old:
                        out.append('\tdb %s' % ','.join(
                            '0%02xh' % b for b in img[a0:bound]))
                    out.append('%s endp ; moved up for %s'
                               % (cur_proc, want))
                    early_closed.add(cur_proc)
                    proc_depth = 0
                    close_seg()
                    open_seg(want)
                    if addr > bound:
                        out.append('\tdb %s' % ','.join(
                            '0%02xh' % b for b in img[bound:addr]))
                    out.append('%s label near' % name)
                    if a0 + aln > addr:
                        out.append('\tdb %s' % ','.join(
                            '0%02xh' % b for b in img[addr:a0 + aln]))
                    seg_off[cur] = a0 + aln - base_of(want)
                    pend_off = 0
                    i += 2
                    continue
                out.append('%s endp ; moved up for %s' % (cur_proc, want))
                early_closed.add(cur_proc)
                proc_depth = 0
                close_seg()
                open_seg(want)
                out.append('%s label near' % name)
                i += 1
                continue
            if want and want != cur and not proc_depth:
                close_seg()
                open_seg(want)
            out.append(l)
            i += 1
            continue
        # any addressed line covering an original reloc site gets its seg
        # word emitted as `dw seg <region>` so the linker creates a real
        # relocation entry (ada bakes them into db runs as literals)
        code_part = l.split(';')[0]
        if a and not re.search(r'\bseg\s+(?:seg|dseg)', code_part):
            a_addr = int(a.group(1), 16)
            alen = int(a.group(2))
            sites = sorted(r for r in relset if a_addr <= r < a_addr + alen)
            # a site at a region's last byte straddles into the next
            # segment — emit literal bytes (image-exact; the reloc table
            # loses this one entry)
            sites = [s for s in sites
                     if region_of(regs, s) == region_of(regs, s + 1)]
            if sites:
                pos = a_addr
                out.append('; %s' % l.strip())
                for s in sites:
                    if s > pos:
                        emit_db(img[pos:s])
                    ow = int.from_bytes(img[s:s + 2], 'little')
                    offw = int.from_bytes(img[s - 2:s], 'little')
                    nm = newname(None, s, offw)
                    if nm:
                        out.append('\tdw seg %s ; reloc @%x' % (nm, s))
                    else:
                        out.append('\tdw 0%xh ; reloc @%x (unmatched)'
                                   % (ow, s))
                    pos = s + 2
                if a_addr + alen > pos:
                    emit_db(img[pos:a_addr + alen])
                if pos > a_addr + alen:
                    # the last reloc word straddled into following lines
                    skip_to = pos
                i += 1
                continue
        # unaddressed continuation `dw seg X` (dword decls): its site is
        # the tracked position — literal word when the original had no
        # reloc there
        if not a and re.search(r'\bdw\s+seg\b', code_part):
            site = seg_base.get(cur, 0) + seg_off.get(cur, 0) + pend_off
            ow = int.from_bytes(img[site:site + 2], 'little')
            offw = int.from_bytes(img[site - 2:site], 'little')
            nm = newname(None, site, offw) if site in relset else None
            if nm:
                out.append('\tdw seg %s ; reloc @%x' % (nm, site))
            else:
                out.append('\tdw 0%xh ; no reloc @%x' % (ow, site))
            pend_off += 2
            i += 1
            continue
        # `dw seg X` paired with preceding `dw <off>` = far pointer;
        # locate the seg word exactly via the original reloc table
        off_word = None
        seg_addr = None
        if re.search(r'\bseg\s+(?:seg|dseg)', code_part) and a:
            a_addr = int(a.group(1), 16)
            alen = int(a.group(2))
            cand = [r for r in relset if a_addr <= r < a_addr + alen]
            if not cand:
                # no real reloc at this site — the original was a plain
                # word whose value merely equals a segment para
                out.append('\tdw 0%xh ; no reloc @%x' % (
                    int.from_bytes(img[a_addr:a_addr + 2], 'little'),
                    a_addr))
                i += 1
                continue
            seg_addr = min(cand)
            pl = out[-1] if out else ''
            pm3 = re.search(r'\bdw\s+([0-9A-Fa-f]+)h\b.*@([0-9A-Fa-f]+):',
                            pl)
            if pm3:
                off_word = int(pm3.group(1), 16)
        def segsub(m):
            nm = newname(m.group(2), seg_addr, off_word)
            return '%s %s' % (m.group(1), nm) if nm else m.group(0)
        rewritten = re.sub(
            r'\b(seg)\s+((?:seg|dseg)\d*_\d+|dseg|seg_stack)\b',
            segsub, l)
        if a and re.search(r'\bseg\s+(?:seg|dseg)\d*_\d+\b',
                           rewritten.split(';')[0]):
            # unresolved operand — emit literal bytes instead
            out.append('; %s' % l.strip())
            emit_db(img[int(a.group(1), 16):
                        int(a.group(1), 16) + int(a.group(2))])
            i += 1
            continue
        out.append(rewritten)
        if not a:
            pend_off += est_bytes(rewritten)
        i += 1
        pm = PROC.match(l)
        em2 = ENDP.match(l)
        if pm:
            proc_depth += 1
            cur_proc = pm.group(1)
        elif em2:
            if em2.group(1) in early_closed:
                out.pop()
                out.append('; %s (already closed)' % l.strip())
            else:
                proc_depth = max(0, proc_depth - 1)
        if proc_depth == 0 and pending_boundary:
            close_seg()
            open_seg(pending_boundary)
            pending_boundary = None
    close_seg()
    print('\n'.join(out))


if __name__ == '__main__':
    main()
