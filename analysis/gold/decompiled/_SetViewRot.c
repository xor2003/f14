// 170c:01c9

void __cdecl16far _SetViewRot(int param_1,int param_2,int param_3,undefined2 param_4)

{
  _MAKEROTATIONMATRIX(0xb8a,-param_1,-param_2,-param_3);
  _MakematHPR(-param_1,-param_2,param_3,0xb9c);
  _NSETVIEWMAT(0xb9c,param_4);
  return;
}

