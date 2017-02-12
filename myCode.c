#include "stm32l4xx.h"
#include "myCode.h"
#include <string.h>
#include <stdio.h>
//#include "UART.h"

char str2;
int uvalue = 950 ;
int lvalue = 1050;


int j;
int k,b,c;
int buck[1051];
int main()
{	
		
		
		char buffer1[20];
		char buffer2[20];
		int a1= strlen("The value of the upper limit and lower limit\n\r\n\r\n");
		int a2= strlen("DO you want to change the upper limit and lower limtit \n\r\n\r\n");
		int a3= strlen("please change the upper limit and lower limit value\n\r\n\r\n");
		int a4= strlen("the new uvalue and lvalue are \n\r\n\r\n");
		int a5= strlen("correct value \n\r\n\r\n");
		int a6= strlen( "please enter the correct value");
		
		
		USART_Write(USART2, (uint8_t *)"The value of the upper limit and lower limit is 950 and 1050\n\r\n\r\n", a1);/////printf("The value of the upper limit and lower limit\n");
		USART_Write(USART2, (uint8_t *)"DO you want to change the upper limit and lower limtit \n\r\n\r\n",a2) ;////////////////////////////////printf("DO you want to change the upper limit and lower limti");
		str2 = USART_Read(USART2);
		if(str2 == 'Y' || str2 =='y')
		{
			
			USART_Write(USART2, (uint8_t *)"please change the upper limit and lower limit value\n\r\n\r\n",a3);
			g:
		
			for(j=0; j<20 ; j++)
			{
			buffer1[j]=USART_Read(USART2);   ////scanf("%d", &uvalue);
			}
			for(k=0; k<20 ; k++)
			{
			buffer2[k]=USART_Read(USART2);   ////scanf("%d" ,&lvalue);
			}
			uvalue=atoi(buffer1);
			lvalue=atoi(buffer2);
			

			if( lvalue > 50 && lvalue <9950 && uvalue-lvalue ==100)
			{
				USART_Write(USART2, (uint8_t *)"correct value \n\r\n\r\n",a5);
			}
			else
				{
				USART_Write(USART2, (uint8_t *) "please enter the correct value",a6);
					goto g;
				}
				
		}
	else 
	{
		timer_working();
	}
}



int timer_working()
	{
			while( uvalue-lvalue ==100)
	{
			while(TIM_SR_CC1IF ==1)
			{
					int n;
					int a=TIM2->CCR1;
				  buck[lvalue]=a;
					n=strlen("Histogram \r\n Buck  ||  Value");
					USART_Write(USART2, (uint8_t *)"Histogram \r\n Buck  ||  Value",n);
					lvalue++;
			}
		
	}
		
		}	

	




void post_check()
	{
		int a7= strlen(" \n\rPOST TEST is succesfull . Program is going into Histogram mode \n\r \n\r ");
		int a8= strlen("\n\rPOST TEST has failed.Give Y if you want to retry the POST TEST else N to end the program \n\r\n\r");
		while(TIM_SR_CC1IF ==1)
		{
				int b;
				b = TIM2->CCR1;
		}
		while(TIM_SR_CC1IF ==1)
		{
			int c;
			c= TIM2->CCR1;
		}
		if(c-b<100000)
		{
			
			USART_Write(USART2, (uint8_t *) "\n\rPOST TEST is succesfull . Program is going into Histogram mode \n\r \n\r ",a7);
			timer_working();
		}
		else
		{
			USART_Write(USART2, (uint8_t *)"\n\rPOST TEST has failed.Give Y if you want to retry the POST TEST else N to end the program \n\r\n\r",a8);
			str2 = USART_Read(USART2);
			if(str2 == 'Y' || str2 =='y')
			{
				post_check();
			}
			else(str2 == 'N' || str2 =='n')
			{
				exit(void);
			}
			
		}
}

	
void  GPIO_config()
{
RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN;   ////enabling the GPIOA port
	GPIOA->MODER = 0;   /////clearing the moder register
	GPIOA->MODER = GPIO_MODER_MODER0_1 ;   ///// configuring into the Alternate function mode
	GPIOA->AFR[0] = GPIO_AFRL_AFRL1;  ///// configuring the lower part of the AFR to the alternate function 1.
} 


void timer_cofig()
{
	///enabling the system clock for the timer
		 RCC->APB1ENR1 = RCC_APB1ENR1_TIM2EN ;		 
	///loading the prescalar vlaue into the psc register                 
		 TIM2->PSC = 80;
	///how to force new load to the prescalar value by creating an update???
			
}
		 
void timer_capture_config()
	{
		
		TIM2->CCER= TIM_CCER_CC1NE; //////disable the input capture
		TIM2->CCMR1 = TIM_CCMR1_CC1S_1;   ////
		TIM2->CCER= TIM_CCER_CC1E; ///RE-ENBALE CAPTURE INPUT.
		
	}
	 
	
	int timer_operation()
	{
	 TIM2->CR1= TIM_CR1_CEN ; ////starting the input capture 
		if( TIM_SR_CC1IF ==1)
		{
			
			
			return TIM2  ->CCR1;   ///reading the captured counter value from the TIMx->CCRx register	
	}
	return TIM2  ->CCR1;
		
}