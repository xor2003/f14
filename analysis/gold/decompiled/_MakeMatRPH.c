// 2e8c:06c6

int __cdecl16far _MakeMatRPH(undefined2 param_1,undefined2 param_2,undefined2 param_3,int *param_4)

{
  long lVar1;
  int iVar2;
  
  iRam00050458 = _SinB(param_3);
  iRam0005045a = _CosB();
  iRam00050460 = _SinB(param_1);
  iRam00050462 = _CosB();
  iRam0005045c = _SinB(param_2);
  iRam0005045e = _CosB();
  lVar1 = (long)(int)((int)((ulong)((long)iRam00050458 * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam00050458 * (long)iRam0005045c) < 0)) *
          (long)iRam00050460;
  *param_4 = ((int)((ulong)((long)iRam0005045a * (long)iRam00050462) >> 0x10) << 1 |
             (uint)((int)((long)iRam0005045a * (long)iRam00050462) < 0)) +
             ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0));
  param_4[1] = (int)((ulong)((long)iRam0005045e * (long)iRam00050460) >> 0x10) << 1 |
               (uint)((int)((long)iRam0005045e * (long)iRam00050460) < 0);
  lVar1 = (long)(int)((int)((ulong)((long)iRam0005045a * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam0005045a * (long)iRam0005045c) < 0)) *
          (long)iRam00050460;
  param_4[2] = ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0)) -
               ((int)((ulong)((long)iRam00050458 * (long)iRam00050462) >> 0x10) << 1 |
               (uint)((int)((long)iRam00050458 * (long)iRam00050462) < 0));
  lVar1 = (long)(int)((int)((ulong)((long)iRam00050458 * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam00050458 * (long)iRam0005045c) < 0)) *
          (long)iRam00050462;
  param_4[3] = ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0)) -
               ((int)((ulong)((long)iRam0005045a * (long)iRam00050460) >> 0x10) << 1 |
               (uint)((int)((long)iRam0005045a * (long)iRam00050460) < 0));
  param_4[4] = (int)((ulong)((long)iRam0005045e * (long)iRam00050462) >> 0x10) << 1 |
               (uint)((int)((long)iRam0005045e * (long)iRam00050462) < 0);
  lVar1 = (long)(int)((int)((ulong)((long)iRam0005045a * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam0005045a * (long)iRam0005045c) < 0)) *
          (long)iRam00050462;
  param_4[5] = ((int)((ulong)((long)iRam00050458 * (long)iRam00050460) >> 0x10) << 1 |
               (uint)((int)((long)iRam00050458 * (long)iRam00050460) < 0)) +
               ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0));
  param_4[6] = (int)((ulong)((long)iRam00050458 * (long)iRam0005045e) >> 0x10) << 1 |
               (uint)((int)((long)iRam00050458 * (long)iRam0005045e) < 0);
  param_4[7] = -iRam0005045c;
  iVar2 = (int)((long)iRam0005045a * (long)iRam0005045e);
  param_4[8] = (int)((ulong)((long)iRam0005045a * (long)iRam0005045e) >> 0x10) << 1 |
               (uint)(iVar2 < 0);
  return iVar2 << 1;
}

