// 15ce:013e

void __cdecl16far _INVERTMATRIX(void)

{
  undefined2 uVar1;
  undefined2 uVar2;
  undefined2 uVar3;
  
  uVar3 = uRam0004db9a;
  uVar2 = uRam0004db98;
  uVar1 = uRam0004db92;
  LOCK();
  uRam0004db92 = uRam0004db8e;
  UNLOCK();
  uRam0004db8e = uVar1;
  LOCK();
  uRam0004db98 = uRam0004db90;
  UNLOCK();
  uRam0004db90 = uVar2;
  LOCK();
  uRam0004db9a = uRam0004db96;
  UNLOCK();
  uRam0004db96 = uVar3;
  return;
}

