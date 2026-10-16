// 25e4:01d8

void __cdecl16far _EMSHSIZE(undefined2 param_1,undefined2 *param_2)

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
    *param_2 = in_BX;
  }
  return;
}

