	.text
	.file	"WarpModule"
	.globl	my_function_1                   # -- Begin function my_function_1
	.p2align	4, 0x90
	.type	my_function_1,@function
my_function_1:                          # @my_function_1
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rbx
	.cfi_def_cfa_offset 16
	.cfi_offset %rbx, -16
	movl	%edi, %ebx
	callq	my_function_1_select_index@PLT
	cltq
	movq	my_function_1_table@GOTPCREL(%rip), %rcx
	movl	%ebx, %edi
	callq	*(%rcx,%rax,8)
	popq	%rbx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end0:
	.size	my_function_1, .Lfunc_end0-my_function_1
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1_0                 # -- Begin function my_function_1_0
	.p2align	4, 0x90
	.type	my_function_1_0,@function
my_function_1_0:                        # @my_function_1_0
	.cfi_startproc
# %bb.0:                                # %entry
	imull	$42, %edi, %eax
	retq
.Lfunc_end1:
	.size	my_function_1_0, .Lfunc_end1-my_function_1_0
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1_1                 # -- Begin function my_function_1_1
	.p2align	4, 0x90
	.type	my_function_1_1,@function
my_function_1_1:                        # @my_function_1_1
	.cfi_startproc
# %bb.0:                                # %entry
                                        # kill: def $edi killed $edi def $rdi
	leal	10(%rdi), %eax
	retq
.Lfunc_end2:
	.size	my_function_1_1, .Lfunc_end2-my_function_1_1
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1_2                 # -- Begin function my_function_1_2
	.p2align	4, 0x90
	.type	my_function_1_2,@function
my_function_1_2:                        # @my_function_1_2
	.cfi_startproc
# %bb.0:                                # %entry
                                        # kill: def $edi killed $edi def $rdi
	leal	(%rdi,%rdi), %eax
	retq
.Lfunc_end3:
	.size	my_function_1_2, .Lfunc_end3-my_function_1_2
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1_3                 # -- Begin function my_function_1_3
	.p2align	4, 0x90
	.type	my_function_1_3,@function
my_function_1_3:                        # @my_function_1_3
	.cfi_startproc
# %bb.0:                                # %entry
	pushq	%rax
	.cfi_def_cfa_offset 16
	decl	%edi
	callq	my_function_1@PLT
	popq	%rcx
	.cfi_def_cfa_offset 8
	retq
.Lfunc_end4:
	.size	my_function_1_3, .Lfunc_end4-my_function_1_3
	.cfi_endproc
                                        # -- End function
	.globl	my_function_1_select_index      # -- Begin function my_function_1_select_index
	.p2align	4, 0x90
	.type	my_function_1_select_index,@function
my_function_1_select_index:             # @my_function_1_select_index
	.cfi_startproc
# %bb.0:                                # %entry
	cmpl	$10, %edi
	setge	%al
	xorl	%ecx, %ecx
	xorl	%edx, %edx
	cmpl	$42, %edi
	setl	%cl
	setge	%dl
	andb	%cl, %al
	movzbl	%al, %eax
	imull	%eax, %ecx
	leal	(%rcx,%rdx,2), %ecx
	xorl	%eax, %eax
	xorl	%edx, %edx
	cmpl	$1000, %edi                     # imm = 0x3E8
	setl	%al
	setge	%dl
	leal	(%rdx,%rdx,2), %edx
	imull	%ecx, %eax
	addl	%edx, %eax
	retq
.Lfunc_end5:
	.size	my_function_1_select_index, .Lfunc_end5-my_function_1_select_index
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
	.quad	my_function_1_3
	.size	my_function_1_table, 32

	.section	".note.GNU-stack","",@progbits
