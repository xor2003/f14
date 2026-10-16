// 5140:1334

undefined2 __cdecl16near __stbuf(int *param_1)

{
  undefined2 uVar1;
  int iVar2;
  int *piVar3;
  
  piVar3 = (int *)0x5c7a;
  if ((((param_1 == (int *)0x5b40) || (piVar3 = (int *)0x5c7c, param_1 == (int *)0x5b48)) ||
      (piVar3 = (int *)0x5c7e, param_1 == (int *)0x5b58)) &&
     (((*(byte *)(param_1 + 3) & 0xc) == 0 && ((*(byte *)(param_1 + 0x50) & 1) == 0)))) {
    iVar2 = *piVar3;
    if (iVar2 == 0) {
      iVar2 = __nmalloc(0x200);
      if (iVar2 == 0) goto LAB_5140_13a1;
      *piVar3 = iVar2;
    }
    param_1[2] = iVar2;
    *param_1 = iVar2;
    param_1[1] = 0x200;
    param_1[0x51] = 0x200;
    *(byte *)(param_1 + 3) = *(byte *)(param_1 + 3) | 2;
    *(byte *)(param_1 + 0x50) = 0x11;
    uVar1 = 1;
  }
  else {
LAB_5140_13a1:
    uVar1 = 0;
  }
  return uVar1;
}

