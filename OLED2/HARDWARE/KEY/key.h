#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//读取按键0


 


void KEY_Init(void);//IO初始化
#endif
