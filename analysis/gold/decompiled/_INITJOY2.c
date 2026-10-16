// 2c92:007a

void __cdecl16far _INITJOY2(byte param_1)

{
  uint uVar1;
  bool bVar2;
  
  _COLOSPECSETUP();
  _JOYREAD(0x4cff);
  uVar1 = 0;
  do {
    bVar2 = (bool)(param_1 & 1);
    param_1 = param_1 >> 1;
    if (bVar2) {
      _INITS();
    }
    uVar1 = uVar1 + 1;
  } while (uVar1 < 4);
  return;
}

