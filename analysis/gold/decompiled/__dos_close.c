// 5140:0d2c

void __dos_close(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  __dosreturn();
  return;
}

