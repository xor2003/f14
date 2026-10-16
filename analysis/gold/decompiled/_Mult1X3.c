// 2e8c:03ca

void __cdecl16far _Mult1X3(int *param_1,int *param_2,long *param_3)

{
  long lVar1;
  long lVar2;
  long lVar3;
  
  lVar1 = (long)*param_1;
  lVar2 = (long)param_1[1];
  lVar3 = (long)param_1[2];
  *param_3 = *param_2 * lVar1 + param_2[3] * lVar2 + param_2[6] * lVar3;
  param_3[1] = param_2[1] * lVar1 + param_2[4] * lVar2 + param_2[7] * lVar3;
  param_3[2] = param_2[2] * lVar1 + param_2[5] * lVar2 + param_2[8] * lVar3;
  return;
}

