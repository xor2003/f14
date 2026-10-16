// 14c7:0311

int __cdecl16far _MapInEMSSprite(int param_1,int param_2)

{
  int iVar1;
  
  iVar1 = _MapIn3dObjects(param_1);
  if (iVar1 != 0) {
    param_1 = param_1 * 2;
    param_2 = param_2 * 4;
    _EMSmap(0,param_2,*(undefined2 *)(param_1 + 0x7e0c));
    _EMSmap(1,param_2 + 1,*(undefined2 *)(param_1 + 0x7e0c));
    _EMSmap(2,param_2 + 2,*(undefined2 *)(param_1 + 0x7e0c));
    _EMSmap(3,param_2 + 3,*(undefined2 *)(param_1 + 0x7e0c));
  }
  return iVar1;
}

