// 5140:1a64

/* WARNING: Removing unreachable block (ram,0x00052f4b) */
/* WARNING: Removing unreachable block (ram,0x00052f40) */

void _read(undefined2 param_1,uint param_2,undefined2 param_3,int param_4)

{
  char *pcVar1;
  char cVar2;
  code *pcVar3;
  undefined2 uVar4;
  undefined1 extraout_AH;
  undefined1 extraout_AH_00;
  int iVar5;
  undefined1 extraout_AH_01;
  int iVar7;
  uint extraout_DX;
  char *pcVar8;
  char *pcVar9;
  undefined1 uVar10;
  bool bVar11;
  undefined4 uVar12;
  char cVar6;
  
  if (((uRam00052ad7 <= param_2) || (param_4 == 0)) || ((*(byte *)(param_2 + 0x5ae9) & 2) != 0)) {
LAB_5140_1ae1:
    __dosretax();
    return;
  }
  uVar10 = uRam00052f82 < 0xd6d6;
  if (uRam00052f82 == 0xd6d6) {
    (*pcRam00052f84)();
  }
  pcVar3 = (code *)swi(0x21);
  uVar12 = (*pcVar3)();
  pcVar9 = (char *)((ulong)uVar12 >> 0x10);
  if ((((bool)uVar10) || ((*(byte *)(param_2 + 0x5ae9) & 0x80) == 0)) ||
     (*(byte *)(param_2 + 0x5ae9) = *(byte *)(param_2 + 0x5ae9) & 0xfb, (int)uVar12 == 0))
  goto LAB_5140_1ae1;
  uVar4 = 0xd00;
  if (*pcVar9 == '\n') {
    *(byte *)(param_2 + 0x5ae9) = *(byte *)(param_2 + 0x5ae9) | 4;
  }
LAB_5140_1ac6:
  pcVar8 = (char *)((ulong)uVar12 >> 0x10);
  iVar7 = (int)uVar12;
  pcVar1 = pcVar8 + 1;
  cVar2 = *pcVar8;
  cVar6 = (char)((uint)uVar4 >> 8);
  uVar4 = CONCAT11(cVar6,cVar2);
  if (cVar2 == cVar6) {
    if (iVar7 != 1) {
      if (*pcVar1 != '\n') goto LAB_5140_1ad6;
      goto LAB_5140_1ad9;
    }
    bVar11 = false;
    if ((*(byte *)(param_2 + 0x5ae9) & 0x40) == 0) {
      pcVar3 = (code *)swi(0x21);
      iVar5 = (*pcVar3)();
      if (!bVar11) {
        uVar10 = 0;
        if (iVar5 != 0) {
          pcVar3 = (code *)swi(0x21);
          (*pcVar3)();
          iVar7 = 1;
          uVar10 = extraout_AH_01;
        }
        uVar4 = CONCAT11(uVar10,0xd);
        goto LAB_5140_1ad6;
      }
      goto LAB_5140_1ae1;
    }
    pcVar3 = (code *)swi(0x21);
    (*pcVar3)();
    bVar11 = false;
    uVar10 = extraout_AH;
    if ((extraout_DX & 0x20) == 0) {
      pcVar3 = (code *)swi(0x21);
      (*pcVar3)();
      uVar10 = extraout_AH_00;
      if (bVar11) goto LAB_5140_1ae1;
    }
    uVar4 = CONCAT11(uVar10,10);
  }
  else if (cVar2 == '\x1a') {
    *(byte *)(param_2 + 0x5ae9) = *(byte *)(param_2 + 0x5ae9) | 2;
    goto LAB_5140_1ae1;
  }
LAB_5140_1ad6:
  *pcVar9 = (char)uVar4;
  pcVar9 = pcVar9 + 1;
LAB_5140_1ad9:
  uVar12 = CONCAT22(pcVar1,iVar7 + -1);
  if (iVar7 + -1 == 0) goto LAB_5140_1ae1;
  goto LAB_5140_1ac6;
}

