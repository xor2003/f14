// 5140:0bca

char * __cdecl16far _strupr(char *param_1)

{
  char cVar1;
  char *pcVar2;
  
  for (pcVar2 = param_1; cVar1 = *pcVar2, cVar1 != '\0'; pcVar2 = pcVar2 + 1) {
    if ((byte)(cVar1 + 0x9fU) < 0x1a) {
      *pcVar2 = cVar1 + -0x20;
    }
  }
  return param_1;
}

