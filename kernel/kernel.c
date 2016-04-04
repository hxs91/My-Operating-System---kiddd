#include "screen.h"
#include "type.h"
#include "global.h"

extern void load_idt_ptr();
extern void init_prot();
extern void enable_interrupt();

void say_hello() {
	clear_screen();
	print("Welcome to my developed OS: kiddd~\n");
}

/* initialize the setting of interrupt */
void init_interrupt() {
	print("Initialize the interrupt...\n");
	
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

	print("Initialize end.\n");
}

int main(){
	say_hello();
	init_interrupt();
	//__asm__("ud2":::);
	while(1){}
	return 0;
}