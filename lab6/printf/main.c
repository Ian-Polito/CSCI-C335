/*
Ian Polito
ipolito
Lab 6
Printf
September 28, 2018

Partner: Colin Curry
*/

//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

// Simple looping delay function
void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop"); /* This stops it optimising code out */
  }
}

int main(void){
	f3d_uart_init();
	f3d_gyro_init();
	f3d_user_btn_init();
	f3d_led_init();
	
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	
	while (1) {
		f3d_led_all_off();
		printf("1 Call to printf.\n");
		printf("2 Call to printf.\n");
		printf("3 Call to printf.\n");
		printf("4 Call to printf.\n");
		printf("5 Call to printf.\n");
		f3d_led_on(0);
		delay();
	}
}
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
