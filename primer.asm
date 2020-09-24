section .data
arg1: db 'wdwdwdwdwdwdwdwdw', 0
section .text
global _start
_start:
mov rdi, arg1
mov rdx, 2
call string_copy
mov rax, 60
xor rdi, rdi
syscall
string_length:

xor rax, rax

.loop:

cmp byte[rdi+rax],0

je .end

inc rax

jmp .loop

.end:

ret
string_copy:
        xor rax, rax
        push rbx
        call string_length
        mov rcx, 0
        cmp rax, rdx
        jbe .loop
        mov rax, 0
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
