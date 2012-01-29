;-----------------------------------------------------------------------------
; Second sector
;-----------------------------------------------------------------------------

	org	0x0400

idtr:	dw	1023;idt_end-idt-1	; Length of the idt
	dd	idt		; physical address of idt
idt:	dw	0;timer
	dw	8		; selector
	dw	0x8E00
	dw	0

times 248	db 0

int_clock:	dw	0;timer
	dw	8
	dw	0x8E00
	dw	0

	dw	0;timer
	dw	8
	dw	0x8E00
	dw	0
idt_end:

bits 32
;----------------------------------------------------------------------------
;timerc	db	0
;----------------------------------------------------------------------------
;printMsg:
;	push	ecx
;	mov	cl, byte [timerc]
;	cmp	cl, 0
;	jne	dispB
;	mov	word [gs:0], 0x0920
;	mov	word [gs:2], 0x0941
;	jmp	noz
;dispB:	mov	word [gs:0], 0x0941
;	mov	word [gs:2], 0x0920
;
;noz:	xor	cl, 1
;	mov	byte [timerc], cl
;	pop	ecx
;	ret
;----------------------------------------------------------------------------
;timer:	cli
;	call	printMsg
;	mov	al, 0x20
;	out	0x20, al
;	sti
;	iret
;----------------------------------------------------------------------------
times 512-($-$$)	db 0
