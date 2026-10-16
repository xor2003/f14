// 1328:00dc

undefined2 __cdecl16far _EMMMAPPAGE(int param_1,int param_2,uint param_3)

{
  code *pcVar1;
  char extraout_AH;
  
  if ((iRam0004d276 != 0) && (param_3 < 4)) {
    if (param_1 == iRam0004d2ae) {
      if (*(int *)(param_3 + 0x290) == param_2) {
        return 0xffff;
      }
    }
    else {
      iRam0004d2ae = param_1;
    }
    *(int *)(param_3 + 0x290) = param_2;
    pcVar1 = (code *)swi(0x67);
    (*pcVar1)();
    if (extraout_AH == '\0') {
      return 0xffff;
    }
  }
  return 0;
}

