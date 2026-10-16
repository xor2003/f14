// 25e4:0028

void __cdecl16far _EMSinit(void)

{
  code *pcVar1;
  char cVar2;
  char extraout_AH;
  char extraout_AH_00;
  undefined2 in_BX;
  
  uRam0004f36e = 0;
  cVar2 = _EMSPRSNT();
  if (cVar2 == '\0') {
    pcVar1 = (code *)swi(0x67);
    (*pcVar1)();
    cRam0004f370 = extraout_AH;
    if (extraout_AH == '\0') {
      pcVar1 = (code *)swi(0x67);
      (*pcVar1)();
      cRam0004f370 = extraout_AH_00;
      if (extraout_AH_00 == '\0') {
        uRam0004f36e = in_BX;
      }
    }
  }
  return;
}

