// 4e5c:0002

int __cdecl16far _TrgMul(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = (int)((long)param_1 * (long)param_2);
  return ((int)((ulong)((long)param_1 * (long)param_2) >> 0x10) << 1 | (uint)(iVar1 < 0)) +
         (uint)(iVar1 << 1 < 0);
}

