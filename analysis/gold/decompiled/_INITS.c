// 2c92:00a2

void __cdecl16near _INITS(void)

{
  undefined2 uVar1;
  int in_BX;
  int iVar2;
  
  iVar2 = in_BX * 2;
  uVar1 = *(undefined2 *)(iVar2 + 0x30cc);
  *(undefined2 *)(iVar2 + 0x30b4) = uVar1;
  *(undefined2 *)(iVar2 + 0x30a4) = uVar1;
  *(undefined2 *)(iVar2 + 0x30ac) = uVar1;
  return;
}

