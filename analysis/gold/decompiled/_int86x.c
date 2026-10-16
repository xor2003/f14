// 5140:085e

undefined2 __cdecl16far _int86x(byte param_1,int param_2,undefined2 *param_3)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  undefined2 uVar4;
  undefined2 uVar5;
  bool bVar6;
  undefined4 uVar7;
  undefined1 local_c;
  byte local_b;
  undefined1 local_a;
  undefined1 local_9;
  undefined1 local_8;
  
  local_c = (code)0xcd;
  local_b = param_1;
  bVar6 = param_1 < 0x25;
  if ((param_1 == 0x25) || (bVar6 = param_1 < 0x26, param_1 == 0x26)) {
    local_8 = 0xcb;
    local_9 = 0x44;
    local_a = 0x44;
  }
  else {
    local_a = 0xcb;
  }
  uVar5 = *(undefined2 *)(param_2 + 2);
  uVar4 = *(undefined2 *)(param_2 + 4);
  uVar1 = *(undefined2 *)(param_2 + 8);
  uVar2 = *(undefined2 *)(param_2 + 10);
  uVar7 = (*(code *)&local_c)(0x5140);
  uVar3 = (undefined2)uVar7;
  *param_3 = uVar3;
  param_3[1] = uVar5;
  param_3[2] = uVar4;
  param_3[3] = (int)((ulong)uVar7 >> 0x10);
  param_3[4] = uVar1;
  param_3[5] = uVar2;
  if (bVar6) {
    FUN_5140_10c4();
    uVar3 = *param_3;
  }
  param_3[6] = (uint)bVar6;
  return uVar3;
}

