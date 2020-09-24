section .text

string_length:
	xor rax,rax
	.loop:
		cmp byte[rdi+rax],0
		je .end
		inc rax
		jmp .loop
	.end:
		ret

print_string:
	xor rax,rax
	mov rsi, rdi
	call string_length
	mov rdx, rax
	mov rax, 1
	mov rdi, 1
	syscall
	ret

print_char:
	xor rax, rax
	push rdi
	mov rsi, rsp
	mov rdx, 1
	mov rax, 1
	mov rdi, 1
	syscall
	pop rdi
	ret

string_copy:
	xor rax, rax
	push rbx
	call string_length
	mov rcx, 0
	cmp rax, rdx
	jbe .loop
	mov rax, 0
	pop rbx
	ret
	.loop:
		mov bl, [rdi+rcx]
		mov [rsi+rcx], bl
		cmp bl, 0
		jz .end
		inc rcx
		jmp .loop
	.end:
		pop rbx
		ret

print_newline:
	xor rax, rax
	mov rdi, 0xA
	call print_char
	ret

print_uint:
	xor rax, rax
	push rbp
	mov rbp, rsp
	mov rax, rdi
	mov r8, 10
	sub rsp, 40h
	dec rbp
	mov byte[rbp], 0
	.loop:
		xor rdx, rdx
		div r8
		add rdx, '0'
		dec rbp
		mov byte[rbp], dl
		test rax, rax
		jnz .loop
	mov rdi, rbp
	call print_string
	add rsp, 40h
	pop rbp
	ret

print_int:
	xor rax, rax
	test rdi, rdi
	jns .uint
	push rdi
	mov rdi, '-'
	call print_char
	pop rdi
	neg rdi
	call print_uint
	ret
	.uint:
		jmp print_uint

read_char:
	xor rax, rax
	push rax
	mov rsi, rsp
	mov rdx, 1
	mov rdi, 0
	syscall
	pop rax
	ret

read_word:
	xor rax, rax
	push r8
	push r9
	xor r8, r8
	mov r9, rsi
	.symbol:
		push rdi
		call read_char
		pop rdi
		cmp al, 9
		jz .symbol
		cmp al, 32
		jz .symbol
	.worD:
		mov byte[rdi+r8], al
		cmp al, 0
		jz .end
		cmp al, 32
		jz .end
		cmp al, 9
		jz .end
		inc r8
		cmp r8, r9
		je .error
		push rdi
		call read_char
		pop rdi
		jmp .worD
	.end:
		mov rax, 0
		mov rdx, r8
		mov byte[rdi+r8],0
		mov rax, rdi
		pop r8
		pop r9
		ret
	.error:
		xor rax, rax
		pop r8
		pop r9
		ret

parse_uint:
	xor rax, rax
	push rbx
	call string_length
	mov rcx, rax
	xor rax, rax
	xor rdx, rdx
	.loop:
		xor rbx, rbx
		mov bl, byte[rdi+rdx]
		cmp bl, '0'
		jb .end
		cmp bl, '9'
		ja .end
		sub bl, '0'
		imul rax, 10
		add rax, rbx
		inc rdx
		dec rcx
		jnz .loop
	.end:
		pop rbx
		ret

parse_int:
	xor rax, rax
	push rbx
	call string_length
	mov rcx, rax
	xor rax, rax
	xor rdx, rdx
	mov rsi, rdi
	xor bl, bl
	mov bl, byte[rsi+rdx]
	cmp bl, '-'
	je .minus
	pop rbx
	jmp parse_uint
	.minus:
		inc rdi
		call parse_uint
		neg rax
		inc rdx
		pop rbx
		ret

string_equals:
	xor rax, rax
	push rbx
	call string_length
	mov rcx, rax
	.loop:
		mov dl, byte[rdi+rcx]
		mov bl, byte[rsi+rcx]
		cmp dl, bl
		jnz .noEqual
		cmp bl, 0
		jz .equal
		dec rcx
		jnz .loop
	.equal:
		pop rbx
		mov rax, 1
		ret
	.noEqual:
		pop rbx
		mov rax, 0
		ret
