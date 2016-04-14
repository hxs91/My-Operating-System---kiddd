#include "const.h"
#include "type.h"
#include "global.h"
#include "screen.h"

extern int k_reenter;

/*
clock interrupt handler
*/
PUBLIC void clock_interrupt(int irq) {
	//print("I will refresh per second.\n");
	print("#");

	if (k_reenter != 0) {
		print("!");
		return;
	}

	p_proc_ready ++;
	if (p_proc_ready >= proc_table + NR_TASKS) {
		p_proc_ready = proc_table;
	}
}