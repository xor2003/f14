// 2a3c:0074

void __cdecl16far
_TACTRANSSCALELINE(undefined2 param_1,int param_2,int *param_3,int param_4,int param_5,int param_6,
                  undefined2 param_7,char param_8)

{
  char *pcVar1;
  undefined2 unaff_SS;
  
  pcVar1 = (char *)(param_5 * 0x140 + param_4);
  do {
    if (*(byte *)(*param_3 + param_2) != 0) {
      *pcVar1 = (*(byte *)(*param_3 + param_2) >> 5) + param_8;
    }
    param_3 = param_3 + 1;
    pcVar1 = pcVar1 + 1;
    param_6 = param_6 + -1;
  } while (param_6 != 0);
  return;
}

