// 2e8c:03b0

void __cdecl16far _COPY3X3(undefined2 *param_1,undefined2 *param_2)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  int iVar3;
  
  for (iVar3 = 9; iVar3 != 0; iVar3 = iVar3 + -1) {
    puVar2 = param_2;
    param_2 = param_2 + 1;
    puVar1 = param_1;
    param_1 = param_1 + 1;
    *puVar2 = *puVar1;
  }
  return;
}

