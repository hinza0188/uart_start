#include "Timer.h"
#include "stm32l476xx.h"
#include "UART.h"
#include <string.h>

char yes[] = "yes\r\n";
char no[] = "x\r\n";
char stop[] = "Timer has stopped!\r\n\r\n";

/*
* Initialize GPIO Port A into Alternate Function Mode
*/
void GPIO_Init() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable GPIO Port A
	GPIOA->MODER |= 0x2; 								// Set Port A to Alternate Function Mode
	//GPIOA->AFR[0] = GPIO_AFRL_AFRL1;		// Now the Register is set to AFSEL1
	GPIOA->AFR[0] = 0x1;								// Assign AF 1 Register for timer 2 connection
	GPIOA->PUPDR = 0x2;									// Don't know what this does yet
}

/*
* Initialize Timer 2 into Compare and Capture Mode
*/
void Timer_Init() {	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable Timer 2 Clock
	TIM2->PSC &= 80;											// 80Mhz CPU Pre-Scalar
	TIM2->CCER |= TIM_CCER_CC1NE;  // disable Timer 2 output register
	TIM2->CCMR1 |= TIM_CCMR1_CC1S_1; // the channel is now input, CCR1 register is now read-only
	TIM2->CCER |= TIM_CCER_CC1E;		// enable Timer 2 output register
}


void run_timer() {
	TIM2->CR1 |= 0x1; // start input capturing
	while (1) {
		//TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
		if (capture_event()) {
			USART_Write(USART2, (uint8_t *)yes, strlen(yes));
			break;
		} else{ 
			USART_Write(USART2, (uint8_t *)no, strlen(no));
		}
	}
	TIM2->CR1 |= 0; // clear the CR1 bit to stop timer
	USART_Write(USART2, (uint8_t *)stop, strlen(stop));
}

int capture_event(void){
	while (TIM2->SR == TIM_SR_CC1IF);	// captured an event!
	return TIM2->CCR1;
}
