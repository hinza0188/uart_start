#include "Timer.h"
#include "stm32l476xx.h"
#include "UART.h"
#include <stdio.h>
#include <string.h>

#define bSize 100
#define numBucket 101

char yes[] = "yes\r\n";
char no[] = "x\r\n";

char post_i[] = "Initializing Power On Slef Test(POST)\r\n";
char post_s[] = "POST: SUCCESS |  \r\n\r\n";
char post_f[] = "Post Failed :, Try again? (Y/N)\r\n";
char done[] = "Histogram Generated\r\n";

static char timerBuffer[bSize];

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
	TIM2->PSC = 79;											// Set 80Mhz CPU Pre-Scalar
	TIM2->EGR |= 0x01;											// Reset TIM2 for applying PSC
	TIM2->CCER &= 0x1;										// disable Timer 2 output register
	TIM2->CCMR1 |= 0x1;										// the channel is now input, CCR1 register is now read-only
	TIM2->CCER |= 0x1;										// enable Timer 2 output register
  TIM2->DIER = TIM_DIER_BIE;                              // Enable update interrupt (timer level)
  NVIC_EnableIRQ(TIM2_IRQn);                              // Enable interrupt from TIM2 (NVIC level)
}


void TIM2_IRQHandler() {
	if (TIM2->SR & TIM_SR_CC1IF) {
        int n = sprintf(timerBuffer, "Result: %u\r\n", TIM2->CCR1);
        USART_Write(USART2, (uint8_t *)timerBuffer, n);
	}
}


void histogram(int l_lim, int u_lim) {
	int buck[numBucket] =  0;
	int old_count = 0;
	int new_count = 0;
	int check;
	int i;
	int j;
	
	Timer_Init();
	TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
	for (i=0; i<1000; i++) { // capture 1001 measurements

		while (!(TIM2->SR & TIM_SR_CC1IF)); // wait until the first event has occured
		old_count = TIM2->CCR1; 	// copy the new one to old
		while (!(TIM2->SR & TIM_SR_CC1IF));
		new_count = TIM2->CCR1; // The first captured timer count
		
		// now check the values if its in the range
		check = (new_count - old_count - l_lim);
		
		if (check >= numBucket || check < 0) {
			i--;				// decrement the repeat count
			continue;		// forget about this iteration
		}
		
		// the index value has been sanitized
		buck[check]++;		// increment the count of selected index
	}
	
	// print the header for the answer
	USART_Write(USART2, (uint8_t *)done, strlen(done));
		
	for(j=0; j<numBucket; j++) {
		if (buck[j] > 0 ) { // check bucket if 
			int ans = sprintf(timerBuffer, "Time: %u | Count: %u\r\n", l_lim+j, buck[j]); 
			USART_Write(USART2, (uint8_t *)timerBuffer, ans);
		}
	}
	TIM2->CR1 &= 0x0; // stop input capturing
}


int POST() {
    // start timer, and check the counter value
    USART_Write(USART2, (uint8_t *)post_i, strlen(post_i));
    Timer_Init();
    TIM2->CR1 |= TIM_CR1_CEN; // start input capturing
    while(!(TIM2->SR & TIM_SR_CC1IF)) {  // repeat this code block until we find the interrupt
			if (TIM2->CNT > (100*1000)) { // wait for 100,000 micro-second
					// fail case
					USART_Write(USART2, (uint8_t *)post_f, strlen(post_f));
					TIM2->CR1 &= 0x0; // clear the CR1 bit to stop timer
					return 1; // indicate failure
			}
		}
		// success case
    USART_Write(USART2, (uint8_t *)post_s, strlen(post_s));
    TIM2->CR1 &= 0x0; // clear the CR1 bit to stop timer
    return 0;
}
