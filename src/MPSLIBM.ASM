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
	push	bp
	mov	bp, sp
	push	si
	push	di
	mov	di, word ptr [bp + 6]
	mov	cx, word ptr [bp + 8]
	mov	si, word ptr [bp + 0ah]
	mov	bp, word ptr [bp + 0ch]
	call far ptr g5d02
	pop	di
	pop	si
	pop	bp
	retf
g5d02:
	mov	bx, cx
	call far ptr F_SinDP
	mov	word ptr ds:[0b7eh], bx
	mov	bx, cx
	call far ptr F_CosDP
	mov	word ptr ds:[0b80h], bx
	mov	bx, bp
	call far ptr F_SinDP
	mov	word ptr ds:[0b86h], bx
	mov	bx, bp
	call far ptr F_CosDP
	mov	word ptr ds:[0b88h], bx
	mov	bx, si
	call far ptr F_SinDP
	mov	word ptr ds:[0b82h], bx
	mov	bx, si
	call far ptr F_CosDP
	mov	word ptr ds:[0b84h], bx
	mov	ax, word ptr ds:[0b82h]
	imul	word ptr ds:[0b86h]
	shl	ax, 1
	rcl	dx, 1
	mov	si, dx
	mov	ax, dx
	imul	word ptr ds:[0b7eh]
	shl	ax, 1
	rcl	dx, 1
	mov	bx, ax
	mov	cx, dx
	mov	ax, word ptr ds:[0b80h]
	imul	word ptr ds:[0b88h]
	shl	ax, 1
	rcl	dx, 1
	add	ax, bx
	adc	dx, cx
	mov	word ptr [di], dx
	mov	ax, word ptr ds:[0b82h]
	imul	word ptr ds:[0b88h]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, dx
	mov	ax, dx
	imul	word ptr ds:[0b7eh]
	shl	ax, 1
	rcl	dx, 1
	mov	bx, ax
	mov	cx, dx
	mov	ax, word ptr ds:[0b80h]
	imul	word ptr ds:[0b86h]
	shl	ax, 1
	rcl	dx, 1
	sub	bx, ax
	sbb	cx, dx
	mov	word ptr [di + 2], cx
	mov	ax, word ptr ds:[0b7eh]
	imul	word ptr ds:[0b84h]
	shl	ax, 1
	rcl	dx, 1
	mov	word ptr [di + 4], dx
	mov	ax, word ptr ds:[0b86h]
	imul	word ptr ds:[0b84h]
	shl	ax, 1
	rcl	dx, 1
	mov	word ptr [di + 6], dx
	mov	ax, word ptr ds:[0b88h]
	imul	word ptr ds:[0b84h]
	shl	ax, 1
	rcl	dx, 1
	mov	word ptr [di + 8], dx
	mov	ax, word ptr ds:[0b82h]
	neg	ax
	mov	word ptr [di + 0ah], ax
	mov	ax, si
	imul	word ptr ds:[0b80h]
	shl	ax, 1
	rcl	dx, 1
	mov	bx, ax
	mov	cx, dx
	mov	ax, word ptr ds:[0b7eh]
	imul	word ptr ds:[0b88h]
	shl	ax, 1
	rcl	dx, 1
	sub	bx, ax
	sbb	cx, dx
	mov	word ptr [di + 0ch], cx
	mov	ax, bp
	imul	word ptr ds:[0b80h]
	shl	ax, 1
	rcl	dx, 1
	mov	bx, ax
	mov	cx, dx
	mov	ax, word ptr ds:[0b7eh]
	imul	word ptr ds:[0b86h]
	shl	ax, 1
	rcl	dx, 1
	add	ax, bx
	adc	dx, cx
	mov	word ptr [di + 0eh], dx
	mov	ax, word ptr ds:[0b80h]
	imul	word ptr ds:[0b84h]
	shl	ax, 1
	rcl	dx, 1
	mov	word ptr [di + 010h], dx
	retf
	mov bx,offset mat_b9c
	mov	ax, word ptr [bx + 2]
	xchg	word ptr [bx + 6], ax
	mov	word ptr [bx + 2], ax
	mov	ax, word ptr [bx + 4]
	xchg	word ptr [bx + 0ch], ax
	mov	word ptr [bx + 4], ax
	mov	ax, word ptr [bx + 0ah]
	xchg	word ptr [bx + 0eh], ax
	mov	word ptr [bx + 0ah], ax
	retf
	push	bp
	mov	bp, sp
	push	si
	push	di
	mov	bx, word ptr [bp + 6]
	mov	si, word ptr [bp + 8]
	mov	di, word ptr [bp + 0ah]
	call far ptr g5e54
	pop	di
	pop	si
	pop	bp
	retf
g5e54:
	mov	ax, word ptr [bx]
	imul	word ptr [si]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 2]
	imul	word ptr [si + 6]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 4]
	imul	word ptr [si + 0ch]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di], cx
	mov	ax, word ptr [bx]
	imul	word ptr [si + 2]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 2]
	imul	word ptr [si + 8]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 4]
	imul	word ptr [si + 0eh]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 2], cx
	mov	ax, word ptr [bx]
	imul	word ptr [si + 4]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 2]
	imul	word ptr [si + 0ah]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 4]
	imul	word ptr [si + 010h]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 4], cx
	mov	ax, word ptr [bx + 6]
	imul	word ptr [si]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 8]
	imul	word ptr [si + 6]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 0ah]
	imul	word ptr [si + 0ch]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 6], cx
	mov	ax, word ptr [bx + 6]
	imul	word ptr [si + 2]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 8]
	imul	word ptr [si + 8]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 0ah]
	imul	word ptr [si + 0eh]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 8], cx
	mov	ax, word ptr [bx + 6]
	imul	word ptr [si + 4]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 8]
	imul	word ptr [si + 0ah]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 0ah]
	imul	word ptr [si + 010h]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 0ah], cx
	mov	ax, word ptr [bx + 0ch]
	imul	word ptr [si]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 0eh]
	imul	word ptr [si + 6]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 010h]
	imul	word ptr [si + 0ch]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 0ch], cx
	mov	ax, word ptr [bx + 0ch]
	imul	word ptr [si + 2]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 0eh]
	imul	word ptr [si + 8]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 010h]
	imul	word ptr [si + 0eh]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 0eh], cx
	mov	ax, word ptr [bx + 0ch]
	imul	word ptr [si + 4]
	shl	ax, 1
	rcl	dx, 1
	mov	bp, ax
	mov	cx, dx
	mov	ax, word ptr [bx + 0eh]
	imul	word ptr [si + 0ah]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	ax, word ptr [bx + 010h]
	imul	word ptr [si + 010h]
	shl	ax, 1
	rcl	dx, 1
	add	bp, ax
	adc	cx, dx
	mov	word ptr [di + 010h], cx
	retf
	db	000h
g5fe4:
	cmp byte ptr [dg_drawn],00h
	jne	g5ff7
	pop	word ptr cs:[cs_j1]
	jmp short	g5ff2
g5ff2:
	jmp	dword ptr cs:[0000h]	; far jmp via cs:[cs_j1], operand patched at runtime
cs_j1	EQU	g5ff2 + 3
	PUBLIC	_MouseX
	PUBLIC	_MouseY
	PUBLIC	_Button
	PUBLIC	_MOUSE
g5ff7:
	mov byte ptr [dg_lock],01h
	pop	word ptr cs:[cs_j2]
	pop word ptr [dg_ret0]
	pop word ptr [dg_ret2]
	jmp short	g600b
g600b:
	call	dword ptr cs:[0000h]	; far call via cs:[cs_j2], operand patched at runtime
cs_j2	EQU	g600b + 3
g6010:
	dec byte ptr [dg_lock]
	je	g601b
	call	g6b22
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
	db	000h,000h
	db	000h,000h
	db	000h,000h
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
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_FontHeight
_FontHeight	LABEL FAR
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h,0eah,000h,000h,000h,000h
g6088:
	call	g5fe4
	db	000h,000h
	db	000h,000h
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_LIBCANDO
_LIBCANDO	LABEL FAR
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_OpenSprite
_OpenSprite	LABEL FAR
	db	000h,000h
g60a6:
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_RectCopy
_RectCopy	LABEL FAR
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_RpPrint
_RpPrint	LABEL FAR
	call	g5fe4
	db	000h,000h
	db	000h,000h
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_ScreenOff
_ScreenOff	LABEL FAR
	db	000h,000h
	db	0eah,000h,000h,000h,000h	; far jmp cell, patched at runtime
	PUBLIC	_ScreenOn
_ScreenOn	LABEL FAR
	db	000h,000h
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
	db	000h,000h
	db	000h,000h
	PUBLIC	_ShowPage
_ShowPage	LABEL FAR
	call	g5fe4
	db	000h,000h
	db	000h,000h
	call	g5fe4
	db	000h,000h
	db	000h,000h
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
	mov	bp, sp
	mov	ah, 048h
	mov	bx, 0ffffh
	int	021h
	jb	g633b
	lea si,[mps_e1]
	jmp	g648d
g633b:
	mov	ah, 048h
	sub	bx, 0100h
	mov [mps_freep],bx
	int	021h
	jae	g6350
	lea si,[mps_e2]
	jmp	g648d
g6350:
	push	ds
	pop	es
	lea bx,[mps_exec0]
	mov	word ptr [bx], ax
	mov	word ptr [bx + 2], ax
	mov	ax, 04b03h
	mov	dx, word ptr [bp + 6]
	call	g646c
	jae	g6387
	cmp	ax, 2
	jne	g6372
	lea si,[mps_e3]
	jmp	g648d
g6372:
	cmp	ax, 8
	jne	g637e
	lea si,[mps_e4]
	jmp	g648d
g637e:
	je	g6387
	lea si,[mps_e5]
	jmp	g648d
g6387:
	mov es,[mps_exec0]
	mov	bx, word ptr es:[02ah]
	mov	ax, word ptr es:[02ch]
	add	ax, 0fh
	rcr	ax, 1
	shr	ax, 1
	shr	ax, 1
	shr	ax, 1
	add	bx, ax
	mov	dx, word ptr [bp + 8]
	or	dx, dx
	je	g63ab
	call	g63d0
g63ab:
	mov	ax, es
	sub	bx, ax
	add	bx, 8
	cmp bx,[mps_freep]
	jbe	g63bf
	lea si,[mps_e6]
	jmp	g648d
g63bf:
	mov	ah, 04ah
	int	021h
	jae	g63cc
	lea si,[mps_e7]
	jmp	g648d
g63cc:
	mov	ax, es
	pop	bp
	retf
g63d0:
	push	ds
	mov	ax, 03d00h
	int	021h
	jae	g63df
	lea si,[mps_e3]
	jmp	g648d
g63df:
	mov	ds, bx
	mov	bx, ax
g63e3:
	mov	ah, 03fh
	xor	dx, dx
	mov	cx, 08000h
	int	021h
	jae	g63f5
	lea si,[mps_e3]
	jmp	g648d
g63f5:
	cmp	ax, cx
	jb	g6402
	mov	ax, ds
	add	ax, 0800h
	mov	ds, ax
	jmp short	g63e3
g6402:
	add	ax, 0fh
	rcr	ax, 1
	shr	ax, 1
	shr	ax, 1
	shr	ax, 1
	mov	dx, ds
	add	dx, ax
	mov	ah, 03eh
	int	021h
	jae	g641e
	lea si,[mps_e3]
	jmp short	g648d
	nop
g641e:
	mov	bx, dx
	pop	ds
	ret
	PUBLIC	_InitOverlay
_InitOverlay	LABEL FAR
	push	bp
	mov	bp, sp
	push	si
	push	di
	push	ds
	mov	ds, word ptr [bp + 6]
	mov	si, 032h
	mov	ax, word ptr ds:[02eh]
	mov	dl, 7
	mul	dl
	lea di,[g601f]
	add	di, ax
	mov ax,seg g5ce0
	mov	es, ax
	mov	cx, word ptr ds:[030h]
	mov	ax, word ptr ds:[028h]
g6447:
	add	di, 3
	movsw	word ptr es:[di], word ptr [si]
	stosw	word ptr es:[di], ax
	loop	g6447
	pop	ds
	pop	di
	pop	si
	pop	bp
	retf
	push	bp
	mov	bp, sp
	mov	es, word ptr [bp + 6]
	mov	ah, 049h
	int	021h
	jae	g6466
	lea si,[mps_e8]
	jmp short	g648d
	nop
g6466:
	pop	bp
	retf
cs_sp	LABEL BYTE
	db	000h,000h
cs_ss	LABEL BYTE
	db	000h,000h
g646c:
	push	bp
	push	si
	push	di
	push	ds
	push	es
	mov	word ptr cs:[cs_ss], ss
	mov	word ptr cs:[cs_sp], sp
	int	021h
	mov	ss, word ptr cs:[cs_ss]
	mov	sp, word ptr cs:[cs_sp]
	pop	es
	pop	ds
	pop	di
	pop	si
	pop	bp
	ret
g648d:
	mov	ah, 2
	cmp	al, 0ah
	sbb	al, 069h
	das
	mov	dl, al
	int	021h
	lea dx,[mps_dash]
	mov	ah, 9
	int	021h
	mov	ah, 9
	mov	dx, si
	int	021h
	mov	ax, 04c99h
	int	021h
	db	000h
	PUBLIC	_RectFill
_RectFill	LABEL FAR
	push	bp
	mov	bp, sp
	push	di
	push	si
	xor	ax, ax
	cmp	word ptr [bp + 0ch], ax
	jle	g651f
	cmp	word ptr [bp + 0eh], ax
	jle	g651f
	call	g6540
	mov	bx, word ptr [bp + 6]
	mov	ax, word ptr [bx]
	call far ptr g6171
	mov	si, word ptr [bp + 0ah]
	add	si, word ptr [bx + 4]
	mov [mps_penx],si
	mov	cx, word ptr [bp + 0eh]
	shl	si, 1
	lea di,[mps_span0]
	add	di, si
	mov	ax, word ptr [bp + 8]
	add	ax, word ptr [bx + 2]
	mov	dx, cx
	rep stosw	word ptr es:[di], ax
	mov	cx, dx
	lea di,[mps_span1]
	add	di, si
	mov	ax, word ptr [bp + 8]
	add	ax, word ptr [bx + 2]
	add	ax, word ptr [bp + 0ch]
	dec	ax
	rep stosw	word ptr es:[di], ax
	mov	ax, word ptr [bp + 010h]
	call far ptr g6140
	mov ax,[mps_penx]
	mov	cx, ax
	add	cx, word ptr [bp + 0eh]
	dec	cx
	mov [mps_peny],cx
	mov	bx, 05740h
	call far ptr g6132
	call far ptr g6139
g651f:
	pop	si
	pop	di
	pop	bp
	retf
	db	000h
	push	di
	push	si
	push	bp
	mov	ax, word ptr ds:[05a64h]
	mov	cx, word ptr ds:[05a66h]
	mov	bx, 05740h
	call	far ptr g6132
	pop	bp
	pop	si
	pop	di
	retf
	push	di
	call	g6540
	pop	di
	retf
g6540:
	mov ax,DGROUP
	mov	es, ax
	mov di,[mps_penx]
	or	di, di
	js	g6575
	mov cx,[mps_peny]
	inc	cx
	sub	cx, di
	shl	di, 1
	mov	bx, cx
	mov	dx, di
	add	di, 05740h
	mov	ax, 0ffffh
	rep stosw	word ptr es:[di], ax
	mov [mps_penx],ax
	mov	cx, bx
	mov	di, dx
	add	di, 058d0h
	sub	ax, ax
	rep stosw	word ptr es:[di], ax
	mov [mps_peny],ax
g6575:
	ret
g6576:
	mov ax,DGROUP
	mov	es, ax
	mov	cx, di
	or	si, si
	lea di,[mps_span0]
	je	g658f
	cmp si,[mps_clipR]
	jne	g65d0
	lea di,[mps_span1]
g658f:
	cmp	cx, dx
	jge	g6595
	xchg	cx, dx
g6595:
	or	dx, dx
	jns	g659b
	sub	dx, dx
g659b:
	cmp cx,[mps_clipB]
	jle	g65a5
	mov cx,[mps_clipB]
g65a5:
	add si,[mps_clipL]
	add cx,[mps_clipT]
	add dx,[mps_clipT]
	cmp cx,[mps_peny]
	jbe	g65bb
	mov [mps_peny],cx
g65bb:
	cmp dx,[mps_penx]
	jae	g65c5
	mov [mps_penx],dx
g65c5:
	add	di, dx
	add	di, dx
	sub	cx, dx
	inc	cx
	mov	ax, si
	rep stosw	word ptr es:[di], ax
g65d0:
	ret
g65d1:
	sub	si, si
	or	bx, bx
	js	g6576
	mov si,[mps_clipR]
	cmp	bx, si
	jg	g6576
	ret
g65e0:
	mov ax,[mps_clipL]
	add [mps_x1],ax
	add [mps_x2],ax
	mov ax,[mps_clipT]
	add [mps_y1],ax
	add [mps_y2],ax
	mov ax,[mps_x2]
	cmp ax,[mps_x1]
	jae	g6618
	mov bx,[mps_x1]
	mov [mps_x2],bx
	mov [mps_x1],ax
	mov ax,[mps_y2]
	mov bx,[mps_y1]
	mov [mps_y2],bx
	mov [mps_y1],ax
g6618:
	mov bx,[mps_x2]
	sub bx,[mps_x1]
	mov [ln_tmp0],bx
	mov ax,[mps_y2]
	mov	dx, ax
	mov cx,[mps_y1]
	sub	ax, cx
	mov	bp, 2
	jns	g663a
	xchg	cx, dx
	neg	bp
	neg	ax
g663a:
	mov [ln_tmp1],ax
	cmp dx,[mps_peny]
	jbe	g6647
	mov [mps_peny],dx
g6647:
	cmp cx,[mps_penx]
	jae	g6651
	mov [mps_penx],cx
g6651:
	cmp	ax, bx
	jae	g66a0
	mov di,[mps_y1]
	shl	di, 1
	mov ax,[mps_x1]
	mov dx,[ln_tmp0]
	mov	cx, dx
	mov	bx, cx
	inc	bx
	shr	bx, 1
	neg	bx
	mov si,[ln_tmp1]
	jmp short	g6681
	nop
g6672:
	cmp	ax, word ptr [di + 058d0h]
	jbe	g667c
	mov	word ptr [di + 058d0h], ax
g667c:
	inc	ax
	sub	bx, dx
	add	di, bp
g6681:
	cmp	ax, word ptr [di + 05740h]
	jae	g668b
	mov	word ptr [di + 05740h], ax
g668b:
	dec	cx
	js	g6695
	add	bx, si
	jns	g6672
	inc	ax
	jmp short	g668b
g6695:
	cmp	ax, word ptr [di + 058d0h]
	jbe	g669f
	mov	word ptr [di + 058d0h], ax
g669f:
	ret
g66a0:
	mov di,[mps_y1]
	shl	di, 1
	mov ax,[mps_x1]
	mov dx,[ln_tmp1]
	mov	cx, dx
	mov	bx, cx
	inc	bx
	shr	bx, 1
	neg	bx
	mov si,[ln_tmp0]
g66ba:
	cmp	ax, word ptr [di + 05740h]
	jae	g66c4
	mov	word ptr [di + 05740h], ax
g66c4:
	cmp	ax, word ptr [di + 058d0h]
	jbe	g66ce
	mov	word ptr [di + 058d0h], ax
g66ce:
	dec	cx
	js	g66dc
	add	di, bp
	add	bx, si
	js	g66ba
	sub	bx, dx
	inc	ax
	jmp short	g66ba
g66dc:
	ret
	db	000h
	call	g66ee
	retf
	push	bp
	push	si
	push	di
	push	es
	call	g66ee
	pop	es
	pop	di
	pop	si
	pop	bp
	retf
g66ee:
	sub	ax, ax
	mov	es, ax
	push	word ptr es:[0]
	push	word ptr es:[2]
	lea	ax, ds:[0bbbh]
	mov	word ptr es:[0], ax
	mov word ptr es:[2],seg g5ce0
	push	ds
	pop	es
	jmp short	g6776
	nop
g6710:
	sub	ax, ax
	mov	es, ax
	pop	word ptr es:[2]
	pop	word ptr es:[0]
	push	ds
	pop	es
	mov ax,[mps_x1]
	mov bx,[mps_y1]
	mov cx,[mps_x2]
	mov dx,[mps_y2]
	call far ptr g6147
	clc
	ret
g6736:
	sub	ax, ax
	mov	es, ax
	pop	word ptr es:[2]
	pop	word ptr es:[0]
	push	ds
	pop	es
	stc
	ret
g6748:
	cmc
	rcr	dx, 1
	mov [eg_dx],dx
	sar	dx, 1
	mov [eg_cx],dx
	mov	dx, di
	sub	dx, bp
	jno	g6761
	cmc
	rcr	dx, 1
	jmp short	g67d1
	nop
g6761:
	sar	dx, 1
	jmp short	g67d1
	nop
g6766:
	cmc
	rcr	dx, 1
	sar word ptr [eg_dx],1
	sar word ptr [eg_cx],1
	jmp short	g67d1
	nop
g6774:
	jmp short	g6736
g6776:
	mov cx,[mps_x1]
	mov dx,[mps_y1]
	mov si,[mps_x2]
	mov di,[mps_y2]
	mov	bx, cx
	mov	bp, dx
	call	g687a
	mov byte ptr [eg_flag],al
	mov	bx, si
	mov	bp, di
	call	g687a
	jne	g67b3
	cmp byte ptr [eg_flag],00h
	jne	g67a3
	jmp	g6710
g67a3:
	xchg	cx, si
	xchg	dx, di
	xchg al,[eg_flag]
	mov [mps_x1],cx
	mov [mps_y1],dx
g67b3:
	test byte ptr [eg_flag],al
	jne	g6774
	mov	bp, dx
	mov	dx, si
	sub	dx, cx
	jo	g6748
	mov [eg_dx],dx
	sar	dx, 1
	mov [eg_cx],dx
	mov	dx, di
	sub	dx, bp
	jo	g6766
g67d1:
	mov [eg_dy],dx
	sar	dx, 1
	mov [eg_cy],dx
g67db:
	test	al, 9
	je	g6817
	sub	bx, bx
	or	si, si
	js	g67e9
	mov bx,[mps_clipR]
g67e9:
	mov	ax, bx
	sub	ax, cx
	imul word ptr [eg_dy]
	push	bx
	mov	bx, dx
	idiv word ptr [eg_dx]
	mov	bl, bh
	xor bl,[eg_s0]
	jns	g6803
	neg	dx
	dec	ax
g6803:
	sub dx,[eg_cx]
	xor	dh, bh
	js	g680c
	inc	ax
g680c:
	pop	bx
	add	ax, bp
	js	g681f
	cmp ax,[mps_clipB]
	jle	g6850
g6817:
	mov bx,[mps_clipB]
	cmp	di, bx
	jg	g6821
g681f:
	sub	bx, bx
g6821:
	mov	ax, bx
	sub	ax, bp
	imul word ptr [eg_dx]
	push	bx
	mov	bx, dx
	idiv word ptr [eg_dy]
	mov	bl, bh
	xor bl,[eg_s1]
	jns	g683b
	neg	dx
	dec	ax
g683b:
	sub dx,[eg_cy]
	xor	dh, bh
	js	g6844
	inc	ax
g6844:
	pop	bx
	add	ax, cx
	js	g6861
	cmp ax,[mps_clipR]
	jg	g6861
	xchg	bx, ax
g6850:
	cmp byte ptr [eg_flag],00h
	jne	g6864
	mov [mps_y2],ax
	mov [mps_x2],bx
	jmp	g6710
g6861:
	jmp	g6736
g6864:
	mov [mps_y1],ax
	mov [mps_x1],bx
	xchg	cx, si
	xchg	bp, di
	mov al,[eg_flag]
	mov byte ptr [eg_flag],00h
	jmp	g67db
g687a:
	mov	al, 0fh
	or	bx, bx
	js	g6882
	and	al, 0f7h
g6882:
	cmp bx,[mps_clipR]
	jg	g688a
	and	al, 0feh
g688a:
	or	bp, bp
	js	g6890
	and	al, 0fbh
g6890:
	cmp bp,[mps_clipB]
	jg	g6898
	and	al, 0fdh
g6898:
	or	al, al
	ret
	push	bp
	mov	bp, sp
	cmp	word ptr [bp + 2], 0b4ch
	mov ax,[eg_dy]
	je	g68ab
	mov ax,[eg_dx]
g68ab:
	xor	dx, ax
	mov	ax, 07f00h
	jns	g68b4
	neg	ax
g68b4:
	add	word ptr [bp + 2], 4
	sub	dx, dx
	pop	bp
	iret
	PUBLIC	_DrawLine
_DrawLine	LABEL FAR
	push	bp
	mov	bp, sp
	push	si
	push	di
	mov	bx, word ptr [bp + 6]
	mov	ax, word ptr [bx + 6]
	mov [mps_clipR],ax
	mov	ax, word ptr [bx + 8]
	mov [mps_clipB],ax
	call far ptr g6178
	mov	ax, word ptr [bp + 8]
	mov [mps_x1],ax
	mov	ax, word ptr [bp + 0ah]
	mov [mps_y1],ax
	mov	ax, word ptr [bp + 0ch]
	mov [mps_x2],ax
	mov	ax, word ptr [bp + 0eh]
	mov [mps_y2],ax
	mov	ax, word ptr [bp + 010h]
	call far ptr g6140
	call	g66ee
	call far ptr g6139
	pop	di
	pop	si
	pop	bp
	retf
	PUBLIC	_DrawDot
_DrawDot	LABEL FAR
	push	bp
	mov	bp, sp
	push	si
	push	di
	mov	bx, word ptr [bp + 6]
	mov	cx, word ptr [bp + 8]
	or	cx, cx
	js	g693c
	cmp	cx, word ptr [bx + 6]
	jg	g693c
	mov	dx, word ptr [bp + 0ah]
	or	dx, dx
	js	g693c
	cmp	dx, word ptr [bx + 8]
	jg	g693c
	call far ptr g6178
	mov	ax, word ptr [bp + 0ch]
	call far ptr g6140
	mov	ax, cx
	mov	bx, dx
	call far ptr g612b
	call far ptr g6139
g693c:
	pop	di
	pop	si
	pop	bp
	retf
	PUBLIC	_DrawPoly
_DrawPoly	LABEL FAR
	push	bp
	mov	bp, sp
	push	es
	push	ds
	push	si
	push	di
	mov	ax, word ptr [bp + 0ch]
	call far ptr g6140
	mov	bx, word ptr [bp + 6]
	call far ptr g6178
	mov	ax, word ptr [bx + 2]
	mov [mps_clipL],ax
	mov	ax, word ptr [bx + 4]
	mov [mps_clipT],ax
	mov	ax, word ptr [bx + 6]
	mov [mps_clipR],ax
	mov	ax, word ptr [bx + 8]
	mov [mps_clipB],ax
	call	g6540
	mov	si, word ptr [bp + 0ah]
	mov	cx, word ptr [bp + 8]
	dec	cx
	lodsw	ax, word ptr [si]
	mov [mps_x1],ax
	push	ax
	lodsw	ax, word ptr [si]
	mov [mps_y1],ax
	push	ax
g6983:
	push	cx
	lodsw	ax, word ptr [si]
	mov [mps_x2],ax
	push	ax
	lodsw	ax, word ptr [si]
	mov [mps_y2],ax
	push	ax
	push	si
	call	g6bbe
	pop	si
	pop word ptr [mps_y1]
	pop word ptr [mps_x1]
	pop	cx
	loop	g6983
	pop word ptr [mps_y2]
	pop word ptr [mps_x2]
	call	g6bbe
	mov ax,[mps_penx]
	mov cx,[mps_peny]
	mov	bx, 05740h
	call far ptr g6132
	call far ptr g6139
	pop	di
	pop	si
	pop	ds
	pop	es
	pop	bp
	retf
	db	000h
	PUBLIC	_MouseON
_MouseON	LABEL FAR
	xor	ax, ax
	mov	es, ax
	mov	ax, word ptr es:[0cch]
	or	ax, word ptr es:[0ceh]
	je	g6a09
	mov	ax, 0
	int	033h
	or	ax, ax
	je	g6a09
	mov	ax, 0ch
	mov	cx, 01fh
	mov	dx, cs
	mov	es, dx
	lea	dx, ds:[0e83h]
	int	033h
	mov	ax, 3
	int	033h
	mov	ax, cx
	mov	cl, ch
	shr	ax, cl
	mov byte ptr [mps_xsh],cl
	mov [_MouseX],ax
	mov [_MouseY],dx
	mov [_Button],bx
	mov	ax, 0ffffh
g6a09:
	mov byte ptr [_MOUSE],al
	retf
	PUBLIC	_MouseOFF
_MouseOFF	LABEL FAR
	xor	al, al
	xchg al,[_MOUSE]
	or	al, al
	je	g6a1c
	mov	ax, 0
	int	033h
g6a1c:
	retf
	PUBLIC	_MouseTYPE
_MouseTYPE	LABEL FAR
	push	bp
	mov	bp, sp
	mov	ax, word ptr [bp + 6]
	mov [mps_hotx],ax
	mov	ax, word ptr [bp + 8]
	mov [mps_hoty],ax
	mov	ax, word ptr [bp + 0ah]
	mov [mps_curs],ax
	pop	bp
	retf
	PUBLIC	_MouseSPOT
_MouseSPOT	LABEL FAR
	push	bp
	mov	bp, sp
	mov	ax, word ptr [bp + 6]
	mov	dx, word ptr [bp + 8]
	mov [_MouseX],ax
	mov [_MouseY],dx
	cmp byte ptr [_MOUSE],00h
	je	g6a58
	mov cl,[mps_xsh]
	shl	ax, cl
	mov	cx, ax
	mov	ax, 4
	int	033h
g6a58:
	pop	bp
	retf
	PUBLIC	_MouseHITS
_MouseHITS	LABEL FAR
	xor	ax, ax
	xchg ax,[mps_hits]
	retf
	PUBLIC	_MouseSHOW
_MouseSHOW	LABEL FAR
	cmp word ptr [mps_curs],+00h
	je	g6a90
	cmp byte ptr [dg_drawn],00h
	jne	g6a90
	push word ptr [mps_curs]
	mov ax,[_MouseY]
	sub ax,[mps_hoty]
	push	ax
	mov ax,[_MouseX]
	sub ax,[mps_hotx]
	push	ax
	call far ptr g60f1
	add	sp, 6
	mov byte ptr [dg_drawn],01h
g6a90:
	retf
	PUBLIC	_MouseHIDE
_MouseHIDE	LABEL FAR
	cmp word ptr [mps_curs],+00h
	je	g6aa9
	cmp byte ptr [dg_drawn],00h
	je	g6aa9
	mov byte ptr [dg_drawn],00h
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
	mov	byte ptr cs:[cs_lock], al
	mov	ax, word ptr [bp + 6]
	call	far ptr g605e
g6ad0:
	pop	bp
	retf
	cmp	word ptr ds:[05a82h], 0
	je	g6aed
	cmp	byte ptr cs:[cs_lock], 0
	je	g6aed
	call	far ptr g6088
	mov	al, byte ptr cs:[cs_lock]
	mov	byte ptr ds:[0560fh], al
g6aed:
	retf
	push	bp
	mov	bp, sp
	mov	ax, word ptr [bp + 6]
	mov	bx, word ptr [bp + 8]
	mov	cx, word ptr [bp + 0ah]
	mov	dx, word ptr [bp + 0ch]
	int	033h
	mov	word ptr ds:[05a8eh], ax
	mov	word ptr ds:[05a90h], bx
	mov	word ptr ds:[05a92h], cx
	mov	word ptr ds:[05a94h], dx
	cmp	word ptr [bp + 6], 6
	ja	g6b1c
	mov	cl, byte ptr ds:[05a88h]
	shr	word ptr ds:[05a92h], cl
g6b1c:
	lea	ax, ds:[05a8eh]
	pop	bp
	retf
g6b22:
	push	ax
	push	dx
	push	es
	mov ax,seg g5ce0
	mov	es, ax
	call	dword ptr es:[03abh]
	push word ptr [mps_curs]
	mov ax,[_MouseY]
	sub ax,[mps_hoty]
	push	ax
	mov ax,[_MouseX]
	sub ax,[mps_hotx]
	push	ax
	mov ax,seg g5ce0
	mov	es, ax
	call	dword ptr es:[0414h]
	add	sp, 6
	pop	es
	pop	dx
	pop	ax
	ret
	push	bp
	mov	bp, sp
	les	ax, dword ptr [bp + 6]
	mov byte ptr [mps_ucb],ax
	mov byte ptr [mps_ucb+2],es
	pop	bp
	retf
	mov	bp, 055f1h
	mov	ds, bp
	mov	bp, cx
	mov	cl, byte ptr ds:[05a88h]
	shr	bp, cl
	mov	cx, bp
	or	word ptr ds:[05a80h], bx
	mov	word ptr ds:[05a7eh], bx
	mov	word ptr ds:[05a7ah], cx
	mov	word ptr ds:[05a7ch], dx
	mov	bp, word ptr ds:[05a8ah]
	or	bp, word ptr ds:[05a8ch]
	mov	bp, ax
	je	g6b92
	call	dword ptr ds:[05a8ah]
g6b92:
	test	bp, 1
	je	g6bb1
	cmp	byte ptr ds:[0560fh], 0
	je	g6bb1
	mov	al, 2
	xchg	al, byte ptr ds:[0560eh]
	or	al, al
	jne	g6bb1
	call	g6b22
	mov	byte ptr ds:[0560eh], 0
g6bb1:
	retf
	push	es
	push	si
	push	di
	push	bp
	call	g6bbe
	pop	bp
	pop	di
	pop	si
	pop	es
	retf
g6bbe:
	sub	ax, ax
	mov	es, ax
	push	word ptr es:[0]
	push	word ptr es:[2]
	lea ax,[g6d66]
	mov	word ptr es:[0], ax
	mov word ptr es:[2],seg g5ce0
	push	ds
	pop	es
	jmp short	g6c2d
	nop
g6be0:
	sub	ax, ax
	mov	es, ax
	pop	word ptr es:[2]
	pop	word ptr es:[0]
	push	ds
	pop	es
	ret
g6bf1:
	and al,[eg2_flag]
	test	al, 6
	jne	g6be0
	mov	bx, cx
	jmp	g6d28
g6bfe:
	jmp	g6d53
g6c01:
	cmc
	rcr	dx, 1
	mov [eg2_dx],dx
	sar	dx, 1
	mov [eg2_cx],dx
	mov	dx, di
	sub	dx, bp
	jno	g6c1a
	cmc
	rcr	dx, 1
	jmp short	g6c85
	nop
g6c1a:
	sar	dx, 1
	jmp short	g6c85
	nop
g6c1f:
	cmc
	rcr	dx, 1
	sar word ptr [eg2_dx],1
	sar word ptr [eg2_cx],1
	jmp short	g6c85
	nop
g6c2d:
	mov cx,[mps_x1]
	mov dx,[mps_y1]
	mov si,[mps_x2]
	mov di,[mps_y2]
	mov	bx, cx
	mov	bp, dx
	call	g687a
	mov byte ptr [eg2_flag],al
	mov	bx, si
	mov	bp, di
	call	g687a
	jne	g6c67
	cmp byte ptr [eg2_flag],00h
	je	g6bfe
	xchg	cx, si
	xchg	dx, di
	xchg al,[eg2_flag]
	mov [mps_x1],cx
	mov [mps_y1],dx
g6c67:
	mov	bp, dx
	test byte ptr [eg2_flag],al
	jne	g6bf1
	mov	dx, si
	sub	dx, cx
	jo	g6c01
	mov [eg2_dx],dx
	sar	dx, 1
	mov [eg2_cx],dx
	mov	dx, di
	sub	dx, bp
	jo	g6c1f
g6c85:
	mov [eg2_dy],dx
	sar	dx, 1
	mov [eg2_cy],dx
g6c8f:
	test	al, 9
	je	g6ccb
	sub	bx, bx
	or	si, si
	js	g6c9d
	mov bx,[mps_clipR]
g6c9d:
	mov	ax, bx
	sub	ax, cx
	imul word ptr [eg2_dy]
	push	bx
	mov	bx, dx
	idiv word ptr [eg2_dx]
	mov	bl, bh
	xor bl,[eg2_s0]
	jns	g6cb7
	neg	dx
	dec	ax
g6cb7:
	sub dx,[eg2_cx]
	xor	dh, bh
	js	g6cc0
	inc	ax
g6cc0:
	pop	bx
	add	ax, bp
	js	g6cd3
	cmp ax,[mps_clipB]
	jle	g6d04
g6ccb:
	mov bx,[mps_clipB]
	cmp	di, bx
	jg	g6cd5
g6cd3:
	sub	bx, bx
g6cd5:
	mov	ax, bx
	sub	ax, bp
	imul word ptr [eg2_dx]
	push	bx
	mov	bx, dx
	idiv word ptr [eg2_dy]
	mov	bl, bh
	xor bl,[eg2_s1]
	jns	g6cef
	neg	dx
	dec	ax
g6cef:
	sub dx,[eg2_cy]
	xor	dh, bh
	js	g6cf8
	inc	ax
g6cf8:
	pop	bx
	add	ax, cx
	xchg	bx, ax
	js	g6d28
	cmp bx,[mps_clipR]
	jg	g6d28
g6d04:
	push	ax
	push	bx
	push	cx
	push	si
	push	di
	mov	dx, di
	mov	di, ax
	mov	si, bx
	call	g6576
	pop	di
	pop	si
	pop	cx
	pop	bx
	pop	ax
	cmp byte ptr [eg2_flag],00h
	jne	g6d3d
	mov [mps_y2],ax
	mov [mps_x2],bx
	jmp short	g6d53
	nop
g6d28:
	mov	dx, bp
	sub	ax, ax
	mov	es, ax
	pop	word ptr es:[2]
	pop	word ptr es:[0]
	push	ds
	pop	es
	jmp	g65d1
g6d3d:
	mov [mps_y1],ax
	mov [mps_x1],bx
	xchg	cx, si
	xchg	bp, di
	mov al,[eg2_flag]
	mov byte ptr [eg2_flag],00h
	jmp	g6c8f
g6d53:
	sub	ax, ax
	mov	es, ax
	pop	word ptr es:[2]
	pop	word ptr es:[0]
	push	ds
	pop	es
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
	mov	bp, sp
	push	si
	push	di
	xor	ax, ax
	push	ax
	call far ptr g60e5
	add	sp, 2
g6d98:
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6db0
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6db0:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	cmp	al, 058h
	je	g6e32
	mov	di, ds
	mov	es, di
	lea di,[pic_dict]
	cmp	ax, 0304dh
	jne	g6dd7
	cmp	word ptr [bp + 6], 1
	jb	g6dd4
	je	g6dd7
	mov	di, word ptr [bp + 6]
	jmp short	g6dd7
	nop
g6dd4:
	add	di, 2
g6dd7:
	push	di
	stosw	word ptr es:[di], ax
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6df1
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6df1:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	stosw	word ptr es:[di], ax
	mov	cx, ax
	shr	cx, 1
	jcxz	g6e1d
g6dfd:
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6e15
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6e15:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	stosw	word ptr es:[di], ax
	loop	g6dfd
g6e1d:
	pop	di
	lea ax,[pic_dict]
	cmp	ax, di
	jne	g6e2f
	push	ax
	call far ptr g60e5
	add	sp, 2
g6e2f:
	jmp	g6d98
g6e32:
	and	ah, 1
	mov byte ptr [pic_state+41],ah
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6e51
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6e51:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	mov byte ptr [pic_state+24],ax
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6e71
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6e71:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	mov byte ptr [pic_state+20],ax
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6e91
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6e91:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	mov byte ptr [pic_state+22],ax
	call	g6f21
	pop	di
	pop	si
	pop	bp
	retf
g6ea0:
	push	si
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6eb9
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6eb9:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	and	al, 1
	mov byte ptr [pic_state+41],al
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6edb
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6edb:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	mov byte ptr [pic_state+24],ax
	call	g6f21
	pop	si
	retf
g6ee8:
	push	bp
	mov	bp, sp
	push	si
	push	di
	mov si,[pic_src]
	les	di, dword ptr [bp + 6]
	mov	cx, word ptr [bp + 0ah]
	call	g6fa0
	mov [pic_src],si
	pop	di
	pop	si
	pop	bp
	retf
g6f02:
	push	bp
	mov	bp, sp
	push	si
	push	di
	mov	ax, ds
	mov	es, ax
	mov si,[pic_src]
	mov	di, word ptr [bp + 6]
	mov cx,[pic_state+20]
	call	g6fa0
	mov [pic_src],si
	pop	di
	pop	si
	pop	bp
	retf
g6f21:
	mov ax,[pic_state+20]
	or ax,[pic_state+22]
	jne	g6f2b
	ret
g6f2b:
	mov byte ptr [pic_state+30],00h
	mov byte ptr [pic_state+31],00h
	lea ax,[pic_6425]
	mov byte ptr [pic_state+26],ax
	mov si,[pic_src]
	cmp si,[pic_bound]
	jb	g6f54
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g6f54:
	lodsw	ax, word ptr [si]
	mov [pic_src],si
	cmp	al, 0bh
	jbe	g6f5f
	mov	al, 0bh
g6f5f:
	mov byte ptr [pic_state+33],al
	mov byte ptr [pic_state+38],ax
	mov byte ptr [pic_state+40],08h
g6f6a:
	mov byte ptr [pic_state+32],09h
	mov word ptr [pic_state+34],01ffh
	mov	dx, 0100h
	mov byte ptr [pic_state+36],dx
	mov	ax, 0ffffh
	xor	bx, bx
	mov	cx, 0800h
g6f84:
	mov	word ptr [bx - 06f6ch], ax
	add	bx, 3
	loop	g6f84
	mov	al, 0
	xor	bx, bx
	mov	cx, 0100h
g6f94:
	mov	byte ptr [bx - 06f6ah], al
	inc	al
	add	bx, 3
	loop	g6f94
	ret
g6fa0:
	cmp byte ptr [pic_state+41],00h
	je	g6faa
	inc	cx
	shr	cx, 1
g6faa:
	mov byte ptr [pic_state+28],cx
	mov dx,[pic_state+36]
	xchg sp,[pic_state+26]
g6fb6:
	cmp byte ptr [pic_state+30],00h
	jne	g6fde
	call	g7012
	cmp	al, 090h
	je	g6fca
	mov byte ptr [pic_state+31],al
	jmp short	g6fe5
	nop
g6fca:
	call	g7012
	or	al, al
	jne	g6fd9
	mov	al, 090h
	mov byte ptr [pic_state+31],al
	jmp short	g6fe5
	nop
g6fd9:
	dec	al
	mov byte ptr [pic_state+30],al
g6fde:
	mov al,[pic_state+31]
	dec byte ptr [pic_state+30]
g6fe5:
	cmp byte ptr [pic_state+41],00h
	je	g7002
	mov	ah, al
	and	al, 0fh
	shr	ah, 1
	shr	ah, 1
	shr	ah, 1
	shr	ah, 1
	stosw	word ptr es:[di], ax
	dec word ptr [pic_state+28]
	jne	g6fb6
	jmp short	g7009
	nop
g7002:
	stosb	byte ptr es:[di], al
	dec word ptr [pic_state+28]
	jne	g6fb6
g7009:
	mov byte ptr [pic_state+36],dx
	xchg sp,[pic_state+26]
	ret
g7012:
	pop	bp
	cmp	sp, 06425h
	je	g701c
	pop	ax
	jmp	bp
g701c:
	mov bx,[pic_state+38]
	mov	cl, 010h
	mov ch,[pic_state+40]
	sub	cl, ch
	shr	bx, cl
	mov	cl, ch
g702c:
	cmp cl,[pic_state+32]
	jge	g7053
	cmp si,[pic_bound]
	jb	g7046
	push	bx
	push	cx
	push	dx
	call dword ptr [pic_pump]
	pop	dx
	pop	cx
	pop	bx
	mov si,[pic_src]
g7046:
	lodsw	ax, word ptr [si]
	mov byte ptr [pic_state+38],ax
	shl	ax, cl
	or	bx, ax
	add	cl, 010h
	jmp short	g702c
g7053:
	sub cl,[pic_state+32]
	mov byte ptr [pic_state+40],cl
	mov	ax, bx
	and ax,[pic_state+34]
	mov	cx, ax
	cmp	ax, dx
	jl	g7071
	mov	cx, dx
	mov ax,[pic_state+42]
	mov bl,[pic_state+44]
	push	bx
g7071:
	mov	bx, ax
	add	bx, ax
	add	bx, ax
	mov	ax, word ptr [bx - 06f6ch]
	inc	ax
	je	g7086
	dec	ax
	mov	bl, byte ptr [bx - 06f6ah]
	push	bx
	jmp short	g7071
g7086:
	mov	al, byte ptr [bx - 06f6ah]
	mov byte ptr [pic_state+44],al
	push	ax
	mov	bx, dx
	add	bx, dx
	add	bx, dx
	mov	byte ptr [bx - 06f6ah], al
	mov ax,[pic_state+42]
	mov	word ptr [bx - 06f6ch], ax
	inc	dx
	cmp dx,[pic_state+34]
	jle	g70af
	inc byte ptr [pic_state+32]
	stc
	rcl word ptr [pic_state+34],1
g70af:
	mov al,[pic_state+32]
	cmp al,[pic_state+33]
	jle	g70bb
	call	g6f6a
g70bb:
	mov byte ptr [pic_state+42],cx
	db	0e9h		; near-jmp opcode, operand read from next segment at runtime
MPSLIBM_TEXT ENDS
PIC_TEXT SEGMENT BYTE PUBLIC 'CODE'
	ASSUME CS:PIC_TEXT, DS:DGROUP
p40b50:
	push	bp
	mov	bp, sp
	sub	sp, 01ah
	lea	ax, [bp - 01ah]
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p4129a
	add	sp, 4
	or	ax, ax
	je	p40b9c
	push	word ptr [bp - 4]
	push	word ptr [bp - 6]
	push word ptr [pic_handle]
	push	cs
	call	p413cc
	add	sp, 6
	or	ax, ax
	je	p40b87
	push	word ptr [bp + 6]
	push	cs
	call	p411e2
	add	sp, 2
p40b87:
	mov	ax, word ptr [bp - 0ah]
	mov	dx, word ptr [bp - 8]
	mov byte ptr [pic_state],ax
	mov byte ptr [pic_state+2],dx
	mov ax,[pic_handle]
	mov	sp, bp
	pop	bp
	retf
	nop
p40b9c:
	lea	ax, [bp - 2]
	push	ax
	push	word ptr [bp + 8]
	push	word ptr [bp + 6]
	call far ptr __dos_open
	add	sp, 6
	or	ax, ax
	je	p40bbc
	push	word ptr [bp + 6]
	push	cs
	call	p411e2
	add	sp, 2
p40bbc:
	mov	ax, word ptr [bp - 2]
	mov	sp, bp
	pop	bp
	retf
	nop
p40bc4:
	push	bp
	mov	bp, sp
	sub	sp, 2
	lea	ax, [bp - 2]
	push	ax
	sub	ax, ax
	push	ax
	push	word ptr [bp + 6]
	call far ptr l_dos_creat
	add	sp, 6
	or	ax, ax
	je	p40bea
	push	word ptr [bp + 6]
	push	cs
	call	p411e2
	add	sp, 2
p40bea:
	mov	ax, word ptr [bp - 2]
	mov	sp, bp
	pop	bp
	retf
	nop
p40bf2:
	push	bp
	mov	bp, sp
	mov ax,[pic_handle]
	cmp	word ptr [bp + 6], ax
	je	p40c16
	push	word ptr [bp + 6]
	call far ptr __dos_close
	add	sp, 2
	or	ax, ax
	je	p40c16
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p40c16:
	pop	bp
	retf
	push	bp
	mov	bp, sp
	sub	sp, 4
	sub	ax, ax
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p40b50
	add	sp, 4
	mov	word ptr [bp - 4], ax
	cmp word ptr [pic_handle],-01h
	jne	p40c3a
	mov	ax, 0ffffh
	jmp short	p40c3d
p40c3a:
	mov ax,[pic_state]
p40c3d:
	mov	word ptr [bp - 2], ax
	lea	ax, [bp - 2]
	push	ax
	push	word ptr [bp - 2]
	push	ds
	push	word ptr [bp + 8]
	push	word ptr [bp - 4]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	je	p40c64
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p40c64:
	push	word ptr [bp - 4]
	push	cs
	call	p40bf2
	mov	ax, word ptr [bp - 2]
	mov	sp, bp
	pop	bp
	retf
	push	bp
	mov	bp, sp
	sub	sp, 4
	sub	ax, ax
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p40b50
	add	sp, 4
	mov	word ptr [bp - 4], ax
	cmp word ptr [pic_handle],-01h
	jne	p40c94
	mov	ax, 0ffffh
	jmp short	p40c97
p40c94:
	mov ax,[pic_state]
p40c97:
	mov	word ptr [bp - 2], ax
	lea	ax, [bp - 2]
	push	ax
	push	word ptr [bp - 2]
	push	word ptr [bp + 0ah]
	push	word ptr [bp + 8]
	push	word ptr [bp - 4]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	je	p40cc0
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p40cc0:
	push	word ptr [bp - 4]
	push	cs
	call	p40bf2
	mov	ax, word ptr [bp - 2]
	mov	sp, bp
	pop	bp
	retf
	push	bp
	mov	bp, sp
	sub	sp, 2
	push	word ptr [bp + 6]
	push	cs
	call	p40bc4
	add	sp, 2
	mov	word ptr [bp - 2], ax
	lea	ax, [bp + 0ch]
	push	ax
	push	word ptr [bp + 0ch]
	push	word ptr [bp + 0ah]
	push	word ptr [bp + 8]
	push	word ptr [bp - 2]
	call far ptr __dos_write
	add	sp, 0ah
	or	ax, ax
	je	p40d07
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p40d07:
	push	word ptr [bp - 2]
	push	cs
	call	p40bf2
	mov	ax, word ptr [bp + 0ch]
	mov	sp, bp
	pop	bp
	retf
	nop
p40d16:
	push	bp
	mov	bp, sp
	sub	sp, 4
	push	di
	push	si
	sub	ax, ax
	push	ax
	push	word ptr [bp + 0ch]
	push	cs
	call	p40b50
	add	sp, 4
	mov	word ptr [bp - 4], ax
	push	ax
	push	cs
	call	p41154
	add	sp, 2
	push	word ptr [bp + 0eh]
	call far ptr g6d88
	add	sp, 2
	cmp	word ptr [bp + 6], 0
	jge	p40d4d
	mov word ptr [pic_state+22],00h
p40d4d:
	sub	si, si
	mov	di, word ptr [bp + 6]
	jmp short	p40d7b
p40d54:
	mov	ax, 0ca5eh
	push	ax
	call far ptr g6f02
	add	sp, 2
	push word ptr [pic_state+20]
	mov	ax, word ptr [bp + 0ah]
	add	ax, si
	push	ax
	push	word ptr [bp + 8]
	push	di
	mov	ax, 0ca5eh
	push	ax
	call far ptr g60a6
	add	sp, 0ah
	inc	si
p40d7b:
	cmp si,[pic_state+22]
	jl	p40d54
	mov	word ptr [bp - 2], si
	push	word ptr [bp - 4]
	push	cs
	call	p40bf2
	add	sp, 2
	pop	si
	pop	di
	mov	sp, bp
	pop	bp
	retf
	PUBLIC	_LoadPicture
_LoadPicture	LABEL FAR
	push	bp
	mov	bp, sp
	mov	ax, 1
	push	ax
	push	word ptr [bp + 8]
	sub	ax, ax
	push	ax
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p40d16
	add	sp, 0ah
	pop	bp
	retf
	PUBLIC	_LoadPicToPal
_LoadPicToPal	LABEL FAR
	push	bp
	mov	bp, sp
	push	word ptr [bp + 0ah]
	push	word ptr [bp + 8]
	sub	ax, ax
	push	ax
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p40d16
	add	sp, 0ah
	pop	bp
	retf
	nop
	PUBLIC	_LoadPicNoPal
_LoadPicNoPal	LABEL FAR
	push	bp
	mov	bp, sp
	sub	ax, ax
	push	ax
	push	word ptr [bp + 8]
	push	ax
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p40d16
	add	sp, 0ah
	pop	bp
	retf
	nop
	PUBLIC	_LoadPalNoPic
_LoadPalNoPic	LABEL FAR
	push	bp
	mov	bp, sp
	mov	ax, 1
	push	ax
	push	word ptr [bp + 6]
	sub	ax, ax
	push	ax
	push	ax
	mov	ax, 0ffffh
	push	ax
	push	cs
	call	p40d16
	add	sp, 0ah
	pop	bp
	retf
	nop
	PUBLIC	_LoadPalToPal
_LoadPalToPal	LABEL FAR
	push	bp
	mov	bp, sp
	push	word ptr [bp + 8]
	push	word ptr [bp + 6]
	sub	ax, ax
	push	ax
	push	ax
	mov	ax, 0ffffh
	push	ax
	push	cs
	call	p40d16
	add	sp, 0ah
	pop	bp
	retf
	push	bp
	mov	bp, sp
	sub	sp, 2
	sub	ax, ax
	push	ax
	push	word ptr [bp + 0ah]
	push	cs
	call	p40b50
	add	sp, 4
	mov	word ptr [bp - 2], ax
	push	ax
	push	cs
	call	p41154
	add	sp, 2
	call far ptr g6ea0
	push word ptr [pic_state+24]
	push	word ptr [bp + 8]
	push	word ptr [bp + 6]
	call far ptr g6ee8
	add	sp, 6
	push	word ptr [bp - 2]
	push	cs
	call	p40bf2
	mov ax,[pic_state+24]
	mov	sp, bp
	pop	bp
	retf
	nop
	push	bp
	mov	bp, sp
	sub	sp, 0eh
	lea	ax, [bp - 0eh]
	push	ax
	sub	ax, ax
	push	ax
	push	word ptr [bp + 0ah]
	call far ptr __dos_open
	add	sp, 6
	or	ax, ax
	je	p40e7e
p40e76:
	mov ax,[_errno]
	mov	sp, bp
	pop	bp
	retf
	nop
p40e7e:
	lea	ax, [bp - 0ch]
	push	ax
	mov	ax, 2
	push	ax
	lea	ax, [bp - 2]
	push	ss
	push	ax
	push	word ptr [bp - 0eh]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	jne	p40e76
	mov word ptr [_errno],02h
	mov	ax, word ptr [bp - 2]
	cmp	word ptr [bp + 8], ax
	jbe	p40eb0
	mov	ax, 2
	mov	sp, bp
	pop	bp
	retf
	nop
p40eb0:
	mov	ax, word ptr [bp + 8]
	shl	ax, 1
	cwd
	push	dx
	push	ax
	push	word ptr [bp - 0eh]
	push	cs
	call	p413cc
	add	sp, 6
	or	ax, ax
	jne	p40e76
	lea	ax, [bp - 0ch]
	push	ax
	mov	ax, 2
	push	ax
	lea	ax, [bp - 2]
	push	ss
	push	ax
	push	word ptr [bp - 0eh]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	jne	p40e76
	sub	word ptr [bp - 2], 8
	sub	ax, ax
	push	ax
	push	word ptr [bp - 2]
	push	word ptr [bp - 0eh]
	push	cs
	call	p413cc
	add	sp, 6
	or	ax, ax
	je	p40efd
	jmp	p40e76
p40efd:
	lea	ax, [bp - 0ch]
	push	ax
	mov	ax, 8
	push	ax
	mov	ax, 061fch
	push	ds
	push	ax
	push	word ptr [bp - 0eh]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	je	p40f1c
	jmp	p40e76
p40f1c:
	mov al,[pic_state+5]
	sub	ah, ah
	mov cl,[pic_state+4]
	sub	ch, ch
	sub	ax, cx
	inc	ax
	mov	word ptr [bp - 4], ax
	mov al,[pic_state+6]
	sub	ah, ah
	mov	cx, ax
	mov al,[pic_state+8]
	mov dl,[pic_state+10]
	sub	dh, dh
	add	ax, dx
	mul	word ptr [bp - 4]
	mul	cx
	add	ax, 8
	mov	word ptr [bp - 0ah], ax
	push	word ptr [bp + 6]
	call far ptr g606e
	add	sp, 2
	sub	ax, 8
	mov	word ptr [bp - 8], ax
	mov	word ptr [bp - 6], dx
	cmp byte ptr [pic_state+7],00h
	jne	p40f71
	mov	ax, word ptr [bp - 4]
	sub	word ptr [bp - 8], ax
	sub	word ptr [bp - 2], ax
	add	word ptr [bp - 0ah], ax
p40f71:
	sub	ax, ax
	push	ax
	push	word ptr [bp - 2]
	push	word ptr [bp - 0eh]
	push	cs
	call	p413cc
	add	sp, 6
	or	ax, ax
	je	p40f88
	jmp	p40e76
p40f88:
	lea	ax, [bp - 0ch]
	push	ax
	push	word ptr [bp - 0ah]
	push	word ptr [bp - 6]
	push	word ptr [bp - 8]
	push	word ptr [bp - 0eh]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	je	p40fa7
	jmp	p40e76
p40fa7:
	push	word ptr [bp - 0eh]
	call far ptr __dos_close
	add	sp, 2
	or	ax, ax
	je	p40fb9
	jmp	p40e76
p40fb9:
	sub	ax, ax
	mov	sp, bp
	pop	bp
	retf
	nop
	push	bp
	mov	bp, sp
	sub	sp, 4
	push	si
	sub	ax, ax
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p40b50
	add	sp, 4
	mov	word ptr [bp - 4], ax
	push	ax
	push	cs
	call	p41154
	add	sp, 2
	sub	ax, ax
	push	ax
	call far ptr g6d88
	add	sp, 2
	push word ptr [pic_state+22]
	push word ptr [pic_state+20]
	call far ptr g6116
	add	sp, 4
	sub	si, si
	jmp short	p41017
p40ffe:
	mov	ax, 0ca5eh
	push	ax
	call far ptr g6f02
	add	sp, 2
	mov	ax, 0ca5eh
	push	ax
	call far ptr g610f
	add	sp, 2
	inc	si
p41017:
	cmp si,[pic_state+22]
	jl	p40ffe
	mov	word ptr [bp - 2], si
	push	word ptr [bp - 4]
	push	cs
	call	p40bf2
	add	sp, 2
	call far ptr g6108
	pop	si
	mov	sp, bp
	pop	bp
	retf
	PUBLIC	_LoadPicGmem
_LoadPicGmem	LABEL FAR
	push	bp
	mov	bp, sp
	push	word ptr [bp + 6]
	push	word ptr [bp + 8]
	push	cs
	call near ptr	_LoadPicture
	add	sp, 4
	pop	bp
	retf
	push	bp
	mov	bp, sp
	sub	sp, 4
	push	di
	push	si
	sub	ax, ax
	push	ax
	push	word ptr [bp + 6]
	push	cs
	call	p40b50
	add	sp, 4
	mov	word ptr [bp - 4], ax
	push	ax
	push	cs
	call	p41154
	add	sp, 2
	sub	ax, ax
	push	ax
	call far ptr g6d88
	add	sp, 2
	sub	si, si
	mov	di, word ptr [bp + 8]
	jmp short	p4109a
p41078:
	mov	ax, 0ca5eh
	push	ax
	call far ptr g6f02
	add	sp, 2
	push word ptr [pic_state+20]
	push	si
	sub	ax, ax
	push	ax
	push	di
	mov	ax, 0ca5eh
	push	ax
	call far ptr g60a6
	add	sp, 0ah
	inc	si
p4109a:
	cmp si,[pic_state+22]
	jl	p41078
	mov	word ptr [bp - 2], si
	pop	si
	pop	di
	mov	sp, bp
	pop	bp
	retf
	nop
	push	bp
	mov	bp, sp
	sub	sp, 2
	push	di
	push	si
	mov word ptr [pic_state+14],00h
	mov	ax, word ptr [bp + 6]
	mov byte ptr [pic_state+16],ax
	mov ax,[pic_bound]
	mov [pic_src],ax
	mov word ptr [pic_pump],offset p41172
	mov word ptr [pic_pump+2],seg p40b50
	sub	ax, ax
	push	ax
	call far ptr g6d88
	add	sp, 2
	sub	si, si
	mov	di, word ptr [bp + 8]
	jmp short	p41104
p410e2:
	mov	ax, 0ca5eh
	push	ax
	call far ptr g6f02
	add	sp, 2
	push word ptr [pic_state+20]
	push	si
	sub	ax, ax
	push	ax
	push	di
	mov	ax, 0ca5eh
	push	ax
	call far ptr g60a6
	add	sp, 0ah
	inc	si
p41104:
	cmp si,[pic_state+22]
	jl	p410e2
	mov	word ptr [bp - 2], si
	pop	si
	pop	di
	mov	sp, bp
	pop	bp
	retf
	nop
	push	bp
	mov	bp, sp
	sub	sp, 2
	mov	ax, word ptr [bp + 6]
	mov byte ptr [pic_state+14],ax
	mov	ax, word ptr [bp + 0ch]
	mov byte ptr [pic_state+16],ax
	mov ax,[pic_bound]
	mov [pic_src],ax
	mov word ptr [pic_pump],offset p41172
	mov word ptr [pic_pump+2],seg p40b50
	call far ptr g6ea0
	push word ptr [pic_state+24]
	push	word ptr [bp + 0ch]
	push	word ptr [bp + 0ah]
	call far ptr g6ee8
	mov ax,[pic_state+24]
	mov	sp, bp
	pop	bp
	retf
	nop
p41154:
	push	bp
	mov	bp, sp
	mov	ax, word ptr [bp + 6]
	mov byte ptr [pic_state+12],ax
	mov ax,[pic_bound]
	mov [pic_src],ax
	mov word ptr [pic_pump],offset p411b8
	mov word ptr [pic_pump+2],seg p40b50
	pop	bp
	retf
	nop
p41172:
	push	bp
	mov	bp, sp
	sub	sp, 4
	mov ax,offset pic_cbuf
	mov	word ptr [bp - 4], ax
	mov	word ptr [bp - 2], ds
	mov	ax, 0200h
	push	ax
	push	word ptr [bp - 4]
	push	ds
	push word ptr [pic_state+14]
	push word ptr [pic_state+16]
	call far ptr _movedata
	mov word ptr [pic_src],offset pic_cbuf
	add byte ptr [pic_state+15],02h
	mov	ax, 0200h
	mov	sp, bp
	pop	bp
	retf
	push	bp
	mov	bp, sp
	mov	ax, word ptr [bp + 6]
	mov byte ptr [pic_state+12],ax
	push	cs
	call	p411b8
	pop	bp
	retf
	nop
p411b8:
	push	bp
	mov	bp, sp
	sub	sp, 2
	lea	ax, [bp - 2]
	push	ax
	mov	ax, 0200h
	push	ax
	mov ax,offset pic_cbuf
	push	ds
	push	ax
	push word ptr [pic_state+12]
	call far ptr __dos_read
	mov word ptr [pic_src],offset pic_cbuf
	mov	ax, word ptr [bp - 2]
	mov	sp, bp
	pop	bp
	retf
	nop
p411e2:
	push	bp
	mov	bp, sp
	mov	ax, 3
	push	ax
	push	cs
	call	p4120a
	add	sp, 2
	push	word ptr [bp + 6]
	call far ptr l_perror
	add	sp, 2
	mov	ax, 063h
	push	ax
	call far ptr _exit
	add	sp, 2
	pop	bp
	retf
	nop
p4120a:
	push	bp
	mov	bp, sp
	sub	sp, 010h
	mov ax,[_errno]
	mov	word ptr [bp - 010h], ax
	mov	byte ptr [bp - 0dh], 0
	mov	al, byte ptr [bp + 6]
	mov	byte ptr [bp - 0eh], al
	lea	ax, [bp - 0eh]
	push	ax
	lea	ax, [bp - 0eh]
	push	ax
	mov	ax, 010h
	push	ax
	call far ptr _int86x
	mov	ax, word ptr [bp - 010h]
	mov [_errno],ax
	mov	sp, bp
	pop	bp
	retf
	nop
	push	bp
	mov	bp, sp
	sub	sp, 2
	lea	ax, [bp - 2]
	push	ax
	sub	ax, ax
	push	ax
	push	word ptr [bp + 6]
	call far ptr __dos_open
	add	sp, 6
	or	ax, ax
	je	p4125e
	sub	ax, ax
	mov	sp, bp
	pop	bp
	retf
p4125e:
	mov	ax, word ptr [bp - 2]
	mov [pic_handle],ax
	mov word ptr [pic_state+18],00h
	mov	ax, 1
	mov	sp, bp
	pop	bp
	retf
	nop
	cmp	word ptr ds:[05736h], -1
	je	p41299
	push	word ptr ds:[05736h]
	db	09ah,02ch,00dh,040h,041h	; far call 4140:0d2c (external)
	add	sp, 2
	or	ax, ax
	je	p41293
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p41293:
	mov	word ptr ds:[05736h], 0ffffh
p41299:
	retf
p4129a:
	push	bp
	mov	bp, sp
	sub	sp, 8
	push	di
	push	si
	cmp word ptr [pic_handle],-01h
	jne	p412b2
p412a9:
	sub	ax, ax
	pop	si
	pop	di
	mov	sp, bp
	pop	bp
	retf
	nop
p412b2:
	sub	ax, ax
	push	ax
	push	ax
	push word ptr [pic_handle]
	push	cs
	call	p413cc
	add	sp, 6
	or	ax, ax
	je	p412cf
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p412cf:
	lea	ax, [bp - 2]
	push	ax
	mov	ax, 2
	push	ax
	lea	ax, [bp - 4]
	push	ss
	push	ax
	push word ptr [pic_handle]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	je	p412f6
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p412f6:
	cmp	word ptr [bp + 6], 0
	jne	p4135a
	inc word ptr [pic_state+18]
	mov ax,[pic_state+18]
	cmp	word ptr [bp - 4], ax
	jl	p412a9
	mov	word ptr [bp - 4], ax
	dec	word ptr [bp - 4]
	or	ax, ax
	je	p41350
	mov	ax, word ptr [bp + 8]
	mov	word ptr [bp - 8], ax
	mov	word ptr [bp - 6], ds
	mov	si, word ptr [bp - 4]
p4131e:
	lea	ax, [bp - 2]
	push	ax
	mov	ax, 018h
	push	ax
	push	word ptr [bp - 6]
	push	word ptr [bp - 8]
	push word ptr [pic_handle]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	je	p41346
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p41346:
	mov	ax, si
	dec	si
	or	ax, ax
	jne	p4131e
	mov	word ptr [bp - 4], si
p41350:
	mov	ax, 1
	pop	si
	pop	di
	mov	sp, bp
	pop	bp
	retf
	nop
p4135a:
	mov	ax, word ptr [bp - 4]
	dec	word ptr [bp - 4]
	or	ax, ax
	jne	p41367
	jmp	p412a9
p41367:
	mov	ax, word ptr [bp + 8]
	mov	word ptr [bp - 8], ax
	mov	word ptr [bp - 6], ds
	mov	di, ax
	mov	si, word ptr [bp - 4]
p41375:
	lea	ax, [bp - 2]
	push	ax
	mov	ax, 018h
	push	ax
	push	word ptr [bp - 6]
	push	word ptr [bp - 8]
	push word ptr [pic_handle]
	call far ptr __dos_read
	add	sp, 0ah
	or	ax, ax
	je	p4139d
	sub	ax, ax
	push	ax
	push	cs
	call	p411e2
	add	sp, 2
p4139d:
	mov	ax, 0ch
	push	ax
	push	word ptr [bp + 6]
	push	di
	call far ptr l_strnicmp
	add	sp, 6
	or	ax, ax
	jne	p413be
	mov	ax, 1
	mov	word ptr [bp - 4], si
	pop	si
	pop	di
	mov	sp, bp
	pop	bp
	retf
	nop
p413be:
	mov	ax, si
	dec	si
	or	ax, ax
	jne	p41375
	mov	word ptr [bp - 4], si
	jmp	p412a9
	nop
p413cc:
	push	bp
	mov	bp, sp
	sub	sp, 0eh
	mov	word ptr [bp - 0eh], 04200h
	mov	ax, word ptr [bp + 6]
	mov	word ptr [bp - 0ch], ax
	mov	ax, word ptr [bp + 0ah]
	mov	word ptr [bp - 0ah], ax
	mov	ax, word ptr [bp + 8]
	mov	word ptr [bp - 8], ax
	lea	ax, [bp - 0eh]
	push	ax
	lea	ax, [bp - 0eh]
	push	ax
	call far ptr l_intdosx
	add	sp, 4
	cmp	word ptr [bp - 2], 0
	db	075h,007h	; jne p41406 (unresolved)
	db	02bh		; truncated sub opcode at segment end (runtime-patched cell)
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
	push	bp
	mov	bp, sp
	push	di
	push	si
	mov	si, word ptr [bp + 6]
	mov	di, word ptr [bp + 8]
	push	ds
	pop	es
	mov	cx, word ptr [bp + 0ah]
	jcxz	l_sn_done
	mov	bh, 041h
	mov	bl, 05ah
	mov	dh, 020h
l_sn_loop:
	mov	ah, byte ptr [si]
	mov	al, byte ptr [di]
	or	ah, ah
	je	l_sn_end
	or	al, al
	je	l_sn_end
	inc	si
	inc	di
	cmp	ah, bh
	jb	l_sn_nofold1
	cmp	ah, bl
	ja	l_sn_nofold1
	add	ah, dh
l_sn_nofold1:
	cmp	al, bh
	jb	l_sn_nofold2
	cmp	al, bl
	ja	l_sn_nofold2
	add	al, dh
l_sn_nofold2:
	cmp	ah, al
	jne	l_sn_diff
	loop	l_sn_loop
l_sn_end:
	xor	cx, cx
	cmp	ah, al
	je	l_sn_done
l_sn_diff:
	mov	cx, 0
	jb	l_sn_neg
	dec	cx
	dec	cx
l_sn_neg:
	not	cx
l_sn_done:
	mov	ax, cx
	pop	si
	pop	di
	mov	sp, bp
	pop	bp
	retf
l_intdosx	LABEL FAR
	PUBLIC	_intdosx
_intdosx	LABEL FAR
	push	bp
	mov	bp, sp
	push	si
	push	di
	mov	di, word ptr [bp + 6]
	mov	ax, word ptr [di]
	mov	bx, word ptr [di + 2]
	mov	cx, word ptr [di + 4]
	mov	dx, word ptr [di + 6]
	mov	si, word ptr [di + 8]
	mov	di, word ptr [di + 0ah]
	int	021h
	push	di
	mov	di, word ptr [bp + 8]
	mov	word ptr [di], ax
	mov	word ptr [di + 2], bx
	mov	word ptr [di + 4], cx
	mov	word ptr [di + 6], dx
	mov	word ptr [di + 8], si
	pop	word ptr [di + 0ah]
	jb	l_id_err
	xor	si, si
	jmp	short l_id_mid
l_id_err:
	mov	si, 1
l_id_mid EQU l_id_err + 2
l_id_out:
	mov	word ptr [di + 0ch], si
	pop	di
	pop	si
	mov	sp, bp
	pop	bp
	retf
AUX2_TEXT ENDS
EXTRN _strlen:FAR,_write:FAR
END
