// 11ac:1550

void __cdecl16far
_MYWATERLINE(int param_1,int param_2,int param_3,int param_4,undefined2 param_5,undefined2 *param_6,
            undefined2 param_7,int param_8,uint param_9)

{
  uint uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  param_3 = param_3 * 2;
  param_4 = param_4 * 2;
  uVar1 = param_9 >> 2;
  uRam0004d172 = uVar1;
  if (param_8 == 0) {
    uRam0004d172 = param_9 >> 3;
    do {
      *param_6 = CONCAT11(*(undefined1 *)
                           (CONCAT11((char)((uint)(param_2 + param_4) >> 8),
                                     (char)((uint)(param_1 + param_3) >> 8)) & 0x7f7f),
                          *(undefined1 *)
                           (CONCAT11((char)((uint)param_2 >> 8),(char)((uint)param_1 >> 8)) & 0x7f7f
                           ));
      iVar2 = param_1 + param_3 + param_3;
      iVar4 = param_2 + param_4 + param_4;
      iVar3 = iVar2 + param_3;
      iVar5 = iVar4 + param_4;
      param_6[1] = CONCAT11(*(undefined1 *)
                             (CONCAT11((char)((uint)iVar5 >> 8),(char)((uint)iVar3 >> 8)) & 0x7f7f),
                            *(undefined1 *)
                             (CONCAT11((char)((uint)iVar4 >> 8),(char)((uint)iVar2 >> 8)) & 0x7f7f))
      ;
      iVar3 = iVar3 + param_3;
      iVar5 = iVar5 + param_4;
      iVar2 = iVar3 + param_3;
      iVar4 = iVar5 + param_4;
      param_6[2] = CONCAT11(*(undefined1 *)
                             (CONCAT11((char)((uint)iVar4 >> 8),(char)((uint)iVar2 >> 8)) & 0x7f7f),
                            *(undefined1 *)
                             (CONCAT11((char)((uint)iVar5 >> 8),(char)((uint)iVar3 >> 8)) & 0x7f7f))
      ;
      iVar2 = iVar2 + param_3;
      iVar4 = iVar4 + param_4;
      param_1 = iVar2 + param_3;
      param_2 = iVar4 + param_4;
      param_6[3] = CONCAT11(*(undefined1 *)
                             (CONCAT11((char)((uint)param_2 >> 8),(char)((uint)param_1 >> 8)) &
                             0x7f7f),*(undefined1 *)
                                      (CONCAT11((char)((uint)iVar4 >> 8),(char)((uint)iVar2 >> 8)) &
                                      0x7f7f));
      param_1 = param_1 + param_3;
      param_2 = param_2 + param_4;
      param_6 = param_6 + 4;
      uRam0004d172 = uRam0004d172 - 1;
    } while (uRam0004d172 != 0);
    return;
  }
  do {
    *(undefined1 *)param_6 =
         *(undefined1 *)
          (uint)*(byte *)(CONCAT11((char)((uint)param_2 >> 8),(char)((uint)param_1 >> 8)) & 0x7f7f);
    *(undefined1 *)((int)param_6 + 1) =
         *(undefined1 *)
          (uint)*(byte *)(CONCAT11((char)((uint)(param_2 + param_4) >> 8),
                                   (char)((uint)(param_1 + param_3) >> 8)) & 0x7f7f);
    param_1 = param_1 + param_3 + param_3;
    param_2 = param_2 + param_4 + param_4;
    *(undefined1 *)(param_6 + 1) =
         *(undefined1 *)
          (uint)*(byte *)(CONCAT11((char)((uint)param_2 >> 8),(char)((uint)param_1 >> 8)) & 0x7f7f);
    param_1 = param_1 + param_3;
    param_2 = param_2 + param_4;
    *(undefined1 *)((int)param_6 + 3) =
         *(undefined1 *)
          (uint)*(byte *)(CONCAT11((char)((uint)param_2 >> 8),(char)((uint)param_1 >> 8)) & 0x7f7f);
    param_1 = param_1 + param_3;
    param_2 = param_2 + param_4;
    param_6 = param_6 + 2;
    uVar1 = uVar1 - 1;
  } while (uVar1 != 0);
  return;
}

