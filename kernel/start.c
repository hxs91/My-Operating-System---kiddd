#include "screen.h"
#include "type.h"
#include "global.h"
#include "const.h"

extern void load_idt_ptr();
extern void init_prot();
extern void enable_interrupt();
extern void init_descriptor_of_tss_and_ldt();

/* initialize the setting of interrupt */
void init_interrupt() {
	print("-----Initialize the interrupt.-----\n");
	
	//initialize the value in idt_ptr
	//idt_ptr is declared in globalh and defined in interrupt.c
	u16 * p_idt_limit = (u16 *)(&idt_ptr[0]);
	u32 * p_idt_base = (u32 *)(&idt_ptr[2]);
	*p_idt_limit = IDT_SIZE * sizeof(GATE) - 1;
	*p_idt_base = (u32)idt;	//TODO: this line of code is different from the book

	//this function is defined in interrupt_func.asm
	load_idt_ptr();

	// building the mapping between interrupt and its handler
	init_prot();

	//open the response of interrup
	enable_interrupt();

	print("-----interrupt initialize end.-----\n");
}

/* all initialize */
void system_init() {
	init_interrupt();
	init_descriptor_of_tss_and_ldt();
}

PUBLIC void cstart() {
	clear_screen();
	print("-----cstart-----\n");

	memory_copy((void *)(*((u32*)(&gdt_ptr[2]))),   // Base  of Old GDT
				&gdt,				    // New GDT
				*((u16*)(&gdt_ptr[0])) + 1	    // Limit of Old GDT
				);
	// gdt_ptr[6] 共 6 个字节：0~15:Limit  16~47:Base。用作 sgdt 以及 lgdt 的参数。
	u16* p_gdt_limit = (u16*)(&gdt_ptr[0]);
	u32* p_gdt_base  = (u32*)(&gdt_ptr[2]);
	*p_gdt_limit = GDT_SIZE * sizeof(DESCRIPTOR) - 1;
	*p_gdt_base  = (u32)(&gdt);

	system_init();
	
	print("-----cstart ends.-----\n");
}