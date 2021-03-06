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
#include "stdlib.h"
#include "delay.h"
#include "usart.h"
#include "timer.h"
#include "led.h"
#include "stm32f10x.h"
#include "oled.h"
#include "fdc2214.h"
#include "key.h"
#include "lcd.h"
#include "exti.h"
#include "math.h"
#define  N  50
#define  M  50
#define  T  30
#define  C_DATA            (res3 - temp3) * 5000
float res0, res1, res2, res3;
float temp0, temp1, temp2, temp3;
float ch0[50], ch1[50], ch2[50], ch3[50];
int i, j,q=0, k = 0;
float value_buf[N];
u8 flag = 0;

float data;
u8 exti_flag = 0;
u8 train_flag;
u8 train_flag1;
u8 test_flag;
char count;
float temp;
float sum = 0;
float data_max[M];//训练时1~30张数据最大值
float data_min[M];//训练时1~30张数据最小值
float min, max;
extern u8  paper_num;

float tnum[T];
u8 g = 0;
int rate[M];
int l=0, s, b;
int mx, mx_i;
float ysum, avg, yang;
float rateM[T], yangmx;

void key_scan()
{
    if(KEY_Train == 0)
    {
        delay_ms(10);
        if(KEY_Train == 0)
        {
            while(!KEY_Train)
                train_flag = 1;
            if(k == 1)
            {
                train_flag1 = 1;
                k=1;
            }
            k=1;
        }
    }

    if(KEY_Test == 0)
    {
        delay_ms(10);
        if(KEY_Test == 0)
        {
            while(!KEY_Test);
            test_flag = 1;

        }
    }
}


int main(void)
{
    paper_num = 0;
    delay_init();	    	 //延时函数初始化
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//设置NVIC中断分组2:2位抢占优先级，2位响应优先级
    uart_init(115200);	 //串口初始化为115200
    OLED_Init();
    LED_Init();//初始化
    LCD_Init();
    BEEP_Init();
    while(FDC2214_Init());
    temp0 = Cap_Calculate(0);//读取初始值
    temp1 = Cap_Calculate(1);
    temp2 = Cap_Calculate(2);
    temp3 = Cap_Calculate(3);
    TIM5_Ctrl_Init(1000 - 1, 7200 - 1);
    EXTIX_Init();
    LCD_Init();
    POINT_COLOR = BLACK;
    printf("RESET\r\n");
    TIM3_Int_Init(1000-1,7200-1);
    while(1)
    {
        if(train_flag == 0)
        {
            LCD_ShowString(60,208,210,24,24,"Working...");
        }
        key_scan();
//      if(paper_num==50)paper_num=0;
        if(train_flag1 == 1)
        {
            paper_num++;
            train_flag1 = 0;
        }
        if(train_flag == 1) //按键被按下
        {

            //printf("paper_num%d\r\n", paper_num);
//					printf("%d张纸训练开始\r\n",paper_num);
//					LCD_ShowString(30,30,30,30,16,"training start");		//显示一个字符串,12/16字体
            train_flag = 1;
            LED = 0;
            if(flag == 1) //定时器中断
            {
                flag = 0; //定时中断标志位清零
                if((res3 - temp3) * 5000 < 0)
                {
                    LCD_Clear(WHITE);
                    LCD_ShowString(60,208,210,24,24,"Attention!!!");
                }
                value_buf[exti_flag] = (res3 - temp3) * 5000; //向数组里存传感器数据
                exti_flag++; //中断标志位加1
                printf("数组：");
                for(q=0; q<29; q++)
                {
                    printf("shu%3.3f\r\n", value_buf[q]);
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
                            }
                        }
                    }


                    data_min[ paper_num-1] = value_buf[N - 2]; //将排序完的数据从小到大排序后存放到匹配的数组中
                    data_max[ paper_num-1] = value_buf[0];
                    printf("value_buf数组：");
                    for(q=0; q<80; q++)
                    {
                        printf("shu%3.3f\r\n", value_buf[q]);
                    }
                    printf("min%3.3f\r\n", value_buf[N - 2]);
                    printf("max%3.3f\r\n", value_buf[0]);
                    printf("data_min数组：");
                    for(q=0; q<50; q++)
                    {
                        printf("shu%3.3f\r\n", data_min[q]);
                    }
                    printf("data_max数组：");
                    for(q=0; q<50; q++)
                    {
                        printf("shu%3.3f\r\n", data_max[q]);
                    }

                    train_flag = 0;
                    printf("%d张纸训练结束\r\n",paper_num);
                    LCD_Clear(WHITE);
                    LCD_ShowString(60,308,210,24,24,"Train Paper_num:");
                    LCD_ShowxNum(250,308, paper_num,5,24,0);
                    LED = 1;
                }
            }

        }

        if(test_flag == 1)//开始测试
        {
            g = 0;
            while(g < T)
            {
                if(flag == 1) //定时器中断
                {
                    flag = 0; //定时中断标志位清零
                    tnum[g++] = C_DATA;
                    printf("gggg%d",g);
                }
            }


            for(l = 0; l < M; l++)
            {
                rate[l] = 0;
            }

            // 2
//						for (i = 0; i < T - 1; ++i) //比较n-1轮
//						{
//								for (j = 0; j < T - 1 - i; ++j) //每轮比较n-1-i次,
//								{
//										if (tnum[j] < tnum[j + 1])
//										{
//												temp = tnum[j];
//												tnum[j] = tnum[j + 1];
//												tnum[j + 1] = temp;
//										}
//								}
//						}
//						ymax = tnum[0];
//						ymin = tnum[T-1];
//						printf("ymax = %3.3f\r\n",ymax)	;
//						printf("ymin = %3.3f\r\n",ymin)	;


            printf("tnum：");
            for(l = 0; l < T; l++)
            {
                printf("%3.3f\r\n,", tnum[l]);
            }

            // 2
//						for(b = 0; b < M; b++)
//						{
//							if(b >= paper_num)
//								break;
//							printf("min = %3.3f\r\n",data_min[b])	;
//							printf("max = %3.3f\r\n",data_max[b])	;
//
//							if(data_min[b] <= ymin && data_max[b] >= ymax)
//							{
//								rate[b] = ymax - ymin;
//							}
//							else if(data_max[b] > ymax && data_min[b] > ymin)
//							{
//									rate[b] = ymax - data_min[b];
//							}
//							else if(ymax > data_max[b] && ymin > data_min[b])
//							{
//									rate[b] = data_max[b] - ymin;
//							}
//
//						}


            // 1
            for(b = 0; b < paper_num; b++)
            {
                printf("min = %3.3f\r\n",data_min[b])	;
                printf("max = %3.3f\r\n",data_max[b])	;
                s = 0;
                while(s < T)
                {
                    if(tnum[s] > data_min[b] && tnum[s] < data_max[b])
                    {
                        rate[b]++;
                    }
                    s++;
                }
            }

            for(l = 0; l < M; l++)
            {
                printf("rate[%d]=%d\r\n,", l, rate[l]);
            }

            mx = rate[0];
            mx_i = 0;
            for(i = 0; i < M; i++)
            {
                if(i >= paper_num)
                    break;
                printf("rate[i]%d\r\n,",rate[i]);
                if(rate[i] > mx)
                {
                    mx = rate[i];
                    mx_i = i+1;
                }
                printf("mx_i%d\r\n", mx_i);
            }

            if(mx_i == 0)
            {
                s = 0;
                sum = 0;
                while(s < T)
                {
                    sum += tnum[s];
                    s++;
                }
                avg = 0;
                avg = sum / T;
                printf("avg = %3.3f\r\n", avg)	;
                for(l = 0; l < M; l++)
                {
                    rateM[l] = 0;
                }
                for(b = 0; b < paper_num; b++)
                {
                    rateM[b] = abs(data_max[b] - avg);
                    printf("rateM[b] = %3.3f\r\n", rateM[b])	;
                }
                yangmx = rateM[0];
                mx_i = 0;
                for(i = 0; i < M; i++)
                {
                    if(i >= paper_num)
                        break;
                    printf("rate[i] %3.3f\r\n,",rateM[i]);
                    if(rateM[i] < yangmx)
                    {
                        yangmx = rateM[i];
                        mx_i = i+1;
                    }
                    printf("mx_i%d\r\n", mx_i);
                }

            }

            printf("%d", mx_i);


            printf("张纸");
            LCD_Clear(WHITE);
            LCD_ShowString(60,308,210,24,24," Paper Pages:");
            LCD_ShowxNum(250,308,mx_i,5,24,0);
            BEEP = 0;
            delay_ms(1000);
            BEEP = 1;

            test_flag = 0;
        }
    }
}




//定时器5中断服务程序
//    void TIM5_IRQHandler(void)   //TIM5中断
//    {
//        if(TIM5->SR & 0X0001) //20ms定时中断
//        {
//            res0 = Cap_Calculate(0);//采集数据
//            res1 = Cap_Calculate(1);
//            res2 = Cap_Calculate(2);
//            res3 = Cap_Calculate(3);
//            flag = 1;

//        }
//        TIM5->SR &= ~(1 << 0); //中断标志位清0，等待下一次中断
//    }


//定时器3中断服务程序
//    void TIM3_IRQHandler(void)   //TIM3中断
//    {
//        if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
//        {

//            int time;
//            time++;
//            if((test_flag ==1)&&(time==1))
//            {
//                time=0;
//                test_flag = 0;
//                printf("超时\r\n");
//            }
//            TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志
//        }
//    }

void TIM5_IRQHandler(void)   //TIM5中断
{
    if(TIM5->SR & 0X0001) //20ms定时中断
    {
//        res0 = Cap_Calculate(0);//采集数据
//        res1 = Cap_Calculate(1);
//        res2 = Cap_Calculate(2);
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
        int time;
        time++;
        if((test_flag ==1)&&(time==1))
        {
            time=0;
            test_flag = 0;
            printf("超时\r\n");
        }

        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志
    }
}




