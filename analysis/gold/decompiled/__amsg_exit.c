// 5140:00f1

void __amsg_exit(void)

{
  code *pcVar1;
  undefined2 in_AX;
  undefined2 uVar2;
  undefined2 unaff_SS;
  
  uVar2 = 0x5140;
  __FF_MSGBANNER();
  __NMSG_WRITE(in_AX);
  if (*(int *)0x5f92 == -0x292a) {
    uVar2 = 0x5000;
    (*(code *)*(undefined2 *)0x5f96)();
  }
  (*pcRam00052a92)(uVar2,0xff);
  pcVar1 = (code *)swi(1);
  (*pcVar1)();
  return;
}

