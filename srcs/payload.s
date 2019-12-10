section .text
    global _start


_start:
; OFFSET
; 
	call _pc
_pc:
	pop r15

	sub r15, 0x5
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
   mov rdi, 0x2222222222222222

   sub r15, rdi
   add r15, rax
   mov rax, r15
   jmp rax

align 8
   msg      db 'yeet', 10, 0
   msg_end  db 0x0
