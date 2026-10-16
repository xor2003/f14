// 1328:0125

undefined2 __cdecl16far _EMMMapMultiPage(undefined2 param_1,int param_2,int param_3,int param_4)

{
  code *pcVar1;
  int iVar2;
  int iVar3;
  
  if (iRam0004d276 == 0) {
    return 0;
  }
  iVar2 = param_2 * 2;
  iVar3 = 0;
  do {
    *(int *)(iVar3 + 0x298) = param_4;
    *(int *)(iVar3 + 0x29a) = param_2;
    *(int *)(iVar2 + 0x290) = param_4;
    param_4 = param_4 + 1;
    param_2 = param_2 + 1;
    iVar2 = iVar2 + 2;
    iVar3 = iVar3 + 4;
    param_3 = param_3 + -1;
  } while (param_3 != 0);
  uRam0004d2ae = param_1;
  pcVar1 = (code *)swi(0x67);
  (*pcVar1)();
  return 0xffff;
}

