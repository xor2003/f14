// 2e8c:0460

int __cdecl16far _MakematHPR(undefined2 param_1,undefined2 param_2,undefined2 param_3,int *param_4)

{
  long lVar1;
  int iVar2;
  
  iRam00050458 = _SinB(param_1);
  iRam0005045a = _CosB();
  iRam00050460 = _SinB(param_3);
  iRam00050462 = _CosB();
  iRam0005045c = _SinB(param_2);
  iRam0005045e = _CosB();
  lVar1 = (long)(int)((int)((ulong)((long)iRam00050460 * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam00050460 * (long)iRam0005045c) < 0)) *
          (long)iRam00050458;
  *param_4 = ((int)((ulong)((long)iRam00050462 * (long)iRam0005045a) >> 0x10) << 1 |
             (uint)((int)((long)iRam00050462 * (long)iRam0005045a) < 0)) -
             ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0));
  lVar1 = (long)(int)((int)((ulong)((long)iRam00050462 * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam00050462 * (long)iRam0005045c) < 0)) *
          (long)iRam00050458;
  param_4[1] = ((int)((ulong)((long)iRam00050460 * (long)iRam0005045a) >> 0x10) << 1 |
               (uint)((int)((long)iRam00050460 * (long)iRam0005045a) < 0)) +
               ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0));
  param_4[2] = -((int)((ulong)((long)iRam0005045e * (long)iRam00050458) >> 0x10) << 1 |
                (uint)((int)((long)iRam0005045e * (long)iRam00050458) < 0));
  param_4[3] = -((int)((ulong)((long)iRam00050460 * (long)iRam0005045e) >> 0x10) << 1 |
                (uint)((int)((long)iRam00050460 * (long)iRam0005045e) < 0));
  param_4[4] = (int)((ulong)((long)iRam00050462 * (long)iRam0005045e) >> 0x10) << 1 |
               (uint)((int)((long)iRam00050462 * (long)iRam0005045e) < 0);
  param_4[5] = iRam0005045c;
  lVar1 = (long)(int)((int)((ulong)((long)iRam00050460 * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam00050460 * (long)iRam0005045c) < 0)) *
          (long)iRam0005045a;
  param_4[6] = ((int)((ulong)((long)iRam00050462 * (long)iRam00050458) >> 0x10) << 1 |
               (uint)((int)((long)iRam00050462 * (long)iRam00050458) < 0)) +
               ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0));
  lVar1 = (long)(int)((int)((ulong)((long)iRam00050462 * (long)iRam0005045c) >> 0x10) << 1 |
                     (uint)((int)((long)iRam00050462 * (long)iRam0005045c) < 0)) *
          (long)iRam0005045a;
  param_4[7] = ((int)((ulong)((long)iRam00050460 * (long)iRam00050458) >> 0x10) << 1 |
               (uint)((int)((long)iRam00050460 * (long)iRam00050458) < 0)) -
               ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)((int)lVar1 < 0));
  iVar2 = (int)((long)iRam0005045e * (long)iRam0005045a);
  param_4[8] = (int)((ulong)((long)iRam0005045e * (long)iRam0005045a) >> 0x10) << 1 |
               (uint)(iVar2 < 0);
  return iVar2 << 1;
}

