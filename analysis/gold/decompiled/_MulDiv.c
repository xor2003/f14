// 2e8c:062d

int __cdecl16far _MulDiv(int param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  
  uVar2 = (int)param_3 >> 0xf;
  uVar1 = param_3 ^ uVar2;
  if (uVar1 == uVar2) {
    param_3 = param_3 + 1;
  }
  if ((param_2 ^ (int)param_2 >> 0xf) - ((int)param_2 >> 0xf) < uVar1 - uVar2) {
    return (int)(((long)param_1 * (long)(int)param_2) / (long)(int)param_3);
  }
  return ((int)param_2 / (int)param_3) * param_1;
}

