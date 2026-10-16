// 172e:0d90

void __cdecl16far _InitOurCoolPlane(int param_1)

{
  bool bVar1;
  
  bVar1 = param_1 == 0;
  if (bVar1) {
    uRam00057e4a = 0;
    uRam00057e48 = 0;
  }
  else {
    uRam00057e4a = 0x4000;
    uRam00057e48 = 0x4000;
  }
  uRam00057e2e = 0;
  uRam00057e2c = (uint)bVar1;
  uRam00057e2a = (uint)bVar1;
  uRam00057e28 = 0;
  uRam00057e44 = 0x4000;
  uRam00057e46 = 0;
  return;
}

