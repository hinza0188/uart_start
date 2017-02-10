#include "stm32l4xx.h"
void  GPIO_config()
{
	
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;
	GPIOA->MODER |= 0;
	GPIOA->MODER |= 2;
	GPIOA->AFR[0] = 0x10;
}


void timer_cofig()
{
	///enabling the system clock for the timer
		 RCC->APB1ENR1 = RCC_APB1ENR1_TIM2EN ;		 
	///loading the prescalar vlaue into the psc register                 // Device header
		 TIM2->PSC = 80;
	///how to force new load to the prescalar value by creating an update???
	
}
		 
void timer_capture_config()
	{
		
		TIM2->CCMR1 = TIM_CCMR1_CC1S_1;
		TIM2->CCER= TIM_CCER_CC1NE; ///DISABLE THE CA
		TIM2->CCER= TIM_CCER_CC1E; ///RE-ENBALE CAPTURE INPUT.
		
	}
	 
	
	int timer_operation()
	{
	TIM2->CR1= TIM_CR1_CEN
	;////enable's input capture 
	/////HOW TO CLEAR THIS BIT TO STOP INPUT CAPTURE
		if( TIM_SR_CC1IF ==1)
		{
		return TIM2  ->CCR1;   ///reading the captured counter value from the TIMx->CCRx register	
	}
	
		
		