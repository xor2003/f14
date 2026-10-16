// 5140:01db

void __cdecl16far _exit(void)

{
  code *pcVar1;
  
  FUN_5a3f_0287();
  FUN_5a3f_0287();
  if (iRam00052f82 == -0x292a) {
    (*pcRam00052f88)();
  }
  FUN_5a3f_0287();
  FUN_5a3f_0287();
  __nullcheck();
  __ctermsub();
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  return;
}

