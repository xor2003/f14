// 5140:19ea

void _lseek(undefined2 param_1,uint param_2,uint param_3,uint param_4,uint param_5)

{
  uint uVar1;
  code *pcVar2;
  uint uVar3;
  uint uVar4;
  bool bVar5;
  undefined4 uVar6;
  
  if (uRam00052ad7 <= param_2) goto LAB_5140_1a61;
  bVar5 = false;
  if ((param_4 & 0x8000) != 0) {
    if (param_5 == 0) goto LAB_5140_1a61;
    bVar5 = false;
    pcVar2 = (code *)swi(0x21);
    uVar6 = (*pcVar2)();
    uVar3 = (uint)((ulong)uVar6 >> 0x10);
    if (bVar5) goto LAB_5140_1a61;
    if ((param_5 & 2) == 0) {
      uVar1 = (uint)CARRY2((uint)uVar6,param_3);
      bVar5 = CARRY2(uVar3,param_4) || CARRY2(uVar3 + param_4,uVar1);
      if ((int)(uVar3 + param_4 + uVar1) < 0) goto LAB_5140_1a61;
    }
    else {
      pcVar2 = (code *)swi(0x21);
      uVar6 = (*pcVar2)(uVar3);
      uVar4 = (uint)((ulong)uVar6 >> 0x10);
      uVar3 = (uint)CARRY2((uint)uVar6,param_3);
      uVar1 = uVar4 + param_4;
      bVar5 = CARRY2(uVar4,param_4) || CARRY2(uVar1,uVar3);
      if ((int)(uVar1 + uVar3) < 0) {
        pcVar2 = (code *)swi(0x21);
        (*pcVar2)();
        goto LAB_5140_1a61;
      }
    }
  }
  pcVar2 = (code *)swi(0x21);
  (*pcVar2)();
  if (!bVar5) {
    *(byte *)(param_2 + 0x5ae9) = *(byte *)(param_2 + 0x5ae9) & 0xfd;
  }
LAB_5140_1a61:
  __dosretax();
  return;
}

