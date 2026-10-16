#!/bin/bash
# F-14 Fleet Defender — batch build
# C files: MSC 6.00 (c6/BIN/CL.EXE) under kvikdos, flags per src/MAKEFILE
# ASM:     UASM -Zm (bundled MASM.EXE is PE32/DPMI, masm510 is MASM 5.0-only syntax)
#          -Zm gives reg.const member syntax; OPTION SCOPED gives proc-local labels
# Link:    LINK 5.31.009 (C700/BIN/LINK.EXE) under kvikdos

KV=/home/xor/kvikdos/kvikdos
ROOT=/home/xor/games/f14src
SRC=$ROOT/src
HDR=$ROOT/h
LIBD=$ROOT/lib
OBJ=$ROOT/build/obj
LOG=$ROOT/build/log
ASTAGE=$ROOT/build/asmstage
MASM510=/home/xor/games/f19ru/F19/dos/masm510
UASM=/usr/bin/uasm

mkdir -p "$OBJ" "$LOG" "$ASTAGE"

CFLAGS="/FPa /G2 /AM /Osegcl /nologo /c /Zi /Zd"

# object list from MAKEFILE OBJS — original link order (determines segment layout)
OBJS="monoprin adicrt npolys n3d emsload pasm emm nqfly 3dexcept 3dloadt 3dloader 3dmatrix 3dobject 3dplanes 3dscene aaa aaahit armt awacs awg9 billasm boats bullets carr carrai cmds cockpit ctrlvect ddd egraph ems flight2 flir fltmath gmain nhorz horzasm hud2 hsd joystx njoycal keyjoy libglue loadmiss map math mcga mcgamask nmenus message options ovl planes planes2 planes3 radars rastport riohead riomap sams savemode sound sprites stealth support tac tews tgtflir tid trig track views vdi world _rp"

src_for() {
    local b=$1
    for ext in c C asm ASM; do
        [ -f "$SRC/$b.$ext" ] && { echo "$SRC/$b.$ext"; return; }
    done
    find "$SRC" -maxdepth 1 \( -iname "$b.c" -o -iname "$b.asm" \) | head -1
}

do_c() {
    local f=$1 b=$2
    "$KV" --mount=C:"$ROOT/c6/" --mount=D:"$SRC/" --mount=E:"$OBJ/" \
        --mount=F:"$HDR/" --env='PATH=C:\BIN;C:\BINB' \
        --env='INCLUDE=F:\;C:\INCLUDE' --env='LIB=C:\LIB' --env='TMP=E:\' --drive=E \
        'C:\BIN\CL.EXE' $CFLAGS -IF:\\ "/FoE:\\$b.OBJ" "D:\\$(basename "$f")" \
        > "$LOG/$b.cl.log" 2>&1
}

prep_stage() {
    # include-file aliases as referenced inside the .ASM sources
    cd "$ASTAGE" || return 1
    for i in CLIP.EQU GRAPH.EXT SIN.TBL ATN.TBL TANTORAD.TBL ARCSIN.TBL; do
        [ -f "$SRC/$i" ] && ln -sf "$SRC/$i" .
    done
    # lowercase aliases uasm asks for
    for i in clip.equ graph.ext sin.tbl atn.tbl tantorad.tbl arcsin.tbl; do
        src=$(ls "$SRC" | grep -ix "$i" | head -1)
        [ -n "$src" ] && ln -sf "$SRC/$src" "$i"
    done
    # literal '..\h\RpAsm.H' filename (backslashes are valid chars on ext4)
    ln -sf "$HDR/RPASM.H" '..\h\RpAsm.H' 2>/dev/null
    ln -sf "$HDR/RPASM.H" 'RpAsm.H' 2>/dev/null
}

# files mixing EQU const-dot fields ([reg].NAME) with 386 instrs:
# -Zm enables const-dot but rejects movsd etc; default mode needs scoped labels
# (which is default anyway) — so sed-convert const-dots to [reg+NAME] for these.
NOZM_SED="math pasm"

do_asm() {
    local f=$1 b=$2
    local st="$ASTAGE/$(basename "$f")"
    if grep -qw "$b" <<< "$NOZM_SED"; then
        sed -E '/include/I!{
            :a
            s/([]A-Za-z0-9_])\.([A-Za-z_][A-Za-z0-9_]*)/\1+\2/g
            s/([]A-Za-z_])\.([0-9]+)/\1+\2/g
            ta
        }' "$f" | sed -E 's/\bXADD\b/XADD_/g' > "$st"
        pubs=$(python3 "$ROOT/tools/procpubs.py" "$st")
        awk -v pubs="$pubs" 'NR==1{print "OPTION SCOPED"} /\.MODEL/{print; print pubs; next} {print}' "$st" > "$st.tmp" && mv "$st.tmp" "$st"
        far=$(grep -E '^[ \t]*(EXTRN|extrn)[ \t].*:FAR' "$st")
        if [ -n "$far" ]; then
            awk -v far="$far" '
                !done && (/\.MODEL/ || /\.(CODE|DATA|STACK|CONST|FARDATA)/ || /SEGMENT/) {print far; done=1}
                /^[ \t]*(EXTRN|extrn)[ \t].*:FAR/{next}
                {print}' "$st" > "$st.tmp" && mv "$st.tmp" "$st"
        fi
        (cd "$ASTAGE" && "$UASM" -q -Zi -Zd -I. -I"$HDR" \
            -Fo "$OBJ/$(tr 'a-z' 'A-Z' <<< "$b").OBJ" "$st") > "$LOG/$b.masm.log" 2>&1
    else
        pubs=$(python3 "$ROOT/tools/procpubs.py" "$f")
        awk -v pubs="$pubs" 'NR==1{print "OPTION SCOPED"} /\.MODEL/{print; print pubs; next} {print}' "$f" > "$st"
        # hoist FAR extrns out of segment blocks: UASM tags extdefs with the
        # enclosing segment as the fixup frame (far calls then overflow).
        far=$(grep -E '^[ \t]*(EXTRN|extrn)[ \t].*:FAR' "$st")
        if [ -n "$far" ]; then
            awk -v far="$far" '
                !done && (/\.MODEL/ || /\.(CODE|DATA|STACK|CONST|FARDATA)/ || /SEGMENT/) {if(!/SEGMENT/||!done){print far; done=1}}
                /^[ \t]*(EXTRN|extrn)[ \t].*:FAR/{next}
                {print}' "$st" > "$st.tmp" && mv "$st.tmp" "$st"
        fi
        (cd "$ASTAGE" && "$UASM" -q -Zm -Zi -Zd -I. -I"$HDR" \
            -Fo "$OBJ/$(tr 'a-z' 'A-Z' <<< "$b").OBJ" "$st") > "$LOG/$b.masm.log" 2>&1
    fi
}

case "$1" in
c|asm|all)
    mode=$1
    [ "$mode" != "c" ] && prep_stage
    for b in $OBJS; do
        f=$(src_for "$b")
        [ -z "$f" ] && { echo "MISSING SOURCE: $b"; continue; }
        lc=$(tr 'A-Z' 'a-z' <<< "${f##*.}")
        if [ "$lc" = "c" ]; then
            [ "$mode" = "asm" ] && continue
            do_c "$f" "$b"
        else
            [ "$mode" = "c" ] && continue
            do_asm "$f" "$b"
        fi
        if [ -f "$OBJ/$b.OBJ" ] || [ -f "$OBJ/$(tr 'a-z' 'A-Z' <<< $b).OBJ" ]; then
            echo "OK   $b"
        else
            echo "FAIL $b  (see build/log/$b.*.log)"
        fi
    done
    ;;
link)
    rsp=$OBJ/f14.lrf
    : > "$rsp"
    last=$(echo $OBJS | tr ' ' '\n' | tail -1)
    for b in $OBJS; do
        B=$(tr 'a-z' 'A-Z' <<< "$b")
        if [ "$b" = "$last" ]; then echo "E:\\$B.OBJ" >> "$rsp"; else echo "E:\\$B.OBJ+" >> "$rsp"; fi
    done
    printf 'E:\\F14.EXE\nE:\\F14.MAP\nG:\\MPSLIBM.LIB\n' >> "$rsp"
    "$KV" --mount=C:"$ROOT/C700/" --mount=E:"$OBJ/" --mount=G:"$LIBD/" \
        --mount=H:"$ROOT/c6/" --env='LIB=G:\;H:\LIB' --drive=E \
        'C:\BIN\LINK.EXE' /MAP /SEG:256 /ST:3500 /CODEVIEW "@E:\\f14.lrf" \
        > "$LOG/link.log" 2>&1
    tail -20 "$LOG/link.log"
    ;;
esac
