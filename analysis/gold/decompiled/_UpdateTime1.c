// 4b61:0790

void __cdecl16far _UpdateTime1(void)

{
  int iVar1;
  undefined2 uVar2;
  bool bVar3;
  
  uRam000521c2 = 0;
  uVar2 = 0x4b61;
  while (uRam000521c2 < uRam000521d4) {
    iVar1 = thunk_EXT_FUN_0000_0000(uVar2);
    uVar2 = 0x15ce;
    uRam000521c2 = iVar1 - iRam0005222e;
  }
  iRam0005222e = iRam0005222e + uRam000521c2;
  if (1000 < uRam000521c2) {
    uRam000521c2 = 1000;
  }
  if (uRam000521c2 == 0) {
    uRam000521c2 = 1;
  }
  bVar3 = CARRY2(uRam00054e44,uRam000521c2);
  uRam00054e44 = uRam00054e44 + uRam000521c2;
  iRam00054e46 = iRam00054e46 + (uint)bVar3;
  uRam000521c0 = (int)(600 / (ulong)uRam000521c2);
  if (1 < uRam000521ce) {
    uRam000521c2 = uRam000521ce * uRam000521c2;
  }
  return;
}

