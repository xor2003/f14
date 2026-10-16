// 5140:0f0c

undefined4 __stdcall16far __aFuldiv(uint param_1,uint param_2,uint param_3,uint param_4)

{
  ulong uVar1;
  long lVar2;
  uint uVar3;
  uint uVar4;
  int iVar5;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  uint uVar9;
  uint uVar10;
  
  uVar4 = param_1;
  uVar9 = param_4;
  uVar3 = param_2;
  uVar10 = param_3;
  if (param_4 == 0) {
    uVar4 = param_2 / param_3;
    iVar5 = (int)(((ulong)param_2 % (ulong)param_3 << 0x10 | (ulong)param_1) / (ulong)param_3);
  }
  else {
    do {
      uVar8 = uVar3;
      uVar6 = uVar4;
      uVar7 = uVar9 >> 1;
      uVar10 = uVar10 >> 1 | (uint)((uVar9 & 1) != 0) << 0xf;
      uVar4 = uVar6 >> 1 | (uint)((uVar8 & 1) != 0) << 0xf;
      uVar9 = uVar7;
      uVar3 = uVar8 >> 1;
    } while (uVar7 != 0);
    uVar1 = (CONCAT22(uVar8,uVar6) >> 1) / (ulong)uVar10;
    iVar5 = (int)uVar1;
    lVar2 = (ulong)param_3 * (uVar1 & 0xffff);
    uVar4 = (uint)((ulong)lVar2 >> 0x10);
    uVar9 = uVar4 + iVar5 * param_4;
    if (((CARRY2(uVar4,iVar5 * param_4)) || (param_2 < uVar9)) ||
       ((param_2 <= uVar9 && (param_1 < (uint)lVar2)))) {
      iVar5 = iVar5 + -1;
    }
    uVar4 = 0;
  }
  return CONCAT22(uVar4,iVar5);
}

