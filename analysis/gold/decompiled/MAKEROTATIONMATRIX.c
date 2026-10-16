// 15ce:0022

int __cdecl16far MAKEROTATIONMATRIX(void)

{
  long lVar1;
  int iVar2;
  int iVar3;
  int in_CX;
  uint uVar4;
  uint uVar5;
  int unaff_BP;
  int unaff_SI;
  int *unaff_DI;
  
  iVar2 = in_CX;
  FUN_4e5c_0071();
  iRam0004db6e = iVar2;
  FUN_4e5c_0075();
  iVar2 = unaff_BP;
  iRam0004db70 = in_CX;
  FUN_4e5c_0071();
  iRam0004db76 = iVar2;
  FUN_4e5c_0075();
  iVar2 = unaff_SI;
  iRam0004db78 = unaff_BP;
  FUN_4e5c_0071();
  iRam0004db72 = iVar2;
  FUN_4e5c_0075();
  uVar4 = (int)((ulong)((long)iRam0004db72 * (long)iRam0004db76) >> 0x10) << 1 |
          (uint)((int)((long)iRam0004db72 * (long)iRam0004db76) < 0);
  lVar1 = (long)(int)uVar4 * (long)iRam0004db6e;
  iVar2 = (int)lVar1;
  iVar3 = (int)((long)iRam0004db70 * (long)iRam0004db78);
  iRam0004db74 = unaff_SI;
  *unaff_DI = ((int)((ulong)((long)iRam0004db70 * (long)iRam0004db78) >> 0x10) << 1 |
              (uint)(iVar3 < 0)) + ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)(iVar2 < 0)) +
              (uint)CARRY2(iVar3 << 1,iVar2 << 1);
  uVar5 = (int)((ulong)((long)iRam0004db72 * (long)iRam0004db78) >> 0x10) << 1 |
          (uint)((int)((long)iRam0004db72 * (long)iRam0004db78) < 0);
  lVar1 = (long)(int)uVar5 * (long)iRam0004db6e;
  iVar2 = (int)lVar1;
  iVar3 = (int)((long)iRam0004db70 * (long)iRam0004db76);
  unaff_DI[1] = (((int)((ulong)lVar1 >> 0x10) << 1 | (uint)(iVar2 < 0)) -
                ((int)((ulong)((long)iRam0004db70 * (long)iRam0004db76) >> 0x10) << 1 |
                (uint)(iVar3 < 0))) - (uint)((uint)(iVar2 << 1) < (uint)(iVar3 << 1));
  unaff_DI[2] = (int)((ulong)((long)iRam0004db6e * (long)iRam0004db74) >> 0x10) << 1 |
                (uint)((int)((long)iRam0004db6e * (long)iRam0004db74) < 0);
  unaff_DI[3] = (int)((ulong)((long)iRam0004db76 * (long)iRam0004db74) >> 0x10) << 1 |
                (uint)((int)((long)iRam0004db76 * (long)iRam0004db74) < 0);
  unaff_DI[4] = (int)((ulong)((long)iRam0004db78 * (long)iRam0004db74) >> 0x10) << 1 |
                (uint)((int)((long)iRam0004db78 * (long)iRam0004db74) < 0);
  unaff_DI[5] = -iRam0004db72;
  lVar1 = (long)(int)uVar4 * (long)iRam0004db70;
  iVar2 = (int)lVar1;
  iVar3 = (int)((long)iRam0004db6e * (long)iRam0004db78);
  unaff_DI[6] = (((int)((ulong)lVar1 >> 0x10) << 1 | (uint)(iVar2 < 0)) -
                ((int)((ulong)((long)iRam0004db6e * (long)iRam0004db78) >> 0x10) << 1 |
                (uint)(iVar3 < 0))) - (uint)((uint)(iVar2 << 1) < (uint)(iVar3 << 1));
  lVar1 = (long)(int)uVar5 * (long)iRam0004db70;
  iVar2 = (int)lVar1;
  iVar3 = (int)((long)iRam0004db6e * (long)iRam0004db76);
  unaff_DI[7] = ((int)((ulong)((long)iRam0004db6e * (long)iRam0004db76) >> 0x10) << 1 |
                (uint)(iVar3 < 0)) + ((int)((ulong)lVar1 >> 0x10) << 1 | (uint)(iVar2 < 0)) +
                (uint)CARRY2(iVar3 << 1,iVar2 << 1);
  iVar2 = (int)((long)iRam0004db70 * (long)iRam0004db74);
  unaff_DI[8] = (int)((ulong)((long)iRam0004db70 * (long)iRam0004db74) >> 0x10) << 1 |
                (uint)(iVar2 < 0);
  return iVar2 << 1;
}

