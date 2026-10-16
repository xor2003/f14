// 5140:1e64

void __dos_close_g1(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  __dosret0();
  return;
}

