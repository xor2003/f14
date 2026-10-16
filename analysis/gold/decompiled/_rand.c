// 5140:0c7e

uint __cdecl16far _rand(void)

{
  long lVar1;
  
  lVar1 = __aFulmul((undefined2)lRam00052b0c,lRam00052b0c._2_2_,0x43fd,3);
  lRam00052b0c = lVar1 + 0x269ec3;
  return (uint)((ulong)(lVar1 + 0x269ec3) >> 0x10) & 0x7fff;
}

