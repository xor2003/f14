// 25e4:0169

void __cdecl16far _EMSfree_g1(void)

{
  code *pcVar1;
  undefined1 extraout_AH;
  bool bVar2;
  
  bVar2 = iRam0004f36e == 0;
  if ((!bVar2) || (_EMSinit(), bVar2)) {
    pcVar1 = (code *)swi(0x67);
    (*pcVar1)();
    uRam0004f370 = extraout_AH;
  }
  return;
}

