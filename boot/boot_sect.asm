[bits 16]					; 16-bit mode 
[org 0x7c00]				; when cpu is in 16-bit mode, it will load the boot sector in 
							; memory begin in 0x7c00 

KERNEL_OFFSET equ 0x1000	; adress which the cpu will load kernel to

	mov [BOOT_DRIVE], dl 	; BIOS stores our boot drive in DL

	xor ax, ax
	mov ds, ax
	mov es, ax
	mov bx, 0x9000
	mov ss, bx
	mov sp, ax 
	cld

	mov bx, MSG_REAL_MODE
	call print_string

	call load_kernel     	; load the kernel from disk

	call switch_to_pm 		; entering 32-bit protected mode

	jmp $

;load kernel
load_kernel:
	pusha
	mov bx, MSG_LOAD_KERNEL
	call print_string


	; disk_load funciton will load the kernel to ES:BX from sector DL, and read DH sectors
	mov bx, KERNEL_OFFSET
	mov dh, 15	; read 15 sectors
	mov dl, [BOOT_DRIVE]
	
	call disk_load

	popa
	ret

; All includes
%include "disk_load.asm"
%include "pm/gdt.asm"
%include "pm/switch_to_pm.asm"
%include "print/print_string.asm"

[bits 32]
BEGIN_PM:

	mov ebx, MSG_PROT_MODE
	call print_string_pm
	
	call KERNEL_OFFSET		;our main function is just right in this KERNEL_OFFSET

	jmp $

%include "pm/print_string_pm.asm"

; Global variables
BOOT_DRIVE 		db 0
MSG_REAL_MODE 	db "Started in 16-bit Real Mode...\n", 0
MSG_PROT_MODE	db "Successfully landed in 32-bit Protected Mode...\n", 0
MSG_LOAD_KERNEL db "Loading kernel into memory...\n", 0

; padding
times 510-($-$$) db 0
dw 0xaa55