#include "Timer.h"
#include "stm32l476xx.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

#define bSize 15

char yes[] = "yes\r\n";
char no[] = "x\r\n";

char post_i[] = "Initializing Power On Slef Test(POST)\r\n";
char post_s[] = "Found an interrupt signal within 100 ms! \r\n";
char post_f[] = "No singal detected within 100 ms, Try again? (Y/N)\r\n";

static char p[bSize];

/*
* Initialize GPIO Port A into Alternate Function Mode
*/
void GPIO_Init() {
	RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // Enable GPIO Port A
    GPIOA->MODER &= ~3;                                 // clear MODER Bytes
	GPIOA->MODER |= 0x2; 								// Set Port A to Alternate Function Mode
	GPIOA->AFR[0] |= 0x1;								// Assign AF 1 Register for timer 2 connection
	//GPIOA->PUPDR |= 0x2;									// Don't know what this does yet
}

/*
* Initialize Timer 2 into Compare and Capture Mode
*/
void Timer_Init() {	
	RCC->APB1ENR1 |= RCC_APB1ENR1_TIM2EN; // Enable Timer 2 Clock
	TIM2->PSC &= 80;											// Set 80Mhz CPU Pre-Scalar
	TIM2->EGR |= 0x01;											// Reset TIM2 for applying PSC
	TIM2->CCER &= 0x1;										// disable Timer 2 output register
	TIM2->CCMR1 |= 0x1;										// the channel is now input, CCR1 register is now read-only
	TIM2->CCER |= 0x1;										// enable Timer 2 output register
    TIM2->DIER = TIM_DIER_BIE;                              // Enable update interrupt (timer level)
    NVIC_EnableIRQ(TIM2_IRQn);                              // Enable interrupt from TIM2 (NVIC level)
}


void TIM2_IRQHandler() {
	if (TIM2->SR & TIM_SR_CC1IF) {
        int n = sprintf(p, "Result: %u\r\n", TIM2->CCR1);
        USART_Write(USART2, (uint8_t *)p, n);
	}
}


void run_timer() {
    int i;
    Timer_Init();
	TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
	for (i=0; i<1000; i++) {
        TIM2_IRQHandler();
	}
	TIM2->CR1 &= 0x0; // clear the CR1 bit to stop timer
}


int POST() {
    // start timer, and check the counter value
    USART_Write(USART2, (uint8_t *)post_i, strlen(post_i));
    Timer_Init();
    TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
    while(!(TIM2->SR & TIM_SR_CC1IF)) {        
        // success case
        int m = sprintf(p, "waiting at: %u\r\n", TIM2->CCR1);
        USART_Write(USART2, (uint8_t *)p, m);
        if (TIM2->CCR1 > 500000) {
            // fail case
            USART_Write(USART2, (uint8_t *)post_f, strlen(post_f));
            TIM2->CR1 &= 0x0; // clear the CR1 bit to stop timer
            return 0;
        }
    }
    USART_Write(USART2, (uint8_t *)post_s, strlen(post_s));
    TIM2->CR1 &= 0x0; // clear the CR1 bit to stop timer
    return 1;
}
