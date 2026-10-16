// 2cac:0064

void __cdecl16far _GetRidOfKeyJoy(void)

{
  if (iRam00050126 != 0) {
    _RemoveKeyJoy();
    iRam00050126 = 0;
  }
  return;
}

