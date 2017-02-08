// written by prof Kiser
// input_pa0.c

#include "input_pa0.h"
#include "stm32l476xx.h"
#include "LED.h"

void PA0_Init(void) {
    RCC->AHB2ENR |= RCC_AHB2ENR_GPIOAEN; // enable clock for A group of GPIO
    GPIOA->MODER &= ~3;

    // PA0 is now in input mode
    // GPIOA->Moder | = 2;

}

int read_pa0(void) {
    return GPIOA->IDR & 1;       // read the bottom bit of the GPIOA input lines which is PA0
                                 // returns 1 if high, 0 if low
}

void monitor_pa0(void) {
    while(1){
        if ( read_pa0() ) {
            GREEN_LED_On();
        } else {
            GREEN_LED_Off();
        }
    }
}

