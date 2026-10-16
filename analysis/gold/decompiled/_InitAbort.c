// 28d5:0105

void __cdecl16far _InitAbort(void)

{
  FUN_1504_079f();
  FUN_15ce_0d2d();
  _RESET_CTRL_B();
  FUN_14c7_0381();
  FUN_4a61_00b6();
  thunk_EXT_FUN_0000_0000(0x4a61);
  FUN_28d5_01a5();
  _exit(uRam0004f81d);
  return;
}

