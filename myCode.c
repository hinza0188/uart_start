#include "stm32l4xx.h"
#include "myCode.h"
#include <string.h>
#include <stdio.h>
#include "UART.h"
#include <stdlib.h>
#include <ctype.h>

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
		
			for(j=0; j<5||buffer1[j]!='\r'||buffer1[j]!='\n' ; j++)
			{
			buffer1[j]=USART_Read(USART2);   ////scanf("%d", &uvalue);
			}
			for(k=0; k<5||buffer2[k]!='\r'||buffer2[k]!='\n' ; k++)
			{
			buffer2[k]=USART_Read(USART2);   ////scanf("%d" ,&lvalue);
			}
			uvalue=atoi(buffer1);
			lvalue=atoi(buffer2);
			

			if( lvalue > 50 && lvalue <9950 && uvalue-lvalue ==100)
			{
				USART_Write(USART2, (uint8_t *)"correct value \n\r\n\r\n",a5);
				
				histogram();
			}
			else
		
			{
				USART_Write(USART2, (uint8_t *) "please enter the correct value",a6);
					goto g;
				}
				
		}
	else 
	{
		 histogram();
	}
}



int histogram()
	{
			while(!(TIM_SR_CC1IF ==1);
				
					
					int x=TIM2->CCR1;
		
						while (!(TIM_SR_CC1IF==1));
						
							 int y=TIM2->CCR1;
						buck[y-x]++;
						
						
					int n;
					n=strlen("Histogram \r\n Buck  ||  Value  \n\r %d  ||  %d",y);////
					USART_Write(USART2, (uint8_t *)"Histogram \r\n Buck  ||  Value",n);////
					





			}
			

	




			
			
			
int post_check()
	{
		int a7= strlen(" \n\rPOST TEST is succesfull . Program is going into Histogram mode \n\r \n\r ");
		int a8= strlen("\n\rPOST TEST has failed.Give Y if you want to retry the POST TEST else N to end the program \n\r\n\r");
		while(TIM2->CR1 == TIM_CR1_CEN)
		{	
				int cnt;
				for( int t=0; t<100 ; t++)
						{
							cnt++;
						}
				if( cnt == 100 && (TIM_SR_CCIF==1))
				{
							USART_Write(USART2, (uint8_t *) "\n\rPOST TEST is succesfull . Program is going into Histogram mode \n\r \n\r ",a7);
							 main();	
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
									return 0;
								}
			
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


int timer_cofig()
{
	///enabling the system clock for the timer
		RCC->APB1ENR1 = RCC_APB1ENR1_TIM2EN ;		 
	///loading the prescalar vlaue into the psc register                 
		TIM2->PSC = 0x4f;
	///how to force new load to the prescalar value by creating an update???
		TIM2->EGR |=0X01;
		TIM2->CCER &= TIM_CCER_CC1NE; //////disable the input capture
		TIM2->CCMR1 = TIM_CCMR1_CC1S_1;   ////
		TIM2->CCER |= TIM_CCER_CC1E; ///RE-ENBALE CAPTURE INPUT.
		TIM2->CR1= TIM_CR1_CEN ; ////starting the input capture 
		
	if( TIM_SR_CC1IF ==1)
		{
			
			
			return TIM2  ->CCR1;   ///reading the captured counter value from the TIMx->CCRx register	
	}
	return TIM2  ->CCR1;
		
}
