#ifndef __KEY_H_
#define __KEY_H_
#include "sys.h"
#define KEY1 GPIO_ReadInputDataBit(GPIOF,GPIO_Pin_3)
#define KEY2 GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)
void key_init(void);
#endif



