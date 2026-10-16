// 11ac:110e

void __cdecl16far _NCLOUDFILLLINE2(void)

{
  undefined2 *puVar1;
  undefined1 uVar2;
  undefined2 *in_stack_0000000e;
  undefined2 in_stack_00000010;
  undefined2 in_stack_00000012;
  uint in_stack_00000014;
  
  uVar2 = *(undefined1 *)0x8c;
  for (in_stack_00000014 = in_stack_00000014 >> 1; in_stack_00000014 != 0;
      in_stack_00000014 = in_stack_00000014 - 1) {
    puVar1 = in_stack_0000000e;
    in_stack_0000000e = in_stack_0000000e + 1;
    *puVar1 = CONCAT11(uVar2,uVar2);
  }
  return;
}

