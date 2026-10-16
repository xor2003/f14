// 5140:0eee

void __stdcall16far __aFNalshl_g1(undefined2 *param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  
  uVar2 = param_1[1];
  uVar1 = __aFlshr();
  *param_1 = uVar1;
  param_1[1] = uVar2;
  return;
}

