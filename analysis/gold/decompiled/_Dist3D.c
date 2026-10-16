// 2e8c:01de

int __cdecl16far _Dist3D(undefined2 param_1,undefined2 param_2,uint param_3)

{
  int iVar1;
  
  iVar1 = _Dist2D(param_1,param_2);
  if (-1 < iVar1) {
    iVar1 = _Dist2D(iVar1,param_3);
    return iVar1;
  }
  return ((param_3 ^ (int)param_3 >> 0xf) - ((int)param_3 >> 0xf) >> 1) + iVar1;
}

