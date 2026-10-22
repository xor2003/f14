#!/usr/bin/env python3
"""Unpack the EARLY Microsoft EXEPACK variant used by F.EXE (F-14 Fleet
Defender front-end). This is the bit-level LZ format from LINK.EXE /E
(v1-era), NOT the later 0xB0/0xB2 RLE format that w4kfu/unEXEPACK expects
(theirs requires the 0x4252 'RB' signature which this variant lacks).

Layout of this variant (from the bundled stub's own disassembly):
  MZ hdr (e_cparhdr*16 bytes)
  packed data block        (size = word [hdr_vars+8] * 16)
  14-byte vars at (e_cparhdr + e_cs)*16:
     +0 real_ip, +2 real_cs, +4 real_sp, +6 real_ss,
     +8 packed_len_para, +0xa dest_delta, +0xc trailer_len
  unpacker code            (entry = vars_start + 0xe)
  reloc delta stream       (read by stub; EMPTY in F.EXE -> "00 01 00")

Bit stream (read FORWARD, LSB-first per u16; refill is EAGER after the
16th bit -- refills consume 2 stream bytes before the pending dispatch):
  bit=1 -> literal byte
  bit=0 -> match; next bit selects form:
     1 -> short: read u16 w; disp = ((w>>8)>>3 | 0xe0)<<8 | (w&0xff) - 0x10000;
          len_field = (w>>8)&7;  if len_field: len = len_field+2
          else read byte al: 0=end, 1=seg-wrap(no-op linearly), else len=al+1
     0 -> long: len = (2 bits)+2;  disp = byte - 256
  match copies 'len' bytes from current_out + disp (overlaps allowed).

Verified byte-identical against the exe's own unpacker running under
kvikdos (--hlt-dump of guest memory after patching the final
'jmp far cs:[bx]' -> 'hlt'; unpacked image compared at PSP+0x100).

Usage: exepack_unpack.py <packed.exe> <out.exe>
"""
import struct, sys

def unpack(src):
    out = bytearray()
    pos = 0
    bp = src[0] | (src[1] << 8); pos = 2; nb = 16
    def bit():
        nonlocal pos, bp, nb
        b = bp & 1; bp >>= 1; nb -= 1
        if nb == 0:
            bp = src[pos] | (src[pos+1] << 8); pos += 2; nb = 16
        return b
    while True:
        if bit() == 1:
            out.append(src[pos]); pos += 1
            continue
        if bit() == 1:
            w = src[pos] | (src[pos+1] << 8); pos += 2
            disp = ((((w >> 8) >> 3) | 0xe0) << 8 | (w & 0xff)) - 0x10000
            lf = (w >> 8) & 7
            if lf == 0:
                al = src[pos]; pos += 1
                if al == 0:
                    break
                if al == 1:
                    continue          # segment-wrap: linear no-op
                ln = al + 1
            else:
                ln = lf + 2
        else:
            ln = ((bit() << 1) | bit()) + 2
            disp = src[pos] - 256; pos += 1
        p = len(out) + disp
        for _ in range(ln):
            out.append(out[p]); p += 1
    return bytes(out), pos

def main():
    d = open(sys.argv[1], 'rb').read()
    assert d[:2] == b'MZ', 'not an MZ'
    e_cparhdr = struct.unpack('<H', d[8:10])[0]
    e_cs = struct.unpack('<H', d[0x16:0x18])[0]
    varoff = (e_cparhdr + e_cs) * 16
    vars = struct.unpack('<7H', d[varoff:varoff+14])
    real_ip, real_cs, real_sp, real_ss, packed_para = vars[0], vars[1], vars[2], vars[3], vars[4]
    hs = e_cparhdr * 16
    src = d[hs:hs + packed_para * 16]
    img, consumed = unpack(src)
    print('vars: real %x:%x ss:sp=%x:%x  packed_para=%x  consumed=%x' %
          (real_cs, real_ip, real_ss, real_sp, packed_para, consumed))
    # reloc delta stream follows the compressed data (usually empty: '00 01 00')
    print('unpacked %d bytes (0x%x)' % (len(img), len(img)))
    size = len(img)
    img_paras = (size + 15) >> 4
    minalloc = max(0, real_ss + 0x10 - img_paras)
    hdr = struct.pack('<14H', 0x5A4D, size % 512, (size + 511)//512, 0, 2,
                      minalloc, 0xffff, real_ss, real_sp, 0, real_ip, real_cs, 0x1c, 0)
    hdr += b'\x00' * (2 * 16 - len(hdr))
    open(sys.argv[2], 'wb').write(hdr + img)

if __name__ == '__main__':
    main()
