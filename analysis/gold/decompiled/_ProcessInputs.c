// 2081:0693

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */

void __cdecl16far _ProcessInputs(void)

{
  if (FUN_4e65_01f2 != 0) {
    iRam0004f488 = ((FUN_4e65_01f2 + -1) * 0x90) / 0x80;
    if (iRam0004f488 < 0x83) {
      if (100 < iRam0004f488) {
        iRam0004f488 = 100;
      }
    }
    else {
      iRam0004f488 = 0x90;
    }
  }
  if (iRam0004ed26 != 0) {
    FUN_1d04_0044();
  }
  if (DAT_57de_0f38 != 0) {
    FUN_2081_0711();
  }
  if ((((iRam0004f488 != 0) && (iRam00054dee == 0)) &&
      (bRam00052016 = bRam00052016 & 0xf7, iRam00054a68 == 0)) &&
     ((iRam00050486 != 1 && (iRam00050486 != 3)))) {
    FUN_4a6f_00b5(0x13,1,0);
  }
  return;
}

