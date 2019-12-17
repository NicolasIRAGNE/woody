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

   mov rax, 0x1111111111111111
   push rdi
   mov rdi, 0x2222222222222222 ; addr of text section
   mov r13, 0x3333333333333333 ; size of text section
   mov r8,	0x4444444444444444 ; rc4 key

   add rax, r14
   add rdi, r14
   push rbp
   mov rbp, rsp
   sub rsp, 256

   mov r12, 0
_init_perm:
	mov [rsp + r12], r12b
	inc r12d
	cmp r12d, 256
	jne _init_perm

   mov r12, 0
   mov r9, 0
_set_perm:
	add r9, [rsp + r12]
	and r9, 0xff
	
	; NEED TO SWAP rsp + r12 and rsp + r9
	mov r11, [rsp + r9]
	mov r10, [rsp + r12]
	mov [rsp + r9], r10b
	mov [rsp + r12], r11b

	inc r12d
	cmp r12d, 256
	jne _set_perm

   mov r12, 0 ; i
   mov r9, 0 ; j
   mov r10, 0 ; k

_loop:
	inc r12
	and r12, 0xff
	add r9, [rsp + r12]
	and r9, 0xff
	 mov r11, [rsp + r9]
	 mov r15, [rsp + r12]
	 mov [rsp + r9], r15b
	 mov [rsp + r12], r11b
	and r9, 0xff
	and r11, 0xff
	mov r14, [rsp + r9]
	and r14, 0xff
	add r14, [rsp + r12]
	and r14, 0xff
	mov r14, [rsp + r14]
	xor r14, [rdi + r10]
	mov [rdi + r10], r14b
	inc r10
	cmp r10, r13
	jl _loop
   
   add rsp, 256
   pop rbp
   pop rdi
   jmp rax

align 8
   msg      db '...WOODY...', 10, 0
   msg_end  db 0x0
