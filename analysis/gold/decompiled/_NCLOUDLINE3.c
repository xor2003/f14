// 11ac:1035

int __cdecl16far
_NCLOUDLINE3(int param_1,int param_2,int param_3,int param_4,undefined2 param_5,undefined2 *param_6,
            undefined2 param_7,undefined2 param_8,uint param_9)

{
  undefined1 uVar1;
  undefined2 uVar2;
  
  param_4 = param_4 * 8;
  param_9 = param_9 >> 3;
  do {
    uVar1 = *(undefined1 *)
             (CONCAT11((char)((uint)param_2 >> 8),(char)((uint)param_1 >> 8)) & 0x7f7f);
    uVar2 = CONCAT11(uVar1,uVar1);
    *param_6 = uVar2;
    param_6[1] = uVar2;
    param_1 = param_1 + param_3 * 8;
    uVar1 = *(undefined1 *)
             (CONCAT11((char)((uint)(param_2 + param_4) >> 8),(char)((uint)param_1 >> 8)) & 0x7f7f);
    uVar2 = CONCAT11(uVar1,uVar1);
    param_6[2] = uVar2;
    param_6[3] = uVar2;
    param_1 = param_1 + param_3 * 8;
    param_2 = param_2 + param_4 + param_4;
    param_6 = param_6 + 4;
    param_9 = param_9 - 1;
  } while (param_9 != 0);
  return param_4;
}

