#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "fdc2214.h"
#include "beep.h"
#include "key.h"
#include "led.h"
#include "McuDataSend.h"

//�˲�
#define n 11
u8 i,j;
float buf;
float a[n];
/********************************************************
�������ch0����һ��ͨ��
********************************************************/
void Sys_Init(void);
float maopao(void);
float res2,res3;//��ǰֵ
float temp2,temp3;

int main(void)
{
    Sys_Init();
    while(1)
    {
        printf("ch = %f\r\n",maopao());
		
//		while(USART_GetFlagStatus(USART1,USART_FLAG_TC)==RESET);
//		delay_ms(5000);
    }
}

void Sys_Init(void)
{
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_Configuration();//����NVIC�жϷ���2:2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
//    BEEP_Init();
//	key_init();
//	led_init();
    while(FDC2214_Init());
    delay_ms(500);
    temp2 = Cap_Calculate(2);//��ȡ��ʼֵ
    temp3 = Cap_Calculate(3);//��ȡ��ʼֵ
    TIM5_Ctrl_Init(500-1,7200-1);   //50ms�ɼ�һ�Σ�500/10
}

//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)   //TIM5�ж�
{
    if(TIM5->SR&0X0001)//50ms��ʱ�ж�
    {
        res2 = Cap_Calculate(2);//�ɼ�����
        res3 = Cap_Calculate(3);//�ɼ�����
    }
    TIM5->SR&=~(1<<0);  //�жϱ�־λ��0���ȴ���һ���ж�
}

float maopao(void)
{
    for(i=0;i<n;i++)
    {
		a[i] = (res2 / res3 * 1000.0 - 996) * 100.0 -25;
		delay_ms(50);
    }
	for(i=0; i<n-1; ++i)  
    {
        
		for (j=0; j<n-1-i; ++j)  
        {
            
			if (a[j] < a[j + 1])
            {
                buf = a[j];
                a[j] = a[j+1];
                a[j+1] = buf;
            }
        }
    }
	return a[(n + 1) / 2];
}

