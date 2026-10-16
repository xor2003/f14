// 2e8c:058e

void __cdecl16far _GetHPR(undefined2 *param_1,undefined2 *param_2,uint *param_3,undefined2 *param_4)

{
  uint uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  
  uVar1 = _ARCSIN(param_1[5]);
  if ((uVar1 ^ (int)uVar1 >> 0xf) - ((int)uVar1 >> 0xf) < 0x3fad) {
    uVar2 = _ARCTAN(-param_1[2],param_1[8]);
    uVar3 = _ARCTAN(-param_1[3],param_1[4]);
  }
  else {
    uVar2 = _ARCTAN(param_1[6],*param_1);
    uVar3 = 0;
  }
  *param_4 = uVar3;
  *param_2 = uVar2;
  *param_3 = uVar1;
  if (15999 < (uVar1 ^ (int)uVar1 >> 0xf) - ((int)uVar1 >> 0xf)) {
    uVar1 = _ARCTAN(param_1[5],param_1[4]);
    *param_3 = uVar1;
  }
  return;
}

