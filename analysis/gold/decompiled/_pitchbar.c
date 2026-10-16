// 2b89:0128

void _pitchbar(undefined2 param_1,int param_2,undefined2 param_3,int param_4,int param_5)

{
  int local_a;
  int local_8;
  int local_6;
  int local_4;
  
  FUN_4be9_0000(param_2,param_3,uRam0004f46e,&local_8,&local_4);
  local_8 = (local_8 * 0x16) / 0x1e;
  local_4 = (local_4 * 0x16) / 0x1e;
  local_4 = FUN_2b89_0008(local_4);
  FUN_4be9_0000(-param_2,param_3,uRam0004f46e,&local_a,&local_6);
  local_a = (local_a * 0x16) / 0x1e;
  local_6 = (local_6 * 0x16) / 0x1e;
  local_6 = FUN_2b89_0008(local_6);
                    /* WARNING: Subroutine does not return */
  FUN_15ce_0bdc(uRam00055984,local_8 + param_4,local_4 + param_5,local_a + param_4,local_6 + param_5
                ,0xf);
}

