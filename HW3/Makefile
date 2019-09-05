diss: hw3.c
	gcc -o diss hw3.c
	arm-none-eabi-as test.s -o test.o
	arm-none-eabi-objcopy test.o -O binary --only-section=.text test.bin
	hexdump -x test.bin |sed -e 's/^[^ ]*//' > test.hex
	./diss < test.hex
	diff test.out test.s > test.diff
clean:
	rm -f diss
	rm -f test.o
	rm -f test.bin
	rm -f test.hex
	rm -f test.out
	rm -f test.diff
