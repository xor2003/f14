// 2528:04c8

void __cdecl16far
_draw_circle(undefined2 param_1,undefined2 param_2,undefined2 param_3,int param_4,undefined2 param_5
            ,undefined2 param_6)

{
  int iVar1;
  int iVar2;
  undefined2 local_10;
  undefined2 local_e;
  int local_c;
  int local_a;
  int local_8;
  int local_6;
  int local_4;
  
  local_e = param_6;
  local_10 = param_6;
  local_c = param_4;
  local_a = -(param_4 + -1);
  local_6 = 3;
  local_8 = -(param_4 * 2 + -5);
  local_4 = 0;
  while( true ) {
    FUN_2528_056b(param_1,local_4,local_c,param_2,param_3,param_5,&local_10,&local_e,param_6);
    if (local_c <= local_4) break;
    if (local_a < 0) {
      iVar2 = local_8 + 2;
      iVar1 = local_6;
    }
    else {
      local_c = local_c + -1;
      iVar2 = local_8 + 4;
      iVar1 = local_8;
    }
    local_8 = iVar2;
    local_4 = local_4 + 1;
    local_6 = local_6 + 2;
    local_a = local_a + iVar1;
  }
  return;
}

