// 1d04:000e

void __cdecl16far _InitCannonRate(void)

{
  iRam0005373c = DAT_5140_0c2a;
  iRam00054de4 = FUN_4be9_004d(DAT_5140_0c2a,3,0x10);
  iRam00057e88 = iRam00054de4 + 4;
  iRam00059a10 = iRam00054de4;
  iRam00055948 = ((6 - iRam00052026) * 0xfa) / iRam0005373c;
  return;
}

