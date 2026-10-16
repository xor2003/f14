// 5140:1e96

/* WARNING: Removing unreachable block (ram,0x000533a6) */

uint __cdecl16far _sopen(undefined2 param_1,uint param_2,char param_3,uint param_4)

{
  code *pcVar1;
  char cVar2;
  uint uVar3;
  int iVar4;
  uint uVar5;
  byte bVar6;
  uint extraout_DX;
  bool bVar7;
  undefined1 uVar8;
  bool bVar9;
  byte local_6;
  char local_4;
  char local_3;
  
  cVar2 = '\0';
  if (2 < bRam00052ad2) {
    cVar2 = param_3;
  }
  _param_3 = param_4;
  local_6 = 0;
  if (((param_2 & 0x8000) == 0) && (((param_2 & 0x4000) != 0 || ((bRam00052f7d & 0x80) == 0)))) {
    local_6 = 0x80;
  }
  bVar7 = false;
  pcVar1 = (code *)swi(0x21);
  uVar5 = param_2;
  uVar3 = (*pcVar1)();
  if (bVar7) {
    if ((uVar3 != 2) || ((uVar5 & 0x100) == 0)) goto LAB_5140_1ef4;
    bVar7 = false;
    local_4 = -99;
    __cXENIXtoDOSmode();
    uVar8 = 0;
    uVar5 = 0;
    _param_3 = param_4;
LAB_5140_1fb0:
    pcVar1 = (code *)swi(0x21);
    uVar3 = (*pcVar1)();
    if ((bool)uVar8) {
LAB_5140_1fb9:
      uVar5 = __dosretax();
      return uVar5;
    }
    if ((local_4 != '\0') || ((param_2 & 2) == 0)) {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
      bVar9 = false;
      pcVar1 = (code *)swi(0x21);
      uVar3 = (*pcVar1)();
      if (bVar9) goto LAB_5140_1fb9;
      if ((!bVar7) && ((_param_3 & 1) != 0)) {
        bVar7 = false;
        uVar5 = (uint)(byte)((byte)uVar5 | 1);
        pcVar1 = (code *)swi(0x21);
        (*pcVar1)();
        if (bVar7) goto LAB_5140_1fb9;
      }
    }
  }
  else {
    if ((uVar5 & 0x500) == 0x500) {
      pcVar1 = (code *)swi(0x21);
      (*pcVar1)();
      goto LAB_5140_1ef4;
    }
    bVar7 = true;
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
    if ((extraout_DX & 0x80) != 0) {
      local_6 = local_6 | 0x40;
    }
    if ((local_6 & 0x40) == 0) {
      if ((param_2 & 0x200) == 0) {
        if (((local_6 & 0x80) != 0) && ((param_2 & 2) != 0)) {
          pcVar1 = (code *)swi(0x21);
          (*pcVar1)();
          pcVar1 = (code *)swi(0x21);
          iVar4 = (*pcVar1)();
          if ((iVar4 != 0) && (local_3 == '\x1a')) {
            pcVar1 = (code *)swi(0x21);
            (*pcVar1)();
            pcVar1 = (code *)swi(0x21);
            (*pcVar1)();
          }
          uVar5 = 0;
          pcVar1 = (code *)swi(0x21);
          (*pcVar1)();
        }
      }
      else {
        uVar8 = 0;
        if ((param_2 & 3) == 0) {
          pcVar1 = (code *)swi(0x21);
          (*pcVar1)();
          pcVar1 = (code *)swi(0x21);
          (*pcVar1)();
          local_4 = cVar2;
          goto LAB_5140_1fb0;
        }
        uVar5 = 0;
        pcVar1 = (code *)swi(0x21);
        (*pcVar1)();
      }
    }
  }
  if ((local_6 & 0x40) == 0) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
    bVar6 = 0;
    if ((uVar5 & 1) != 0) {
      bVar6 = 0x10;
    }
    if ((param_2 & 8) != 0) {
      bVar6 = bVar6 | 0x20;
    }
  }
  else {
    bVar6 = 0;
  }
  if (uVar3 < uRam00052ad7) {
    *(byte *)(uVar3 + 0x5ae9) = bVar6 | local_6 | 1;
    return uVar3;
  }
  pcVar1 = (code *)swi(0x21);
  (*pcVar1)();
LAB_5140_1ef4:
  uVar5 = __dosretax();
  return uVar5;
}

