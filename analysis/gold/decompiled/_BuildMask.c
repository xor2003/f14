// 2f18:00e1

int __cdecl16far
_BuildMask(int *param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  char *pcVar1;
  undefined2 uVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int *piVar6;
  int *piVar7;
  int *piVar8;
  char *pcVar9;
  char *pcVar10;
  undefined2 uVar11;
  bool bVar12;
  
  uVar2 = thunk_EXT_FUN_0000_0000(0x2f18,param_6 * 2 + 10,0x4cff);
  uVar11 = (undefined2)((ulong)param_1 >> 0x10);
  piVar6 = (int *)param_1;
  *param_1 = param_2;
  piVar6[1] = param_3;
  piVar6[2] = param_4;
  piVar6[3] = param_5;
  pcVar9 = (char *)(param_2 + param_3 * 0x140);
  bVar12 = true;
  iVar3 = param_4;
  pcVar10 = pcVar9;
  piVar8 = piVar6 + 4;
  iVar4 = param_4;
code_r0x0002f2a6:
  do {
    piVar7 = piVar8;
    if (iVar3 != 0) {
      iVar3 = iVar3 + -1;
      pcVar1 = pcVar9;
      pcVar9 = pcVar9 + 1;
      bVar12 = *pcVar1 == '\0';
      piVar8 = piVar7;
      if (!bVar12) goto code_r0x0002f2a6;
    }
    if (bVar12) {
      iVar3 = iVar3 + 1;
      pcVar9 = pcVar9 + -1;
    }
    *piVar7 = iVar4 - iVar3;
    bVar12 = iVar3 == 0;
    iVar4 = iVar3;
    do {
      if (iVar4 == 0) break;
      iVar4 = iVar4 + -1;
      pcVar1 = pcVar9;
      pcVar9 = pcVar9 + 1;
      bVar12 = *pcVar1 == '\0';
    } while (bVar12);
    if (!bVar12) {
      iVar4 = iVar4 + 1;
      pcVar9 = pcVar9 + -1;
    }
    iVar5 = iVar3 - iVar4;
    piVar7[1] = iVar5;
    piVar8 = piVar7 + 2;
    bVar12 = false;
    iVar3 = iVar4;
    if (iVar4 == 0) {
      if ((iVar5 == 0) && (param_4 != *piVar7)) {
        piVar8 = piVar7;
      }
      piVar8[-1] = piVar8[-1] | 0x8000;
      pcVar9 = pcVar10 + 0x140;
      param_5 = param_5 + -1;
      bVar12 = false;
      iVar3 = param_4;
      pcVar10 = pcVar9;
      iVar4 = param_4;
      if (param_5 == 0) {
        if ((iVar5 == 0) && (param_4 != *piVar8)) {
          piVar8 = piVar8 + 2;
        }
        return (int)piVar8 + (1 - (int)piVar6);
      }
    }
  } while( true );
}

