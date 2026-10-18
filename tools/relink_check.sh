#!/bin/bash
# relink_check.sh MOD ... -- recompile given module basenames, reassemble
# gdseg, relink, regen f14_named.map (image size = last segment stop).
cd /home/xor/games/f14src
KV=~/kvikdos/kvikdos
CFLAGS="/FPa /G2 /Gs /AM /Osegcl /nologo /c /Zi /Zd"
for b in "$@"; do
  f=$(ls src/$b.C src/$b.c 2>/dev/null | head -1)
  [ -z "$f" ] && { echo "no src for $b"; continue; }
  $KV --mount=C:/home/xor/games/f14src/c6 --mount=D:/home/xor/games/f14src/src \
      --mount=E:/home/xor/games/f14src/build/obj --mount=F:/home/xor/games/f14src/h \
      --env='PATH=C:\BIN;C:\BINB' --env='INCLUDE=F:\;C:\INCLUDE' \
      --env='LIB=C:\LIB' --env='TMP=E:\' --drive=E \
      'C:\BIN\CL.EXE' $CFLAGS -IF:\\ "/FoE:\\$(basename ${f%.[Cc]} | tr a-z A-Z).OBJ" \
      "D:\\$(basename $f)" > /tmp/cl_$b.log 2>&1
  echo "$b errors: $(grep -ci ' error ' /tmp/cl_$b.log)"
done
bash build_all.sh asm gdseg >/dev/null 2>&1
bash build_all.sh link >/dev/null 2>&1
SZ=$(python3 -c "
import re
mx=0
for l in open('build/obj/F14.MAP'):
    for m in re.finditer(r'([0-9A-F]{4,5})H', l):
        mx=max(mx,int(m.group(1),16))
print(hex(mx))")
python3 tools/linkmap2mzmap.py build/obj/F14.MAP 64637 > build/f14_named.map
echo "map regenerated size=64637"
