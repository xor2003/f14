// 2604:0b63

void __cdecl16far _TAStoIAS(undefined2 param_1)

{
  int iVar1;
  int iVar2;
  
  iVar2 = (uRam0004f472 / 5000) * 2;
  iVar1 = _MDiv(*(int *)(iVar2 + 0x24a0) - *(int *)(iVar2 + 0x249e),(uRam0004f472 % 5000) / 10,500);
  _MDiv(param_1,1000,iVar1 + *(int *)(iVar2 + 0x249e));
  return;
}

