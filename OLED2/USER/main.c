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
//#include "lcd.h"

#define  N  40
#define  M  10    
#define  V  15   //����ֽ����
#define  NN  100 //�ɼ�������
#define  MM  10 //������
#define  T  10
float a[NN];
float b[NN/MM];
float c[NN/MM];
int x,y,z=0,o;
int p,q;
float res0, res1, res2, res3;
float temp0, temp1, temp2, temp3;
float ch0[50], ch1[50], ch2[50], ch3[50];
float FIR;
int i, j;
float value_buf[N];
u8 flag = 0;
u8 key_val=0;
float data;
u8 exti_flag = 0;
char count;
float temp;
float sum = 0;
float data_max[M];//ѵ��ʱ1~30���������ֵ
float data_min[M];//ѵ��ʱ1~30��������Сֵ
float min, max;
float da;

float getmid(void);
//void key_scan()
//{

//    if(KEY == 0)
//    {
//        delay_ms(20);

//        if(KEY == 0)
//        {
//            while(!KEY);
//           
//				}  
//    }
//}

int main(void)
{
    delay_init();	    	 //��ʱ������ʼ��
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
    OLED_Init();
    LED_Init();//��ʼ��
    KEY_Init();
    while(FDC2214_Init());
    temp0 = Cap_Calculate(0);//��ȡ��ʼֵ
    temp1 = Cap_Calculate(1);
    temp2 = Cap_Calculate(2);
    temp3 = Cap_Calculate(3);
    TIM5_Ctrl_Init(1000 - 1, 7200 - 1);
//    LCD_Init();
    printf("��λ\r\n");

    
    

//        filter();
//			 printf("%3.3f\r\n",data);
//        key_scan();  //����ɨ��ѵ������ֽ����
        if(KEY==0) //����������
        {
            printf("ѵ��\r\n");
            LED = 0;
					getmid();
					data_min[key_val]=c[NN/MM-1];
					data_max[key_val]=c[0];
//            if(flag == 1) //��ʱ���ж�
//            {
//                flag = 0; //��ʱ�жϱ�־λ����
//                value_buf[exti_flag] = ((res2 - temp2)+(res3 - temp3))*100;//��������洫��������
//                exti_flag++; //�жϱ�־λ��1
//            }
//            if(exti_flag == N)
//            {
//                exti_flag = 0;
//                for (i = 0; i < N - 1; ++i) //�Ƚ�n-1��
//                {
//                    for (j = 0; j < N - 1 - i; ++j) //ÿ�ֱȽ�n-1-i��,
//                    {
//                        if (value_buf[j] < value_buf[j + 1])
//                        {
//                            temp = value_buf[j];
//                            value_buf[j] = value_buf[j + 1];
//                            value_buf[j + 1] = temp;
//                        }
//                    }
//                }

//                data_min[key - 1] = value_buf[N - 1]; //������������ݴ�С����������ŵ�ƥ���������
//                data_max[key - 1] = value_buf[0];
//                key_flag = 0;
//                LED = 1;
             key_val++;
          }
				 if(KEY == 0&&KEY==V)
        {
            printf("���\r\n");
            for(i = 0; i < V; i++)
            {

                if(data_min[i] > ((res3 - temp3))*100 && ((res3 - temp3) < data_max[i])*100)
								{ printf("%d��\r\n", i);
                printf("imin = %3.3f\r\n", data_min[i]);
                printf("imax = %3.3f\r\n", data_max[i]);
                printf("----------------------------------------------------------------\r\n");
								}

            }

          }
        da=res3-temp3;
        printf("%3.3f\r\n", da);
  }
       
//        min = data_min[key];
//        max = data_max[key];
//        printf("min:%3.3f,max:%3.3f\r\n", min, max);


//       printf("%3.3f\r\n",(res0 - temp0)*100*0.9);
//   if(flag == 1)
//	 {
//     flag=0;
//		 value_buf[exti_flag] = (res2 - temp2) * 500;//��������洫��������
//		 exti_flag++;
//	 }
//    if(exti_flag==N)
//		{
//        exti_flag=0;
//				for (i = 0; i < N - 1; ++i) //�Ƚ�n-1��
//        {
//            for (j = 0; j < N - 1 - i; ++j) //ÿ�ֱȽ�n-1-i��,
//            {
//                if (value_buf[j] < value_buf[j + 1])
//                {
//                    temp = value_buf[j];
//                    value_buf[j] = value_buf[j + 1];
//                    value_buf[j + 1] = temp;
//                }
//            }
//        }

//        data_min[key - 1] = value_buf[N - 1]; //������������ݴ�С����������ŵ�ƥ���������
//        data_max[key - 1] = value_buf[0];

//       }
//    }




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


        TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //���TIMx�����жϱ�־
    }
}

float getmid(void)
{
    for(x=0; x<NN; x++)
    {
        a[x]=(res2/res3)*1000;
        delay_ms(T);
    }

    for (p=0; p<NN-1; p++)
    {
        for (q=0; q<NN-1-p; ++q)
        {
            if (a[q] < a[q+1])
            {
                o = a[q];
                a[q] = a[q+1];
                a[q+1] = o;
            }
        }
    }//ð��
    for(y=NN/MM/2; y<NN-NN/MM/2; y+=NN/MM)
    {
        c[z]=b[y];
    }



}

