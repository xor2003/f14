// 2b89:01c9

void __cdecl16far _pitchnum(int param_1,int param_2,int param_3,int param_4,int param_5,int param_6)

{
  int iVar1;
  int local_6;
  int local_4;
  
  local_4 = FUN_28a7_019e((param_2 - param_4) - param_3);
  local_4 = local_4 / 10;
  if (8 < local_4) {
    local_4 = -(local_4 % 8 + -9);
  }
  _itoa(local_4,0x64ce,10);
  FUN_4be9_0000(param_1,param_4 + param_3,uRam0004f46e,&local_4,&local_6);
  local_4 = (local_4 * 0x16) / 0x1e;
  local_6 = (local_6 * 0x16) / 0x1e;
  iVar1 = FUN_2b89_0008(local_6,0x305e);
  thunk_EXT_FUN_0000_0000(0x4be9,uRam00055984,local_4 + param_5 + -1,iVar1 + param_6 + -1);
  FUN_4be9_0000(-param_1,param_4 + param_3,uRam0004f46e,&local_4,&local_6);
  local_4 = (local_4 * 0x16) / 0x1e;
  local_6 = (local_6 * 0x16) / 0x1e;
  if (__searchseg == '1') {
    local_4 = local_4 + 2;
  }
  iVar1 = FUN_2b89_0008(local_6,0x64ce);
  thunk_EXT_FUN_0000_0000(0x4be9,uRam00055984,local_4 + param_5 + -1,iVar1 + param_6 + -1);
  return;
}

