// 11ac:0d29

void __cdecl16far _FMEMFILL(undefined4 *param_1,undefined2 param_2,uint param_3)

{
  undefined4 *puVar1;
  uint uVar2;
  undefined4 *puVar3;
  undefined2 uVar4;
  
  if (param_3 != 0) {
    uVar4 = (undefined2)((ulong)param_1 >> 0x10);
    puVar3 = (undefined4 *)param_1;
    if (3 < param_3) {
      for (uVar2 = param_3 >> 2; uVar2 != 0; uVar2 = uVar2 - 1) {
        puVar1 = puVar3;
        puVar3 = puVar3 + 1;
        *puVar1 = CONCAT22(param_2,param_2);
      }
      param_3 = param_3 & 3;
    }
    if (param_3 != 0) {
      for (; param_3 != 0; param_3 = param_3 - 1) {
        puVar1 = puVar3;
        puVar3 = (undefined4 *)((int)puVar3 + 1);
        *(char *)puVar1 = (char)param_2;
      }
    }
  }
  return;
}

