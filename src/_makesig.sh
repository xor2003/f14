#!/usr/bin/bash
rm f14.pat
ls *.OBJ | while read name; do /home/xor/ida77/flair77/bin/linux/plb -a $name  f14.pat;done
/home/xor/ida77/flair77/bin/linux/sigmake ./f14.pat f14.sig
