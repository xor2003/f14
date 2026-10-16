// 11ac:1463

undefined2 __cdecl16far
_BuildPolyList(int param_1,undefined4 *param_2,int *param_3,undefined4 param_4,undefined4 param_5,
              undefined2 param_6)

{
  uint uVar1;
  undefined4 uVar2;
  int *piVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  int iVar7;
  undefined4 *puVar8;
  int *piVar9;
  int iVar10;
  
  if (param_1 == 0) {
    return 0xffff;
  }
  uVar4 = 0xffff;
  uVar6 = 0;
  iVar5 = param_1;
  puVar8 = param_2;
  do {
    uVar1 = *(uint *)((int)*puVar8 + 0x1e);
    uVar4 = uVar4 & uVar1;
    uVar6 = uVar6 | uVar1;
    puVar8 = puVar8 + 1;
    iVar5 = iVar5 + -1;
  } while (iVar5 != 0);
  if ((uVar4 & 0x3f) != 0) {
    return 0xffff;
  }
  if ((uVar6 & 0x10) != 0) {
    return 1;
  }
  uRam0004d170 = 0xffff;
  iVar5 = param_1;
  puVar8 = param_2;
  piVar3 = param_3;
  do {
    piVar9 = piVar3;
    iVar10 = (int)((ulong)*puVar8 >> 0x10);
    iVar7 = (int)*puVar8;
    uVar4 = *(uint *)(iVar7 + 0x1e);
    if ((uVar4 & 0x1000) == 0) {
      _Perspect3D();
      uVar4 = *(uint *)(iVar7 + 0x1e);
    }
    uRam0004d170 = uRam0004d170 & uVar4;
    piVar9[1] = iVar10;
    *piVar9 = iVar7;
    puVar8 = puVar8 + 1;
    iVar5 = iVar5 + -1;
    piVar3 = piVar9 + 2;
  } while (iVar5 != 0);
  uVar2 = *param_2;
  piVar9[3] = (int)((ulong)uVar2 >> 0x10);
  piVar9[2] = (int)uVar2;
  if ((uRam0004d170 & 0xf00) == 0) {
    do {
      _NPFLine((int)*(undefined4 *)param_3 + 0xc,(int)((ulong)*(undefined4 *)param_3 >> 0x10),
               (int)*(undefined4 *)(param_3 + 2) + 0xc,
               (int)((ulong)*(undefined4 *)(param_3 + 2) >> 0x10),param_6);
      param_3 = param_3 + 2;
      param_1 = param_1 + -1;
    } while (param_1 != 0);
    _NPFFill(param_4,param_6,param_5);
    return 0;
  }
  return 0xffff;
}

