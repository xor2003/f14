// 11ac:113a

void __cdecl16far _NDLine(long *param_1,long *param_2,undefined4 param_3,undefined1 param_4)

{
  undefined1 *puVar1;
  long *plVar2;
  long lVar3;
  long lVar4;
  int iVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  undefined1 *puVar10;
  undefined2 uVar11;
  undefined2 uVar12;
  undefined2 uVar13;
  
  uVar11 = (undefined2)((ulong)param_1 >> 0x10);
  uVar13 = (undefined2)((ulong)param_2 >> 0x10);
  lVar3 = ((long *)param_2)[1];
  lVar4 = ((long *)param_1)[1];
  lVar7 = lVar3;
  uVar12 = uVar11;
  plVar2 = param_2;
  if (lVar3 <= lVar4) {
    lVar7 = lVar4;
    lVar4 = lVar3;
    uVar12 = uVar13;
    uVar13 = uVar11;
    plVar2 = param_1;
    param_1 = param_2;
  }
  if ((lRam0004d0fe <= lVar7) && (lVar4 <= lRam0004d106)) {
    lVar3 = *(long *)param_1;
    lRam0004d144 = lVar3 * 0x800 + 0x400;
    lRam0004d150 = ((*(long *)plVar2 - lVar3) * 0x800) / ((lVar7 - lVar4) + 1);
    lVar3 = ((long *)param_1)[1];
    if (lVar3 < lRam0004d0fe) {
      lVar3 = -(lVar3 - lRam0004d0fe);
      lRam0004d14c = lRam0004d14c + lVar3 * lRam0004d158;
      lRam0004d148 = lRam0004d148 + lVar3 * lRam0004d154;
      lRam0004d144 = lRam0004d144 + lVar3 * lRam0004d150;
      lVar3 = lRam0004d0fe;
    }
    lVar4 = ((long *)plVar2)[1];
    if (lRam0004d106 < lVar4) {
      lVar4 = lRam0004d106;
    }
    iVar9 = (int)lVar3 * 0x140 + (int)param_3;
    iVar5 = 1 - ((int)lVar3 - (int)lVar4);
    lVar3 = lRam0004d144;
    do {
      lVar8 = lVar3 + lRam0004d150;
      lVar3 = lVar3 >> 0xb;
      lVar7 = lVar8 >> 0xb;
      lVar4 = lVar7;
      if (lVar7 < lVar3) {
        lVar4 = lVar3;
        lVar3 = lVar7;
      }
      if (lVar3 < lRam0004d0fa) {
        lVar3 = lRam0004d0fa;
      }
      if (lRam0004d102 < lVar4) {
        lVar4 = lRam0004d102;
      }
      if (-1 < lVar4 - lVar3) {
        puVar10 = (undefined1 *)((int)lVar3 + iVar9);
        for (iVar6 = (int)(lVar4 - lVar3) + 1; iVar6 != 0; iVar6 = iVar6 + -1) {
          puVar1 = puVar10;
          puVar10 = puVar10 + 1;
          *puVar1 = param_4;
        }
      }
      iVar9 = iVar9 + 0x140;
      iVar5 = iVar5 + -1;
      lVar3 = lVar8;
    } while (iVar5 != 0);
  }
  return;
}

