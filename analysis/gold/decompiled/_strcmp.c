// 5140:06ca

int __cdecl16far _strcmp(byte *param_1,char *param_2)

{
  byte *pbVar1;
  char *pcVar2;
  byte *pbVar3;
  int iVar4;
  uint uVar5;
  char *pcVar6;
  byte *pbVar7;
  bool bVar8;
  bool bVar9;
  
  iVar4 = 0;
  uVar5 = 0xffff;
  do {
    if (uVar5 == 0) break;
    uVar5 = uVar5 - 1;
    pcVar2 = param_2;
    param_2 = param_2 + 1;
  } while (*pcVar2 != '\0');
  pcVar6 = (char *)~uVar5;
  bVar8 = param_2 < pcVar6;
  pbVar7 = (byte *)(param_2 + -(int)pcVar6);
  bVar9 = pbVar7 == (byte *)0x0;
  do {
    if (pcVar6 == (char *)0x0) break;
    pcVar6 = pcVar6 + -1;
    pbVar3 = pbVar7;
    pbVar7 = pbVar7 + 1;
    pbVar1 = param_1;
    param_1 = param_1 + 1;
    bVar8 = *pbVar1 < *pbVar3;
    bVar9 = *pbVar1 == *pbVar3;
  } while (bVar9);
  if (!bVar9) {
    iVar4 = (1 - (uint)bVar8) - (uint)(bVar8 != 0);
  }
  return iVar4;
}

