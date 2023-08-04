/************************************************
fork from F4_HAL_emwin https://github.com/framist/STemWinForHAL
ʵ��ƽ̨ ALIENTEK STM32F407ZGT6 ��Сϵͳ��

AUEDC 2022
ps. ʵ������ NUEDC ��֮ǰ���뱾���� UI �����е� AUEDC ���Ǳ��� -- 2023-8-4

�漶�ӿڣ�

printf ������Ϊ�������

TIM14 PA7 ����PWM���
PC0~PC2 ������������
PF8 ��Ϊ����ʿ������

���һʱˬ���ع����᳡��
************************************************/

#include "main.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "ILI93xx.h"
#include "key.h"
#include "sram.h"
#include "malloc.h"
#include "touch.h"
#include "timer.h"
#include "arm_math.h"  
#include "adc.h"
#include "exti.h"
//GUI֧��
#include "GUI.h"
#include "FramewinDLG.h"

__IO uint32_t nowTime;


// ���� DS3120 �Ƕ�
// 1000~2500ns -> 0~270 deg
void PWM_SetDegree(double deg) {
    deg += 90;
    deg = deg * 90 / 106; // �Ƕȳ�������
    deg = (deg - 0.2193)/1.0221 ; // �Ƕȸ��ݼ�������
    u32 compare = 5000 - (u32)(deg/180.0 * 1500 + 1000);
    TIM_SetTIM14Compare1(compare);
    // mainLogPrintf("\nset deg: %f deg, %u cmp",deg,compare);
}

// �����㺯��
//#define NOW_TIME_TICK (1/168000000.0)
#define NOW_TIME_TICK (1/200000.0)
#include "math.h"
void main_solve(struct exti_data *ed) {
    //mainLogPrintf("\nmain_solve:");
    if (ed->data_writed == 0x0001U+0x0002U+0x0004U) {
        LASER(GPIO_PIN_SET);   //�رռ����
        double t1 = ed->timestap[0]*NOW_TIME_TICK;
        double t2 = ed->timestap[1]*NOW_TIME_TICK;
        double t3 = ed->timestap[2]*NOW_TIME_TICK;

        double tau1 = t2 - t3;
        double tau2 = t1 - t2; 
        double v = 340.0;   // ���� m/s
        double d = 0.5;       // ���ڴ�����֮��ľ��� m
        double theta, gamma;
        gamma = (2*pow(d,2) - pow(tau2*v,2) + pow(tau1*v,2) - 2*tau1*tau2*pow(v,2))
                /(2*(tau2-tau1)*v);
        gamma = gamma + tau1*v;
        theta = PI/2 - acos( (pow(gamma,2) + pow(d,2) - pow(gamma+tau2*v,2))
                /(2*(gamma)*d) );
        theta = theta * 180.0 / PI;

        mainLogPrintf("\n[+]solved t: %.8f, %.8f, %.8f s\n    tau: %.8f, %.8f",t1,t2,t3,tau1,tau2);
        mainLogPrintf("\n   theta: %f deg; gamma: %f m",theta,gamma);
        
        // ���У׼
        theta = theta*1.0362-1.4403;
        gamma = 0.3869*gamma+1.8659;

        // gamma ��ֵ
        if (gamma * cos(theta*PI/180.0) > 3.05) {
            gamma = 3.05/cos(theta*PI/180.0);
        }
        if (gamma * cos(theta*PI/180.0) < 2.55) {
            gamma = 2.55/cos(theta*PI/180.0);
        }

        // �޳���ֵ
        if ( -60 < theta && theta < 60 
            && fabs(tau1)+fabs(tau2) < 0.005 // <1.5e-4 * 2
            /* && 0 < gamma && gamma < 5*/) {
            PWM_SetDegree(theta);
            show_reflesh(theta,gamma);
        }
        
        //����
        while (1) {
            GUI_Delay(100);  // ĿǰΨһ�ֶ��ӳ�
            if (ed->data_writed == 0x0U) {
                break;
            } else {
                ed->data_writed = 0x0U;
            }
        }
        LASER(GPIO_PIN_RESET);   //�򿪼����
        
    } else {
        //mainLogPrintf(".%x.",ed->data_writed);
    }
//    HAL_NVIC_EnableIRQ(EXTI0_IRQn);
//    HAL_NVIC_EnableIRQ(EXTI1_IRQn);
//    HAL_NVIC_EnableIRQ(EXTI2_IRQn);

    
}
void voiceSpeed_solve(struct exti_data *ed) {
    mainLogPrintf("\nmain_solve");
    if (ed->data_writed == 0x0001U+0x0002U) {
        double t1 = ed->timestap[0]*NOW_TIME_TICK;
        double t2 = ed->timestap[1]*NOW_TIME_TICK;
        mainLogPrintf("\n   t: %f, %f s",t1,t2);
        mainLogPrintf("\n   d: %f m",fabs(t1-t2)*340.0);
    } else {
        mainLogPrintf("\n   nodata: ed->data_writed=%x",ed->data_writed);
    }
    ed->data_writed = 0;
}

int main(void)
{
    HAL_Init();                   	//��ʼ��HAL��  
    
    Stm32_Clock_Init(336,8,2,7);  	//����ʱ��,168Mhz
    delay_init(168);                //��ʼ����ʱ����
    uart_init(9600);                //��ʼ��USART
    
    TIM3_Init(999,83); 	            //1KHZ ��ʱ��3����Ϊ1ms
    TIM4_Init(999,839);             //������ɨ���ٶ�,100HZ.
    TIM5_Init(4,83);                //nowTime 200kHZ 0.000 005s.  NOW_TIME_TICK
    TIM14_PWM_Init(5000-1,84-1);    //84M/84=1M�ļ���Ƶ�ʣ��Զ���װ��Ϊ5000����ôPWMƵ��Ϊ1M/5000=200HZ

    LED_Init();						//��ʼ��LED	
    KEY_Init();                     //��ʼ������
    EXTI_Init();                    //�ⲿ�жϳ�ʼ��
    TFTLCD_Init();           	    //��ʼ��LCD FSMC�ӿ�
    TP_Init();				        //��������ʼ��
    
    
    my_mem_init(SRAMIN);			//��ʼ���ڲ��ڴ��
    //my_mem_init(SRAMEX);			//��ʹ���ⲿ�ڴ��
    my_mem_init(SRAMCCM);			//��ʼ��CCM�ڴ��
    
    __HAL_RCC_CRC_CLK_ENABLE();//ʹ��CRCʱ�ӣ�����STemWin����ʹ��

    WM_SetCreateFlags(WM_CF_MEMDEV);//Ϊ�ػ�����Զ�ʹ�ô洢�豸
    GUI_Init();
    GUI_CURSOR_Show();

    //����������
    WM_HWIN CreatemainFramewin(void);
    CreatemainFramewin();
    
    LASER(GPIO_PIN_RESET);
    PWM_SetDegree(-90);
    GUI_Delay(2000);
    PWM_SetDegree(90);  
    GUI_Delay(2000);
    PWM_SetDegree(0);   
    LASER(GPIO_PIN_SET);   
    mainLogPrint("\ninit OK!");
    show_reflesh(0,0);
    while(1)
    {
        //GUI_Delay(500);
        if (myMODE == SIGNLE_RUN || myMODE == TRACK_RUN) {
            main_solve(&ExtiData); 
        }
        //voiceSpeed_solve(&ExtiData); 
        GUI_Exec();
        LED0 = !LED0;
    } 
}

