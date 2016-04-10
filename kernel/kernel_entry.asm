[bits 32]


;const define

; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                               sconst.inc
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
;                                                     Forrest Yu, 2005
; ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

P_STACKBASE	equ	0
GSREG		equ	P_STACKBASE
FSREG		equ	GSREG		+ 4
ESREG		equ	FSREG		+ 4
DSREG		equ	ESREG		+ 4
EDIREG		equ	DSREG		+ 4
ESIREG		equ	EDIREG		+ 4
EBPREG		equ	ESIREG		+ 4
KERNELESPREG	equ	EBPREG		+ 4
EBXREG		equ	KERNELESPREG	+ 4
EDXREG		equ	EBXREG		+ 4
ECXREG		equ	EDXREG		+ 4
EAXREG		equ	ECXREG		+ 4
RETADR		equ	EAXREG		+ 4
EIPREG		equ	RETADR		+ 4
CSREG		equ	EIPREG		+ 4
EFLAGSREG	equ	CSREG		+ 4
ESPREG		equ	EFLAGSREG	+ 4
SSREG		equ	ESPREG		+ 4
P_STACKTOP	equ	SSREG		+ 4
P_LDT_SEL	equ	P_STACKTOP
P_LDT		equ	P_LDT_SEL	+ 4

TSS3_S_SP0	equ	4

INT_M_CTL	equ	0x20	; I/O port for interrupt controller         <Master>
INT_M_CTLMASK	equ	0x21	; setting bits in this port disables ints   <Master>
INT_S_CTL	equ	0xA0	; I/O port for second interrupt controller  <Slave>
INT_S_CTLMASK	equ	0xA1	; setting bits in this port disables ints   <Slave>

EOI		equ	0x20

; 以下选择子值必须与 protect.h 中保持一致!!!
SELECTOR_FLAT_C		equ		0x08		; LOADER 里面已经确定了的.
SELECTOR_TSS		equ		0x18		; TSS. 从外层跳到内存时 SS 和 ESP 的值从里面获得.
SELECTOR_KERNEL_CS	equ		SELECTOR_FLAT_C
;



; import function
extern cstart
extern main
extern gdt_ptr
extern p_proc_ready
extern tss

[SECTION .bss]
StackSpace	resb	2*1024	;define the size of stack
StackTop:

[section .text]	; the location of code

global _start
global restart

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