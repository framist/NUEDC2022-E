#ifndef _FRAMEWINDLG_H
#define _FRAMEWINDLG_H
#include "sys.h"
#include "WM.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK STM32F103战舰开发板
//STemWin GUIBulider使用
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//创建日期:2015/4/15
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

WM_HWIN CreatemainFramewin(void);


void mainLogPrint(const char *log);
void mainLogPrintf(const char *fmt, ...);
void show_nowTime(uint32_t time);
void show_reflesh(double theta, double gamma);

extern WM_HWIN hWin_LogFramewin;
extern WM_HWIN hWin_ShowWindow;

#endif
