%ifndef PRINT_STRING_PM_ASM
%define PRINT_STRING_PM_ASM

[bits 32]

;define constants
VIDEO_MEMORY equ 0xb8000	; the memory started from 0xb8000 is mapping to the screen.
							; so when we modify the content in this scope, actually we are 
							; modify what will be printed on screen
WHITE_ON_BLACK equ 0x0f		; control the color

print_string_pm:
	pusha
	mov edx, VIDEO_MEMORY	; store the adresss in edx
	
print_string_pm_loop:
	mov al, [ebx]			; the adress of string is stored in ebx, thus ebx works as parameters
	mov ah, WHITE_ON_BLACK
	cmp al, 0				; string is always ending with 0x0
	je print_string_pm_done
	mov [edx], ax
	add ebx, 1
	add edx, 2				; consume two bytes
	jmp print_string_pm_loop

print_string_pm_done:
	popa
	ret

%endif