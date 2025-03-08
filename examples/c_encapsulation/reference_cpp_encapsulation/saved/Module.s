	.file	"Module.cpp"
	.text
	.align 2
	.globl	_ZN6ModuleD2Ev
	.type	_ZN6ModuleD2Ev, @function
_ZN6ModuleD2Ev:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	nop
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	_ZN6ModuleD2Ev, .-_ZN6ModuleD2Ev
	.globl	_ZN6ModuleD1Ev
	.set	_ZN6ModuleD1Ev,_ZN6ModuleD2Ev
	.align 2
	.globl	_ZN6ModuleC2Ei
	.type	_ZN6ModuleC2Ei, @function
_ZN6ModuleC2Ei:
.LFB4:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rax
	movl	-12(%rbp), %edx
	movl	%edx, (%rax)
	movq	-8(%rbp), %rax
	movl	$5555, 4(%rax)
	cmpl	$1, -12(%rbp)
	jne	.L3
	movq	-8(%rbp), %rax
	movl	$2, (%rax)
	jmp	.L2
.L3:
	movq	-8(%rbp), %rax
	movl	-12(%rbp), %edx
	movl	%edx, (%rax)
.L2:
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE4:
	.size	_ZN6ModuleC2Ei, .-_ZN6ModuleC2Ei
	.globl	_ZN6ModuleC1Ei
	.set	_ZN6ModuleC1Ei,_ZN6ModuleC2Ei
	.align 2
	.globl	_ZN6Module8addToFooEi
	.type	_ZN6Module8addToFooEi, @function
_ZN6Module8addToFooEi:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movl	%esi, -12(%rbp)
	movq	-8(%rbp), %rax
	movl	(%rax), %edx
	movl	-12(%rbp), %eax
	addl	%edx, %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	_ZN6Module8addToFooEi, .-_ZN6Module8addToFooEi
	.align 2
	.globl	_ZN6Module9getBeaconEv
	.type	_ZN6Module9getBeaconEv, @function
_ZN6Module9getBeaconEv:
.LFB7:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	movq	%rdi, -8(%rbp)
	movq	-8(%rbp), %rax
	movl	4(%rax), %eax
	popq	%rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE7:
	.size	_ZN6Module9getBeaconEv, .-_ZN6Module9getBeaconEv
	.ident	"GCC: (Debian 12.2.0-14) 12.2.0"
	.section	.note.GNU-stack,"",@progbits
