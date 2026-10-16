// 4be9:00e5

void __cdecl16far _uangle(int param_1,uint param_2)

{
  if (0x7fff < param_2) {
    param_2 = param_2 >> 1;
    param_1 = param_1 >> 1;
  }
  _ARCTAN(param_1,param_2);
  return;
}

