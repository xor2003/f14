// 5140:001e

/* WARNING: Stack frame is not setup normally: Input value of stackpointer is not used */
/* WARNING: This function may have set the stack pointer */

void __cdecl16far __astart(void)

{
  undefined1 *puVar1;
  code *pcVar2;
  code *pcVar3;
  byte bVar4;
  int iVar5;
  undefined1 *puVar6;
  undefined1 *puVar7;
  undefined1 *puVar8;
  undefined1 *puVar9;
  undefined2 *puVar10;
  uint uVar11;
  undefined1 *puVar12;
  undefined2 unaff_ES;
  undefined2 uVar13;
  undefined2 uVar14;
  
  uVar13 = 0x4cff;
  uVar14 = 0x5140;
  puVar6 = (undefined1 *)0xdac;
  pcVar3 = (code *)swi(0x21);
  bVar4 = (*pcVar3)();
  if (bVar4 < 2) {
    *(undefined2 *)(puVar6 + -2) = unaff_ES;
    *(undefined2 *)(puVar6 + -4) = 0;
    return;
  }
  uVar11 = iRam0004cff2 + 0x9a0f;
  if (0xfff < uVar11) {
    uVar11 = 0x1000;
  }
  puVar7 = puVar6 + -0x29a2;
  puVar12 = puVar6 + -0x29a2;
  if ((undefined1 *)0x29a1 < puVar6) {
    *(undefined2 *)(puVar6 + -0x29a4) = 0x65f1;
    uVar13 = *(undefined2 *)(puVar6 + -0x29a4);
    *(undefined2 *)(puVar6 + -0x29a4) = 0x5140;
    *(undefined2 *)(puVar6 + -0x29a6) = 0x144d;
    __FF_MSGBANNER();
    *(undefined2 *)(puVar6 + -0x29a4) = 0;
    *(undefined2 *)(puVar6 + -0x29a6) = 0x5140;
    *(undefined2 *)(puVar6 + -0x29a8) = 0x1454;
    __NMSG_WRITE();
    pcVar3 = (code *)swi(0x21);
    (*pcVar3)();
    puVar12 = puVar7;
  }
  iRam0006b9b4 = uVar11 * 0x10 + -1;
  uRam0006b9b6 = 0x65f1;
  puVar8 = (undefined1 *)((uint)puVar12 & 0xfffe);
  puRam0006b9c0 = puVar8 + -2;
  puRam0006b9ba = puVar8;
  *(undefined2 *)(puVar8 + -2) = 0xfffe;
  puVar9 = puVar8 + -4;
  puRam0006b9bc = puVar8 + -4;
  puRam0006b9be = puVar8 + -4;
  puRam0006b9b0 = puVar8 + -4;
  *(undefined2 *)(puVar8 + -4) = 1;
  *(int *)0x2 = uVar11 + 0x65f1;
  pcVar3 = (code *)swi(0x21);
  (*pcVar3)();
  uRam0006b9f0 = uVar13;
  *(undefined2 *)(puVar9 + -2) = 0x65f1;
  uVar13 = *(undefined2 *)(puVar9 + -2);
  puVar12 = (undefined1 *)0x60c8;
  for (iVar5 = 0x7598; iVar5 != 0; iVar5 = iVar5 + -1) {
    puVar1 = puVar12;
    puVar12 = puVar12 + 1;
    *puVar1 = 0;
  }
  *(undefined2 *)(puVar9 + -2) = 0x65f1;
  pcVar2 = (code *)*(int *)0x5f90;
  if (pcVar2 != (code *)0x0) {
    uVar14 = 0x5000;
    puVar10 = (undefined2 *)(puVar9 + -2);
    puVar9 = puVar9 + -2;
    *puVar10 = 0x14b7;
    (*pcVar2)();
  }
  *(undefined2 *)(puVar9 + -2) = uVar14;
  *(undefined2 *)(puVar9 + -4) = 0x14bc;
  FUN_28d5_0540();
  *(undefined2 *)(puVar9 + -2) = 0x28d5;
  *(undefined2 *)(puVar9 + -4) = 0x14c1;
  FUN_28d5_053f();
  *(undefined2 *)(puVar9 + -2) = 0x28d5;
  *(undefined2 *)(puVar9 + -4) = 0x14c8;
  __cinit();
  *(undefined2 *)(puVar9 + -2) = 0x65f1;
  uVar14 = *(undefined2 *)(puVar9 + -2);
  *(undefined2 *)(puVar9 + -2) = *(undefined2 *)0x5b01;
  *(undefined2 *)(puVar9 + -4) = *(undefined2 *)0x5aff;
  *(undefined2 *)(puVar9 + -6) = *(undefined2 *)0x5afd;
  *(undefined2 *)(puVar9 + -8) = 0x5140;
  *(undefined2 *)(puVar9 + -10) = 0x14db;
  uVar14 = FUN_28d5_000e();
  *(undefined2 *)(puVar9 + -8) = uVar14;
  *(undefined2 *)(puVar9 + -10) = 0x28d5;
  *(undefined2 *)(puVar9 + -0xc) = 0x14e0;
  _exit();
  return;
}

