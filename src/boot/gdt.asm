	org	0x10000

bits 16

gdtr:

	dw	8192	; Length of the gdt
	dd	gdt		; physical address of gdt

gdt:

nullsel	equ	$-gdt	; $->current location,so nullsel = 0h

gdt0:			; Null descriptor,as per convention gdt0 is 0

	dd	0	; Each gdt entry is 8 bytes, so at 08h it is CS
	dd	0	; In all the segment descriptor is 64 bits

codesel	equ	$-gdt		; This is 8h,ie 2nd descriptor in gdt

code_gdt:			; Code descriptor 4Gb flat segment at 0000:0000h

	dw	0x0ffff	; Limit 4Gb  bits 0-15 of segment descriptor
	dw	0x0000	; Base 0h bits 16-31 of segment descriptor (sd)
	db	0x00	; Base addr of seg 16-23 of 32bit addr,32-39 of sd
	db	0x09a	; P,DPL(2),S,TYPE(3),A->Present bit 1,Descriptor
			; privilege level 0-3,Segment descriptor 1 ie code
			; or data seg descriptor,Type of seg,Accessed bit
	db	0x0cf	; Upper 4 bits G,D,0,AVL ->1 segment len is page
			; granular, 1 default operation size is 32bit seg
			; AVL : Available field for user or OS
			; Lower nibble bits 16-19 of segment limit
	db	0x00	; Base addr of seg 24-31 of 32bit addr,56-63 of sd

datasel	equ	$-gdt	; ie 10h, beginning of next 8 bytes for data sd

data_gdt:			; Data descriptor 4Gb flat seg at 0000:0000h
	dw	0x0ffff	; Limit 4Gb
	dw	0x0000	; Base 0000:0000h
	db	0x00	; Descriptor format same as above
	db	0x092
	db	0x0cf
	db	0x00

gdt_end:

times 512-($-$$)	db 0
