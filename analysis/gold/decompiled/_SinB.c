// 2e8c:0002

int __cdecl16far _SinB(uint param_1)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = param_1 >> 2;
  if ((param_1 & 0x4000) != 0) {
    uVar1 = ~uVar1;
  }
  iVar2 = *(int *)(uVar1 & 0xffe);
  if ((int)param_1 < 0) {
    iVar2 = -iVar2;
  }
  return iVar2;
}

