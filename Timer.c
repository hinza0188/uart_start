#include "Timer.h"
#include "stm32l476xx.h"


void Timer_Init() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable GPIO Port A
	GPIOA->MODER |= 2; 										// Set Port A to Alternate Function Mode
	GPIOA->AFR[0] = GPIO_AFRL_AFRL1;					// Now the Register is set to AFSEL1
	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable Timer 2 Clock
	TIM2->PSC &= 80;
	
	TIM2->CCMR1 = TIM_CCMR1_CC1S_1; // the channel is now input, CCR1 register is now read-only
	
	TIM2->CCER = TIM_CCER_CC1NE;  // disable Timer 2 output enable register
	TIM2->CCER = TIM_CCER_CC1E;		// enable Timer 2 output enable register
	
	//TIM2->CCER |= 0; // turn off Timer 2 output enable resgister
	//TIM2->CCER |= 1;
	
}


int run_timer() {
	TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
	return 1;
}

int stop_timer() {
	TIM2->CR1 |= 0; // clear the CR1 
	return 0;
}

int capture_event(){
	while (TIM2->SR == TIM_SR_CC1IF);	// captured an event!
	return TIM2->CCR1;
}
