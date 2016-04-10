#include "screen.h"
#include "type.h"
#include "global.h"
#include "const.h"

extern void restart();

void say_hello() {
	print("Welcome to my developed OS: kiddd~\n");
}

void test_A() {
// process A
	int i = 0;
	while(1) {
		print("A");
		print_hex(i++);
		print(".");
		delay(1);
	}
}

/* entry for kernel */
void kernel_main() {
	
	PROCESS*	p_proc		= proc_table;
	p_proc->ldt_sel = SELECTOR_LDT_FIRST;

	memory_copy(&gdt[SELECTOR_KERNEL_CS >> 3], &p_proc->ldts[0],
	       sizeof(DESCRIPTOR));
	p_proc->ldts[0].attr1 = DA_C | PRIVILEGE_TASK << 5;
	memory_copy(&gdt[SELECTOR_KERNEL_DS >> 3], &p_proc->ldts[1],
	       sizeof(DESCRIPTOR));
	p_proc->ldts[1].attr1 = DA_DRW | PRIVILEGE_TASK << 5;
	p_proc->regs.cs	= ((8 * 0) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.ds	=  ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.es	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.fs	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	p_proc->regs.ss	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;
	// p_proc->regs.gs	= (SELECTOR_KERNEL_GS & SA_RPL_MASK)
	// 	| RPL_TASK;	//I really do not know the purpose of the special setting for gs
	p_proc->regs.gs	= ((8 * 1) & SA_RPL_MASK & SA_TI_MASK)
		| SA_TIL | RPL_TASK;

	p_proc->regs.eip = (u32)test_A;
	p_proc->regs.esp = (u32)task_stack + STACK_SIZE_TOTAL;
	p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

	p_proc_ready	= proc_table;
	restart();

}

int main(){
	say_hello();
	kernel_main();
	while(1) {}
	return 0;
}