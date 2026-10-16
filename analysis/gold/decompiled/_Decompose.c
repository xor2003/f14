// 28a7:0032

void __cdecl16far _Decompose(void)

{
  int iVar1;
  int iVar2;
  undefined2 uVar3;
  
  uRam0004f46c = FUN_28a7_0218(-iRam0004f41a);
  iVar1 = _Icos(uRam0004f46c);
  if (iVar1 == 0) {
    iRam0004f46e = 0;
    iRam0004f46a = FUN_28a7_0218(uRam0004f412);
    if ((iRam0004f416 < 1) && (iRam0004f418 < 0)) {
      iRam0004f46a = iRam0004f46a + -0x8000;
    }
    if ((0 < iRam0004f416) && (iRam0004f418 < 0)) {
      iRam0004f46a = -0x8000 - iRam0004f46a;
    }
    if ((iRam0004f416 < 0) && (0 < iRam0004f418)) {
      iRam0004f46a = -iRam0004f46a;
    }
  }
  else {
    iVar2 = FUN_28a7_019e(iRam0004f414);
    if (iVar2 < 0x5a81) {
      uVar3 = _IntDiv(iRam0004f414,iVar1);
      uVar3 = FUN_28a7_019e(uVar3);
      iRam0004f46a = FUN_28a7_0218(uVar3);
    }
    else {
      uVar3 = _IntDiv(iRam0004f420,iVar1);
      uVar3 = FUN_28a7_019e(uVar3);
      iRam0004f46a = FUN_28a7_029c(uVar3);
    }
    if ((iRam0004f414 < 1) && (iRam0004f420 < 0)) {
      iRam0004f46a = iRam0004f46a + -0x8000;
    }
    if ((0 < iRam0004f414) && (iRam0004f420 < 0)) {
      iRam0004f46a = -0x8000 - iRam0004f46a;
    }
    if ((iRam0004f414 < 0) && (0 < iRam0004f420)) {
      iRam0004f46a = -iRam0004f46a;
    }
    iVar2 = FUN_28a7_019e(iRam0004f416);
    if (iVar2 < 0x5a81) {
      uVar3 = _IntDiv(iRam0004f416,iVar1);
      uVar3 = FUN_28a7_019e(uVar3);
      iRam0004f46e = FUN_28a7_0218(uVar3);
    }
    else {
      uVar3 = _IntDiv(iRam0004f418,iVar1);
      uVar3 = FUN_28a7_019e(uVar3);
      iRam0004f46e = FUN_28a7_029c(uVar3);
    }
    if ((iRam0004f416 < 1) && (iRam0004f418 < 0)) {
      iRam0004f46e = iRam0004f46e + -0x8000;
    }
    if ((0 < iRam0004f416) && (iRam0004f418 < 0)) {
      iRam0004f46e = -0x8000 - iRam0004f46e;
    }
    if ((iRam0004f416 < 0) && (0 < iRam0004f418)) {
      iRam0004f46e = -iRam0004f46e;
      return;
    }
  }
  return;
}

