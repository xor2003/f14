// 2e8c:0118

uint __cdecl16far _ACosB(int param_1,uint param_2)

{
  long lVar1;
  uint uVar2;
  int iVar3;
  
  uVar2 = param_2 >> 2;
  if ((param_2 & 0x4000) == 0) {
    uVar2 = ~uVar2;
  }
  iVar3 = *(int *)(uVar2 & 0xffe);
  if ((int)((uint)(byte)((char)(param_2 >> 8) + 0x40) << 8) < 0) {
    iVar3 = -iVar3;
  }
  lVar1 = (long)((iVar3 >> 1) + 1) * (long)param_1;
  iVar3 = (int)lVar1;
  return ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)(iVar3 < 0)) << 1 | (uint)(iVar3 << 1 < 0);
}

