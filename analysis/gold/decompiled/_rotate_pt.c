// 1c2f:000a

int __cdecl16far _rotate_pt(int *param_1,int *param_2,int param_3)

{
  int iVar1;
  long lVar2;
  long lVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar6 = *param_1;
  iVar1 = param_1[1];
  iVar4 = _CosB(-param_3);
  iVar5 = _SinB();
  lVar2 = (long)iVar5 * (long)iVar1;
  lVar3 = (long)iVar4 * (long)iVar6;
  *param_2 = ((int)((ulong)lVar3 >> 0x10) << 1 | (uint)((int)lVar3 < 0)) -
             ((int)((ulong)lVar2 >> 0x10) << 1 | (uint)((int)lVar2 < 0));
  lVar2 = (long)iVar4 * (long)iVar1;
  lVar3 = (long)iVar5 * (long)iVar6;
  iVar6 = (int)lVar3;
  param_2[1] = ((int)((ulong)lVar3 >> 0x10) << 1 | (uint)(iVar6 < 0)) +
               ((int)((ulong)lVar2 >> 0x10) << 1 | (uint)((int)lVar2 < 0));
  return iVar6 << 1;
}

