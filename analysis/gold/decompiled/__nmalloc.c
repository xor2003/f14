// 5140:2093

undefined2 __cdecl16far __nmalloc(uint param_1)

{
  undefined2 uVar1;
  bool bVar2;
  
  bVar2 = param_1 < 0xffe8;
  if (param_1 < 0xffe9) {
    uVar1 = __searchseg();
    if (!bVar2) {
      return uVar1;
    }
    __growseg();
    if ((!bVar2) && (uVar1 = __searchseg(), !bVar2)) {
      return uVar1;
    }
  }
  return 0;
}

