/* main.c --- 
 * 
 * Filename: main.c
 * Description: main code for lab 6
 * Author: Colin Curry | colcurry, Ian Polito | ipolito 
 * Maintainer: 
 * Created: Thu Jan 10 11:23:43 2013
 * Last-Updated: Fri Sept 28 16:19:10 2018
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
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_user_btn.h>

int main(void) {
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	f3d_uart_init();
	delay(100);
	f3d_gyro_init();
	delay(100);
	f3d_lcd_init();
	f3d_lcd_fillScreen2(RED);
	f3d_user_btn_init();
	char c;
	char temp[15];
	uint16_t colors[10];
	int i;
	for(i = 0; i < 10;i++) {
	  colors[i] = WHITE;
	}
	char axis = 'x';
	int val;
	float test[3] = {};
	while (1) {
	  /*
	  	f3d_gyro_getdata(test);
		printf("%f %f %f\n", test[0], test[1], test[2]);
		
	  */
		  
		//each time the button is pressed, cycle through the order of axes
		if(user_btn_read()) {
		  if(axis == 'x') {
		    axis = 'y';
		  }
		  else if(axis == 'y') {
		    axis = 'z';
		  }
		  else {
		    axis = 'x';
		  }
		}
		//if the user inputs x, y, or z, change the axis appropriately
	        if (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) != (uint16_t)RESET) {
		  c = USART_ReceiveData(USART1);
		}
		if(c == 'x') {
		  axis = 'x';
		}
		else if(c == 'y') {
		  axis = 'y';
		}
		else if(c == 'z') {
		  axis = 'z';
		}

		//read data on the gyro
		f3d_gyro_getdata(test);
		delay(50);
		f3d_lcd_fillScreen2(RED);
		//draw gyro data on lcd
		//help with drawing:
		//f3d_lcd_setAddrWindow(x0, y0, x1, y1, MADCTLGRAPHICS)
		if(axis == 'x') {
		  val = (int)(test[0]);
		  sprintf(temp, "x: %d", val);
		  f3d_lcd_drawString(0,0,temp,WHITE,BLACK);
		  //draw bar
		  f3d_lcd_setAddrWindow(76,76,86,(76 + val), MADCTLGRAPHICS);
		  uint8_t x, y;
		  for(x = 0; x < val; x++) {
		    for(y = 0; y < 86 - 76; y++) {
		      f3d_lcd_pushColor(colors, 1);
		    }
		  }
		  //f3d_lcd_pushColor(colors, 10);
		  //printf("x: %f\n", test[0]);
		}
		else if(axis == 'y') {
		  val = (int)(test[1]);
		  sprintf(temp, "y: %d", val);
		  f3d_lcd_drawString(0,0,temp,WHITE,BLACK);
		  //draw bar
		  f3d_lcd_setAddrWindow(76,76,86,(76 + val), MADCTLGRAPHICS);
		  uint8_t x, y;
		  for(x = 0; x < val; x++) {
		    for(y = 0; y < 86 - 76; y++) {
		      f3d_lcd_pushColor(colors, 1);
		    }
		  }
		  
		}
		else if(axis == 'z') {
		  val = (int)(test[2]);
		  sprintf(temp, "z: %d", val);
		  f3d_lcd_drawString(0,0,temp,WHITE,BLACK);
		  //draw bar
		  f3d_lcd_setAddrWindow(76,76,86,(76 + val), MADCTLGRAPHICS);
		  uint8_t x, y;
		  for(x = 0; x < val; x++) {
		    for(y = 0; y < 86 - 76; y++) {
		      f3d_lcd_pushColor(colors, 1);
		    }
		  }
		}
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
