// 1342:0410

void __cdecl16far _MakeMidGridPt(int *param_1,undefined2 param_2,int *param_3,int *param_4)

{
  uint *puVar1;
  uint uVar2;
  int *piVar3;
  int *piVar4;
  uint uVar5;
  int iVar6;
  int *piVar7;
  int *piVar8;
  undefined2 uVar9;
  undefined2 uVar10;
  
  uVar9 = (undefined2)((ulong)param_4 >> 0x10);
  piVar8 = (int *)param_4;
  piVar7 = piVar8;
  for (iVar6 = 0x11; iVar6 != 0; iVar6 = iVar6 + -1) {
    piVar4 = piVar7;
    piVar7 = piVar7 + 1;
    piVar3 = param_1;
    param_1 = param_1 + 1;
    *piVar4 = *piVar3;
  }
  *param_4 = *param_4 + *param_3;
  *param_4 = *param_4 >> 1;
  uVar10 = (undefined2)((ulong)param_3 >> 0x10);
  piVar7 = (int *)param_3;
  uVar5 = piVar7[1];
  iVar6 = piVar7[2];
  puVar1 = (uint *)(piVar8 + 1);
  uVar2 = *puVar1;
  *puVar1 = *puVar1 + uVar5;
  piVar8[2] = piVar8[2] + iVar6 + (uint)CARRY2(uVar2,uVar5);
  puVar1 = (uint *)(piVar8 + 2);
  uVar2 = *puVar1;
  *puVar1 = (int)*puVar1 >> 1;
  piVar8[1] = (uint)piVar8[1] >> 1 | (uint)((uVar2 & 1) != 0) << 0xf;
  uVar5 = piVar7[3];
  iVar6 = piVar7[4];
  puVar1 = (uint *)(piVar8 + 3);
  uVar2 = *puVar1;
  *puVar1 = *puVar1 + uVar5;
  piVar8[4] = piVar8[4] + iVar6 + (uint)CARRY2(uVar2,uVar5);
  puVar1 = (uint *)(piVar8 + 4);
  uVar2 = *puVar1;
  *puVar1 = (int)*puVar1 >> 1;
  piVar8[3] = (uint)piVar8[3] >> 1 | (uint)((uVar2 & 1) != 0) << 0xf;
  uVar5 = piVar7[5];
  iVar6 = piVar7[6];
  puVar1 = (uint *)(piVar8 + 5);
  uVar2 = *puVar1;
  *puVar1 = *puVar1 + uVar5;
  piVar8[6] = piVar8[6] + iVar6 + (uint)CARRY2(uVar2,uVar5);
  puVar1 = (uint *)(piVar8 + 6);
  uVar2 = *puVar1;
  *puVar1 = (int)*puVar1 >> 1;
  piVar8[5] = (uint)piVar8[5] >> 1 | (uint)((uVar2 & 1) != 0) << 0xf;
  _FLAGROTATE3D(piVar8 + 1,uVar9);
  return;
}

