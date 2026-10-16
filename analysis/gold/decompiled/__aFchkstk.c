// 5140:1e72

/* WARNING (jumptable): Unable to track spacebase fully for stack */
/* WARNING: Unable to track spacebase fully for stack */

void __cdecl16far __aFchkstk(void)

{
  int iVar1;
  undefined1 *in_AX;
  undefined2 unaff_SS;
  undefined2 in_stack_00000000;
  undefined2 in_stack_00000002;
  
  iVar1 = -(int)in_AX;
  if ((in_AX <= &stack0x0004) && (puRam00052f5a <= &stack0x0004 + iVar1)) {
    *(undefined2 *)(&stack0x0002 + iVar1) = in_stack_00000002;
    *(undefined2 *)(&stack0x0000 + iVar1) = in_stack_00000000;
    return;
  }
  if (uRam00052f56 == 0xffff) {
    __amsg_exit();
    return;
  }
                    /* WARNING: Could not recover jumptable at 0x00053292. Too many branches */
                    /* WARNING: Treating indirect jump as call */
  (*(code *)(ulong)uRam00052f56)();
  return;
}

