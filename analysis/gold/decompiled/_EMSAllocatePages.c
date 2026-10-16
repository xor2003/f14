// 1328:006e

undefined2 __cdecl16far _EMSAllocatePages(int param_1)

{
  code *pcVar1;
  char extraout_AH;
  undefined2 extraout_DX;
  
  if ((iRam0004d276 != 0) && (param_1 != 0)) {
    pcVar1 = (code *)swi(0x67);
    (*pcVar1)();
    if (extraout_AH == '\0') {
      return extraout_DX;
    }
  }
  return 0;
}

