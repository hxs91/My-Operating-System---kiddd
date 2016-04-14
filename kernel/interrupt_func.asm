[bits 32]
;load idt_ptr and define the interrupt jump

%include "sconst.inc"

;global varibles
extern idt_ptr
extern p_proc_ready
extern tss
extern StackTop
extern irq_table
extern spurious_irq

;global function
extern exception_handler
extern clock_interrupt
extern keyboard_interrupt
extern delay
extern k_reenter
extern print_hex
extern print
extern restart
extern restart_reenter

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
global disable_irq
global enable_irq
global	hwint00
global	hwint01
global	hwint02
global	hwint03
global	hwint04
global	hwint05
global	hwint06
global	hwint07

clock_int_msg db "^", 0

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
ALIGN	16
clock_handler_invoker:
	call save

	sti

	;call interrupt handler
	push 0
	call clock_interrupt
	add esp, 4

	cli

	ret ; if reenter, then jump to restart_reenter; otherwise, jump to restart
	; this "ret" if very important~~~, keep it in mind!


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
	hlt

save:
	;store the value of register
	pushad
	push ds
	push es
	push fs
	push gs

	mov dx, ss
	mov ds, dx
	mov es, dx

	mov eax, esp	; eax = the start address of proc_table

	inc dword [k_reenter]
	cmp dword [k_reenter], 0
	jne .1	;jmp if reenter

	;switch to the kernel stack
	mov esp, StackTop

	push restart
	jmp [eax + RETADR - P_STACKBASE]
.1:
	push restart_reenter
	jmp [eax + RETADR - P_STACKBASE]


; 中断和异常 -- 硬件中断
; ---------------------------------
%macro	hwint_master	1
	call	save
	in	al, INT_M_CTLMASK	; `.
	or	al, (1 << %1)		;  | 屏蔽当前中断
	out	INT_M_CTLMASK, al	; /
	mov	al, EOI			; `. 置EOI位
	out	INT_M_CTL, al		; /
	sti	; CPU在响应中断的过程中会自动关中断，这句之后就允许响应新的中断
	push	%1			; `.
	call	[irq_table + 4 * %1]	;  | 中断处理程序
	pop	ecx			; /
	cli
	in	al, INT_M_CTLMASK	; `.
	and	al, ~(1 << %1)		;  | 恢复接受当前中断
	out	INT_M_CTLMASK, al	; /
	ret
%endmacro


ALIGN	16
hwint00:		; Interrupt routine for irq 0 (the clock).
	hwint_master	0

ALIGN	16
hwint01:		; Interrupt routine for irq 1 (keyboard)
	hwint_master	1

ALIGN	16
hwint02:		; Interrupt routine for irq 2 (cascade!)
	hwint_master	2

ALIGN	16
hwint03:		; Interrupt routine for irq 3 (second serial)
	hwint_master	3

ALIGN	16
hwint04:		; Interrupt routine for irq 4 (first serial)
	hwint_master	4

ALIGN	16
hwint05:		; Interrupt routine for irq 5 (XT winchester)
	hwint_master	5

ALIGN	16
hwint06:		; Interrupt routine for irq 6 (floppy)
	hwint_master	6

ALIGN	16
hwint07:		; Interrupt routine for irq 7 (printer)
	hwint_master	7



; ========================================================================
;                  void disable_irq(int irq);
; ========================================================================
; Disable an interrupt request line by setting an 8259 bit.
; Equivalent code:
;	if(irq < 8){
;		out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) | (1 << irq));
;	}
;	else{
;		out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) | (1 << irq));
;	}
disable_irq:
        mov     ecx, [esp + 4]          ; irq
        pushf
        cli
        mov     ah, 1
        rol     ah, cl                  ; ah = (1 << (irq % 8))
        cmp     cl, 8
        jae     disable_8               ; disable irq >= 8 at the slave 8259
disable_0:
        in      al, INT_M_CTLMASK
        test    al, ah
        jnz     dis_already             ; already disabled?
        or      al, ah
        out     INT_M_CTLMASK, al       ; set bit at master 8259
        popf
        mov     eax, 1                  ; disabled by this function
        ret
disable_8:
        in      al, INT_S_CTLMASK
        test    al, ah
        jnz     dis_already             ; already disabled?
        or      al, ah
        out     INT_S_CTLMASK, al       ; set bit at slave 8259
        popf
        mov     eax, 1                  ; disabled by this function
        ret
dis_already:
        popf
        xor     eax, eax                ; already disabled
        ret

; ========================================================================
;                  void enable_irq(int irq);
; ========================================================================
; Enable an interrupt request line by clearing an 8259 bit.
; Equivalent code:
;       if(irq < 8){
;               out_byte(INT_M_CTLMASK, in_byte(INT_M_CTLMASK) & ~(1 << irq));
;       }
;       else{
;               out_byte(INT_S_CTLMASK, in_byte(INT_S_CTLMASK) & ~(1 << irq));
;       }
;
enable_irq:
        mov     ecx, [esp + 4]          ; irq
        pushf
        cli
        mov     ah, ~1
        rol     ah, cl                  ; ah = ~(1 << (irq % 8))
        cmp     cl, 8
        jae     enable_8                ; enable irq >= 8 at the slave 8259
enable_0:
        in      al, INT_M_CTLMASK
        and     al, ah
        out     INT_M_CTLMASK, al       ; clear bit at master 8259
        popf
        ret
enable_8:
        in      al, INT_S_CTLMASK
        and     al, ah
        out     INT_S_CTLMASK, al       ; clear bit at slave 8259
        popf
        ret
