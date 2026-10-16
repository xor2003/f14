// 2520:000e

void __cdecl16far _SET_CTRL_B(void)

{
  code *pcVar1;
  undefined2 uVar2;
  undefined2 in_DX;
  undefined2 in_BX;
  
  uRam0004ee4d = FUN_2852_006a(0x4cff,0x4cff);
  pcVar1 = (code *)swi(0x21);
  uRam0004ee4f = in_BX;
  (*pcVar1)();
  uVar2 = FUN_2852_006a();
  *(undefined2 *)0x1e63 = in_BX;
  *(undefined2 *)0x1e61 = uVar2;
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}

