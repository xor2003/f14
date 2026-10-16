// 2f0d:0006

uint __cdecl16far _MCGAColors(int param_1,int param_2,byte *param_3)

{
  byte *pbVar1;
  byte *pbVar2;
  byte *pbVar3;
  long lVar4;
  byte bVar5;
  uint uVar6;
  undefined1 uVar8;
  int iVar7;
  undefined1 uVar9;
  int iVar10;
  uint uVar11;
  
  uVar6 = 0;
  if ((bRam00000489 & 2) != 0) {
    do {
      pbVar1 = param_3;
      pbVar2 = param_3 + 1;
      pbVar3 = param_3 + 2;
      param_3 = param_3 + 3;
      lVar4 = (ulong)*pbVar2 * 0x970a + (ulong)*pbVar1 * 0x4ccd + (ulong)*pbVar3 * 0x1c29;
      iVar10 = (int)lVar4;
      iVar7 = iVar10 << 1;
      uVar8 = (undefined1)param_1;
      while( true ) {
        uVar9 = (undefined1)((uint)iVar7 >> 8);
        bVar5 = in(0x3da);
        iVar7 = CONCAT11(uVar9,bVar5);
        if ((bVar5 & 1) != 0) break;
        do {
          bVar5 = in(0x3da);
          iVar7 = CONCAT11((char)((uint)iVar7 >> 8),bVar5);
        } while ((bVar5 & 8) == 0);
      }
      param_1 = CONCAT11((char)((ulong)lVar4 >> 0x10) + (iVar10 < 0),uVar8) + 1;
      out(0x3c8,uVar8);
      uVar8 = (undefined1)((uint)param_1 >> 8);
      out(0x3c9,uVar8);
      out(0x3c9,uVar8);
      out(0x3c9,uVar8);
      param_2 = param_2 + -1;
    } while (param_2 != 0);
    return CONCAT11(uVar9,uVar8);
  }
  do {
    iVar10 = 0x3da;
    do {
      bVar5 = in(0x3da);
      uVar6 = CONCAT11((char)(uVar6 >> 8),bVar5);
    } while ((bVar5 & 8) == 0);
    uVar11 = 0;
    while( true ) {
      do {
        iVar10 = CONCAT11((char)((uint)iVar10 >> 8),200);
        uVar8 = (undefined1)(uVar6 >> 8);
        uVar9 = (undefined1)param_1;
        param_1 = param_1 + 1;
        out(iVar10,uVar9);
        iVar10 = iVar10 + 1;
        out(iVar10,*param_3);
        pbVar2 = param_3 + 2;
        out(iVar10,param_3[1]);
        param_3 = param_3 + 3;
        uVar6 = CONCAT11(uVar8,*pbVar2);
        out(iVar10,*pbVar2);
        param_2 = param_2 + -1;
        if (param_2 == 0) {
          return uVar6;
        }
        uVar11 = uVar11 + 1;
      } while ((int)uVar11 < 0);
      if (uVar11 == 0) break;
      iVar10 = CONCAT11((char)((uint)iVar10 >> 8),0xda);
      bVar5 = in(iVar10);
      uVar6 = CONCAT11(uVar8,bVar5);
      if ((bVar5 & 8) == 0) {
        uVar6 = (uVar11 & 0xff) * -0xd;
        uVar11 = uVar6;
      }
    }
  } while( true );
}

