// 3ad2:47d0

void __cdecl16far
_Langle(undefined2 param_1,undefined2 param_2,undefined2 param_3,undefined2 param_4)

{
  uint uVar1;
  uint uVar2;
  int in_DX;
  int extraout_DX;
  int iVar3;
  int iVar4;
  
  uVar1 = _absL(param_1,param_2);
  iVar4 = in_DX;
  uVar2 = _absL(param_3,param_4);
  iVar3 = in_DX + iVar4 + (uint)CARRY2(uVar1,uVar2);
  if ((-1 < iVar3) && ((0 < iVar3 || (0x3fff < uVar1 + uVar2)))) {
    __aFNalshl_g1(&param_1,8);
    __aFNalshl_g1(&param_3,8);
    iVar4 = extraout_DX;
  }
  uVar1 = _absL(param_1,param_2);
  iVar3 = iVar4;
  uVar2 = _absL(param_3,param_4);
  iVar4 = iVar4 + iVar3 + (uint)CARRY2(uVar1,uVar2);
  if ((-1 < iVar4) && ((0 < iVar4 || (0x3fff < uVar1 + uVar2)))) {
    __aFNalshl_g1(&param_1,8);
    __aFNalshl_g1(&param_3,8);
  }
  _ARCTAN(param_1,param_3);
  return;
}

