// 5140:0ca6

undefined2 __cdecl16far _setjmp(undefined2 *param_1)

{
  undefined2 unaff_BP;
  undefined2 unaff_SI;
  undefined2 unaff_DI;
  undefined2 in_stack_00000000;
  undefined2 in_stack_00000002;
  
  *param_1 = unaff_BP;
  param_1[1] = unaff_DI;
  param_1[2] = unaff_SI;
  param_1[3] = register0x00000010;
  param_1[4] = in_stack_00000000;
  param_1[5] = in_stack_00000002;
  return 0;
}

