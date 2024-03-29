/************************** 
 *f3d_uart.c 
 *contains the initialization basic i/o functions for UART
****************************/ 

/* Code: */

#include <stm32f30x.h>
#include <f3d_uart.h>
//the initialization function to call
void f3d_uart_init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOC, ENABLE);
	
	//initialize TX Pin
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//initialize RX Pin
	GPIO_StructInit(&GPIO_InitStructure);
        GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
        GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
        GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
        GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//alternate function configuration
	GPIO_PinAFConfig(GPIOC,4,GPIO_AF_7);
	GPIO_PinAFConfig(GPIOC,5,GPIO_AF_7);
	
	//initialize UART
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	USART_StructInit(&USART_InitStructure);
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1 ,&USART_InitStructure);
	USART_Cmd(USART1 , ENABLE);
	
}
//sends a character
int putchar(int c) {
	while (USART_GetFlagStatus(USART1,USART_FLAG_TXE) == (uint16_t)RESET);
	USART_SendData(USART1, c);
	return 0;
} 
//gets a character
int getchar(void) {
	while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == (uint16_t)RESET);
	int c = USART_ReceiveData(USART1);
	return c;
}
//sends a string
void putstring(char *s) {
	char c = *s;
	while (c != '\0') {
		putchar(c);
		c = *(s++);
	}
}
/* f3d_uart.c ends here */
