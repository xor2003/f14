// 5140:11dc

void __cdecl16near __freebuf(undefined2 *param_1)

{
  if (((*(byte *)(param_1 + 3) & 0x83) != 0) && ((*(byte *)(param_1 + 3) & 8) != 0)) {
    __nfree(param_1[2]);
    *(byte *)(param_1 + 3) = *(byte *)(param_1 + 3) & 0xf7;
    param_1[2] = 0;
    *param_1 = 0;
    param_1[1] = 0;
  }
  return;
}

