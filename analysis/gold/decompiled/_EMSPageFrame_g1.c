// 1328:0057

undefined2 __cdecl16far _EMSPageFrame_g1(void)

{
  code *pcVar1;
  char extraout_AH;
  undefined2 in_BX;
  
  if (iRam0004d276 != 0) {
    pcVar1 = (code *)swi(0x67);
    (*pcVar1)();
    if (extraout_AH == '\0') {
      return in_BX;
    }
  }
  return 0;
}

