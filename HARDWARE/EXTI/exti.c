#include "exti.h"
#include "delay.h"
#include "led.h"
#include "key.h"


struct exti_data ExtiData = {0};

//外部中断初始化
void EXTI_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    
    __HAL_RCC_GPIOC_CLK_ENABLE();               //开启GPIOC时钟
    
    GPIO_Initure.Mode=GPIO_MODE_IT_RISING;      //上升沿触发
    GPIO_Initure.Pull=GPIO_PULLDOWN;            //下拉
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;
    GPIO_Initure.Pin=GPIO_PIN_0;                //PC0
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_1;                //PC1
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    GPIO_Initure.Pin=GPIO_PIN_2;                //PC2
    HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_3;                //PC3
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_4;                //PC4
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);
    
    // GPIO_Initure.Pin=GPIO_PIN_5;                //PC5
    // HAL_GPIO_Init(GPIOC,&GPIO_Initure);

    //中断线0-PC0
    HAL_NVIC_SetPriority(EXTI0_IRQn,2,0);       //抢占优先级为2，子优先级为0
    HAL_NVIC_EnableIRQ(EXTI0_IRQn);             //使能中断线0

    //中断线1-PC1
    HAL_NVIC_SetPriority(EXTI1_IRQn,2,1);       //抢占优先级为2，子优先级为1
    HAL_NVIC_EnableIRQ(EXTI1_IRQn);             //使能中断线0
    
    //中断线2-PC2
    HAL_NVIC_SetPriority(EXTI2_IRQn,2,2);   	//抢占优先级为2，子优先级为2
    HAL_NVIC_EnableIRQ(EXTI2_IRQn);         	//使能中断线4

    ExtiData.data_len = 3;
}


//中断服务函数
void EXTI0_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);		//调用中断处理公用函数
}

void EXTI1_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);		//调用中断处理公用函数
}

void EXTI2_IRQHandler(void)
{
    HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);		//调用中断处理公用函数
}

// void EXTI3_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3);		//调用中断处理公用函数
// }

// void EXTI4_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);		//调用中断处理公用函数
// }

// void EXTI5_IRQHandler(void)
// {
//     HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);		//调用中断处理公用函数
// }


//中断服务程序中需要做的事情
//在HAL库中所有的外部中断服务函数都会调用此函数
//GPIO_Pin:中断引脚号
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    uint32_t nowTime = SysTick->VAL;
    switch (GPIO_Pin) {
    case GPIO_PIN_0:
        if ((ExtiData.data_writed & GPIO_Pin) == 0) {
            ExtiData.data_writed |= GPIO_Pin;
            ExtiData.timestap[0] = nowTime;
            // HAL_NVIC_DisableIRQ(EXTI0_IRQn);
        }
        break;
    case GPIO_PIN_1:
        if ((ExtiData.data_writed & GPIO_Pin) == 0) {
            ExtiData.data_writed |= GPIO_Pin;
            ExtiData.timestap[1] = nowTime;
            // HAL_NVIC_DisableIRQ(EXTI1_IRQn);
        }
        break;
    case GPIO_PIN_2:
        if ((ExtiData.data_writed & GPIO_Pin) == 0) {
            ExtiData.data_writed |= GPIO_Pin;
            ExtiData.timestap[2] = nowTime;
            // HAL_NVIC_DisableIRQ(EXTI2_IRQn);
        }
        break;
    default:
        break;
    }
    return;

}
