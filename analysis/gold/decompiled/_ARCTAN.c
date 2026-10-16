// 2e8c:005f

int __cdecl16far _ARCTAN(uint param_1,uint param_2)

{
  ulong uVar1;
  char cVar2;
  uint uVar3;
  undefined2 uVar4;
  int iVar6;
  byte bVar5;
  
  iVar6 = 0;
  if ((int)param_2 < 0) {
    iVar6 = 8;
    param_2 = -param_2;
  }
  if ((int)param_1 < 0) {
    iVar6 = iVar6 + 4;
    param_1 = -param_1;
  }
  uVar3 = param_2;
  if (param_2 < param_1) {
    iVar6 = iVar6 + 2;
    uVar3 = param_1;
    param_1 = param_2;
  }
  if ((param_1 < uVar3) && (uVar3 != 0)) {
    uVar1 = ((ulong)param_1 << 0x10) / (ulong)uVar3;
    uVar4 = *(undefined2 *)(iVar6 + 0x3474);
    cVar2 = (char)uVar1;
    iVar6 = (((((uint)(byte)(uVar1 >> 8) << 1 | (uint)(cVar2 < '\0')) << 1 |
              (uint)((char)(cVar2 << 1) < '\0')) << 1 | (uint)((char)(cVar2 << 2) < '\0')) << 1 |
            (uint)((char)(cVar2 << 3) < '\0')) << 1;
  }
  else {
    uVar4 = *(undefined2 *)(iVar6 + 0x3474);
    iVar6 = 0x1ffe;
  }
  bVar5 = (byte)((uint)uVar4 >> 8);
  if (-1 < (char)uVar4) {
    return (uint)bVar5 * 0x100 + *(int *)(iVar6 + 0x1000);
  }
  return (uint)bVar5 * 0x100 - *(int *)(iVar6 + 0x1000);
}

