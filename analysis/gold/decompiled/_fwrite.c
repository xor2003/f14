// 5140:047e

uint __cdecl16far _fwrite(undefined1 *param_1,uint param_2,uint param_3,int *param_4)

{
  uint uVar1;
  uint uVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  byte *pbVar6;
  uint local_6;
  
  uVar1 = param_2 * param_3;
  if (uVar1 == 0) {
    param_3 = 0;
  }
  else {
    pbVar6 = (byte *)(param_4 + 0x50);
    uVar5 = uVar1;
    if (((*(byte *)(param_4 + 3) & 0xc) == 0) && ((*pbVar6 & 1) == 0)) {
      local_6 = 0x200;
    }
    else {
      local_6 = param_4[0x51];
    }
    do {
      if ((((*(byte *)(param_4 + 3) & 8) == 0) && ((*pbVar6 & 1) == 0)) ||
         (uVar2 = param_4[1], uVar2 == 0)) {
        if (uVar5 < local_6) {
          iVar3 = __flsbuf(*param_1,param_4);
          if (iVar3 == -1) break;
          param_1 = param_1 + 1;
          uVar5 = uVar5 - 1;
          local_6 = param_4[0x51];
          if (local_6 == 0) {
            local_6 = 1;
          }
        }
        else {
          if ((((*(byte *)(param_4 + 3) & 8) != 0) || ((*pbVar6 & 1) != 0)) &&
             (iVar3 = _fflush(param_4), iVar3 != 0)) break;
          iVar3 = uVar5 - uVar5 % local_6;
          iVar4 = _write(0x5140,*(undefined1 *)((int)param_4 + 7),param_1,iVar3);
          if ((iVar4 == -1) || (uVar5 = uVar5 - iVar4, iVar4 != iVar3)) {
            *(byte *)(param_4 + 3) = *(byte *)(param_4 + 3) | 0x20;
            break;
          }
          param_1 = param_1 + iVar4;
        }
      }
      else {
        if (uVar5 < uVar2) {
          uVar2 = uVar5;
        }
        _memcpy(*param_4,param_1,uVar2);
        uVar5 = uVar5 - uVar2;
        param_4[1] = param_4[1] - uVar2;
        param_1 = param_1 + uVar2;
        *param_4 = *param_4 + uVar2;
      }
    } while (uVar5 != 0);
    if (uVar5 != 0) {
      param_3 = (uVar1 - uVar5) / param_2;
    }
  }
  return param_3;
}

