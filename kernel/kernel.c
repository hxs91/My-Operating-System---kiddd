#include "screen.h"
#include "type.h"
#include "global.h"
#include "const.h"
#include "proc.h"

extern void restart();
extern void put_irq_handler(int irq, irq_handler handler);
extern void enable_irq(int irq);
extern void clock_interrupt(int irq);

PUBLIC u32 k_reenter;

void say_hello() {
	print("Welcome to my developed OS: kiddd~\n");
}

PUBLIC void test_A() {
// process A
	int i = 0;
	while(1) {
		print("A");
		print_hex(i++);
		print(".");
		delay(1);
	}
}

PUBLIC void test_B() {
	int i = 0x1000;
	while(1) {
		print("B");
		print_hex(i++);
		print(".");
		delay(1);
	}
}


PUBLIC void test_C() {
	int i = 0x2000;
	while(1) {
		print("C");
		print_hex(i++);
		print(".");
		delay(1);
	}
}

void init_process() {

	task_table[0].initial_eip = test_A;
	task_table[0].stacksize = STACK_SIZE_TESTA;

	task_table[1].initial_eip = test_B;
	task_table[1].stacksize = STACK_SIZE_TESTB;

	task_table[2].initial_eip = test_C;
	task_table[2].stacksize = STACK_SIZE_TESTC;
}

/* entry for kernel */
void kernel_main() {
	
	TASK * 		p_task 		= task_table;
	PROCESS*	p_proc		= proc_table;
	char * p_task_stack 	= task_stack + STACK_SIZE_TOTAL;
	u16 		selector_ldt = SELECTOR_LDT_FIRST;
	int i;
	for(i = 0 ; i < NR_TASKS; i++) {

		p_proc->pid = i;
		p_proc->ldt_sel = selector_ldt;

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

		p_proc->regs.eip = (u32)p_task->initial_eip;
		p_proc->regs.esp = (u32)p_task_stack;
		p_proc->regs.eflags = 0x1202; /* IF=1, IOPL=1 */

		p_task_stack -= p_task->stacksize;
		p_proc ++;
		p_task ++;
		selector_ldt += 1<<3;
	}

	put_irq_handler(CLOCK_IRQ, clock_interrupt); /* 设定时钟中断处理程序 */
    enable_irq(CLOCK_IRQ);                     /* 让8259A可以接收时钟中断 */

	k_reenter = 0;
	p_proc_ready	= proc_table;
	restart();

}

int main(){
	say_hello();
	init_process();
	kernel_main();
	while(1) {}
	return 0;
}