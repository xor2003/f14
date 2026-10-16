// 2f18:0175

void __cdecl16far _FlirCopy(int *param_1)

{
  uint *puVar1;
  undefined2 uVar2;
  uint uVar3;
  int iVar4;
  int *piVar5;
  int iVar6;
  byte *pbVar7;
  undefined2 unaff_SS;
  undefined2 uVar8;
  
  uRam0002000c = thunk_EXT_FUN_0000_0000(0x2f18,10,0x4cff);
  uRam0002000a = thunk_EXT_FUN_0000_0000(0x2000,0xc);
  uVar8 = (undefined2)((ulong)param_1 >> 0x10);
  piVar5 = (int *)param_1;
  iVar6 = *param_1 + piVar5[1] * 0x140;
  iVar4 = piVar5[3];
  if ((piVar5[2] != 0) && (iVar4 != 0)) {
    piVar5 = piVar5 + 4;
    do {
      uVar2 = uRam0002000a;
      pbVar7 = (byte *)(iVar6 + *piVar5);
      puVar1 = (uint *)(piVar5 + 1);
      piVar5 = piVar5 + 2;
      for (uVar3 = *puVar1 & 0x7fff; uVar3 != 0; uVar3 = uVar3 - 1) {
        *pbVar7 = *(byte *)(*pbVar7 + 0x2e3a);
        pbVar7 = pbVar7 + 1;
      }
      iVar6 = iVar6 + 0x140;
      iVar4 = iVar4 + -1;
    } while (iVar4 != 0);
  }
  return;
}

