[bits 32]
;load idt_ptr and define the interrupt jump

extern idt_ptr
extern exception_handler
extern clock_interrupt
extern keyboard_interrupt

global load_idt_ptr
global disable_interrupt
global enable_interrupt
global nmi
global divide_error
global single_step_exception
global breakpoint_exception
global overflow
global bounds_check
global inval_opcode
global copr_not_available
global double_fault
global copr_seg_overrun
global inval_tss
global segment_not_present
global stack_exception
global general_protection
global page_fault
global copr_error
global clock_handler_invoker
global keyboard_interrupt_invoker

load_idt_ptr:
	cli
	lidt [idt_ptr]
	ret

enable_interrupt:
	sti
	ret

disable_interrupt:
	cli
	ret

;clock
clock_handler_invoker:
	call clock_interrupt
	iretd

;keyboard
keyboard_interrupt_invoker:
	call keyboard_interrupt
	iretd

divide_error:
	push 	0xFFFFFFFF
	push	0
	jmp 	exception
single_step_exception:
	push 	0xFFFFFFFF
	push	1
	jmp 	exception
nmi:
	push 	0xFFFFFFFF
	push	2
	jmp 	exception
breakpoint_exception:
	push 	0xFFFFFFFF
	push	3
	jmp 	exception
overflow:
	push 	0xFFFFFFFF
	push	4
	jmp 	exception
bounds_check:
	push 	0xFFFFFFFF
	push	5
	jmp 	exception
inval_opcode:
	push 	0xFFFFFFFF
	push	6
	jmp 	exception
copr_not_available:
	push 	0xFFFFFFFF
	push	7
	jmp 	exception
double_fault:
	push	8
	jmp 	exception
copr_seg_overrun:
	push 	0xFFFFFFFF
	push	9
	jmp 	exception
inval_tss:
	push	10
	jmp 	exception
segment_not_present:
	push	11
	jmp 	exception
stack_exception:
	push	12
	jmp 	exception
general_protection:
	push	13
	jmp 	exception
page_fault:
	push	14
	jmp 	exception
copr_error:
	push 	0xFFFFFFFF
	push	16
	jmp 	exception
exception:
	call exception_handler
	add	esp, 4*2
	iretd
