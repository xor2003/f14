// 2c92:00b7

undefined2 __cdecl16far _ReadJoy2(byte param_1)

{
  uint uVar1;
  bool bVar2;
  
  _JOYREAD(0x4cff);
  uVar1 = 0;
  do {
    bVar2 = (bool)(param_1 & 1);
    param_1 = param_1 >> 1;
    if (bVar2) {
      _JOYCONVERT();
    }
    uVar1 = uVar1 + 1;
  } while (uVar1 < 4);
  return uRam00068fe4;
}

