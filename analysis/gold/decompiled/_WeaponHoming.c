// 48b6:007a

void __cdecl16far _WeaponHoming(void)

{
  if (*(int *)(iRam000531bc + 0x28) == 0) {
    FUN_48b6_0229();
  }
  else {
    FUN_48b6_00a7();
  }
  if (iRam000531ba == 0) {
    *(int *)(iRam000531bc + 0x2a) = *(int *)(iRam000531bc + 0x2a) + 1;
    return;
  }
  *(undefined2 *)(iRam000531bc + 0x2a) = 0;
  return;
}

