	.text
	.syntax unified
	.thumb
	.global string_compare_n
	.type string_compare_n, %function
string_compare_n:
	push {r4-r7}
	b .loop
	
.loop:
	cmp r2, #0
	beq .end
	subs r2, r2, #1
	ldrb r3, [r0, #0]
	ldrb r4, [r1, #0]
	cmp r3, r4
	bne .notequal
	adds r0, r0, #1
	adds r1, r1, #1
	b .loop

.notequal:
	subs r0, r3, r4
	b .end

.finished:
	movs r0, #0

.end:
	pop {r4-r7}
	bx lr
