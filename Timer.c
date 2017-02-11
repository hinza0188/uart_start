#include "Timer.h"
#include "stm32l476xx.h"
#include "UART.h"

#include <string.h>
#include <stdio.h>

void Timer_Init() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable GPIO Port A
	GPIOA->MODER |= 2; 										// Set Port A to Alternate Function Mode
	GPIOA->AFR[0] = GPIO_AFRL_AFRL1;					// Now the Register is set to AFSEL1
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable Timer 2 Clock
	TIM2->PSC &= 80;											// 80Mhz CPU Pre-Scalar
	
	TIM2->CCER = TIM_CCER_CC1NE;  // disable Timer 2 output enable register
	
	TIM2->CCMR1 = TIM_CCMR1_CC1S_1; // the channel is now input, CCR1 register is now read-only
	
	TIM2->CCER = TIM_CCER_CC1E;		// enable Timer 2 output enable register
}


void run_timer() {
	int runCount = 1000; // hard coded testing round
	int result = 0;
	
	while (runCount >= 0) {
		//TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
		TIM2->CR1 |= 0x1; // start input capturing
		result = capture_event();
		runCount--;
	}
	TIM2->CR1 |= 0; // clear the CR1 bit to stop timer
}

int capture_event(void){
	while (TIM2->SR == TIM_SR_CC1IF);	// captured an event!
	return TIM2->CCR1;
}
