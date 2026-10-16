// 5140:073a

byte * __cdecl16far _itoa(int param_1,byte *param_2,uint param_3)

{
  ulong uVar1;
  byte bVar2;
  uint uVar4;
  uint uVar5;
  uint uVar6;
  byte *pbVar7;
  byte *pbVar8;
  byte *pbVar9;
  bool bVar10;
  char cVar3;
  
  uVar5 = 0;
  pbVar8 = param_2;
  pbVar7 = param_2;
  if ((param_3 == 10) && (uVar5 = param_1 >> 0xf, (int)uVar5 < 0)) {
    pbVar8 = param_2 + 1;
    *param_2 = 0x2d;
    bVar10 = param_1 != 0;
    param_1 = -param_1;
    uVar5 = -(uVar5 + bVar10);
    pbVar7 = pbVar8;
  }
  do {
    uVar6 = 0;
    uVar4 = uVar5;
    if (uVar5 != 0) {
      uVar4 = uVar5 / param_3;
      uVar6 = uVar5 % param_3;
    }
    uVar1 = CONCAT22(uVar6,param_1);
    param_1 = (int)(uVar1 / param_3);
    cVar3 = (char)(uVar1 % (ulong)param_3);
    bVar2 = cVar3 + 0x30;
    if (0x39 < bVar2) {
      bVar2 = cVar3 + 0x57;
    }
    pbVar9 = pbVar8 + 1;
    *pbVar8 = bVar2;
    uVar5 = uVar4;
    pbVar8 = pbVar9;
  } while (uVar4 != 0 || param_1 != 0);
  *pbVar9 = 0;
  do {
    pbVar9 = pbVar9 + -1;
    LOCK();
    bVar2 = *pbVar9;
    *pbVar9 = *pbVar7;
    UNLOCK();
    *pbVar7 = bVar2;
    pbVar8 = pbVar7 + 2;
    pbVar7 = pbVar7 + 1;
  } while (pbVar8 < pbVar9);
  return param_2;
}

