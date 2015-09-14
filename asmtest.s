	.file	"asmtest.c"
	.section	.rodata
	.align 8
.LC0:
	.string	"trial\tassembly\tmonotonic\tcputime\tthread\trusage"
.LC1:
	.string	"%d\t%ld\t\t%ld\t\t%ld\t%ld\t%ld\n"
	.text
.globl main
	.type	main, @function
main:
.LFB0:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	pushq	%rbx
	subq	$1208, %rsp
	movl	%edi, -1188(%rbp)
	movq	%rsi, -1200(%rbp)
	movl	$0, -36(%rbp)
	jmp	.L2
	.cfi_offset 3, -24
.L3:
#APP
# 15 "asmtest.c" 1
	rdtsc; mov %eax, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -52(%rbp)
#APP
# 16 "asmtest.c" 1
	mov %edx, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -48(%rbp)
	movl	$0, -56(%rbp)
	movl	$1, -56(%rbp)
	movl	$2, -56(%rbp)
	movl	$3, -56(%rbp)
	movl	$4, -56(%rbp)
	movl	$5, -56(%rbp)
#APP
# 23 "asmtest.c" 1
	rdtsc; mov %eax, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -44(%rbp)
#APP
# 24 "asmtest.c" 1
	mov %edx, %eax;
# 0 "" 2
#NO_APP
	movl	%eax, -40(%rbp)
	movl	-36(%rbp), %ecx
	movl	-48(%rbp), %eax
	movl	-40(%rbp), %edx
	movl	%edx, %ebx
	subl	%eax, %ebx
	movl	%ebx, %eax
	imull	$1000000000, %eax, %eax
	addl	-44(%rbp), %eax
	subl	-52(%rbp), %eax
	movslq	%eax, %rdx
	movslq	%ecx, %rax
	movq	%rdx, -224(%rbp,%rax,8)
	addl	$1, -36(%rbp)
.L2:
	cmpl	$19, -36(%rbp)
	jle	.L3
	movl	$0, -36(%rbp)
	jmp	.L4
.L5:
	leaq	-880(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi

		MONOTONIC CALLS

	call	clock_gettime
	leaq	-896(%rbp), %rax
	movq	%rax, %rsi
	movl	$1, %edi
	call	clock_gettime
	movl	-36(%rbp), %eax
	movq	-896(%rbp), %rcx
	movq	-880(%rbp), %rdx
	movq	%rcx, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
	imulq	$1000000000, %rdx, %rcx
	movq	-888(%rbp), %rdx
	addq	%rdx, %rcx
	movq	-872(%rbp), %rdx
	movq	%rcx, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
	cltq
	movq	%rdx, -384(%rbp,%rax,8)
	addl	$1, -36(%rbp)
.L4:
	cmpl	$19, -36(%rbp)
	jle	.L5
	movl	$0, -36(%rbp)
	jmp	.L6
.L7:
	leaq	-880(%rbp), %rax
	movq	%rax, %rsi
	movl	$2, %edi

		CPU TIME CALLS

	call	clock_gettime
	leaq	-896(%rbp), %rax
	movq	%rax, %rsi
	movl	$2, %edi
	call	clock_gettime
	movl	-36(%rbp), %eax
	movq	-896(%rbp), %rcx
	movq	-880(%rbp), %rdx
	movq	%rcx, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
	imulq	$1000000000, %rdx, %rcx
	movq	-888(%rbp), %rdx
	addq	%rdx, %rcx
	movq	-872(%rbp), %rdx
	movq	%rcx, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
	cltq
	movq	%rdx, -544(%rbp,%rax,8)
	addl	$1, -36(%rbp)
.L6:
	cmpl	$19, -36(%rbp)
	jle	.L7
	movl	$0, -36(%rbp)
	jmp	.L8
.L9:
	leaq	-880(%rbp), %rax
	movq	%rax, %rsi
	movl	$3, %edi
	call	clock_gettime
	leaq	-896(%rbp), %rax
	movq	%rax, %rsi
	movl	$3, %edi
	call	clock_gettime
	movl	-36(%rbp), %eax
	movq	-896(%rbp), %rcx
	movq	-880(%rbp), %rdx
	movq	%rcx, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
	imulq	$1000000000, %rdx, %rcx
	movq	-888(%rbp), %rdx
	addq	%rdx, %rcx
	movq	-872(%rbp), %rdx
	movq	%rcx, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
	cltq
	movq	%rdx, -704(%rbp,%rax,8)
	addl	$1, -36(%rbp)
.L8:
	cmpl	$19, -36(%rbp)
	jle	.L9
	movl	$0, -36(%rbp)
	jmp	.L10
.L11:
	leaq	-1040(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	getrusage
	leaq	-1184(%rbp), %rax
	movq	%rax, %rsi
	movl	$0, %edi
	call	getrusage
	movq	-1040(%rbp), %rdx
	movq	-1024(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	-1016(%rbp), %rdx
	movq	-1032(%rbp), %rax
	leaq	(%rdx,%rax), %rax
	imulq	$1000000, %rax, %rax
	leaq	(%rcx,%rax), %rax
	movq	%rax, -32(%rbp)
	movq	-1184(%rbp), %rdx
	movq	-1168(%rbp), %rax
	leaq	(%rdx,%rax), %rcx
	movq	-1160(%rbp), %rdx
	movq	-1176(%rbp), %rax
	leaq	(%rdx,%rax), %rax
	imulq	$1000000, %rax, %rax
	leaq	(%rcx,%rax), %rax
	movq	%rax, -24(%rbp)
	movl	-36(%rbp), %eax
	movq	-32(%rbp), %rdx
	movq	-24(%rbp), %rcx
	movq	%rcx, %rbx
	subq	%rdx, %rbx
	movq	%rbx, %rdx
	cltq
	movq	%rdx, -864(%rbp,%rax,8)
	addl	$1, -36(%rbp)
.L10:
	cmpl	$19, -36(%rbp)
	jle	.L11
	movl	$.LC0, %edi
	call	puts
	movl	$1, -36(%rbp)
	jmp	.L12
.L13:
	movl	-36(%rbp), %eax
	cltq
	movq	-864(%rbp,%rax,8), %rsi
	movl	-36(%rbp), %eax
	cltq
	movq	-704(%rbp,%rax,8), %r8
	movl	-36(%rbp), %eax
	cltq
	movq	-544(%rbp,%rax,8), %rdi
	movl	-36(%rbp), %eax
	cltq
	movq	-384(%rbp,%rax,8), %rcx
	movl	-36(%rbp), %eax
	cltq
	movq	-224(%rbp,%rax,8), %rdx
	movl	$.LC1, %eax
	movl	-36(%rbp), %ebx
	movq	%rsi, (%rsp)
	movq	%r8, %r9
	movq	%rdi, %r8
	movl	%ebx, %esi
	movq	%rax, %rdi
	movl	$0, %eax
	call	printf
	addl	$1, -36(%rbp)
.L12:
	cmpl	$19, -36(%rbp)
	jle	.L13
	movl	$0, %eax
	addq	$1208, %rsp
	popq	%rbx
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE0:
	.size	main, .-main
	.ident	"GCC: (GNU) 4.4.7 20120313 (Red Hat 4.4.7-16)"
	.section	.note.GNU-stack,"",@progbits
