/************************
 *f3d_led.c - contains intializations/functions for the leds
 ************************/

#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>
#include <f3d_led.h>


//intializes the port and pins for the leds on the board
void f3d_led_init(void) {
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOE, ENABLE);
  	GPIO_InitTypeDef GPIO_InitStructure;
  	GPIO_StructInit(&GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | 
GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | 
GPIO_Pin_15;  
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;   
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}
/*Turns on the appropriate led as specified by the parameter.*/
void f3d_led_on(int led) {
	if (led == 0) {
		GPIOE->BSRR = GPIO_Pin_9;
	} else if (led == 1) {
		GPIOE->BSRR = GPIO_Pin_10;
	} else if (led == 2) {
		GPIOE->BSRR = GPIO_Pin_11;
	} else if (led == 3) {
		GPIOE->BSRR = GPIO_Pin_12;
	} else if (led == 4) {
		GPIOE->BSRR = GPIO_Pin_13;
	} else if (led == 5) {
		GPIOE->BSRR = GPIO_Pin_14;
	} else if (led == 6) {
		GPIOE->BSRR = GPIO_Pin_15;
	} else if (led == 7) {
		GPIOE->BSRR = GPIO_Pin_8;
	}
}

/*Turns off the approiate led as specified by the parameter*/ 
void f3d_led_off(int led) {
	if (led == 0) {
                GPIOE->BRR = GPIO_Pin_9;
        } else if (led == 1) {
                GPIOE->BRR = GPIO_Pin_10;
        } else if (led == 2) {
                GPIOE->BRR = GPIO_Pin_11;
        } else if (led == 3) {
                GPIOE->BRR = GPIO_Pin_12;
        } else if (led == 4) {
                GPIOE->BRR = GPIO_Pin_13;
        } else if (led == 5) {
                GPIOE->BRR = GPIO_Pin_14;
        } else if (led == 6) {
                GPIOE->BRR = GPIO_Pin_15;
        } else if (led == 7) {
                GPIOE->BRR = GPIO_Pin_8;
        }
} 

/*Turns on all LEDs*/
void f3d_led_all_on(void) {
	GPIOE->BSRR = GPIO_Pin_9;
	GPIOE->BSRR = GPIO_Pin_10;
	GPIOE->BSRR = GPIO_Pin_11;
	GPIOE->BSRR = GPIO_Pin_12;
	GPIOE->BSRR = GPIO_Pin_13;
	GPIOE->BSRR = GPIO_Pin_14;
	GPIOE->BSRR = GPIO_Pin_15;
	GPIOE->BSRR = GPIO_Pin_8;
} 

/*Turns off all LEDs*/
void f3d_led_all_off(void) {
	GPIOE->BRR = GPIO_Pin_9;
        GPIOE->BRR = GPIO_Pin_10;       
        GPIOE->BRR = GPIO_Pin_11;
        GPIOE->BRR = GPIO_Pin_12;
        GPIOE->BRR = GPIO_Pin_13;
        GPIOE->BRR = GPIO_Pin_14;
        GPIOE->BRR = GPIO_Pin_15;
        GPIOE->BRR = GPIO_Pin_8;
} 

/* led.c ends here */

