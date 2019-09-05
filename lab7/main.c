/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

/*
Ian Polito
ipolito
C335
Lab 7
Lab Partner: Colin Curry | colcurry
*/

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000
#define PI 3.14159265

int main(void) {
	setvbuf(stdin, NULL, _IONBF, 0);
   	setvbuf(stdout, NULL, _IONBF, 0);
   	setvbuf(stderr, NULL, _IONBF, 0);
	f3d_uart_init();
	f3d_i2c1_init();
	delay(10);
	f3d_accel_init();
	delay(10);
	f3d_mag_init();
	delay(10);
	
	float angle[3] = {};
	float compass[3] = {};
	float tilt[2] = {};
	float heading[3] = {};
	float direction = 0;
	
	float pitch;
	float roll;
	float heading_angle;
	f3d_lcd_init();
	f3d_lcd_fillScreen(RED);
	f3d_user_btn_init();
	char visual = 't';
	char str[30];
	uint8_t y;
	uint16_t colors[10];
	int i;
	for (i = 0; i < 10; i++) {
		colors[i] = WHITE;
	}
	double dir = 0;
	while(1) {
		//check if user button is pressed to switch visualizations
		if (user_btn_read()) {
			if (visual == 't') {
				visual = 'c';
			} else {
				visual = 't';
			}
		}
		if (visual == 't') {
			f3d_lcd_fillScreen(RED);
			//visualize title angle
			f3d_accel_read(angle);
			tilt[0] = atan2(
				angle[0],
		     		sqrt(pow(angle[1],2) + pow(angle[2],2))
		     	);
     			pitch = tilt[0] * (180 / PI);
     			tilt[1] = atan2(
		     		angle[1],
		     		sqrt(pow(angle[0],2) + pow(angle[2],2))
		     	);
     			roll = tilt[1] * (180 / PI);
			sprintf(str, "Pitch: %.3f, Roll: %.3f", pitch, roll);
			f3d_lcd_drawString(0,0,str,WHITE,BLACK);
			//pitch
			if (pitch <= 0) {
				pitch *= -1;
				f3d_lcd_setAddrWindow(5,76,15,(76 + pitch), MADCTLGRAPHICS);
				for (y = 0; y < pitch; y++) {
					f3d_lcd_pushColor(colors, 10);
				}
			} else {
				f3d_lcd_setAddrWindow(5,(76 - pitch),15,76, MADCTLGRAPHICS);
				for (y = 0; y < pitch; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
			}
			//roll
			if (roll <= 0) {
				roll *= -1;
                                f3d_lcd_setAddrWindow(20,76,30,(76 + roll), MADCTLGRAPHICS);
                                for (y = 0; y < roll; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
                        } else {
                                f3d_lcd_setAddrWindow(20,(76 - roll),30,76, MADCTLGRAPHICS);
                                for (y = 0; y < roll; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
                        }
			delay(10);
		} else {
			f3d_lcd_fillScreen(RED);
			//visualize compass
			f3d_mag_read(compass);
			delay(10);
			heading[0] = (compass[0] * cosf(pitch) + compass[2] * sinf(pitch));
     			heading[1] = (compass[0] * sinf(roll) * sinf(pitch) + compass[1] * cosf(roll) - compass[2] * sinf(roll) * cosf(pitch));
			direction = atan2(heading[1], heading[0]); //in radians
     			heading_angle = direction * (180 / PI); //in degrees
			
			sprintf(str, "Compass Heading: %.3f", heading_angle);
                        f3d_lcd_drawString(0,0,str,WHITE,BLACK);
			
			//compass direction
			f3d_lcd_setAddrWindow(76+(cos(direction)*20),76+(sin(direction)*20),76+(cos(direction)*20)+2,76+(sin(direction)*20)+2, MADCTLGRAPHICS);
                        for (y = 0; y < 2; y++) {
                        	f3d_lcd_pushColor(colors, 10);
                        }
			delay(10);
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

