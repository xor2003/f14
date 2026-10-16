// 5140:0836

void __cdecl16far _getch(void)

{
  code *pcVar1;
  
  if ((char)((uint)uRam00052b0a >> 8) == '\0') {
    uRam00052b0a = 0xffff;
  }
  else {
    if (iRam00052f82 == -0x292a) {
      (*pcRam00052f84)();
    }
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
  }
  return;
}

