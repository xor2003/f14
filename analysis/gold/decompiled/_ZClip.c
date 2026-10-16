// 11ac:0101

void __cdecl16far _ZClip(long *param_1,long *param_2,long *param_3,uint param_4)

{
  long lVar1;
  long *plVar2;
  long *plVar3;
  long *plVar4;
  undefined2 uVar5;
  undefined2 uVar6;
  undefined2 uVar7;
  
  uVar5 = (undefined2)((ulong)param_1 >> 0x10);
  plVar2 = (long *)param_1;
  uVar6 = (undefined2)((ulong)param_2 >> 0x10);
  plVar3 = (long *)param_2;
  uVar7 = (undefined2)((ulong)param_3 >> 0x10);
  plVar4 = (long *)param_3;
  lRam0004d140 = plVar3[2] - plVar2[2];
  lVar1 = lRam0004d036 - plVar2[2];
  *param_3 = (long)(((sqword)(*param_2 - *param_1) * (sqword)lVar1) / (sqword)lRam0004d140) +
             *param_1;
  plVar4[1] = (long)(((sqword)(plVar3[1] - plVar2[1]) * (sqword)lVar1) / (sqword)lRam0004d140) +
              plVar2[1];
  plVar4[2] = lRam0004d036;
  param_4 = param_4 & 3;
  if ((param_4 != 0) && (param_4 != 3)) {
    if (param_4 != 1) {
      *(int *)((int)plVar4 + 0x1a) =
           (int)(((sqword)(long)(*(int *)((int)plVar3 + 0x1a) - *(int *)((int)plVar2 + 0x1a)) *
                 (sqword)lVar1) / (sqword)lRam0004d140) + *(int *)((int)plVar2 + 0x1a);
    }
    *(int *)(plVar4 + 6) =
         (int)(((sqword)(long)((int)plVar3[6] - (int)plVar2[6]) * (sqword)lVar1) /
              (sqword)lRam0004d140) + (int)plVar2[6];
  }
  _FLAGROTATE3D(plVar4,uVar7);
  return;
}

