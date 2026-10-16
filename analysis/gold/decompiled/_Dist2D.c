// 2e8c:018c

uint __cdecl16far _Dist2D(uint param_1,uint param_2)

{
  uint uVar1;
  uint uVar2;
  uint uVar3;
  
  uVar1 = (param_1 ^ (int)param_1 >> 0xf) - ((int)param_1 >> 0xf);
  uVar2 = (param_2 ^ (int)param_2 >> 0xf) - ((int)param_2 >> 0xf);
  uVar3 = uRam000575d0;
  if (uVar2 != uVar1) {
    uVar3 = uVar2;
    if (uVar1 <= uVar2) {
      uVar3 = uVar1;
      uVar1 = uVar2;
    }
    uVar3 = *(uint *)((uint)(byte)(((ulong)uVar3 << 0x10) / (ulong)uVar1 >> 8) * 2 + 0x3000);
  }
  uVar3 = (int)((ulong)uVar3 * (ulong)uVar1 >> 0x10) + uVar1;
  if (uVar3 < uVar1) {
    uVar3 = uVar1;
  }
  return uVar3;
}

