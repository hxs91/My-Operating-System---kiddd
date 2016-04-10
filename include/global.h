/* global variables */
#include "const.h"
#include "proc.h"

extern u8 			idt_ptr[6];
extern GATE			idt[IDT_SIZE];
extern u8 			gdt_ptr[6];
extern DESCRIPTOR	gdt[GDT_SIZE];

extern PROCESS 		proc_table[NR_TASKS];
extern	char		task_stack[];
extern TSS tss;
extern PROCESS*		p_proc_ready;
