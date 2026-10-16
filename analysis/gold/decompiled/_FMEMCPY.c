// 11ac:0d7e

void __cdecl16far _FMEMCPY(undefined4 *param_1,undefined4 *param_2,uint param_3)

{
  undefined4 *puVar1;
  undefined4 *puVar2;
  uint uVar3;
  undefined4 *puVar4;
  undefined4 *puVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  
  if (param_3 != 0) {
    uVar7 = (undefined2)((ulong)param_1 >> 0x10);
    puVar4 = (undefined4 *)param_1;
    uVar6 = (undefined2)((ulong)param_2 >> 0x10);
    puVar5 = (undefined4 *)param_2;
    if (3 < param_3) {
      for (uVar3 = param_3 >> 2; uVar3 != 0; uVar3 = uVar3 - 1) {
        puVar2 = puVar5;
        puVar5 = puVar5 + 1;
        puVar1 = puVar4;
        puVar4 = puVar4 + 1;
        *puVar2 = *puVar1;
      }
      param_3 = param_3 & 3;
    }
    if (param_3 != 0) {
      for (; param_3 != 0; param_3 = param_3 - 1) {
        puVar2 = puVar5;
        puVar5 = (undefined4 *)((int)puVar5 + 1);
        puVar1 = puVar4;
        puVar4 = (undefined4 *)((int)puVar4 + 1);
        *(undefined1 *)puVar2 = *(undefined1 *)puVar1;
      }
    }
  }
  return;
}

