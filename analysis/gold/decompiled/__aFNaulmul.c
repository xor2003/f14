// 5140:0e90

void __stdcall16far __aFNaulmul(undefined2 *param_1,undefined2 param_2,undefined2 param_3)

{
  undefined4 uVar1;
  
  uVar1 = __aFldiv(*param_1,param_1[1],param_2,param_3);
  param_1[1] = (int)((ulong)uVar1 >> 0x10);
  *param_1 = (int)uVar1;
  return;
}

