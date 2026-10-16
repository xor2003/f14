// 5140:05c2

uint __cdecl16far __filbuf(undefined2 *param_1)

{
  byte *pbVar1;
  byte bVar2;
  undefined2 uVar3;
  int iVar4;
  uint uVar5;
  
  bVar2 = *(byte *)(param_1 + 3);
  if (((bVar2 & 0x83) != 0) && ((bVar2 & 0x40) == 0)) {
    if ((bVar2 & 2) == 0) {
      *(byte *)(param_1 + 3) = bVar2 | 1;
      if (((bVar2 & 0xc) == 0) && ((*(byte *)(param_1 + 0x50) & 1) == 0)) {
        __getbuf(param_1);
      }
      uVar3 = param_1[2];
      *param_1 = uVar3;
      uVar5 = (uint)*(byte *)((int)param_1 + 7);
      iVar4 = _read(0x5140,uVar5,uVar3,param_1[0x51]);
      if (iVar4 == 0) {
        *(byte *)(param_1 + 3) = *(byte *)(param_1 + 3) | 0x10;
      }
      else {
        if (iVar4 != -1) {
          if (((*(byte *)(uVar5 + 0x5ae9) & 0x82) == 0x82) && ((*(byte *)(param_1 + 3) & 0x82) == 0)
             ) {
            pbVar1 = (byte *)(param_1 + 0x50);
            *pbVar1 = *pbVar1 | 0x20;
          }
          param_1[1] = iVar4 + -1;
          bVar2 = *(byte *)*param_1;
          *param_1 = (byte *)*param_1 + 1;
          return (uint)bVar2;
        }
        *(byte *)(param_1 + 3) = *(byte *)(param_1 + 3) | 0x20;
      }
      param_1[1] = 0;
    }
    else {
      *(byte *)(param_1 + 3) = *(byte *)(param_1 + 3) | 0x20;
    }
  }
  return 0xffff;
}

