/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
**********************************************************************
*                                                                    *
*        (c) 1996 - 2012  SEGGER Microcontroller GmbH & Co. KG       *
*                                                                    *
*        Internet: www.segger.com    Support:  support@segger.com    *
*                                                                    *
**********************************************************************

** emWin V5.16 - Graphical user interface for embedded applications **
All  Intellectual Property rights  in the Software belongs to  SEGGER.
emWin is protected by  international copyright laws.  Knowledge of the
source code may not be used to write a similar product.  This file may
only be used in accordance with the following terms:

The software has been licensed to  ARM LIMITED whose registered office
is situated at  110 Fulbourn Road,  Cambridge CB1 9NJ,  England solely
for  the  purposes  of  creating  libraries  for  ARM7, ARM9, Cortex-M
series,  and   Cortex-R4   processor-based  devices,  sublicensed  and
distributed as part of the  MDK-ARM  Professional  under the terms and
conditions  of  the   End  User  License  supplied  with  the  MDK-ARM
Professional. 
Full source code is available at: www.segger.com

We appreciate your understanding and fairness.
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/
#include "GUI.h"
#include "touch.h"
#include "ILI93xx.h"
#include "usart.h"


void GUI_TOUCH_X_ActivateX(void) 
{
 // XPT2046_WriteCMD(0x90);
}


void GUI_TOUCH_X_ActivateY(void)
{
  //XPT2046_WriteCMD(0xd0);
}

const u16 GT9147_TPX_TBL1[5] = {GT_TP1_REG,GT_TP2_REG,GT_TP3_REG,GT_TP4_REG,GT_TP5_REG};

//GT9147 扫描触摸屏(采用查询方式)
void GT9147Touch_Scan(void)
{
    u8 mode=0;
    u8 buf[4];
    u8 temp;
    u8 i;
    GT9147_RD_Reg(GT_GSTID_REG,&mode,1);//读取触摸点的状态
    if((mode&0X80&&((mode&0XF)<6))||(mode&0x20))
	{
        temp=0;
        GT9147_WR_Reg(GT_GSTID_REG,&temp,1);//清标志 		
	}
    if((mode&0XF)&&((mode&0XF)<6))
    {
        temp=~(0xFF << (mode & 0x0F));
        tp_dev.sta = temp|TP_PRES_DOWN|TP_CATH_PRES;
        for(i=0;i<5;i++)
        {
            if(tp_dev.sta & (1<<i))
            {
                GT9147_RD_Reg(GT9147_TPX_TBL1[i],buf,4); //读取 XY 坐标值
                tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
                tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
            }
            if(tp_dev.touchtype&0X01)//横屏
            {
                tp_dev.y[i]=((u16)buf[1]<<8)+buf[0];
                tp_dev.x[i]=800-(((u16)buf[3]<<8)+buf[2]);
            }else
			{
                tp_dev.x[i]=((u16)buf[1]<<8)+buf[0];
                tp_dev.y[i]=((u16)buf[3]<<8)+buf[2];
			} 
        }
        mode=0;
        GT9147_WR_Reg(GT_GSTID_REG,&mode,1);//清标志
    }
    else if((mode&0X8F)==0X80)//无触摸点按下
    {
        if(tp_dev.sta&TP_PRES_DOWN) //之前是被按下的
        {
            tp_dev.sta&=~(1<<7); //标记按键松开
        }else //之前就没有被按下
        {
            tp_dev.x[0]=0xffff;
            tp_dev.y[0]=0xffff;
            tp_dev.sta&=0XE0;//清除点有效标记
        }   
    }
}

//获取电容触摸屏的 X 轴值
int32_t Get_CapacTouchValueX(void)
{
    int32_t lost_x;

    GT9147Touch_Scan();
    lost_x = tp_dev.x[0]; 
    return lost_x;
}
  
//获取电容触摸屏的 Y 轴值
int32_t Get_CapacTouchValueY(void)
{
    int32_t lost_y;

    GT9147Touch_Scan();
    lost_y=tp_dev.y[0];
    return lost_y;
}


int  GUI_TOUCH_X_MeasureX(void) 
{
	int32_t xvalue;
	
	if(lcddev.id == 0X1963) //7寸电容屏的触摸值获取
	{
		tp_dev.scan(0);
		xvalue=tp_dev.x[0];
		return xvalue;

	}
	else if(lcddev.id == 0X5510)//4.3寸电容屏的触摸值获取（单独出来采用查询方式，解决运行一段时间触摸失效问题）
	{
		xvalue= Get_CapacTouchValueX();
        return xvalue;
	}
	else				//电阻屏
	{
		return TP_Read_XOY(0XD0);  //CMD_RDX=0XD0
	}
}

int  GUI_TOUCH_X_MeasureY(void) 
{	
	int32_t yvalue;
	if(lcddev.id == 0X1963)//7寸电容屏的触摸值获取
	{
		tp_dev.scan(0);
		yvalue = tp_dev.y[0];
		return yvalue;
	}
	else if(lcddev.id == 0X5510)//4.3寸电容屏的触摸值获取（单独出来采用查询方式，解决运行一段时间触摸失效问题）
	{
		yvalue=Get_CapacTouchValueY();
        return yvalue;
	}
	else				//电阻屏
	{
		return TP_Read_XOY(0X90);  //CMD_RDX=0XD0
	}
}

