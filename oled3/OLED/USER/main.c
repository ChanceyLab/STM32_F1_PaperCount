/*******OLED���Žӷ�����****
DC����A15
RET����B3
D1����B4��SDA��
D0����B5��SCL��
*******��������****************
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
#include "exti.h"
#define  N  80
#define  M  50
#define  C_DATA            (res3 - temp3) * 5000
float res0, res1, res2, res3;
float temp0, temp1, temp2, temp3;
float ch0[50], ch1[50], ch2[50], ch3[50];
int i, j;
float value_buf[N];
u8 flag = 0;

float data;
u8 exti_flag = 0;
u8 train_flag;
u8 test_flag;
char count;
float temp;
float sum = 0;
float data_max[M];//ѵ��ʱ1~30���������ֵ
float data_min[M];//ѵ��ʱ1~30��������Сֵ
float min, max;
extern u8  paper_num;
void key_scan()
{
    if(KEY_Train == 0)
    {
        delay_ms(10);
        if(KEY_Train == 0)
        {
            while(!KEY_Train);
            train_flag = 1;

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
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
    OLED_Init();
    LED_Init();//��ʼ��

    while(FDC2214_Init());
    temp0 = Cap_Calculate(0);//��ȡ��ʼֵ
    temp1 = Cap_Calculate(1);
    temp2 = Cap_Calculate(2);
    temp3 = Cap_Calculate(3);
    TIM5_Ctrl_Init(1000 - 1, 7200 - 1);
    EXTIX_Init();
    LCD_Init();
    printf("RESET\r\n");
    TIM3_Int_Init(5000-1,7200-1);
    while(1)
    {
		
      LCD_ShowNum(20,20,paper_num,3,16);  						//��ʾһ������
      key_scan();
      if(paper_num==50)paper_num=0;
        if(train_flag ==1) //����������
        {
//					printf("%d��ֽѵ����ʼ\r\n",paper_num);
//					LCD_ShowString(30,30,30,30,16,"training start");		//��ʾһ���ַ���,12/16����
          train_flag = 1;
            LED = 0;
            if(flag == 1) //��ʱ���ж�
            {
                flag = 0; //��ʱ�жϱ�־λ����
                value_buf[exti_flag] = C_DATA; //��������洫��������
                exti_flag++; //�жϱ�־λ��1
            }
            if(exti_flag == N)
            {
                exti_flag = 0;
                for (i = 0; i < N - 1; ++i) //�Ƚ�n-1��
                {
                    for (j = 0; j < N - 1 - i; ++j) //ÿ�ֱȽ�n-1-i��,
                    {
                        if (value_buf[j] < value_buf[j + 1])
                        {
                            temp = value_buf[j];
                            value_buf[j] = value_buf[j + 1];
                            value_buf[j + 1] = temp;
                        }
                    }
                }

                data_min[ paper_num-1] = value_buf[N - 1]; //������������ݴ�С����������ŵ�ƥ���������
                data_max[ paper_num-1] = value_buf[0];
                train_flag = 0;
								printf("%d��ֽѵ������\r\n",paper_num);
                LED = 1;
            }
        }


        if(test_flag == 1)//��ʼ����
        {

            for(i = 0; i < M; i++)
            {

               if( C_DATA>data_min[i]&&data_max[i]>C_DATA)
                {
                    printf("%d��\r\n", i+1);
                    printf("imin = %3.3f\r\n", data_min[i]);
                    printf("imax = %3.3f\r\n", data_max[i]);
                 
                    printf("----------------------------------------------------------------\r\n");
									  test_flag = 0;
                }
//								printf("%3.3f\r\n", (res3 - temp3) * 5000);
								
//             printf("�����\r\n");
            }

        }
//     printf("%3.3f\r\n",C_DATA);


    }
}




//��ʱ��5�жϷ������
void TIM5_IRQHandler(void)   //TIM5�ж�
{
    if(TIM5->SR & 0X0001) //20ms��ʱ�ж�
    {
        res0 = Cap_Calculate(0);//�ɼ�����
        res1 = Cap_Calculate(1);
        res2 = Cap_Calculate(2);
        res3 = Cap_Calculate(3);
        flag = 1;

    }
    TIM5->SR &= ~(1 << 0); //�жϱ�־λ��0���ȴ���һ���ж�
}


//��ʱ��3�жϷ������
void TIM3_IRQHandler(void)   //TIM3�ж�
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //���TIM3�����жϷ������
    {
			
			int time;
			 time++;
       if((test_flag ==1)&&(time==1))
			 {
				time=0;
			  test_flag = 0;
			  printf("��ʱ\r\n");
			 }
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־
    }
}





