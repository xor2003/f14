// 2591:01f3

void __cdecl16far _VERTFLINE(int param_1,int param_2,int param_3)

{
  int iVar1;
  
  iVar1 = param_2;
  if (param_2 < param_1) {
    iVar1 = param_1;
    param_1 = param_2;
  }
  if ((iRam0004ee78 < iVar1) && (param_1 < iRam0004ee7c)) {
    if (param_1 < iRam0004ee78) {
      param_1 = iRam0004ee78;
    }
    if (iRam0004ee7c < iVar1) {
      iVar1 = iRam0004ee7c;
    }
    if (iVar1 - param_1 != 0) {
      if (param_1 < iRam0004f34c) {
        iRam0004f34c = param_1;
      }
      iVar1 = (iVar1 - param_1) + 1;
      param_1 = param_1 << 1;
      do {
        if (param_3 < *(int *)(param_1 + 0x2038)) {
          *(int *)(param_1 + 0x2038) = param_3;
        }
        if (*(int *)(param_1 + 0x21ca) < param_3) {
          *(int *)(param_1 + 0x21ca) = param_3;
        }
        param_1 = param_1 + 2;
        iVar1 = iVar1 + -1;
      } while (iVar1 != 0);
    }
  }
  return;
}

