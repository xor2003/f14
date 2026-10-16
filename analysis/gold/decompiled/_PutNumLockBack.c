// 2081:18cf

void __cdecl16far _PutNumLockBack(void)

{
  if ((bRam00000496 & 0x10) != 0) {
    uRam00000417 = uRam00053694;
  }
  return;
}

