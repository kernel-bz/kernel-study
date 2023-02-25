	.file	"ptr-offset-contain-01.c"
	.option nopic
	.text
	.section	.rodata
	.align	3
.LC0:
	.string	"%p\n"
	.text
	.align	1
	.globl	main
	.type	main, @function
main:
	addi	sp,sp,-96
	sd	ra,88(sp)
	sd	s0,80(sp)
	addi	s0,sp,96
	li	a5,10
	sb	a5,-96(s0)
	li	a5,20
	sw	a5,-92(s0)
	li	a5,30
	sd	a5,-88(s0)
	addi	a5,s0,-96
	addi	a5,a5,8
	sd	a5,-24(s0)
	ld	a5,-24(s0)
	addi	a5,a5,-8
	sd	a5,-32(s0)
	ld	a1,-32(s0)
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	addi	a5,s0,-96
	mv	a1,a5
	lui	a5,%hi(.LC0)
	addi	a0,a5,%lo(.LC0)
	call	printf
	li	a5,0
	mv	a0,a5
	ld	ra,88(sp)
	ld	s0,80(sp)
	addi	sp,sp,96
	jr	ra
	.size	main, .-main
	.ident	"GCC: (GNU) 8.3.0"
	.section	.note.GNU-stack,"",@progbits
