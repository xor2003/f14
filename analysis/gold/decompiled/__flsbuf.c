// 5140:10f8

uint __cdecl16far __flsbuf(uint param_1,int *param_2)

{
  int *piVar1;
  byte bVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  
  piVar1 = param_2;
  bVar2 = *(byte *)(param_2 + 3);
  if (((bVar2 & 0x82) != 0) && ((bVar2 & 0x40) == 0)) {
    param_2[1] = 0;
    if ((bVar2 & 1) != 0) {
      if ((bVar2 & 0x10) == 0) goto LAB_5140_1170;
      *param_2 = param_2[2];
      bVar2 = bVar2 & 0xfe;
    }
    *(byte *)(param_2 + 3) = bVar2 & 0xef | 2;
    uVar5 = (uint)*(byte *)((int)param_2 + 7);
    if (((bVar2 & 8) == 0) &&
       (((bVar2 & 4) != 0 ||
        (((*(byte *)(param_2 + 0x50) & 1) == 0 &&
         (((((param_2 == (int *)0x5b40 || (param_2 == (int *)0x5b48)) || (param_2 == (int *)0x5b58))
           && ((*(byte *)(uVar5 + 0x5ae9) & 0x40) != 0)) ||
          (__getbuf(param_2), (*(byte *)(piVar1 + 3) & 8) == 0)))))))) {
      iVar3 = _write(0x5140,uVar5,&param_1,1);
      iVar4 = 1;
    }
    else {
      iVar4 = *piVar1 - piVar1[2];
      *piVar1 = piVar1[2] + 1;
      piVar1[1] = piVar1[0x51] + -1;
      if (iVar4 == 0) {
        iVar3 = 0;
        if ((*(byte *)(uVar5 + 0x5ae9) & 0x20) != 0) {
          _lseek(0x5140,uVar5,0,0,2);
          iVar3 = 0;
          iVar4 = 0;
        }
      }
      else {
        iVar3 = _write(0x5140,uVar5,piVar1[2],iVar4);
      }
      *(undefined1 *)piVar1[2] = (char)param_1;
    }
    if (iVar3 == iVar4) {
      return param_1 & 0xff;
    }
  }
LAB_5140_1170:
  *(byte *)(piVar1 + 3) = *(byte *)(piVar1 + 3) | 0x20;
  return 0xffff;
}

