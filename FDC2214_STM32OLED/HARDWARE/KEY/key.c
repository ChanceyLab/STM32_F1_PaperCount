#include "key.h"
#include "stm32f10x.h"
//key-gnd
//PF3
/****/
//key-VCC
//PB8

void key_init()
{
    GPIO_InitTypeDef GPIO_Initstructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF | RCC_APB2Periph_GPIOB,ENABLE);//GPIOF    πƒ‹ ±÷”

    GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPU;
    GPIO_Initstructure.GPIO_Pin=GPIO_Pin_3;
    GPIO_Init(GPIOF,&GPIO_Initstructure);

    GPIO_Initstructure.GPIO_Mode=GPIO_Mode_IPD;
    GPIO_Initstructure.GPIO_Pin=GPIO_Pin_8;
    GPIO_Init(GPIOB,&GPIO_Initstructure);

}

