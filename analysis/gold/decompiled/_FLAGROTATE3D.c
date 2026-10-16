// 11ac:0008

void __cdecl16far _FLAGROTATE3D(long *param_1)

{
  long lVar1;
  long lVar2;
  uint uVar3;
  long *plVar4;
  undefined2 uVar5;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  plVar4 = (long *)param_1;
  lVar1 = plVar4[2];
  uVar3 = 0;
  lVar2 = *param_1;
  if (lVar1 < lVar2) {
    uVar3 = 2;
  }
  if (-lVar1 != lVar2 && lVar1 <= -lVar2) {
    uVar3 = uVar3 | 1;
  }
  lVar2 = plVar4[1];
  if (lVar1 < lVar2) {
    uVar3 = uVar3 | 8;
  }
  if (-lVar1 != lVar2 && lVar1 <= -lVar2) {
    uVar3 = uVar3 | 4;
  }
  lVar1 = plVar4[2];
  plVar4[2] = lVar1;
  if (lVar1 < lRam0004d036) {
    uVar3 = uVar3 | 0x10;
  }
  *(uint *)((int)plVar4 + 0x1e) = uVar3 | 0x40;
  return;
}

