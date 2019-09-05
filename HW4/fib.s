	.text
	.syntax unified
	.thumb
	.global	fib	
	.type fib, %function
fib:
	push {r4-r7}
	
	movs r1, #1
        movs r2, #1
        movs r3, #1
        movs r4, #0
	//check if value in r0 equals 0 or 1, in which case that is the fib number
	cmp r0, #0
	beq .end
	cmp r0, #1
	beq .end
	//find fib
	subs r0, #2
	b .loop

.loop:
	cmp r4, r0
	beq .end2
	adds r3, r1, r2
	movs r2, r1
	movs r1, r3
	adds r5, 1
	b .loop
	
.end2:
	movs r0, r3
	b .end
	
.end:
	pop {r4-r7}
	bx	lr			
