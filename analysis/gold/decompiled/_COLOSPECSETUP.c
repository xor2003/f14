// 2c92:0008

void __cdecl16far _COLOSPECSETUP(void)

{
  code *pcVar1;
  int iVar2;
  int in_BX;
  
  if (iRam000500c9 == -1) {
    iRam000500c9 = 0;
    pcVar1 = (code *)swi(0x15);
    iVar2 = (*pcVar1)();
    if ((iVar2 == 0x5347) && (in_BX != 0)) {
      iRam000500c9 = 1;
      iRam000500cb = in_BX;
    }
  }
  return;
}

