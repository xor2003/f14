// 5140:0584

undefined2 __cdecl16far _printf(undefined2 param_1)

{
  undefined2 uVar1;
  undefined2 uVar2;
  
  uVar1 = __stbuf(0x5b40);
  uVar2 = __output(0x5b40,param_1,&stack0x0006);
  __ftbuf(uVar1,0x5b40);
  return uVar2;
}

