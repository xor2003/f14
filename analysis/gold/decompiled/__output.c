// 5140:14be

undefined2 __cdecl16far __output(undefined2 param_1,char *param_2)

{
  char cVar1;
  byte bVar2;
  undefined2 uVar3;
  
  __aFchkstk();
  cVar1 = *param_2;
  if (cVar1 == '\0') {
    return 0;
  }
  if ((byte)(cVar1 - 0x20U) < 0x59) {
    bVar2 = *(byte *)(ulong)((byte)(cVar1 - 0x20U) + 0x5c80) & 0xf;
  }
  else {
    bVar2 = 0;
  }
                    /* WARNING: Could not emulate address calculation at 0x00052903 */
                    /* WARNING: Treating indirect jump as call */
  uVar3 = (*(code *)*(undefined2 *)
                     ((char)(*(byte *)(ulong)((byte)(bVar2 * '\b') + 0x5c80) >> 4) * 2 + 0x14ae))
                    (cVar1);
  return uVar3;
}

