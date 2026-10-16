// 206a:0108

void __cdecl16far _PlaneLaunched(int param_1)

{
  int iVar1;
  
  iVar1 = FUN_1dc3_184d(uRam00059560,uRam00059562,uRam00059564,uRam00059566,uRam00059568,
                        uRam0005956a);
  if (iVar1 != 0) {
    FUN_4a6f_00b5(0x24,2,0);
  }
  *(undefined2 *)(param_1 * 2 + 0x1838) = 0;
  *(undefined2 *)(param_1 * 2 + 0x1830) = 0xfc19;
  if (param_1 < 2) {
    uRam00055958 = uRam00055958 & ~(0x10 << ((byte)param_1 & 0x1f));
    FUN_3fb2_5f44();
  }
  return;
}

