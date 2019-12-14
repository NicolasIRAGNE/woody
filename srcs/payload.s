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

   add r14, rax
   mov rax, r14
   jmp rax

align 8
   msg      db 'yeet', 10, 0
   msg_end  db 0x0
