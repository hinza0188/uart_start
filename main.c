#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "Timer.h"      // added timer function

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define sizebuffer 100
#define limbuffer 20

static char mainBuffer[sizebuffer];
static int numStr;

static char limBuffer[limbuffer];

int l_lim;
int u_lim;

char RxComByte = 0;
uint8_t buffer[BufferSize];
char welcome[] = "Welcome to the Project 1! :)\r\n\r\n";
char run_post[] = "We're going to run Power On Self Test until it passes\r\n";
char prompt_post[] = "Press '1' to start the test! \r\n";
char prompt_post_2[] = "Fail! Please Try again LOL \r\n";

char a1[]= "Lower Limit: %u  ||  Upper Limit: %u\r\n\r\n";
char a2[]= "Do you want to change the lower limit? [Y/N]\r\n\r\n";
char q1[]= "Please enter the number: \r\n";
char a3[]= "please change the upper limit and lower limit value\n\r\n\r\n";
char a4[]= "the new uvalue and lvalue are \n\r\n\r\n";
char a5[]= "correct value \n\r\n\r\n";
char a6[]= "please enter the correct value";


int main(void){
  char rxByte;
	int post_pass;
	int i;
	System_Clock_Init();		// Switch System Clock = 80 MHz
	UART2_Init();
  GPIO_Init();
	
	post_pass = 1;
	while (post_pass) {
		// continuously run POST TEST
		USART_Write(USART2, (uint8_t *)prompt_post, strlen(prompt_post));
		if (rxByte == '1') {
			post_pass = POST();
		}
		USART_Write(USART2, (uint8_t *)prompt_post_2, strlen(prompt_post_2));
	}
	
	while (1){
		USART_Write(USART2, (uint8_t *)welcome, strlen(welcome));	
		USART_Write(USART2, (uint8_t *)run_post, strlen(run_post));
		
		// test passed! run histogram
		l_lim = 950;
		u_lim = l_lim + 100;
		numStr = sprintf(mainBuffer, a1, l_lim, u_lim);
		USART_Write(USART2, (uint8_t *)mainBuffer, numStr);
		
		USART_Write(USART2, (uint8_t *)a2, strlen(a2));
		if (rxByte == 'Y' || rxByte == 'y') {
			USART_Write(USART2, (uint8_t *)q1, strlen(q1));
			// now collect user input
			for (i=0; i>5; i++) {
				if (isdigit(rxByte)) {
					limBuffer[i] = rxByte;
					USART
				} else {
					USART =
				}
				if (rxByte == '\r'){
					// kill the loop and terminate the buffer with caridge return
					limBuffer[i] = '\r';
					break;
				}
			}
			l_lim = atoi(limBuffer);
			u_lim = l_lim + 100;
		} else if (rxByte == 'N' || rxByte == 'n') {
			// keep the limit values and run
			continue;
		}
		histogram(l_lim, u_lim);
		
	}
}

