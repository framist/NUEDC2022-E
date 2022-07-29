#ifndef _EXTI_H
#define _EXTI_H
#include "sys.h"

/**
 * @brief 
 * 开始置0，
 * 数据写入置1，并记录时间戳，
 * 若发现重复写入，不写入（消抖）
 * 读取数据后，全部置零
 */
struct exti_data
{
    u16 data_len; //数据长度
    u16 data_writed;    //已写入的数据位置1
    u32 timestap[10];
} ;
extern struct exti_data ExtiData;


void EXTI_Init(void);
#endif
