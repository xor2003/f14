// 2c92:003c

byte __cdecl16far _TRIGGER2(void)

{
  code *pcVar1;
  byte bVar2;
  undefined2 in_CX;
  undefined1 *unaff_SI;
  
  if (iRam000500c9 == -1) {
    _COLOSPECSETUP();
  }
  if (iRam000500c9 == 1) {
    pcVar1 = (code *)swi(0x15);
    bVar2 = (*pcVar1)(in_CX);
    register0x00000010 = (BADSPACEBASE *)unaff_SI;
  }
  else {
    bVar2 = in(0x201);
  }
  return (byte)~bVar2 >> ((char)*(undefined2 *)((int)register0x00000010 + 4) + 4U & 0x1f) & 1;
}

