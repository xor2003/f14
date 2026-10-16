// 2591:0052

void __cdecl16far _VGADRAWDOT(int param_1,int param_2,undefined2 param_3)

{
  if ((((iRam0004ee76 <= param_1) && (param_1 <= iRam0004ee7a)) && (iRam0004ee78 <= param_2)) &&
     (param_2 <= iRam0004ee7c)) {
    if ((char)((uint)param_3 >> 8) != -5) {
      *(undefined1 *)(*(int *)(param_2 * 2 + 0x1ea8) + param_1) = (char)param_3;
    }
  }
  return;
}

