    	.text
    	.syntax unified
    	.thumb
    	.global mult_by_7
    	.type mult_by_7, %function
mult_by_7:
	push {r4-r7}
	
	lsls r1, r0, #3
	subs r1, r0
	movs r0, r1
	
    	pop {r4-r7} 
    	bx  lr
