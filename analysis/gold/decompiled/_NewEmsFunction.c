// 1125:006a

void __cdecl16far _NewEmsFunction(void)

{
  int iVar1;
  
  iVar1 = _EMMThere();
  if (iVar1 != 0) {
    iRam0004d066 = _EMSPageFrame();
    if (iRam0004d066 != 0) {
      iRam0004d06a = _EMSPageFrame_g1();
      iRam0004d06c = iRam0004d06a;
      uRam0004d068 = _EMSAllocatePages(iRam0004d06a);
      iRam0004d06c = iRam0004d06c + -0x34;
    }
  }
  return;
}

