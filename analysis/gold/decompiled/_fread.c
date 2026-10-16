// 5140:039a

uint __cdecl16far _fread(undefined1 *param_1,uint param_2,uint param_3,int *param_4)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  uint uVar4;
  uint uVar5;
  uint local_6;
  
  uVar1 = param_2 * param_3;
  if (uVar1 == 0) {
    param_3 = 0;
  }
  else {
    uVar5 = uVar1;
    if (((*(byte *)(param_4 + 3) & 0xc) == 0) && ((*(byte *)(param_4 + 0x50) & 1) == 0)) {
      local_6 = 0x200;
    }
    else {
      local_6 = param_4[0x51];
    }
    do {
      if ((((*(byte *)(param_4 + 3) & 0xc) == 0) && ((*(byte *)(param_4 + 0x50) & 1) == 0)) ||
         (uVar2 = param_4[1], uVar2 == 0)) {
        if (uVar5 < local_6) {
          uVar4 = uVar5;
          iVar3 = __filbuf(param_4);
          if (iVar3 == -1) break;
          *param_1 = (char)iVar3;
          param_1 = param_1 + 1;
          uVar4 = uVar4 - 1;
          local_6 = param_4[0x51];
        }
        else {
          uVar4 = uVar5 - uVar5 % local_6;
          iVar3 = _read(0x5140,*(undefined1 *)((int)param_4 + 7),param_1,uVar4,uVar5,param_1);
          if (iVar3 == 0) {
            *(byte *)(param_4 + 3) = *(byte *)(param_4 + 3) | 0x10;
            break;
          }
          if (iVar3 == -1) {
            *(byte *)(param_4 + 3) = *(byte *)(param_4 + 3) | 0x20;
            break;
          }
          uVar4 = uVar4 - iVar3;
          param_1 = (undefined1 *)(uVar5 + iVar3);
        }
      }
      else {
        if (uVar5 < uVar2) {
          uVar2 = uVar5;
        }
        _memcpy(param_1,*param_4,uVar2);
        uVar4 = uVar5 - uVar2;
        param_4[1] = param_4[1] - uVar2;
        param_1 = param_1 + uVar2;
        *param_4 = *param_4 + uVar2;
      }
      uVar5 = uVar4;
    } while (uVar4 != 0);
    if (uVar4 != 0) {
      param_3 = (uVar1 - uVar4) / param_2;
    }
  }
  return param_3;
}

