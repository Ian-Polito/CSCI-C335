	.file	"test.c"
	.local	state
	.comm	state,4,4
	.section	.rodata
main:
.LFB1:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$16, %rsp
	movl	$0, -4(%rbp)
	jmp	.L12
.L13:
	movl	$0, %eax
	call	stop_light
	movl	-4(%rbp), %eax
	movl	%eax, %edi
	call	inc
	movl	%eax, -4(%rbp)
.L12:
	cmpl	$35, -4(%rbp)
	jle	.L13
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE1:
	.size	main, .-main
	.local	state_counter.2185
	.comm	state_counter.2185,4,4
	.ident	"GCC: (GNU) 4.8.5 20150623 (Red Hat 4.8.5-28)"
	.section	.note.GNU-stack,"",@progbits
