// 15ce:0d54

void __cdecl16far _SetViewPos_g1(undefined2 param_1,undefined2 param_2)

{
  code *pcVar1;
  
  uRam00052a6a = param_1;
  uRam00052a6c = param_2;
  if (cRam00052a79 != '\0') {
    pcVar1 = (code *)swi(0x33);
    (*pcVar1)();
  }
  return;
}

