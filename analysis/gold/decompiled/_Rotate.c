// 2e8c:0217

undefined2 __cdecl16far _Rotate(int *param_1,int *param_2)

{
  int iVar1;
  int iVar2;
  int iVar3;
  
  iVar1 = *param_1;
  iVar2 = param_1[1];
  iVar3 = param_1[2];
  *param_1 = (int)((ulong)((long)iVar1 * (long)*param_2) >> 0x10);
  param_1[1] = (int)((ulong)((long)iVar1 * (long)param_2[1]) >> 0x10);
  param_1[2] = (int)((ulong)((long)iVar1 * (long)param_2[2]) >> 0x10);
  *param_1 = ((int)((ulong)((long)iVar2 * (long)param_2[3]) >> 0x10) +
              (int)((ulong)((long)iVar3 * (long)param_2[6]) >> 0x10) + *param_1) * 2;
  param_1[1] = ((int)((ulong)((long)iVar2 * (long)param_2[4]) >> 0x10) +
                (int)((ulong)((long)iVar3 * (long)param_2[7]) >> 0x10) + param_1[1]) * 2;
  iVar1 = param_2[8];
  param_1[2] = ((int)((ulong)((long)iVar2 * (long)param_2[5]) >> 0x10) +
                (int)((ulong)((long)iVar3 * (long)iVar1) >> 0x10) + param_1[2]) * 2;
  return (int)((long)iVar3 * (long)iVar1);
}

