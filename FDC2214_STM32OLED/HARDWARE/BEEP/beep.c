#include "stm32f10x.h"
#include "beep.h"
//PG1
//
//ͨ���̵��������첻��

void BEEP_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstructure;//��ʼ��ѡ����ʽ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOG,ENABLE);
	//���ֳ�ʼ��Ҫ��
	GPIO_Initstructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_Initstructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_Initstructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_ResetBits(GPIOG,GPIO_Pin_1);
	GPIO_Init(GPIOG,&GPIO_Initstructure);

}





