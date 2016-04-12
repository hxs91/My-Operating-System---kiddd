/*
Add interrupt surpport
*/
#include "const.h"
#include "type.h"
#include "global.h"
#include "screen.h"

/* 中断处理函数 */
extern void	divide_error();
extern void	single_step_exception();
extern void	nmi();
extern void	breakpoint_exception();
extern void	overflow();
extern void	bounds_check();
extern void	inval_opcode();
extern void	copr_not_available();
extern void	double_fault();
extern void	copr_seg_overrun();
extern void	inval_tss();
extern void	segment_not_present();
extern void	stack_exception();
extern void	general_protection();
extern void	page_fault();
extern void	copr_error();
extern void clock_handler_invoker();
extern void keyboard_interrupt_invoker();
extern void enable_interrupt();

PUBLIC void exception_handler(int vec_no, int err_code, int eip, int cs, int eflag) {
	print("\nexception vector number:");
	print_hex(vec_no);
	print(", and the error code:");
	print_hex(err_code);
	print("\n");
}

PRIVATE void init_idt_desc(u8 vector, u8 desc_type, int_handler handler, u8 privilege) {
	GATE * 	p_gate = &idt[vector];
	u32 	base = (u32)handler;
	p_gate -> offset_low = base & 0xFFFF;
	p_gate -> selector 	 = SELECTOR_CODE_SEGMENT;
	p_gate -> dcount = 0;
	p_gate -> attr = desc_type | (privilege << 5);
	p_gate -> offset_high = (base >> 16) & 0xFFFF;
}

/*
clock interrupt handler
*/
PUBLIC void clock_interrupt() {
	//print("I will refresh per second.\n");
	u8 tmp = *((u8 *) VIDEO_ADDRESS);
	tmp ++;
	*((u8 *) VIDEO_ADDRESS) = tmp;
	port_byte_out(INT_M_CTL, EOI);
}

/*
keyboard interrupt handler
*/
PUBLIC void keyboard_interrupt() {
	u8 scan_code = port_byte_in(KEYBOARD_INPUT_BUFFER);
	print_hex(scan_code);
	port_byte_out(INT_M_CTL, EOI);
}

/*======================================================================*
                            init_prot
 *======================================================================*/
PUBLIC void init_prot()
{
	init_8259A();

	// 全部初始化成中断门(没有陷阱门)
	init_idt_desc(INT_VECTOR_DIVIDE,	DA_386IGate,
		      divide_error,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_DEBUG,		DA_386IGate,
		      single_step_exception,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_NMI,		DA_386IGate,
		      nmi,			PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_BREAKPOINT,	DA_386IGate,
		      breakpoint_exception,	PRIVILEGE_USER);

	init_idt_desc(INT_VECTOR_OVERFLOW,	DA_386IGate,
		      overflow,			PRIVILEGE_USER);

	init_idt_desc(INT_VECTOR_BOUNDS,	DA_386IGate,
		      bounds_check,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_INVAL_OP,	DA_386IGate,
		      inval_opcode,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_NOT,	DA_386IGate,
		      copr_not_available,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_DOUBLE_FAULT,	DA_386IGate,
		      double_fault,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_SEG,	DA_386IGate,
		      copr_seg_overrun,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_INVAL_TSS,	DA_386IGate,
		      inval_tss,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_SEG_NOT,	DA_386IGate,
		      segment_not_present,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_STACK_FAULT,	DA_386IGate,
		      stack_exception,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_PROTECTION,	DA_386IGate,
		      general_protection,	PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_PAGE_FAULT,	DA_386IGate,
		      page_fault,		PRIVILEGE_KRNL);

	init_idt_desc(INT_VECTOR_COPROC_ERR,	DA_386IGate,
		      copr_error,		PRIVILEGE_KRNL);

	//clock
	init_idt_desc(INT_VECTOR_IRQ0, DA_386IGate,
			 clock_handler_invoker, PRIVILEGE_KRNL);

	//keyboard
	init_idt_desc(INT_VECTOR_IRQ0 + 1, DA_386IGate,
			 keyboard_interrupt_invoker, PRIVILEGE_KRNL);
}

