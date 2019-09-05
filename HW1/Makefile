hw1: test.c t1.s
	arm-none-eabi-as -g -mcpu=cortex-m0 -mthumb t1.s -o t1.o
	arm-none-eabi-gcc -g -c -O1 -mcpu=cortex-m0 -mthumb test.c -o test.o
	arm-none-eabi-gcc -g -mcpu=cortex-m0 -mthumb --specs=rdimon.specs -lc -lrdimon t1.o test.o -o HW1.elf

test: HW1.elf
	qemu-system-arm -machine versatilepb -cpu cortex-m3 -nographic \
	-monitor null -serial null \
	-semihosting -kernel HW1.elf
