// 5140:2215

void __cdecl16near __findlast(void)

{
  int in_BX;
  uint *puVar1;
  
  puVar1 = (uint *)*(undefined2 *)(in_BX + 8);
  if (puVar1 == (uint *)*(undefined2 *)(in_BX + 10)) {
    puVar1 = (uint *)*(undefined2 *)(in_BX + 6);
  }
  while( true ) {
    if (*puVar1 == 0xfffe) break;
    puVar1 = (uint *)((int)puVar1 + (*puVar1 & 0xfffe) + 2);
  }
  return;
}

