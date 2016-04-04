#ifndef INCLUDE_CONST_H
#define INCLUDE_CONST_H

/* SIZE of IDT */
#define IDT_SIZE 256

/* 8295 interrupt controller ports */
#define INT_M_CTL		0x20
#define INT_M_CTLMASK	0x21
#define INT_S_CTL		0xA0
#define INT_S_CTLMASK	0xA1

/* interrupt vector */
#define INT_VECTOR_IRQ0		0x20
#define INT_VECTOR_IRQ8		0x28

/* types of function */
#define PUBLIC
#define PRIVATE	static

/* interrupt vector */
#define	INT_VECTOR_DIVIDE		0x0
#define	INT_VECTOR_DEBUG		0x1
#define	INT_VECTOR_NMI			0x2
#define	INT_VECTOR_BREAKPOINT		0x3
#define	INT_VECTOR_OVERFLOW		0x4
#define	INT_VECTOR_BOUNDS		0x5
#define	INT_VECTOR_INVAL_OP		0x6
#define	INT_VECTOR_COPROC_NOT		0x7
#define	INT_VECTOR_DOUBLE_FAULT		0x8
#define	INT_VECTOR_COPROC_SEG		0x9
#define	INT_VECTOR_INVAL_TSS		0xA
#define	INT_VECTOR_SEG_NOT		0xB
#define	INT_VECTOR_STACK_FAULT		0xC
#define	INT_VECTOR_PROTECTION		0xD
#define	INT_VECTOR_PAGE_FAULT		0xE
#define	INT_VECTOR_COPROC_ERR		0x10

/* selector */
#define SELECTOR_CODE_SEGMENT	0x08 //this is the location of the code segment in the memory

/*Gate descriptor*/
#define DA_386IGate 0x8E	

/* privilege definition */
#define	PRIVILEGE_KRNL	0
#define	PRIVILEGE_TASK	1
#define	PRIVILEGE_USER	3


/* keyboard */
#define KEYBOARD_INPUT_BUFFER 0x60

#endif