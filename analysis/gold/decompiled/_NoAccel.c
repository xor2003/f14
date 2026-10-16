// 2081:187c

void __cdecl16far _NoAccel(void)

{
  if (iRam000521ce != 1) {
    DAT_5140_0c2a = DAT_5140_0c2a / iRam000521ce;
    if (DAT_5140_0c2a < 3) {
      DAT_5140_0c2a = 3;
    }
    iRam000521ce = 1;
  }
  return;
}

