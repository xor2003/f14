// 5140:0cf0

void __dos_allocmem(undefined2 param_1,undefined2 param_2,undefined2 *param_3)

{
  code *pcVar1;
  undefined2 uVar2;
  undefined1 in_CF;
  
  pcVar1 = (code *)swi(0x21);
  uVar2 = (*pcVar1)();
  if (!(bool)in_CF) {
    param_2 = uVar2;
  }
  *param_3 = param_2;
  __dosreturn();
  return;
}

