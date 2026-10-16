// 1328:0091

undefined2 __cdecl16far _EMSAllocatePages_g1(int param_1)

{
  code *pcVar1;
  char extraout_AH;
  
  if ((iRam0004d276 != 0) && (param_1 != 0)) {
    pcVar1 = (code *)swi(0x67);
    (*pcVar1)();
    if (extraout_AH == '\0') {
      return 0xffff;
    }
  }
  return 0;
}

