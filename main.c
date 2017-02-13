#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "Timer.h"      // added timer function

#include <string.h>
#include <stdio.h>

char RxComByte = 0;
uint8_t buffer[BufferSize];
char welcome[] = "Welcome to the Project 1! :)\r\n";
char inst_1[] = "Press 1 to start timer\r\n";
char running[] = "Timer is running!\r\n";
char dude[] = "Unexpected input!\r\n";
char stop[] = "Timer has stopped!\r\n";
char e_prompt[] = "Press 0 to start again!\r\n\r\n";

int main(void){
  char rxByte;
	
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

	System_Clock_Init();		// Switch System Clock = 80 MHz
	UART2_Init();
    GPIO_Init();
	
	while (1){
        USART_Write(USART2, (uint8_t *)welcome, strlen(welcome));	
		USART_Write(USART2, (uint8_t *)inst_1, strlen(inst_1));	
        rxByte = USART_Read(USART2);
        if (rxByte == '1'){ // input is 1 for start the timer!
			USART_Write(USART2, (uint8_t *)running, strlen(running));
            TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
			run_timer();
			USART_Write(USART2, (uint8_t *)stop, strlen(stop));
		} else if (rxByte == '2') {
            char a[] = "running POST\r\n";
            USART_Write(USART2, (uint8_t *)a, strlen(a));
            POST();
        } else {
			USART_Write(USART2, (uint8_t *)dude, strlen(dude));
		}
        if (rxByte == '0') {
            USART_Write(USART2, (uint8_t *)e_prompt, strlen(e_prompt));
        }
	}
}

