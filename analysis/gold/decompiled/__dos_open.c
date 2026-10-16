// 5140:0d64

void __dos_open(void)

{
  code *pcVar1;
  undefined2 uVar2;
  undefined1 in_CF;
  undefined2 *in_stack_00000008;
  
  pcVar1 = (code *)swi(0x21);
  uVar2 = (*pcVar1)();
  if (!(bool)in_CF) {
    *in_stack_00000008 = uVar2;
  }
  __dosreturn();
  return;
}

