// 5140:0810

uint __cdecl16far _kbhit(void)

{
  code *pcVar1;
  byte bVar2;
  char cVar4;
  uint uVar3;
  
  cVar4 = (char)((uint)uRam00052b0a >> 8);
  uVar3 = CONCAT11(cVar4,0xff);
  if (cVar4 != '\0') {
    if (iRam00052f82 == -0x292a) {
      (*pcRam00052f84)();
    }
    pcVar1 = (code *)swi(0x21);
    bVar2 = (*pcVar1)();
    uVar3 = (uint)bVar2;
  }
  return uVar3;
}

