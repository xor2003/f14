// 3060:0cb8

void __cdecl16far _TimeConditionCheck(void)

{
  if (uRam00054e32 < 0x12d) {
    iRam00057e96 = 0;
    uRam00050804 = 5;
    uRam00050806 = 1;
    uRam00052028 = 1;
  }
  if ((299 < uRam00054e32) && (uRam00054e32 < 0x169)) {
    iRam00057e96 = uRam00054e32 - 300;
    uRam00050804 = 0;
    uRam00050806 = 1;
    uRam00052028 = 1;
  }
  if ((0x167 < uRam00054e32) && (uRam00054e32 < 0x1a5)) {
    iRam00057e96 = uRam00054e32 - 0x168;
    uRam00050804 = 1;
    uRam00050806 = 1;
    uRam00052028 = 0;
  }
  if ((0x1a3 < uRam00054e32) && (uRam00054e32 < 0x439)) {
    iRam00057e96 = 0;
    uRam00050804 = 2;
    uRam00050806 = 0;
    uRam00052028 = 0;
  }
  if ((0x437 < uRam00054e32) && (uRam00054e32 < 0x475)) {
    iRam00057e96 = uRam00054e32 - 0x438;
    uRam00050804 = 3;
    uRam00050806 = 0;
    uRam00052028 = 0;
  }
  if ((0x474 < uRam00054e32) && (uRam00054e32 < 0x4b1)) {
    iRam00057e96 = uRam00054e32 - 0x474;
    uRam00050804 = 4;
    uRam00050806 = 1;
    uRam00052028 = 1;
  }
  if ((0x4af < uRam00054e32) && (uRam00054e32 < 0x5a1)) {
    iRam00057e96 = 0;
    uRam00050804 = 5;
    uRam00050806 = 1;
    uRam00052028 = 1;
  }
  return;
}

