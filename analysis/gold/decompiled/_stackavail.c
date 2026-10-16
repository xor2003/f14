// 5140:2052

int __cdecl16far _stackavail(void)

{
  int iVar1;
  
  if (puRam00052f5a < &stack0x0004) {
    iVar1 = -((int)puRam00052f5a - (int)&stack0x0004);
  }
  else {
    iVar1 = 0;
  }
  return iVar1;
}

