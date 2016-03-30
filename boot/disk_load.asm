%ifndef DISK_LOAD_ASM
%define DISK_LOAD_ASM
;load DH sectors to ES:BS from drive DL

disk_load:
	pusha
	push dx
	
	mov ah, 0x02
	mov al, dh
	mov ch, 0x00
	mov dh, 0x00
	mov cl, 0x02
	
	int 0x13
	jc disk_error

	pop dx
	cmp dh, al
	jne wrong_sec
	popa
	ret

disk_error:
	mov bx, DISK_ERROR_MSG
	call print_string
	jmp $

wrong_sec:
	mov bx, READ_WRONG_SEC
	call print_string
	jmp $

%include "print/print_string.asm"

DISK_ERROR_MSG db "Disk read error!", 0
READ_WRONG_SEC db "Read wrong sectors!", 0

%endif
