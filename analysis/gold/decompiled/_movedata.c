// 5140:0b1c

void __cdecl16far
_movedata(undefined2 param_1,undefined1 *param_2,undefined2 param_3,undefined1 *param_4,int param_5)

{
  undefined1 *puVar1;
  undefined1 *puVar2;
  
  for (; param_5 != 0; param_5 = param_5 + -1) {
    puVar2 = param_4;
    param_4 = param_4 + 1;
    puVar1 = param_2;
    param_2 = param_2 + 1;
    *puVar2 = *puVar1;
  }
  return;
}

