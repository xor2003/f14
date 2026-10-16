// 2e8c:015c

int __cdecl16far _ARCSIN(uint param_1)

{
  uint uVar1;
  int iVar2;
  
  uVar1 = param_1;
  if ((int)param_1 < 0) {
    uVar1 = -param_1;
  }
  iVar2 = *(int *)((uVar1 >> 4 & 0x7fe) + 0x3202);
  if ((int)param_1 < 0) {
    iVar2 = -iVar2;
  }
  return iVar2;
}

