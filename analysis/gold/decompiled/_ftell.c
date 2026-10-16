// 5140:1c8e

int __cdecl16far _ftell(int *param_1)

{
  uint uVar1;
  char *pcVar2;
  char *pcVar3;
  long lVar4;
  long lVar5;
  int local_e;
  int local_a;
  int local_6;
  
  uVar1 = (uint)*(byte *)((int)param_1 + 7);
  if (param_1[1] < 0) {
    param_1[1] = 0;
  }
  lVar4 = _lseek(0x5140,uVar1,0,0,1);
  local_6 = (int)lVar4;
  if (lVar4 < 0) {
LAB_5140_1cd4:
    local_a = -1;
  }
  else {
    if (((*(byte *)(param_1 + 3) & 8) == 0) && ((*(byte *)(param_1 + 0x50) & 1) == 0)) {
      return local_6 - param_1[1];
    }
    local_a = *param_1 - param_1[2];
    if ((*(byte *)(param_1 + 3) & 3) == 0) {
      if ((*(byte *)(param_1 + 3) & 0x80) == 0) {
        uRam00052aca = 0x16;
        goto LAB_5140_1cd4;
      }
    }
    else if ((*(byte *)(uVar1 + 0x5ae9) & 0x80) != 0) {
      for (pcVar3 = (char *)param_1[2]; pcVar3 < (char *)*param_1; pcVar3 = pcVar3 + 1) {
        if (*pcVar3 == '\n') {
          local_a = local_a + 1;
        }
      }
    }
    if (lVar4 != 0) {
      if ((*(byte *)(param_1 + 3) & 1) != 0) {
        if (param_1[1] == 0) {
          local_a = 0;
        }
        else {
          local_e = (*param_1 - param_1[2]) + param_1[1];
          if ((*(byte *)(uVar1 + 0x5ae9) & 0x80) != 0) {
            lVar5 = _lseek(0x5140,uVar1,0,0,2);
            if (lVar5 == lVar4) {
              pcVar2 = (char *)(local_e + param_1[2]);
              for (pcVar3 = (char *)param_1[2]; pcVar3 < pcVar2; pcVar3 = pcVar3 + 1) {
                if (*pcVar3 == '\n') {
                  local_e = local_e + 1;
                }
              }
              if ((*(byte *)(param_1 + 0x50) & 0x20) != 0) {
                local_e = local_e + 1;
              }
            }
            else {
              _lseek(0x5140,uVar1,lVar4,0);
              local_e = param_1[0x51];
              if ((*(byte *)(uVar1 + 0x5ae9) & 4) != 0) {
                local_e = local_e + 1;
              }
            }
          }
          local_6 = local_6 - local_e;
        }
      }
      local_a = local_6 + local_a;
    }
  }
  return local_a;
}

