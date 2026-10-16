// 1d04:08c2

undefined2 __cdecl16far
_BulletDist(uint param_1,int param_2,uint param_3,int param_4,int param_5,uint *param_6)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  undefined2 uVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = (param_2 - param_6[1]) - (uint)(param_1 < *param_6);
  uVar1 = _absL_g1(param_1 - *param_6,iVar5);
  iVar6 = (param_4 - param_6[3]) - (uint)(param_3 < param_6[2]);
  uVar2 = _absL_g1(param_3 - param_6[2],iVar6);
  uVar3 = FUN_28a7_019e(param_5 - param_6[4]);
  iVar5 = ((int)uVar3 >> 0xf) + iVar6 + (uint)CARRY2(uVar3,uVar2) + iVar5 +
          (uint)CARRY2(uVar3 + uVar2,uVar1);
  if ((-1 < iVar5) && ((0 < iVar5 || (22999 < uVar3 + uVar2 + uVar1)))) {
    return 0x7fff;
  }
  uVar4 = _Dist3D(uVar1,uVar2,uVar3);
  return uVar4;
}

