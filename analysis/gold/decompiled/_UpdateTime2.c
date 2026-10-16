// 4b61:07f6

uint __cdecl16far _UpdateTime2(void)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  if (uRam000521c2 < uRam000521d4) {
    iRam0004e83e = uRam000521d4 - uRam000521c2;
    uRam000521c2 = uRam000521d4;
  }
  else {
    iRam0004e83e = 0;
    if (0x14 < uRam000521c2) {
      uRam000521c2 = 0x14;
    }
  }
  uVar3 = ((uRam000521c2 >> 1) + 0x3c) / uRam000521c2;
  DAT_5140_0c2a = uVar3;
  iRam00052230 = iRam00052230 + uRam000521c2;
  if (iRam00052230 < 0xf) {
    uRam000521be = 0xffff;
  }
  else {
    iRam00052230 = iRam00052230 + -0xf;
    uVar1 = uRam000521bc + 1;
    uRam000521be = uRam000521bc;
    uVar2 = uRam000521bc & 3;
    uVar3 = uRam000521bc;
    uRam000521bc = uVar1;
    if (uVar2 == 0) {
      iRam0005368a = iRam0005368a + 1;
      uVar3 = iRam0005368a / 0x1e;
      if ((iRam0005368a % 0x1e == 0) && (iRam00054e32 = iRam00054e32 + 1, iRam00054e32 == 0x5a0)) {
        iRam00054e32 = 0;
      }
    }
  }
  iRam000521ba = iRam000521ba + 1;
  return uVar3;
}

