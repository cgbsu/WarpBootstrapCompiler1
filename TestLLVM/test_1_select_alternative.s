	.text
	.file	"WarpModule"
	.globl	my_function_1_0                 # -- Begin function my_function_1_0
	.p2align	4, 0x90
	.type	my_function_1_0,@function
my_function_1_0:                        # @my_function_1_0
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	xorl	%edi, %edi
	callq	print_integer@PLT
	imull	$42, %ebx, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	my_function_1_0, .Lfunc_end0-my_function_1_0
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1_1                 # -- Begin function my_function_1_1
	.p2align	4, 0x90
	.type	my_function_1_1,@function
my_function_1_1:                        # @my_function_1_1
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	movl	$1, %edi
	callq	print_integer@PLT
	addl	$10, %ebx
	movl	%ebx, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end1:
	.size	my_function_1_1, .Lfunc_end1-my_function_1_1
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1_2                 # -- Begin function my_function_1_2
	.p2align	4, 0x90
	.type	my_function_1_2,@function
my_function_1_2:                        # @my_function_1_2
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	movl	$2, %edi
	callq	print_integer@PLT
	addl	%ebx, %ebx
	movl	%ebx, %eax
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end2:
	.size	my_function_1_2, .Lfunc_end2-my_function_1_2
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1                   # -- Begin function my_function_1
	.p2align	4, 0x90
	.type	my_function_1,@function
my_function_1:                          # @my_function_1
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%r15
	.cfi_def_cfa_offset 16
	pushq	%r14
	.cfi_def_cfa_offset 24
	pushq	%rbx
	.cfi_def_cfa_offset 32
	.cfi_offset %rbx, -32
	.cfi_offset %r14, -24
	.cfi_offset %r15, -16
	movl	%edi, %ebx
	cmpl	$42, %edi
	setl	%al
	cmpl	$10, %edi
	setge	%cl
	andb	%al, %cl
	movzbl	%cl, %eax
	xorl	%ecx, %ecx
	cmpl	$43, %edi
	setge	%cl
	leal	(%rax,%rcx,2), %r14d
	movq	my_function_1_table@GOTPCREL(%rip), %r15
	leaq	(%r15,%r14,8), %rdi
                                        # kill: def $edi killed $edi killed $rdi
	callq	print_integer@PLT
	movl	%ebx, %edi
	callq	*(%r15,%r14,8)
	popq	%rbx
	.cfi_def_cfa_offset 24
	popq	%r14
	.cfi_def_cfa_offset 16
	popq	%r15
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end3:
	.size	my_function_1, .Lfunc_end3-my_function_1
	.cfi_endproc
                                        # -- End function
	.type	my_function_1_table,@object     # @my_function_1_table
	.data
	.globl	my_function_1_table
	.p2align	4
my_function_1_table:
	.quad	my_function_1_0
	.quad	my_function_1_1
	.quad	my_function_1_2
	.size	my_function_1_table, 24

	.section	".note.GNU-stack","",@progbits
