// 2a48:000c

undefined2 __cdecl16far _HUD(void)

{
  undefined2 uVar1;
  
  uRam000513e8 = 6;
  if ((bRam00052015 & 1) != 0) {
    return 0;
  }
  _HUD_standard();
  uVar1 = FUN_2a48_0025();
  return uVar1;
}

