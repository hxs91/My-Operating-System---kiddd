[bits 32]

%include "sconst.inc"

global _start
global restart
global StackTop

; import function
extern cstart
extern main
extern gdt_ptr
extern p_proc_ready
extern tss

[SECTION .bss]
StackSpace	resb	2*1024	;define the size of stack ;kernel stack
StackTop:

[section .text]	; the location of code

_start:
	mov esp, StackTop	

	sgdt	[gdt_ptr]	;store the value of gdt ptr
	call	cstart		;modify the value of dgt ptr
	lgdt	[gdt_ptr]	;use the modified dgt_ptr

	jmp SELECTOR_KERNEL_CS:csinit	;this jmp cal use the new gdt ptr

csinit:
	xor	eax, eax
	mov	ax, SELECTOR_TSS
	ltr	ax
	
	call main
	jmp $

restart:
	mov	esp, [p_proc_ready]
	lldt	[esp + P_LDT_SEL]
	lea	eax, [esp + P_STACKTOP]
	mov	dword [tss + TSS3_S_SP0], eax

	pop	gs
	pop	fs
	pop	es
	pop	ds
	popad
	add	esp, 4
	iretd