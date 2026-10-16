// 5140:2072

void __cdecl16far __nfree(uint param_1)

{
  byte *pbVar1;
  
  if (uRam00052a9c < param_1) {
    pbVar1 = (byte *)(param_1 - 2);
    *pbVar1 = *pbVar1 | 1;
    if (pbVar1 < pbRam00052a9e) {
      pbRam00052a9e = pbVar1;
    }
  }
  return;
}

