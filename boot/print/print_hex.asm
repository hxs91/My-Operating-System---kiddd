%ifndef PRINT_HEX
%define PRINT_HEX
print_hex:
	pusha
	mov cx, 5

get_one:
	mov bx, HEX_OUT		;store the address of HEX_OUT in bx
	add bx, cx			;let bx point to the right position
	mov ax, dx			;store a copy of dx to ax
	and ax, 0x000f		;get the last byte and store it in ax
	cmp ax, 9			;compare ax to 9
	jg letter			;if the value stored in ax is greater that 9, then it is a letter
	add ax, 48			;if not, it is a number
	jmp modify_str
letter:
	sub ax, 10			;figure out the difference between ax and 10
	add ax, 97			;store it corresponding ascii value to ax

modify_str:	
	mov [bx], al		;store the last byte into the position which bx point to
	dec cx				;point bx to its previous position
	shr dx, 4			;shift right dx
	cmp cx, 2
	jge get_one 
	
print_routine:	

	mov bx, HEX_OUT
	call print_string
	popa
	ret
	
%include "print_string.asm"

HEX_OUT:
	db '0x0000', 0

%endif