// 25e4:0004

void __cdecl16far _EMSPRSNT(void)

{
  char *pcVar1;
  char *pcVar2;
  int iVar3;
  char *pcVar4;
  char *pcVar5;
  bool bVar6;
  
  uRam0004f370 = 0;
  bVar6 = true;
  pcVar5 = (char *)0xa;
  pcVar4 = (char *)0x2376;
  iVar3 = 8;
  do {
    if (iVar3 == 0) break;
    iVar3 = iVar3 + -1;
    pcVar2 = pcVar5;
    pcVar5 = pcVar5 + 1;
    pcVar1 = pcVar4;
    pcVar4 = pcVar4 + 1;
    bVar6 = *pcVar1 == *pcVar2;
  } while (bVar6);
  if (!bVar6) {
    uRam0004f370 = 0x90;
  }
  return;
}

