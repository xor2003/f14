// 25e4:011f

void __cdecl16far _EMSfree(void)

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

