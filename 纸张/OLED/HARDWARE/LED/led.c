#include "led.h"
#include "oled.h"
#include "delay.h"


void LED_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;				    
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOD, &GPIO_InitStructure);					       
 GPIO_SetBits(GPIOB,GPIO_Pin_9);						           

}


void BEEP_Init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE); 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;				    
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		
 GPIO_Init(GPIOF, &GPIO_InitStructure);					       
 GPIO_SetBits(GPIOF,GPIO_Pin_2);						           

}

