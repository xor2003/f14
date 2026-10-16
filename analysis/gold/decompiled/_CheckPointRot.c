// 11ac:12dc

long * __cdecl16far _CheckPointRot(uint param_1)

{
  int iVar1;
  uint uVar2;
  undefined4 *puVar3;
  int *piVar4;
  long *plVar5;
  long *plVar6;
  undefined2 uVar7;
  undefined2 uVar8;
  
  iVar1 = iRam000539ae;
  uVar7 = (undefined2)((ulong)*(undefined4 *)(iRam000539ae + 0xc) >> 0x10);
  plVar6 = (long *)((int)*(undefined4 *)(iRam000539ae + 0xc) + (param_1 & 0xff) * 0x20);
  if ((*(uint *)((int)plVar6 + 0x1e) & 0x40) == 0) {
    uVar8 = (undefined2)((ulong)*(undefined4 *)(iRam000539ae + 8) >> 0x10);
    puVar3 = (undefined4 *)((int)*(undefined4 *)(iRam000539ae + 8) + (param_1 & 0xff) * 8);
    uRam0004d15e = *puVar3;
    iRam0004d162 = *(int *)(puVar3 + 1);
    uVar2 = *(uint *)((int)puVar3 + 6);
    if ((uVar2 & 0xfff0) != 0) {
      piVar4 = (int *)(*(int *)(iRam000539ae + 0x54) + (uVar2 >> 3 & 0xfffe));
      uRam0004d15e._2_2_ = (int)((ulong)uRam0004d15e >> 0x10);
      uRam0004d15e = CONCAT22(uRam0004d15e._2_2_ + piVar4[1],(int)uRam0004d15e + *piVar4);
      iRam0004d162 = iRam0004d162 + piVar4[2];
      uVar2 = uVar2 & 0xf;
    }
    _Mult1X3(0x16e,uVar2 * 0x24 + *(int *)(iRam000539ae + 0x52),0x174);
    plVar5 = (long *)(iVar1 + 0x16 + uVar2 * 0xc);
    *plVar6 = *plVar5 + lRam0004d164;
    plVar6[1] = plVar5[1] + lRam0004d168;
    plVar6[2] = plVar5[2] + lRam0004d16c;
    _FLAGROTATE3D(plVar6,uVar7);
    return plVar6;
  }
  return plVar6;
}

