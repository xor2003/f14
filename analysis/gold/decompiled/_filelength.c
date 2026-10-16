// 5140:09b8

undefined2 __cdecl16far _filelength(int param_1)

{
  undefined2 uVar1;
  long lVar2;
  long lVar3;
  
  if ((param_1 < 0) || (iRam00052ad7 <= param_1)) {
    uRam00052aca = 9;
    uVar1 = 0xffff;
  }
  else {
    lVar2 = _lseek(0x5140,param_1,0,0,1);
    if (lVar2 == -1) {
      uVar1 = 0xffff;
    }
    else {
      lVar3 = _lseek(0x5140,param_1,0,0,2);
      uVar1 = (undefined2)lVar3;
      if (lVar3 != lVar2) {
        _lseek(0x5140,param_1,lVar2,0);
      }
    }
  }
  return uVar1;
}

