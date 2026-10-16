// 5140:0be8

void __cdecl16far _memcpy(undefined2 *param_1,undefined2 *param_2,uint param_3)

{
  undefined2 *puVar1;
  undefined2 *puVar2;
  uint uVar3;
  
  if (param_3 != 0) {
    if (((uint)param_1 & 1) != 0) {
      puVar2 = param_1;
      param_1 = (undefined2 *)((int)param_1 + 1);
      puVar1 = param_2;
      param_2 = (undefined2 *)((int)param_2 + 1);
      *(undefined1 *)puVar2 = *(undefined1 *)puVar1;
      param_3 = param_3 - 1;
    }
    for (uVar3 = param_3 >> 1; uVar3 != 0; uVar3 = uVar3 - 1) {
      puVar2 = param_1;
      param_1 = param_1 + 1;
      puVar1 = param_2;
      param_2 = param_2 + 1;
      *puVar2 = *puVar1;
    }
    for (uVar3 = (uint)((param_3 & 1) != 0); uVar3 != 0; uVar3 = uVar3 - 1) {
      puVar2 = param_1;
      param_1 = (undefined2 *)((int)param_1 + 1);
      puVar1 = param_2;
      param_2 = (undefined2 *)((int)param_2 + 1);
      *(undefined1 *)puVar2 = *(undefined1 *)puVar1;
    }
  }
  return;
}

