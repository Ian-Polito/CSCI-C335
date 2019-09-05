//main.c for lab6
#include <f3d_uart.h>
#include <stdio.h>
#include <f3d_gyro.h>
#include <f3d_led.h>
#include <f3d_user_btn.h>

int main(void){
  f3d_uart_init();
  f3d_gyro_init();
  f3d_user_btn_init();
  f3d_led_init();
  setvbuf(stdin, NULL, _IONBF, 0);
  setvbuf(stdout, NULL, _IONBF, 0);
  setvbuf(stderr, NULL, _IONBF, 0);
  
  char axis = 'x';
  char c = 'r';
  float test[3] = {1,2,3};
  while (1) {
    if (user_btn_read() != 0) {
      if (axis == 'x') {
	axis = 'y';
      } else if (axis == 'y') {
	axis = 'z';
      } else if (axis == 'z') {
	axis = 'x';
      }
    }
    //c = getchar();
    if (c == 'x') {
      axis = 'x';
    } else if (c == 'y') {
      axis = 'y';
    } else if (c == 'z') {
      axis = 'z';
    }
    f3d_gyro_getdata(test);
    if (axis == 'x') {
      printf("x: %f\n", test[0]);
    } else if (axis == 'y') {
      printf("y: %f\n", test[1]);
    } else {
      printf("z: %f\n", test[2]);
    }
    //change led's based on acceleration
    f3d_led_all_off();
    if (axis == 'x') {
	if (test[0] < 0) {
		if (test[0] < -5) {
			f3d_led_on(0);
		}
		if (test[0] < -10) {
			f3d_led_on(1);
		}
        	if (test[0] < -15) {
                	f3d_led_on(2);
        	}
        	if (test[0] < -20) {   
                	f3d_led_on(3);
        	}
		if (test[0] < -25) {
			f3d_led_on(4);
		}
	} else {
		if (test[0] > 5) {
			f3d_led_on(0);
		}
		if (test[0] > 10) {
			f3d_led_on(7);
		}
		if (test[0] > 15) {
                	f3d_led_on(6);
        	}
        	if (test[0] > 20) {
                	f3d_led_on(5);
        	}
        	if (test[0] > 25) {
                	f3d_led_on(4);
        	}
	}
    } else if (axis == 'y') {
	if (test[1] < 0) {
                if (test[1] < -5) {
                        f3d_led_on(0);
                }
                if (test[1] < -10) {
                        f3d_led_on(1);
                }
                if (test[1] < -15) {
                        f3d_led_on(2);
                }
                if (test[1] < -20) {
                        f3d_led_on(3);
                }
                if (test[1] < -25) {
                        f3d_led_on(4);
                }
        } else {
                if (test[1] > 5) {
                        f3d_led_on(0);
                }
                if (test[1] > 10) {
                        f3d_led_on(7);
                }
                if (test[1] > 15) {
                        f3d_led_on(6);
                }
                if (test[1] > 20) {
                        f3d_led_on(5);
                }
                if (test[1] > 25) {
                        f3d_led_on(4);
                }
        }
    } else {
	if (test[2] < 0) {
                if (test[2] < -5) {
                        f3d_led_on(0);
                }
                if (test[2] < -10) {
                        f3d_led_on(1);
                }
                if (test[2] < -15) {
                        f3d_led_on(2);
                }
                if (test[2] < -20) {
                        f3d_led_on(3);
                }
                if (test[2] < -25) {
                        f3d_led_on(4);
                }
        } else {
                if (test[2] > 5) {
                        f3d_led_on(0);
                }
                if (test[2] > 10) {
                        f3d_led_on(7);
                }
                if (test[2] > 15) {
                        f3d_led_on(6);
                }
                if (test[2] > 20) {
                        f3d_led_on(5);
                }
                if (test[2] > 25) {
                        f3d_led_on(4);
                }
        }
    }
  }
}

void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
