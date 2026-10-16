// 2a48:00e6

undefined2 __cdecl16far _vel_vector(void)

{
  undefined2 in_AX;
  
  if (iRam0004dd6e == 0) {
    iRam00053170 = (iRam00053658 >> 6) + 0x23;
    if (iRam0004ec6a != 0) {
      return 0;
    }
    in_AX = FUN_15ce_0369(0x2a48,uRam0004ff54,0x33,(iRam00053658 >> 6) + 0x1f,uRam00054e52);
  }
  return in_AX;
}

