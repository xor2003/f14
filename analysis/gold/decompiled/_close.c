// 5140:19ca

void _close(undefined2 param_1,uint param_2)

{
  code *pcVar1;
  bool bVar2;
  
  bVar2 = param_2 < uRam00052ad7;
  if (bVar2) {
    pcVar1 = (code *)swi(0x21);
    (*pcVar1)();
    if (!bVar2) {
      *(undefined1 *)(param_2 + 0x5ae9) = 0;
    }
  }
  __dosret0();
  return;
}

