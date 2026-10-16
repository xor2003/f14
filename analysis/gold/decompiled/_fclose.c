// 5140:029e

undefined2 __cdecl16far _fclose(int param_1)

{
  int iVar1;
  undefined2 uVar2;
  char local_10;
  undefined1 uStack_f;
  undefined1 local_e [8];
  int local_6;
  undefined1 *local_4;
  
  uVar2 = 0xffff;
  if (((*(byte *)(param_1 + 6) & 0x40) != 0) || ((*(byte *)(param_1 + 6) & 0x83) == 0))
  goto LAB_5140_034c;
  uVar2 = _fflush(param_1);
  local_6 = *(int *)(param_1 + 0xa4);
  __freebuf(param_1);
  iVar1 = _close(0x5140,*(undefined1 *)(param_1 + 7));
  if (-1 < iVar1) {
    if (local_6 == 0) goto LAB_5140_034c;
    _strcpy(&local_10,0x5b10);
    local_4 = local_e;
    if (local_10 == '\\') {
      local_4 = &uStack_f;
    }
    else {
      _strcat(&local_10,0x5b12);
    }
    _itoa(local_6,local_4,10);
    iVar1 = __dos_close_g1(0x5140,&local_10);
    if (iVar1 == 0) goto LAB_5140_034c;
  }
  uVar2 = 0xffff;
LAB_5140_034c:
  *(undefined1 *)(param_1 + 6) = 0;
  return uVar2;
}

