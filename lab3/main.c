/*
* Filename: main.c
* Part of: Lab 3
* Author: Ian Polito
* Date Created: 9/7/18
* Date Last Modified: 9/7/18
* Last Modified by: Ian Polito
*
* Partner Colin Curry colcurry
*/

#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void) {
	f3d_led_init();
	f3d_user_btn_init();
	int i;
	while (1) {
		i = 0;
		while (i < 8) {
			if (user_btn_read() == 0) {
                		f3d_led_all_off();
                		f3d_led_on(i);
				delay();
				i++;
			}
        	}
		f3d_led_all_off();
		delay();
		f3d_led_all_on();
		delay();
		f3d_led_all_off();
		delay();
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
