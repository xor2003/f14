// 18de:1b85

void __cdecl16far _SetupNewTarget(int *param_1,int param_2)

{
  uint uVar1;
  int iVar2;
  int local_e;
  int local_c;
  int local_a;
  int local_8;
  int local_6;
  int local_4;
  
  *param_1 = 0;
  iVar2 = param_2 * 0x28;
  local_e = *(uint *)(iVar2 + 0x7e6c) - uRam00056076;
  local_c = (*(int *)(iVar2 + 0x7e6e) - iRam00056078) -
            (uint)(*(uint *)(iVar2 + 0x7e6c) < uRam00056076);
  uVar1 = *(uint *)(iVar2 + 0x7e6a);
  local_6 = uVar1 - uRam0004f470;
  local_4 = ((int)uVar1 >> 0xf) - (uint)(uVar1 < uRam0004f470);
  uVar1 = -*(int *)(iVar2 + 0x7e70);
  local_a = uVar1 - uRam00057e16;
  iVar2 = (((0x10 - *(int *)(iVar2 + 0x7e72)) - (uint)(*(int *)(iVar2 + 0x7e70) != 0)) -
          iRam00057e18) - (uint)(uVar1 < uRam00057e16);
  local_8 = iVar2;
  uVar1 = _absL_g1(local_e,local_c);
  if ((iVar2 < 1) && ((iVar2 < 0 || (uVar1 < 0x49d4)))) {
    uVar1 = _absL_g1(local_6,local_4);
    if ((iVar2 < 1) && ((iVar2 < 0 || (uVar1 < 0x49d4)))) {
      uVar1 = _absL_g1(local_a,local_8);
      if ((iVar2 < 0) || ((iVar2 < 1 && (uVar1 < 0x49d4)))) goto LAB_18de_1c46;
    }
  }
  __aFNalshl_g1(&local_e,5);
  __aFNalshl_g1(&local_6,5);
  __aFNalshl_g1(&local_a,5);
  *param_1 = *param_1 + 1;
LAB_18de_1c46:
  param_1[1] = local_e;
  param_1[2] = local_6;
  param_1[3] = local_a;
  param_1[0x10] = iRam0004f46a - *(int *)(param_2 * 0x28 + 0x7e74);
  param_1[0xd] = param_2;
  return;
}

