// 4fa0:0981

void __cdecl16far
_dashline_g1(undefined2 param_1,int param_2,int param_3,int param_4,int param_5,undefined2 param_6,
            int param_7)

{
  bool bVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *local_14;
  int *local_12;
  int local_10;
  int local_e;
  int local_c;
  int local_a;
  int local_6;
  int local_4;
  
  iVar4 = param_3;
  iVar2 = param_2;
  if (param_4 < param_2) {
    param_2 = param_4;
    param_4 = iVar2;
    param_3 = param_5;
    param_5 = iVar4;
  }
  local_10 = 1;
  if (param_5 < param_3) {
    iVar2 = -1;
  }
  else {
    iVar2 = 1;
  }
  iVar3 = param_4 - param_2;
  local_12 = &param_2;
  local_14 = &param_3;
  local_e = FUN_28a7_019e(param_5 - param_3);
  iVar4 = iVar3;
  local_c = iVar2;
  if (iVar3 < local_e) {
    local_c = 1;
    local_12 = &param_3;
    local_14 = &param_2;
    iVar4 = local_e;
    local_10 = iVar2;
    local_e = iVar3;
  }
  bVar1 = true;
  local_a = param_7;
  local_6 = (-1 - iVar4) / 2;
  local_4 = iVar4;
  while (iVar2 = local_4 + -1, local_4 != 0) {
    if (bVar1) {
      FUN_15ce_0c21(param_1,param_2,param_3,param_6);
    }
    local_a = local_a + -1;
    if (local_a == 0) {
      bVar1 = (bool)(bVar1 ^ 1);
      local_a = param_7;
    }
    *local_12 = *local_12 + local_10;
    local_6 = local_6 + local_e;
    local_4 = iVar2;
    if (0 < local_6) {
      local_6 = local_6 - iVar4;
      *local_14 = *local_14 + local_c;
    }
  }
  return;
}

