/* main.c ---
*
* Filename: main.c
* Description:
* Author:
* Maintainer:
* Created: Thu Jan 10 11:23:43 2013
/* Code: */

/*
Colin Curry | colcurry
Ian Polito | ipolito
C335
Lab 8
*/

#include <stm32f30x.h> // Pull in include files for F30x standard drivers
#include <f3d_uart.h>
#include <f3d_user_btn.h>
#include <f3d_gyro.h>
#include <f3d_lcd_sd.h>
#include <f3d_i2c.h>
#include <f3d_accel.h>
#include <f3d_mag.h>
#include <f3d_nunchuk.h>
#include <stdio.h>
#include <math.h>

#define TIMER 20000
#define PI 3.14152965

int main(void) {
	setvbuf(stdin, NULL, _IONBF, 0);
   	setvbuf(stdout, NULL, _IONBF, 0);
   	setvbuf(stderr, NULL, _IONBF, 0);
	f3d_uart_init();
	f3d_gyro_init();
	f3d_i2c1_init();
	delay(10);
	f3d_accel_init();
	delay(10);
	f3d_mag_init();
	delay(10);
	//	printf("loading nunchuk");
	f3d_nunchuk_init();
	//	printf("nunchuk loaded");
	
	nunchuk_t test;
	float angle[3] = {};
        float compass[3] = {};
        float tilt[2] = {};
        float heading[3] = {};
        float direction = 0;
	float gyro[3] = {};
        float pitch; 
        float roll;
	
	float pre_pitch;
	float pre_roll;
	float pre_dir;
	float pre_gyro[3] = {};
	
        float heading_angle;
        f3d_lcd_init();
        f3d_lcd_fillScreen(RED);
        f3d_user_btn_init();
        int visual = 0;
        char str[30];   
        uint8_t y;
        uint16_t colors[10];
	uint16_t red[10];
        int i;
        for (i = 0; i < 10; i++) {
                colors[i] = WHITE;
		red[i] = RED;
        }
        double dir = 0;
	
	while(1) {
		f3d_nunchuk_read(&test);
		//check if display type should be changed
		if (test.c == 1 || test.jx > 147) {
			//go to the next type
			visual++;
			if (visual == 4) {
				visual = 0;
			}
			f3d_lcd_fillScreen(RED);
		} else if (test.z == 1 || test.jx < 107) {
			//go to the previous type
			visual--;
			if (visual == -1) {
				visual = 3;
			}
			f3d_lcd_fillScreen(RED);
		}
		//draw the correct display to lcd screen
/*gyro*/	if (visual == 0) {
			//draw the gyro display
		        pre_gyro[0] = gyro[0];
			pre_gyro[1] = gyro[1];
			pre_gyro[2] = gyro[2];
			f3d_gyro_getdata(gyro);
			sprintf(str, "Gyro x:%+.3f y:%+.3f z:%+.3f", gyro[0], gyro[1], gyro[2]);
                        f3d_lcd_drawString(0,0,str,WHITE,BLACK);
			//draw axis x
			//scale the values
			gyro[0] = (int)(gyro[0] / 5);
			gyro[1] = (int)(gyro[1] / 5);
			gyro[2] = (int)(gyro[2] / 5);
			//pre_gyro[0] = pre_gyro[0] / 5;
			//pre_gyro[1] = pre_gyro[1] / 5;
			//pre_gyro[2] = pre_gyro[2] / 5;
			//	printf("pre_gyro: %f\n", pre_gyro[0]);
			//	printf("gyro: %f\n", gyro[0]);
			if(gyro[0] == 0) {
			  f3d_lcd_setAddrWindow(5,20,15,152,MADCTLGRAPHICS);
			  for (y = 20; y < 152; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
		        if(gyro[1] == 0) {
			  f3d_lcd_setAddrWindow(20,20,30,152,MADCTLGRAPHICS);
			  for (y = 20; y < 152; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
			if(gyro[2] == 0) {
			  f3d_lcd_setAddrWindow(35,20,45,152,MADCTLGRAPHICS);
			  for (y = 20; y < 152; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
			if (gyro[0] != pre_gyro[0]) {
			  if (gyro[0] > 0 && pre_gyro[0] > 0 && pre_gyro[0] > gyro[0]) {
			    f3d_lcd_setAddrWindow(5,76-pre_gyro[0],15,76-gyro[0], MADCTLGRAPHICS);
			    for (y = 0; y < (pre_gyro[0] - gyro[0]); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			  } else if (gyro[0] < 0 && pre_gyro[0] < 0 && pre_gyro[0] < gyro[0]) {
			    pre_gyro[0] *= -1;
			    gyro[0] *= -1;
			    f3d_lcd_setAddrWindow(5,76+pre_gyro[0],15,76+gyro[0], MADCTLGRAPHICS);
			    for (y = 0; y < (gyro[0] + pre_gyro[0]); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			    pre_gyro[0] *= -1;
			    gyro[0] *= -1;
			  } else if (gyro[0] > 0 && pre_gyro[0] < 0) {
					pre_gyro[0] *= -1;
					f3d_lcd_setAddrWindow(5,76,15,76 + pre_gyro[0], MADCTLGRAPHICS);
                                        for (y = 0; y < pre_gyro[0]; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
					pre_gyro[0] *= -1;
				} else if (gyro[0] < 0 && pre_gyro[0] > 0) {
					f3d_lcd_setAddrWindow(5,76 - pre_gyro[0],15,76, MADCTLGRAPHICS);
                                        for (y = 0; y < pre_gyro[0]; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
				}
			}
			delay(10);
			if (gyro[0] <= 0) {
                                gyro[0] *= -1;   
                                f3d_lcd_setAddrWindow(5,76,15,(76 + gyro[0]), MADCTLGRAPHICS);
                                for (y = 0; y < gyro[0]; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
				gyro[0] *= -1;
                        } else {
                                f3d_lcd_setAddrWindow(5,(76 - gyro[0]),15,76, MADCTLGRAPHICS);
                                for (y = 0; y < gyro[0]; y++) {
                                        f3d_lcd_pushColor(colors, 10); 
                                }
                        }
			if ((int)gyro[1] != (int)pre_gyro[1]) {
                                if (gyro[1] > 0 && pre_gyro[1] > 0 && pre_gyro[1] > gyro[1]) {
			    f3d_lcd_setAddrWindow(20,76-pre_gyro[1],30,76-gyro[1], MADCTLGRAPHICS);
			    for (y = 0; y < (pre_gyro[1] - gyro[1]); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			  } else if (gyro[1] < 0 && pre_gyro[1] < 0 && pre_gyro[1] < gyro[1]) {
			    pre_gyro[1] *= -1;
			    gyro[1] *= -1;
			    f3d_lcd_setAddrWindow(20,76+pre_gyro[1],30,76+gyro[1], MADCTLGRAPHICS);
			    for (y = 0; y < (gyro[1] + pre_gyro[1]); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			    pre_gyro[1] *= -1;
			    gyro[1] *= -1;
			  } else if (gyro[1] > 0 && pre_gyro[1] < 0) {
					pre_gyro[1] *= -1;
					f3d_lcd_setAddrWindow(20,76,30,76 + pre_gyro[1], MADCTLGRAPHICS);
                                        for (y = 0; y < pre_gyro[1]; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
					pre_gyro[1] *= -1;
				} else if (gyro[1] < 0 && pre_gyro[1] > 0) {
					f3d_lcd_setAddrWindow(20,76 - pre_gyro[0],30,76, MADCTLGRAPHICS);
                                        for (y = 0; y < pre_gyro[1]; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
				}
                        }
			delay(10);
			//draw axis y
			if (gyro[1] <= 0) {
                                gyro[1] *= -1;
                                f3d_lcd_setAddrWindow(20,76,30,(76 + gyro[1]), MADCTLGRAPHICS);
                                for (y = 0; y < gyro[1]; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
				gyro[1] *= -1;
                        } else {
                                f3d_lcd_setAddrWindow(20,(76 - gyro[1]),30,76, MADCTLGRAPHICS);
                                for (y = 0; y < gyro[1]; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
                        }
			if ((int)gyro[2] != (int)pre_gyro[2]) {
			  if (gyro[2] > 0 && pre_gyro[2] > 0 && pre_gyro[2] > gyro[2]) {
			    f3d_lcd_setAddrWindow(35,76-pre_gyro[2],45,76-gyro[2], MADCTLGRAPHICS);
			    for (y = 0; y < (pre_gyro[2] - gyro[2]); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			  } else if (gyro[2] < 0 && pre_gyro[2] < 0 && pre_gyro[2] < gyro[2]) {
			    pre_gyro[2] *= -1;
			    gyro[2] *= -1;
			    f3d_lcd_setAddrWindow(35,76+pre_gyro[2],45,76+gyro[2], MADCTLGRAPHICS);
			    for (y = 0; y < (gyro[2] + pre_gyro[2]); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			    pre_gyro[2] *= -1;
			    gyro[2] *= -1;
			  } else if (gyro[2] > 0 && pre_gyro[2] < 0) {
					pre_gyro[2] *= -1;
					f3d_lcd_setAddrWindow(35,76,45,76 + pre_gyro[2], MADCTLGRAPHICS);
                                        for (y = 0; y < pre_gyro[2]; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
					pre_gyro[2] *= -1;
				} else if (gyro[2] < 0 && pre_gyro[2] > 0) {
					f3d_lcd_setAddrWindow(35,76 - pre_gyro[2],45,76, MADCTLGRAPHICS);
                                        for (y = 0; y < pre_gyro[2]; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
				}
                        }
			delay(10);
			//draw axis z
			if (gyro[2] <= 0) {
                                gyro[2] *= -1;
                                f3d_lcd_setAddrWindow(35,76,45,(76 + gyro[2]), MADCTLGRAPHICS);
                                for (y = 0; y < gyro[2]; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
				gyro[2] *= -1;
                        } else {
                                f3d_lcd_setAddrWindow(35,(76 - gyro[2]),45,76, MADCTLGRAPHICS);
                                for (y = 0; y < gyro[2]; y++) {
                                        f3d_lcd_pushColor(colors, 10);
                                }
                        }
/*pitch-roll*/	}
	 else if (visual == 1) {
	   //draw the board pitch-roll
	   pre_pitch = pitch;
	   pre_roll = roll;
			f3d_accel_read(angle);
                        tilt[0] = atan2(
                                angle[0],
                                sqrt(pow(angle[1],2) + pow(angle[2],2))
                        );
                        pitch = (int)(tilt[0] * (180 / PI));
			
                        tilt[1] = atan2(
                                angle[1],
                                sqrt(pow(angle[0],2) + pow(angle[2],2))
                        );
                        roll = (int)(tilt[1] * (180 / PI));
			
                        sprintf(str, "Pitch: %+.3f, Roll: %+.3f", pitch, roll);
                        f3d_lcd_drawString(0,0,str,WHITE,BLACK);
			if(pitch == 0) {
			  f3d_lcd_setAddrWindow(5,20,15,152,MADCTLGRAPHICS);
			  for (y = 20; y < 152; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
			if(roll == 0) {
			  f3d_lcd_setAddrWindow(20,20,30,152,MADCTLGRAPHICS);
			  for (y = 20; y < 152; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
                        //pitch
			if ((int)pitch != (int)pre_pitch) {
				if (pitch > 0 && pre_pitch > 0 && pre_pitch > pitch) {
				  f3d_lcd_setAddrWindow(5,76-pre_pitch,15,76-pitch, MADCTLGRAPHICS);
				  for (y = 0; y < (pre_pitch - pitch); y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				} else if (pitch < 0 && pre_pitch < 0 && pre_pitch < pitch) {
				  pre_pitch *= -1;
				  pitch *= -1;
				  f3d_lcd_setAddrWindow(5,76+pre_pitch,15,76+pitch, MADCTLGRAPHICS);
				  for (y = 0; y < (pitch + pre_pitch); y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				  pre_pitch *= -1;
				  pitch *= -1;
				} else if (pitch > 0 && pre_pitch < 0) {
				  pre_pitch *= -1;
				  f3d_lcd_setAddrWindow(5,76,15,76 + pre_pitch, MADCTLGRAPHICS);
				  for (y = 0; y < pre_pitch; y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				  pre_pitch *= -1;
				} else if (pitch < 0 && pre_pitch > 0) {
				  f3d_lcd_setAddrWindow(5,76 - pre_pitch,15,76, MADCTLGRAPHICS);
				  for (y = 0; y < pre_pitch; y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				}
                        }
			delay(10);
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
			if ((int)roll != (int)pre_roll) {
			  if (roll > 0 && pre_roll > 0 && pre_roll > roll) {
			    f3d_lcd_setAddrWindow(20,76-pre_roll,30,76-roll, MADCTLGRAPHICS);
			    for (y = 0; y < (pre_roll - roll); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			  } else if (roll < 0 && pre_roll < 0 && pre_roll < roll) {
			    pre_roll *= -1;
			    roll *= -1;
			    f3d_lcd_setAddrWindow(20,76+pre_roll,30,76+roll, MADCTLGRAPHICS);
			    for (y = 0; y < (roll + pre_roll); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			    pre_roll *= -1;
			    roll *= -1;
			  } else if (roll > 0 && roll < 0) {
					pre_roll *= -1;
					f3d_lcd_setAddrWindow(20,76,30,76 + pre_roll, MADCTLGRAPHICS);
                                        for (y = 0; y < pre_roll; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
					pre_roll *= -1;
				} else if (roll < 0 && pre_roll > 0) {
					f3d_lcd_setAddrWindow(20,76 - pre_roll,30,76, MADCTLGRAPHICS);
                                        for (y = 0; y < pre_roll; y++) {
                                                f3d_lcd_pushColor(red, 10);
                                        }
				}
                        }
			delay(10);
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
/*compass*/	} else if (visual == 2) {
			//draw the compass heading
pre_dir = direction;
			f3d_mag_read(compass);
                        heading[0] = (compass[0] * cosf(pitch) + compass[2] * sinf(pitch));
                        heading[1] = (compass[0] * sinf(roll) * sinf(pitch) + compass[1] * cosf(roll) - compass[2] * sinf(roll) * cosf(pitch));
                        direction = atan2(heading[1], heading[0]); //in radians
			//check if direction has changed for more efficient drawing
			printf("pre_dir: %.3f\n", pre_dir);
			printf("dir: %.3f\n", direction);
			if (floorf(direction * 100)/100 != floorf(pre_dir * 100)/100) {
			  f3d_lcd_setAddrWindow(76+(cos(pre_dir)*20),76+(sin(pre_dir)*20),76+(cos(pre_dir)*20)+2, 76+(sin(pre_dir)*20)+2, MADCTLGRAPHICS);
			  for (y = 0; y < 2; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
			
                        heading_angle = direction * (180 / PI); //in degrees
                        sprintf(str, "Compass Heading: %+.3f", heading_angle);
                        f3d_lcd_drawString(0,0,str,WHITE,BLACK);
                        //compass center
			f3d_lcd_setAddrWindow(75,75,77,77, MADCTLGRAPHICS);
                        for (y = 0; y < 3; y++) {
                        	f3d_lcd_pushColor(colors, 10);
                        }
			//compass direction
                        f3d_lcd_setAddrWindow(76+(cos(direction)*20),76+(sin(direction)*20),76+(cos(direction)*20)+2, 76+(sin(direction)*20)+2, MADCTLGRAPHICS);
                        for (y = 0; y < 2; y++) {
                                f3d_lcd_pushColor(colors, 10);
                        }
/*nun tilt*/	} else if (visual == 3) {
			//draw the nunchuk tilt angle (ax, ay, az)
	   pre_pitch = pitch;
	   pre_roll = roll;
	   f3d_nunchuk_read(&test);
	   //printf("jx: %u\n", test.jx);
                	//printf("jy: %u\n", test.jy);
                	//printf("ax: %u\n", test.ax);
                	//printf("ay: %u\n", test.ay);
                	//printf("az: %u\n", test.az);   
                	//printf("c: %u\n", test.c);
                	//printf("z: %u\n", test.z);
			tilt[0] = atan2(
                                test.ax,
                                sqrt(pow(test.ay,2) + pow(test.az,2))
                        );
                        pitch = (int)(tilt[0] * (180 / PI));
                        tilt[1] = atan2(
                                test.ay,
                                sqrt(pow(test.ax,2) + pow(test.az,2))
                        );
                        roll = (int)(tilt[1] * (180 / PI));
                        sprintf(str, "Pitch: %+.3f, Roll: %+.3f", pitch, roll);
                        f3d_lcd_drawString(0,0,str,WHITE,BLACK);
			if(pitch == 0) {
			  f3d_lcd_setAddrWindow(5,20,15,152,MADCTLGRAPHICS);
			  for (y = 20; y < 152; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
			if(roll == 0) {
			  f3d_lcd_setAddrWindow(20,20,30,152,MADCTLGRAPHICS);
			  for (y = 20; y < 152; y++) {
			    f3d_lcd_pushColor(red, 10);
			  }
			}
			if ((int)pitch != (int)pre_pitch) {
                                if (pitch > 0 && pre_pitch > 0 && pre_pitch > pitch) {
				  f3d_lcd_setAddrWindow(5,76-pre_pitch,15,76-pitch, MADCTLGRAPHICS);
				  for (y = 0; y < (pre_pitch - pitch); y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				} else if (pitch < 0 && pre_pitch < 0 && pre_pitch < pitch) {
				  pre_pitch *= -1;
				  pitch *= -1;
				  f3d_lcd_setAddrWindow(5,76+pre_pitch,15,76+pitch, MADCTLGRAPHICS);
				  for (y = 0; y < (pitch + pre_pitch); y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				  pre_pitch *= -1;
				  pitch *= -1;
				} else if (pitch > 0 && pre_pitch < 0) {
				  pre_pitch *= -1;
				  f3d_lcd_setAddrWindow(5,76,15,76 + pre_pitch, MADCTLGRAPHICS);
				  for (y = 0; y < pre_pitch; y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				  pre_pitch *= -1;
				} else if (pitch < 0 && pre_pitch > 0) {
				  f3d_lcd_setAddrWindow(5,76 - pre_pitch,15,76, MADCTLGRAPHICS);
				  for (y = 0; y < pre_pitch; y++) {
				    f3d_lcd_pushColor(red, 10);
				  }
				}
                        }
                        delay(10);
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
			if ((int)roll != (int)pre_roll) {
			  if (roll > 0 && pre_roll > 0 && pre_roll > roll) {
			    f3d_lcd_setAddrWindow(20,76-pre_roll,30,76-roll, MADCTLGRAPHICS);
			    for (y = 0; y < (pre_roll - roll); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			  } else if (roll < 0 && pre_roll < 0 && pre_roll < roll) {
			    pre_roll *= -1;
			    roll *= -1;
			    f3d_lcd_setAddrWindow(20,76+pre_roll,30,76+roll, MADCTLGRAPHICS);
			    for (y = 0; y < (roll + pre_roll); y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			    pre_roll *= -1;
			    roll *= -1;
			  } else if (roll > 0 && roll < 0) {
			    pre_roll *= -1;
			    f3d_lcd_setAddrWindow(20,76,30,76 + pre_roll, MADCTLGRAPHICS);
			    for (y = 0; y < pre_roll; y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			    pre_roll *= -1;
			  } else if (roll < 0 && pre_roll > 0) {
			    f3d_lcd_setAddrWindow(20,76 - pre_roll,30,76, MADCTLGRAPHICS);
			    for (y = 0; y < pre_roll; y++) {
			      f3d_lcd_pushColor(red, 10);
			    }
			  }
                        }
			delay(10);
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
