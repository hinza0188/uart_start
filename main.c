#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "Timer.h"      // added timer function
#include "input_pa0.h"  // added GPIO pa0

#include <string.h>
#include <stdio.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char str[] = "Give Red LED control input (Y = On, N = off):\r\n";

int main(void){
    char rxByte;
	int		 a ;
	int		 n ;
	int		 i ;
	float    b ;
	
	/**
	* start timer 2 
	* copy timer when interrupt happens
	* then take the differences of the timer
	
	* use pa0 make it input line
	* capture event on timer 2
	
	* run 1 second -> million micro second (worry about overflow)
	*/
    // init PA0 to be input mode
    // have loop code to look for input data register (IDR) (GPIOA_IDR)
	// change

	System_Clock_Init(); // Switch System Clock = 80 MHz
    PA0_Init();
	LED_Init();
	UART2_Init();
	
	while (1){
		USART_Write(USART2, (uint8_t *)str, strlen(str));	
   	    rxByte = USART_Read(USART2);
   	    if (rxByte == 'N' || rxByte == 'n'){
			Red_LED_Off();
			USART_Write(USART2, (uint8_t *)"LED is Off\r\n\r\n", 16);
			// run timer code here
			run_timer();
			USART_Write(USART2, (uint8_t *)"Timer is on!\r\n\r\n", 18);
		} else if (rxByte == 'Y' || rxByte == 'y'){
			Red_LED_On();
			USART_Write(USART2, (uint8_t *)"LED is on\r\n\r\n", 15);
			// stop timer code here
			stop_timer();
			USART_Write(USART2, (uint8_t *)"Timer is off!\r\n\r\n", 19);
		}
	}
}

