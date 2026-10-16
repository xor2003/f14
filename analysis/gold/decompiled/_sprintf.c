// 5140:095e

undefined2 __cdecl16far _sprintf(undefined1 *param_1,undefined2 param_2)

{
  undefined1 *puVar1;
  undefined2 uVar2;
  
  uRam0005341c = 0x42;
  puRam0005341a = param_1;
  puRam00053416 = param_1;
  iRam00053418 = 0x7fff;
  uVar2 = __output(0x6426,param_2,&stack0x0008);
  iRam00053418 = iRam00053418 + -1;
  if (iRam00053418 < 0) {
    __flsbuf(0,0x6426);
  }
  else {
    puVar1 = puRam00053416;
    puRam00053416 = puRam00053416 + 1;
    *puVar1 = 0;
  }
  return uVar2;
}

