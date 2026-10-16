// 28a7:01bd

int __cdecl16far _IntDiv(uint param_1,uint param_2)

{
  int iVar1;
  ulong uVar2;
  char local_4;
  char local_3;
  
  local_4 = '\x01';
  local_3 = '\x01';
  if ((int)param_1 < 0) {
    local_4 = -1;
  }
  if ((int)param_2 < 0) {
    local_3 = -1;
  }
  iVar1 = (param_2 ^ (int)param_2 >> 0xf) - ((int)param_2 >> 0xf);
  uVar2 = __aFuldiv(0,(param_1 ^ (int)param_1 >> 0xf) - ((int)param_1 >> 0xf),iVar1,iVar1 >> 0xf);
  return ((uint)uVar2 >> 1 | (uint)((uVar2 & 0x10000) != 0) << 0xf) * (int)local_3 * (int)local_4;
}

