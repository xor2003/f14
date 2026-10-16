// 2d3b:0008

void __cdecl16far _InstallKeyJoy(void)

{
  undefined2 unaff_SS;
  
  *(undefined1 *)0x3266 = 0;
  *(undefined2 *)0x3267 = 0;
  *(undefined1 *)0x3269 = 0;
  *(undefined1 *)0x326a = 0;
  *(undefined1 *)0x326b = 0;
  *(undefined1 *)&DAT_4fa0_0854 = 0x80;
  *(undefined1 *)&DAT_4fa0_0855 = 0x80;
  uRam00020182 = uRam00000024;
  uRam00020184 = uRam00000026;
  uRam00000024 = 0x6d;
  uRam00000026 = 0x2000;
  return;
}

