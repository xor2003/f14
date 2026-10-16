// 5140:0ff4

void __cdecl16far __FF_MSGBANNER(void)

{
  __NMSG_WRITE(0xfc);
  if (iRam00052b12 != 0) {
    (*pcRam00052b10)(0x5140);
  }
  __NMSG_WRITE(0xff);
  return;
}

