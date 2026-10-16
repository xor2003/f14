// 25e4:0142

void __cdecl16far _EMSVER(undefined2 *param_1)

{
  code *pcVar1;
  undefined2 uVar2;
  bool bVar3;
  bool bVar4;
  
  bVar3 = iRam0004f36e == 0;
  if ((!bVar3) || (uVar2 = _EMSinit(), bVar4 = false, bVar3)) {
    pcVar1 = (code *)swi(0x67);
    uVar2 = (*pcVar1)();
    cRam0004f370 = (char)((uint)uVar2 >> 8);
    bVar4 = cRam0004f370 == '\0';
  }
  if (bVar4) {
    *param_1 = uVar2;
  }
  return;
}

