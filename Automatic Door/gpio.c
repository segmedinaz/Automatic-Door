#include "gpio.h"


void initPins(){

RCC->APB1ENR|=0x00000004; //enable TIM4 na APB1
RCC->AHB1ENR|=0x00000002; //GPIOB ENABLE
GPIOB->MODER|=0x00002000; //PB6 AF
GPIOB->OSPEEDR|=0x00003000;// high speed
GPIOB->OTYPER|=0x00000000; //push-pull output
GPIOB->PUPDR|=0x00000000;//no pull-up, no pull-down
GPIOB->AFR[0]|=0x02000000; //AF config



}


