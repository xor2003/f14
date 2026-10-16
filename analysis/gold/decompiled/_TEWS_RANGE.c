// 4cb5:0312

void __cdecl16far _TEWS_RANGE(int param_1)

{
  iRam00052266 = iRam00052266 + param_1;
  if (2 < iRam00052266) {
    iRam00052266 = 2;
  }
  if (iRam00052266 < 0) {
    iRam00052266 = 0;
  }
  return;
}

