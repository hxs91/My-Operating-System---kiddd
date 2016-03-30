%ifndef PRINT_STRING
%define PRINT_STRING

print_string:
	pusha
	mov ah, 0x0e
one_string:
	mov cl, [bx]
	cmp cl, 0
	je the_end
	mov al, cl
	int 0x10
	inc bx
	jmp one_string
the_end:
	popa
	ret
	
%endif