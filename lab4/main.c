/* main.c --- 
 * 
 * Filename: main.c
 * Description: 
 * Author: 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: 
 *           By: 
 *     Update #: 0
 * Keywords: 
 * Compatibility: 
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* Change log:
 * 
 * 
 */
/* Code: */

#include <f3d_uart.h>
#include <stdio.h>
#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
	f3d_uart_init();
	
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	putchar(getchar());	
	printf("Hello World!\n");
	
	//mywc
	int chars = 0;
	int words = 0;
	int lines = 0;
	int c;
	while ((c = getchar()) != 0x1b) {
		putchar(c);
		chars++;
		if (c == 32 || c == 13 || c == 9 || c == 11 || c == 12) {
			words++;
		} else if (c == 10) {
			//new line
			words++;
			lines++;
		}
	}
	printf("Total Characters: %d\n", chars);
	printf("Total Words: %d\n", words);
	printf("Total Lines: %d\n", lines);
	
	while (1){
		putchar(getchar());
		putchar('a');
		putstring("hello");
	}
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
