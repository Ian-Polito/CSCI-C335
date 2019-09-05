	.text
	.syntax unified
	.thumb
	.global	gcd	
	.type gcd, %function
gcd:
	push {r4-r7}
	
	movs r2, #0
	movs r5, #1
	b .loop
.loop:
	movs r3, #1
	movs r4, #1
	ands r3, r0
	ands r4, r1
	eors r3, r5
	eors r4, r5
	ands r3, r4
	beq .loop2
	asrs r0, #1
	asrs r1, #1
	adds r2, #1
	b .loop
.loop2:
	cmp r0, r1
	beq .end
	movs r3, #1
	ands r3, r0
	eors r3, r5
	bne .agreater
	movs r4, #1
	ands r4, r1
	eors r4, r5
	bne .bgreater
	cmp r0, r1
	bgt .third //if a = ((a-b) >> 1), special case
	subs r1, r0
	asrs r1, #1
	b .loop2
.agreater:
	//if a >>= 1
	asrs r0, #1
	b .loop2
.bgreater:
	//if b >>= 1
	asrs r1, #1
	b .loop2
.third:
	//special case
	subs r0, r1
	asrs r0, #1
	b .loop2
	
.end:
	lsls r0, r2
	pop {r4-r7}
	bx	lr
