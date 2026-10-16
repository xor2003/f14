// 220f:0615

void __cdecl16far _DoCockpitDisplays(void)

{
  undefined2 uVar1;
  
  if (iRam0004ec66 == 0) {
    if (iRam00057e84 == 3) {
      FUN_220f_28b9(0);
      FUN_220f_28b9(1);
      FUN_220f_28b9(2);
      FUN_220f_28b9(3);
      FUN_220f_1e35(0x220f);
      FUN_220f_118f();
      FUN_220f_1f13(0x220f);
      FUN_220f_1069(0x220f);
      FUN_220f_1d70(0x220f);
      FUN_220f_2068();
      FUN_220f_2d25();
      FUN_220f_24f2();
      FUN_220f_08e1();
    }
    if (iRam00057e84 == 0) {
      _HUD();
      FUN_220f_1e35(0x2a48);
      FUN_220f_118f();
      FUN_220f_1f13(0x2a48);
      FUN_220f_1069(0x2a48);
      FUN_220f_22b5(0x2a48);
      FUN_220f_2385();
      FUN_220f_2068();
      FUN_220f_1d70(0x2a48);
      FUN_220f_1229(0x2a48);
      FUN_220f_28b9(0);
      FUN_220f_28b9(1);
      FUN_220f_28b9(3);
      FUN_220f_2a50();
      FUN_220f_0750();
      FUN_220f_08e1();
    }
    if (((iRam00057e84 == 4) || (iRam00057e84 == 5)) && (iRam0004f8ba != 0)) {
      FUN_4746_0008();
    }
    if (iRam00057e84 == 1) {
      FUN_220f_0aa6();
    }
    if (iRam00057e84 != 2) goto LAB_220f_073c;
    uVar1 = 1;
  }
  else {
    if (iRam00057e84 == 0) {
      FUN_220f_28b9(4);
      FUN_220f_197c();
      FUN_220f_100b(0x220f);
      FUN_220f_0782();
      FUN_220f_16e7();
    }
    if (iRam00057e84 == 3) {
      FUN_220f_28b9(5);
      FUN_220f_0782();
      FUN_220f_158a();
      FUN_220f_1323();
      FUN_220f_09e7();
    }
    if (iRam00057e84 == 1) {
      FUN_220f_13d1();
    }
    if (iRam00057e84 != 2) goto LAB_220f_073c;
    uVar1 = 0;
  }
  FUN_220f_0c5f(uVar1);
LAB_220f_073c:
  if ((iRam00057e84 == 0x47) || (iRam00057e84 == 0x48)) {
    FUN_4e88_0f4e();
  }
  return;
}

