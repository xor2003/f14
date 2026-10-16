// 4d41:03ad

void __cdecl16far _GunCameraZoom(int param_1)

{
  iRam000522ac = iRam000522ac + param_1;
  if (iRam000522ac < 1) {
    iRam000522ac = 1;
  }
  if (0x14 < iRam000522ac) {
    iRam000522ac = 0x14;
  }
  return;
}

