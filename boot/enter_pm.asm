[bits 16]
[org 0x7c00]

xor ax, ax
mov ds, ax
mov es, ax
mov bx, 0x9000
mov ss, bx
mov sp, ax
cld

	mov bx, MSG_REAL_MODE
	call print_string
	
	call switch_to_pm
	
	jmp $
	
%include "print/print_string.asm"
%include "pm/gdt.asm"
%include "pm/print_string_pm.asm"
%include "pm/switch_to_pm.asm"

[bits 32]

BEGIN_PM:
	
	mov ebx, MSG_PROT_MODE
	call print_string_pm
	jmp $
	
MSG_REAL_MODE:
	db "Started in 16-bit Real Mode", 0
MSG_PROT_MODE:
	db "Now in 32-bit protected mode", 0
MSG_TEST:
	db "Test~"

times 510-($-$$) db 0
dw 0xaa55
