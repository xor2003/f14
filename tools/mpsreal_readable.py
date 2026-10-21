#!/usr/bin/env python3
"""mpsreal_readable.py IN.ASM OUT.ASM  -- decode byte-lift asm to readable MASM.

The lift stores most code as `db` streams between offset-named labels
(gXXXX/pXXXX).  This pass:

  * disassembles db/dw/dd runs with capstone into real mnemonics
  * resolves near branch/call targets back to their label names
  * resolves baked intra-segment far calls (9A seg:off) to `call far ptr
    <lbl>` so they carry a proper fixup instead of a hardwired para
  * merges `dw offset <sym>` operand-carriers back into their insn
  * passes mnemonic lines, data decls, labels, PUBLIC/EXTRN through
    verbatim, preserving order
"""
import re, sys
from capstone import Cs, CS_ARCH_X86, CS_MODE_16

MD = Cs(CS_ARCH_X86, CS_MODE_16)
MD.detail = True

def hd(v):
    return '0%xh' % v if v > 9 else '%d' % v

SEGTAG = {0x2e: 'cs:', 0x26: 'es:', 0x3e: 'ds:', 0x36: 'ss:', 0x64: 'fs:', 0x65: 'gs:'}

def addtag(ops, tag):
    """Insert seg override before the memory bracket: word ptr cs:[x].
    A bare `[imm]` (no register) needs an explicit ds: in MASM 5.x or it
    parses as an immediate operand."""
    if '[' not in ops:
        return ops
    mem = ops[ops.index('['):ops.index(']') + 1]
    if tag and tag not in ops:
        ops = ops.replace('[', tag + '[', 1)
    elif not tag and not re.search(r'[bs]x|[sb]p|[sd]i', mem):
        # absolute [disp16]: MASM wants ds:[imm]
        if not re.search(r'(ds|cs|es|ss|fs|gs):', ops):
            ops = ops.replace('[', 'ds:[', 1)
    return ops

STROP = {('lods','word'):'lodsw', ('lods','byte'):'lodsb',
         ('stos','word'):'stosw', ('stos','byte'):'stosb',
         ('movs','word'):'movsw', ('movs','byte'):'movsb',
         ('scas','word'):'scasw', ('scas','byte'):'scasb',
         ('cmps','word'):'cmpsw', ('cmps','byte'):'cmpsb',
         ('ins','word'):'insw', ('ins','byte'):'insb',
         ('outs','word'):'outsw', ('outs','byte'):'outsb'}

def norm_ops(ins):
    ops = ins.op_str
    m = ins.mnemonic
    if m in STROP:
        mm = re.match(r'(word|byte) ptr', ops)
        if mm:
            rep = ('rep ' if ins.prefix[0] == 0xf3 else
                   'repne ' if ins.prefix[0] == 0xf2 else '')
            return rep + STROP[(m, mm.group(1))]
        return m
    # capstone prints xchg as (rm,reg) but MASM encodes operand1 into the
    # reg field -- swap to re-emit gold's byte order for xchg reg,reg
    if m == 'xchg' and ',' in ops and ins.bytes and (ins.bytes[-1] & 0xc0) == 0xc0 \
       and not any('[' in o for o in ops.split(',')):
        a_, b_ = ops.split(',', 1)
        ops = '%s, %s' % (b_.strip(), a_.strip())
    ops = re.sub(r'0x([0-9a-f]+)', lambda mm: hd(int(mm.group(1), 16)), ops)
    if m in ('les', 'lds') and ' ptr [' in ops:
        ops = ops.replace(' ptr [', ' dword ptr [')
    if m in ('lcall', 'ljmp'):
        m = 'call' if m == 'lcall' else 'jmp'
        if 'ptr' not in ops:
            ops = 'dword ptr ' + ops
    ops = addtag(ops, SEGTAG.get(ins.prefix[0]))
    return ('%s\t%s' % (m, ops)).rstrip()

def sym_of(arg):
    a = arg.strip()
    m = re.match(r'offset\s+(\w+)', a, re.I)
    if m: return 'offset', m.group(1)
    m = re.match(r'seg\s+(\w+)', a, re.I)
    if m: return 'seg', m.group(1)
    return 'plain', a

def render_sym(ins, kind, sym):
    """Re-render insn with its tail disp/imm operand replaced by sym."""
    ops = ins.op_str
    new = re.sub(r'(0x[0-9a-f]+|\d+)(\])?\s*$',
                 lambda m: sym + (m.group(2) or ''), ops)
    new = re.sub(r'0x([0-9a-f]+)', lambda m: hd(int(m.group(1), 16)), new)
    new = addtag(new, SEGTAG.get(ins.prefix[0]))
    if kind == 'seg':
        new = new.replace(sym, 'seg ' + sym)
    return ('%s\t%s' % (ins.mnemonic, new)).rstrip()

def parse_db(argstr):
    """'08Bh,0ECh' -> bytes; None if not pure byte literals."""
    out = bytearray()
    for a in argstr.split(','):
        a = a.strip()
        m = re.fullmatch(r'0?([0-9a-fA-F]{1,2})[hH]', a) or re.fullmatch(r'0x([0-9a-fA-F]{1,2})', a)
        if m:
            out.append(int(m.group(1), 16)); continue
        if re.fullmatch(r'-?\d+', a):
            out.append(int(a) & 0xff); continue
        return None
    return bytes(out)

def emit_run(run, labels, seglo, seghi, pre, emitted, refs, out, farset=None):
    """run = list of (off|None, kind, payload). Emit decoded lines.

    Positions: anchor from the first known off, else from the last known
    off minus bytes after it.  items are contiguous in memory (file
    order == emission order in the lift).  `labels` may gain entries for
    branch targets that had no label yet.
    """
    # total byte length + index of first/last anchored item
    size = []
    for k, off, v, ln_ in run:
        size.append(len(v) if k == 'bytes' else (2 if k == 'dw' else 4))
    anchor = None   # (index, off) preferring the first known
    for i, (k, off, v, ln_) in enumerate(run):
        if off is not None:
            anchor = (i, off); break
    if anchor is None:
        for i in range(len(run) - 1, -1, -1):
            if run[i][1] is not None:
                anchor = (i, run[i][1]); break
    if anchor is None:
        # no position info at all -> keep raw db/dw/dd verbatim
        for k, off, v, ln_ in run:
            if k == 'bytes':
                out.append('\tdb\t%s' % ','.join('0%02xh' % b for b in v))
            else:
                out.append('\t%s\t%s' % (k, v))
        return
    base = anchor[1] - sum(size[:anchor[0]])
    buf = bytearray()
    pos2item = []
    p = 0
    for (k, off, v, ln_), ln in zip(run, size):
        pos2item.append((p, k, v)); p += ln
        buf += v if k == 'bytes' else (b'\0\0' if k == 'dw' else b'\0\0\0\0')
    # disasm the whole buffer
    TERM = {0xc2, 0xc3, 0xca, 0xcb, 0xcf, 0xcc, 0xea, 0xe9, 0xeb}  # ret/iret/int3/jmp*
    dead = False
    ip = 0
    while ip < len(buf):
        a = base + ip
        # any labels landing exactly here?
        if a in labels:
            nms = labels.pop(a)
            for nm in nms:
                out.append('%s:' % nm)
                emitted.setdefault(a, nm)
            dead = False
        if dead:
            nxt = len(buf)
            for p, k, v in pos2item:
                if p > ip:
                    nxt = min(nxt, p); break
            it = item_start_at(pos2item, ip, ('dw', 'dd'))
            if it:
                out.append('\t%s\t%s' % (pos2item_kind(pos2item, ip),
                                         dwmap_arg(pos2item, ip)))
                ip += 4 if pos2item_kind(pos2item, ip) == 'dd' else 2
            else:
                out.append('\tdb\t%s' % ','.join('0%02xh' % b for b in buf[ip:nxt]))
                ip = nxt
            continue
        kind_at = pos2item_kind(pos2item, ip)
        insns = list(MD.disasm(bytes(buf[ip:]), a, count=1))
        if not insns or insns[0].size == 0:
            out.append('\tdb\t0%02xh' % buf[ip]); ip += 1; continue
        ins = insns[0]
        # jump-table pad: '00 00' pairs after far cells decode as a
        # spurious `add [bx+si],al` -- keep them as bytes
        if ins.size == 2 and ins.bytes == b'\x00\x00':
            out.append('\tdb\t000h,000h')
            ip += 2; continue
        end = ip + ins.size
        if end > len(buf):
            out.append('\tdb\t%s' % ','.join('0%02xh' % b for b in buf[ip:]))
            break
        # dw/dd item boundary inside or at insn start?
        dwpos = item_start_in(pos2item, ip + 1, end)
        if item_start_at(pos2item, ip, ('dw', 'dd')):
            arg = dwmap_arg(pos2item, ip)
            sz = 4 if kind_at == 'dd' else 2
            out.append('\t%s\t%s' % ('dd' if sz == 4 else 'dw', arg))
            ip += sz; continue
        if dwpos is not None and dwpos >= end - 4 and dwpos >= ip + 1:
            # operand-carrier: dw/dd is tail of this insn
            arg = dwmap_arg(pos2item, dwpos)
            kind, sym = sym_of(arg)
            if ins.size == (dwpos - ip) + 4 and item_start_at(pos2item, dwpos, ('dd',)):
                out.append('\t' + render_sym(ins, 'offset', arg if kind == 'plain' else sym))
            else:
                out.append('\t' + render_sym(ins, kind, sym))
            ip = dwpos + (4 if item_start_at(pos2item, dwpos, ('dd',)) else 2)
            continue
        b0 = ins.bytes[0]
        ops = ins.op_str
        # 386+ instructions MASM 5.x can't emit -> keep raw bytes
        if ins.mnemonic in ('cdq', 'cwde') or 0x66 in ins.prefix:
            out.append('\tdb\t%s\t; %s %s' % (
                ','.join('0%02xh' % x for x in ins.bytes),
                ins.mnemonic, ops))
            ip += ins.size; continue
        if b0 in (0xe8, 0xe9, 0xeb, 0xe0, 0xe1, 0xe2, 0xe3) or 0x70 <= b0 <= 0x7f or \
           (b0 == 0x0f and 0x80 <= ins.bytes[1] <= 0x8f):
            tgt = ins.operands[-1].imm
            if seglo <= tgt < seghi:
                nm = labels.get(tgt)
                if not nm and tgt in emitted:
                    nm = [emitted[tgt]]
                newlbl = None
                if nm:
                    t = nm[0]
                else:
                    t = '%s%x' % (pre, tgt)
                    labels[tgt] = [t]
                    newlbl = tgt
                isfar = farset and t in farset
                if b0 == 0xeb:
                    out.append('\tjmp short\t%s' % t)
                elif b0 == 0xe9 and -129 < (tgt - (a + 3)) < 128:
                    # MASM shrinks reachable `jmp` to EB -- force E9 via
                    # explicit rel16 so the encoding stays 3 bytes.
                    out.append('\tdb\t0e9h')
                    out.append('\tdw\t%s-$-2\t; jmp %s' % (t, t))
                elif b0 == 0xe8 and isfar:
                    # `call <far label>` would assemble to 9A; keep E8
                    out.append('\tcall near ptr\t%s' % t)
                elif b0 == 0xe9 and isfar:
                    out.append('\tjmp near ptr\t%s' % t)
                else:
                    out.append('\t%s\t%s' % (ins.mnemonic, t))
                if newlbl is not None:
                    refs.setdefault(newlbl, []).append(
                        (len(out) - 1, ins.mnemonic, bytes(ins.bytes)))
            else:
                out.append('\tdb\t%s\t; %s %s' % (
                    ','.join('0%02xh' % x for x in ins.bytes),
                    ins.mnemonic, ops))
            if b0 in TERM:
                dead = True
            ip += ins.size; continue
        if b0 in (0x9a, 0xea) and ins.size == 5:
            try:
                sg = int(ops.split(':')[0], 16); oo = int(ops.split(':')[1], 16)
            except (ValueError, IndexError):
                sg = oo = -1
            lin = sg * 16 + oo
            if sg == 0 and oo == 0:
                out.append('\tdb\t%s\t; far %s cell, patched at runtime'
                           % (','.join('0%02xh' % x for x in ins.bytes),
                              'call' if b0 == 0x9a else 'jmp'))
            elif seglo <= lin < seghi:
                nm = labels.get(lin)
                t = nm[0] if nm else emitted.get(lin, '%s%x' % (pre, lin))
                if not nm and lin not in emitted:
                    labels[lin] = [t]
                out.append('\t%s\tfar ptr %s' % ('call' if b0 == 0x9a else 'jmp', t))
            else:
                out.append('\tdb\t%s\t; far %s %s' % (
                    ','.join('0%02xh' % x for x in ins.bytes),
                    'call' if b0 == 0x9a else 'jmp', ops))
            if b0 in TERM:
                dead = True
            ip += ins.size; continue
        out.append('\t' + norm_ops(ins))
        if b0 in TERM:
            dead = True
        ip += ins.size

def pos2item_kind(pos2item, bp):
    for p, k, v in pos2item:
        if p == bp: return k
    return None

def item_start_at(pos2item, bp, kinds):
    for p, k, v in pos2item:
        if p == bp and k in kinds: return True
    return False

def item_start_in(pos2item, lo, hi):
    for p, k, v in pos2item:
        if lo <= p < hi and k in ('dw', 'dd'):
            return p
    return None

def dwmap_arg(pos2item, bp):
    for p, k, v in pos2item:
        if p == bp: return v
    return ''

def main():
    src, dst = sys.argv[1], sys.argv[2]
    items = []          # (kind, off, val, line)
    curseg = None; pos = None; pendpub = []
    for raw in open(src, encoding='latin1'):
        line = raw.rstrip('\n'); s = line.strip()
        if not s or s.startswith(';'):
            continue
        m = re.match(r'(\w+)\s+SEGMENT\b(.*)', s)
        if m:
            items.append(('seg', None, s, line)); curseg = m.group(1); pos = None; continue
        if re.match(r'(\w+)\s+ENDS', s):
            items.append(('ends', None, s, line)); curseg = None; continue
        if curseg is None:
            items.append(('top', None, None, line)); continue
        m = re.match(r'([A-Za-z_?@$][\w?@$.]*)\s*:', s)
        if m:
            nm = m.group(1)
            if re.fullmatch(r'[gp][0-9a-f]+', nm):
                pos = int(nm[1:], 16)
            items.append(('label', pos, nm, line)); continue
        m = re.match(r'([A-Za-z_?@$][\w?@$.]*)\s+LABEL\s+(\w+)', s, re.I)
        if m:
            items.append(('nlabel', pos, (m.group(1), m.group(2).upper(), list(pendpub)), line))
            pendpub = []; continue
        m = re.match(r'PUBLIC\s+(.*)', s, re.I)
        if m:
            pendpub += [x.strip() for x in m.group(1).split(',')]; continue
        m = re.match(r'db\s+([^;]+)', s, re.I)
        if m:
            bs = parse_db(m.group(1))
            if bs is None:
                items.append(('mn', pos, s, line)); pos = None
            else:
                items.append(('bytes', pos, bs, line))
                if pos is not None: pos += len(bs)
            continue
        m = re.match(r'dw\s+(.+)', s, re.I)
        if m:
            items.append(('dw', pos, m.group(1), line))
            if pos is not None: pos += 2
            continue
        m = re.match(r'dd\s+(.+)', s, re.I)
        if m:
            items.append(('dd', pos, m.group(1), line))
            if pos is not None: pos += 4
            continue
        items.append(('mn', pos, s, line))
        pos = None

    # second pass: emit
    out = []
    i = 0
    while i < len(items):
        k, off, v, line = items[i]
        if k in ('seg', 'ends', 'top'):
            if k == 'seg' and '.286' not in '\n'.join(out):
                out.append('.286')
            out.append(line); i += 1; continue
        # collect segment body
        body = []
        while i < len(items) and items[i][0] not in ('seg', 'ends', 'top'):
            body.append(items[i]); i += 1
        # label map for the segment
        labels = {}
        for b in body:
            if b[0] in ('label', 'nlabel') and b[1] is not None:
                nm = b[2] if b[0] == 'label' else b[2][0]
                labels.setdefault(b[1], []).append(nm)
        seglo = min(labels) if labels else 0
        seghi = max(labels) + 0x400 if labels else 0
        farset = {b[2][0] for b in body
                  if b[0] == 'nlabel' and b[2][1] == 'FAR'}
        # segment label prefix for newly-created targets
        pre = 'p' if any(x[0] == 'label' and x[2].startswith('p') for x in body) else 'g'

        # walk body: contiguous byte items (regardless of anchoring) form
        # a run; label/nlabel/mn items interrupt runs
        j = 0
        emitted = {}
        refs = {}
        created = set()
        while j < len(body):
            b = body[j]
            if b[0] in ('bytes', 'dw', 'dd'):
                run = [b]; j += 1
                while j < len(body) and body[j][0] in ('bytes', 'dw', 'dd'):
                    run.append(body[j]); j += 1
                emit_run(run, labels, seglo, seghi, pre, emitted, refs, out, farset)
                continue
            if b[0] == 'label':
                out.append('%s:' % b[2])
                if b[1] is not None:
                    emitted.setdefault(b[1], b[2])
                    if b[1] in labels and b[2] in labels[b[1]]:
                        labels[b[1]].remove(b[2])
                        if not labels[b[1]]:
                            del labels[b[1]]
                j += 1; continue
            if b[0] == 'nlabel':
                nm, kind, pubs = b[2]
                for p in pubs:
                    out.append('\tPUBLIC\t%s' % p)
                out.append('%s\tLABEL %s' % (nm, kind))
                if b[1] is not None:
                    emitted.setdefault(b[1], nm)
                    if b[1] in labels and nm in labels[b[1]]:
                        labels[b[1]].remove(nm)
                        if not labels[b[1]]:
                            del labels[b[1]]
                j += 1; continue
            # mn / other items: labels may point at their position
            if b[1] is not None and b[1] in labels:
                for nm in labels.pop(b[1]):
                    out.append('%s:' % nm)
                    emitted.setdefault(b[1], nm)
            out.append('\t' + b[2]); j += 1
        # created labels never emitted -> rewrite referencing lines to
        # raw bytes so nothing references an undefined symbol
        for loff, rlist in refs.items():
            if loff in labels:      # never emitted -> dangling
                for oi, mn, bs in rlist:
                    out[oi] = '\tdb\t%s\t; %s %s%x (unresolved)' % (
                        ','.join('0%02xh' % x for x in bs), mn, pre, loff)
        for loff in sorted(labels):
            for nm in labels[loff]:
                out.append('; UNEMITTED-LABEL %s: at %x' % (nm, loff))
    open(dst, 'w').write('\n'.join(out) + '\n')
    print('wrote', dst, len(out), 'lines')

if __name__ == '__main__':
    main()
