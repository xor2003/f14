; MPSREAL.ASM - reconstructed MicroProse graphics/mouse/overlay/PIC library
; Verbatim port of retail F14.EXE CODE_9 + CODE_69 (picload) segments.
DGROUP GROUP MPSDAT
MPSDAT SEGMENT WORD PUBLIC 'DATA'
dg_lock db 0
dg_drawn db 0
dg_ret0 dw 0
dg_ret2 dw 0
mps_exec0 dw 0
mps_exec1 dw 0
mps_freep dw 0
mps_dash db " - $"
mps_e1 db "Allocated 1Mb of space????$"
mps_e2 db "MS-DOS lied to us about how much memory was available$"
mps_e3 db "Filename not found$"
mps_e4 db "Not enough memory to load file$"
mps_e5 db "Overlay load failed for some reason$"
mps_e6 db "Overlay has overrun allocated memory$"
mps_e7 db "Unable to shrink overlay memory allocation$"
mps_e8 db "Error releasing overlay memory$"
pic_bound dw offset pic_cbuf
pic_handle dw 0ffffh
mps_x1 dw 0
mps_x2 dw 0
mps_y1 dw 0
mps_y2 dw 0
mps_span0 dw 200 dup(0ffffh)
mps_span1 dw 200 dup(0)
ln_tmp0 dw 0
ln_tmp1 dw 0
mps_penx dw 0ffffh
mps_peny dw 0
mps_clipL dw 0
mps_clipT dw 0
eg_flag db 0
eg_dx dw 0
eg_s0 db 0
eg_dy dw 0
eg_s1 db 0
eg_cx dw 0
eg_cy dw 0
mps_clipR dw 13fh
mps_clipB dw 6fh
PUBLIC _MouseX,_MouseY,_Button,_MOUSE
_MouseX dw 0
_MouseY dw 0
_Button dw 0
mps_hits dw 0
mps_curs dw 0
mps_hotx dw 0
mps_hoty dw 0
mps_xsh db 0
_MOUSE db 0
mps_ucb dd 0
m3_regs dw 4 dup(0)
eg2_flag db 0
eg2_dx dw 0
eg2_s0 db 0
eg2_dy dw 0
eg2_s1 db 0
eg2_cx dw 0
eg2_cy dw 0
pic_state db 46 dup(0)
pic_6425 db 8 dup(0)
pic_6428 dw 0
pic_src dw 0
pic_dict db 327eH dup(0)
pic_pump dd 0
pic_cbuf db 400H dup(0)
mat_b7e dw 8 dup(0)
mat_b9c dw 8 dup(0)
MPSDAT ENDS

MPSLIBM_TEXT SEGMENT BYTE PUBLIC 'CODE'
        ASSUME CS:MPSLIBM_TEXT, DS:DGROUP
g5ce0:
	db 068h,0f7h,05bh,05eh,05fh,0c9h,0cbh,000h
g5ce8:
	db 055h
g5ce9:
	db 08Bh,0ECh
g5ceb:
	db 056h
g5cec:
	db 057h
g5ced:
	db 08Bh,07Eh,006h
g5cf0:
	db 08Bh,04Eh,008h
g5cf3:
	db 08Bh,076h,00Ah
g5cf6:
	db 08Bh,06Eh,00Ch
g5cf9:
	call far ptr g5d02
g5cfe:
	db 05Fh
g5cff:
	db 05Eh
g5d00:
	db 05Dh
g5d01:
	db 0CBh
g5d02:
	db 08Bh,0D9h
g5d04:
	call far ptr F_SinDP
g5d09:
	db 089h,01Eh,07Eh,00Bh
g5d0d:
	db 08Bh,0D9h
g5d0f:
	call far ptr F_CosDP
g5d14:
	db 089h,01Eh,080h,00Bh
g5d18:
	db 08Bh,0DDh
g5d1a:
	call far ptr F_SinDP
g5d1f:
	db 089h,01Eh,086h,00Bh
g5d23:
	db 08Bh,0DDh
g5d25:
	call far ptr F_CosDP
g5d2a:
	db 089h,01Eh,088h,00Bh
g5d2e:
	db 08Bh,0DEh
g5d30:
	call far ptr F_SinDP
g5d35:
	db 089h,01Eh,082h,00Bh
g5d39:
	db 08Bh,0DEh
g5d3b:
	call far ptr F_CosDP
g5d40:
	db 089h,01Eh,084h,00Bh
g5d44:
	db 0A1h,082h,00Bh
g5d47:
	db 0F7h,02Eh,086h,00Bh
g5d4b:
	db 0D1h,0E0h
g5d4d:
	db 0D1h,0D2h
g5d4f:
	db 08Bh,0F2h
g5d51:
	db 08Bh,0C2h
g5d53:
	db 0F7h,02Eh,07Eh,00Bh
g5d57:
	db 0D1h,0E0h
g5d59:
	db 0D1h,0D2h
g5d5b:
	db 08Bh,0D8h
g5d5d:
	db 08Bh,0CAh
g5d5f:
	db 0A1h,080h,00Bh
g5d62:
	db 0F7h,02Eh,088h,00Bh
g5d66:
	db 0D1h,0E0h
g5d68:
	db 0D1h,0D2h
g5d6a:
	db 003h,0C3h
g5d6c:
	db 013h,0D1h
g5d6e:
	db 089h,015h
g5d70:
	db 0A1h,082h,00Bh
g5d73:
	db 0F7h,02Eh,088h,00Bh
g5d77:
	db 0D1h,0E0h
g5d79:
	db 0D1h,0D2h
g5d7b:
	db 08Bh,0EAh
g5d7d:
	db 08Bh,0C2h
g5d7f:
	db 0F7h,02Eh,07Eh,00Bh
g5d83:
	db 0D1h,0E0h
g5d85:
	db 0D1h,0D2h
g5d87:
	db 08Bh,0D8h
g5d89:
	db 08Bh,0CAh
g5d8b:
	db 0A1h,080h,00Bh
g5d8e:
	db 0F7h,02Eh,086h,00Bh
g5d92:
	db 0D1h,0E0h
g5d94:
	db 0D1h,0D2h
g5d96:
	db 02Bh,0D8h
g5d98:
	db 01Bh,0CAh
g5d9a:
	db 089h,04Dh,002h
g5d9d:
	db 0A1h,07Eh,00Bh
g5da0:
	db 0F7h,02Eh,084h,00Bh
g5da4:
	db 0D1h,0E0h
g5da6:
	db 0D1h,0D2h
g5da8:
	db 089h,055h,004h
g5dab:
	db 0A1h,086h,00Bh
g5dae:
	db 0F7h,02Eh,084h,00Bh
g5db2:
	db 0D1h,0E0h
g5db4:
	db 0D1h,0D2h
g5db6:
	db 089h,055h,006h
g5db9:
	db 0A1h,088h,00Bh
g5dbc:
	db 0F7h,02Eh,084h,00Bh
g5dc0:
	db 0D1h,0E0h
g5dc2:
	db 0D1h,0D2h
g5dc4:
	db 089h,055h,008h
g5dc7:
	db 0A1h,082h,00Bh
g5dca:
	db 0F7h,0D8h
g5dcc:
	db 089h,045h,00Ah
g5dcf:
	db 08Bh,0C6h
g5dd1:
	db 0F7h,02Eh,080h,00Bh
g5dd5:
	db 0D1h,0E0h
g5dd7:
	db 0D1h,0D2h
g5dd9:
	db 08Bh,0D8h
g5ddb:
	db 08Bh,0CAh
g5ddd:
	db 0A1h,07Eh,00Bh
g5de0:
	db 0F7h,02Eh,088h,00Bh
g5de4:
	db 0D1h,0E0h
g5de6:
	db 0D1h,0D2h
g5de8:
	db 02Bh,0D8h
g5dea:
	db 01Bh,0CAh
g5dec:
	db 089h,04Dh,00Ch
g5def:
	db 08Bh,0C5h
g5df1:
	db 0F7h,02Eh,080h,00Bh
g5df5:
	db 0D1h,0E0h
g5df7:
	db 0D1h,0D2h
g5df9:
	db 08Bh,0D8h
g5dfb:
	db 08Bh,0CAh
g5dfd:
	db 0A1h,07Eh,00Bh
g5e00:
	db 0F7h,02Eh,086h,00Bh
g5e04:
	db 0D1h,0E0h
g5e06:
	db 0D1h,0D2h
g5e08:
	db 003h,0C3h
g5e0a:
	db 013h,0D1h
g5e0c:
	db 089h,055h,00Eh
g5e0f:
	db 0A1h,080h,00Bh
g5e12:
	db 0F7h,02Eh,084h,00Bh
g5e16:
	db 0D1h,0E0h
g5e18:
	db 0D1h,0D2h
g5e1a:
	db 089h,055h,010h
g5e1d:
	db 0CBh
g5e1e:
	mov bx,offset mat_b9c
g5e21:
	db 08Bh,047h,002h
g5e24:
	db 087h,047h,006h
g5e27:
	db 089h,047h,002h
g5e2a:
	db 08Bh,047h,004h
g5e2d:
	db 087h,047h,00Ch
g5e30:
	db 089h,047h,004h
g5e33:
	db 08Bh,047h,00Ah
g5e36:
	db 087h,047h,00Eh
g5e39:
	db 089h,047h,00Ah
g5e3c:
	db 0CBh
g5e3d:
	db 055h
g5e3e:
	db 08Bh,0ECh
g5e40:
	db 056h
g5e41:
	db 057h
g5e42:
	db 08Bh,05Eh,006h
g5e45:
	db 08Bh,076h,008h
g5e48:
	db 08Bh,07Eh,00Ah
g5e4b:
	call far ptr g5e54
g5e50:
	db 05Fh
g5e51:
	db 05Eh
g5e52:
	db 05Dh
g5e53:
	db 0CBh
g5e54:
	db 08Bh,007h
g5e56:
	db 0F7h,02Ch
g5e58:
	db 0D1h,0E0h
g5e5a:
	db 0D1h,0D2h
g5e5c:
	db 08Bh,0E8h
g5e5e:
	db 08Bh,0CAh
g5e60:
	db 08Bh,047h,002h
g5e63:
	db 0F7h,06Ch,006h
g5e66:
	db 0D1h,0E0h
g5e68:
	db 0D1h,0D2h
g5e6a:
	db 003h,0E8h
g5e6c:
	db 013h,0CAh
g5e6e:
	db 08Bh,047h,004h
g5e71:
	db 0F7h,06Ch,00Ch
g5e74:
	db 0D1h,0E0h
g5e76:
	db 0D1h,0D2h
g5e78:
	db 003h,0E8h
g5e7a:
	db 013h,0CAh
g5e7c:
	db 089h,00Dh
g5e7e:
	db 08Bh,007h
g5e80:
	db 0F7h,06Ch,002h
g5e83:
	db 0D1h,0E0h
g5e85:
	db 0D1h,0D2h
g5e87:
	db 08Bh,0E8h
g5e89:
	db 08Bh,0CAh
g5e8b:
	db 08Bh,047h,002h
g5e8e:
	db 0F7h,06Ch,008h
g5e91:
	db 0D1h,0E0h
g5e93:
	db 0D1h,0D2h
g5e95:
	db 003h,0E8h
g5e97:
	db 013h,0CAh
g5e99:
	db 08Bh,047h,004h
g5e9c:
	db 0F7h,06Ch,00Eh
g5e9f:
	db 0D1h,0E0h
g5ea1:
	db 0D1h,0D2h
g5ea3:
	db 003h,0E8h
g5ea5:
	db 013h,0CAh
g5ea7:
	db 089h,04Dh,002h
g5eaa:
	db 08Bh,007h
g5eac:
	db 0F7h,06Ch,004h
g5eaf:
	db 0D1h,0E0h
g5eb1:
	db 0D1h,0D2h
g5eb3:
	db 08Bh,0E8h
g5eb5:
	db 08Bh,0CAh
g5eb7:
	db 08Bh,047h,002h
g5eba:
	db 0F7h,06Ch,00Ah
g5ebd:
	db 0D1h,0E0h
g5ebf:
	db 0D1h,0D2h
g5ec1:
	db 003h,0E8h
g5ec3:
	db 013h,0CAh
g5ec5:
	db 08Bh,047h,004h
g5ec8:
	db 0F7h,06Ch,010h
g5ecb:
	db 0D1h,0E0h
g5ecd:
	db 0D1h,0D2h
g5ecf:
	db 003h,0E8h
g5ed1:
	db 013h,0CAh
g5ed3:
	db 089h,04Dh,004h
g5ed6:
	db 08Bh,047h,006h
g5ed9:
	db 0F7h,02Ch
g5edb:
	db 0D1h,0E0h
g5edd:
	db 0D1h,0D2h
g5edf:
	db 08Bh,0E8h
g5ee1:
	db 08Bh,0CAh
g5ee3:
	db 08Bh,047h,008h
g5ee6:
	db 0F7h,06Ch,006h
g5ee9:
	db 0D1h,0E0h
g5eeb:
	db 0D1h,0D2h
g5eed:
	db 003h,0E8h
g5eef:
	db 013h,0CAh
g5ef1:
	db 08Bh,047h,00Ah
g5ef4:
	db 0F7h,06Ch,00Ch
g5ef7:
	db 0D1h,0E0h
g5ef9:
	db 0D1h,0D2h
g5efb:
	db 003h,0E8h
g5efd:
	db 013h,0CAh
g5eff:
	db 089h,04Dh,006h
g5f02:
	db 08Bh,047h,006h
g5f05:
	db 0F7h,06Ch,002h
g5f08:
	db 0D1h,0E0h
g5f0a:
	db 0D1h,0D2h
g5f0c:
	db 08Bh,0E8h
g5f0e:
	db 08Bh,0CAh
g5f10:
	db 08Bh,047h,008h
g5f13:
	db 0F7h,06Ch,008h
g5f16:
	db 0D1h,0E0h
g5f18:
	db 0D1h,0D2h
g5f1a:
	db 003h,0E8h
g5f1c:
	db 013h,0CAh
g5f1e:
	db 08Bh,047h,00Ah
g5f21:
	db 0F7h,06Ch,00Eh
g5f24:
	db 0D1h,0E0h
g5f26:
	db 0D1h,0D2h
g5f28:
	db 003h,0E8h
g5f2a:
	db 013h,0CAh
g5f2c:
	db 089h,04Dh,008h
g5f2f:
	db 08Bh,047h,006h
g5f32:
	db 0F7h,06Ch,004h
g5f35:
	db 0D1h,0E0h
g5f37:
	db 0D1h,0D2h
g5f39:
	db 08Bh,0E8h
g5f3b:
	db 08Bh,0CAh
g5f3d:
	db 08Bh,047h,008h
g5f40:
	db 0F7h,06Ch,00Ah
g5f43:
	db 0D1h,0E0h
g5f45:
	db 0D1h,0D2h
g5f47:
	db 003h,0E8h
g5f49:
	db 013h,0CAh
g5f4b:
	db 08Bh,047h,00Ah
g5f4e:
	db 0F7h,06Ch,010h
g5f51:
	db 0D1h,0E0h
g5f53:
	db 0D1h,0D2h
g5f55:
	db 003h,0E8h
g5f57:
	db 013h,0CAh
g5f59:
	db 089h,04Dh,00Ah
g5f5c:
	db 08Bh,047h,00Ch
g5f5f:
	db 0F7h,02Ch
g5f61:
	db 0D1h,0E0h
g5f63:
	db 0D1h,0D2h
g5f65:
	db 08Bh,0E8h
g5f67:
	db 08Bh,0CAh
g5f69:
	db 08Bh,047h,00Eh
g5f6c:
	db 0F7h,06Ch,006h
g5f6f:
	db 0D1h,0E0h
g5f71:
	db 0D1h,0D2h
g5f73:
	db 003h,0E8h
g5f75:
	db 013h,0CAh
g5f77:
	db 08Bh,047h,010h
g5f7a:
	db 0F7h,06Ch,00Ch
g5f7d:
	db 0D1h,0E0h
g5f7f:
	db 0D1h,0D2h
g5f81:
	db 003h,0E8h
g5f83:
	db 013h,0CAh
g5f85:
	db 089h,04Dh,00Ch
g5f88:
	db 08Bh,047h,00Ch
g5f8b:
	db 0F7h,06Ch,002h
g5f8e:
	db 0D1h,0E0h
g5f90:
	db 0D1h,0D2h
g5f92:
	db 08Bh,0E8h
g5f94:
	db 08Bh,0CAh
g5f96:
	db 08Bh,047h,00Eh
g5f99:
	db 0F7h,06Ch,008h
g5f9c:
	db 0D1h,0E0h
g5f9e:
	db 0D1h,0D2h
g5fa0:
	db 003h,0E8h
g5fa2:
	db 013h,0CAh
g5fa4:
	db 08Bh,047h,010h
g5fa7:
	db 0F7h,06Ch,00Eh
g5faa:
	db 0D1h,0E0h
g5fac:
	db 0D1h,0D2h
g5fae:
	db 003h,0E8h
g5fb0:
	db 013h,0CAh
g5fb2:
	db 089h,04Dh,00Eh
g5fb5:
	db 08Bh,047h,00Ch
g5fb8:
	db 0F7h,06Ch,004h
g5fbb:
	db 0D1h,0E0h
g5fbd:
	db 0D1h,0D2h
g5fbf:
	db 08Bh,0E8h
g5fc1:
	db 08Bh,0CAh
g5fc3:
	db 08Bh,047h,00Eh
g5fc6:
	db 0F7h,06Ch,00Ah
g5fc9:
	db 0D1h,0E0h
g5fcb:
	db 0D1h,0D2h
g5fcd:
	db 003h,0E8h
g5fcf:
	db 013h,0CAh
g5fd1:
	db 08Bh,047h,010h
g5fd4:
	db 0F7h,06Ch,010h
g5fd7:
	db 0D1h,0E0h
g5fd9:
	db 0D1h,0D2h
g5fdb:
	db 003h,0E8h
g5fdd:
	db 013h,0CAh
g5fdf:
	db 089h,04Dh,010h
g5fe2:
	db 0CBh
	db 000h
g5fe4:
	cmp byte ptr [dg_drawn],00h
g5fe9:
	db 075h,00Ch
g5feb:
	db 02eh,08fh,006h ; pop word [cs:0x315]
	dw offset cs_j1 ;pop word [cs:0x315]
g5ff0:
	db 0EBh,000h
g5ff2:
	db 2Eh,0FFh,2Eh
cs_j1 LABEL BYTE
	dw 0
g5ff7:
	mov byte ptr [dg_lock],01h
g5ffc:
	db 02eh,08fh,006h ; pop word [cs:0x32e]
	dw offset cs_j2 ;pop word [cs:0x32e]
g6001:
	pop word ptr [dg_ret0]
g6005:
	pop word ptr [dg_ret2]
g6009:
	db 0EBh,000h
g600b:
	db 2Eh,0FFh,1Eh
cs_j2 LABEL BYTE
	dw 0
g6010:
	dec byte ptr [dg_lock]
g6014:
	db 074h,005h
g6016:
	db 0E8h,009h,00Bh
g6019:
	db 0EBh,0F5h
g601b:
	jmp dword ptr [dg_ret0]
g601f:
	db 000h,000h
PUBLIC _AllocGraphicPage
_AllocGraphicPage LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _AvailSysMem
_AvailSysMem LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _BuildSprite
_BuildSprite LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _ChangeColor
_ChangeColor LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _CharWidth
_CharWidth LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _ClearPage
_ClearPage LABEL FAR
	db 0EAh,000h,000h,000h,000h
PUBLIC _ClipSprite
_ClipSprite LABEL FAR
	db 0E8h,098h,0FFh
g604c:
	db 000h,000h
g604e:
	db 000h,000h
g6050:
	db 000h,000h
g6052:
	db 0EAh,000h,000h,000h,000h
PUBLIC _CloseSprite
_CloseSprite LABEL FAR
	db 000h,000h,0eah,000h,000h,000h,000h
g605e:
	db 0e8h,083h,0ffh,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
g606e:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6075:
	db 0EAh,000h,000h,000h,000h
PUBLIC _FontHeight
_FontHeight LABEL FAR
	db 000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h
g6088:
	db 0E8h,059h,0FFh
g608b:
	db 000h,000h
g608d:
	db 000h,000h
g608f:
	db 000h,000h
g6091:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6098:
	db 0EAh,000h,000h,000h,000h
PUBLIC _LIBCANDO
_LIBCANDO LABEL FAR
	db 000h,000h
g609f:
	db 0EAh,000h,000h,000h,000h
PUBLIC _OpenSprite
_OpenSprite LABEL FAR
	db 000h,000h
g60a6:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g60ad:
	db 0EAh,000h,000h,000h,000h
PUBLIC _RectCopy
_RectCopy LABEL FAR
	db 000h,000h
g60b4:
	db 0EAh,000h,000h,000h,000h
PUBLIC _RpPrint
_RpPrint LABEL FAR
	db 0e8h,028h,0ffh,000h,000h,000h,000h,000h,000h
g60c2:
	db 0EAh,000h,000h,000h,000h
PUBLIC _ScreenOff
_ScreenOff LABEL FAR
	db 000h,000h
g60c9:
	db 0EAh,000h,000h,000h,000h
PUBLIC _ScreenOn
_ScreenOn LABEL FAR
	db 000h,000h
g60d0:
	db 0EAh,000h,000h,000h,000h
PUBLIC _SetGraphicPage
_SetGraphicPage LABEL FAR
	db 000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
g60e5:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _Shake
_Shake LABEL FAR
	db 0EAh,000h,000h,000h,000h
g60f1:
	db 0E8h,0F0h,0FEh
g60f4:
	db 000h,000h
g60f6:
	db 000h,000h
PUBLIC _ShowPage
_ShowPage LABEL FAR
	db 0E8h,0E9h,0FEh
g60fb:
	db 000h,000h
g60fd:
	db 000h,000h
g60ff:
	db 0E8h,0E2h,0FEh
g6102:
	db 000h,000h
g6104:
	db 000h,000h
g6106:
	db 000h,000h
g6108:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g610f:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6116:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h,0eah,000h,000h,000h,000h,000h,000h
PUBLIC _TurnOnGraphicsMode
_TurnOnGraphicsMode LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g612b:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6132:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6139:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6140:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6147:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
	db 0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h
	db 000h,000h,000h,000h,000h
g6171:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
g6178:
	db 0EAh,000h,000h,000h,000h
	db 000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h
	db 000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah
	db 000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h
	db 000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h
	db 000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
PUBLIC _SndSysSetup
_SndSysSetup LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _SndSounds
_SndSounds LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h
PUBLIC _SndShutdown
_SndShutdown LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
PUBLIC _GETTIME
_GETTIME LABEL FAR
	db 0EAh,000h,000h,000h,000h
	db 000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
	db 0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h
	db 000h,000h,000h
PUBLIC _LoadOverlay
_LoadOverlay LABEL FAR
	db 055h
g6329:
	db 08Bh,0ECh
g632b:
	db 0B4h,048h
g632d:
	db 0BBh,0FFh,0FFh
g6330:
	db 0CDh,021h
g6332:
	db 072h,007h
g6334:
	lea si,[mps_e1]
g6338:
	db 0E9h,052h,001h
g633b:
	db 0B4h,048h
g633d:
	db 081h,0EBh,000h,001h
g6341:
	mov [mps_freep],bx
g6345:
	db 0CDh,021h
g6347:
	db 073h,007h
g6349:
	lea si,[mps_e2]
g634d:
	db 0E9h,03Dh,001h
g6350:
	db 01Eh
g6351:
	db 007h
g6352:
	lea bx,[mps_exec0]
g6356:
	db 089h,007h
g6358:
	db 089h,047h,002h
g635b:
	db 0B8h,003h,04Bh
g635e:
	db 08Bh,056h,006h
g6361:
	db 0E8h,008h,001h
g6364:
	db 073h,021h
g6366:
	db 03Dh,002h,000h
g6369:
	db 075h,007h
g636b:
	lea si,[mps_e3]
g636f:
	db 0E9h,01Bh,001h
g6372:
	db 03Dh,008h,000h
g6375:
	db 075h,007h
g6377:
	lea si,[mps_e4]
g637b:
	db 0E9h,00Fh,001h
g637e:
	db 074h,007h
g6380:
	lea si,[mps_e5]
g6384:
	db 0E9h,006h,001h
g6387:
	mov es,[mps_exec0]
g638b:
	db 026h,08Bh,01Eh,02Ah,000h
g6390:
	db 026h,0A1h,02Ch,000h
g6394:
	db 005h,00Fh,000h
g6397:
	db 0D1h,0D8h
g6399:
	db 0D1h,0E8h
g639b:
	db 0D1h,0E8h
g639d:
	db 0D1h,0E8h
g639f:
	db 003h,0D8h
g63a1:
	db 08Bh,056h,008h
g63a4:
	db 00Bh,0D2h
g63a6:
	db 074h,003h
g63a8:
	db 0E8h,025h,000h
g63ab:
	db 08Ch,0C0h
g63ad:
	db 02Bh,0D8h
g63af:
	db 083h,0C3h,008h
g63b2:
	cmp bx,[mps_freep]
g63b6:
	db 076h,007h
g63b8:
	lea si,[mps_e6]
g63bc:
	db 0E9h,0CEh,000h
g63bf:
	db 0B4h,04Ah
g63c1:
	db 0CDh,021h
g63c3:
	db 073h,007h
g63c5:
	lea si,[mps_e7]
g63c9:
	db 0E9h,0C1h,000h
g63cc:
	db 08Ch,0C0h
g63ce:
	db 05Dh
g63cf:
	db 0CBh
g63d0:
	db 01Eh
g63d1:
	db 0B8h,000h,03Dh
g63d4:
	db 0CDh,021h
g63d6:
	db 073h,007h
g63d8:
	lea si,[mps_e3]
g63dc:
	db 0E9h,0AEh,000h
g63df:
	db 08Eh,0DBh
g63e1:
	db 08Bh,0D8h
g63e3:
	db 0B4h,03Fh
g63e5:
	db 033h,0D2h
g63e7:
	db 0B9h,000h,080h
g63ea:
	db 0CDh,021h
g63ec:
	db 073h,007h
g63ee:
	lea si,[mps_e3]
g63f2:
	db 0E9h,098h,000h
g63f5:
	db 03Bh,0C1h
g63f7:
	db 072h,009h
g63f9:
	db 08Ch,0D8h
g63fb:
	db 005h,000h,008h
g63fe:
	db 08Eh,0D8h
g6400:
	db 0EBh,0E1h
g6402:
	db 005h,00Fh,000h
g6405:
	db 0D1h,0D8h
g6407:
	db 0D1h,0E8h
g6409:
	db 0D1h,0E8h
g640b:
	db 0D1h,0E8h
g640d:
	db 08Ch,0DAh
g640f:
	db 003h,0D0h
g6411:
	db 0B4h,03Eh
g6413:
	db 0CDh,021h
g6415:
	db 073h,007h
g6417:
	lea si,[mps_e3]
g641b:
	db 0EBh,070h
	db 090h
g641e:
	db 08Bh,0DAh
g6420:
	db 01Fh
g6421:
	db 0C3h
PUBLIC _InitOverlay
_InitOverlay LABEL FAR
	db 055h
g6423:
	db 08Bh,0ECh
g6425:
	db 056h
g6426:
	db 057h
g6427:
	db 01Eh
g6428:
	db 08Eh,05Eh,006h
g642b:
	db 0BEh,032h,000h
g642e:
	db 0A1h,02Eh,000h
g6431:
	db 0B2h,007h
g6433:
	db 0F6h,0E2h
g6435:
	lea di,[g601f]
g6439:
	db 003h,0F8h
g643b:
	mov ax,seg g5ce0
g643e:
	db 08Eh,0C0h
g6440:
	db 08Bh,00Eh,030h,000h
g6444:
	db 0A1h,028h,000h
g6447:
	db 083h,0C7h,003h
g644a:
	db 0A5h
g644b:
	db 0ABh
g644c:
	db 0E2h,0F9h
g644e:
	db 01Fh
g644f:
	db 05Fh
g6450:
	db 05Eh
g6451:
	db 05Dh
g6452:
	db 0CBh
g6453:
	db 055h
g6454:
	db 08Bh,0ECh
g6456:
	db 08Eh,046h,006h
g6459:
	db 0B4h,049h
g645b:
	db 0CDh,021h
g645d:
	db 073h,007h
g645f:
	lea si,[mps_e8]
g6463:
	db 0EBh,028h
	db 090h
g6466:
	db 05Dh
g6467:
	db 0CBh
cs_sp LABEL BYTE
	db 000h,000h
cs_ss LABEL BYTE
	db 000h,000h
g646c:
	db 055h
g646d:
	db 056h
g646e:
	db 057h
g646f:
	db 01Eh
g6470:
	db 006h
g6471:
	db 02eh,08ch,016h ; mov [cs:0x78a],ss
	dw offset cs_ss ;mov [cs:0x78a],ss
g6476:
	db 02eh,089h,026h ; mov [cs:0x788],sp
	dw offset cs_sp ;mov [cs:0x788],sp
g647b:
	db 0CDh,021h
g647d:
	db 02eh,08eh,016h ; mov ss,[cs:0x78a]
	dw offset cs_ss ;mov ss,[cs:0x78a]
g6482:
	db 02eh,08bh,026h ; mov sp,[cs:0x788]
	dw offset cs_sp ;mov sp,[cs:0x788]
g6487:
	db 007h
g6488:
	db 01Fh
g6489:
	db 05Fh
g648a:
	db 05Eh
g648b:
	db 05Dh
g648c:
	db 0C3h
g648d:
	db 0B4h,002h
g648f:
	db 03Ch,00Ah
g6491:
	db 01Ch,069h
g6493:
	db 02Fh
g6494:
	db 08Ah,0D0h
g6496:
	db 0CDh,021h
g6498:
	lea dx,[mps_dash]
g649c:
	db 0B4h,009h
g649e:
	db 0CDh,021h
g64a0:
	db 0B4h,009h
g64a2:
	db 08Bh,0D6h
g64a4:
	db 0CDh,021h
g64a6:
	db 0B8h,099h,04Ch
g64a9:
	db 0CDh,021h
g64ab:
	db 000h
PUBLIC _RectFill
_RectFill LABEL FAR
	db 055h
g64ad:
	db 08Bh,0ECh
g64af:
	db 057h
g64b0:
	db 056h
g64b1:
	db 033h,0C0h
g64b3:
	db 039h,046h,00Ch
g64b6:
	db 07Eh,067h
g64b8:
	db 039h,046h,00Eh
g64bb:
	db 07Eh,062h
g64bd:
	db 0E8h,080h,000h
g64c0:
	db 08Bh,05Eh,006h
g64c3:
	db 08Bh,007h
g64c5:
	call far ptr g6171
g64ca:
	db 08Bh,076h,00Ah
g64cd:
	db 003h,077h,004h
g64d0:
	mov [mps_penx],si
g64d4:
	db 08Bh,04Eh,00Eh
g64d7:
	db 0D1h,0E6h
g64d9:
	lea di,[mps_span0]
g64dd:
	db 003h,0FEh
g64df:
	db 08Bh,046h,008h
g64e2:
	db 003h,047h,002h
g64e5:
	db 08Bh,0D1h
g64e7:
	db 0F3h,0ABh
g64e9:
	db 08Bh,0CAh
g64eb:
	lea di,[mps_span1]
g64ef:
	db 003h,0FEh
g64f1:
	db 08Bh,046h,008h
g64f4:
	db 003h,047h,002h
g64f7:
	db 003h,046h,00Ch
g64fa:
	db 048h
g64fb:
	db 0F3h,0ABh
g64fd:
	db 08Bh,046h,010h
g6500:
	call far ptr g6140
g6505:
	mov ax,[mps_penx]
g6508:
	db 08Bh,0C8h
g650a:
	db 003h,04Eh,00Eh
g650d:
	db 049h
g650e:
	mov [mps_peny],cx
g6512:
	db 0BBh,040h,057h
g6515:
	call far ptr g6132
g651a:
	call far ptr g6139
g651f:
	db 05Eh
g6520:
	db 05Fh
g6521:
	db 05Dh
g6522:
	db 0CBh
	db 000h,057h,056h,055h,0a1h,064h,05ah,08bh,00eh,066h,05ah,0bbh,040h,057h,09ah,052h
	db 004h,0ceh,005h,05dh,05eh,05fh,0cbh,057h,0e8h,002h,000h,05fh,0cbh
g6540:
	mov ax,DGROUP
g6543:
	db 08Eh,0C0h
g6545:
	mov di,[mps_penx]
g6549:
	db 00Bh,0FFh
g654b:
	db 078h,028h
g654d:
	mov cx,[mps_peny]
g6551:
	db 041h
g6552:
	db 02Bh,0CFh
g6554:
	db 0D1h,0E7h
g6556:
	db 08Bh,0D9h
g6558:
	db 08Bh,0D7h
g655a:
	db 081h,0C7h,040h,057h
g655e:
	db 0B8h,0FFh,0FFh
g6561:
	db 0F3h,0ABh
g6563:
	mov [mps_penx],ax
g6566:
	db 08Bh,0CBh
g6568:
	db 08Bh,0FAh
g656a:
	db 081h,0C7h,0D0h,058h
g656e:
	db 02Bh,0C0h
g6570:
	db 0F3h,0ABh
g6572:
	mov [mps_peny],ax
g6575:
	db 0C3h
g6576:
	mov ax,DGROUP
g6579:
	db 08Eh,0C0h
g657b:
	db 08Bh,0CFh
g657d:
	db 00Bh,0F6h
g657f:
	lea di,[mps_span0]
g6583:
	db 074h,00Ah
g6585:
	cmp si,[mps_clipR]
g6589:
	db 075h,045h
g658b:
	lea di,[mps_span1]
g658f:
	db 03Bh,0CAh
g6591:
	db 07Dh,002h
g6593:
	db 087h,0CAh
g6595:
	db 00Bh,0D2h
g6597:
	db 079h,002h
g6599:
	db 02Bh,0D2h
g659b:
	cmp cx,[mps_clipB]
g659f:
	db 07Eh,004h
g65a1:
	mov cx,[mps_clipB]
g65a5:
	add si,[mps_clipL]
g65a9:
	add cx,[mps_clipT]
g65ad:
	add dx,[mps_clipT]
g65b1:
	cmp cx,[mps_peny]
g65b5:
	db 076h,004h
g65b7:
	mov [mps_peny],cx
g65bb:
	cmp dx,[mps_penx]
g65bf:
	db 073h,004h
g65c1:
	mov [mps_penx],dx
g65c5:
	db 003h,0FAh
g65c7:
	db 003h,0FAh
g65c9:
	db 02Bh,0CAh
g65cb:
	db 041h
g65cc:
	db 08Bh,0C6h
g65ce:
	db 0F3h,0ABh
g65d0:
	db 0C3h
g65d1:
	db 02Bh,0F6h
g65d3:
	db 00Bh,0DBh
g65d5:
	db 078h,09Fh
g65d7:
	mov si,[mps_clipR]
g65db:
	db 03Bh,0DEh
g65dd:
	db 07Fh,097h
g65df:
	db 0C3h
g65e0:
	mov ax,[mps_clipL]
g65e3:
	add [mps_x1],ax
g65e7:
	add [mps_x2],ax
g65eb:
	mov ax,[mps_clipT]
g65ee:
	add [mps_y1],ax
g65f2:
	add [mps_y2],ax
g65f6:
	mov ax,[mps_x2]
g65f9:
	cmp ax,[mps_x1]
g65fd:
	db 073h,019h
g65ff:
	mov bx,[mps_x1]
g6603:
	mov [mps_x2],bx
g6607:
	mov [mps_x1],ax
g660a:
	mov ax,[mps_y2]
g660d:
	mov bx,[mps_y1]
g6611:
	mov [mps_y2],bx
g6615:
	mov [mps_y1],ax
g6618:
	mov bx,[mps_x2]
g661c:
	sub bx,[mps_x1]
g6620:
	mov [ln_tmp0],bx
g6624:
	mov ax,[mps_y2]
g6627:
	db 08Bh,0D0h
g6629:
	mov cx,[mps_y1]
g662d:
	db 02Bh,0C1h
g662f:
	db 0BDh,002h,000h
g6632:
	db 079h,006h
g6634:
	db 087h,0CAh
g6636:
	db 0F7h,0DDh
g6638:
	db 0F7h,0D8h
g663a:
	mov [ln_tmp1],ax
g663d:
	cmp dx,[mps_peny]
g6641:
	db 076h,004h
g6643:
	mov [mps_peny],dx
g6647:
	cmp cx,[mps_penx]
g664b:
	db 073h,004h
g664d:
	mov [mps_penx],cx
g6651:
	db 03Bh,0C3h
g6653:
	db 073h,04Bh
g6655:
	mov di,[mps_y1]
g6659:
	db 0D1h,0E7h
g665b:
	mov ax,[mps_x1]
g665e:
	mov dx,[ln_tmp0]
g6662:
	db 08Bh,0CAh
g6664:
	db 08Bh,0D9h
g6666:
	db 043h
g6667:
	db 0D1h,0EBh
g6669:
	db 0F7h,0DBh
g666b:
	mov si,[ln_tmp1]
g666f:
	db 0EBh,010h
	db 090h
g6672:
	db 03Bh,085h,0D0h,058h
g6676:
	db 076h,004h
g6678:
	db 089h,085h,0D0h,058h
g667c:
	db 040h
g667d:
	db 02Bh,0DAh
g667f:
	db 003h,0FDh
g6681:
	db 03Bh,085h,040h,057h
g6685:
	db 073h,004h
g6687:
	db 089h,085h,040h,057h
g668b:
	db 049h
g668c:
	db 078h,007h
g668e:
	db 003h,0DEh
g6690:
	db 079h,0E0h
g6692:
	db 040h
g6693:
	db 0EBh,0F6h
g6695:
	db 03Bh,085h,0D0h,058h
g6699:
	db 076h,004h
g669b:
	db 089h,085h,0D0h,058h
g669f:
	db 0C3h
g66a0:
	mov di,[mps_y1]
g66a4:
	db 0D1h,0E7h
g66a6:
	mov ax,[mps_x1]
g66a9:
	mov dx,[ln_tmp1]
g66ad:
	db 08Bh,0CAh
g66af:
	db 08Bh,0D9h
g66b1:
	db 043h
g66b2:
	db 0D1h,0EBh
g66b4:
	db 0F7h,0DBh
g66b6:
	mov si,[ln_tmp0]
g66ba:
	db 03Bh,085h,040h,057h
g66be:
	db 073h,004h
g66c0:
	db 089h,085h,040h,057h
g66c4:
	db 03Bh,085h,0D0h,058h
g66c8:
	db 076h,004h
g66ca:
	db 089h,085h,0D0h,058h
g66ce:
	db 049h
g66cf:
	db 078h,00Bh
g66d1:
	db 003h,0FDh
g66d3:
	db 003h,0DEh
g66d5:
	db 078h,0E3h
g66d7:
	db 02Bh,0DAh
g66d9:
	db 040h
g66da:
	db 0EBh,0DEh
g66dc:
	db 0C3h
	db 000h,0e8h,00dh,000h,0cbh,055h,056h,057h,006h,0e8h,005h,000h,007h,05fh,05eh,05dh
	db 0cbh
g66ee:
	db 02Bh,0C0h
g66f0:
	db 08Eh,0C0h
g66f2:
	db 026h,0FFh,036h,000h,000h
g66f7:
	db 026h,0FFh,036h,002h,000h
g66fc:
	db 08Dh,006h,0BBh,00Bh
g6700:
	db 026h,0A3h,000h,000h
g6704:
	mov word ptr es:[2],seg g5ce0
g670b:
	db 01Eh
g670c:
	db 007h
g670d:
	db 0EBh,067h
	db 090h
g6710:
	db 02Bh,0C0h
g6712:
	db 08Eh,0C0h
g6714:
	db 026h,08Fh,006h,002h,000h
g6719:
	db 026h,08Fh,006h,000h,000h
g671e:
	db 01Eh
g671f:
	db 007h
g6720:
	mov ax,[mps_x1]
g6723:
	mov bx,[mps_y1]
g6727:
	mov cx,[mps_x2]
g672b:
	mov dx,[mps_y2]
g672f:
	call far ptr g6147
g6734:
	db 0F8h
g6735:
	db 0C3h
g6736:
	db 02Bh,0C0h
g6738:
	db 08Eh,0C0h
g673a:
	db 026h,08Fh,006h,002h,000h
g673f:
	db 026h,08Fh,006h,000h,000h
g6744:
	db 01Eh
g6745:
	db 007h
g6746:
	db 0F9h
g6747:
	db 0C3h
g6748:
	db 0F5h
g6749:
	db 0D1h,0DAh
g674b:
	mov [eg_dx],dx
g674f:
	db 0D1h,0FAh
g6751:
	mov [eg_cx],dx
g6755:
	db 08Bh,0D7h
g6757:
	db 02Bh,0D5h
g6759:
	db 071h,006h
g675b:
	db 0F5h
g675c:
	db 0D1h,0DAh
g675e:
	db 0EBh,071h
	db 090h
g6761:
	db 0D1h,0FAh
g6763:
	db 0EBh,06Ch
	db 090h
g6766:
	db 0F5h
g6767:
	db 0D1h,0DAh
g6769:
	sar word ptr [eg_dx],1
g676d:
	sar word ptr [eg_cx],1
g6771:
	db 0EBh,05Eh
	db 090h
g6774:
	db 0EBh,0C0h
g6776:
	mov cx,[mps_x1]
g677a:
	mov dx,[mps_y1]
g677e:
	mov si,[mps_x2]
g6782:
	mov di,[mps_y2]
g6786:
	db 08Bh,0D9h
g6788:
	db 08Bh,0EAh
g678a:
	db 0E8h,0EDh,000h
g678d:
	mov byte ptr [eg_flag],al
g6790:
	db 08Bh,0DEh
g6792:
	db 08Bh,0EFh
g6794:
	db 0E8h,0E3h,000h
g6797:
	db 075h,01Ah
g6799:
	cmp byte ptr [eg_flag],00h
g679e:
	db 075h,003h
g67a0:
	db 0E9h,06Dh,0FFh
g67a3:
	db 087h,0CEh
g67a5:
	db 087h,0D7h
g67a7:
	xchg al,[eg_flag]
g67ab:
	mov [mps_x1],cx
g67af:
	mov [mps_y1],dx
g67b3:
	test byte ptr [eg_flag],al
g67b7:
	db 075h,0BBh
g67b9:
	db 08Bh,0EAh
g67bb:
	db 08Bh,0D6h
g67bd:
	db 02Bh,0D1h
g67bf:
	db 070h,087h
g67c1:
	mov [eg_dx],dx
g67c5:
	db 0D1h,0FAh
g67c7:
	mov [eg_cx],dx
g67cb:
	db 08Bh,0D7h
g67cd:
	db 02Bh,0D5h
g67cf:
	db 070h,095h
g67d1:
	mov [eg_dy],dx
g67d5:
	db 0D1h,0FAh
g67d7:
	mov [eg_cy],dx
g67db:
	db 0A8h,009h
g67dd:
	db 074h,038h
g67df:
	db 02Bh,0DBh
g67e1:
	db 00Bh,0F6h
g67e3:
	db 078h,004h
g67e5:
	mov bx,[mps_clipR]
g67e9:
	db 08Bh,0C3h
g67eb:
	db 02Bh,0C1h
g67ed:
	imul word ptr [eg_dy]
g67f1:
	db 053h
g67f2:
	db 08Bh,0DAh
g67f4:
	idiv word ptr [eg_dx]
g67f8:
	db 08Ah,0DFh
g67fa:
	xor bl,[eg_s0]
g67fe:
	db 079h,003h
g6800:
	db 0F7h,0DAh
g6802:
	db 048h
g6803:
	sub dx,[eg_cx]
g6807:
	db 032h,0F7h
g6809:
	db 078h,001h
g680b:
	db 040h
g680c:
	db 05Bh
g680d:
	db 003h,0C5h
g680f:
	db 078h,00Eh
g6811:
	cmp ax,[mps_clipB]
g6815:
	db 07Eh,039h
g6817:
	mov bx,[mps_clipB]
g681b:
	db 03Bh,0FBh
g681d:
	db 07Fh,002h
g681f:
	db 02Bh,0DBh
g6821:
	db 08Bh,0C3h
g6823:
	db 02Bh,0C5h
g6825:
	imul word ptr [eg_dx]
g6829:
	db 053h
g682a:
	db 08Bh,0DAh
g682c:
	idiv word ptr [eg_dy]
g6830:
	db 08Ah,0DFh
g6832:
	xor bl,[eg_s1]
g6836:
	db 079h,003h
g6838:
	db 0F7h,0DAh
g683a:
	db 048h
g683b:
	sub dx,[eg_cy]
g683f:
	db 032h,0F7h
g6841:
	db 078h,001h
g6843:
	db 040h
g6844:
	db 05Bh
g6845:
	db 003h,0C1h
g6847:
	db 078h,018h
g6849:
	cmp ax,[mps_clipR]
g684d:
	db 07Fh,012h
g684f:
	db 093h
g6850:
	cmp byte ptr [eg_flag],00h
g6855:
	db 075h,00Dh
g6857:
	mov [mps_y2],ax
g685a:
	mov [mps_x2],bx
g685e:
	db 0E9h,0AFh,0FEh
g6861:
	db 0E9h,0D2h,0FEh
g6864:
	mov [mps_y1],ax
g6867:
	mov [mps_x1],bx
g686b:
	db 087h,0CEh
g686d:
	db 087h,0EFh
g686f:
	mov al,[eg_flag]
g6872:
	mov byte ptr [eg_flag],00h
g6877:
	db 0E9h,061h,0FFh
g687a:
	db 0B0h,00Fh
g687c:
	db 00Bh,0DBh
g687e:
	db 078h,002h
g6880:
	db 024h,0F7h
g6882:
	cmp bx,[mps_clipR]
g6886:
	db 07Fh,002h
g6888:
	db 024h,0FEh
g688a:
	db 00Bh,0EDh
g688c:
	db 078h,002h
g688e:
	db 024h,0FBh
g6890:
	cmp bp,[mps_clipB]
g6894:
	db 07Fh,002h
g6896:
	db 024h,0FDh
g6898:
	db 00Ah,0C0h
g689a:
	db 0C3h
g689b:
	db 055h
g689c:
	db 08Bh,0ECh
g689e:
	db 081h,07Eh,002h,04Ch,00Bh
g68a3:
	mov ax,[eg_dy]
g68a6:
	db 074h,003h
g68a8:
	mov ax,[eg_dx]
g68ab:
	db 033h,0D0h
g68ad:
	db 0B8h,000h,07Fh
g68b0:
	db 079h,002h
g68b2:
	db 0F7h,0D8h
g68b4:
	db 083h,046h,002h,004h
g68b8:
	db 02Bh,0D2h
g68ba:
	db 05Dh
g68bb:
	db 0CFh
PUBLIC _DrawLine
_DrawLine LABEL FAR
	db 055h
g68bd:
	db 08Bh,0ECh
g68bf:
	db 056h
g68c0:
	db 057h
g68c1:
	db 08Bh,05Eh,006h
g68c4:
	db 08Bh,047h,006h
g68c7:
	mov [mps_clipR],ax
g68ca:
	db 08Bh,047h,008h
g68cd:
	mov [mps_clipB],ax
g68d0:
	call far ptr g6178
g68d5:
	db 08Bh,046h,008h
g68d8:
	mov [mps_x1],ax
g68db:
	db 08Bh,046h,00Ah
g68de:
	mov [mps_y1],ax
g68e1:
	db 08Bh,046h,00Ch
g68e4:
	mov [mps_x2],ax
g68e7:
	db 08Bh,046h,00Eh
g68ea:
	mov [mps_y2],ax
g68ed:
	db 08Bh,046h,010h
g68f0:
	call far ptr g6140
g68f5:
	db 0E8h,0F6h,0FDh
g68f8:
	call far ptr g6139
g68fd:
	db 05Fh
g68fe:
	db 05Eh
g68ff:
	db 05Dh
g6900:
	db 0CBh
PUBLIC _DrawDot
_DrawDot LABEL FAR
	db 055h
g6902:
	db 08Bh,0ECh
g6904:
	db 056h
g6905:
	db 057h
g6906:
	db 08Bh,05Eh,006h
g6909:
	db 08Bh,04Eh,008h
g690c:
	db 00Bh,0C9h
g690e:
	db 078h,02Ch
g6910:
	db 03Bh,04Fh,006h
g6913:
	db 07Fh,027h
g6915:
	db 08Bh,056h,00Ah
g6918:
	db 00Bh,0D2h
g691a:
	db 078h,020h
g691c:
	db 03Bh,057h,008h
g691f:
	db 07Fh,01Bh
g6921:
	call far ptr g6178
g6926:
	db 08Bh,046h,00Ch
g6929:
	call far ptr g6140
g692e:
	db 08Bh,0C1h
g6930:
	db 08Bh,0DAh
g6932:
	call far ptr g612b
g6937:
	call far ptr g6139
g693c:
	db 05Fh
g693d:
	db 05Eh
g693e:
	db 05Dh
g693f:
	db 0CBh
PUBLIC _DrawPoly
_DrawPoly LABEL FAR
	db 055h
g6941:
	db 08Bh,0ECh
g6943:
	db 006h
g6944:
	db 01Eh
g6945:
	db 056h
g6946:
	db 057h
g6947:
	db 08Bh,046h,00Ch
g694a:
	call far ptr g6140
g694f:
	db 08Bh,05Eh,006h
g6952:
	call far ptr g6178
g6957:
	db 08Bh,047h,002h
g695a:
	mov [mps_clipL],ax
g695d:
	db 08Bh,047h,004h
g6960:
	mov [mps_clipT],ax
g6963:
	db 08Bh,047h,006h
g6966:
	mov [mps_clipR],ax
g6969:
	db 08Bh,047h,008h
g696c:
	mov [mps_clipB],ax
g696f:
	db 0E8h,0CEh,0FBh
g6972:
	db 08Bh,076h,00Ah
g6975:
	db 08Bh,04Eh,008h
g6978:
	db 049h
g6979:
	db 0ADh
g697a:
	mov [mps_x1],ax
g697d:
	db 050h
g697e:
	db 0ADh
g697f:
	mov [mps_y1],ax
g6982:
	db 050h
g6983:
	db 051h
g6984:
	db 0ADh
g6985:
	mov [mps_x2],ax
g6988:
	db 050h
g6989:
	db 0ADh
g698a:
	mov [mps_y2],ax
g698d:
	db 050h
g698e:
	db 056h
g698f:
	db 0E8h,02Ch,002h
g6992:
	db 05Eh
g6993:
	pop word ptr [mps_y1]
g6997:
	pop word ptr [mps_x1]
g699b:
	db 059h
g699c:
	db 0E2h,0E5h
g699e:
	pop word ptr [mps_y2]
g69a2:
	pop word ptr [mps_x2]
g69a6:
	db 0E8h,015h,002h
g69a9:
	mov ax,[mps_penx]
g69ac:
	mov cx,[mps_peny]
g69b0:
	db 0BBh,040h,057h
g69b3:
	call far ptr g6132
g69b8:
	call far ptr g6139
g69bd:
	db 05Fh
g69be:
	db 05Eh
g69bf:
	db 01Fh
g69c0:
	db 007h
g69c1:
	db 05Dh
g69c2:
	db 0CBh
	db 000h
PUBLIC _MouseON
_MouseON LABEL FAR
	db 033h,0C0h
g69c6:
	db 08Eh,0C0h
g69c8:
	db 026h,0A1h,0CCh,000h
g69cc:
	db 026h,00Bh,006h,0CEh,000h
g69d1:
	db 074h,036h
g69d3:
	db 0B8h,000h,000h
g69d6:
	db 0CDh,033h
g69d8:
	db 00Bh,0C0h
g69da:
	db 074h,02Dh
g69dc:
	db 0B8h,00Ch,000h
g69df:
	db 0B9h,01Fh,000h
g69e2:
	db 08Ch,0CAh
g69e4:
	db 08Eh,0C2h
g69e6:
	db 08Dh,016h,083h,00Eh
g69ea:
	db 0CDh,033h
g69ec:
	db 0B8h,003h,000h
g69ef:
	db 0CDh,033h
g69f1:
	db 08Bh,0C1h
g69f3:
	db 08Ah,0CDh
g69f5:
	db 0D3h,0E8h
g69f7:
	mov byte ptr [mps_xsh],cl
g69fb:
	mov [_MouseX],ax
g69fe:
	mov [_MouseY],dx
g6a02:
	mov [_Button],bx
g6a06:
	db 0B8h,0FFh,0FFh
g6a09:
	mov byte ptr [_MOUSE],al
g6a0c:
	db 0CBh
PUBLIC _MouseOFF
_MouseOFF LABEL FAR
	db 032h,0C0h
g6a0f:
	xchg al,[_MOUSE]
g6a13:
	db 00Ah,0C0h
g6a15:
	db 074h,005h
g6a17:
	db 0B8h,000h,000h
g6a1a:
	db 0CDh,033h
g6a1c:
	db 0CBh
PUBLIC _MouseTYPE
_MouseTYPE LABEL FAR
	db 055h
g6a1e:
	db 08Bh,0ECh
g6a20:
	db 08Bh,046h,006h
g6a23:
	mov [mps_hotx],ax
g6a26:
	db 08Bh,046h,008h
g6a29:
	mov [mps_hoty],ax
g6a2c:
	db 08Bh,046h,00Ah
g6a2f:
	mov [mps_curs],ax
g6a32:
	db 05Dh
g6a33:
	db 0CBh
PUBLIC _MouseSPOT
_MouseSPOT LABEL FAR
	db 055h
g6a35:
	db 08Bh,0ECh
g6a37:
	db 08Bh,046h,006h
g6a3a:
	db 08Bh,056h,008h
g6a3d:
	mov [_MouseX],ax
g6a40:
	mov [_MouseY],dx
g6a44:
	cmp byte ptr [_MOUSE],00h
g6a49:
	db 074h,00Dh
g6a4b:
	mov cl,[mps_xsh]
	db 0d3h,0e0h,08bh,0c8h,0b8h,004h,000h,0cdh,033h
g6a58:
	db 05Dh
g6a59:
	db 0CBh
PUBLIC _MouseHITS
_MouseHITS LABEL FAR
	db 033h,0C0h
g6a5c:
	xchg ax,[mps_hits]
g6a60:
	db 0CBh
PUBLIC _MouseSHOW
_MouseSHOW LABEL FAR
	cmp word ptr [mps_curs],+00h
g6a66:
	db 074h,028h
g6a68:
	cmp byte ptr [dg_drawn],00h
g6a6d:
	db 075h,021h
g6a6f:
	push word ptr [mps_curs]
g6a73:
	mov ax,[_MouseY]
g6a76:
	sub ax,[mps_hoty]
g6a7a:
	db 050h
g6a7b:
	mov ax,[_MouseX]
g6a7e:
	sub ax,[mps_hotx]
g6a82:
	db 050h
g6a83:
	call far ptr g60f1
g6a88:
	db 083h,0C4h,006h
g6a8b:
	mov byte ptr [dg_drawn],01h
g6a90:
	db 0CBh
PUBLIC _MouseHIDE
_MouseHIDE LABEL FAR
	cmp word ptr [mps_curs],+00h
g6a96:
	db 074h,011h
g6a98:
	cmp byte ptr [dg_drawn],00h
g6a9d:
	db 074h,00Ah
g6a9f:
	mov byte ptr [dg_drawn],00h
g6aa4:
	call far ptr g6088
g6aa9:
	db 0CBh
cs_lock LABEL BYTE
	db 000h,055h,08bh,0ech,083h,03eh,082h,05ah,000h,074h,01bh,080h,03eh,00fh,056h,000h
	db 074h,014h,0a0h,00fh,056h,0c6h,006h,00fh,056h,000h,02eh,0a2h,0cah,00dh,08bh,046h
	db 006h,09ah,07eh,003h,0ceh,005h,05dh,0cbh,083h,03eh,082h,05ah,000h,074h,014h,02eh
	db 080h,03eh,0cah,00dh,000h,074h,00ch,09ah,0a8h,003h,0ceh,005h,02eh,0a0h,0cah,00dh
	db 0a2h,00fh,056h,0cbh,055h,08bh,0ech,08bh,046h,006h,08bh,05eh,008h,08bh,04eh,00ah
	db 08bh,056h,00ch,0cdh,033h,0a3h,08eh,05ah,089h,01eh,090h,05ah,089h,00eh,092h,05ah
	db 089h,016h,094h,05ah,083h,07eh,006h,006h,077h,008h,08ah,00eh,088h,05ah,0d3h,02eh
	db 092h,05ah,08dh,006h,08eh,05ah,05dh,0cbh
g6b22:
	db 050h
g6b23:
	db 052h
g6b24:
	db 006h
g6b25:
	mov ax,seg g5ce0
g6b28:
	db 08Eh,0C0h
g6b2a:
	db 026h,0FFh,01Eh,0ABh,003h
g6b2f:
	push word ptr [mps_curs]
g6b33:
	mov ax,[_MouseY]
g6b36:
	sub ax,[mps_hoty]
g6b3a:
	db 050h
g6b3b:
	mov ax,[_MouseX]
g6b3e:
	sub ax,[mps_hotx]
g6b42:
	db 050h
g6b43:
	mov ax,seg g5ce0
g6b46:
	db 08Eh,0C0h
g6b48:
	db 026h,0FFh,01Eh,014h,004h
g6b4d:
	db 083h,0C4h,006h
g6b50:
	db 007h
g6b51:
	db 05Ah
g6b52:
	db 058h
g6b53:
	db 0C3h
g6b54:
	db 055h
g6b55:
	db 08Bh,0ECh
g6b57:
	db 0C4h,046h,006h
g6b5a:
	mov byte ptr [mps_ucb],ax
g6b5d:
	mov byte ptr [mps_ucb+2],es
g6b61:
	db 05Dh
g6b62:
	db 0CBh
	db 0bdh,0f1h,055h,08eh,0ddh,08bh,0e9h,08ah,00eh,088h,05ah,0d3h,0edh,08bh,0cdh,009h
	db 01eh,080h,05ah,089h,01eh,07eh,05ah,089h,00eh,07ah,05ah,089h,016h,07ch,05ah,08bh
	db 02eh,08ah,05ah,00bh,02eh,08ch,05ah,08bh,0e8h,074h,004h,0ffh,01eh,08ah,05ah,0f7h
	db 0c5h,001h,000h,074h,019h,080h,03eh,00fh,056h,000h,074h,012h,0b0h,002h,086h,006h
	db 00eh,056h,00ah,0c0h,075h,008h,0e8h,076h,0ffh,0c6h,006h,00eh,056h,000h,0cbh,006h
	db 056h,057h,055h,0e8h,005h,000h,05dh,05fh,05eh,007h,0cbh
g6bbe:
	db 02Bh,0C0h
g6bc0:
	db 08Eh,0C0h
g6bc2:
	db 026h,0FFh,036h,000h,000h
g6bc7:
	db 026h,0FFh,036h,002h,000h
g6bcc:
	lea ax,[g6d66]
g6bd0:
	db 026h,0A3h,000h,000h
g6bd4:
	mov word ptr es:[2],seg g5ce0
g6bdb:
	db 01Eh
g6bdc:
	db 007h
g6bdd:
	db 0EBh,04Eh
	db 090h
g6be0:
	db 02Bh,0C0h
g6be2:
	db 08Eh,0C0h
g6be4:
	db 026h,08Fh,006h,002h,000h
g6be9:
	db 026h,08Fh,006h,000h,000h
g6bee:
	db 01Eh
g6bef:
	db 007h
g6bf0:
	db 0C3h
g6bf1:
	and al,[eg2_flag]
g6bf5:
	db 0A8h,006h
g6bf7:
	db 075h,0E7h
g6bf9:
	db 08Bh,0D9h
g6bfb:
	db 0E9h,02Ah,001h
g6bfe:
	db 0E9h,052h,001h
g6c01:
	db 0F5h
g6c02:
	db 0D1h,0DAh
g6c04:
	mov [eg2_dx],dx
g6c08:
	db 0D1h,0FAh
g6c0a:
	mov [eg2_cx],dx
g6c0e:
	db 08Bh,0D7h
g6c10:
	db 02Bh,0D5h
g6c12:
	db 071h,006h
g6c14:
	db 0F5h
g6c15:
	db 0D1h,0DAh
g6c17:
	db 0EBh,06Ch
	db 090h
g6c1a:
	db 0D1h,0FAh
g6c1c:
	db 0EBh,067h
	db 090h
g6c1f:
	db 0F5h
g6c20:
	db 0D1h,0DAh
g6c22:
	sar word ptr [eg2_dx],1
g6c26:
	sar word ptr [eg2_cx],1
g6c2a:
	db 0EBh,059h
	db 090h
g6c2d:
	mov cx,[mps_x1]
g6c31:
	mov dx,[mps_y1]
g6c35:
	mov si,[mps_x2]
g6c39:
	mov di,[mps_y2]
g6c3d:
	db 08Bh,0D9h
g6c3f:
	db 08Bh,0EAh
g6c41:
	db 0E8h,036h,0FCh
g6c44:
	mov byte ptr [eg2_flag],al
g6c47:
	db 08Bh,0DEh
g6c49:
	db 08Bh,0EFh
g6c4b:
	db 0E8h,02Ch,0FCh
g6c4e:
	db 075h,017h
g6c50:
	cmp byte ptr [eg2_flag],00h
g6c55:
	db 074h,0A7h
g6c57:
	db 087h,0CEh
g6c59:
	db 087h,0D7h
g6c5b:
	xchg al,[eg2_flag]
g6c5f:
	mov [mps_x1],cx
g6c63:
	mov [mps_y1],dx
g6c67:
	db 08Bh,0EAh
g6c69:
	test byte ptr [eg2_flag],al
g6c6d:
	db 075h,082h
g6c6f:
	db 08Bh,0D6h
g6c71:
	db 02Bh,0D1h
g6c73:
	db 070h,08Ch
g6c75:
	mov [eg2_dx],dx
g6c79:
	db 0D1h,0FAh
g6c7b:
	mov [eg2_cx],dx
g6c7f:
	db 08Bh,0D7h
g6c81:
	db 02Bh,0D5h
g6c83:
	db 070h,09Ah
g6c85:
	mov [eg2_dy],dx
g6c89:
	db 0D1h,0FAh
g6c8b:
	mov [eg2_cy],dx
g6c8f:
	db 0A8h,009h
g6c91:
	db 074h,038h
g6c93:
	db 02Bh,0DBh
g6c95:
	db 00Bh,0F6h
g6c97:
	db 078h,004h
g6c99:
	mov bx,[mps_clipR]
g6c9d:
	db 08Bh,0C3h
g6c9f:
	db 02Bh,0C1h
g6ca1:
	imul word ptr [eg2_dy]
g6ca5:
	db 053h
g6ca6:
	db 08Bh,0DAh
g6ca8:
	idiv word ptr [eg2_dx]
g6cac:
	db 08Ah,0DFh
g6cae:
	xor bl,[eg2_s0]
g6cb2:
	db 079h,003h
g6cb4:
	db 0F7h,0DAh
g6cb6:
	db 048h
g6cb7:
	sub dx,[eg2_cx]
g6cbb:
	db 032h,0F7h
g6cbd:
	db 078h,001h
g6cbf:
	db 040h
g6cc0:
	db 05Bh
g6cc1:
	db 003h,0C5h
g6cc3:
	db 078h,00Eh
g6cc5:
	cmp ax,[mps_clipB]
g6cc9:
	db 07Eh,039h
g6ccb:
	mov bx,[mps_clipB]
g6ccf:
	db 03Bh,0FBh
g6cd1:
	db 07Fh,002h
g6cd3:
	db 02Bh,0DBh
g6cd5:
	db 08Bh,0C3h
g6cd7:
	db 02Bh,0C5h
g6cd9:
	imul word ptr [eg2_dx]
g6cdd:
	db 053h
g6cde:
	db 08Bh,0DAh
g6ce0:
	idiv word ptr [eg2_dy]
g6ce4:
	db 08Ah,0DFh
g6ce6:
	xor bl,[eg2_s1]
g6cea:
	db 079h,003h
g6cec:
	db 0F7h,0DAh
g6cee:
	db 048h
g6cef:
	sub dx,[eg2_cy]
g6cf3:
	db 032h,0F7h
g6cf5:
	db 078h,001h
g6cf7:
	db 040h
g6cf8:
	db 05Bh
g6cf9:
	db 003h,0C1h
g6cfb:
	db 093h
g6cfc:
	db 078h,02Ah
g6cfe:
	cmp bx,[mps_clipR]
g6d02:
	db 07Fh,024h
g6d04:
	db 050h
g6d05:
	db 053h
g6d06:
	db 051h
g6d07:
	db 056h
g6d08:
	db 057h
g6d09:
	db 08Bh,0D7h
g6d0b:
	db 08Bh,0F8h
g6d0d:
	db 08Bh,0F3h
g6d0f:
	db 0E8h,064h,0F8h
g6d12:
	db 05Fh
g6d13:
	db 05Eh
g6d14:
	db 059h
g6d15:
	db 05Bh
g6d16:
	db 058h
g6d17:
	cmp byte ptr [eg2_flag],00h
g6d1c:
	db 075h,01Fh
g6d1e:
	mov [mps_y2],ax
g6d21:
	mov [mps_x2],bx
g6d25:
	db 0EBh,02Ch
	db 090h
g6d28:
	db 08Bh,0D5h
g6d2a:
	db 02Bh,0C0h
g6d2c:
	db 08Eh,0C0h
g6d2e:
	db 026h,08Fh,006h,002h,000h
g6d33:
	db 026h,08Fh,006h,000h,000h
g6d38:
	db 01Eh
g6d39:
	db 007h
g6d3a:
	db 0E9h,094h,0F8h
g6d3d:
	mov [mps_y1],ax
g6d40:
	mov [mps_x1],bx
g6d44:
	db 087h,0CEh
g6d46:
	db 087h,0EFh
g6d48:
	mov al,[eg2_flag]
g6d4b:
	mov byte ptr [eg2_flag],00h
g6d50:
	db 0E9h,03Ch,0FFh
g6d53:
	db 02Bh,0C0h
g6d55:
	db 08Eh,0C0h
g6d57:
	db 026h,08Fh,006h,002h,000h
g6d5c:
	db 026h,08Fh,006h,000h,000h
g6d61:
	db 01Eh
g6d62:
	db 007h
g6d63:
	db 0E9h,07Ah,0F8h
g6d66:
	db 055h,08bh,0ech,081h,07eh,002h,000h,010h,0a1h,099h,05ah,074h,003h,0a1h,097h,05ah
	db 033h,0d0h,0b8h,000h,07fh,079h,002h,0f7h,0d8h,083h,046h,002h,004h,02bh,0d2h,05dh
	db 0cfh,000h
g6d88:
	db 055h
g6d89:
	db 08Bh,0ECh
g6d8b:
	db 056h
g6d8c:
	db 057h
g6d8d:
	db 033h,0C0h
g6d8f:
	db 050h
g6d90:
	call far ptr g60e5
g6d95:
	db 083h,0C4h,002h
g6d98:
	mov si,[pic_src]
g6d9c:
	cmp si,[pic_bound]
g6da0:
	db 072h,00Eh
g6da2:
	db 053h
g6da3:
	db 051h
g6da4:
	db 052h
g6da5:
	call dword ptr [pic_pump]
g6da9:
	db 05Ah
g6daa:
	db 059h
g6dab:
	db 05Bh
g6dac:
	mov si,[pic_src]
g6db0:
	db 0ADh
g6db1:
	mov [pic_src],si
g6db5:
	db 03Ch,058h
g6db7:
	db 074h,079h
g6db9:
	db 08Ch,0DFh
g6dbb:
	db 08Eh,0C7h
g6dbd:
	lea di,[pic_dict]
g6dc1:
	db 03Dh,04Dh,030h
g6dc4:
	db 075h,011h
g6dc6:
	db 083h,07Eh,006h,001h
g6dca:
	db 072h,008h
g6dcc:
	db 074h,009h
g6dce:
	db 08Bh,07Eh,006h
g6dd1:
	db 0EBh,004h
	db 090h
g6dd4:
	db 083h,0C7h,002h
g6dd7:
	db 057h
g6dd8:
	db 0ABh
g6dd9:
	mov si,[pic_src]
g6ddd:
	cmp si,[pic_bound]
g6de1:
	db 072h,00Eh
g6de3:
	db 053h
g6de4:
	db 051h
g6de5:
	db 052h
g6de6:
	call dword ptr [pic_pump]
g6dea:
	db 05Ah
g6deb:
	db 059h
g6dec:
	db 05Bh
g6ded:
	mov si,[pic_src]
g6df1:
	db 0ADh
g6df2:
	mov [pic_src],si
g6df6:
	db 0ABh
g6df7:
	db 08Bh,0C8h
g6df9:
	db 0D1h,0E9h
g6dfb:
	db 0E3h,020h
g6dfd:
	mov si,[pic_src]
g6e01:
	cmp si,[pic_bound]
g6e05:
	db 072h,00Eh
g6e07:
	db 053h
g6e08:
	db 051h
g6e09:
	db 052h
g6e0a:
	call dword ptr [pic_pump]
g6e0e:
	db 05Ah
g6e0f:
	db 059h
g6e10:
	db 05Bh
g6e11:
	mov si,[pic_src]
g6e15:
	db 0ADh
g6e16:
	mov [pic_src],si
g6e1a:
	db 0ABh
g6e1b:
	db 0E2h,0E0h
g6e1d:
	db 05Fh
g6e1e:
	lea ax,[pic_dict]
g6e22:
	db 03Bh,0C7h
g6e24:
	db 075h,009h
g6e26:
	db 050h
g6e27:
	call far ptr g60e5
g6e2c:
	db 083h,0C4h,002h
g6e2f:
	db 0E9h,066h,0FFh
g6e32:
	db 080h,0E4h,001h
g6e35:
	mov byte ptr [pic_state+41],ah
g6e39:
	mov si,[pic_src]
g6e3d:
	cmp si,[pic_bound]
g6e41:
	db 072h,00Eh
g6e43:
	db 053h
g6e44:
	db 051h
g6e45:
	db 052h
g6e46:
	call dword ptr [pic_pump]
g6e4a:
	db 05Ah
g6e4b:
	db 059h
g6e4c:
	db 05Bh
g6e4d:
	mov si,[pic_src]
g6e51:
	db 0ADh
g6e52:
	mov [pic_src],si
g6e56:
	mov byte ptr [pic_state+24],ax
g6e59:
	mov si,[pic_src]
g6e5d:
	cmp si,[pic_bound]
g6e61:
	db 072h,00Eh
g6e63:
	db 053h
g6e64:
	db 051h
g6e65:
	db 052h
g6e66:
	call dword ptr [pic_pump]
g6e6a:
	db 05Ah
g6e6b:
	db 059h
g6e6c:
	db 05Bh
g6e6d:
	mov si,[pic_src]
g6e71:
	db 0ADh
g6e72:
	mov [pic_src],si
g6e76:
	mov byte ptr [pic_state+20],ax
g6e79:
	mov si,[pic_src]
g6e7d:
	cmp si,[pic_bound]
g6e81:
	db 072h,00Eh
g6e83:
	db 053h
g6e84:
	db 051h
g6e85:
	db 052h
g6e86:
	call dword ptr [pic_pump]
g6e8a:
	db 05Ah
g6e8b:
	db 059h
g6e8c:
	db 05Bh
g6e8d:
	mov si,[pic_src]
g6e91:
	db 0ADh
g6e92:
	mov [pic_src],si
g6e96:
	mov byte ptr [pic_state+22],ax
g6e99:
	db 0E8h,085h,000h
g6e9c:
	db 05Fh
g6e9d:
	db 05Eh
g6e9e:
	db 05Dh
g6e9f:
	db 0CBh
g6ea0:
	db 056h
g6ea1:
	mov si,[pic_src]
g6ea5:
	cmp si,[pic_bound]
g6ea9:
	db 072h,00Eh
g6eab:
	db 053h
g6eac:
	db 051h
g6ead:
	db 052h
g6eae:
	call dword ptr [pic_pump]
g6eb2:
	db 05Ah
g6eb3:
	db 059h
g6eb4:
	db 05Bh
g6eb5:
	mov si,[pic_src]
g6eb9:
	db 0ADh
g6eba:
	mov [pic_src],si
g6ebe:
	db 024h,001h
g6ec0:
	mov byte ptr [pic_state+41],al
g6ec3:
	mov si,[pic_src]
g6ec7:
	cmp si,[pic_bound]
g6ecb:
	db 072h,00Eh
g6ecd:
	db 053h
g6ece:
	db 051h
g6ecf:
	db 052h
g6ed0:
	call dword ptr [pic_pump]
g6ed4:
	db 05Ah
g6ed5:
	db 059h
g6ed6:
	db 05Bh
g6ed7:
	mov si,[pic_src]
g6edb:
	db 0ADh
g6edc:
	mov [pic_src],si
g6ee0:
	mov byte ptr [pic_state+24],ax
g6ee3:
	db 0E8h,03Bh,000h
g6ee6:
	db 05Eh
g6ee7:
	db 0CBh
g6ee8:
	db 055h
g6ee9:
	db 08Bh,0ECh
g6eeb:
	db 056h
g6eec:
	db 057h
g6eed:
	mov si,[pic_src]
g6ef1:
	db 0C4h,07Eh,006h
g6ef4:
	db 08Bh,04Eh,00Ah
g6ef7:
	db 0E8h,0A6h,000h
g6efa:
	mov [pic_src],si
g6efe:
	db 05Fh
g6eff:
	db 05Eh
g6f00:
	db 05Dh
g6f01:
	db 0CBh
g6f02:
	db 055h
g6f03:
	db 08Bh,0ECh
g6f05:
	db 056h
g6f06:
	db 057h
g6f07:
	db 08Ch,0D8h
g6f09:
	db 08Eh,0C0h
g6f0b:
	mov si,[pic_src]
g6f0f:
	db 08Bh,07Eh,006h
g6f12:
	mov cx,[pic_state+20]
g6f16:
	db 0E8h,087h,000h
g6f19:
	mov [pic_src],si
g6f1d:
	db 05Fh
g6f1e:
	db 05Eh
g6f1f:
	db 05Dh
g6f20:
	db 0CBh
g6f21:
	mov ax,[pic_state+20]
g6f24:
	or ax,[pic_state+22]
g6f28:
	db 075h,001h
g6f2a:
	db 0C3h
g6f2b:
	mov byte ptr [pic_state+30],00h
g6f30:
	mov byte ptr [pic_state+31],00h
g6f35:
	lea ax,[pic_6425]
g6f39:
	mov byte ptr [pic_state+26],ax
g6f3c:
	mov si,[pic_src]
g6f40:
	cmp si,[pic_bound]
g6f44:
	db 072h,00Eh
g6f46:
	db 053h
g6f47:
	db 051h
g6f48:
	db 052h
g6f49:
	call dword ptr [pic_pump]
g6f4d:
	db 05Ah
g6f4e:
	db 059h
g6f4f:
	db 05Bh
g6f50:
	mov si,[pic_src]
g6f54:
	db 0ADh
g6f55:
	mov [pic_src],si
g6f59:
	db 03Ch,00Bh
g6f5b:
	db 076h,002h
g6f5d:
	db 0B0h,00Bh
g6f5f:
	mov byte ptr [pic_state+33],al
g6f62:
	mov byte ptr [pic_state+38],ax
g6f65:
	mov byte ptr [pic_state+40],08h
g6f6a:
	mov byte ptr [pic_state+32],09h
g6f6f:
	mov word ptr [pic_state+34],01ffh
g6f75:
	db 0BAh,000h,001h
g6f78:
	mov byte ptr [pic_state+36],dx
g6f7c:
	db 0B8h,0FFh,0FFh
g6f7f:
	db 033h,0DBh
g6f81:
	db 0B9h,000h,008h
g6f84:
	db 089h,087h,094h,090h
g6f88:
	db 083h,0C3h,003h
g6f8b:
	db 0E2h,0F7h
g6f8d:
	db 0B0h,000h
g6f8f:
	db 033h,0DBh
g6f91:
	db 0B9h,000h,001h
g6f94:
	db 088h,087h,096h,090h
g6f98:
	db 0FEh,0C0h
g6f9a:
	db 083h,0C3h,003h
g6f9d:
	db 0E2h,0F5h
g6f9f:
	db 0C3h
g6fa0:
	cmp byte ptr [pic_state+41],00h
g6fa5:
	db 074h,003h
g6fa7:
	db 041h
g6fa8:
	db 0D1h,0E9h
g6faa:
	mov byte ptr [pic_state+28],cx
g6fae:
	mov dx,[pic_state+36]
g6fb2:
	xchg sp,[pic_state+26]
g6fb6:
	cmp byte ptr [pic_state+30],00h
g6fbb:
	db 075h,021h
g6fbd:
	db 0E8h,052h,000h
g6fc0:
	db 03Ch,090h
g6fc2:
	db 074h,006h
g6fc4:
	mov byte ptr [pic_state+31],al
g6fc7:
	db 0EBh,01Ch
	db 090h
g6fca:
	db 0E8h,045h,000h
g6fcd:
	db 00Ah,0C0h
g6fcf:
	db 075h,008h
g6fd1:
	db 0B0h,090h
g6fd3:
	mov byte ptr [pic_state+31],al
g6fd6:
	db 0EBh,00Dh
	db 090h
g6fd9:
	db 0FEh,0C8h
g6fdb:
	mov byte ptr [pic_state+30],al
g6fde:
	mov al,[pic_state+31]
g6fe1:
	dec byte ptr [pic_state+30]
g6fe5:
	cmp byte ptr [pic_state+41],00h
g6fea:
	db 074h,016h
g6fec:
	db 08Ah,0E0h
g6fee:
	db 024h,00Fh
g6ff0:
	db 0D0h,0ECh
g6ff2:
	db 0D0h,0ECh
g6ff4:
	db 0D0h,0ECh
g6ff6:
	db 0D0h,0ECh
g6ff8:
	db 0ABh
g6ff9:
	dec word ptr [pic_state+28]
g6ffd:
	db 075h,0B7h
g6fff:
	db 0EBh,008h
	db 090h
g7002:
	db 0AAh
g7003:
	dec word ptr [pic_state+28]
g7007:
	db 075h,0ADh
g7009:
	mov byte ptr [pic_state+36],dx
g700d:
	xchg sp,[pic_state+26]
g7011:
	db 0C3h
g7012:
	db 05Dh
g7013:
	db 081h,0FCh,025h,064h
g7017:
	db 074h,003h
g7019:
	db 058h
g701a:
	db 0FFh,0E5h
g701c:
	mov bx,[pic_state+38]
g7020:
	db 0B1h,010h
g7022:
	mov ch,[pic_state+40]
g7026:
	db 02Ah,0CDh
g7028:
	db 0D3h,0EBh
g702a:
	db 08Ah,0CDh
g702c:
	cmp cl,[pic_state+32]
g7030:
	db 07Dh,021h
g7032:
	cmp si,[pic_bound]
g7036:
	db 072h,00Eh
g7038:
	db 053h
g7039:
	db 051h
g703a:
	db 052h
g703b:
	call dword ptr [pic_pump]
g703f:
	db 05Ah
g7040:
	db 059h
g7041:
	db 05Bh
g7042:
	mov si,[pic_src]
g7046:
	db 0ADh
g7047:
	mov byte ptr [pic_state+38],ax
g704a:
	db 0D3h,0E0h
g704c:
	db 00Bh,0D8h
g704e:
	db 080h,0C1h,010h
g7051:
	db 0EBh,0D9h
g7053:
	sub cl,[pic_state+32]
g7057:
	mov byte ptr [pic_state+40],cl
g705b:
	db 08Bh,0C3h
g705d:
	and ax,[pic_state+34]
g7061:
	db 08Bh,0C8h
g7063:
	db 03Bh,0C2h
g7065:
	db 07Ch,00Ah
g7067:
	db 08Bh,0CAh
g7069:
	mov ax,[pic_state+42]
g706c:
	mov bl,[pic_state+44]
g7070:
	db 053h
g7071:
	db 08Bh,0D8h
g7073:
	db 003h,0D8h
g7075:
	db 003h,0D8h
g7077:
	db 08Bh,087h,094h,090h
g707b:
	db 040h
g707c:
	db 074h,008h
g707e:
	db 048h
g707f:
	db 08Ah,09Fh,096h,090h
g7083:
	db 053h
g7084:
	db 0EBh,0EBh
g7086:
	db 08Ah,087h,096h,090h
g708a:
	mov byte ptr [pic_state+44],al
g708d:
	db 050h
g708e:
	db 08Bh,0DAh
g7090:
	db 003h,0DAh
g7092:
	db 003h,0DAh
g7094:
	db 088h,087h,096h,090h
g7098:
	mov ax,[pic_state+42]
g709b:
	db 089h,087h,094h,090h
g709f:
	db 042h
g70a0:
	cmp dx,[pic_state+34]
g70a4:
	db 07Eh,009h
g70a6:
	inc byte ptr [pic_state+32]
g70aa:
	db 0F9h
g70ab:
	rcl word ptr [pic_state+34],1
g70af:
	mov al,[pic_state+32]
g70b2:
	cmp al,[pic_state+33]
g70b6:
	db 07Eh,003h
g70b8:
	db 0E8h,0AFh,0FEh
g70bb:
	mov byte ptr [pic_state+42],cx
g70bf:
	db 0E9h
MPSLIBM_TEXT ENDS

PIC_TEXT SEGMENT BYTE PUBLIC 'CODE'
        ASSUME CS:PIC_TEXT, DS:DGROUP
p40b50:
	db 055h
p40b51:
	db 08Bh,0ECh
p40b53:
	db 083h,0ECh,01Ah
p40b56:
	db 08Dh,046h,0E6h
p40b59:
	db 050h
p40b5a:
	db 0FFh,076h,006h
p40b5d:
	db 00Eh
p40b5e:
	db 0E8h,039h,007h
p40b61:
	db 083h,0C4h,004h
p40b64:
	db 00Bh,0C0h
p40b66:
	db 074h,034h
p40b68:
	db 0FFh,076h,0FCh
p40b6b:
	db 0FFh,076h,0FAh
p40b6e:
	push word ptr [pic_handle]
p40b72:
	db 00Eh
p40b73:
	db 0E8h,056h,008h
p40b76:
	db 083h,0C4h,006h
p40b79:
	db 00Bh,0C0h
p40b7b:
	db 074h,00Ah
p40b7d:
	db 0FFh,076h,006h
p40b80:
	db 00Eh
p40b81:
	db 0E8h,05Eh,006h
p40b84:
	db 083h,0C4h,002h
p40b87:
	db 08Bh,046h,0F6h
p40b8a:
	db 08Bh,056h,0F8h
p40b8d:
	mov byte ptr [pic_state],ax
p40b90:
	mov byte ptr [pic_state+2],dx
p40b94:
	mov ax,[pic_handle]
p40b97:
	db 08Bh,0E5h
p40b99:
	db 05Dh
p40b9a:
	db 0CBh
	db 090h
p40b9c:
	db 08Dh,046h,0FEh
p40b9f:
	db 050h
p40ba0:
	db 0FFh,076h,008h
p40ba3:
	db 0FFh,076h,006h
p40ba6:
	call far ptr __dos_open
p40bab:
	db 083h,0C4h,006h
p40bae:
	db 00Bh,0C0h
p40bb0:
	db 074h,00Ah
p40bb2:
	db 0FFh,076h,006h
p40bb5:
	db 00Eh
p40bb6:
	db 0E8h,029h,006h
p40bb9:
	db 083h,0C4h,002h
p40bbc:
	db 08Bh,046h,0FEh
p40bbf:
	db 08Bh,0E5h
p40bc1:
	db 05Dh
p40bc2:
	db 0CBh
	db 090h
p40bc4:
	db 055h
p40bc5:
	db 08Bh,0ECh
p40bc7:
	db 083h,0ECh,002h
p40bca:
	db 08Dh,046h,0FEh
p40bcd:
	db 050h
p40bce:
	db 02Bh,0C0h
p40bd0:
	db 050h
p40bd1:
	db 0FFh,076h,006h
p40bd4:
	call far ptr l_dos_creat
p40bd9:
	db 083h,0C4h,006h
p40bdc:
	db 00Bh,0C0h
p40bde:
	db 074h,00Ah
p40be0:
	db 0FFh,076h,006h
p40be3:
	db 00Eh
p40be4:
	db 0E8h,0FBh,005h
p40be7:
	db 083h,0C4h,002h
p40bea:
	db 08Bh,046h,0FEh
p40bed:
	db 08Bh,0E5h
p40bef:
	db 05Dh
p40bf0:
	db 0CBh
	db 090h
p40bf2:
	db 055h
p40bf3:
	db 08Bh,0ECh
p40bf5:
	mov ax,[pic_handle]
p40bf8:
	db 039h,046h,006h
p40bfb:
	db 074h,019h
p40bfd:
	db 0FFh,076h,006h
p40c00:
	call far ptr __dos_close
p40c05:
	db 083h,0C4h,002h
p40c08:
	db 00Bh,0C0h
p40c0a:
	db 074h,00Ah
p40c0c:
	db 02Bh,0C0h
p40c0e:
	db 050h
p40c0f:
	db 00Eh
p40c10:
	db 0E8h,0CFh,005h
p40c13:
	db 083h,0C4h,002h
p40c16:
	db 05Dh
p40c17:
	db 0CBh
p40c18:
	db 055h
p40c19:
	db 08Bh,0ECh
p40c1b:
	db 083h,0ECh,004h
p40c1e:
	db 02Bh,0C0h
p40c20:
	db 050h
p40c21:
	db 0FFh,076h,006h
p40c24:
	db 00Eh
p40c25:
	db 0E8h,028h,0FFh
p40c28:
	db 083h,0C4h,004h
p40c2b:
	db 089h,046h,0FCh
p40c2e:
	cmp word ptr [pic_handle],-01h
p40c33:
	db 075h,005h
p40c35:
	db 0B8h,0FFh,0FFh
p40c38:
	db 0EBh,003h
p40c3a:
	mov ax,[pic_state]
p40c3d:
	db 089h,046h,0FEh
p40c40:
	db 08Dh,046h,0FEh
p40c43:
	db 050h
p40c44:
	db 0FFh,076h,0FEh
p40c47:
	db 01Eh
p40c48:
	db 0FFh,076h,008h
p40c4b:
	db 0FFh,076h,0FCh
p40c4e:
	call far ptr __dos_read
p40c53:
	db 083h,0C4h,00Ah
p40c56:
	db 00Bh,0C0h
p40c58:
	db 074h,00Ah
p40c5a:
	db 02Bh,0C0h
p40c5c:
	db 050h
p40c5d:
	db 00Eh
p40c5e:
	db 0E8h,081h,005h
p40c61:
	db 083h,0C4h,002h
p40c64:
	db 0FFh,076h,0FCh
p40c67:
	db 00Eh
p40c68:
	db 0E8h,087h,0FFh
p40c6b:
	db 08Bh,046h,0FEh
p40c6e:
	db 08Bh,0E5h
p40c70:
	db 05Dh
p40c71:
	db 0CBh
p40c72:
	db 055h
p40c73:
	db 08Bh,0ECh
p40c75:
	db 083h,0ECh,004h
p40c78:
	db 02Bh,0C0h
p40c7a:
	db 050h
p40c7b:
	db 0FFh,076h,006h
p40c7e:
	db 00Eh
p40c7f:
	db 0E8h,0CEh,0FEh
p40c82:
	db 083h,0C4h,004h
p40c85:
	db 089h,046h,0FCh
p40c88:
	cmp word ptr [pic_handle],-01h
p40c8d:
	db 075h,005h
p40c8f:
	db 0B8h,0FFh,0FFh
p40c92:
	db 0EBh,003h
p40c94:
	mov ax,[pic_state]
p40c97:
	db 089h,046h,0FEh
p40c9a:
	db 08Dh,046h,0FEh
p40c9d:
	db 050h
p40c9e:
	db 0FFh,076h,0FEh
p40ca1:
	db 0FFh,076h,00Ah
p40ca4:
	db 0FFh,076h,008h
p40ca7:
	db 0FFh,076h,0FCh
p40caa:
	call far ptr __dos_read
p40caf:
	db 083h,0C4h,00Ah
p40cb2:
	db 00Bh,0C0h
p40cb4:
	db 074h,00Ah
p40cb6:
	db 02Bh,0C0h
p40cb8:
	db 050h
p40cb9:
	db 00Eh
p40cba:
	db 0E8h,025h,005h
p40cbd:
	db 083h,0C4h,002h
p40cc0:
	db 0FFh,076h,0FCh
p40cc3:
	db 00Eh
p40cc4:
	db 0E8h,02Bh,0FFh
p40cc7:
	db 08Bh,046h,0FEh
p40cca:
	db 08Bh,0E5h
p40ccc:
	db 05Dh
p40ccd:
	db 0CBh
p40cce:
	db 055h
p40ccf:
	db 08Bh,0ECh
p40cd1:
	db 083h,0ECh,002h
p40cd4:
	db 0FFh,076h,006h
p40cd7:
	db 00Eh
p40cd8:
	db 0E8h,0E9h,0FEh
p40cdb:
	db 083h,0C4h,002h
p40cde:
	db 089h,046h,0FEh
p40ce1:
	db 08Dh,046h,00Ch
p40ce4:
	db 050h
p40ce5:
	db 0FFh,076h,00Ch
p40ce8:
	db 0FFh,076h,00Ah
p40ceb:
	db 0FFh,076h,008h
p40cee:
	db 0FFh,076h,0FEh
p40cf1:
	call far ptr __dos_write
p40cf6:
	db 083h,0C4h,00Ah
p40cf9:
	db 00Bh,0C0h
p40cfb:
	db 074h,00Ah
p40cfd:
	db 02Bh,0C0h
p40cff:
	db 050h
p40d00:
	db 00Eh
p40d01:
	db 0E8h,0DEh,004h
p40d04:
	db 083h,0C4h,002h
p40d07:
	db 0FFh,076h,0FEh
p40d0a:
	db 00Eh
p40d0b:
	db 0E8h,0E4h,0FEh
p40d0e:
	db 08Bh,046h,00Ch
p40d11:
	db 08Bh,0E5h
p40d13:
	db 05Dh
p40d14:
	db 0CBh
	db 090h
p40d16:
	db 055h
p40d17:
	db 08Bh,0ECh
p40d19:
	db 083h,0ECh,004h
p40d1c:
	db 057h
p40d1d:
	db 056h
p40d1e:
	db 02Bh,0C0h
p40d20:
	db 050h
p40d21:
	db 0FFh,076h,00Ch
p40d24:
	db 00Eh
p40d25:
	db 0E8h,028h,0FEh
p40d28:
	db 083h,0C4h,004h
p40d2b:
	db 089h,046h,0FCh
p40d2e:
	db 050h
p40d2f:
	db 00Eh
p40d30:
	db 0E8h,021h,004h
p40d33:
	db 083h,0C4h,002h
p40d36:
	db 0FFh,076h,00Eh
p40d39:
	call far ptr g6d88
p40d3e:
	db 083h,0C4h,002h
p40d41:
	db 083h,07Eh,006h,000h
p40d45:
	db 07Dh,006h
p40d47:
	mov word ptr [pic_state+22],00h
p40d4d:
	db 02Bh,0F6h
p40d4f:
	db 08Bh,07Eh,006h
p40d52:
	db 0EBh,027h
p40d54:
	db 0B8h,05Eh,0CAh
p40d57:
	db 050h
p40d58:
	call far ptr g6f02
p40d5d:
	db 083h,0C4h,002h
p40d60:
	push word ptr [pic_state+20]
p40d64:
	db 08Bh,046h,00Ah
p40d67:
	db 003h,0C6h
p40d69:
	db 050h
p40d6a:
	db 0FFh,076h,008h
p40d6d:
	db 057h
p40d6e:
	db 0B8h,05Eh,0CAh
p40d71:
	db 050h
p40d72:
	call far ptr g60a6
p40d77:
	db 083h,0C4h,00Ah
p40d7a:
	db 046h
p40d7b:
	cmp si,[pic_state+22]
p40d7f:
	db 07Ch,0D3h
p40d81:
	db 089h,076h,0FEh
p40d84:
	db 0FFh,076h,0FCh
p40d87:
	db 00Eh
p40d88:
	db 0E8h,067h,0FEh
p40d8b:
	db 083h,0C4h,002h
p40d8e:
	db 05Eh
p40d8f:
	db 05Fh
p40d90:
	db 08Bh,0E5h
p40d92:
	db 05Dh
p40d93:
	db 0CBh
PUBLIC _LoadPicture
_LoadPicture LABEL FAR
	db 055h
p40d95:
	db 08Bh,0ECh
p40d97:
	db 0B8h,001h,000h
p40d9a:
	db 050h
p40d9b:
	db 0FFh,076h,008h
p40d9e:
	db 02Bh,0C0h
p40da0:
	db 050h
p40da1:
	db 050h
p40da2:
	db 0FFh,076h,006h
p40da5:
	db 00Eh
p40da6:
	db 0E8h,06Dh,0FFh
p40da9:
	db 083h,0C4h,00Ah
p40dac:
	db 05Dh
p40dad:
	db 0CBh
PUBLIC _LoadPicToPal
_LoadPicToPal LABEL FAR
	db 055h
p40daf:
	db 08Bh,0ECh
p40db1:
	db 0FFh,076h,00Ah
p40db4:
	db 0FFh,076h,008h
p40db7:
	db 02Bh,0C0h
p40db9:
	db 050h
p40dba:
	db 050h
p40dbb:
	db 0FFh,076h,006h
p40dbe:
	db 00Eh
p40dbf:
	db 0E8h,054h,0FFh
p40dc2:
	db 083h,0C4h,00Ah
p40dc5:
	db 05Dh
p40dc6:
	db 0CBh
	db 090h
PUBLIC _LoadPicNoPal
_LoadPicNoPal LABEL FAR
	db 055h
p40dc9:
	db 08Bh,0ECh
p40dcb:
	db 02Bh,0C0h
p40dcd:
	db 050h
p40dce:
	db 0FFh,076h,008h
p40dd1:
	db 050h
p40dd2:
	db 050h
p40dd3:
	db 0FFh,076h,006h
p40dd6:
	db 00Eh
p40dd7:
	db 0E8h,03Ch,0FFh
p40dda:
	db 083h,0C4h,00Ah
p40ddd:
	db 05Dh
p40dde:
	db 0CBh
	db 090h
PUBLIC _LoadPalNoPic
_LoadPalNoPic LABEL FAR
	db 055h
p40de1:
	db 08Bh,0ECh
p40de3:
	db 0B8h,001h,000h
p40de6:
	db 050h
p40de7:
	db 0FFh,076h,006h
p40dea:
	db 02Bh,0C0h
p40dec:
	db 050h
p40ded:
	db 050h
p40dee:
	db 0B8h,0FFh,0FFh
p40df1:
	db 050h
p40df2:
	db 00Eh
p40df3:
	db 0E8h,020h,0FFh
p40df6:
	db 083h,0C4h,00Ah
p40df9:
	db 05Dh
p40dfa:
	db 0CBh
	db 090h
PUBLIC _LoadPalToPal
_LoadPalToPal LABEL FAR
	db 055h
p40dfd:
	db 08Bh,0ECh
p40dff:
	db 0FFh,076h,008h
p40e02:
	db 0FFh,076h,006h
p40e05:
	db 02Bh,0C0h
p40e07:
	db 050h
p40e08:
	db 050h
p40e09:
	db 0B8h,0FFh,0FFh
p40e0c:
	db 050h
p40e0d:
	db 00Eh
p40e0e:
	db 0E8h,005h,0FFh
p40e11:
	db 083h,0C4h,00Ah
p40e14:
	db 05Dh
p40e15:
	db 0CBh
p40e16:
	db 055h
p40e17:
	db 08Bh,0ECh
p40e19:
	db 083h,0ECh,002h
p40e1c:
	db 02Bh,0C0h
p40e1e:
	db 050h
p40e1f:
	db 0FFh,076h,00Ah
p40e22:
	db 00Eh
p40e23:
	db 0E8h,02Ah,0FDh
p40e26:
	db 083h,0C4h,004h
p40e29:
	db 089h,046h,0FEh
p40e2c:
	db 050h
p40e2d:
	db 00Eh
p40e2e:
	db 0E8h,023h,003h
p40e31:
	db 083h,0C4h,002h
p40e34:
	call far ptr g6ea0
p40e39:
	push word ptr [pic_state+24]
p40e3d:
	db 0FFh,076h,008h
p40e40:
	db 0FFh,076h,006h
p40e43:
	call far ptr g6ee8
p40e48:
	db 083h,0C4h,006h
p40e4b:
	db 0FFh,076h,0FEh
p40e4e:
	db 00Eh
p40e4f:
	db 0E8h,0A0h,0FDh
p40e52:
	mov ax,[pic_state+24]
p40e55:
	db 08Bh,0E5h
p40e57:
	db 05Dh
p40e58:
	db 0CBh
	db 090h
p40e5a:
	db 055h
p40e5b:
	db 08Bh,0ECh
p40e5d:
	db 083h,0ECh,00Eh
p40e60:
	db 08Dh,046h,0F2h
p40e63:
	db 050h
p40e64:
	db 02Bh,0C0h
p40e66:
	db 050h
p40e67:
	db 0FFh,076h,00Ah
p40e6a:
	call far ptr __dos_open
p40e6f:
	db 083h,0C4h,006h
p40e72:
	db 00Bh,0C0h
p40e74:
	db 074h,008h
p40e76:
	mov ax,[_errno]
p40e79:
	db 08Bh,0E5h
p40e7b:
	db 05Dh
p40e7c:
	db 0CBh
	db 090h
p40e7e:
	db 08Dh,046h,0F4h
p40e81:
	db 050h
p40e82:
	db 0B8h,002h,000h
p40e85:
	db 050h
p40e86:
	db 08Dh,046h,0FEh
p40e89:
	db 016h
p40e8a:
	db 050h
p40e8b:
	db 0FFh,076h,0F2h
p40e8e:
	call far ptr __dos_read
p40e93:
	db 083h,0C4h,00Ah
p40e96:
	db 00Bh,0C0h
p40e98:
	db 075h,0DCh
p40e9a:
	mov word ptr [_errno],02h
p40ea0:
	db 08Bh,046h,0FEh
p40ea3:
	db 039h,046h,008h
p40ea6:
	db 076h,008h
p40ea8:
	db 0B8h,002h,000h
p40eab:
	db 08Bh,0E5h
p40ead:
	db 05Dh
p40eae:
	db 0CBh
	db 090h
p40eb0:
	db 08Bh,046h,008h
p40eb3:
	db 0D1h,0E0h
p40eb5:
	db 099h
p40eb6:
	db 052h
p40eb7:
	db 050h
p40eb8:
	db 0FFh,076h,0F2h
p40ebb:
	db 00Eh
p40ebc:
	db 0E8h,00Dh,005h
p40ebf:
	db 083h,0C4h,006h
p40ec2:
	db 00Bh,0C0h
p40ec4:
	db 075h,0B0h
p40ec6:
	db 08Dh,046h,0F4h
p40ec9:
	db 050h
p40eca:
	db 0B8h,002h,000h
p40ecd:
	db 050h
p40ece:
	db 08Dh,046h,0FEh
p40ed1:
	db 016h
p40ed2:
	db 050h
p40ed3:
	db 0FFh,076h,0F2h
p40ed6:
	call far ptr __dos_read
p40edb:
	db 083h,0C4h,00Ah
p40ede:
	db 00Bh,0C0h
p40ee0:
	db 075h,094h
p40ee2:
	db 083h,06Eh,0FEh,008h
p40ee6:
	db 02Bh,0C0h
p40ee8:
	db 050h
p40ee9:
	db 0FFh,076h,0FEh
p40eec:
	db 0FFh,076h,0F2h
p40eef:
	db 00Eh
p40ef0:
	db 0E8h,0D9h,004h
p40ef3:
	db 083h,0C4h,006h
p40ef6:
	db 00Bh,0C0h
p40ef8:
	db 074h,003h
p40efa:
	db 0E9h,079h,0FFh
p40efd:
	db 08Dh,046h,0F4h
p40f00:
	db 050h
p40f01:
	db 0B8h,008h,000h
p40f04:
	db 050h
p40f05:
	db 0B8h,0FCh,061h
p40f08:
	db 01Eh
p40f09:
	db 050h
p40f0a:
	db 0FFh,076h,0F2h
p40f0d:
	call far ptr __dos_read
p40f12:
	db 083h,0C4h,00Ah
p40f15:
	db 00Bh,0C0h
p40f17:
	db 074h,003h
p40f19:
	db 0E9h,05Ah,0FFh
p40f1c:
	mov al,[pic_state+5]
p40f1f:
	db 02Ah,0E4h
p40f21:
	mov cl,[pic_state+4]
p40f25:
	db 02Ah,0EDh
p40f27:
	db 02Bh,0C1h
p40f29:
	db 040h
p40f2a:
	db 089h,046h,0FCh
p40f2d:
	mov al,[pic_state+6]
p40f30:
	db 02Ah,0E4h
p40f32:
	db 08Bh,0C8h
p40f34:
	mov al,[pic_state+8]
p40f37:
	mov dl,[pic_state+10]
p40f3b:
	db 02Ah,0F6h
p40f3d:
	db 003h,0C2h
p40f3f:
	db 0F7h,066h,0FCh
p40f42:
	db 0F7h,0E1h
p40f44:
	db 005h,008h,000h
p40f47:
	db 089h,046h,0F6h
p40f4a:
	db 0FFh,076h,006h
p40f4d:
	call far ptr g606e
p40f52:
	db 083h,0C4h,002h
p40f55:
	db 02Dh,008h,000h
p40f58:
	db 089h,046h,0F8h
p40f5b:
	db 089h,056h,0FAh
p40f5e:
	cmp byte ptr [pic_state+7],00h
p40f63:
	db 075h,00Ch
p40f65:
	db 08Bh,046h,0FCh
p40f68:
	db 029h,046h,0F8h
p40f6b:
	db 029h,046h,0FEh
p40f6e:
	db 001h,046h,0F6h
p40f71:
	db 02Bh,0C0h
p40f73:
	db 050h
p40f74:
	db 0FFh,076h,0FEh
p40f77:
	db 0FFh,076h,0F2h
p40f7a:
	db 00Eh
p40f7b:
	db 0E8h,04Eh,004h
p40f7e:
	db 083h,0C4h,006h
p40f81:
	db 00Bh,0C0h
p40f83:
	db 074h,003h
p40f85:
	db 0E9h,0EEh,0FEh
p40f88:
	db 08Dh,046h,0F4h
p40f8b:
	db 050h
p40f8c:
	db 0FFh,076h,0F6h
p40f8f:
	db 0FFh,076h,0FAh
p40f92:
	db 0FFh,076h,0F8h
p40f95:
	db 0FFh,076h,0F2h
p40f98:
	call far ptr __dos_read
p40f9d:
	db 083h,0C4h,00Ah
p40fa0:
	db 00Bh,0C0h
p40fa2:
	db 074h,003h
p40fa4:
	db 0E9h,0CFh,0FEh
p40fa7:
	db 0FFh,076h,0F2h
p40faa:
	call far ptr __dos_close
p40faf:
	db 083h,0C4h,002h
p40fb2:
	db 00Bh,0C0h
p40fb4:
	db 074h,003h
p40fb6:
	db 0E9h,0BDh,0FEh
p40fb9:
	db 02Bh,0C0h
p40fbb:
	db 08Bh,0E5h
p40fbd:
	db 05Dh
p40fbe:
	db 0CBh
	db 090h
p40fc0:
	db 055h
p40fc1:
	db 08Bh,0ECh
p40fc3:
	db 083h,0ECh,004h
p40fc6:
	db 056h
p40fc7:
	db 02Bh,0C0h
p40fc9:
	db 050h
p40fca:
	db 0FFh,076h,006h
p40fcd:
	db 00Eh
p40fce:
	db 0E8h,07Fh,0FBh
p40fd1:
	db 083h,0C4h,004h
p40fd4:
	db 089h,046h,0FCh
p40fd7:
	db 050h
p40fd8:
	db 00Eh
p40fd9:
	db 0E8h,078h,001h
p40fdc:
	db 083h,0C4h,002h
p40fdf:
	db 02Bh,0C0h
p40fe1:
	db 050h
p40fe2:
	call far ptr g6d88
p40fe7:
	db 083h,0C4h,002h
p40fea:
	push word ptr [pic_state+22]
p40fee:
	push word ptr [pic_state+20]
p40ff2:
	call far ptr g6116
p40ff7:
	db 083h,0C4h,004h
p40ffa:
	db 02Bh,0F6h
p40ffc:
	db 0EBh,019h
p40ffe:
	db 0B8h,05Eh,0CAh
p41001:
	db 050h
p41002:
	call far ptr g6f02
p41007:
	db 083h,0C4h,002h
p4100a:
	db 0B8h,05Eh,0CAh
p4100d:
	db 050h
p4100e:
	call far ptr g610f
p41013:
	db 083h,0C4h,002h
p41016:
	db 046h
p41017:
	cmp si,[pic_state+22]
p4101b:
	db 07Ch,0E1h
p4101d:
	db 089h,076h,0FEh
p41020:
	db 0FFh,076h,0FCh
p41023:
	db 00Eh
p41024:
	db 0E8h,0CBh,0FBh
p41027:
	db 083h,0C4h,002h
p4102a:
	call far ptr g6108
p4102f:
	db 05Eh
p41030:
	db 08Bh,0E5h
p41032:
	db 05Dh
p41033:
	db 0CBh
PUBLIC _LoadPicGmem
_LoadPicGmem LABEL FAR
	db 055h
p41035:
	db 08Bh,0ECh
p41037:
	db 0FFh,076h,006h
p4103a:
	db 0FFh,076h,008h
p4103d:
	db 00Eh
p4103e:
	db 0E8h,053h,0FDh
p41041:
	db 083h,0C4h,004h
p41044:
	db 05Dh
p41045:
	db 0CBh
p41046:
	db 055h
p41047:
	db 08Bh,0ECh
p41049:
	db 083h,0ECh,004h
p4104c:
	db 057h
p4104d:
	db 056h
p4104e:
	db 02Bh,0C0h
p41050:
	db 050h
p41051:
	db 0FFh,076h,006h
p41054:
	db 00Eh
p41055:
	db 0E8h,0F8h,0FAh
p41058:
	db 083h,0C4h,004h
p4105b:
	db 089h,046h,0FCh
p4105e:
	db 050h
p4105f:
	db 00Eh
p41060:
	db 0E8h,0F1h,000h
p41063:
	db 083h,0C4h,002h
p41066:
	db 02Bh,0C0h
p41068:
	db 050h
p41069:
	call far ptr g6d88
p4106e:
	db 083h,0C4h,002h
p41071:
	db 02Bh,0F6h
p41073:
	db 08Bh,07Eh,008h
p41076:
	db 0EBh,022h
p41078:
	db 0B8h,05Eh,0CAh
p4107b:
	db 050h
p4107c:
	call far ptr g6f02
p41081:
	db 083h,0C4h,002h
p41084:
	push word ptr [pic_state+20]
p41088:
	db 056h
p41089:
	db 02Bh,0C0h
p4108b:
	db 050h
p4108c:
	db 057h
p4108d:
	db 0B8h,05Eh,0CAh
p41090:
	db 050h
p41091:
	call far ptr g60a6
p41096:
	db 083h,0C4h,00Ah
p41099:
	db 046h
p4109a:
	cmp si,[pic_state+22]
p4109e:
	db 07Ch,0D8h
p410a0:
	db 089h,076h,0FEh
p410a3:
	db 05Eh
p410a4:
	db 05Fh
p410a5:
	db 08Bh,0E5h
p410a7:
	db 05Dh
p410a8:
	db 0CBh
	db 090h
p410aa:
	db 055h
p410ab:
	db 08Bh,0ECh
p410ad:
	db 083h,0ECh,002h
p410b0:
	db 057h
p410b1:
	db 056h
p410b2:
	mov word ptr [pic_state+14],00h
p410b8:
	db 08Bh,046h,006h
p410bb:
	mov byte ptr [pic_state+16],ax
p410be:
	mov ax,[pic_bound]
p410c1:
	mov [pic_src],ax
p410c4:
	mov word ptr [pic_pump],offset p41172
p410ca:
	mov word ptr [pic_pump+2],seg p40b50
p410d0:
	db 02Bh,0C0h
p410d2:
	db 050h
p410d3:
	call far ptr g6d88
p410d8:
	db 083h,0C4h,002h
p410db:
	db 02Bh,0F6h
p410dd:
	db 08Bh,07Eh,008h
p410e0:
	db 0EBh,022h
p410e2:
	db 0B8h,05Eh,0CAh
p410e5:
	db 050h
p410e6:
	call far ptr g6f02
p410eb:
	db 083h,0C4h,002h
p410ee:
	push word ptr [pic_state+20]
p410f2:
	db 056h
p410f3:
	db 02Bh,0C0h
p410f5:
	db 050h
p410f6:
	db 057h
p410f7:
	db 0B8h,05Eh,0CAh
p410fa:
	db 050h
p410fb:
	call far ptr g60a6
p41100:
	db 083h,0C4h,00Ah
p41103:
	db 046h
p41104:
	cmp si,[pic_state+22]
p41108:
	db 07Ch,0D8h
p4110a:
	db 089h,076h,0FEh
p4110d:
	db 05Eh
p4110e:
	db 05Fh
p4110f:
	db 08Bh,0E5h
p41111:
	db 05Dh
p41112:
	db 0CBh
	db 090h
p41114:
	db 055h
p41115:
	db 08Bh,0ECh
p41117:
	db 083h,0ECh,002h
p4111a:
	db 08Bh,046h,006h
p4111d:
	mov byte ptr [pic_state+14],ax
p41120:
	db 08Bh,046h,00Ch
p41123:
	mov byte ptr [pic_state+16],ax
p41126:
	mov ax,[pic_bound]
p41129:
	mov [pic_src],ax
p4112c:
	mov word ptr [pic_pump],offset p41172
p41132:
	mov word ptr [pic_pump+2],seg p40b50
p41138:
	call far ptr g6ea0
p4113d:
	push word ptr [pic_state+24]
p41141:
	db 0FFh,076h,00Ch
p41144:
	db 0FFh,076h,00Ah
p41147:
	call far ptr g6ee8
p4114c:
	mov ax,[pic_state+24]
p4114f:
	db 08Bh,0E5h
p41151:
	db 05Dh
p41152:
	db 0CBh
	db 090h
p41154:
	db 055h
p41155:
	db 08Bh,0ECh
p41157:
	db 08Bh,046h,006h
p4115a:
	mov byte ptr [pic_state+12],ax
p4115d:
	mov ax,[pic_bound]
p41160:
	mov [pic_src],ax
p41163:
	mov word ptr [pic_pump],offset p411b8
p41169:
	mov word ptr [pic_pump+2],seg p40b50
p4116f:
	db 05Dh
p41170:
	db 0CBh
	db 090h
p41172:
	db 055h
p41173:
	db 08Bh,0ECh
p41175:
	db 083h,0ECh,004h
p41178:
	mov ax,offset pic_cbuf
p4117b:
	db 089h,046h,0FCh
p4117e:
	db 08Ch,05Eh,0FEh
p41181:
	db 0B8h,000h,002h
p41184:
	db 050h
p41185:
	db 0FFh,076h,0FCh
p41188:
	db 01Eh
p41189:
	push word ptr [pic_state+14]
p4118d:
	push word ptr [pic_state+16]
p41191:
	call far ptr _movedata
p41196:
	mov word ptr [pic_src],offset pic_cbuf
p4119c:
	add byte ptr [pic_state+15],02h
p411a1:
	db 0B8h,000h,002h
p411a4:
	db 08Bh,0E5h
p411a6:
	db 05Dh
p411a7:
	db 0CBh
p411a8:
	db 055h
p411a9:
	db 08Bh,0ECh
p411ab:
	db 08Bh,046h,006h
p411ae:
	mov byte ptr [pic_state+12],ax
p411b1:
	db 00Eh
p411b2:
	db 0E8h,003h,000h
p411b5:
	db 05Dh
p411b6:
	db 0CBh
	db 090h
p411b8:
	db 055h
p411b9:
	db 08Bh,0ECh
p411bb:
	db 083h,0ECh,002h
p411be:
	db 08Dh,046h,0FEh
p411c1:
	db 050h
p411c2:
	db 0B8h,000h,002h
p411c5:
	db 050h
p411c6:
	mov ax,offset pic_cbuf
p411c9:
	db 01Eh
p411ca:
	db 050h
p411cb:
	push word ptr [pic_state+12]
p411cf:
	call far ptr __dos_read
p411d4:
	mov word ptr [pic_src],offset pic_cbuf
p411da:
	db 08Bh,046h,0FEh
p411dd:
	db 08Bh,0E5h
p411df:
	db 05Dh
p411e0:
	db 0CBh
	db 090h
p411e2:
	db 055h
p411e3:
	db 08Bh,0ECh
p411e5:
	db 0B8h,003h,000h
p411e8:
	db 050h
p411e9:
	db 00Eh
p411ea:
	db 0E8h,01Dh,000h
p411ed:
	db 083h,0C4h,002h
p411f0:
	db 0FFh,076h,006h
p411f3:
	call far ptr l_perror
p411f8:
	db 083h,0C4h,002h
p411fb:
	db 0B8h,063h,000h
p411fe:
	db 050h
p411ff:
	call far ptr _exit
p41204:
	db 083h,0C4h,002h
p41207:
	db 05Dh
p41208:
	db 0CBh
	db 090h
p4120a:
	db 055h
p4120b:
	db 08Bh,0ECh
p4120d:
	db 083h,0ECh,010h
p41210:
	mov ax,[_errno]
p41213:
	db 089h,046h,0F0h
p41216:
	db 0C6h,046h,0F3h,000h
p4121a:
	db 08Ah,046h,006h
p4121d:
	db 088h,046h,0F2h
p41220:
	db 08Dh,046h,0F2h
p41223:
	db 050h
p41224:
	db 08Dh,046h,0F2h
p41227:
	db 050h
p41228:
	db 0B8h,010h,000h
p4122b:
	db 050h
p4122c:
	call far ptr _int86x
p41231:
	db 08Bh,046h,0F0h
p41234:
	mov [_errno],ax
p41237:
	db 08Bh,0E5h
p41239:
	db 05Dh
p4123a:
	db 0CBh
	db 090h
p4123c:
	db 055h
p4123d:
	db 08Bh,0ECh
p4123f:
	db 083h,0ECh,002h
p41242:
	db 08Dh,046h,0FEh
p41245:
	db 050h
p41246:
	db 02Bh,0C0h
p41248:
	db 050h
p41249:
	db 0FFh,076h,006h
p4124c:
	call far ptr __dos_open
p41251:
	db 083h,0C4h,006h
p41254:
	db 00Bh,0C0h
p41256:
	db 074h,006h
p41258:
	db 02Bh,0C0h
p4125a:
	db 08Bh,0E5h
p4125c:
	db 05Dh
p4125d:
	db 0CBh
p4125e:
	db 08Bh,046h,0FEh
p41261:
	mov [pic_handle],ax
p41264:
	mov word ptr [pic_state+18],00h
p4126a:
	db 0B8h,001h,000h
p4126d:
	db 08Bh,0E5h
p4126f:
	db 05Dh
p41270:
	db 0CBh
	db 090h,083h,03eh,036h,057h,0ffh,074h,020h,0ffh,036h,036h,057h,09ah,02ch,00dh,040h
	db 041h,083h,0c4h,002h,00bh,0c0h,074h,00ah,02bh,0c0h,050h,00eh,0e8h,052h,0ffh,083h
	db 0c4h,002h,0c7h,006h,036h,057h,0ffh,0ffh,0cbh
p4129a:
	db 055h
p4129b:
	db 08Bh,0ECh
p4129d:
	db 083h,0ECh,008h
p412a0:
	db 057h
p412a1:
	db 056h
p412a2:
	cmp word ptr [pic_handle],-01h
p412a7:
	db 075h,009h
p412a9:
	db 02Bh,0C0h
p412ab:
	db 05Eh
p412ac:
	db 05Fh
p412ad:
	db 08Bh,0E5h
p412af:
	db 05Dh
p412b0:
	db 0CBh
	db 090h
p412b2:
	db 02Bh,0C0h
p412b4:
	db 050h
p412b5:
	db 050h
p412b6:
	push word ptr [pic_handle]
p412ba:
	db 00Eh
p412bb:
	db 0E8h,00Eh,001h
p412be:
	db 083h,0C4h,006h
p412c1:
	db 00Bh,0C0h
p412c3:
	db 074h,00Ah
p412c5:
	db 02Bh,0C0h
p412c7:
	db 050h
p412c8:
	db 00Eh
p412c9:
	db 0E8h,016h,0FFh
p412cc:
	db 083h,0C4h,002h
p412cf:
	db 08Dh,046h,0FEh
p412d2:
	db 050h
p412d3:
	db 0B8h,002h,000h
p412d6:
	db 050h
p412d7:
	db 08Dh,046h,0FCh
p412da:
	db 016h
p412db:
	db 050h
p412dc:
	push word ptr [pic_handle]
p412e0:
	call far ptr __dos_read
p412e5:
	db 083h,0C4h,00Ah
p412e8:
	db 00Bh,0C0h
p412ea:
	db 074h,00Ah
p412ec:
	db 02Bh,0C0h
p412ee:
	db 050h
p412ef:
	db 00Eh
p412f0:
	db 0E8h,0EFh,0FEh
p412f3:
	db 083h,0C4h,002h
p412f6:
	db 083h,07Eh,006h,000h
p412fa:
	db 075h,05Eh
p412fc:
	inc word ptr [pic_state+18]
p41300:
	mov ax,[pic_state+18]
p41303:
	db 039h,046h,0FCh
p41306:
	db 07Ch,0A1h
p41308:
	db 089h,046h,0FCh
p4130b:
	db 0FFh,04Eh,0FCh
p4130e:
	db 00Bh,0C0h
p41310:
	db 074h,03Eh
p41312:
	db 08Bh,046h,008h
p41315:
	db 089h,046h,0F8h
p41318:
	db 08Ch,05Eh,0FAh
p4131b:
	db 08Bh,076h,0FCh
p4131e:
	db 08Dh,046h,0FEh
p41321:
	db 050h
p41322:
	db 0B8h,018h,000h
p41325:
	db 050h
p41326:
	db 0FFh,076h,0FAh
p41329:
	db 0FFh,076h,0F8h
p4132c:
	push word ptr [pic_handle]
p41330:
	call far ptr __dos_read
p41335:
	db 083h,0C4h,00Ah
p41338:
	db 00Bh,0C0h
p4133a:
	db 074h,00Ah
p4133c:
	db 02Bh,0C0h
p4133e:
	db 050h
p4133f:
	db 00Eh
p41340:
	db 0E8h,09Fh,0FEh
p41343:
	db 083h,0C4h,002h
p41346:
	db 08Bh,0C6h
p41348:
	db 04Eh
p41349:
	db 00Bh,0C0h
p4134b:
	db 075h,0D1h
p4134d:
	db 089h,076h,0FCh
p41350:
	db 0B8h,001h,000h
p41353:
	db 05Eh
p41354:
	db 05Fh
p41355:
	db 08Bh,0E5h
p41357:
	db 05Dh
p41358:
	db 0CBh
	db 090h
p4135a:
	db 08Bh,046h,0FCh
p4135d:
	db 0FFh,04Eh,0FCh
p41360:
	db 00Bh,0C0h
p41362:
	db 075h,003h
p41364:
	db 0E9h,042h,0FFh
p41367:
	db 08Bh,046h,008h
p4136a:
	db 089h,046h,0F8h
p4136d:
	db 08Ch,05Eh,0FAh
p41370:
	db 08Bh,0F8h
p41372:
	db 08Bh,076h,0FCh
p41375:
	db 08Dh,046h,0FEh
p41378:
	db 050h
p41379:
	db 0B8h,018h,000h
p4137c:
	db 050h
p4137d:
	db 0FFh,076h,0FAh
p41380:
	db 0FFh,076h,0F8h
p41383:
	push word ptr [pic_handle]
p41387:
	call far ptr __dos_read
p4138c:
	db 083h,0C4h,00Ah
p4138f:
	db 00Bh,0C0h
p41391:
	db 074h,00Ah
p41393:
	db 02Bh,0C0h
p41395:
	db 050h
p41396:
	db 00Eh
p41397:
	db 0E8h,048h,0FEh
p4139a:
	db 083h,0C4h,002h
p4139d:
	db 0B8h,00Ch,000h
p413a0:
	db 050h
p413a1:
	db 0FFh,076h,006h
p413a4:
	db 057h
p413a5:
	call far ptr l_strnicmp
p413aa:
	db 083h,0C4h,006h
p413ad:
	db 00Bh,0C0h
p413af:
	db 075h,00Dh
p413b1:
	db 0B8h,001h,000h
p413b4:
	db 089h,076h,0FCh
p413b7:
	db 05Eh
p413b8:
	db 05Fh
p413b9:
	db 08Bh,0E5h
p413bb:
	db 05Dh
p413bc:
	db 0CBh
	db 090h
p413be:
	db 08Bh,0C6h
p413c0:
	db 04Eh
p413c1:
	db 00Bh,0C0h
p413c3:
	db 075h,0B0h
p413c5:
	db 089h,076h,0FCh
p413c8:
	db 0E9h,0DEh,0FEh
	db 090h
p413cc:
	db 055h
p413cd:
	db 08Bh,0ECh
p413cf:
	db 083h,0ECh,00Eh
p413d2:
	db 0C7h,046h,0F2h,000h,042h
p413d7:
	db 08Bh,046h,006h
p413da:
	db 089h,046h,0F4h
p413dd:
	db 08Bh,046h,00Ah
p413e0:
	db 089h,046h,0F6h
p413e3:
	db 08Bh,046h,008h
p413e6:
	db 089h,046h,0F8h
p413e9:
	db 08Dh,046h,0F2h
p413ec:
	db 050h
p413ed:
	db 08Dh,046h,0F2h
p413f0:
	db 050h
p413f1:
	call far ptr l_intdosx
p413f6:
	db 083h,0C4h,004h
p413f9:
	db 083h,07Eh,0FEh,000h
p413fd:
	db 075h,007h
p413ff:
	db 02Bh
PIC_TEXT ENDS


EXTRN F_SinDP:FAR,F_CosDP:FAR,_errno:WORD
EXTRN __dos_open:FAR,__dos_close:FAR,__dos_read:FAR,__dos_write:FAR
EXTRN _movedata:FAR,_int86x:FAR,_exit:FAR

AUX2_TEXT SEGMENT BYTE PUBLIC 'CODE'
 ASSUME CS:AUX2_TEXT
PUBLIC _perror
l_perror LABEL FAR
_perror LABEL FAR
 push bp
 mov bp,sp
 push word ptr [bp+6]
 call far ptr _strlen
 add sp,2
 push ax
 push word ptr [bp+6]
 mov ax,2
 push ax
 call far ptr _write
 add sp,6
 pop bp
 retf
l_dos_creat LABEL FAR
 PUBLIC __dos_creat
__dos_creat LABEL FAR
 push bp
 mov bp,sp
 mov dx,[bp+6]
 mov cx,[bp+8]
 mov ah,3ch
 int 21h
 jc l_dc_err
 mov bx,[bp+0ah]
 mov [bx],ax
 xor ax,ax
 jmp short l_dc_out
l_dc_err:
 mov [_errno],ax
l_dc_out:
 pop bp
 retf
l_strnicmp LABEL FAR
 PUBLIC _strnicmp
_strnicmp LABEL FAR
 db 055h,08bh,0ech,057h,056h,08bh,076h,006h,08bh,07eh,008h,01eh,007h,08bh,04eh,00ah
 db 0e3h,03dh,0b7h,041h,0b3h,05ah,0b6h,020h,08ah,024h,08ah,005h,00ah,0e4h,074h,020h
 db 00ah,0c0h,074h,01ch,046h,047h,03ah,0e7h,072h,006h,03ah,0e3h,077h,002h,002h,0e6h
 db 03ah,0c7h,072h,006h,03ah,0c3h,077h,002h,002h,0c6h,03ah,0e0h,075h,008h,0e2h,0d8h
 db 033h,0c9h,03ah,0e0h,074h,009h,0b9h,000h,000h,072h,002h,049h,049h,0f7h,0d1h,08bh
 db 0c1h,05eh,05fh,08bh,0e5h,05dh,0cbh
l_intdosx LABEL FAR
 PUBLIC _intdosx
_intdosx LABEL FAR
 db 055h,08bh,0ech,056h,057h,08bh,07eh,006h,08bh,005h,08bh,05dh,002h,08bh,04dh,004h
 db 08bh,055h,006h,08bh,075h,008h,08bh,07dh,00ah,0cdh,021h,057h,08bh,07eh,008h,089h
 db 005h,089h,05dh,002h,089h,04dh,004h,089h,055h,006h,089h,075h,008h,08fh,045h,00ah
 db 072h,004h,033h,0f6h,0ebh,002h,0beh,001h,000h,089h,075h,00ch,05fh,05eh,08bh,0e5h
 db 05dh,0cbh
AUX2_TEXT ENDS
EXTRN _strlen:FAR,_write:FAR
END
