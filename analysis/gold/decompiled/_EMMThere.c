// 1328:000c

void __cdecl16far _EMMThere(void)

{
  char *pcVar1;
  char *pcVar2;
  code *pcVar3;
  char extraout_AH;
  int iVar4;
  char *pcVar5;
  char *pcVar6;
  undefined2 unaff_ES;
  undefined1 in_ZF;
  
  pcVar3 = (code *)swi(0x21);
  (*pcVar3)();
  pcVar6 = (char *)0xa;
  pcVar5 = (char *)0x288;
  iVar4 = 8;
  do {
    if (iVar4 == 0) break;
    iVar4 = iVar4 + -1;
    pcVar2 = pcVar6;
    pcVar6 = pcVar6 + 1;
    pcVar1 = pcVar5;
    pcVar5 = pcVar5 + 1;
    in_ZF = *pcVar1 == *pcVar2;
  } while ((bool)in_ZF);
  if ((bool)in_ZF) {
    pcVar3 = (code *)swi(0x67);
    (*pcVar3)();
    if (extraout_AH == '\0') {
      uRam0004d276 = 0xffff;
      return;
    }
  }
  uRam0004d276 = 0;
  return;
}

