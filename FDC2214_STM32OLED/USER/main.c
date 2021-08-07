#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "fdc2214.h"
#include "beep.h"
#include "key.h"
#include "led.h"
#include "McuDataSend.h"

//滤波
#define n 11
u8 i,j;
float buf;
float a[n];
/********************************************************
两极板接ch0，接一个通道
********************************************************/
void Sys_Init(void);
float maopao(void);
float res2,res3;//当前值
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
    delay_init();	    	 //延时函数初始化
    NVIC_Configuration();//设置NVIC中断分组2:2；2位抢占优先级，2位响应优先级
    uart_init(115200);	 //串口初始化为115200
//    BEEP_Init();
//	key_init();
//	led_init();
    while(FDC2214_Init());
    delay_ms(500);
    temp2 = Cap_Calculate(2);//读取初始值
    temp3 = Cap_Calculate(3);//读取初始值
    TIM5_Ctrl_Init(500-1,7200-1);   //50ms采集一次，500/10
}

//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM5中断
{
    if(TIM5->SR&0X0001)//50ms定时中断
    {
        res2 = Cap_Calculate(2);//采集数据
        res3 = Cap_Calculate(3);//采集数据
    }
    TIM5->SR&=~(1<<0);  //中断标志位清0，等待下一次中断
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

