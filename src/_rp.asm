        .MODEL MEDIUM

	public	SetOvrlyRp, FixEGA
.DATA	
	EXTRN ClipWd:WORD
	EXTRN ClipLn:WORD
	EXTRN XBgn:WORD
	EXTRN YBgn:WORD

        .CODE

SetOvrlyRp      PROC far
        retf
SetOvrlyRp      ENDP

FixEGA      PROC far
        retf
FixEGA      ENDP
        END



