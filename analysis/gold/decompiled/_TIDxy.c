// 4d7e:0708

void __cdecl16far _TIDxy(int param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  
  iVar1 = param_1 - iRam000585c8 >> (7U - cRam0005607e & 0x1f);
  iVar4 = param_2 - iRam00058716 >> (7U - cRam0005607e & 0x1f);
  iRam0005373e = 0;
  iRam00053960 = 0;
  iVar2 = FUN_28a7_019e(iVar1);
  iVar3 = FUN_28a7_019e(iVar4);
  if ((uint)(iVar3 + iVar2) <= *(uint *)(iRam00055984 + 6)) {
    FUN_4be9_0000(iVar1,iVar4,uRam0004f46a,0x674e,0x6970);
    iRam00053960 = iRam00053960 + (iRam00053960 >> 1) + *(int *)(iRam00055984 + 8);
    iRam0005373e = iRam0005373e + *(int *)(iRam00055984 + 6) / 2;
  }
  return;
}

