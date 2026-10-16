// 4c08:06bf

void __cdecl16far _Tacxy(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  uRam00058d8e = 0xffff;
  iVar1 = param_1 - iRam000585c8 >> (7U - cRam0005607e & 0x1f);
  iVar4 = param_2 - iRam00058716 >> (7U - cRam0005607e & 0x1f);
  iVar2 = FUN_28a7_019e(iVar1);
  iVar3 = FUN_28a7_019e(iVar4);
  if ((uint)(iVar3 + iVar2) <= *(uint *)(iRam00055984 + 6)) {
    FUN_4be9_0000(iVar1,iVar4,uRam0004f46a,0xb72c,0xbb2a);
    iRam00058b1a = (iRam00058b1a - (iRam00058b1a >> 3)) + *(int *)(iRam00055984 + 8) / 2;
    iRam0005871c = iRam0005871c + *(int *)(iRam00055984 + 6) / 2;
    if ((((-1 < iRam0005871c) && (iRam0005871c < *(int *)(iRam00055984 + 6))) && (-1 < iRam00058b1a)
        ) && (iRam00058b1a < *(int *)(iRam00055984 + 8))) {
      uRam00058d8e = 0;
    }
  }
  return;
}

