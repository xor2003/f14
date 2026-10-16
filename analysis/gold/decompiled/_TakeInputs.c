// 2081:000c

void __cdecl16far _TakeInputs(void)

{
  int iVar1;
  
  bRam00000417 = bRam00000417 & 0x2f;
  DAT_57de_0f38 = 0;
  iVar1 = _kbhit();
  if (iVar1 != 0) {
    DAT_57de_0f38 = __bios_keybrd(0);
  }
  FUN_2081_01f0();
  while( true ) {
    iVar1 = _kbhit();
    if (iVar1 == 0) break;
    __bios_keybrd(0);
  }
  if (DAT_58fc_09a4 == 0) {
    DAT_543d_1582 = (undefined1)((int)((DAT_4fa0_0854 - 0x80) * (iRam0004e840 + 1)) / 3 + 0x80);
    DAT_543d_158a = (char)((int)((DAT_4fa0_0855 - 0x80) * (iRam0004e840 + 1)) / 3) + -0x80;
  }
  else {
    FUN_2081_008c();
  }
  if (cRam0004ee4c != '\0') {
    DAT_57de_0f38 = 0x1000;
  }
  return;
}

