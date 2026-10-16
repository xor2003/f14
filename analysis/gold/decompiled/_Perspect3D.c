// 11ac:006f

void __cdecl16far _Perspect3D(long *param_1)

{
  long lVar1;
  uint uVar2;
  long lVar3;
  long *plVar4;
  undefined2 uVar5;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  plVar4 = (long *)param_1;
  uVar2 = *(uint *)((int)plVar4 + 0x1e) & 0xff;
  if (lRam0004d036 <= plVar4[2]) {
    lVar3 = plVar4[2] >> 8;
    lVar1 = *param_1 / lVar3 + (ulong)uRam0004d10a;
    plVar4[3] = lVar1;
    if (lVar1 < lRam0004d0fa) {
      uVar2 = uVar2 | 0x100;
    }
    if (lRam0004d102 < lVar1) {
      uVar2 = uVar2 | 0x200;
    }
    lVar1 = plVar4[1] / lVar3 + (ulong)uRam0004d10c;
    plVar4[4] = lVar1;
    if (lVar1 < lRam0004d0fe) {
      uVar2 = uVar2 | 0x400;
    }
    if (lRam0004d106 < lVar1) {
      uVar2 = uVar2 | 0x800;
    }
    uVar2 = uVar2 | 0x1000;
  }
  *(uint *)((int)plVar4 + 0x1e) = uVar2;
  return;
}

