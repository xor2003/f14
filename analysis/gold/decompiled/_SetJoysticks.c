// 2cac:0006

void __cdecl16far _SetJoysticks(int param_1)

{
  int iVar1;
  
  iRam00059a0c = param_1;
  DAT_58fc_09a4 = -(uint)(param_1 != 0) & 3;
  iVar1 = param_1 * 2;
  uRam00055988 = *(undefined2 *)(iVar1 + 0x3106);
  uRam0005a102 = *(undefined2 *)(iVar1 + 0x3116);
  uRam00054e06 = uRam00054e06 | *(uint *)(iVar1 + 0x3126);
  if (param_1 == 0) {
    if (iRam00050126 == 0) {
      _InstallKeyJoy();
      iRam00050126 = 1;
    }
  }
  else if (iRam00050126 != 0) {
    _RemoveKeyJoy();
    iRam00050126 = 0;
    return;
  }
  return;
}

