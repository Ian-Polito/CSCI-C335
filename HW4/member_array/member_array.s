	.text
	.syntax unified
	.thumb
	.global member_array
	.type member_array, %function
member_array:
	push {r4-r7}
	
	movs r3, #0
	lsls r2, r2, #2
	b .loop
	
.loop:
	ldr r4, [r0, r3]
	cmp r4, r1
	bne .notequal
	movs r0, #1 //its in the array
	b .end //exit loop

.notequal:
	adds r3, r3, #4
	cmp r3, r2
	bcc .loop //keep searching array
	movs r0, #0 //end of array, so its not in the array
	b .end
.end:
	pop {r4-r7}
	bx lr
