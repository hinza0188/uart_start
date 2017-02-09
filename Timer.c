#include "Timer.h"
#include "stm32l476xx.h"


void PA0_Init() {
	// Initialize GPIO_PA0
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER |= 2; // set it into Alternate Function Mode
}

void Timer_Init() {
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable Timer 2 Clock
	
}


void run_timer() {
	return;
}

void stop_timer() {
	
	return;
}
