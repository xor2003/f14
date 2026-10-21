; MPSLIBM.ASM - reconstructed MicroProse graphics/mouse/overlay/PIC library
; Verbatim port of retail F14.EXE CODE_9 + CODE_69 (picload) segments.
; Readable-ASM form: decoded from the byte lift by tools/mpsreal_readable.py
; (MASM 5.10-compatible mnemonics, symbolic labels, real relocations).
DGROUP GROUP MPSDAT
.286
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
	push	05bf7h
	pop	si
	pop	di
	leave
	retf
	db	000h
g5ce8:
	push	bp
g5ce9:
	mov	bp, sp
g5ceb:
	push	si
g5cec:
	push	di
g5ced:
	mov	di, word ptr [bp + 6]
g5cf0:
	mov	cx, word ptr [bp + 8]
g5cf3:
	mov	si, word ptr [bp + 0ah]
g5cf6:
	mov	bp, word ptr [bp + 0ch]
g5cf9:
	call far ptr g5d02
g5cfe:
	pop	di
g5cff:
	pop	si
g5d00:
	pop	bp
g5d01:
	retf
g5d02:
	mov	bx, cx
g5d04:
	call far ptr F_SinDP
g5d09:
	mov	word ptr ds:[0b7eh], bx
g5d0d:
	mov	bx, cx
g5d0f:
	call far ptr F_CosDP
g5d14:
	mov	word ptr ds:[0b80h], bx
g5d18:
	mov	bx, bp
g5d1a:
	call far ptr F_SinDP
g5d1f:
	mov	word ptr ds:[0b86h], bx
g5d23:
	mov	bx, bp
g5d25:
	call far ptr F_CosDP
g5d2a:
	mov	word ptr ds:[0b88h], bx
g5d2e:
	mov	bx, si
g5d30:
	call far ptr F_SinDP
g5d35:
	mov	word ptr ds:[0b82h], bx
g5d39:
	mov	bx, si
g5d3b:
	call far ptr F_CosDP
g5d40:
	mov	word ptr ds:[0b84h], bx
g5d44:
	mov	ax, word ptr ds:[0b82h]
g5d47:
	imul	word ptr ds:[0b86h]
g5d4b:
	shl	ax, 1
g5d4d:
	rcl	dx, 1
g5d4f:
	mov	si, dx
g5d51:
	mov	ax, dx
g5d53:
	imul	word ptr ds:[0b7eh]
g5d57:
	shl	ax, 1
g5d59:
	rcl	dx, 1
g5d5b:
	mov	bx, ax
g5d5d:
	mov	cx, dx
g5d5f:
	mov	ax, word ptr ds:[0b80h]
g5d62:
	imul	word ptr ds:[0b88h]
g5d66:
	shl	ax, 1
g5d68:
	rcl	dx, 1
g5d6a:
	add	ax, bx
g5d6c:
	adc	dx, cx
g5d6e:
	mov	word ptr [di], dx
g5d70:
	mov	ax, word ptr ds:[0b82h]
g5d73:
	imul	word ptr ds:[0b88h]
g5d77:
	shl	ax, 1
g5d79:
	rcl	dx, 1
g5d7b:
	mov	bp, dx
g5d7d:
	mov	ax, dx
g5d7f:
	imul	word ptr ds:[0b7eh]
g5d83:
	shl	ax, 1
g5d85:
	rcl	dx, 1
g5d87:
	mov	bx, ax
g5d89:
	mov	cx, dx
g5d8b:
	mov	ax, word ptr ds:[0b80h]
g5d8e:
	imul	word ptr ds:[0b86h]
g5d92:
	shl	ax, 1
g5d94:
	rcl	dx, 1
g5d96:
	sub	bx, ax
g5d98:
	sbb	cx, dx
g5d9a:
	mov	word ptr [di + 2], cx
g5d9d:
	mov	ax, word ptr ds:[0b7eh]
g5da0:
	imul	word ptr ds:[0b84h]
g5da4:
	shl	ax, 1
g5da6:
	rcl	dx, 1
g5da8:
	mov	word ptr [di + 4], dx
g5dab:
	mov	ax, word ptr ds:[0b86h]
g5dae:
	imul	word ptr ds:[0b84h]
g5db2:
	shl	ax, 1
g5db4:
	rcl	dx, 1
g5db6:
	mov	word ptr [di + 6], dx
g5db9:
	mov	ax, word ptr ds:[0b88h]
g5dbc:
	imul	word ptr ds:[0b84h]
g5dc0:
	shl	ax, 1
g5dc2:
	rcl	dx, 1
g5dc4:
	mov	word ptr [di + 8], dx
g5dc7:
	mov	ax, word ptr ds:[0b82h]
g5dca:
	neg	ax
g5dcc:
	mov	word ptr [di + 0ah], ax
g5dcf:
	mov	ax, si
g5dd1:
	imul	word ptr ds:[0b80h]
g5dd5:
	shl	ax, 1
g5dd7:
	rcl	dx, 1
g5dd9:
	mov	bx, ax
g5ddb:
	mov	cx, dx
g5ddd:
	mov	ax, word ptr ds:[0b7eh]
g5de0:
	imul	word ptr ds:[0b88h]
g5de4:
	shl	ax, 1
g5de6:
	rcl	dx, 1
g5de8:
	sub	bx, ax
g5dea:
	sbb	cx, dx
g5dec:
	mov	word ptr [di + 0ch], cx
g5def:
	mov	ax, bp
g5df1:
	imul	word ptr ds:[0b80h]
g5df5:
	shl	ax, 1
g5df7:
	rcl	dx, 1
g5df9:
	mov	bx, ax
g5dfb:
	mov	cx, dx
g5dfd:
	mov	ax, word ptr ds:[0b7eh]
g5e00:
	imul	word ptr ds:[0b86h]
g5e04:
	shl	ax, 1
g5e06:
	rcl	dx, 1
g5e08:
	add	ax, bx
g5e0a:
	adc	dx, cx
g5e0c:
	mov	word ptr [di + 0eh], dx
g5e0f:
	mov	ax, word ptr ds:[0b80h]
g5e12:
	imul	word ptr ds:[0b84h]
g5e16:
	shl	ax, 1
g5e18:
	rcl	dx, 1
g5e1a:
	mov	word ptr [di + 010h], dx
g5e1d:
	retf
g5e1e:
	mov bx,offset mat_b9c
g5e21:
	mov	ax, word ptr [bx + 2]
g5e24:
	xchg	word ptr [bx + 6], ax
g5e27:
	mov	word ptr [bx + 2], ax
g5e2a:
	mov	ax, word ptr [bx + 4]
g5e2d:
	xchg	word ptr [bx + 0ch], ax
g5e30:
	mov	word ptr [bx + 4], ax
g5e33:
	mov	ax, word ptr [bx + 0ah]
g5e36:
	xchg	word ptr [bx + 0eh], ax
g5e39:
	mov	word ptr [bx + 0ah], ax
g5e3c:
	retf
g5e3d:
	push	bp
g5e3e:
	mov	bp, sp
g5e40:
	push	si
g5e41:
	push	di
g5e42:
	mov	bx, word ptr [bp + 6]
g5e45:
	mov	si, word ptr [bp + 8]
g5e48:
	mov	di, word ptr [bp + 0ah]
g5e4b:
	call far ptr g5e54
g5e50:
	pop	di
g5e51:
	pop	si
g5e52:
	pop	bp
g5e53:
	retf
g5e54:
	mov	ax, word ptr [bx]
g5e56:
	imul	word ptr [si]
g5e58:
	shl	ax, 1
g5e5a:
	rcl	dx, 1
g5e5c:
	mov	bp, ax
g5e5e:
	mov	cx, dx
g5e60:
	mov	ax, word ptr [bx + 2]
g5e63:
	imul	word ptr [si + 6]
g5e66:
	shl	ax, 1
g5e68:
	rcl	dx, 1
g5e6a:
	add	bp, ax
g5e6c:
	adc	cx, dx
g5e6e:
	mov	ax, word ptr [bx + 4]
g5e71:
	imul	word ptr [si + 0ch]
g5e74:
	shl	ax, 1
g5e76:
	rcl	dx, 1
g5e78:
	add	bp, ax
g5e7a:
	adc	cx, dx
g5e7c:
	mov	word ptr [di], cx
g5e7e:
	mov	ax, word ptr [bx]
g5e80:
	imul	word ptr [si + 2]
g5e83:
	shl	ax, 1
g5e85:
	rcl	dx, 1
g5e87:
	mov	bp, ax
g5e89:
	mov	cx, dx
g5e8b:
	mov	ax, word ptr [bx + 2]
g5e8e:
	imul	word ptr [si + 8]
g5e91:
	shl	ax, 1
g5e93:
	rcl	dx, 1
g5e95:
	add	bp, ax
g5e97:
	adc	cx, dx
g5e99:
	mov	ax, word ptr [bx + 4]
g5e9c:
	imul	word ptr [si + 0eh]
g5e9f:
	shl	ax, 1
g5ea1:
	rcl	dx, 1
g5ea3:
	add	bp, ax
g5ea5:
	adc	cx, dx
g5ea7:
	mov	word ptr [di + 2], cx
g5eaa:
	mov	ax, word ptr [bx]
g5eac:
	imul	word ptr [si + 4]
g5eaf:
	shl	ax, 1
g5eb1:
	rcl	dx, 1
g5eb3:
	mov	bp, ax
g5eb5:
	mov	cx, dx
g5eb7:
	mov	ax, word ptr [bx + 2]
g5eba:
	imul	word ptr [si + 0ah]
g5ebd:
	shl	ax, 1
g5ebf:
	rcl	dx, 1
g5ec1:
	add	bp, ax
g5ec3:
	adc	cx, dx
g5ec5:
	mov	ax, word ptr [bx + 4]
g5ec8:
	imul	word ptr [si + 010h]
g5ecb:
	shl	ax, 1
g5ecd:
	rcl	dx, 1
g5ecf:
	add	bp, ax
g5ed1:
	adc	cx, dx
g5ed3:
	mov	word ptr [di + 4], cx
g5ed6:
	mov	ax, word ptr [bx + 6]
g5ed9:
	imul	word ptr [si]
g5edb:
	shl	ax, 1
g5edd:
	rcl	dx, 1
g5edf:
	mov	bp, ax
g5ee1:
	mov	cx, dx
g5ee3:
	mov	ax, word ptr [bx + 8]
g5ee6:
	imul	word ptr [si + 6]
g5ee9:
	shl	ax, 1
g5eeb:
	rcl	dx, 1
g5eed:
	add	bp, ax
g5eef:
	adc	cx, dx
g5ef1:
	mov	ax, word ptr [bx + 0ah]
g5ef4:
	imul	word ptr [si + 0ch]
g5ef7:
	shl	ax, 1
g5ef9:
	rcl	dx, 1
g5efb:
	add	bp, ax
g5efd:
	adc	cx, dx
g5eff:
	mov	word ptr [di + 6], cx
g5f02:
	mov	ax, word ptr [bx + 6]
g5f05:
	imul	word ptr [si + 2]
g5f08:
	shl	ax, 1
g5f0a:
	rcl	dx, 1
g5f0c:
	mov	bp, ax
g5f0e:
	mov	cx, dx
g5f10:
	mov	ax, word ptr [bx + 8]
g5f13:
	imul	word ptr [si + 8]
g5f16:
	shl	ax, 1
g5f18:
	rcl	dx, 1
g5f1a:
	add	bp, ax
g5f1c:
	adc	cx, dx
g5f1e:
	mov	ax, word ptr [bx + 0ah]
g5f21:
	imul	word ptr [si + 0eh]
g5f24:
	shl	ax, 1
g5f26:
	rcl	dx, 1
g5f28:
	add	bp, ax
g5f2a:
	adc	cx, dx
g5f2c:
	mov	word ptr [di + 8], cx
g5f2f:
	mov	ax, word ptr [bx + 6]
g5f32:
	imul	word ptr [si + 4]
g5f35:
	shl	ax, 1
g5f37:
	rcl	dx, 1
g5f39:
	mov	bp, ax
g5f3b:
	mov	cx, dx
g5f3d:
	mov	ax, word ptr [bx + 8]
g5f40:
	imul	word ptr [si + 0ah]
g5f43:
	shl	ax, 1
g5f45:
	rcl	dx, 1
g5f47:
	add	bp, ax
g5f49:
	adc	cx, dx
g5f4b:
	mov	ax, word ptr [bx + 0ah]
g5f4e:
	imul	word ptr [si + 010h]
g5f51:
	shl	ax, 1
g5f53:
	rcl	dx, 1
g5f55:
	add	bp, ax
g5f57:
	adc	cx, dx
g5f59:
	mov	word ptr [di + 0ah], cx
g5f5c:
	mov	ax, word ptr [bx + 0ch]
g5f5f:
	imul	word ptr [si]
g5f61:
	shl	ax, 1
g5f63:
	rcl	dx, 1
g5f65:
	mov	bp, ax
g5f67:
	mov	cx, dx
g5f69:
	mov	ax, word ptr [bx + 0eh]
g5f6c:
	imul	word ptr [si + 6]
g5f6f:
	shl	ax, 1
g5f71:
	rcl	dx, 1
g5f73:
	add	bp, ax
g5f75:
	adc	cx, dx
g5f77:
	mov	ax, word ptr [bx + 010h]
g5f7a:
	imul	word ptr [si + 0ch]
g5f7d:
	shl	ax, 1
g5f7f:
	rcl	dx, 1
g5f81:
	add	bp, ax
g5f83:
	adc	cx, dx
g5f85:
	mov	word ptr [di + 0ch], cx
g5f88:
	mov	ax, word ptr [bx + 0ch]
g5f8b:
	imul	word ptr [si + 2]
g5f8e:
	shl	ax, 1
g5f90:
	rcl	dx, 1
g5f92:
	mov	bp, ax
g5f94:
	mov	cx, dx
g5f96:
	mov	ax, word ptr [bx + 0eh]
g5f99:
	imul	word ptr [si + 8]
g5f9c:
	shl	ax, 1
g5f9e:
	rcl	dx, 1
g5fa0:
	add	bp, ax
g5fa2:
	adc	cx, dx
g5fa4:
	mov	ax, word ptr [bx + 010h]
g5fa7:
	imul	word ptr [si + 0eh]
g5faa:
	shl	ax, 1
g5fac:
	rcl	dx, 1
g5fae:
	add	bp, ax
g5fb0:
	adc	cx, dx
g5fb2:
	mov	word ptr [di + 0eh], cx
g5fb5:
	mov	ax, word ptr [bx + 0ch]
g5fb8:
	imul	word ptr [si + 4]
g5fbb:
	shl	ax, 1
g5fbd:
	rcl	dx, 1
g5fbf:
	mov	bp, ax
g5fc1:
	mov	cx, dx
g5fc3:
	mov	ax, word ptr [bx + 0eh]
g5fc6:
	imul	word ptr [si + 0ah]
g5fc9:
	shl	ax, 1
g5fcb:
	rcl	dx, 1
g5fcd:
	add	bp, ax
g5fcf:
	adc	cx, dx
g5fd1:
	mov	ax, word ptr [bx + 010h]
g5fd4:
	imul	word ptr [si + 010h]
g5fd7:
	shl	ax, 1
g5fd9:
	rcl	dx, 1
g5fdb:
	add	bp, ax
g5fdd:
	adc	cx, dx
g5fdf:
	mov	word ptr [di + 010h], cx
g5fe2:
	retf
	db	000h
g5fe4:
	cmp byte ptr [dg_drawn],00h
g5fe9:
	jne	g5ff7
g5feb:
	pop	word ptr cs:[cs_j1]
g5ff0:
	jmp short	g5ff2
g5ff2:
	db	02eh
	db	0ffh
	db	02eh
	PUBLIC	_MouseX
	PUBLIC	_MouseY
	PUBLIC	_Button
	PUBLIC	_MOUSE
cs_j1	LABEL BYTE
	db	000h,000h
g5ff7:
	mov byte ptr [dg_lock],01h
g5ffc:
	pop	word ptr cs:[cs_j2]
g6001:
	pop word ptr [dg_ret0]
g6005:
	pop word ptr [dg_ret2]
g6009:
	jmp short	g600b
g600b:
	db	02eh
	db	0ffh
	push	ds
cs_j2	LABEL BYTE
	db	000h,000h
g6010:
	dec byte ptr [dg_lock]
g6014:
	je	g601b
g6016:
	call	g6b22
g6019:
	jmp short	g6010
g601b:
	jmp dword ptr [dg_ret0]
g601f:
	db	000h,000h
	PUBLIC	_AllocGraphicPage
_AllocGraphicPage	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_AvailSysMem
_AvailSysMem	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_BuildSprite
_BuildSprite	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_ChangeColor
_ChangeColor	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_CharWidth
_CharWidth	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_ClearPage
_ClearPage	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_ClipSprite
_ClipSprite	LABEL FAR
	call	g5fe4
g604c:
	db	000h,000h
g604e:
	db	000h,000h
g6050:
	db	000h,000h
g6052:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_CloseSprite
_CloseSprite	LABEL FAR
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
g605e:
	call	g5fe4
	db	000h,000h
	db	000h,000h
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g606e:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6075:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_FontHeight
_FontHeight	LABEL FAR
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h
g6088:
	call	g5fe4
g608b:
	db	000h,000h
g608d:
	db	000h,000h
g608f:
	db	000h,000h
g6091:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6098:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_LIBCANDO
_LIBCANDO	LABEL FAR
	db	000h,000h
g609f:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_OpenSprite
_OpenSprite	LABEL FAR
	db	000h,000h
g60a6:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g60ad:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_RectCopy
_RectCopy	LABEL FAR
	db	000h,000h
g60b4:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_RpPrint
_RpPrint	LABEL FAR
	call	g5fe4
	db	000h,000h
	db	000h,000h
	db	000h,000h
g60c2:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_ScreenOff
_ScreenOff	LABEL FAR
	db	000h,000h
g60c9:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_ScreenOn
_ScreenOn	LABEL FAR
	db	000h,000h
g60d0:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_SetGraphicPage
_SetGraphicPage	LABEL FAR
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h,000h,000h
g60e5:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_Shake
_Shake	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
g60f1:
	call	g5fe4
g60f4:
	db	000h,000h
g60f6:
	db	000h,000h
	PUBLIC	_ShowPage
_ShowPage	LABEL FAR
	call	g5fe4
g60fb:
	db	000h,000h
g60fd:
	db	000h,000h
g60ff:
	call	g5fe4
g6102:
	db	000h,000h
g6104:
	db	000h,000h
g6106:
	db	000h,000h
g6108:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g610f:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6116:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h,000h,000h
	PUBLIC	_TurnOnGraphicsMode
_TurnOnGraphicsMode	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g612b:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6132:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6139:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6140:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6147:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
	db	0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h
	db	000h,000h,000h,000h,000h
g6171:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
g6178:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h
	db	000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah
	db	000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h
	db	000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h
	db	000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h,000h
	PUBLIC	_SndSysSetup
_SndSysSetup	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_SndSounds
_SndSounds	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	PUBLIC	_SndShutdown
_SndShutdown	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
	PUBLIC	_GETTIME
_GETTIME	LABEL FAR
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h
	db	0eah,000h,000h,000h,000h,000h,000h,0eah,000h,000h,000h,000h,000h,000h,0eah,000h
	db	000h,000h,000h
	PUBLIC	_LoadOverlay
_LoadOverlay	LABEL FAR
	push	bp
g6329:
	mov	bp, sp
g632b:
	mov	ah, 048h
g632d:
	mov	bx, 0ffffh
g6330:
	int	021h
g6332:
	jb	g633b
g6334:
	lea si,[mps_e1]
g6338:
	jmp	g648d
g633b:
	mov	ah, 048h
g633d:
	sub	bx, 0100h
g6341:
	mov [mps_freep],bx
g6345:
	int	021h
g6347:
	jae	g6350
g6349:
	lea si,[mps_e2]
g634d:
	jmp	g648d
g6350:
	push	ds
g6351:
	pop	es
g6352:
	lea bx,[mps_exec0]
g6356:
	mov	word ptr [bx], ax
g6358:
	mov	word ptr [bx + 2], ax
g635b:
	mov	ax, 04b03h
g635e:
	mov	dx, word ptr [bp + 6]
g6361:
	call	g646c
g6364:
	jae	g6387
g6366:
	cmp	ax, 2
g6369:
	jne	g6372
g636b:
	lea si,[mps_e3]
g636f:
	jmp	g648d
g6372:
	cmp	ax, 8
g6375:
	jne	g637e
g6377:
	lea si,[mps_e4]
g637b:
	jmp	g648d
g637e:
	je	g6387
g6380:
	lea si,[mps_e5]
g6384:
	jmp	g648d
g6387:
	mov es,[mps_exec0]
g638b:
	mov	bx, word ptr es:[02ah]
g6390:
	mov	ax, word ptr es:[02ch]
g6394:
	add	ax, 0fh
g6397:
	rcr	ax, 1
g6399:
	shr	ax, 1
g639b:
	shr	ax, 1
g639d:
	shr	ax, 1
g639f:
	add	bx, ax
g63a1:
	mov	dx, word ptr [bp + 8]
g63a4:
	or	dx, dx
g63a6:
	je	g63ab
g63a8:
	call	g63d0
g63ab:
	mov	ax, es
g63ad:
	sub	bx, ax
g63af:
	add	bx, 8
g63b2:
	cmp bx,[mps_freep]
g63b6:
	jbe	g63bf
g63b8:
	lea si,[mps_e6]
g63bc:
	jmp	g648d
g63bf:
	mov	ah, 04ah
g63c1:
	int	021h
g63c3:
	jae	g63cc
g63c5:
	lea si,[mps_e7]
g63c9:
	jmp	g648d
g63cc:
	mov	ax, es
g63ce:
	pop	bp
g63cf:
	retf
g63d0:
	push	ds
g63d1:
	mov	ax, 03d00h
g63d4:
	int	021h
g63d6:
	jae	g63df
g63d8:
	lea si,[mps_e3]
g63dc:
	jmp	g648d
g63df:
	mov	ds, bx
g63e1:
	mov	bx, ax
g63e3:
	mov	ah, 03fh
g63e5:
	xor	dx, dx
g63e7:
	mov	cx, 08000h
g63ea:
	int	021h
g63ec:
	jae	g63f5
g63ee:
	lea si,[mps_e3]
g63f2:
	jmp	g648d
g63f5:
	cmp	ax, cx
g63f7:
	jb	g6402
g63f9:
	mov	ax, ds
g63fb:
	add	ax, 0800h
g63fe:
	mov	ds, ax
g6400:
	jmp short	g63e3
g6402:
	add	ax, 0fh
g6405:
	rcr	ax, 1
g6407:
	shr	ax, 1
g6409:
	shr	ax, 1
g640b:
	shr	ax, 1
g640d:
	mov	dx, ds
g640f:
	add	dx, ax
g6411:
	mov	ah, 03eh
g6413:
	int	021h
g6415:
	jae	g641e
g6417:
	lea si,[mps_e3]
g641b:
	jmp short	g648d
	db	090h
g641e:
	mov	bx, dx
g6420:
	pop	ds
g6421:
	ret
	PUBLIC	_InitOverlay
_InitOverlay	LABEL FAR
	push	bp
g6423:
	mov	bp, sp
g6425:
	push	si
g6426:
	push	di
g6427:
	push	ds
g6428:
	mov	ds, word ptr [bp + 6]
g642b:
	mov	si, 032h
g642e:
	mov	ax, word ptr ds:[02eh]
g6431:
	mov	dl, 7
g6433:
	mul	dl
g6435:
	lea di,[g601f]
g6439:
	add	di, ax
g643b:
	mov ax,seg g5ce0
g643e:
	mov	es, ax
g6440:
	mov	cx, word ptr ds:[030h]
g6444:
	mov	ax, word ptr ds:[028h]
g6447:
	add	di, 3
g644a:
	movsw	word ptr es:[di], word ptr [si]
g644b:
	stosw	word ptr es:[di], ax
g644c:
	loop	g6447
g644e:
	pop	ds
g644f:
	pop	di
g6450:
	pop	si
g6451:
	pop	bp
g6452:
	retf
g6453:
	push	bp
g6454:
	mov	bp, sp
g6456:
	mov	es, word ptr [bp + 6]
g6459:
	mov	ah, 049h
g645b:
	int	021h
g645d:
	jae	g6466
g645f:
	lea si,[mps_e8]
g6463:
	jmp short	g648d
	db	090h
g6466:
	pop	bp
g6467:
	retf
cs_sp	LABEL BYTE
	db	000h,000h
cs_ss	LABEL BYTE
	db	000h,000h
g646c:
	push	bp
g646d:
	push	si
g646e:
	push	di
g646f:
	push	ds
g6470:
	push	es
g6471:
	mov	word ptr cs:[0], ss
g6476:
	mov	word ptr cs:[0], sp
g647b:
	int	021h
g647d:
	mov	ss, word ptr cs:[cs_ss]
g6482:
	mov	sp, word ptr cs:[cs_sp]
g6487:
	pop	es
g6488:
	pop	ds
g6489:
	pop	di
g648a:
	pop	si
g648b:
	pop	bp
g648c:
	ret
g648d:
	mov	ah, 2
g648f:
	cmp	al, 0ah
g6491:
	sbb	al, 069h
g6493:
	das
g6494:
	mov	dl, al
g6496:
	int	021h
g6498:
	lea dx,[mps_dash]
g649c:
	mov	ah, 9
g649e:
	int	021h
g64a0:
	mov	ah, 9
g64a2:
	mov	dx, si
g64a4:
	int	021h
g64a6:
	mov	ax, 04c99h
g64a9:
	int	021h
g64ab:
	db	000h
	PUBLIC	_RectFill
_RectFill	LABEL FAR
	push	bp
g64ad:
	mov	bp, sp
g64af:
	push	di
g64b0:
	push	si
g64b1:
	xor	ax, ax
g64b3:
	cmp	word ptr [bp + 0ch], ax
g64b6:
	jle	g651f
g64b8:
	cmp	word ptr [bp + 0eh], ax
g64bb:
	jle	g651f
g64bd:
	call	g6540
g64c0:
	mov	bx, word ptr [bp + 6]
g64c3:
	mov	ax, word ptr [bx]
g64c5:
	call far ptr g6171
g64ca:
	mov	si, word ptr [bp + 0ah]
g64cd:
	add	si, word ptr [bx + 4]
g64d0:
	mov [mps_penx],si
g64d4:
	mov	cx, word ptr [bp + 0eh]
g64d7:
	shl	si, 1
g64d9:
	lea di,[mps_span0]
g64dd:
	add	di, si
g64df:
	mov	ax, word ptr [bp + 8]
g64e2:
	add	ax, word ptr [bx + 2]
g64e5:
	mov	dx, cx
g64e7:
	rep stosw	word ptr es:[di], ax
g64e9:
	mov	cx, dx
g64eb:
	lea di,[mps_span1]
g64ef:
	add	di, si
g64f1:
	mov	ax, word ptr [bp + 8]
g64f4:
	add	ax, word ptr [bx + 2]
g64f7:
	add	ax, word ptr [bp + 0ch]
g64fa:
	dec	ax
g64fb:
	rep stosw	word ptr es:[di], ax
g64fd:
	mov	ax, word ptr [bp + 010h]
g6500:
	call far ptr g6140
g6505:
	mov ax,[mps_penx]
g6508:
	mov	cx, ax
g650a:
	add	cx, word ptr [bp + 0eh]
g650d:
	dec	cx
g650e:
	mov [mps_peny],cx
g6512:
	mov	bx, 05740h
g6515:
	call far ptr g6132
g651a:
	call far ptr g6139
g651f:
	pop	si
g6520:
	pop	di
g6521:
	pop	bp
g6522:
	retf
	db	000h,057h,056h,055h,0a1h,064h,05ah,08bh,00eh,066h,05ah,0bbh,040h,057h,09ah,052h
	db	004h,0ceh,005h,05dh,05eh,05fh,0cbh,057h,0e8h,002h,000h,05fh,0cbh
g6540:
	mov ax,DGROUP
g6543:
	mov	es, ax
g6545:
	mov di,[mps_penx]
g6549:
	or	di, di
g654b:
	js	g6575
g654d:
	mov cx,[mps_peny]
g6551:
	inc	cx
g6552:
	sub	cx, di
g6554:
	shl	di, 1
g6556:
	mov	bx, cx
g6558:
	mov	dx, di
g655a:
	add	di, 05740h
g655e:
	mov	ax, 0ffffh
g6561:
	rep stosw	word ptr es:[di], ax
g6563:
	mov [mps_penx],ax
g6566:
	mov	cx, bx
g6568:
	mov	di, dx
g656a:
	add	di, 058d0h
g656e:
	sub	ax, ax
g6570:
	rep stosw	word ptr es:[di], ax
g6572:
	mov [mps_peny],ax
g6575:
	ret
g6576:
	mov ax,DGROUP
g6579:
	mov	es, ax
g657b:
	mov	cx, di
g657d:
	or	si, si
g657f:
	lea di,[mps_span0]
g6583:
	je	g658f
g6585:
	cmp si,[mps_clipR]
g6589:
	jne	g65d0
g658b:
	lea di,[mps_span1]
g658f:
	cmp	cx, dx
g6591:
	jge	g6595
g6593:
	xchg	cx, dx
g6595:
	or	dx, dx
g6597:
	jns	g659b
g6599:
	sub	dx, dx
g659b:
	cmp cx,[mps_clipB]
g659f:
	jle	g65a5
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
	jbe	g65bb
g65b7:
	mov [mps_peny],cx
g65bb:
	cmp dx,[mps_penx]
g65bf:
	jae	g65c5
g65c1:
	mov [mps_penx],dx
g65c5:
	add	di, dx
g65c7:
	add	di, dx
g65c9:
	sub	cx, dx
g65cb:
	inc	cx
g65cc:
	mov	ax, si
g65ce:
	rep stosw	word ptr es:[di], ax
g65d0:
	ret
g65d1:
	sub	si, si
g65d3:
	or	bx, bx
g65d5:
	js	g6576
g65d7:
	mov si,[mps_clipR]
g65db:
	cmp	bx, si
g65dd:
	jg	g6576
g65df:
	ret
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
	jae	g6618
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
	mov	dx, ax
g6629:
	mov cx,[mps_y1]
g662d:
	sub	ax, cx
g662f:
	mov	bp, 2
g6632:
	jns	g663a
g6634:
	xchg	cx, dx
g6636:
	neg	bp
g6638:
	neg	ax
g663a:
	mov [ln_tmp1],ax
g663d:
	cmp dx,[mps_peny]
g6641:
	jbe	g6647
g6643:
	mov [mps_peny],dx
g6647:
	cmp cx,[mps_penx]
g664b:
	jae	g6651
g664d:
	mov [mps_penx],cx
g6651:
	cmp	ax, bx
g6653:
	jae	g66a0
g6655:
	mov di,[mps_y1]
g6659:
	shl	di, 1
g665b:
	mov ax,[mps_x1]
g665e:
	mov dx,[ln_tmp0]
g6662:
	mov	cx, dx
g6664:
	mov	bx, cx
g6666:
	inc	bx
g6667:
	shr	bx, 1
g6669:
	neg	bx
g666b:
	mov si,[ln_tmp1]
g666f:
	jmp short	g6681
	db	090h
g6672:
	cmp	ax, word ptr [di + 058d0h]
g6676:
	jbe	g667c
g6678:
	mov	word ptr [di + 058d0h], ax
g667c:
	inc	ax
g667d:
	sub	bx, dx
g667f:
	add	di, bp
g6681:
	cmp	ax, word ptr [di + 05740h]
g6685:
	jae	g668b
g6687:
	mov	word ptr [di + 05740h], ax
g668b:
	dec	cx
g668c:
	js	g6695
g668e:
	add	bx, si
g6690:
	jns	g6672
g6692:
	inc	ax
g6693:
	jmp short	g668b
g6695:
	cmp	ax, word ptr [di + 058d0h]
g6699:
	jbe	g669f
g669b:
	mov	word ptr [di + 058d0h], ax
g669f:
	ret
g66a0:
	mov di,[mps_y1]
g66a4:
	shl	di, 1
g66a6:
	mov ax,[mps_x1]
g66a9:
	mov dx,[ln_tmp1]
g66ad:
	mov	cx, dx
g66af:
	mov	bx, cx
g66b1:
	inc	bx
g66b2:
	shr	bx, 1
g66b4:
	neg	bx
g66b6:
	mov si,[ln_tmp0]
g66ba:
	cmp	ax, word ptr [di + 05740h]
g66be:
	jae	g66c4
g66c0:
	mov	word ptr [di + 05740h], ax
g66c4:
	cmp	ax, word ptr [di + 058d0h]
g66c8:
	jbe	g66ce
g66ca:
	mov	word ptr [di + 058d0h], ax
g66ce:
	dec	cx
g66cf:
	js	g66dc
g66d1:
	add	di, bp
g66d3:
	add	bx, si
g66d5:
	js	g66ba
g66d7:
	sub	bx, dx
g66d9:
	inc	ax
g66da:
	jmp short	g66ba
g66dc:
	ret
	db	000h,0e8h,00dh,000h,0cbh,055h,056h,057h,006h,0e8h,005h,000h,007h,05fh,05eh,05dh
	db	0cbh
g66ee:
	sub	ax, ax
g66f0:
	mov	es, ax
g66f2:
	push	word ptr es:[0]
g66f7:
	push	word ptr es:[2]
g66fc:
	lea	ax, ds:[0bbbh]
g6700:
	mov	word ptr es:[0], ax
g6704:
	mov word ptr es:[2],seg g5ce0
g670b:
	push	ds
g670c:
	pop	es
g670d:
	jmp short	g6776
	db	090h
g6710:
	sub	ax, ax
g6712:
	mov	es, ax
g6714:
	pop	word ptr es:[2]
g6719:
	pop	word ptr es:[0]
g671e:
	push	ds
g671f:
	pop	es
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
	clc
g6735:
	ret
g6736:
	sub	ax, ax
g6738:
	mov	es, ax
g673a:
	pop	word ptr es:[2]
g673f:
	pop	word ptr es:[0]
g6744:
	push	ds
g6745:
	pop	es
g6746:
	stc
g6747:
	ret
g6748:
	cmc
g6749:
	rcr	dx, 1
g674b:
	mov [eg_dx],dx
g674f:
	sar	dx, 1
g6751:
	mov [eg_cx],dx
g6755:
	mov	dx, di
g6757:
	sub	dx, bp
g6759:
	jno	g6761
g675b:
	cmc
g675c:
	rcr	dx, 1
g675e:
	jmp short	g67d1
	db	090h
g6761:
	sar	dx, 1
g6763:
	jmp short	g67d1
	db	090h
g6766:
	cmc
g6767:
	rcr	dx, 1
g6769:
	sar word ptr [eg_dx],1
g676d:
	sar word ptr [eg_cx],1
g6771:
	jmp short	g67d1
	db	090h
g6774:
	jmp short	g6736
g6776:
	mov cx,[mps_x1]
g677a:
	mov dx,[mps_y1]
g677e:
	mov si,[mps_x2]
g6782:
	mov di,[mps_y2]
g6786:
	mov	bx, cx
g6788:
	mov	bp, dx
g678a:
	call	g687a
g678d:
	mov byte ptr [eg_flag],al
g6790:
	mov	bx, si
g6792:
	mov	bp, di
g6794:
	call	g687a
g6797:
	jne	g67b3
g6799:
	cmp byte ptr [eg_flag],00h
g679e:
	jne	g67a3
g67a0:
	jmp	g6710
g67a3:
	xchg	cx, si
g67a5:
	xchg	dx, di
g67a7:
	xchg al,[eg_flag]
g67ab:
	mov [mps_x1],cx
g67af:
	mov [mps_y1],dx
g67b3:
	test byte ptr [eg_flag],al
g67b7:
	jne	g6774
g67b9:
	mov	bp, dx
g67bb:
	mov	dx, si
g67bd:
	sub	dx, cx
g67bf:
	jo	g6748
g67c1:
	mov [eg_dx],dx
g67c5:
	sar	dx, 1
g67c7:
	mov [eg_cx],dx
g67cb:
	mov	dx, di
g67cd:
	sub	dx, bp
g67cf:
	jo	g6766
g67d1:
	mov [eg_dy],dx
g67d5:
	sar	dx, 1
g67d7:
	mov [eg_cy],dx
g67db:
	test	al, 9
g67dd:
	je	g6817
g67df:
	sub	bx, bx
g67e1:
	or	si, si
g67e3:
	js	g67e9
g67e5:
	mov bx,[mps_clipR]
g67e9:
	mov	ax, bx
g67eb:
	sub	ax, cx
g67ed:
	imul word ptr [eg_dy]
g67f1:
	push	bx
g67f2:
	mov	bx, dx
g67f4:
	idiv word ptr [eg_dx]
g67f8:
	mov	bl, bh
g67fa:
	xor bl,[eg_s0]
g67fe:
	jns	g6803
g6800:
	neg	dx
g6802:
	dec	ax
g6803:
	sub dx,[eg_cx]
g6807:
	xor	dh, bh
g6809:
	js	g680c
g680b:
	inc	ax
g680c:
	pop	bx
g680d:
	add	ax, bp
g680f:
	js	g681f
g6811:
	cmp ax,[mps_clipB]
g6815:
	jle	g6850
g6817:
	mov bx,[mps_clipB]
g681b:
	cmp	di, bx
g681d:
	jg	g6821
g681f:
	sub	bx, bx
g6821:
	mov	ax, bx
g6823:
	sub	ax, bp
g6825:
	imul word ptr [eg_dx]
g6829:
	push	bx
g682a:
	mov	bx, dx
g682c:
	idiv word ptr [eg_dy]
g6830:
	mov	bl, bh
g6832:
	xor bl,[eg_s1]
g6836:
	jns	g683b
g6838:
	neg	dx
g683a:
	dec	ax
g683b:
	sub dx,[eg_cy]
g683f:
	xor	dh, bh
g6841:
	js	g6844
g6843:
	inc	ax
g6844:
	pop	bx
g6845:
	add	ax, cx
g6847:
	js	g6861
g6849:
	cmp ax,[mps_clipR]
g684d:
	jg	g6861
g684f:
	xchg	bx, ax
g6850:
	cmp byte ptr [eg_flag],00h
g6855:
	jne	g6864
g6857:
	mov [mps_y2],ax
g685a:
	mov [mps_x2],bx
g685e:
	jmp	g6710
g6861:
	jmp	g6736
g6864:
	mov [mps_y1],ax
g6867:
	mov [mps_x1],bx
g686b:
	xchg	cx, si
g686d:
	xchg	bp, di
g686f:
	mov al,[eg_flag]
g6872:
	mov byte ptr [eg_flag],00h
g6877:
	jmp	g67db
g687a:
	mov	al, 0fh
g687c:
	or	bx, bx
g687e:
	js	g6882
g6880:
	and	al, 0f7h
g6882:
	cmp bx,[mps_clipR]
g6886:
	jg	g688a
g6888:
	and	al, 0feh
g688a:
	or	bp, bp
g688c:
	js	g6890
g688e:
	and	al, 0fbh
g6890:
	cmp bp,[mps_clipB]
g6894:
	jg	g6898
g6896:
	and	al, 0fdh
g6898:
	or	al, al
g689a:
	ret
g689b:
	push	bp
g689c:
	mov	bp, sp
g689e:
	cmp	word ptr [bp + 2], 0b4ch
g68a3:
	mov ax,[eg_dy]
g68a6:
	je	g68ab
g68a8:
	mov ax,[eg_dx]
g68ab:
	xor	dx, ax
g68ad:
	mov	ax, 07f00h
g68b0:
	jns	g68b4
g68b2:
	neg	ax
g68b4:
	add	word ptr [bp + 2], 4
g68b8:
	sub	dx, dx
g68ba:
	pop	bp
g68bb:
	iret
	PUBLIC	_DrawLine
_DrawLine	LABEL FAR
	push	bp
g68bd:
	mov	bp, sp
g68bf:
	push	si
g68c0:
	push	di
g68c1:
	mov	bx, word ptr [bp + 6]
g68c4:
	mov	ax, word ptr [bx + 6]
g68c7:
	mov [mps_clipR],ax
g68ca:
	mov	ax, word ptr [bx + 8]
g68cd:
	mov [mps_clipB],ax
g68d0:
	call far ptr g6178
g68d5:
	mov	ax, word ptr [bp + 8]
g68d8:
	mov [mps_x1],ax
g68db:
	mov	ax, word ptr [bp + 0ah]
g68de:
	mov [mps_y1],ax
g68e1:
	mov	ax, word ptr [bp + 0ch]
g68e4:
	mov [mps_x2],ax
g68e7:
	mov	ax, word ptr [bp + 0eh]
g68ea:
	mov [mps_y2],ax
g68ed:
	mov	ax, word ptr [bp + 010h]
g68f0:
	call far ptr g6140
g68f5:
	call	g66ee
g68f8:
	call far ptr g6139
g68fd:
	pop	di
g68fe:
	pop	si
g68ff:
	pop	bp
g6900:
	retf
	PUBLIC	_DrawDot
_DrawDot	LABEL FAR
	push	bp
g6902:
	mov	bp, sp
g6904:
	push	si
g6905:
	push	di
g6906:
	mov	bx, word ptr [bp + 6]
g6909:
	mov	cx, word ptr [bp + 8]
g690c:
	or	cx, cx
g690e:
	js	g693c
g6910:
	cmp	cx, word ptr [bx + 6]
g6913:
	jg	g693c
g6915:
	mov	dx, word ptr [bp + 0ah]
g6918:
	or	dx, dx
g691a:
	js	g693c
g691c:
	cmp	dx, word ptr [bx + 8]
g691f:
	jg	g693c
g6921:
	call far ptr g6178
g6926:
	mov	ax, word ptr [bp + 0ch]
g6929:
	call far ptr g6140
g692e:
	mov	ax, cx
g6930:
	mov	bx, dx
g6932:
	call far ptr g612b
g6937:
	call far ptr g6139
g693c:
	pop	di
g693d:
	pop	si
g693e:
	pop	bp
g693f:
	retf
	PUBLIC	_DrawPoly
_DrawPoly	LABEL FAR
	push	bp
g6941:
	mov	bp, sp
g6943:
	push	es
g6944:
	push	ds
g6945:
	push	si
g6946:
	push	di
g6947:
	mov	ax, word ptr [bp + 0ch]
g694a:
	call far ptr g6140
g694f:
	mov	bx, word ptr [bp + 6]
g6952:
	call far ptr g6178
g6957:
	mov	ax, word ptr [bx + 2]
g695a:
	mov [mps_clipL],ax
g695d:
	mov	ax, word ptr [bx + 4]
g6960:
	mov [mps_clipT],ax
g6963:
	mov	ax, word ptr [bx + 6]
g6966:
	mov [mps_clipR],ax
g6969:
	mov	ax, word ptr [bx + 8]
g696c:
	mov [mps_clipB],ax
g696f:
	call	g6540
g6972:
	mov	si, word ptr [bp + 0ah]
g6975:
	mov	cx, word ptr [bp + 8]
g6978:
	dec	cx
g6979:
	lodsw	ax, word ptr [si]
g697a:
	mov [mps_x1],ax
g697d:
	push	ax
g697e:
	lodsw	ax, word ptr [si]
g697f:
	mov [mps_y1],ax
g6982:
	push	ax
g6983:
	push	cx
g6984:
	lodsw	ax, word ptr [si]
g6985:
	mov [mps_x2],ax
g6988:
	push	ax
g6989:
	lodsw	ax, word ptr [si]
g698a:
	mov [mps_y2],ax
g698d:
	push	ax
g698e:
	push	si
g698f:
	call	g6bbe
g6992:
	pop	si
g6993:
	pop word ptr [mps_y1]
g6997:
	pop word ptr [mps_x1]
g699b:
	pop	cx
g699c:
	loop	g6983
g699e:
	pop word ptr [mps_y2]
g69a2:
	pop word ptr [mps_x2]
g69a6:
	call	g6bbe
g69a9:
	mov ax,[mps_penx]
g69ac:
	mov cx,[mps_peny]
g69b0:
	mov	bx, 05740h
g69b3:
	call far ptr g6132
g69b8:
	call far ptr g6139
g69bd:
	pop	di
g69be:
	pop	si
g69bf:
	pop	ds
g69c0:
	pop	es
g69c1:
	pop	bp
g69c2:
	retf
	db	000h
	PUBLIC	_MouseON
_MouseON	LABEL FAR
	xor	ax, ax
g69c6:
	mov	es, ax
g69c8:
	mov	ax, word ptr es:[0cch]
g69cc:
	or	ax, word ptr es:[0ceh]
g69d1:
	je	g6a09
g69d3:
	mov	ax, 0
g69d6:
	int	033h
g69d8:
	or	ax, ax
g69da:
	je	g6a09
g69dc:
	mov	ax, 0ch
g69df:
	mov	cx, 01fh
g69e2:
	mov	dx, cs
g69e4:
	mov	es, dx
g69e6:
	lea	dx, ds:[0e83h]
g69ea:
	int	033h
g69ec:
	mov	ax, 3
g69ef:
	int	033h
g69f1:
	mov	ax, cx
g69f3:
	mov	cl, ch
g69f5:
	shr	ax, cl
g69f7:
	mov byte ptr [mps_xsh],cl
g69fb:
	mov [_MouseX],ax
g69fe:
	mov [_MouseY],dx
g6a02:
	mov [_Button],bx
g6a06:
	mov	ax, 0ffffh
g6a09:
	mov byte ptr [_MOUSE],al
g6a0c:
	retf
	PUBLIC	_MouseOFF
_MouseOFF	LABEL FAR
	xor	al, al
g6a0f:
	xchg al,[_MOUSE]
g6a13:
	or	al, al
g6a15:
	je	g6a1c
g6a17:
	mov	ax, 0
g6a1a:
	int	033h
g6a1c:
	retf
	PUBLIC	_MouseTYPE
_MouseTYPE	LABEL FAR
	push	bp
g6a1e:
	mov	bp, sp
g6a20:
	mov	ax, word ptr [bp + 6]
g6a23:
	mov [mps_hotx],ax
g6a26:
	mov	ax, word ptr [bp + 8]
g6a29:
	mov [mps_hoty],ax
g6a2c:
	mov	ax, word ptr [bp + 0ah]
g6a2f:
	mov [mps_curs],ax
g6a32:
	pop	bp
g6a33:
	retf
	PUBLIC	_MouseSPOT
_MouseSPOT	LABEL FAR
	push	bp
g6a35:
	mov	bp, sp
g6a37:
	mov	ax, word ptr [bp + 6]
g6a3a:
	mov	dx, word ptr [bp + 8]
g6a3d:
	mov [_MouseX],ax
g6a40:
	mov [_MouseY],dx
g6a44:
	cmp byte ptr [_MOUSE],00h
g6a49:
	je	g6a58
g6a4b:
	mov cl,[mps_xsh]
	db	0d3h,0e0h,08bh,0c8h,0b8h,004h,000h,0cdh,033h
g6a58:
	pop	bp
g6a59:
	retf
	PUBLIC	_MouseHITS
_MouseHITS	LABEL FAR
	xor	ax, ax
g6a5c:
	xchg ax,[mps_hits]
g6a60:
	retf
	PUBLIC	_MouseSHOW
_MouseSHOW	LABEL FAR
	cmp word ptr [mps_curs],+00h
g6a66:
	je	g6a90
g6a68:
	cmp byte ptr [dg_drawn],00h
g6a6d:
	jne	g6a90
g6a6f:
	push word ptr [mps_curs]
g6a73:
	mov ax,[_MouseY]
g6a76:
	sub ax,[mps_hoty]
g6a7a:
	push	ax
g6a7b:
	mov ax,[_MouseX]
g6a7e:
	sub ax,[mps_hotx]
g6a82:
	push	ax
g6a83:
	call far ptr g60f1
g6a88:
	add	sp, 6
g6a8b:
	mov byte ptr [dg_drawn],01h
g6a90:
	retf
	PUBLIC	_MouseHIDE
_MouseHIDE	LABEL FAR
	cmp word ptr [mps_curs],+00h
g6a96:
	je	g6aa9
g6a98:
	cmp byte ptr [dg_drawn],00h
g6a9d:
	je	g6aa9
g6a9f:
	mov byte ptr [dg_drawn],00h
g6aa4:
	call far ptr g6088
g6aa9:
	retf
cs_lock	LABEL BYTE
	add	byte ptr [di - 075h], dl
	in	al, dx
	cmp	word ptr ds:[05a82h], 0
	je	g6ad0
	cmp	byte ptr ds:[0560fh], 0
	je	g6ad0
	mov	al, byte ptr ds:[0560fh]
	mov	byte ptr ds:[0560fh], 0
	mov	byte ptr cs:[0dcah], al
	mov	ax, word ptr [bp + 6]
	db	09ah,07eh,003h,0ceh,005h	; far call 0x5ce, 0x37e
g6ad0:
	pop	bp
	retf
	db	083h,03eh,082h,05ah,000h,074h,014h,02eh
	db	080h,03eh,0cah,00dh,000h,074h,00ch,09ah,0a8h,003h,0ceh,005h,02eh,0a0h,0cah,00dh
	db	0a2h,00fh,056h,0cbh,055h,08bh,0ech,08bh,046h,006h,08bh,05eh,008h,08bh,04eh,00ah
	db	08bh,056h,00ch,0cdh,033h,0a3h,08eh,05ah,089h,01eh,090h,05ah,089h,00eh,092h,05ah
	db	089h,016h,094h,05ah,083h,07eh,006h,006h,077h,008h,08ah,00eh,088h,05ah,0d3h,02eh
	db	092h,05ah,08dh,006h,08eh,05ah,05dh,0cbh
g6b22:
	push	ax
g6b23:
	push	dx
g6b24:
	push	es
g6b25:
	mov ax,seg g5ce0
g6b28:
	mov	es, ax
g6b2a:
	call	dword ptr es:[03abh]
g6b2f:
	push word ptr [mps_curs]
g6b33:
	mov ax,[_MouseY]
g6b36:
	sub ax,[mps_hoty]
g6b3a:
	push	ax
g6b3b:
	mov ax,[_MouseX]
g6b3e:
	sub ax,[mps_hotx]
g6b42:
	push	ax
g6b43:
	mov ax,seg g5ce0
g6b46:
	mov	es, ax
g6b48:
	call	dword ptr es:[0414h]
g6b4d:
	add	sp, 6
g6b50:
	pop	es
g6b51:
	pop	dx
g6b52:
	pop	ax
g6b53:
	ret
g6b54:
	push	bp
g6b55:
	mov	bp, sp
g6b57:
	les	ax, dword ptr [bp + 6]
g6b5a:
	mov byte ptr [mps_ucb],ax
g6b5d:
	mov byte ptr [mps_ucb+2],es
g6b61:
	pop	bp
g6b62:
	retf
	db	0bdh,0f1h,055h,08eh,0ddh,08bh,0e9h,08ah,00eh,088h,05ah,0d3h,0edh,08bh,0cdh,009h
	db	01eh,080h,05ah,089h,01eh,07eh,05ah,089h,00eh,07ah,05ah,089h,016h,07ch,05ah,08bh
	db	02eh,08ah,05ah,00bh,02eh,08ch,05ah,08bh,0e8h,074h,004h,0ffh,01eh,08ah,05ah,0f7h
	db	0c5h,001h,000h,074h,019h,080h,03eh,00fh,056h,000h,074h,012h,0b0h,002h,086h,006h
	db	00eh,056h,00ah,0c0h,075h,008h,0e8h,076h,0ffh,0c6h,006h,00eh,056h,000h,0cbh,006h
	db	056h,057h,055h,0e8h,005h,000h,05dh,05fh,05eh,007h,0cbh
g6bbe:
	sub	ax, ax
g6bc0:
	mov	es, ax
g6bc2:
	push	word ptr es:[0]
g6bc7:
	push	word ptr es:[2]
g6bcc:
	lea ax,[g6d66]
g6bd0:
	mov	word ptr es:[0], ax
g6bd4:
	mov word ptr es:[2],seg g5ce0
g6bdb:
	push	ds
g6bdc:
	pop	es
g6bdd:
	jmp short	g6c2d
	db	090h
g6be0:
	sub	ax, ax
g6be2:
	mov	es, ax
g6be4:
	pop	word ptr es:[2]
g6be9:
	pop	word ptr es:[0]
g6bee:
	push	ds
g6bef:
	pop	es
g6bf0:
	ret
g6bf1:
	and al,[eg2_flag]
g6bf5:
	test	al, 6
g6bf7:
	jne	g6be0
g6bf9:
	mov	bx, cx
g6bfb:
	jmp	g6d28
g6bfe:
	jmp	g6d53
g6c01:
	cmc
g6c02:
	rcr	dx, 1
g6c04:
	mov [eg2_dx],dx
g6c08:
	sar	dx, 1
g6c0a:
	mov [eg2_cx],dx
g6c0e:
	mov	dx, di
g6c10:
	sub	dx, bp
g6c12:
	jno	g6c1a
g6c14:
	cmc
g6c15:
	rcr	dx, 1
g6c17:
	jmp short	g6c85
	db	090h
g6c1a:
	sar	dx, 1
g6c1c:
	jmp short	g6c85
	db	090h
g6c1f:
	cmc
g6c20:
	rcr	dx, 1
g6c22:
	sar word ptr [eg2_dx],1
g6c26:
	sar word ptr [eg2_cx],1
g6c2a:
	jmp short	g6c85
	db	090h
g6c2d:
	mov cx,[mps_x1]
g6c31:
	mov dx,[mps_y1]
g6c35:
	mov si,[mps_x2]
g6c39:
	mov di,[mps_y2]
g6c3d:
	mov	bx, cx
g6c3f:
	mov	bp, dx
g6c41:
	call	g687a
g6c44:
	mov byte ptr [eg2_flag],al
g6c47:
	mov	bx, si
g6c49:
	mov	bp, di
g6c4b:
	call	g687a
g6c4e:
	jne	g6c67
g6c50:
	cmp byte ptr [eg2_flag],00h
g6c55:
	je	g6bfe
g6c57:
	xchg	cx, si
g6c59:
	xchg	dx, di
g6c5b:
	xchg al,[eg2_flag]
g6c5f:
	mov [mps_x1],cx
g6c63:
	mov [mps_y1],dx
g6c67:
	mov	bp, dx
g6c69:
	test byte ptr [eg2_flag],al
g6c6d:
	jne	g6bf1
g6c6f:
	mov	dx, si
g6c71:
	sub	dx, cx
g6c73:
	jo	g6c01
g6c75:
	mov [eg2_dx],dx
g6c79:
	sar	dx, 1
g6c7b:
	mov [eg2_cx],dx
g6c7f:
	mov	dx, di
g6c81:
	sub	dx, bp
g6c83:
	jo	g6c1f
g6c85:
	mov [eg2_dy],dx
g6c89:
	sar	dx, 1
g6c8b:
	mov [eg2_cy],dx
g6c8f:
	test	al, 9
g6c91:
	je	g6ccb
g6c93:
	sub	bx, bx
g6c95:
	or	si, si
g6c97:
	js	g6c9d
g6c99:
	mov bx,[mps_clipR]
g6c9d:
	mov	ax, bx
g6c9f:
	sub	ax, cx
g6ca1:
	imul word ptr [eg2_dy]
g6ca5:
	push	bx
g6ca6:
	mov	bx, dx
g6ca8:
	idiv word ptr [eg2_dx]
g6cac:
	mov	bl, bh
g6cae:
	xor bl,[eg2_s0]
g6cb2:
	jns	g6cb7
g6cb4:
	neg	dx
g6cb6:
	dec	ax
g6cb7:
	sub dx,[eg2_cx]
g6cbb:
	xor	dh, bh
g6cbd:
	js	g6cc0
g6cbf:
	inc	ax
g6cc0:
	pop	bx
g6cc1:
	add	ax, bp
g6cc3:
	js	g6cd3
g6cc5:
	cmp ax,[mps_clipB]
g6cc9:
	jle	g6d04
g6ccb:
	mov bx,[mps_clipB]
g6ccf:
	cmp	di, bx
g6cd1:
	jg	g6cd5
g6cd3:
	sub	bx, bx
g6cd5:
	mov	ax, bx
g6cd7:
	sub	ax, bp
g6cd9:
	imul word ptr [eg2_dx]
g6cdd:
	push	bx
g6cde:
	mov	bx, dx
g6ce0:
	idiv word ptr [eg2_dy]
g6ce4:
	mov	bl, bh
g6ce6:
	xor bl,[eg2_s1]
g6cea:
	jns	g6cef
g6cec:
	neg	dx
g6cee:
	dec	ax
g6cef:
	sub dx,[eg2_cy]
g6cf3:
	xor	dh, bh
g6cf5:
	js	g6cf8
g6cf7:
	inc	ax
g6cf8:
	pop	bx
g6cf9:
	add	ax, cx
g6cfb:
	xchg	bx, ax
g6cfc:
	js	g6d28
g6cfe:
	cmp bx,[mps_clipR]
g6d02:
	jg	g6d28
g6d04:
	push	ax
g6d05:
	push	bx
g6d06:
	push	cx
g6d07:
	push	si
g6d08:
	push	di
g6d09:
	mov	dx, di
g6d0b:
	mov	di, ax
g6d0d:
	mov	si, bx
g6d0f:
	call	g6576
g6d12:
	pop	di
g6d13:
	pop	si
g6d14:
	pop	cx
g6d15:
	pop	bx
g6d16:
	pop	ax
g6d17:
	cmp byte ptr [eg2_flag],00h
g6d1c:
	jne	g6d3d
g6d1e:
	mov [mps_y2],ax
g6d21:
	mov [mps_x2],bx
g6d25:
	jmp short	g6d53
	db	090h
g6d28:
	mov	dx, bp
g6d2a:
	sub	ax, ax
g6d2c:
	mov	es, ax
g6d2e:
	pop	word ptr es:[2]
g6d33:
	pop	word ptr es:[0]
g6d38:
	push	ds
g6d39:
	pop	es
g6d3a:
	jmp	g65d1
g6d3d:
	mov [mps_y1],ax
g6d40:
	mov [mps_x1],bx
g6d44:
	xchg	cx, si
g6d46:
	xchg	bp, di
g6d48:
	mov al,[eg2_flag]
g6d4b:
	mov byte ptr [eg2_flag],00h
g6d50:
	jmp	g6c8f
g6d53:
	sub	ax, ax
g6d55:
	mov	es, ax
g6d57:
	pop	word ptr es:[2]
g6d5c:
	pop	word ptr es:[0]
g6d61:
	push	ds
g6d62:
	pop	es
g6d63:
	jmp	g65e0
g6d66:
	push	bp
	mov	bp, sp
	cmp	word ptr [bp + 2], 01000h
	mov	ax, word ptr ds:[05a99h]
	je	g6d76
	mov	ax, word ptr ds:[05a97h]
g6d76:
	xor	dx, ax
	mov	ax, 07f00h
	jns	g6d7f
	neg	ax
g6d7f:
	add	word ptr [bp + 2], 4
	sub	dx, dx
	pop	bp
	iret
	db	000h
g6d88:
	push	bp
g6d89:
	mov	bp, sp
g6d8b:
	push	si
g6d8c:
	push	di
g6d8d:
	xor	ax, ax
g6d8f:
	push	ax
g6d90:
	call far ptr g60e5
g6d95:
	add	sp, 2
g6d98:
	mov si,[pic_src]
g6d9c:
	cmp si,[pic_bound]
g6da0:
	jb	g6db0
g6da2:
	push	bx
g6da3:
	push	cx
g6da4:
	push	dx
g6da5:
	call dword ptr [pic_pump]
g6da9:
	pop	dx
g6daa:
	pop	cx
g6dab:
	pop	bx
g6dac:
	mov si,[pic_src]
g6db0:
	lodsw	ax, word ptr [si]
g6db1:
	mov [pic_src],si
g6db5:
	cmp	al, 058h
g6db7:
	je	g6e32
g6db9:
	mov	di, ds
g6dbb:
	mov	es, di
g6dbd:
	lea di,[pic_dict]
g6dc1:
	cmp	ax, 0304dh
g6dc4:
	jne	g6dd7
g6dc6:
	cmp	word ptr [bp + 6], 1
g6dca:
	jb	g6dd4
g6dcc:
	je	g6dd7
g6dce:
	mov	di, word ptr [bp + 6]
g6dd1:
	jmp short	g6dd7
	db	090h
g6dd4:
	add	di, 2
g6dd7:
	push	di
g6dd8:
	stosw	word ptr es:[di], ax
g6dd9:
	mov si,[pic_src]
g6ddd:
	cmp si,[pic_bound]
g6de1:
	jb	g6df1
g6de3:
	push	bx
g6de4:
	push	cx
g6de5:
	push	dx
g6de6:
	call dword ptr [pic_pump]
g6dea:
	pop	dx
g6deb:
	pop	cx
g6dec:
	pop	bx
g6ded:
	mov si,[pic_src]
g6df1:
	lodsw	ax, word ptr [si]
g6df2:
	mov [pic_src],si
g6df6:
	stosw	word ptr es:[di], ax
g6df7:
	mov	cx, ax
g6df9:
	shr	cx, 1
g6dfb:
	jcxz	g6e1d
g6dfd:
	mov si,[pic_src]
g6e01:
	cmp si,[pic_bound]
g6e05:
	jb	g6e15
g6e07:
	push	bx
g6e08:
	push	cx
g6e09:
	push	dx
g6e0a:
	call dword ptr [pic_pump]
g6e0e:
	pop	dx
g6e0f:
	pop	cx
g6e10:
	pop	bx
g6e11:
	mov si,[pic_src]
g6e15:
	lodsw	ax, word ptr [si]
g6e16:
	mov [pic_src],si
g6e1a:
	stosw	word ptr es:[di], ax
g6e1b:
	loop	g6dfd
g6e1d:
	pop	di
g6e1e:
	lea ax,[pic_dict]
g6e22:
	cmp	ax, di
g6e24:
	jne	g6e2f
g6e26:
	push	ax
g6e27:
	call far ptr g60e5
g6e2c:
	add	sp, 2
g6e2f:
	jmp	g6d98
g6e32:
	and	ah, 1
g6e35:
	mov byte ptr [pic_state+41],ah
g6e39:
	mov si,[pic_src]
g6e3d:
	cmp si,[pic_bound]
g6e41:
	jb	g6e51
g6e43:
	push	bx
g6e44:
	push	cx
g6e45:
	push	dx
g6e46:
	call dword ptr [pic_pump]
g6e4a:
	pop	dx
g6e4b:
	pop	cx
g6e4c:
	pop	bx
g6e4d:
	mov si,[pic_src]
g6e51:
	lodsw	ax, word ptr [si]
g6e52:
	mov [pic_src],si
g6e56:
	mov byte ptr [pic_state+24],ax
g6e59:
	mov si,[pic_src]
g6e5d:
	cmp si,[pic_bound]
g6e61:
	jb	g6e71
g6e63:
	push	bx
g6e64:
	push	cx
g6e65:
	push	dx
g6e66:
	call dword ptr [pic_pump]
g6e6a:
	pop	dx
g6e6b:
	pop	cx
g6e6c:
	pop	bx
g6e6d:
	mov si,[pic_src]
g6e71:
	lodsw	ax, word ptr [si]
g6e72:
	mov [pic_src],si
g6e76:
	mov byte ptr [pic_state+20],ax
g6e79:
	mov si,[pic_src]
g6e7d:
	cmp si,[pic_bound]
g6e81:
	jb	g6e91
g6e83:
	push	bx
g6e84:
	push	cx
g6e85:
	push	dx
g6e86:
	call dword ptr [pic_pump]
g6e8a:
	pop	dx
g6e8b:
	pop	cx
g6e8c:
	pop	bx
g6e8d:
	mov si,[pic_src]
g6e91:
	lodsw	ax, word ptr [si]
g6e92:
	mov [pic_src],si
g6e96:
	mov byte ptr [pic_state+22],ax
g6e99:
	call	g6f21
g6e9c:
	pop	di
g6e9d:
	pop	si
g6e9e:
	pop	bp
g6e9f:
	retf
g6ea0:
	push	si
g6ea1:
	mov si,[pic_src]
g6ea5:
	cmp si,[pic_bound]
g6ea9:
	jb	g6eb9
g6eab:
	push	bx
g6eac:
	push	cx
g6ead:
	push	dx
g6eae:
	call dword ptr [pic_pump]
g6eb2:
	pop	dx
g6eb3:
	pop	cx
g6eb4:
	pop	bx
g6eb5:
	mov si,[pic_src]
g6eb9:
	lodsw	ax, word ptr [si]
g6eba:
	mov [pic_src],si
g6ebe:
	and	al, 1
g6ec0:
	mov byte ptr [pic_state+41],al
g6ec3:
	mov si,[pic_src]
g6ec7:
	cmp si,[pic_bound]
g6ecb:
	jb	g6edb
g6ecd:
	push	bx
g6ece:
	push	cx
g6ecf:
	push	dx
g6ed0:
	call dword ptr [pic_pump]
g6ed4:
	pop	dx
g6ed5:
	pop	cx
g6ed6:
	pop	bx
g6ed7:
	mov si,[pic_src]
g6edb:
	lodsw	ax, word ptr [si]
g6edc:
	mov [pic_src],si
g6ee0:
	mov byte ptr [pic_state+24],ax
g6ee3:
	call	g6f21
g6ee6:
	pop	si
g6ee7:
	retf
g6ee8:
	push	bp
g6ee9:
	mov	bp, sp
g6eeb:
	push	si
g6eec:
	push	di
g6eed:
	mov si,[pic_src]
g6ef1:
	les	di, dword ptr [bp + 6]
g6ef4:
	mov	cx, word ptr [bp + 0ah]
g6ef7:
	call	g6fa0
g6efa:
	mov [pic_src],si
g6efe:
	pop	di
g6eff:
	pop	si
g6f00:
	pop	bp
g6f01:
	retf
g6f02:
	push	bp
g6f03:
	mov	bp, sp
g6f05:
	push	si
g6f06:
	push	di
g6f07:
	mov	ax, ds
g6f09:
	mov	es, ax
g6f0b:
	mov si,[pic_src]
g6f0f:
	mov	di, word ptr [bp + 6]
g6f12:
	mov cx,[pic_state+20]
g6f16:
	call	g6fa0
g6f19:
	mov [pic_src],si
g6f1d:
	pop	di
g6f1e:
	pop	si
g6f1f:
	pop	bp
g6f20:
	retf
g6f21:
	mov ax,[pic_state+20]
g6f24:
	or ax,[pic_state+22]
g6f28:
	jne	g6f2b
g6f2a:
	ret
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
	jb	g6f54
g6f46:
	push	bx
g6f47:
	push	cx
g6f48:
	push	dx
g6f49:
	call dword ptr [pic_pump]
g6f4d:
	pop	dx
g6f4e:
	pop	cx
g6f4f:
	pop	bx
g6f50:
	mov si,[pic_src]
g6f54:
	lodsw	ax, word ptr [si]
g6f55:
	mov [pic_src],si
g6f59:
	cmp	al, 0bh
g6f5b:
	jbe	g6f5f
g6f5d:
	mov	al, 0bh
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
	mov	dx, 0100h
g6f78:
	mov byte ptr [pic_state+36],dx
g6f7c:
	mov	ax, 0ffffh
g6f7f:
	xor	bx, bx
g6f81:
	mov	cx, 0800h
g6f84:
	mov	word ptr [bx - 06f6ch], ax
g6f88:
	add	bx, 3
g6f8b:
	loop	g6f84
g6f8d:
	mov	al, 0
g6f8f:
	xor	bx, bx
g6f91:
	mov	cx, 0100h
g6f94:
	mov	byte ptr [bx - 06f6ah], al
g6f98:
	inc	al
g6f9a:
	add	bx, 3
g6f9d:
	loop	g6f94
g6f9f:
	ret
g6fa0:
	cmp byte ptr [pic_state+41],00h
g6fa5:
	je	g6faa
g6fa7:
	inc	cx
g6fa8:
	shr	cx, 1
g6faa:
	mov byte ptr [pic_state+28],cx
g6fae:
	mov dx,[pic_state+36]
g6fb2:
	xchg sp,[pic_state+26]
g6fb6:
	cmp byte ptr [pic_state+30],00h
g6fbb:
	jne	g6fde
g6fbd:
	call	g7012
g6fc0:
	cmp	al, 090h
g6fc2:
	je	g6fca
g6fc4:
	mov byte ptr [pic_state+31],al
g6fc7:
	jmp short	g6fe5
	db	090h
g6fca:
	call	g7012
g6fcd:
	or	al, al
g6fcf:
	jne	g6fd9
g6fd1:
	mov	al, 090h
g6fd3:
	mov byte ptr [pic_state+31],al
g6fd6:
	jmp short	g6fe5
	db	090h
g6fd9:
	dec	al
g6fdb:
	mov byte ptr [pic_state+30],al
g6fde:
	mov al,[pic_state+31]
g6fe1:
	dec byte ptr [pic_state+30]
g6fe5:
	cmp byte ptr [pic_state+41],00h
g6fea:
	je	g7002
g6fec:
	mov	ah, al
g6fee:
	and	al, 0fh
g6ff0:
	shr	ah, 1
g6ff2:
	shr	ah, 1
g6ff4:
	shr	ah, 1
g6ff6:
	shr	ah, 1
g6ff8:
	stosw	word ptr es:[di], ax
g6ff9:
	dec word ptr [pic_state+28]
g6ffd:
	jne	g6fb6
g6fff:
	jmp short	g7009
	db	090h
g7002:
	stosb	byte ptr es:[di], al
g7003:
	dec word ptr [pic_state+28]
g7007:
	jne	g6fb6
g7009:
	mov byte ptr [pic_state+36],dx
g700d:
	xchg sp,[pic_state+26]
g7011:
	ret
g7012:
	pop	bp
g7013:
	cmp	sp, 06425h
g7017:
	je	g701c
g7019:
	pop	ax
g701a:
	jmp	bp
g701c:
	mov bx,[pic_state+38]
g7020:
	mov	cl, 010h
g7022:
	mov ch,[pic_state+40]
g7026:
	sub	cl, ch
g7028:
	shr	bx, cl
g702a:
	mov	cl, ch
g702c:
	cmp cl,[pic_state+32]
g7030:
	jge	g7053
g7032:
	cmp si,[pic_bound]
g7036:
	jb	g7046
g7038:
	push	bx
g7039:
	push	cx
g703a:
	push	dx
g703b:
	call dword ptr [pic_pump]
g703f:
	pop	dx
g7040:
	pop	cx
g7041:
	pop	bx
g7042:
	mov si,[pic_src]
g7046:
	lodsw	ax, word ptr [si]
g7047:
	mov byte ptr [pic_state+38],ax
g704a:
	shl	ax, cl
g704c:
	or	bx, ax
g704e:
	add	cl, 010h
g7051:
	jmp short	g702c
g7053:
	sub cl,[pic_state+32]
g7057:
	mov byte ptr [pic_state+40],cl
g705b:
	mov	ax, bx
g705d:
	and ax,[pic_state+34]
g7061:
	mov	cx, ax
g7063:
	cmp	ax, dx
g7065:
	jl	g7071
g7067:
	mov	cx, dx
g7069:
	mov ax,[pic_state+42]
g706c:
	mov bl,[pic_state+44]
g7070:
	push	bx
g7071:
	mov	bx, ax
g7073:
	add	bx, ax
g7075:
	add	bx, ax
g7077:
	mov	ax, word ptr [bx - 06f6ch]
g707b:
	inc	ax
g707c:
	je	g7086
g707e:
	dec	ax
g707f:
	mov	bl, byte ptr [bx - 06f6ah]
g7083:
	push	bx
g7084:
	jmp short	g7071
g7086:
	mov	al, byte ptr [bx - 06f6ah]
g708a:
	mov byte ptr [pic_state+44],al
g708d:
	push	ax
g708e:
	mov	bx, dx
g7090:
	add	bx, dx
g7092:
	add	bx, dx
g7094:
	mov	byte ptr [bx - 06f6ah], al
g7098:
	mov ax,[pic_state+42]
g709b:
	mov	word ptr [bx - 06f6ch], ax
g709f:
	inc	dx
g70a0:
	cmp dx,[pic_state+34]
g70a4:
	jle	g70af
g70a6:
	inc byte ptr [pic_state+32]
g70aa:
	stc
g70ab:
	rcl word ptr [pic_state+34],1
g70af:
	mov al,[pic_state+32]
g70b2:
	cmp al,[pic_state+33]
g70b6:
	jle	g70bb
g70b8:
	call	g6f6a
g70bb:
	mov byte ptr [pic_state+42],cx
g70bf:
	db	0e9h
MPSLIBM_TEXT ENDS
PIC_TEXT SEGMENT BYTE PUBLIC 'CODE'
	ASSUME CS:PIC_TEXT, DS:DGROUP
p40b50:
	push	bp
p40b51:
	mov	bp, sp
p40b53:
	sub	sp, 01ah
p40b56:
	lea	ax, [bp - 01ah]
p40b59:
	push	ax
p40b5a:
	push	word ptr [bp + 6]
p40b5d:
	push	cs
p40b5e:
	call	p4129a
p40b61:
	add	sp, 4
p40b64:
	or	ax, ax
p40b66:
	je	p40b9c
p40b68:
	push	word ptr [bp - 4]
p40b6b:
	push	word ptr [bp - 6]
p40b6e:
	push word ptr [pic_handle]
p40b72:
	push	cs
p40b73:
	call	p413cc
p40b76:
	add	sp, 6
p40b79:
	or	ax, ax
p40b7b:
	je	p40b87
p40b7d:
	push	word ptr [bp + 6]
p40b80:
	push	cs
p40b81:
	call	p411e2
p40b84:
	add	sp, 2
p40b87:
	mov	ax, word ptr [bp - 0ah]
p40b8a:
	mov	dx, word ptr [bp - 8]
p40b8d:
	mov byte ptr [pic_state],ax
p40b90:
	mov byte ptr [pic_state+2],dx
p40b94:
	mov ax,[pic_handle]
p40b97:
	mov	sp, bp
p40b99:
	pop	bp
p40b9a:
	retf
	db	090h
p40b9c:
	lea	ax, [bp - 2]
p40b9f:
	push	ax
p40ba0:
	push	word ptr [bp + 8]
p40ba3:
	push	word ptr [bp + 6]
p40ba6:
	call far ptr __dos_open
p40bab:
	add	sp, 6
p40bae:
	or	ax, ax
p40bb0:
	je	p40bbc
p40bb2:
	push	word ptr [bp + 6]
p40bb5:
	push	cs
p40bb6:
	call	p411e2
p40bb9:
	add	sp, 2
p40bbc:
	mov	ax, word ptr [bp - 2]
p40bbf:
	mov	sp, bp
p40bc1:
	pop	bp
p40bc2:
	retf
	db	090h
p40bc4:
	push	bp
p40bc5:
	mov	bp, sp
p40bc7:
	sub	sp, 2
p40bca:
	lea	ax, [bp - 2]
p40bcd:
	push	ax
p40bce:
	sub	ax, ax
p40bd0:
	push	ax
p40bd1:
	push	word ptr [bp + 6]
p40bd4:
	call far ptr l_dos_creat
p40bd9:
	add	sp, 6
p40bdc:
	or	ax, ax
p40bde:
	je	p40bea
p40be0:
	push	word ptr [bp + 6]
p40be3:
	push	cs
p40be4:
	call	p411e2
p40be7:
	add	sp, 2
p40bea:
	mov	ax, word ptr [bp - 2]
p40bed:
	mov	sp, bp
p40bef:
	pop	bp
p40bf0:
	retf
	db	090h
p40bf2:
	push	bp
p40bf3:
	mov	bp, sp
p40bf5:
	mov ax,[pic_handle]
p40bf8:
	cmp	word ptr [bp + 6], ax
p40bfb:
	je	p40c16
p40bfd:
	push	word ptr [bp + 6]
p40c00:
	call far ptr __dos_close
p40c05:
	add	sp, 2
p40c08:
	or	ax, ax
p40c0a:
	je	p40c16
p40c0c:
	sub	ax, ax
p40c0e:
	push	ax
p40c0f:
	push	cs
p40c10:
	call	p411e2
p40c13:
	add	sp, 2
p40c16:
	pop	bp
p40c17:
	retf
p40c18:
	push	bp
p40c19:
	mov	bp, sp
p40c1b:
	sub	sp, 4
p40c1e:
	sub	ax, ax
p40c20:
	push	ax
p40c21:
	push	word ptr [bp + 6]
p40c24:
	push	cs
p40c25:
	call	p40b50
p40c28:
	add	sp, 4
p40c2b:
	mov	word ptr [bp - 4], ax
p40c2e:
	cmp word ptr [pic_handle],-01h
p40c33:
	jne	p40c3a
p40c35:
	mov	ax, 0ffffh
p40c38:
	jmp short	p40c3d
p40c3a:
	mov ax,[pic_state]
p40c3d:
	mov	word ptr [bp - 2], ax
p40c40:
	lea	ax, [bp - 2]
p40c43:
	push	ax
p40c44:
	push	word ptr [bp - 2]
p40c47:
	push	ds
p40c48:
	push	word ptr [bp + 8]
p40c4b:
	push	word ptr [bp - 4]
p40c4e:
	call far ptr __dos_read
p40c53:
	add	sp, 0ah
p40c56:
	or	ax, ax
p40c58:
	je	p40c64
p40c5a:
	sub	ax, ax
p40c5c:
	push	ax
p40c5d:
	push	cs
p40c5e:
	call	p411e2
p40c61:
	add	sp, 2
p40c64:
	push	word ptr [bp - 4]
p40c67:
	push	cs
p40c68:
	call	p40bf2
p40c6b:
	mov	ax, word ptr [bp - 2]
p40c6e:
	mov	sp, bp
p40c70:
	pop	bp
p40c71:
	retf
p40c72:
	push	bp
p40c73:
	mov	bp, sp
p40c75:
	sub	sp, 4
p40c78:
	sub	ax, ax
p40c7a:
	push	ax
p40c7b:
	push	word ptr [bp + 6]
p40c7e:
	push	cs
p40c7f:
	call	p40b50
p40c82:
	add	sp, 4
p40c85:
	mov	word ptr [bp - 4], ax
p40c88:
	cmp word ptr [pic_handle],-01h
p40c8d:
	jne	p40c94
p40c8f:
	mov	ax, 0ffffh
p40c92:
	jmp short	p40c97
p40c94:
	mov ax,[pic_state]
p40c97:
	mov	word ptr [bp - 2], ax
p40c9a:
	lea	ax, [bp - 2]
p40c9d:
	push	ax
p40c9e:
	push	word ptr [bp - 2]
p40ca1:
	push	word ptr [bp + 0ah]
p40ca4:
	push	word ptr [bp + 8]
p40ca7:
	push	word ptr [bp - 4]
p40caa:
	call far ptr __dos_read
p40caf:
	add	sp, 0ah
p40cb2:
	or	ax, ax
p40cb4:
	je	p40cc0
p40cb6:
	sub	ax, ax
p40cb8:
	push	ax
p40cb9:
	push	cs
p40cba:
	call	p411e2
p40cbd:
	add	sp, 2
p40cc0:
	push	word ptr [bp - 4]
p40cc3:
	push	cs
p40cc4:
	call	p40bf2
p40cc7:
	mov	ax, word ptr [bp - 2]
p40cca:
	mov	sp, bp
p40ccc:
	pop	bp
p40ccd:
	retf
p40cce:
	push	bp
p40ccf:
	mov	bp, sp
p40cd1:
	sub	sp, 2
p40cd4:
	push	word ptr [bp + 6]
p40cd7:
	push	cs
p40cd8:
	call	p40bc4
p40cdb:
	add	sp, 2
p40cde:
	mov	word ptr [bp - 2], ax
p40ce1:
	lea	ax, [bp + 0ch]
p40ce4:
	push	ax
p40ce5:
	push	word ptr [bp + 0ch]
p40ce8:
	push	word ptr [bp + 0ah]
p40ceb:
	push	word ptr [bp + 8]
p40cee:
	push	word ptr [bp - 2]
p40cf1:
	call far ptr __dos_write
p40cf6:
	add	sp, 0ah
p40cf9:
	or	ax, ax
p40cfb:
	je	p40d07
p40cfd:
	sub	ax, ax
p40cff:
	push	ax
p40d00:
	push	cs
p40d01:
	call	p411e2
p40d04:
	add	sp, 2
p40d07:
	push	word ptr [bp - 2]
p40d0a:
	push	cs
p40d0b:
	call	p40bf2
p40d0e:
	mov	ax, word ptr [bp + 0ch]
p40d11:
	mov	sp, bp
p40d13:
	pop	bp
p40d14:
	retf
	db	090h
p40d16:
	push	bp
p40d17:
	mov	bp, sp
p40d19:
	sub	sp, 4
p40d1c:
	push	di
p40d1d:
	push	si
p40d1e:
	sub	ax, ax
p40d20:
	push	ax
p40d21:
	push	word ptr [bp + 0ch]
p40d24:
	push	cs
p40d25:
	call	p40b50
p40d28:
	add	sp, 4
p40d2b:
	mov	word ptr [bp - 4], ax
p40d2e:
	push	ax
p40d2f:
	push	cs
p40d30:
	call	p41154
p40d33:
	add	sp, 2
p40d36:
	push	word ptr [bp + 0eh]
p40d39:
	call far ptr g6d88
p40d3e:
	add	sp, 2
p40d41:
	cmp	word ptr [bp + 6], 0
p40d45:
	jge	p40d4d
p40d47:
	mov word ptr [pic_state+22],00h
p40d4d:
	sub	si, si
p40d4f:
	mov	di, word ptr [bp + 6]
p40d52:
	jmp short	p40d7b
p40d54:
	mov	ax, 0ca5eh
p40d57:
	push	ax
p40d58:
	call far ptr g6f02
p40d5d:
	add	sp, 2
p40d60:
	push word ptr [pic_state+20]
p40d64:
	mov	ax, word ptr [bp + 0ah]
p40d67:
	add	ax, si
p40d69:
	push	ax
p40d6a:
	push	word ptr [bp + 8]
p40d6d:
	push	di
p40d6e:
	mov	ax, 0ca5eh
p40d71:
	push	ax
p40d72:
	call far ptr g60a6
p40d77:
	add	sp, 0ah
p40d7a:
	inc	si
p40d7b:
	cmp si,[pic_state+22]
p40d7f:
	jl	p40d54
p40d81:
	mov	word ptr [bp - 2], si
p40d84:
	push	word ptr [bp - 4]
p40d87:
	push	cs
p40d88:
	call	p40bf2
p40d8b:
	add	sp, 2
p40d8e:
	pop	si
p40d8f:
	pop	di
p40d90:
	mov	sp, bp
p40d92:
	pop	bp
p40d93:
	retf
	PUBLIC	_LoadPicture
_LoadPicture	LABEL FAR
	push	bp
p40d95:
	mov	bp, sp
p40d97:
	mov	ax, 1
p40d9a:
	push	ax
p40d9b:
	push	word ptr [bp + 8]
p40d9e:
	sub	ax, ax
p40da0:
	push	ax
p40da1:
	push	ax
p40da2:
	push	word ptr [bp + 6]
p40da5:
	push	cs
p40da6:
	call	p40d16
p40da9:
	add	sp, 0ah
p40dac:
	pop	bp
p40dad:
	retf
	PUBLIC	_LoadPicToPal
_LoadPicToPal	LABEL FAR
	push	bp
p40daf:
	mov	bp, sp
p40db1:
	push	word ptr [bp + 0ah]
p40db4:
	push	word ptr [bp + 8]
p40db7:
	sub	ax, ax
p40db9:
	push	ax
p40dba:
	push	ax
p40dbb:
	push	word ptr [bp + 6]
p40dbe:
	push	cs
p40dbf:
	call	p40d16
p40dc2:
	add	sp, 0ah
p40dc5:
	pop	bp
p40dc6:
	retf
	db	090h
	PUBLIC	_LoadPicNoPal
_LoadPicNoPal	LABEL FAR
	push	bp
p40dc9:
	mov	bp, sp
p40dcb:
	sub	ax, ax
p40dcd:
	push	ax
p40dce:
	push	word ptr [bp + 8]
p40dd1:
	push	ax
p40dd2:
	push	ax
p40dd3:
	push	word ptr [bp + 6]
p40dd6:
	push	cs
p40dd7:
	call	p40d16
p40dda:
	add	sp, 0ah
p40ddd:
	pop	bp
p40dde:
	retf
	db	090h
	PUBLIC	_LoadPalNoPic
_LoadPalNoPic	LABEL FAR
	push	bp
p40de1:
	mov	bp, sp
p40de3:
	mov	ax, 1
p40de6:
	push	ax
p40de7:
	push	word ptr [bp + 6]
p40dea:
	sub	ax, ax
p40dec:
	push	ax
p40ded:
	push	ax
p40dee:
	mov	ax, 0ffffh
p40df1:
	push	ax
p40df2:
	push	cs
p40df3:
	call	p40d16
p40df6:
	add	sp, 0ah
p40df9:
	pop	bp
p40dfa:
	retf
	db	090h
	PUBLIC	_LoadPalToPal
_LoadPalToPal	LABEL FAR
	push	bp
p40dfd:
	mov	bp, sp
p40dff:
	push	word ptr [bp + 8]
p40e02:
	push	word ptr [bp + 6]
p40e05:
	sub	ax, ax
p40e07:
	push	ax
p40e08:
	push	ax
p40e09:
	mov	ax, 0ffffh
p40e0c:
	push	ax
p40e0d:
	push	cs
p40e0e:
	call	p40d16
p40e11:
	add	sp, 0ah
p40e14:
	pop	bp
p40e15:
	retf
p40e16:
	push	bp
p40e17:
	mov	bp, sp
p40e19:
	sub	sp, 2
p40e1c:
	sub	ax, ax
p40e1e:
	push	ax
p40e1f:
	push	word ptr [bp + 0ah]
p40e22:
	push	cs
p40e23:
	call	p40b50
p40e26:
	add	sp, 4
p40e29:
	mov	word ptr [bp - 2], ax
p40e2c:
	push	ax
p40e2d:
	push	cs
p40e2e:
	call	p41154
p40e31:
	add	sp, 2
p40e34:
	call far ptr g6ea0
p40e39:
	push word ptr [pic_state+24]
p40e3d:
	push	word ptr [bp + 8]
p40e40:
	push	word ptr [bp + 6]
p40e43:
	call far ptr g6ee8
p40e48:
	add	sp, 6
p40e4b:
	push	word ptr [bp - 2]
p40e4e:
	push	cs
p40e4f:
	call	p40bf2
p40e52:
	mov ax,[pic_state+24]
p40e55:
	mov	sp, bp
p40e57:
	pop	bp
p40e58:
	retf
	db	090h
p40e5a:
	push	bp
p40e5b:
	mov	bp, sp
p40e5d:
	sub	sp, 0eh
p40e60:
	lea	ax, [bp - 0eh]
p40e63:
	push	ax
p40e64:
	sub	ax, ax
p40e66:
	push	ax
p40e67:
	push	word ptr [bp + 0ah]
p40e6a:
	call far ptr __dos_open
p40e6f:
	add	sp, 6
p40e72:
	or	ax, ax
p40e74:
	je	p40e7e
p40e76:
	mov ax,[_errno]
p40e79:
	mov	sp, bp
p40e7b:
	pop	bp
p40e7c:
	retf
	db	090h
p40e7e:
	lea	ax, [bp - 0ch]
p40e81:
	push	ax
p40e82:
	mov	ax, 2
p40e85:
	push	ax
p40e86:
	lea	ax, [bp - 2]
p40e89:
	push	ss
p40e8a:
	push	ax
p40e8b:
	push	word ptr [bp - 0eh]
p40e8e:
	call far ptr __dos_read
p40e93:
	add	sp, 0ah
p40e96:
	or	ax, ax
p40e98:
	jne	p40e76
p40e9a:
	mov word ptr [_errno],02h
p40ea0:
	mov	ax, word ptr [bp - 2]
p40ea3:
	cmp	word ptr [bp + 8], ax
p40ea6:
	jbe	p40eb0
p40ea8:
	mov	ax, 2
p40eab:
	mov	sp, bp
p40ead:
	pop	bp
p40eae:
	retf
	db	090h
p40eb0:
	mov	ax, word ptr [bp + 8]
p40eb3:
	shl	ax, 1
p40eb5:
	db	099h	; cdq 
p40eb6:
	push	dx
p40eb7:
	push	ax
p40eb8:
	push	word ptr [bp - 0eh]
p40ebb:
	push	cs
p40ebc:
	call	p413cc
p40ebf:
	add	sp, 6
p40ec2:
	or	ax, ax
p40ec4:
	jne	p40e76
p40ec6:
	lea	ax, [bp - 0ch]
p40ec9:
	push	ax
p40eca:
	mov	ax, 2
p40ecd:
	push	ax
p40ece:
	lea	ax, [bp - 2]
p40ed1:
	push	ss
p40ed2:
	push	ax
p40ed3:
	push	word ptr [bp - 0eh]
p40ed6:
	call far ptr __dos_read
p40edb:
	add	sp, 0ah
p40ede:
	or	ax, ax
p40ee0:
	jne	p40e76
p40ee2:
	sub	word ptr [bp - 2], 8
p40ee6:
	sub	ax, ax
p40ee8:
	push	ax
p40ee9:
	push	word ptr [bp - 2]
p40eec:
	push	word ptr [bp - 0eh]
p40eef:
	push	cs
p40ef0:
	call	p413cc
p40ef3:
	add	sp, 6
p40ef6:
	or	ax, ax
p40ef8:
	je	p40efd
p40efa:
	jmp	p40e76
p40efd:
	lea	ax, [bp - 0ch]
p40f00:
	push	ax
p40f01:
	mov	ax, 8
p40f04:
	push	ax
p40f05:
	mov	ax, 061fch
p40f08:
	push	ds
p40f09:
	push	ax
p40f0a:
	push	word ptr [bp - 0eh]
p40f0d:
	call far ptr __dos_read
p40f12:
	add	sp, 0ah
p40f15:
	or	ax, ax
p40f17:
	je	p40f1c
p40f19:
	jmp	p40e76
p40f1c:
	mov al,[pic_state+5]
p40f1f:
	sub	ah, ah
p40f21:
	mov cl,[pic_state+4]
p40f25:
	sub	ch, ch
p40f27:
	sub	ax, cx
p40f29:
	inc	ax
p40f2a:
	mov	word ptr [bp - 4], ax
p40f2d:
	mov al,[pic_state+6]
p40f30:
	sub	ah, ah
p40f32:
	mov	cx, ax
p40f34:
	mov al,[pic_state+8]
p40f37:
	mov dl,[pic_state+10]
p40f3b:
	sub	dh, dh
p40f3d:
	add	ax, dx
p40f3f:
	mul	word ptr [bp - 4]
p40f42:
	mul	cx
p40f44:
	add	ax, 8
p40f47:
	mov	word ptr [bp - 0ah], ax
p40f4a:
	push	word ptr [bp + 6]
p40f4d:
	call far ptr g606e
p40f52:
	add	sp, 2
p40f55:
	sub	ax, 8
p40f58:
	mov	word ptr [bp - 8], ax
p40f5b:
	mov	word ptr [bp - 6], dx
p40f5e:
	cmp byte ptr [pic_state+7],00h
p40f63:
	jne	p40f71
p40f65:
	mov	ax, word ptr [bp - 4]
p40f68:
	sub	word ptr [bp - 8], ax
p40f6b:
	sub	word ptr [bp - 2], ax
p40f6e:
	add	word ptr [bp - 0ah], ax
p40f71:
	sub	ax, ax
p40f73:
	push	ax
p40f74:
	push	word ptr [bp - 2]
p40f77:
	push	word ptr [bp - 0eh]
p40f7a:
	push	cs
p40f7b:
	call	p413cc
p40f7e:
	add	sp, 6
p40f81:
	or	ax, ax
p40f83:
	je	p40f88
p40f85:
	jmp	p40e76
p40f88:
	lea	ax, [bp - 0ch]
p40f8b:
	push	ax
p40f8c:
	push	word ptr [bp - 0ah]
p40f8f:
	push	word ptr [bp - 6]
p40f92:
	push	word ptr [bp - 8]
p40f95:
	push	word ptr [bp - 0eh]
p40f98:
	call far ptr __dos_read
p40f9d:
	add	sp, 0ah
p40fa0:
	or	ax, ax
p40fa2:
	je	p40fa7
p40fa4:
	jmp	p40e76
p40fa7:
	push	word ptr [bp - 0eh]
p40faa:
	call far ptr __dos_close
p40faf:
	add	sp, 2
p40fb2:
	or	ax, ax
p40fb4:
	je	p40fb9
p40fb6:
	jmp	p40e76
p40fb9:
	sub	ax, ax
p40fbb:
	mov	sp, bp
p40fbd:
	pop	bp
p40fbe:
	retf
	db	090h
p40fc0:
	push	bp
p40fc1:
	mov	bp, sp
p40fc3:
	sub	sp, 4
p40fc6:
	push	si
p40fc7:
	sub	ax, ax
p40fc9:
	push	ax
p40fca:
	push	word ptr [bp + 6]
p40fcd:
	push	cs
p40fce:
	call	p40b50
p40fd1:
	add	sp, 4
p40fd4:
	mov	word ptr [bp - 4], ax
p40fd7:
	push	ax
p40fd8:
	push	cs
p40fd9:
	call	p41154
p40fdc:
	add	sp, 2
p40fdf:
	sub	ax, ax
p40fe1:
	push	ax
p40fe2:
	call far ptr g6d88
p40fe7:
	add	sp, 2
p40fea:
	push word ptr [pic_state+22]
p40fee:
	push word ptr [pic_state+20]
p40ff2:
	call far ptr g6116
p40ff7:
	add	sp, 4
p40ffa:
	sub	si, si
p40ffc:
	jmp short	p41017
p40ffe:
	mov	ax, 0ca5eh
p41001:
	push	ax
p41002:
	call far ptr g6f02
p41007:
	add	sp, 2
p4100a:
	mov	ax, 0ca5eh
p4100d:
	push	ax
p4100e:
	call far ptr g610f
p41013:
	add	sp, 2
p41016:
	inc	si
p41017:
	cmp si,[pic_state+22]
p4101b:
	jl	p40ffe
p4101d:
	mov	word ptr [bp - 2], si
p41020:
	push	word ptr [bp - 4]
p41023:
	push	cs
p41024:
	call	p40bf2
p41027:
	add	sp, 2
p4102a:
	call far ptr g6108
p4102f:
	pop	si
p41030:
	mov	sp, bp
p41032:
	pop	bp
p41033:
	retf
	PUBLIC	_LoadPicGmem
_LoadPicGmem	LABEL FAR
	push	bp
p41035:
	mov	bp, sp
p41037:
	push	word ptr [bp + 6]
p4103a:
	push	word ptr [bp + 8]
p4103d:
	push	cs
p4103e:
	call near ptr	_LoadPicture
p41041:
	add	sp, 4
p41044:
	pop	bp
p41045:
	retf
p41046:
	push	bp
p41047:
	mov	bp, sp
p41049:
	sub	sp, 4
p4104c:
	push	di
p4104d:
	push	si
p4104e:
	sub	ax, ax
p41050:
	push	ax
p41051:
	push	word ptr [bp + 6]
p41054:
	push	cs
p41055:
	call	p40b50
p41058:
	add	sp, 4
p4105b:
	mov	word ptr [bp - 4], ax
p4105e:
	push	ax
p4105f:
	push	cs
p41060:
	call	p41154
p41063:
	add	sp, 2
p41066:
	sub	ax, ax
p41068:
	push	ax
p41069:
	call far ptr g6d88
p4106e:
	add	sp, 2
p41071:
	sub	si, si
p41073:
	mov	di, word ptr [bp + 8]
p41076:
	jmp short	p4109a
p41078:
	mov	ax, 0ca5eh
p4107b:
	push	ax
p4107c:
	call far ptr g6f02
p41081:
	add	sp, 2
p41084:
	push word ptr [pic_state+20]
p41088:
	push	si
p41089:
	sub	ax, ax
p4108b:
	push	ax
p4108c:
	push	di
p4108d:
	mov	ax, 0ca5eh
p41090:
	push	ax
p41091:
	call far ptr g60a6
p41096:
	add	sp, 0ah
p41099:
	inc	si
p4109a:
	cmp si,[pic_state+22]
p4109e:
	jl	p41078
p410a0:
	mov	word ptr [bp - 2], si
p410a3:
	pop	si
p410a4:
	pop	di
p410a5:
	mov	sp, bp
p410a7:
	pop	bp
p410a8:
	retf
	db	090h
p410aa:
	push	bp
p410ab:
	mov	bp, sp
p410ad:
	sub	sp, 2
p410b0:
	push	di
p410b1:
	push	si
p410b2:
	mov word ptr [pic_state+14],00h
p410b8:
	mov	ax, word ptr [bp + 6]
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
	sub	ax, ax
p410d2:
	push	ax
p410d3:
	call far ptr g6d88
p410d8:
	add	sp, 2
p410db:
	sub	si, si
p410dd:
	mov	di, word ptr [bp + 8]
p410e0:
	jmp short	p41104
p410e2:
	mov	ax, 0ca5eh
p410e5:
	push	ax
p410e6:
	call far ptr g6f02
p410eb:
	add	sp, 2
p410ee:
	push word ptr [pic_state+20]
p410f2:
	push	si
p410f3:
	sub	ax, ax
p410f5:
	push	ax
p410f6:
	push	di
p410f7:
	mov	ax, 0ca5eh
p410fa:
	push	ax
p410fb:
	call far ptr g60a6
p41100:
	add	sp, 0ah
p41103:
	inc	si
p41104:
	cmp si,[pic_state+22]
p41108:
	jl	p410e2
p4110a:
	mov	word ptr [bp - 2], si
p4110d:
	pop	si
p4110e:
	pop	di
p4110f:
	mov	sp, bp
p41111:
	pop	bp
p41112:
	retf
	db	090h
p41114:
	push	bp
p41115:
	mov	bp, sp
p41117:
	sub	sp, 2
p4111a:
	mov	ax, word ptr [bp + 6]
p4111d:
	mov byte ptr [pic_state+14],ax
p41120:
	mov	ax, word ptr [bp + 0ch]
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
	push	word ptr [bp + 0ch]
p41144:
	push	word ptr [bp + 0ah]
p41147:
	call far ptr g6ee8
p4114c:
	mov ax,[pic_state+24]
p4114f:
	mov	sp, bp
p41151:
	pop	bp
p41152:
	retf
	db	090h
p41154:
	push	bp
p41155:
	mov	bp, sp
p41157:
	mov	ax, word ptr [bp + 6]
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
	pop	bp
p41170:
	retf
	db	090h
p41172:
	push	bp
p41173:
	mov	bp, sp
p41175:
	sub	sp, 4
p41178:
	mov ax,offset pic_cbuf
p4117b:
	mov	word ptr [bp - 4], ax
p4117e:
	mov	word ptr [bp - 2], ds
p41181:
	mov	ax, 0200h
p41184:
	push	ax
p41185:
	push	word ptr [bp - 4]
p41188:
	push	ds
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
	mov	ax, 0200h
p411a4:
	mov	sp, bp
p411a6:
	pop	bp
p411a7:
	retf
p411a8:
	push	bp
p411a9:
	mov	bp, sp
p411ab:
	mov	ax, word ptr [bp + 6]
p411ae:
	mov byte ptr [pic_state+12],ax
p411b1:
	push	cs
p411b2:
	call	p411b8
p411b5:
	pop	bp
p411b6:
	retf
	db	090h
p411b8:
	push	bp
p411b9:
	mov	bp, sp
p411bb:
	sub	sp, 2
p411be:
	lea	ax, [bp - 2]
p411c1:
	push	ax
p411c2:
	mov	ax, 0200h
p411c5:
	push	ax
p411c6:
	mov ax,offset pic_cbuf
p411c9:
	push	ds
p411ca:
	push	ax
p411cb:
	push word ptr [pic_state+12]
p411cf:
	call far ptr __dos_read
p411d4:
	mov word ptr [pic_src],offset pic_cbuf
p411da:
	mov	ax, word ptr [bp - 2]
p411dd:
	mov	sp, bp
p411df:
	pop	bp
p411e0:
	retf
	db	090h
p411e2:
	push	bp
p411e3:
	mov	bp, sp
p411e5:
	mov	ax, 3
p411e8:
	push	ax
p411e9:
	push	cs
p411ea:
	call	p4120a
p411ed:
	add	sp, 2
p411f0:
	push	word ptr [bp + 6]
p411f3:
	call far ptr l_perror
p411f8:
	add	sp, 2
p411fb:
	mov	ax, 063h
p411fe:
	push	ax
p411ff:
	call far ptr _exit
p41204:
	add	sp, 2
p41207:
	pop	bp
p41208:
	retf
	db	090h
p4120a:
	push	bp
p4120b:
	mov	bp, sp
p4120d:
	sub	sp, 010h
p41210:
	mov ax,[_errno]
p41213:
	mov	word ptr [bp - 010h], ax
p41216:
	mov	byte ptr [bp - 0dh], 0
p4121a:
	mov	al, byte ptr [bp + 6]
p4121d:
	mov	byte ptr [bp - 0eh], al
p41220:
	lea	ax, [bp - 0eh]
p41223:
	push	ax
p41224:
	lea	ax, [bp - 0eh]
p41227:
	push	ax
p41228:
	mov	ax, 010h
p4122b:
	push	ax
p4122c:
	call far ptr _int86x
p41231:
	mov	ax, word ptr [bp - 010h]
p41234:
	mov [_errno],ax
p41237:
	mov	sp, bp
p41239:
	pop	bp
p4123a:
	retf
	db	090h
p4123c:
	push	bp
p4123d:
	mov	bp, sp
p4123f:
	sub	sp, 2
p41242:
	lea	ax, [bp - 2]
p41245:
	push	ax
p41246:
	sub	ax, ax
p41248:
	push	ax
p41249:
	push	word ptr [bp + 6]
p4124c:
	call far ptr __dos_open
p41251:
	add	sp, 6
p41254:
	or	ax, ax
p41256:
	je	p4125e
p41258:
	sub	ax, ax
p4125a:
	mov	sp, bp
p4125c:
	pop	bp
p4125d:
	retf
p4125e:
	mov	ax, word ptr [bp - 2]
p41261:
	mov [pic_handle],ax
p41264:
	mov word ptr [pic_state+18],00h
p4126a:
	mov	ax, 1
p4126d:
	mov	sp, bp
p4126f:
	pop	bp
p41270:
	retf
	db	090h,083h,03eh,036h,057h,0ffh,074h,020h,0ffh,036h,036h,057h,09ah,02ch,00dh,040h
	db	041h,083h,0c4h,002h,00bh,0c0h,074h,00ah,02bh,0c0h,050h,00eh,0e8h,052h,0ffh,083h
	db	0c4h,002h,0c7h,006h,036h,057h,0ffh,0ffh,0cbh
p4129a:
	push	bp
p4129b:
	mov	bp, sp
p4129d:
	sub	sp, 8
p412a0:
	push	di
p412a1:
	push	si
p412a2:
	cmp word ptr [pic_handle],-01h
p412a7:
	jne	p412b2
p412a9:
	sub	ax, ax
p412ab:
	pop	si
p412ac:
	pop	di
p412ad:
	mov	sp, bp
p412af:
	pop	bp
p412b0:
	retf
	db	090h
p412b2:
	sub	ax, ax
p412b4:
	push	ax
p412b5:
	push	ax
p412b6:
	push word ptr [pic_handle]
p412ba:
	push	cs
p412bb:
	call	p413cc
p412be:
	add	sp, 6
p412c1:
	or	ax, ax
p412c3:
	je	p412cf
p412c5:
	sub	ax, ax
p412c7:
	push	ax
p412c8:
	push	cs
p412c9:
	call	p411e2
p412cc:
	add	sp, 2
p412cf:
	lea	ax, [bp - 2]
p412d2:
	push	ax
p412d3:
	mov	ax, 2
p412d6:
	push	ax
p412d7:
	lea	ax, [bp - 4]
p412da:
	push	ss
p412db:
	push	ax
p412dc:
	push word ptr [pic_handle]
p412e0:
	call far ptr __dos_read
p412e5:
	add	sp, 0ah
p412e8:
	or	ax, ax
p412ea:
	je	p412f6
p412ec:
	sub	ax, ax
p412ee:
	push	ax
p412ef:
	push	cs
p412f0:
	call	p411e2
p412f3:
	add	sp, 2
p412f6:
	cmp	word ptr [bp + 6], 0
p412fa:
	jne	p4135a
p412fc:
	inc word ptr [pic_state+18]
p41300:
	mov ax,[pic_state+18]
p41303:
	cmp	word ptr [bp - 4], ax
p41306:
	jl	p412a9
p41308:
	mov	word ptr [bp - 4], ax
p4130b:
	dec	word ptr [bp - 4]
p4130e:
	or	ax, ax
p41310:
	je	p41350
p41312:
	mov	ax, word ptr [bp + 8]
p41315:
	mov	word ptr [bp - 8], ax
p41318:
	mov	word ptr [bp - 6], ds
p4131b:
	mov	si, word ptr [bp - 4]
p4131e:
	lea	ax, [bp - 2]
p41321:
	push	ax
p41322:
	mov	ax, 018h
p41325:
	push	ax
p41326:
	push	word ptr [bp - 6]
p41329:
	push	word ptr [bp - 8]
p4132c:
	push word ptr [pic_handle]
p41330:
	call far ptr __dos_read
p41335:
	add	sp, 0ah
p41338:
	or	ax, ax
p4133a:
	je	p41346
p4133c:
	sub	ax, ax
p4133e:
	push	ax
p4133f:
	push	cs
p41340:
	call	p411e2
p41343:
	add	sp, 2
p41346:
	mov	ax, si
p41348:
	dec	si
p41349:
	or	ax, ax
p4134b:
	jne	p4131e
p4134d:
	mov	word ptr [bp - 4], si
p41350:
	mov	ax, 1
p41353:
	pop	si
p41354:
	pop	di
p41355:
	mov	sp, bp
p41357:
	pop	bp
p41358:
	retf
	db	090h
p4135a:
	mov	ax, word ptr [bp - 4]
p4135d:
	dec	word ptr [bp - 4]
p41360:
	or	ax, ax
p41362:
	jne	p41367
p41364:
	jmp	p412a9
p41367:
	mov	ax, word ptr [bp + 8]
p4136a:
	mov	word ptr [bp - 8], ax
p4136d:
	mov	word ptr [bp - 6], ds
p41370:
	mov	di, ax
p41372:
	mov	si, word ptr [bp - 4]
p41375:
	lea	ax, [bp - 2]
p41378:
	push	ax
p41379:
	mov	ax, 018h
p4137c:
	push	ax
p4137d:
	push	word ptr [bp - 6]
p41380:
	push	word ptr [bp - 8]
p41383:
	push word ptr [pic_handle]
p41387:
	call far ptr __dos_read
p4138c:
	add	sp, 0ah
p4138f:
	or	ax, ax
p41391:
	je	p4139d
p41393:
	sub	ax, ax
p41395:
	push	ax
p41396:
	push	cs
p41397:
	call	p411e2
p4139a:
	add	sp, 2
p4139d:
	mov	ax, 0ch
p413a0:
	push	ax
p413a1:
	push	word ptr [bp + 6]
p413a4:
	push	di
p413a5:
	call far ptr l_strnicmp
p413aa:
	add	sp, 6
p413ad:
	or	ax, ax
p413af:
	jne	p413be
p413b1:
	mov	ax, 1
p413b4:
	mov	word ptr [bp - 4], si
p413b7:
	pop	si
p413b8:
	pop	di
p413b9:
	mov	sp, bp
p413bb:
	pop	bp
p413bc:
	retf
	db	090h
p413be:
	mov	ax, si
p413c0:
	dec	si
p413c1:
	or	ax, ax
p413c3:
	jne	p41375
p413c5:
	mov	word ptr [bp - 4], si
p413c8:
	jmp	p412a9
	db	090h
p413cc:
	push	bp
p413cd:
	mov	bp, sp
p413cf:
	sub	sp, 0eh
p413d2:
	mov	word ptr [bp - 0eh], 04200h
p413d7:
	mov	ax, word ptr [bp + 6]
p413da:
	mov	word ptr [bp - 0ch], ax
p413dd:
	mov	ax, word ptr [bp + 0ah]
p413e0:
	mov	word ptr [bp - 0ah], ax
p413e3:
	mov	ax, word ptr [bp + 8]
p413e6:
	mov	word ptr [bp - 8], ax
p413e9:
	lea	ax, [bp - 0eh]
p413ec:
	push	ax
p413ed:
	lea	ax, [bp - 0eh]
p413f0:
	push	ax
p413f1:
	call far ptr l_intdosx
p413f6:
	add	sp, 4
p413f9:
	cmp	word ptr [bp - 2], 0
p413fd:
	db	075h,007h	; jne p41406 (unresolved)
p413ff:
	db	02bh
PIC_TEXT ENDS
EXTRN F_SinDP:FAR,F_CosDP:FAR,_errno:WORD
EXTRN __dos_open:FAR,__dos_close:FAR,__dos_read:FAR,__dos_write:FAR
EXTRN _movedata:FAR,_int86x:FAR,_exit:FAR
AUX2_TEXT SEGMENT BYTE PUBLIC 'CODE'
	ASSUME CS:AUX2_TEXT
	PUBLIC	_perror
l_perror	LABEL FAR
_perror	LABEL FAR
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
l_dos_creat	LABEL FAR
	PUBLIC	__dos_creat
__dos_creat	LABEL FAR
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
l_strnicmp	LABEL FAR
	PUBLIC	_strnicmp
_strnicmp	LABEL FAR
	db	055h,08bh,0ech,057h,056h,08bh,076h,006h,08bh,07eh,008h,01eh,007h,08bh,04eh,00ah
	db	0e3h,03dh,0b7h,041h,0b3h,05ah,0b6h,020h,08ah,024h,08ah,005h,00ah,0e4h,074h,020h
	db	00ah,0c0h,074h,01ch,046h,047h,03ah,0e7h,072h,006h,03ah,0e3h,077h,002h,002h,0e6h
	db	03ah,0c7h,072h,006h,03ah,0c3h,077h,002h,002h,0c6h,03ah,0e0h,075h,008h,0e2h,0d8h
	db	033h,0c9h,03ah,0e0h,074h,009h,0b9h,000h,000h,072h,002h,049h,049h,0f7h,0d1h,08bh
	db	0c1h,05eh,05fh,08bh,0e5h,05dh,0cbh
l_intdosx	LABEL FAR
	PUBLIC	_intdosx
_intdosx	LABEL FAR
	db	055h,08bh,0ech,056h,057h,08bh,07eh,006h,08bh,005h,08bh,05dh,002h,08bh,04dh,004h
	db	08bh,055h,006h,08bh,075h,008h,08bh,07dh,00ah,0cdh,021h,057h,08bh,07eh,008h,089h
	db	005h,089h,05dh,002h,089h,04dh,004h,089h,055h,006h,089h,075h,008h,08fh,045h,00ah
	db	072h,004h,033h,0f6h,0ebh,002h,0beh,001h,000h,089h,075h,00ch,05fh,05eh,08bh,0e5h
	db	05dh,0cbh
AUX2_TEXT ENDS
EXTRN _strlen:FAR,_write:FAR
END
