#include "stm32l476xx.h"
#include "SysClock.h"
#include "LED.h"
#include "UART.h"
#include "Timer.h"      // added timer function

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAIN_BUFFER_SIZE 100
#define LIMIIT_BUFFER_SIZE 5

static char mainBuffer[MAIN_BUFFER_SIZE];
static int numStr;

static char limBuffer[LIMIIT_BUFFER_SIZE];

int l_lim;
int u_lim;

char RxComByte = 0;
uint8_t buffer[BufferSize];
char welcome[] = "Welcome to the Project 1! :)\r\n\r\n";
char run_post[] = "We're going to run Power On Self Test until it passes\r\n";
char prompt_post[] = "Press '1' to start the test! \r\n";
char prompt_post_2[] = "Fail! Please Try again LOL \r\n";
char cont_prompt[] = "press any key to continue! \r\n";

char a1[]= "Lower Limit: %u  ||  Upper Limit: %u\r\n\r\n";
char a2[]= "Do you want to change the lower limit? [Y/N]\r\n\r\n";
char q1[]= "Please enter the number [50-9950]: \r\n";
char a3[]= "please change the upper limit and lower limit value\n\r\n\r\n";
char a4[]= "the new uvalue and lvalue are \n\r\n\r\n";
char a5[]= "correct value \n\r\n\r\n";
char a6[]= "please enter the correct value";

char nay[] = "Unexpected input! please type numbers only!\r\n";
char default_prompt[] = "Keep the default values for the lower | upper limits\r\n";
char end_promtp[] = "\r\nDo you want to run the program again? [Y/N]\r\n";
char terminate[] = "Program terminated\r\n";
char ob_promtp[] = "Lower bound went out of range! please try again";

int main(void){
  char rxByte;
	int post_pass;
	int i;
	int oo;
	System_Clock_Init();		// Switch System Clock = 80 MHz
	UART2_Init();
  GPIO_Init();
	
	post_pass = 1;
	USART_Write(USART2, (uint8_t *)welcome, strlen(welcome));
	USART_Write(USART2, (uint8_t *)run_post, strlen(run_post));
	while (post_pass) {
		// continuously run POST TEST
		USART_Write(USART2, (uint8_t *)prompt_post, strlen(prompt_post));
		rxByte = USART_Read(USART2);
		if (rxByte == '1') {
			post_pass = POST();
		} else {
			continue;	// skip this iteration when wrong input
		}
		if (post_pass) {
			// print fail message if post returns fail(1)
			USART_Write(USART2, (uint8_t *)prompt_post_2, strlen(prompt_post_2));
		}
	}
	// now start the main program
	while (1){
		// test passed! run histogram
		l_lim = 950;
		u_lim = l_lim + 100;
		numStr = sprintf(mainBuffer, a1, l_lim, u_lim);
		USART_Write(USART2, (uint8_t *)mainBuffer, numStr);
		USART_Write(USART2, (uint8_t *)a2, strlen(a2));
		rxByte = USART_Read(USART2);
		if (rxByte == 'Y' || rxByte == 'y') {
			USART_Write(USART2, (uint8_t *)q1, strlen(q1));
			// now collect user input
			for (i=0; i<LIMIIT_BUFFER_SIZE; i++) {
				rxByte = USART_Read(USART2);
				if (isdigit(rxByte)) {
					limBuffer[i] = rxByte;
				} else if (rxByte == '\r'){
					// kill the loop and terminate the buffer with caridge return
					limBuffer[i] = '\0';

					// before break out, print user input
					oo = sprintf(mainBuffer, "Input taken: %u\r\n", atoi(limBuffer));
					USART_Write(USART2, (uint8_t *) mainBuffer, oo);
					
					USART_Write(USART2, (uint8_t *) cont_prompt, strlen(cont_prompt));
					rxByte = USART_Read(USART2);
					if (rxByte) {
						break;
					}					
				} else {
					USART_Write(USART2, (uint8_t *) nay, strlen(nay));
				}
			}
			l_lim = atoi(limBuffer);
		} else if (rxByte == 'N' || rxByte == 'n') {
			// keep the limit values and run
			USART_Write(USART2, (uint8_t *)default_prompt , strlen(default_prompt));
			l_lim = 950;
		} else {
			continue;		// restart upon other inputs
		}
		u_lim = l_lim + 100;
		if (l_lim <= 9950 && l_lim >= 50) {
			histogram(l_lim, u_lim);
		} else {
			USART_Write(USART2, (uint8_t *)ob_promtp, strlen(ob_promtp));
		}
		
		USART_Write(USART2, (uint8_t *)end_promtp, strlen(end_promtp));
		
		rxByte = USART_Read(USART2);
		if (rxByte == 'Y' || rxByte == 'y') {
			continue;
		} else if (rxByte == 'N' || rxByte == 'n') {
			USART_Write(USART2, (uint8_t *)terminate, strlen(terminate));
			break;
		}
	}
}


