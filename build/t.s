	.file	"rc4.c"
	.text
	.globl	generate_key
	.type	generate_key, @function
generate_key:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$24, %rsp
	.cfi_offset 3, -24
	call	rand@PLT
	cltq
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	salq	$32, %rax
	movq	%rax, %rbx
	call	rand@PLT
	cltq
	orq	%rbx, %rax
	movq	%rax, -24(%rbp)
	movq	-24(%rbp), %rax
	addq	$24, %rsp
	popq	%rbx
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	generate_key, .-generate_key
	.globl	swap
	.type	swap, @function
swap:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -24(%rbp)
	movq	%rsi, -32(%rbp)
	movq	-24(%rbp), %rax
	movzbl	(%rax), %eax
	movzbl	%al, %eax
	movl	%eax, -4(%rbp)
	movq	-32(%rbp), %rax
	movzbl	(%rax), %edx
	movq	-24(%rbp), %rax
	movb	%dl, (%rax)
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	movq	-32(%rbp), %rax
	movb	%dl, (%rax)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	swap, .-swap
	.globl	rc4_crypt
	.type	rc4_crypt, @function
rc4_crypt:
.LFB8:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$176, %rsp
	movq	%rdi, -280(%rbp)
	movq	%rsi, -288(%rbp)
	movq	%rdx, -296(%rbp)
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	movl	$0, -12(%rbp)
	jmp	.L5
.L6:
	movl	-4(%rbp), %eax
	movl	%eax, %edx
	movl	-4(%rbp), %eax
	cltq
	movb	%dl, -272(%rbp,%rax)
	addl	$1, -4(%rbp)
.L5:
	movl	-4(%rbp), %eax
	cmpl	$255, %eax
	jbe	.L6
	movl	$0, -4(%rbp)
	jmp	.L7
.L8:
	movl	-4(%rbp), %eax
	cltq
	movzbl	-272(%rbp,%rax), %eax
	movzbl	%al, %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	andl	$255, %eax
	movl	%eax, -8(%rbp)
	addl	$1, -4(%rbp)
.L7:
	movl	-4(%rbp), %eax
	cmpl	$255, %eax
	jbe	.L8
	movl	$0, -4(%rbp)
	movl	$0, -8(%rbp)
	jmp	.L9
.L10:
	movl	-4(%rbp), %eax
	addl	$1, %eax
	andl	$255, %eax
	movl	%eax, -4(%rbp)
	movl	-4(%rbp), %eax
	cltq
	movzbl	-272(%rbp,%rax), %eax
	movzbl	%al, %edx
	movl	-8(%rbp), %eax
	addl	%edx, %eax
	andl	$255, %eax
	movl	%eax, -8(%rbp)
	movl	-4(%rbp), %eax
	cltq
	movzbl	-272(%rbp,%rax), %eax
	movzbl	%al, %edx
	movl	-8(%rbp), %eax
	cltq
	movzbl	-272(%rbp,%rax), %eax
	movzbl	%al, %eax
	addl	%edx, %eax
	cltq
	movzbl	%al, %eax
	movzbl	-272(%rbp,%rax), %eax
	movzbl	%al, %eax
	movl	%eax, -16(%rbp)
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-280(%rbp), %rax
	addq	%rdx, %rax
	movzbl	(%rax), %ecx
	movl	-16(%rbp), %eax
	movl	%eax, %esi
	movl	-12(%rbp), %eax
	movslq	%eax, %rdx
	movq	-280(%rbp), %rax
	addq	%rdx, %rax
	xorl	%esi, %ecx
	movl	%ecx, %edx
	movb	%dl, (%rax)
	addl	$1, -12(%rbp)
.L9:
	movl	-12(%rbp), %eax
	cltq
	cmpq	%rax, -288(%rbp)
	ja	.L10
	nop
	nop
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE8:
	.size	rc4_crypt, .-rc4_crypt
	.ident	"GCC: (Debian 9.2.1-17) 9.2.1 20191102"
	.section	.note.GNU-stack,"",@progbits
