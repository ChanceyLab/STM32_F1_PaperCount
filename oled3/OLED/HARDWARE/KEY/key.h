#ifndef __KEY_H
#define __KEY_H
#include "sys.h"


#define KEY_Train          GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)
#define KEY_Test           GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)
#define KEY_Paper_Add      GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define KEY_Paper_Reduce   GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)




void KEY_Init(void);//IO≥ı ºªØ


#endif
