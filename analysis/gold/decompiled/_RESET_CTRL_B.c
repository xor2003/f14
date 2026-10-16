// 2520:004f

void __cdecl16far _RESET_CTRL_B(void)

{
  code *pcVar1;
  
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)(0x4cff);
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}

