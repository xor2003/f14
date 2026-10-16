// 11ac:0c62

int __cdecl16far _NSETVIEWMAT(undefined2 param_1,int param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  uint uVar4;
  
  uVar4 = (param_2 << 1) >> 0xf;
  uVar4 = (param_2 << 1 ^ uVar4) - uVar4;
  if (uVar4 < 0x100) {
    uVar4 = 0x100;
  }
  iRam0004d13a = (int)(0x7fff00 / (ulong)uVar4);
  _COPY3X3(param_1,0x126);
  _Trans3X3(0x126,0x138);
  iVar1 = -iRam0004d13a;
  uRam0004d11a = (int)((ulong)((long)iVar1 * (long)(int)uRam0004d11a) >> 0x10) << 1 |
                 (uint)((int)((long)iVar1 * (long)(int)uRam0004d11a) < 0);
  uRam0004d120 = (int)((ulong)((long)iVar1 * (long)(int)uRam0004d120) >> 0x10) << 1 |
                 (uint)((int)((long)iVar1 * (long)(int)uRam0004d120) < 0);
  iVar2 = (int)((long)iVar1 * (long)(int)uRam0004d126);
  iVar3 = iVar2 << 1;
  uRam0004d126 = (int)((ulong)((long)iVar1 * (long)(int)uRam0004d126) >> 0x10) << 1 |
                 (uint)(iVar2 < 0);
  iRam0004d116 = iRam0004d116 >> 1;
  iRam0004d11c = iRam0004d11c >> 1;
  iRam0004d122 = iRam0004d122 >> 1;
  if (iRam0004d13c == 0) {
    iRam0004d118 = -(iRam0004d118 >> 1);
    iRam0004d118 = iRam0004d118 - (iRam0004d118 >> 2);
    iRam0004d11e = -(iRam0004d11e >> 1);
    iRam0004d11e = iRam0004d11e - (iRam0004d11e >> 2);
    iVar3 = -(iRam0004d124 >> 1);
    iVar3 = iVar3 - (iVar3 >> 2);
    iRam0004d124 = iVar3;
  }
  else {
    iRam0004d118 = -(iRam0004d118 >> 1);
    iRam0004d11e = -(iRam0004d11e >> 1);
    iRam0004d124 = -(iRam0004d124 >> 1);
  }
  return iVar3;
}

