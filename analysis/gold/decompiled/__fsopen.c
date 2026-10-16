// 5140:0358

undefined2 __cdecl16far __fsopen(undefined2 param_1,undefined2 param_2,undefined2 param_3)

{
  int iVar1;
  undefined2 uVar2;
  
  iVar1 = __getstream();
  if (iVar1 == 0) {
    uVar2 = 0;
  }
  else {
    uVar2 = __openfile(param_1,param_2,param_3,iVar1);
  }
  return uVar2;
}

