#!/bin/bash
# Runnable-build link: same OBJS as build_all.sh but GDSEGR (ORG-free compact
# GDSEG) linked LAST so real module data symbols win over stub dups, and DGROUP
# drops to real content (~25KB) leaving room for a real stack.
# Produces build/obj/F14RUN.EXE -- for emulator runtime verification only;
# layout differs from retail so NOT for binary-diff matching.
set -e
ROOT=$(cd "$(dirname "$0")/.." && pwd)
OBJ=$ROOT/build/obj
LOG=$ROOT/build/log
LIBD=$ROOT/lib
SRC=$ROOT/src
MASMSTAGE=$ROOT/build/masmstag
source <(grep -E '^(KV|OBJS|MASM510BIN|UASM)=' "$ROOT/build_all.sh" | sed 's/^/export /')
mkdir -p "$LOG" "$MASMSTAGE" "$ROOT/build/h"
# assemble GDSEGR via MASM staging (same path as gdseg)
cp -f "$SRC/GDSEGR.ASM" "$MASMSTAGE/GDSEGR.ASM"
for i in "$ROOT/build/h/"*; do ln -sf "$i" "$MASMSTAGE/" 2>/dev/null; done
"$KV" --mount=C:"$MASM510BIN/" --mount=D:"$ROOT/build/" \
    --mount=E:"$OBJ/" \
    --drive=D --cwd-dos=D:/masmstag \
    --env='INCLUDE=D:\h\;D:\masmstag\' \
    'C:\MASM.EXE' "GDSEGR.ASM,E:\\GDSEGR.OBJ,,;" \
    > "$LOG/gdsegr.masm.log" 2>&1
tail -3 "$LOG/gdsegr.masm.log"
[ -f "$OBJ/GDSEGR.OBJ" ] || { echo "GDSEGR.OBJ missing"; exit 1; }
rsp=$OBJ/f14run.lrf
: > "$rsp"
rest=$(echo $OBJS | tr ' ' '\n' | grep -v '^gdseg$')
for b in $rest; do
    B=$(tr 'a-z' 'A-Z' <<< "$b")
    echo "E:\\$B.OBJ+" >> "$rsp"
done
echo "E:\\GDSEGR.OBJ" >> "$rsp"
printf 'E:\\F14RUN.EXE\nE:\\F14RUN.MAP\nH:\\LIB\\MLIBC7.LIB\n' >> "$rsp"
"$KV" --mount=C:"$ROOT/C700/" --mount=E:"$OBJ/" --mount=G:"$LIBD/" \
    --mount=H:"$ROOT/c6/" --env='LIB=G:\;H:\LIB' --drive=E \
    'C:\BIN\LINK.EXE' /MAP /SEG:256 /ST:8192 /CODEVIEW "@E:\\f14run.lrf" \
    > "$LOG/linkrun.log" 2>&1
tail -5 "$LOG/linkrun.log"
