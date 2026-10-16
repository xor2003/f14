// 2c92:014d

void __cdecl16near _JOYCONVERT(void)

{
  ulong uVar1;
  byte bVar2;
  byte bVar4;
  uint uVar3;
  uint uVar5;
  uint in_BX;
  int iVar6;
  
  iVar6 = in_BX * 2;
  uVar3 = *(uint *)(iVar6 + 0x30cc);
  uVar5 = uVar3 - *(uint *)(iVar6 + 0x30b4);
  if (uVar3 < *(uint *)(iVar6 + 0x30b4)) {
    if (*(uint *)(iVar6 + 0x30a4) < uVar3) {
      uVar1 = ((ulong)-uVar5 << 0x10) / (ulong)*(uint *)(iVar6 + 0x30bc);
      uVar3 = (uint)uVar1 >> 1;
      if ((byte)((byte)(uVar1 >> 8) >> 1) < bRam000500c8) {
        uVar3 = 0;
      }
      bVar4 = (byte)(-uVar3 >> 8);
    }
    else {
      bVar4 = 0x81;
    }
  }
  else if (uVar5 == 0) {
    bVar4 = 0;
  }
  else if (uVar3 < *(uint *)(iVar6 + 0x30ac)) {
    bVar2 = (byte)(((ulong)uVar5 << 0x10) / (ulong)*(uint *)(iVar6 + 0x30c4) >> 8);
    bVar4 = bVar2 >> 1;
    if (bVar2 >> 1 < bRam000500c8) {
      bVar4 = 0;
    }
  }
  else {
    bVar4 = 0x7f;
  }
  *(byte *)((in_BX & 0x7fff) + 0x30d4) = bVar4;
  return;
}

