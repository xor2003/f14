// 5140:025e

void __cdecl16near __ctermsub(undefined2 param_1)

{
  code *pcVar1;
  
  if (iRam00052f94 != 0) {
    (*pcRam00052f92)(0x5140);
  }
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
  if (*(char *)0x5b0a != '\0') {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  return;
}

