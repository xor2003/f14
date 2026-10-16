// 11ac:0554

/* WARNING: Globals starting with '_' overlap smaller symbols at the same address */
/* WARNING: Restarted to delay deadcode elimination for space: stack */

void __cdecl16far _NPFFill(ulong param_1,uint param_2,ulong param_3)

{
  byte *pbVar1;
  byte *pbVar2;
  ulong *puVar3;
  undefined1 uVar4;
  char cVar5;
  int iVar6;
  long lVar7;
  long lVar8;
  int iVar9;
  uint uVar10;
  uint uVar11;
  long lVar12;
  ulong uVar13;
  int iVar14;
  long lVar15;
  long lVar16;
  char cVar17;
  char cVar18;
  long *plVar19;
  long lVar20;
  byte *pbVar21;
  ulong *puVar22;
  undefined1 *puVar23;
  char *pcVar24;
  undefined2 uVar25;
  
  if (((lRam0004d10e <= _FUN_4cb5_05c2) && (iVar6 = (int)lRam0004d10e, iVar6 != 0x7fff)) &&
     (iVar14 = (int)_FUN_4cb5_05c2 - iVar6, -1 < iVar14)) {
    iVar14 = iVar14 + 1;
    param_1 = param_1 + lRam0004d10e * 0x140;
    plVar19 = (long *)(iVar6 << 3);
    if ((param_2 & 3) != 2) {
      if ((param_2 & 3) == 1) {
        do {
          iRam0004d13e = iVar14;
          uVar25 = (undefined2)(param_1 >> 0x10);
          lVar7 = *plVar19;
          *plVar19 = 0x7fffffff;
          lVar7 = lVar7 >> 0xb;
          lVar12 = plVar19[0x1a4];
          plVar19[0x1a4] = -1;
          lVar12 = lVar12 >> 0xb;
          if (((lVar7 <= lVar12) && (lVar7 <= lRam0004d102)) && (lRam0004d0fa <= lVar12)) {
            lVar8 = (long)((ulong)(uint)((int)plVar19[0x1a5] - (int)plVar19[1]) << 0x10) /
                    ((lVar12 - lVar7) + 1);
            lVar15 = (ulong)*(uint *)(plVar19 + 1) * 0x10000;
            if (lVar7 < lRam0004d0fa) {
              lVar15 = lVar15 + -(lVar7 - lRam0004d0fa) * lVar8;
              lVar7 = lRam0004d0fa;
            }
            puVar23 = (undefined1 *)((int)param_1 + (int)lVar7);
            lVar16 = lVar15 * 0x100;
            lVar8 = lVar8 * 0x100;
            if (lRam0004d102 < lVar12) {
              lVar12 = lRam0004d102;
            }
            uVar11 = (int)lVar12 - (int)lVar7;
            uVar10 = uVar11 + 1;
            if (((uint)puVar23 & 1) != 0) {
              lVar16 = lVar16 + lVar8;
              *puVar23 = (char)((ulong)lVar15 >> 0x10);
              puVar23 = puVar23 + 1;
              uVar10 = uVar11;
              if (uVar11 == 0) goto LAB_11ac_08d2;
            }
            uVar11 = uVar10 >> 3;
            uVar13 = (ulong)uVar11 | (ulong)uVar10 << 0x1d;
            while (uVar4 = (undefined1)((ulong)lVar16 >> 0x18), uVar11 != 0) {
              *puVar23 = uVar4;
              puVar23[1] = (char)((ulong)(lVar16 + lVar8) >> 0x18);
              lVar16 = lVar16 + lVar8 + lVar8;
              puVar23[2] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23[3] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23[4] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23[5] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23[6] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23[7] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23 = puVar23 + 8;
              uVar11 = (int)uVar13 - 1;
              uVar13 = CONCAT22((int)(uVar13 >> 0x10),uVar11);
            }
            if ((long)uVar13 < 0) {
              *puVar23 = uVar4;
              puVar23[1] = (char)((ulong)(lVar16 + lVar8) >> 0x18);
              lVar16 = lVar16 + lVar8 + lVar8;
              puVar23[2] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23[3] = (char)((ulong)lVar16 >> 0x18);
              lVar16 = lVar16 + lVar8;
              puVar23 = puVar23 + 4;
            }
            if ((long)(uVar13 << 1) < 0) {
              *puVar23 = (char)((ulong)lVar16 >> 0x18);
              puVar23[1] = (char)((ulong)(lVar16 + lVar8) >> 0x18);
              lVar16 = lVar16 + lVar8 + lVar8;
              puVar23 = puVar23 + 2;
            }
            if ((long)(uVar13 << 2) < 0) {
              *puVar23 = (char)((ulong)lVar16 >> 0x18);
            }
          }
LAB_11ac_08d2:
          plVar19 = plVar19 + 2;
          param_1 = param_1 + 0x140;
          iRam0004d13e = iRam0004d13e + -1;
          iVar14 = iRam0004d13e;
          if (iRam0004d13e == 0) {
            lRam0004d10e = 0x7fff;
            _FUN_4cb5_05c2 = 0xffffffff;
            return;
          }
        } while( true );
      }
      uVar25 = (undefined2)(param_1 >> 0x10);
      param_1 = param_1 & 0xffff;
      cVar17 = (char)(param_3 >> 8);
      if (cVar17 == -6) {
        do {
          lVar7 = *plVar19;
          *plVar19 = 0x7fffffff;
          lVar7 = lVar7 >> 0xb;
          lVar12 = plVar19[0x1a4];
          plVar19[0x1a4] = -1;
          iVar6 = iRam0004d274;
          lVar12 = lVar12 >> 0xb;
          if (((lVar7 <= lVar12) && (lVar7 <= lRam0004d102)) && (lRam0004d0fa <= lVar12)) {
            if (lVar7 < lRam0004d0fa) {
              lVar7 = lRam0004d0fa;
            }
            if (lRam0004d102 < lVar12) {
              lVar12 = lRam0004d102;
            }
            iVar9 = ((int)lVar12 - (int)lVar7) + 1;
            pbVar21 = (byte *)((int)param_1 + (int)lVar7);
            do {
              pbVar1 = pbVar21;
              pbVar2 = pbVar21;
              pbVar21 = pbVar21 + 1;
              *pbVar2 = *(byte *)(ulong)(iVar6 + (uint)*pbVar1);
              iVar9 = iVar9 + -1;
            } while (iVar9 != 0);
          }
          plVar19 = plVar19 + 2;
          param_1 = (ulong)((int)param_1 + 0x140);
          iVar14 = iVar14 + -1;
        } while (iVar14 != 0);
        lRam0004d10e = 0x7fff;
        _FUN_4cb5_05c2 = 0xffffffff;
        return;
      }
      do {
        lVar7 = *plVar19;
        *plVar19 = 0x7fffffff;
        lVar7 = lVar7 >> 0xb;
        lVar12 = plVar19[0x1a4];
        plVar19[0x1a4] = -1;
        lVar12 = lVar12 >> 0xb;
        if (((lVar7 <= lVar12) && (lVar7 <= lRam0004d102)) && (lRam0004d0fa <= lVar12)) {
          if (lVar7 < lRam0004d0fa) {
            lVar7 = lRam0004d0fa;
          }
          if (lRam0004d102 < lVar12) {
            lVar12 = lRam0004d102;
          }
          uVar10 = (int)lVar12 - (int)lVar7;
          puVar22 = (ulong *)((int)param_1 + (int)lVar7);
          uVar13 = param_3 & 0xffff;
          if (((uint)plVar19 & 8) != 0) {
            uVar13 = (ulong)CONCAT11((char)param_3,cVar17);
          }
          uVar11 = uVar10 + 1;
          if (((uint)puVar22 & 1) != 0) {
            *(char *)puVar22 = (char)(uVar13 >> 8);
            puVar22 = (ulong *)((int)puVar22 + 1);
            uVar11 = uVar10;
            if (uVar10 == 0) goto LAB_11ac_0704;
          }
          if (7 < uVar11) {
            uVar13 = CONCAT22((int)uVar13,(int)uVar13);
            for (uVar10 = uVar11 >> 2; uVar10 != 0; uVar10 = uVar10 - 1) {
              puVar3 = puVar22;
              puVar22 = puVar22 + 1;
              *puVar3 = uVar13;
            }
            uVar11 = uVar11 & 3;
          }
          uVar10 = uVar11 >> 1;
          if (uVar10 != 0) {
            for (; uVar10 != 0; uVar10 = uVar10 - 1) {
              puVar3 = puVar22;
              puVar22 = (ulong *)((int)puVar22 + 2);
              *(int *)puVar3 = (int)uVar13;
            }
          }
          if ((uVar11 & 1) != 0) {
            *(char *)puVar22 = (char)uVar13;
          }
        }
LAB_11ac_0704:
        plVar19 = plVar19 + 2;
        param_1 = (ulong)((int)param_1 + 0x140);
        iVar14 = iVar14 + -1;
        if (iVar14 == 0) {
          lRam0004d10e = 0x7fff;
          _FUN_4cb5_05c2 = 0xffffffff;
          return;
        }
      } while( true );
    }
    iVar6 = (int)(param_3 >> 0x10) + ((uint)param_3 >> 4);
    do {
      uVar25 = (undefined2)(param_1 >> 0x10);
      lVar7 = *plVar19;
      *plVar19 = 0x7fffffff;
      lVar7 = lVar7 >> 0xb;
      lVar12 = plVar19[0x1a4];
      plVar19[0x1a4] = -1;
      lVar12 = lVar12 >> 0xb;
      if (((lVar7 <= lVar12) && (lVar7 <= lRam0004d102)) && (lRam0004d0fa <= lVar12)) {
        lVar8 = (lVar12 - lVar7) + 1;
        lVar15 = (long)((ulong)(uint)(*(int *)((int)plVar19 + 0x696) - *(int *)((int)plVar19 + 6))
                       << 0x10) / lVar8;
        lVar8 = (long)((ulong)(uint)((int)plVar19[0x1a5] - (int)plVar19[1]) << 0x10) / lVar8;
        lVar16 = (ulong)*(uint *)(plVar19 + 1) * 0x10000;
        lVar20 = (ulong)*(uint *)((int)plVar19 + 6) * 0x10000;
        if (lVar7 < lRam0004d0fa) {
          lVar16 = lVar16 + -(lVar7 - lRam0004d0fa) * lVar8;
          lVar20 = lVar20 + -(lVar7 - lRam0004d0fa) * lVar15;
          lVar7 = lRam0004d0fa;
        }
        if (lRam0004d102 < lVar12) {
          lVar12 = lRam0004d102;
        }
        uVar10 = ((int)lVar12 - (int)lVar7) + 1;
        pcVar24 = (char *)((int)param_1 + (int)lVar7);
        uVar13 = lVar20 << 8;
        lVar12 = lVar8 * 0x100;
        lVar20 = lVar15 * 0x100;
        lVar7 = CONCAT31((int3)lVar16,(char)(param_2 >> 8));
        if ((param_2 & 0x7c) == 0) {
          if ((param_2 & 0x80) == 0) {
            if (7 < (int)uVar10) {
              uVar11 = uVar10 >> 2;
              do {
                *pcVar24 = *(char *)((uint)((uVar13 >> 0x18) << 8) |
                                    (uint)(byte)((ulong)lVar7 >> 0x18)) + (char)lVar7;
                lVar7 = lVar7 + lVar12;
                pcVar24[1] = *(char *)((uint)((uVar13 + lVar20 >> 0x18) << 8) |
                                      (uint)(byte)((ulong)lVar7 >> 0x18)) + (char)lVar7;
                lVar7 = lVar7 + lVar12;
                uVar13 = uVar13 + lVar20 + lVar20;
                pcVar24[2] = *(char *)((uint)((uVar13 >> 0x18) << 8) |
                                      (uint)(byte)((ulong)lVar7 >> 0x18)) + (char)lVar7;
                lVar7 = lVar7 + lVar12;
                uVar13 = uVar13 + lVar20;
                pcVar24[3] = *(char *)((uint)((uVar13 >> 0x18) << 8) |
                                      (uint)(byte)((ulong)lVar7 >> 0x18)) + (char)lVar7;
                lVar7 = lVar7 + lVar12;
                uVar13 = uVar13 + lVar20;
                pcVar24 = pcVar24 + 4;
                uVar11 = uVar11 - 1;
              } while (uVar11 != 0);
              uVar10 = uVar10 & 3;
              if (uVar10 == 0) goto LAB_11ac_0c07;
            }
            do {
              *pcVar24 = *(char *)((uint)((uVar13 >> 0x18) << 8) |
                                  (uint)(byte)((ulong)lVar7 >> 0x18)) + (char)lVar7;
              lVar7 = lVar7 + lVar12;
              uVar13 = uVar13 + lVar20;
              pcVar24 = pcVar24 + 1;
              uVar10 = uVar10 - 1;
            } while (uVar10 != 0);
          }
          else {
            if (7 < (int)uVar10) {
              uVar11 = uVar10 >> 2;
              do {
                cVar17 = *(char *)((uint)((uVar13 >> 0x18) << 8) |
                                  (uint)(byte)((ulong)lVar7 >> 0x18));
                if (cVar17 != '\0') {
                  *pcVar24 = cVar17 + (char)lVar7;
                }
                lVar7 = lVar7 + lVar12;
                cVar17 = *(char *)((uint)((uVar13 + lVar20 >> 0x18) << 8) |
                                  (uint)(byte)((ulong)lVar7 >> 0x18));
                if (cVar17 != '\0') {
                  pcVar24[1] = cVar17 + (char)lVar7;
                }
                lVar7 = lVar7 + lVar12;
                uVar13 = uVar13 + lVar20 + lVar20;
                cVar17 = *(char *)((uint)((uVar13 >> 0x18) << 8) |
                                  (uint)(byte)((ulong)lVar7 >> 0x18));
                if (cVar17 != '\0') {
                  pcVar24[2] = cVar17 + (char)lVar7;
                }
                lVar7 = lVar7 + lVar12;
                uVar13 = uVar13 + lVar20;
                cVar17 = *(char *)((uint)((uVar13 >> 0x18) << 8) |
                                  (uint)(byte)((ulong)lVar7 >> 0x18));
                if (cVar17 != '\0') {
                  pcVar24[3] = cVar17 + (char)lVar7;
                }
                lVar7 = lVar7 + lVar12;
                uVar13 = uVar13 + lVar20;
                pcVar24 = pcVar24 + 4;
                uVar11 = uVar11 - 1;
              } while (uVar11 != 0);
              uVar10 = uVar10 & 3;
              if (uVar10 == 0) goto LAB_11ac_0c07;
            }
            do {
              cVar17 = *(char *)((uint)((uVar13 >> 0x18) << 8) | (uint)(byte)((ulong)lVar7 >> 0x18))
              ;
              if (cVar17 != '\0') {
                *pcVar24 = cVar17 + (char)lVar7;
              }
              lVar7 = lVar7 + lVar12;
              uVar13 = uVar13 + lVar20;
              pcVar24 = pcVar24 + 1;
              uVar10 = uVar10 - 1;
            } while (uVar10 != 0);
          }
        }
        else {
          lVar8 = lVar8 * 0x200;
          lVar15 = lVar15 * 0x200;
          if (3 < uVar10) {
            uVar11 = uVar10 >> 2;
            do {
              cVar17 = *(char *)((uint)((uVar13 >> 0x18) << 8) | (uint)(byte)((ulong)lVar7 >> 0x18))
              ;
              if (cVar17 != '\0') {
                cVar17 = cVar17 + (char)lVar7;
                *pcVar24 = cVar17;
                pcVar24[1] = cVar17 + '\x01';
              }
              lVar7 = lVar7 + lVar8;
              cVar17 = *(char *)((uint)((uVar13 + lVar15 >> 0x18) << 8) |
                                (uint)(byte)((ulong)lVar7 >> 0x18));
              if (cVar17 != '\0') {
                cVar17 = cVar17 + (char)lVar7;
                pcVar24[2] = cVar17;
                pcVar24[3] = cVar17 + '\x01';
              }
              lVar7 = lVar7 + lVar8;
              uVar13 = uVar13 + lVar15 + lVar15;
              pcVar24 = pcVar24 + 4;
              uVar11 = uVar11 - 1;
            } while (uVar11 != 0);
            uVar10 = uVar10 & 3;
            if (uVar10 == 0) goto LAB_11ac_0c07;
          }
          do {
            cVar17 = *(char *)((uint)((uVar13 >> 0x18) << 8) | (uint)(byte)((ulong)lVar7 >> 0x18));
            cVar5 = (char)lVar7;
            cVar18 = '\0';
            if (cVar17 != '\0') {
              cVar17 = cVar17 + cVar5;
              *pcVar24 = cVar17;
              cVar18 = cVar17 - cVar5;
            }
            if (uVar10 == 1) break;
            if (cVar18 != '\0') {
              pcVar24[1] = cVar18 + cVar5 + '\x01';
            }
            pcVar24 = pcVar24 + 2;
            lVar7 = lVar7 + lVar8;
            uVar13 = uVar13 + lVar15;
            uVar10 = uVar10 - 2;
          } while (uVar10 != 0);
        }
      }
LAB_11ac_0c07:
      plVar19 = plVar19 + 2;
      param_1 = param_1 + 0x140;
      iVar14 = iVar14 + -1;
    } while (iVar14 != 0);
  }
  lRam0004d10e = 0x7fff;
  _FUN_4cb5_05c2 = 0xffffffff;
  return;
}

