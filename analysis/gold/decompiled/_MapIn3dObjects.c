// 14c7:0205

void __cdecl16far _MapIn3dObjects(int param_1)

{
  int iVar1;
  
  iVar1 = *(int *)(param_1 * 2 + 0x7e0c);
  if ((iVar1 != 0) && (iRam0004d2ae != iVar1)) {
    iRam0004d2ae = iVar1;
    _EMSmap(0,0,iVar1);
    _EMSmap(1,1,iVar1);
    _EMSmap(2,2,iVar1);
    _EMSmap(3,3,iVar1);
  }
  uRam0004fd2c = *(undefined2 *)(param_1 * 2 + -0x7688);
  return;
}

