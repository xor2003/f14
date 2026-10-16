// 5140:1b4e

/* WARNING: Unable to track spacebase fully for stack */

undefined2 _write(undefined2 param_1,uint param_2,char *param_3,int param_4)

{
  char *pcVar1;
  code *pcVar2;
  char cVar3;
  undefined2 uVar4;
  uint uVar5;
  int iVar6;
  char *pcVar7;
  char *pcVar8;
  undefined2 unaff_SS;
  bool bVar9;
  
  if (uRam00052ad7 <= param_2) {
LAB_5140_1b61:
    uVar4 = __dosretax();
    return uVar4;
  }
  if (iRam00052f82 == -0x292a) {
    (*pcRam00052f84)();
  }
  if ((*(byte *)(param_2 + 0x5ae9) & 0x20) != 0) {
    bVar9 = false;
    pcVar2 = (code *)swi(0x21);
    (*pcVar2)();
    if (bVar9) goto LAB_5140_1b61;
  }
  if ((*(byte *)(param_2 + 0x5ae9) & 0x80) != 0) {
    bVar9 = true;
    iVar6 = param_4;
    pcVar8 = param_3;
    if (param_4 != 0) {
      do {
        if (iVar6 == 0) break;
        iVar6 = iVar6 + -1;
        pcVar1 = pcVar8;
        pcVar8 = pcVar8 + 1;
        bVar9 = *pcVar1 == '\n';
      } while (!bVar9);
      if (!bVar9) goto LAB_5140_1bf9;
      pcVar7 = param_3;
      uVar5 = _stackavail();
      if (uVar5 < 0xa9) {
        uVar4 = __aFchkstk();
        bVar9 = pcVar8 < pcVar7;
        if (pcVar8 != pcVar7) {
          pcVar2 = (code *)swi(0x21);
          uVar5 = (*pcVar2)(iVar6,param_2);
          if ((bVar9) || (uVar5 < (uint)((int)pcVar8 - (int)pcVar7))) {
            uVar4 = __dosretax();
            return uVar4;
          }
        }
        return uVar4;
      }
      pcVar7 = &stack0xfff0;
      pcVar8 = &stack0xfff2;
      do {
        pcVar1 = param_3;
        param_3 = param_3 + 1;
        cVar3 = *pcVar1;
        if (cVar3 == '\n') {
          cVar3 = '\r';
          if (pcVar8 == pcVar7) {
            cVar3 = FUN_5a3f_1cc0();
          }
          pcVar1 = pcVar8;
          pcVar8 = pcVar8 + 1;
          *pcVar1 = cVar3;
          cVar3 = '\n';
        }
        if (pcVar8 == pcVar7) {
          cVar3 = FUN_5a3f_1cc0();
        }
        pcVar1 = pcVar8;
        pcVar8 = pcVar8 + 1;
        *pcVar1 = cVar3;
        param_4 = param_4 + -1;
      } while (param_4 != 0);
      FUN_5a3f_1cc0();
    }
    uVar4 = FUN_5a3f_1d0a();
    return uVar4;
  }
LAB_5140_1bf9:
  uVar4 = FUN_5a3f_1d18();
  return uVar4;
}

