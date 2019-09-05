	.text
	.syntax unified
	.thumb
	.global	abs	
	.type abs, %function
abs:
	push {r4-r7}
	
	cmp r0, 0
	bgt .over
	rsbs r0, r0, #0
	b .over
over:
	
	pop {r4-r7}	
	bx	lr
