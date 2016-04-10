#include "type.h"
#include "const.h"
#include "proc.h"
#include "screen.h"

PUBLIC u8 		idt_ptr[6];		// idt descriptor, 0-15:limit, 16-47:base
PUBLIC GATE	idt[IDT_SIZE];	// the whole idt.

PUBLIC u8 gdt_ptr[6];
PUBLIC DESCRIPTOR	gdt[GDT_SIZE];

PUBLIC PROCESS proc_table[NR_TASKS];

PUBLIC	char	task_stack[STACK_SIZE_TOTAL];
PUBLIC TSS tss;

PUBLIC PROCESS*		p_proc_ready;