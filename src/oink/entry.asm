[bits 32]
[global start]
[global taskJump]
[global taskJumpFirst]
[extern tssSize]
[global currentTask]
[global ticks]
[extern tasks]
[extern task0]
[extern task1]
[extern taskCount]

[extern main]

section .text

start:	cli
	call	main

section .text

ticks			dd 0
jmpParam		dd 0
currentTask			dd 0
			db 0

taskJump:	pop ebx
	pop eax		; eax holds the selector number
	push ebx

;	mov ebx, [_tssSize]
;	mov ebp, esp
;	mov ecx, eax
;	shr eax, 3
;	sub eax, 3
;	mul ebx
;	mov esp, dword [_tasks + eax + 4]
;	mov eax, ecx

	shl eax, 16
	mov dword [jmpParam], eax
	mov ebx, jmpParam
	mov al, 0x20
	out 0x20, al
	mov al, 0x20
	out 0xA0, al
	sti
	jmp word far [ebx]
	cli

;	mov esp, ebp
	leave
	ret

taskJumpFirst: mov [initStack], esp
	mov esp, [tasks + 4]

	call task0

	ret

initStack dd 0
