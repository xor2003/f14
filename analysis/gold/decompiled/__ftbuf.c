// 5140:13a7

void __cdecl16near __ftbuf(int param_1,undefined2 *param_2)

{
  if (((*(byte *)(param_2 + 0x50) & 0x10) != 0) &&
     ((*(byte *)(*(byte *)((int)param_2 + 7) + 0x5ae9) & 0x40) != 0)) {
    _fflush(param_2);
    if (param_1 != 0) {
      *(byte *)(param_2 + 0x50) = 0;
      param_2[0x51] = 0;
      *param_2 = 0;
      param_2[2] = 0;
    }
  }
  return;
}

