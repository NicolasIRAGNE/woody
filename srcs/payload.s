section .text
    global _start


_start:
; OFFSET
; 
	call _pc
_pc:
	pop r14

	sub r14, 0x5
   push rax
   push rdi
   push rsi
   push rdx


   mov rax, 1             
   mov rdi, 1             
   lea rsi, [rel msg]     
   mov rdx, msg_end - msg 
   syscall                

   ;; restore cpu state
   pop rdx
   pop rsi
   pop rdi
   pop rax

   ;; jump to main

   mov rax, 0x1111111111111111
   mov rdi, 0x2222222222222222 ; addr of text section
   mov r13, 0x3333333333333333 ; size of text section
   mov r8,	0x4444444444444444 ; rc4 key

   add rax, r14
   add rdi, r14

_loop:
	mov r15b, BYTE [rdi]
	sub r15b, 42
	mov [rdi], r15b
	dec r13
	inc rdi
	cmp r13, 0
	jne _loop

   jmp rax

align 8
   msg      db '...WOODY...', 10, 0
   msg_end  db 0x0
