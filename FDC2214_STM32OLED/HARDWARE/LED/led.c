#include "stm32f10x.h"
#include "led.h"
//LED  PB9

void led_init()//��ʼ��a0��c5
{
  GPIO_InitTypeDef GPIO_Initstructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);//GPIO B


	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;//�������
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_9;//5��
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;//��50mhz
	GPIO_Init(GPIOB,&GPIO_Initstructure);//E��
	GPIO_SetBits(GPIOB,GPIO_Pin_9);
	
}



