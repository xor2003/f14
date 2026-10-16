// 2a3c:0002

void __cdecl16far
_TRANSSCALELINE(undefined2 param_1,int param_2,int *param_3,int param_4,int param_5,int param_6,
               undefined2 param_7,int param_8)

{
  char *pcVar1;
  undefined2 unaff_SS;
  
  pcVar1 = (char *)(param_5 * 0x140 + param_4);
  if (param_8 != -1) {
    do {
      if (*(char *)(*param_3 + param_2) != (char)param_8) {
        *pcVar1 = *(char *)(*param_3 + param_2);
      }
      param_3 = param_3 + 1;
      pcVar1 = pcVar1 + 1;
      param_6 = param_6 + -1;
    } while (param_6 != 0);
    return;
  }
  do {
    *pcVar1 = *(char *)(*param_3 + param_2);
    param_3 = param_3 + 1;
    pcVar1 = pcVar1 + 1;
    param_6 = param_6 + -1;
  } while (param_6 != 0);
  return;
}

