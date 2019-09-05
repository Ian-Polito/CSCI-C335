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

/*
Ian Polito
ipolito
Lab 10
Main.c
Partner: Colin Curry | colcurry
*/

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>
#include <f3d_systick.h>
#include <f3d_uart.h>
#include <f3d_gyro.h>
#include <f3d_delay.h>
#include <queue.h>
#include <f3d_lcd_sd.h>
#include <f3d_user_btn.h>
#include <stdio.h>
//#include <f3d_rtc.h>
#include <math.h>

#define PI 3.14152965

int main(void) { 
  	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	
	printf("loading uart\n");
        f3d_uart_init();
	printf("loading button\n");
	f3d_user_btn_init();
	printf("loading led\n");
	f3d_led_init();
	
	f3d_systick_init(100);
 	while (1) {
		//putchar(getchar());
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
