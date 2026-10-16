// 18de:00fe

undefined2 __cdecl16far _InitMode(void)

{
  _MakematHPR(-iRam0004f46a,uRam0004f46c,0,0xca2a);
  _memcpy(0x7b7c,0x2420,0x12);
  uRam00057e8c = FUN_189f_0188();
  if (iRam00054e4c != 0) {
    iRam00054e4c = iRam00054e4c + -1;
  }
  if (iRam0005954e != 0) {
    iRam0005954e = iRam0005954e + -1;
  }
  if (iRam00053692 != -1) {
    if ((iRam0004dffc != 200) && (iRam0004dffc != 0xca)) {
      return uRam00057e8c;
    }
    FUN_18de_116f();
  }
  return 0;
}

