// 2e8c:0676

int __cdecl16far _LDIV(undefined2 param_1,uint param_2,uint param_3)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar1 = (param_3 ^ (int)param_3 >> 0xf) - ((int)param_3 >> 0xf);
  uVar3 = (int)param_2 >> 0xf;
  uVar2 = (((param_2 ^ uVar3) - uVar3) + 1) * 2;
  if (uVar2 < uVar1 || uVar2 - uVar1 == 0) {
    return (int)(CONCAT22(param_2,param_1) / (long)(int)uVar1);
  }
  return (uVar3 ^ 0x7fff) - uVar3;
}

