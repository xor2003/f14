// 4b61:0004

void __cdecl16far _InitStealth(void)

{
  uRam000521c6 = 0;
  uRam00051ffe = 1;
  uRam0005607e = 1;
  uRam000521ca = 0xffff;
  uRam000521c8 = 0xffff;
  DAT_5140_0c2a = 4;
  _InitCannonRate();
  uRam000521ba = FUN_4be9_0198(0x1000);
  uRam000521ba = uRam000521ba & 0x7ff8;
  uRam0005368a = 0;
  uRam00054e46 = 0;
  uRam00054e44 = 0;
  FUN_2604_000a();
  FUN_4b61_00ab();
  _Help();
  return;
}

