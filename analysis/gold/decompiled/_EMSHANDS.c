// 25e4:01af

void __cdecl16far _EMSHANDS(undefined2 *param_1)

{
  code *pcVar1;
  char extraout_AH;
  undefined2 in_BX;
  bool bVar2;
  bool bVar3;
  
  bVar2 = iRam0004f36e == 0;
  if ((!bVar2) || (_EMSinit(), bVar3 = false, bVar2)) {
    pcVar1 = (code *)swi(0x67);
    (*pcVar1)();
    bVar3 = extraout_AH == '\0';
    cRam0004f370 = extraout_AH;
  }
  if (bVar3) {
    *param_1 = in_BX;
  }
  return;
}

