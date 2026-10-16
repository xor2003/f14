// 5140:076c

int __cdecl16far _toupper(int param_1)

{
  if ((*(byte *)(param_1 + 0x5ce3) & 2) != 0) {
    param_1 = param_1 + -0x20;
  }
  return param_1;
}

