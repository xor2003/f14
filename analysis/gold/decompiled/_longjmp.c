// 5140:0cc9

int __cdecl16far _longjmp(int param_1,int param_2)

{
  undefined2 *puVar1;
  undefined2 unaff_SS;
  
  if (param_2 == 0) {
    param_2 = 1;
  }
  puVar1 = (undefined2 *)*(int *)(param_1 + 6);
  *puVar1 = *(undefined2 *)(param_1 + 8);
  puVar1[1] = *(undefined2 *)(param_1 + 10);
  return param_2;
}

