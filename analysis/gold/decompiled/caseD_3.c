// 1504:0000

int __cdecl16far switchD_1000:8a3c::caseD_3(int param_1)

{
  int iVar1;
  int iVar2;
  undefined2 local_3c;
  undefined1 local_3a [8];
  undefined2 local_32;
  undefined2 local_30;
  undefined2 local_2e;
  undefined1 local_2c [14];
  undefined2 local_1e;
  undefined2 local_1c;
  undefined2 local_1a;
  undefined2 local_18;
  undefined2 local_16;
  undefined2 local_14;
  undefined2 local_12;
  undefined2 local_10;
  undefined2 local_e;
  undefined2 local_c;
  undefined2 local_a;
  int local_8;
  int local_6;
  int local_4;
  
  if (iRam0005200e + 1 < 0x29) {
    iVar1 = FUN_1504_0278(param_1);
    if (iVar1 != -1) {
      return iVar1;
    }
    local_4 = _fopen(0x2eb,0x2e8);
    if (local_4 == 0) {
      FUN_2fc6_0009(0x2f7,0);
      _exit(0xffff);
    }
    _fseek(local_4,param_1 * iRam0004d326,param_1 * iRam0004d326 >> 0xf,0);
    if ((*(byte *)(local_4 + 6) & 0x10) == 0) {
      _fread(&local_3c,iRam0004d326,1,local_4);
      iVar1 = FUN_1058_0bf4(local_2c,iRam0004d32c);
      if (iVar1 == 0) {
        _sprintf(0x64ce,0x311,local_3c,local_2c);
        FUN_2fc6_0009(0x64ce,0);
      }
      *(int *)((int)uRam00052002 + iRam0005200e * 0x28 + 0x10) = iRam0004d32c;
      iRam0004d32c = iRam0004d32c + 1;
      *(undefined2 *)(iRam0005200e * 0x28 + (int)uRam00052002) = local_3c;
      FUN_1125_0008(iRam0005200e * 0x28 + (int)uRam00052002 + 2,uRam00052002._2_2_,local_3a);
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 10) = local_32;
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 0xc) = local_30;
      FUN_1504_02a3(local_2e);
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 0xe) = local_2e;
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 0x12) = local_1e;
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 0x14) = local_1c;
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 0x16) = local_1a;
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 0x18) = local_18;
      iVar1 = FUN_1504_040a(local_16);
      *(undefined2 *)((int)uRam00052002 + iRam0005200e * 0x28 + 0x1a) = local_16;
      iVar2 = FUN_1504_040a(local_14);
      *(undefined2 *)(iRam0005200e * 0x28 + (int)uRam00052002 + 0x1c) = local_14;
      local_6 = FUN_1504_040a(local_12);
      *(undefined2 *)(iRam0005200e * 0x28 + (int)uRam00052002 + 0x1e) = local_12;
      *(undefined2 *)(iRam0005200e * 0x28 + (int)uRam00052002 + 0x20) = local_10;
      *(undefined2 *)(iRam0005200e * 0x28 + (int)uRam00052002 + 0x22) = local_e;
      *(undefined2 *)(iRam0005200e * 0x28 + (int)uRam00052002 + 0x24) = local_c;
      *(undefined2 *)(iRam0005200e * 0x28 + (int)uRam00052002 + 0x26) = local_a;
      iRam0005200e = iRam0005200e + 1;
      _fclose(local_4);
      local_8 = iRam0005200e + -1;
      if (iVar1 < 0) {
        caseD_3(-iVar1);
      }
      if (iVar2 < 0) {
        caseD_3(-iVar2);
      }
      if (-1 < local_6) {
        return local_8;
      }
      caseD_3(-local_6);
      return local_8;
    }
  }
  else {
    _sprintf(0x64ce,0x2c0,iRam0005200e,0x28);
    FUN_2fc6_0009(0x64ce,0);
  }
  return -1;
}

