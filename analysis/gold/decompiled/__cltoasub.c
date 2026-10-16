// 5140:1e04

byte * __cdecl16far __cltoasub(void)

{
  uint uVar1;
  byte *pbVar2;
  ulong uVar3;
  byte bVar4;
  uint uVar6;
  uint uVar7;
  uint uVar8;
  char in_BL;
  int iVar9;
  int unaff_BP;
  byte *pbVar10;
  byte *pbVar11;
  byte *pbVar12;
  undefined2 unaff_SS;
  bool bVar13;
  char cVar5;
  
  uVar1 = *(uint *)(unaff_BP + 0xc);
  iVar9 = *(int *)(unaff_BP + 6);
  uVar7 = *(uint *)(unaff_BP + 8);
  pbVar2 = (byte *)*(undefined2 *)(unaff_BP + 10);
  pbVar11 = pbVar2;
  pbVar10 = pbVar2;
  if (((in_BL != '\0') && (uVar1 == 10)) && ((int)uVar7 < 0)) {
    pbVar11 = pbVar2 + 1;
    *pbVar2 = 0x2d;
    bVar13 = iVar9 != 0;
    iVar9 = -iVar9;
    uVar7 = -(uVar7 + bVar13);
    pbVar10 = pbVar11;
  }
  do {
    uVar8 = 0;
    uVar6 = uVar7;
    if (uVar7 != 0) {
      uVar6 = uVar7 / uVar1;
      uVar8 = uVar7 % uVar1;
    }
    uVar3 = CONCAT22(uVar8,iVar9);
    iVar9 = (int)(uVar3 / uVar1);
    cVar5 = (char)(uVar3 % (ulong)uVar1);
    bVar4 = cVar5 + 0x30;
    if (0x39 < bVar4) {
      bVar4 = cVar5 + 0x57;
    }
    pbVar12 = pbVar11 + 1;
    *pbVar11 = bVar4;
    uVar7 = uVar6;
    pbVar11 = pbVar12;
  } while (uVar6 != 0 || iVar9 != 0);
  *pbVar12 = 0;
  do {
    pbVar12 = pbVar12 + -1;
    LOCK();
    bVar4 = *pbVar12;
    *pbVar12 = *pbVar10;
    UNLOCK();
    *pbVar10 = bVar4;
    pbVar11 = pbVar10 + 2;
    pbVar10 = pbVar10 + 1;
  } while (pbVar11 < pbVar12);
  return pbVar2;
}

