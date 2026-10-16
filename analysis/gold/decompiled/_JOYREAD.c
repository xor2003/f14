// 2c92:00e1

undefined2 __cdecl16near _JOYREAD(void)

{
  code *pcVar1;
  byte bVar2;
  undefined2 in_AX;
  byte bVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  undefined4 uVar9;
  
  iVar5 = 0;
  iVar6 = 0;
  iVar7 = 0;
  iVar8 = 0;
  iVar4 = -1;
  if (iRam000500c9 == 1) {
    pcVar1 = (code *)swi(0x15);
    iVar6 = iRam000500cb;
    uVar9 = (*pcVar1)();
    iVar5 = (int)uVar9;
    iVar7 = iVar4;
  }
  else {
    out(0x201,(char)in_AX);
    do {
      bVar3 = (byte)((uint)in_AX >> 8);
      bVar2 = in(0x201);
      bVar2 = bVar2 & bVar3;
      uVar9 = CONCAT22(iVar8,CONCAT11(bVar3,bVar2));
      if (bVar2 == 0) break;
      iVar5 = iVar5 + (uint)(bVar2 & 1);
      iVar6 = iVar6 + (uint)(bVar2 >> 1 & 1);
      iVar7 = iVar7 + (uint)(bVar2 >> 2 & 1);
      in_AX = CONCAT11(bVar3,bVar2 >> 4);
      iVar8 = iVar8 + (uint)(bVar2 >> 3 & 1);
      uVar9 = CONCAT22(iVar8,in_AX);
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  iRam000500bc = iVar5;
  iRam000500be = iVar6;
  iRam000500c0 = iVar7;
  uRam000500c2 = (int)((ulong)uVar9 >> 0x10);
  return (int)uVar9;
}

