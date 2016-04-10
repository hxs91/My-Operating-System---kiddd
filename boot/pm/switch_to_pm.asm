%ifndef SWITCH_TO_PM
%define SWITCH_TO_PM

[bits 16]

; switch to protected mode
switch_to_pm:
	cli							; disable the interrupts
	
	lgdt [gdt_descriptor]		; load the GDT
	
	mov eax, cr0				; entering the 32-bit mode
	or eax, 0x1
	mov cr0, eax
	
	jmp dword CODE_SEG:init_pm		; to ensure all stage of one instruction, we use a far jmp here
	
[bits 32]
;initialise registers and the stach once in PM
init_pm:
	
	mov ax, DATA_SEG
	mov ds, ax
	mov ss, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	
	mov ebp, 0x90000
	mov esp, ebp
	
	call BEGIN_PM

%endif