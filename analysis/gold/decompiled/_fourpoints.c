// 2b89:0017

void __cdecl16far _fourpoints(undefined4 param_1,int param_2,int param_3,undefined1 param_4)

{
  int iVar1;
  undefined2 uVar2;
  
  uVar2 = (undefined2)((ulong)param_1 >> 0x10);
  iVar1 = (int)param_1;
  *(undefined1 *)(iVar1 + param_2 + param_3) = param_4;
  *(undefined1 *)((iVar1 - param_3) + param_2) = param_4;
  *(undefined1 *)((iVar1 - param_2) + param_3) = param_4;
  *(undefined1 *)((iVar1 - param_2) - param_3) = param_4;
  return;
}

