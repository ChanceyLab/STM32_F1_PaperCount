#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"


#define KEY   GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)//��ȡ����0


 


void KEY_Init(void);//IO��ʼ��
#endif
