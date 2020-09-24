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
	mov rcx, 0
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
