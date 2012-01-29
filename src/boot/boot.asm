
;-----------------------------------------------------------------------------
; First sector
;-----------------------------------------------------------------------------

	org	0x07c00		; Start address 0000:7c00 
	jmp	short begin_boot	; Jump to start of boot routine & skip other data

signature	db	85h, 06h, 13h, 65h, 80h

begin_boot:

bits 16

	mov	sp, 0x1FD
	;-----------------------------------------------------------------
	call	enableA20
	;-----------------------------------------------------------------
	mov	ax, 0x0600		; Clear screen
	xor	cx, cx
	mov	dx, 0x194F
	mov	bh, 0x0F
	int	0x10
	;-----------------------------------------------------------------
	mov	ah, 0x00		; Init floppy
	mov	dl, 0x00
	int	0x13
	;-----------------------------------------------------------------
	mov	ah, 0x02		; Read second sector - IDT -> 0000:0400
	mov	al, 0x01
	xor	bx, bx
	mov	es, bx
	mov	bx, 0x0400
	mov	ch, 0x00
	mov	cl, 0x02
	mov	dx, 0
	int	0x13
	;-----------------------------------------------------------------
	mov	ah, 0x02		; Read third sector - GDT -> 1000:0000
	mov	al, 0x01
	mov	bx, 0x1000
	mov	es, bx
	mov	bx, 0x0000
	mov	ch, 0x00
	mov	cl, 0x03
	mov	dx, 0
	int	0x13
	;-----------------------------------------------------------------

	; !!!!!!!!!!!

	mov	ax, 0x0003
	mov	bx, 0x0021
	mov	cx, 0x2000
	mov	es, cx
	mov	cx, 0x0000
	call	loadSectors

;	jmp fdc_finish_sec
;
;fdc_again_fir:
;
;	mov	ah, 0x02		; Read 4th-> sector - OINK -> 2000:0000
;	mov	al, 0x0F		; 15 sectors
;	mov	bx, 0x2000		; segment 2000
;	mov	es, bx
;	mov	bx, 0x0000		; offset  0000
;	mov	ch, 0x00		; track   0
;	mov	cl, 0x04		; sector  4
;	mov	dh, 0x00		; side
;	mov	dl, 0x00		; floppy
;	int	0x13
;
;	jnc	fdc_finish_fir
;
;	mov	ah, 0x00		; Init floppy
;	mov	dl, 0x00
;	int	0x13
;
;	jmp	fdc_again_fir
;
;fdc_finish_fir:

	;-----------------------------------------------------------------

;fdc_again_sec:
;
;	mov	ah, 0x02	; Read second track - OINK -> 21E000
;	mov	al, 0x12	; n sectors
;	mov	bx, 0x21E0
;	mov	es, bx
;	mov	bx, 0x0000
;	mov	ch, 0x00	; track
;	mov	cl, 0x01	; sector
;	mov	dh, 0x01	; side
;	mov	dl, 0x00	; floppy
;	int	0x13
;
;	jnc	fdc_finish_sec
;
;	mov	ah, 0x00	; Init floppy
;	mov	dl, 0x00
;	int	0x13
;
;	jmp	fdc_again_sec
;
;fdc_finish_sec:

	;-----------------------------------------------------------------
	; SWITCH TO PROTECTED MODE
	;-----------------------------------------------------------------
	mov	bx, 0x1000
	mov	es, bx
	cli			; Clear or disable interrupts
	es	lgdt	[0x0000]		; Load GDT
	lidt	[0x0400]		; Load IDT
	mov	eax, cr0		; The lsb of cr0 is the protected mode bit
	or	al, 0x01		; Set protected mode bit
	mov	cr0, eax		; Mov modified word to the control register
	jmp	8:go_pm
	;-----------------------------------------------------------------
bits 32
go_pm:
	mov	ax, 0x10
	mov	ds, ax		; Initialise ds & es to data segment
	mov	es, ax
	mov	ss, ax
	mov	esp, 0xFFFF

	; reprogram the pic (intr 0 -> intr 32) --------------------------
	
	mov	al,	0x11
	out	0x20, al
	
	call simpleLoop
	
	mov	al, 0x11
	out	0xA0, al
	
	call simpleLoop
	
	mov	al, 0x20
	out	0x21, al
	
	call simpleLoop
	
	mov	al, 0x28
	out	0xA1, al
	
	call simpleLoop
	
	mov	al, 0x04
	out	0x21, al
	
	call simpleLoop
	
	mov	al, 0x02
	out	0xA1, al
	
	call simpleLoop
	
	mov	al, 0x01
	out	0x21, al
	
	call simpleLoop
	
	mov	al, 0x01
	out	0xA1, al
	
	call simpleLoop
	
	sti

spin:	jmp	0x20000

;-----------------------------------------------------------------------------
enableA20:
	;; This subroutine will enable the A20 address line in the keyboard
	;; controller.  Takes no arguments.  Returns 0 in EAX on success, 
	;; -1 on failure.  Written for use in 16-bit code, see lines marked
	;; with 32-BIT for use in 32-bit code.

	pusha

	;; Make sure interrupts are disabled
	cli

	;; Keep a counter so that we can make up to 5 attempts to turn
	;; on A20 if necessary
	mov CX, 5

	.startAttempt1:		
	;; Wait for the controller to be ready for a command
	.commandWait1:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait1

	;; Tell the controller we want to read the current status.
	;; Send the command D0h: read output port.
	mov AL, 0D0h
	out 64h, AL

	;; Wait for the controller to be ready with a byte of data
	.dataWait1:
	xor AX, AX
	in AL, 64h
	bt AX, 0
	jnc .dataWait1

	;; Read the current port status from port 60h
	xor AX, AX
	in AL, 60h

	;; Save the current value of (E)AX
	;; push AX			; 16-BIT
	push EAX		; 32-BIT

	;; Wait for the controller to be ready for a command
	.commandWait2:
	in AL, 64h
	bt AX, 1
	jc .commandWait2

	;; Tell the controller we want to write the status byte again
	mov AL, 0D1h
	out 64h, AL	

	;; Wait for the controller to be ready for the data
	.commandWait3:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait3

	;; Write the new value to port 60h.  Remember we saved the old
	;; value on the stack
	;; pop AX			; 16-BIT
	pop EAX		; 32-BIT

	;; Turn on the A20 enable bit
	or AL, 00000010b
	out 60h, AL

	;; Finally, we will attempt to read back the A20 status
	;; to ensure it was enabled.

	;; Wait for the controller to be ready for a command
	.commandWait4:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait4

	;; Send the command D0h: read output port.
	mov AL, 0D0h
	out 64h, AL	

	;; Wait for the controller to be ready with a byte of data
	.dataWait2:
	xor AX, AX
	in AL, 64h
	bt AX, 0
	jnc .dataWait2

	;; Read the current port status from port 60h
	xor AX, AX
	in AL, 60h

	;; Is A20 enabled?
	bt AX, 1

	;; Check the result.  If carry is on, A20 is on.
	jc .success

	;; Should we retry the operation?  If the counter value in ECX
	;; has not reached zero, we will retry
	loop .startAttempt1


	;; Well, our initial attempt to set A20 has failed.  Now we will
	;; try a backup method (which is supposedly not supported on many
	;; chipsets, but which seems to be the only method that works on
	;; other chipsets).


	;; Keep a counter so that we can make up to 5 attempts to turn
	;; on A20 if necessary
	mov CX, 5

	.startAttempt2:
	;; Wait for the keyboard to be ready for another command
	.commandWait6:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait6

	;; Tell the controller we want to turn on A20
	mov AL, 0DFh
	out 64h, AL

	;; Again, we will attempt to read back the A20 status
	;; to ensure it was enabled.

	;; Wait for the controller to be ready for a command
	.commandWait7:
	xor AX, AX
	in AL, 64h
	bt AX, 1
	jc .commandWait7

	;; Send the command D0h: read output port.
	mov AL, 0D0h
	out 64h, AL	

	;; Wait for the controller to be ready with a byte of data
	.dataWait3:
	xor AX, AX
	in AL, 64h
	bt AX, 0
	jnc .dataWait3

	;; Read the current port status from port 60h
	xor AX, AX
	in AL, 60h

	;; Is A20 enabled?
	bt AX, 1

	;; Check the result.  If carry is on, A20 is on, but we might warn
	;; that we had to use this alternate method
	jc .warn

	;; Should we retry the operation?  If the counter value in ECX
	;; has not reached zero, we will retry
	loop .startAttempt2


	;; OK, we weren't able to set the A20 address line.  Do you want
	;; to put an error message here?
	jmp .fail


	.warn:
	;; Here you may or may not want to print a warning message about
	;; the fact that we had to use the nonstandard alternate enabling
	;; method

	.success:
	sti
	popa
	xor EAX, EAX
	ret

	.fail:
	sti
	popa
	mov EAX, -1

	ret

;-----------------------------------------------------------------------------
; lba2chs - converts lba sector numbers to cylinder - head - sector values
; Input:
;   ax - sector (lba)
; Output:
;   ax - sector number
;   bx - head number
;   cx - cylinder number
;-----------------------------------------------------------------------------

bits 16

lba2chs:
	push	dx
	xor	dx, dx
	mov	bx, 18	; 18 sectors per track
	div	bx
	inc	dx
	push	dx
	xor	dx, dx
	mov	bx, 2
	div	bx
	mov	cx, ax
	mov	bx, dx
	pop	ax
	pop	dx

	ret

;-----------------------------------------------------------------------------

simpleLoop:
	xor	cx, cx
simpleLoop_loop:
	inc	cx
	cmp	cx, 0xFFFF
	jl simpleLoop_loop
	ret

;-----------------------------------------------------------------------------
; loadSectors - reads sectors from the floppy and outputs them in memory
; Input:
;   ax    - first sector (lba)
;   bx    - last sector (lba)
;   es:cx - address of the buffer
; Output:
;   none
;-----------------------------------------------------------------------------

loadSectors:

	push	dx

loadSectors_loopBegin:

	cmp	ax, bx
	jnl	loadSectors_loopEnd

loadSectors_readBegin:

	push	ax
	push	bx
	push	cx

	call	lba2chs	; ax - s, bx - h, cx - c

	mov	dh, bl	; head
	mov	dl, 0x00	; drive
	mov	ch, cl	; cylinder
	mov	cl, al	; sector
	pop	bx	; offset
	push	bx
	mov	al, 0x01	; sector count
	mov	ah, 0x02	; BIOS ReadSector

	int	0x13

	jnc	loadSectors_readEnd

	mov	ah, 0x00	; BIOS InitFloppy
	mov	dl, 0x00
	int	0x13

	pop	cx
	pop	bx
	pop	ax

	jmp	loadSectors_readBegin

loadSectors_readEnd:

	pop	cx
	pop	bx
	pop	ax

	inc	ax
	add	cx, 0x200

	jmp	loadSectors_loopBegin

loadSectors_loopEnd:

	pop	dx

	ret
	
;-----------------------------------------------------------------------------

times 510-($-$$)	db 0	; Fill bytes from present loc to 510 with 0s
	dw 0x0aa55	; Write aa55 in bytes 511,512 to indicate that
				; it is a bootable sector.

; ... Add second sector ...
	incbin	'bin/idt'

; ... Add third sector ....
	incbin	'bin/gdt'

; ... Add fourth and so on
	incbin	'bin/kernel.img'

times 2880 * 512-($-$$)	db 0
