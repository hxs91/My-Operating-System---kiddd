#include "type.h"
#include "const.h"
#include "proc.h"
#include "screen.h"

extern void test_A();
extern void test_B();

PUBLIC u8 		idt_ptr[6];		// idt descriptor, 0-15:limit, 16-47:base
PUBLIC GATE	idt[IDT_SIZE];	// the whole idt.

PUBLIC u8 gdt_ptr[6];
PUBLIC DESCRIPTOR	gdt[GDT_SIZE];

PUBLIC PROCESS proc_table[NR_TASKS];

PUBLIC	char	task_stack[STACK_SIZE_TOTAL];
PUBLIC TSS tss;

PUBLIC PROCESS*		p_proc_ready;
PUBLIC TASK 	task_table[NR_TASKS] = {
	{test_A, STACK_SIZE_TESTA, "test_A"},
	{test_B, STACK_SIZE_TESTB, "test_B"}
};