/*******OLED引脚接法如下****
DC——A15
RET——B3
D1——B4（SDA）
D0——B5（SCL）
*******按键引脚****************
//key0   A7
//key1   C3
//key2   F6
//key3   C14
//key4   E0
//LED    D0
//B10    SCL
//B11    SDA
**************************/
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "stm32f10x.h"
#include "oled.h"
#include "fdc2214.h"
#include "key.h"
#include "lcd.h"

#define  N  30
#define  M  15
#define  C_DATA            (res3 - temp3) * 1000
float res0, res1, res2, res3;
float temp0, temp1, temp2, temp3;
float ch0[50], ch1[50], ch2[50], ch3[50];
float FIR;
int i, j,q;
float value_buf[N];
u8 flag = 0;
u8 key, key_flag;
float data;
u8 exti_flag = 0;
char count;
float temp;
float sum = 0;
float data_max[M];//训练时1~30张数据最大值
float data_min[M];//训练时1~30张数据最小值
float min, max;



void KEY_scan(void)
{

    if(KEY == 0)
    {
        delay_ms(20);

        if(KEY == 0)
        {
            while(!KEY);
					
            key++;
					printf("KEY=%d\r\n",key);
            key_flag = 1;
        }
    }
}

int main(void)
{
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(115200);	 //串口初始化为115200
    OLED_Init();
    LED_Init();//初始化
    KEY_Init();
    while(FDC2214_Init());
    temp0 = Cap_Calculate(0);//读取初始值
    temp1 = Cap_Calculate(1);
    temp2 = Cap_Calculate(2);
    temp3 = Cap_Calculate(3);
    TIM5_Ctrl_Init(2000 - 1, 7200 - 1);
    LCD_Init();
    printf("RESET\r\n");
//   TIM3_Int_Init(50000-1,7200-1);
    while(1)
    {
      

//        key=KEY_scan();
        if(KEY==0) //按键被按下
        {
           key_flag = 1;
					if(key_flag == 1)            
					{
						LED = 0;
            if(flag == 1) //定时器中断
            {
                flag = 0; //定时中断标志位清零
                value_buf[exti_flag] = C_DATA; //向数组里存传感器数据
							
                exti_flag++; //中断标志位加1
						}
            if(exti_flag == N)
            {
                exti_flag = 0;
                for (i = 0; i < N - 1; ++i) //比较n-1轮
                {
                    for (j = 0; j < N - 1 - i; ++j) //每轮比较n-1-i次,
                    {
                        if (value_buf[j] < value_buf[j + 1])
                        {
                            temp = value_buf[j];
                            value_buf[j] = value_buf[j + 1];
                            value_buf[j + 1] = temp;
							  for(q=0;q<29;q++)
							{
							printf("%3.3f\r\n", value_buf[q]);
              }  
                        }
                    }
                }
                
                data_min[key - 1] = value_buf[N - 1]; //将排序完的数据从小到大排序后存放到匹配的数组中
                data_max[key - 1] = value_buf[0];
                key_flag = 0;
								printf("训练结束\r\n");
                LED = 1;
								
            }
					}
						
        }


        if( KEY== M)
        {
            
            for(i = 0; i < M; i++)
            {

               if( C_DATA>data_min[i]&&data_max[i]>C_DATA)
                {
                    printf("%d张\r\n", i+1);
                    printf("imin = %3.3f\r\n", data_min[i]);
                    printf("imax = %3.3f\r\n", data_max[i]);
                    printf("%3.3f\r\n", (res3 - temp3) * 100);
                    printf("----------------------------------------------------------------\r\n");
                }
//             printf("检测中\r\n");
            }

        }
//     printf("%3.3f\r\n",C_DATA);
// LCD_DrawRectangle(50,50,400,400); 	//画矩形
// LCD_ShowString(30, 50, 200, 16, 16, "WarShip STM32");
// LCD_ShowString(0, 24, 240, 24, 24, "纸");
    }
}




//定时器5中断服务程序
void TIM5_IRQHandler(void)   //TIM5中断
{
    if(TIM5->SR & 0X0001) //20ms定时中断
    {
        res0 = Cap_Calculate(0);//采集数据
        res1 = Cap_Calculate(1);
        res2 = Cap_Calculate(2);
        res3 = Cap_Calculate(3);
        flag = 1;

    }
    TIM5->SR &= ~(1 << 0); //中断标志位清0，等待下一次中断
}


//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
    {


        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志
    }
}





