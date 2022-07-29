/************************************************
fork from F4_HAL_emwin https://github.com/framist/STemWinForHAL
实现平台 ALIENTEK STM32F407ZGT6最小系统版
AUEDC 2022

版级接口：

printf 已重载为串口输出

TIM14 PA7 用作PWM输出
PC0~PC5 用作触发输入
PF8 作为激光笔控制输出

耦合一时爽，重构火葬场！
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
//GUI支持
#include "GUI.h"
#include "FramewinDLG.h"

__IO uint32_t nowTime;

// 设置 DS3120 角度
// 1000~2500ns -> 0~270 deg
void PWM_SetDegree(double deg)
{
    LASER(GPIO_PIN_SET);   //关闭激光笔
    deg += 90;
    deg = deg * 90 / 106; // 角度初步修正
    deg = (deg - 0.2193)/1.0221 ; // 角度根据激光修正
    u32 compare = 5000 - (u32)(deg/180.0 * 1500 + 1000);
	TIM_SetTIM14Compare1(compare);
    // mainLogPrintf("\nset deg: %f deg, %u cmp",deg,compare);
    LASER(GPIO_PIN_RESET);   
}

// 主计算函数
//#define NOW_TIME_TICK (1/168000000.0)
#define NOW_TIME_TICK (1/200000.0)
#include "math.h"
void main_solve(struct exti_data *ed) {
    //mainLogPrintf("\nmain_solve:");
    if (ed->data_writed == 0x0001U+0x0002U+0x0004U) {
        double t1 = ed->timestap[0]*NOW_TIME_TICK;
        double t2 = ed->timestap[1]*NOW_TIME_TICK;
        double t3 = ed->timestap[2]*NOW_TIME_TICK;

        double tau1 = t2 - t3;
        double tau2 = t1 - t2; 
        double v = 340.0;   // 声速 m/s
        double d = 0.5;       // 相邻传感器之间的距离 m
        double theta, gamma;
        gamma = (2*pow(d,2) - pow(tau2*v,2) + pow(tau1*v,2) - 2*tau1*tau2*pow(v,2))
                /(2*(tau2-tau1)*v);
        gamma = gamma + tau1*v;
        theta = PI/2 - acos( (pow(gamma,2) + pow(d,2) - pow(gamma+tau2*v,2))
                /(2*(gamma)*d) );
        theta = theta * 180.0 / PI;
        mainLogPrintf("\n[+]solved t: %.8f, %.8f, %.8f s\n    tau: %.8f, %.8f",t1,t2,t3,tau1,tau2);
        mainLogPrintf("\n   theta: %f deg; gamma: %f m",theta,gamma);
        
        //剔除坏值
        if ( -60 < theta && theta < 60 
            && fabs(tau1)+fabs(tau2) < 0.005 // <1.5e-4 * 2
            /* && 0 < gamma && gamma < 5*/) {
            PWM_SetDegree(theta);
            show_reflesh(theta*1.0362-1.4403, 0.3869*gamma+1.8659);
        }
        
        //消抖
        while (1) {
            GUI_Delay(20);
            if (ed->data_writed == 0x0U) {
                break;
            } else {
                ed->data_writed = 0x0U;
            }
        }
        
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
    HAL_Init();                   	//初始化HAL库  
    
    Stm32_Clock_Init(336,8,2,7);  	//设置时钟,168Mhz
	delay_init(168);                //初始化延时函数
	uart_init(9600);                //初始化USART
    
    TIM3_Init(999,83); 	            //1KHZ 定时器3设置为1ms
    TIM4_Init(999,839);             //触摸屏扫描速度,100HZ.
    TIM5_Init(4,83);                //nowTime 200kHZ 0.000 005s.  NOW_TIME_TICK
    TIM14_PWM_Init(5000-1,84-1);    //84M/84=1M的计数频率，自动重装载为5000，那么PWM频率为1M/5000=200HZ

	LED_Init();						//初始化LED	
    KEY_Init();                     //初始化按键
	EXTI_Init();                    //外部中断初始化
	TFTLCD_Init();           	    //初始化LCD FSMC接口
    TP_Init();				        //触摸屏初始化
    
	
	my_mem_init(SRAMIN);			//初始化内部内存池
	//my_mem_init(SRAMEX);			//不使用外部内存池
	my_mem_init(SRAMCCM);			//初始化CCM内存池
    
    __HAL_RCC_CRC_CLK_ENABLE();//使能CRC时钟，否则STemWin不能使用

	WM_SetCreateFlags(WM_CF_MEMDEV);//为重绘操作自动使用存储设备
	GUI_Init();
    GUI_CURSOR_Show();

    //创建主窗口
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

    while(1)
	{
		//GUI_Delay(500);

        main_solve(&ExtiData); 
        
        //voiceSpeed_solve(&ExtiData); 
        //show_nowTime(SysTick->VAL);
        GUI_Exec();
        LED0 = !LED0;
	} 
}

