// 5140:1996

undefined2 * __cdecl16far __getstream(void)

{
  undefined2 *puVar1;
  
  puVar1 = (undefined2 *)0x5b38;
  while( true ) {
    if (puRam00052c68 < puVar1) {
      return (undefined2 *)0x0;
    }
    if ((*(byte *)(puVar1 + 3) & 0x83) == 0) break;
    puVar1 = puVar1 + 4;
  }
  puVar1[1] = 0;
  *(undefined1 *)(puVar1 + 3) = 0;
  puVar1[2] = 0;
  *puVar1 = 0;
  *(undefined1 *)((int)puVar1 + 7) = 0xff;
  return puVar1;
}

