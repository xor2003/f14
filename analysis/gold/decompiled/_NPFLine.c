// 11ac:01d6

void __cdecl16far _NPFLine(long *param_1,long *param_2,uint param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  uint uVar5;
  uint uVar6;
  long lVar7;
  long lVar8;
  ulong uVar9;
  int iVar10;
  long lVar11;
  long *plVar12;
  long *plVar13;
  undefined2 uVar14;
  undefined2 uVar15;
  undefined2 uVar16;
  
  uVar14 = (undefined2)((ulong)param_1 >> 0x10);
  uVar16 = (undefined2)((ulong)param_2 >> 0x10);
  lVar2 = ((long *)param_2)[1];
  lVar3 = ((long *)param_1)[1];
  lVar1 = lVar2;
  lVar7 = lVar3;
  plVar12 = (long *)param_1;
  uVar15 = uVar14;
  if (lVar2 < lVar3) {
    lVar1 = lVar3;
    lVar7 = lVar2;
    plVar12 = (long *)param_2;
    uVar15 = uVar16;
    uVar16 = uVar14;
    param_2 = param_1;
  }
  uRam0004d15c = (uint)(lVar2 >= lVar3);
  plVar13 = (long *)param_2;
  if ((lRam0004d0fe <= lVar1) && (lVar7 <= lRam0004d106)) {
    lVar2 = (lVar1 - lVar7) + 1;
    if ((param_3 & 3) != 0) {
      if ((param_3 & 3) != 1) {
        lRam0004d14c = (ulong)*(uint *)((int)plVar12 + 0xe) << 0x10;
        lRam0004d158 = (long)((ulong)(*(int *)((int)plVar13 + 0xe) - *(uint *)((int)plVar12 + 0xe))
                             << 0x10) / lVar2;
      }
      lRam0004d148 = (ulong)*(uint *)(plVar12 + 3) << 0x10;
      lRam0004d154 = (long)((ulong)((int)plVar13[3] - *(uint *)(plVar12 + 3)) << 0x10) / lVar2;
    }
    lVar7 = lRam0004d158;
    lVar1 = lRam0004d154;
    lRam0004d144 = *plVar12 * 0x800 + 0x400;
    lVar2 = ((*plVar13 - *plVar12) * 0x800) / lVar2;
    lVar3 = plVar12[1];
    if (lVar3 < lRam0004d0fe) {
      lVar3 = -(lVar3 - lRam0004d0fe);
      lRam0004d14c = lRam0004d14c + lVar3 * lRam0004d158;
      lRam0004d148 = lRam0004d148 + lVar3 * lRam0004d154;
      lRam0004d144 = lRam0004d144 + lVar3 * lVar2;
      lVar3 = lRam0004d0fe;
    }
    lVar8 = plVar13[1];
    if (lRam0004d106 < plVar13[1]) {
      lVar8 = lRam0004d106;
    }
    if (lVar3 < lRam0004d10e) {
      lRam0004d10e = lVar3;
    }
    if (lRam0004d112 < lVar8) {
      lRam0004d112 = lVar8;
    }
    uVar9 = (lVar8 - lVar3) + 1;
    uVar6 = (uint)uVar9;
    iVar4 = (int)lVar3;
    lRam0004d150 = lVar2;
    if ((int)uVar6 < 2) {
      uRam0004d13e = uVar6;
      if (-1 < lVar2) {
        plVar12 = (long *)(iVar4 << 3);
        lVar3 = lRam0004d144;
        lVar8 = lRam0004d14c;
        lVar11 = lRam0004d148;
        do {
          if (lVar3 <= *plVar12) {
            plVar12[1] = lVar8;
            *(long *)((int)plVar12 + 2) = lVar11;
            *plVar12 = lVar3;
          }
          lVar8 = lVar8 + lVar7;
          lVar11 = lVar11 + lVar1;
          lVar3 = lVar3 + lVar2;
          if (plVar12[0x1a4] <= lVar3) {
            plVar12[0x1a5] = lVar8;
            *(long *)((int)plVar12 + 0x692) = lVar11;
            plVar12[0x1a4] = lVar3;
          }
          plVar12 = plVar12 + 2;
          uRam0004d13e = uRam0004d13e - 1;
        } while (uRam0004d13e != 0);
        return;
      }
      plVar12 = (long *)(iVar4 << 3);
      lVar3 = lRam0004d144;
      lVar8 = lRam0004d14c;
      lVar11 = lRam0004d148;
      do {
        if (plVar12[0x1a4] <= lVar3) {
          plVar12[0x1a5] = lVar8;
          *(long *)((int)plVar12 + 0x692) = lVar11;
          plVar12[0x1a4] = lVar3;
        }
        lVar8 = lVar8 + lVar7;
        lVar11 = lVar11 + lVar1;
        lVar3 = lVar3 + lVar2;
        if (lVar3 <= *plVar12) {
          plVar12[1] = lVar8;
          *(long *)((int)plVar12 + 2) = lVar11;
          *plVar12 = lVar3;
        }
        plVar12 = plVar12 + 2;
        uRam0004d13e = uRam0004d13e - 1;
      } while (uRam0004d13e != 0);
      return;
    }
    iVar10 = 0;
    if (uRam0004d15c == 0) {
      if (lVar2 < 0) {
        lRam0004d144 = lRam0004d144 + lVar2;
        lRam0004d148 = lRam0004d148 + lRam0004d154;
        lRam0004d14c = lRam0004d14c + lRam0004d158;
      }
    }
    else {
      iVar10 = 0x690;
      if (-1 < lVar2) {
        lRam0004d144 = lRam0004d144 + lVar2;
        lRam0004d148 = lRam0004d148 + lRam0004d154;
        lRam0004d14c = lRam0004d14c + lRam0004d158;
      }
    }
    lVar2 = lRam0004d14c;
    plVar12 = (long *)(iVar10 + iVar4 * 8);
    if ((param_3 & 3) != 0) {
      if ((param_3 & 3) != 1) {
        uVar5 = uVar6 >> 1;
        plVar13 = plVar12;
        if ((uVar9 & 1) != 0) {
          plVar12[1] = lRam0004d14c;
          lVar2 = lVar2 + lVar7;
          plVar13 = plVar12 + 2;
        }
        for (; uVar5 != 0; uVar5 = uVar5 - 1) {
          plVar13[1] = lVar2;
          plVar13[3] = lVar2 + lVar7;
          lVar2 = lVar2 + lVar7 + lVar7;
          plVar13 = plVar13 + 4;
        }
      }
      lVar3 = lRam0004d154;
      lVar2 = lRam0004d148;
      uVar5 = uVar6 >> 1;
      plVar13 = plVar12;
      if ((uVar9 & 1) != 0) {
        *(long *)((int)plVar12 + 2) = lRam0004d148;
        lVar2 = lVar2 + lVar3;
        plVar13 = plVar12 + 2;
      }
      for (; uVar5 != 0; uVar5 = uVar5 - 1) {
        *(long *)((int)plVar13 + 2) = lVar2;
        *(long *)((int)plVar13 + 10) = lVar2 + lVar3;
        lVar2 = lVar2 + lVar3 + lVar3;
        plVar13 = plVar13 + 4;
      }
    }
    lVar3 = lRam0004d150;
    lVar2 = lRam0004d144;
    uVar6 = uVar6 >> 1;
    if ((uVar9 & 1) != 0) {
      *plVar12 = lRam0004d144;
      lVar2 = lVar2 + lVar3;
      plVar12 = plVar12 + 2;
    }
    for (; uVar6 != 0; uVar6 = uVar6 - 1) {
      *plVar12 = lVar2;
      plVar12[2] = lVar2 + lVar3;
      plVar12 = plVar12 + 4;
      lVar2 = lVar2 + lVar3 + lVar3;
    }
  }
  return;
}

