// 11ac:1280

void __cdecl16far _NDDot(long *param_1,undefined4 param_2,undefined1 param_3)

{
  long lVar1;
  long lVar2;
  
  lVar1 = ((long *)param_1)[1];
  if ((((lRam0004d0fe <= lVar1) && (lVar1 <= lRam0004d106)) &&
      (lVar2 = *param_1, lRam0004d0fa <= lVar2)) && (lVar2 <= lRam0004d102)) {
    *(undefined1 *)((int)lVar1 * 0x140 + (int)param_2 + (int)lVar2) = param_3;
  }
  return;
}

