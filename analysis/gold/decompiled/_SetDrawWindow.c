// 170c:0126

void __cdecl16far _SetDrawWindow(int *param_1)

{
  iRam000595c2 = param_1[3] + 1 >> 1;
  iRam000534b8 = param_1[4] + 1 >> 1;
  if ((param_1[2] == 0) && ((bRam00057e84 & 0x40) == 0)) {
    iRam000534b8 = 0x38;
  }
  _SetOvrlyRp_g1(param_1);
  iRam0004ee76 = param_1[1];
  iRam0004ee78 = param_1[2];
  iRam0004ee7a = param_1[1] + param_1[3];
  iRam0004ee7c = param_1[2] + param_1[4];
  iRam0004ee7e = param_1[1] + iRam000595c2;
  iRam0004ee80 = param_1[2] + iRam000534b8;
  uRam0004ee96 = *(undefined2 *)(*param_1 * 2 + 0x1e9e);
  iRam0004d0fa = param_1[1];
  iRam0004d102 = param_1[1] + param_1[3];
  iRam0004d0fe = param_1[2];
  iRam0004d106 = param_1[2] + param_1[4];
  iRam0004d10a = param_1[1] + iRam000595c2;
  iRam0004d10c = param_1[2] + iRam000534b8;
  uRam0005996a = uRam0004ee96;
  uRam00059968 = 0;
  return;
}

