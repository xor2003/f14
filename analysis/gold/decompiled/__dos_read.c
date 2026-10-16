// 5140:0d83

void __dos_read(void)

{
  code *pcVar1;
  undefined2 uVar2;
  undefined1 uVar3;
  undefined2 in_stack_00000000;
  undefined2 *in_stack_0000000c;
  
  uVar3 = uRam00052f82 < 0xd6d6;
  if (uRam00052f82 == 0xd6d6) {
    (*pcRam00052f84)();
  }
  pcVar1 = (code *)swi(0x21);
  uVar2 = (*pcVar1)();
  if (!(bool)uVar3) {
    *in_stack_0000000c = uVar2;
  }
  __dosreturn();
  return;
}

