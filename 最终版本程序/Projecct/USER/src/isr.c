/*********************************************************************************************************************
* COPYRIGHT NOTICE
* Copyright (c) 2017,逐飞科技
* All rights reserved.
* 技术讨论QQ群：179029047
*
* 以下所有内容版权均属逐飞科技所有，未经允许不得用于商业用途，
* 欢迎各位使用并传播本程序，修改内容时必须保留逐飞科技的版权声明。
*
* @file       		isr
* @company	   		成都逐飞科技有限公司
* @author     		Go For It(1325536866)
* @version    		v2.0
* @Software 		IAR 7.7 or MDK 5.17
* @Target core		MK60DN and MK60FX
* @Taobao   		https://seekfree.taobao.com/
* @date       		2017-09-19
********************************************************************************************************************/

#include "isr.h"  




//-------------------------------------------------------------------------------------------------------------------
//  @brief      PROTA中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当A口启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void PORTA_IRQHandler(void)    //PROTA中断执行函数,此处不要改动
{
  //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
  PORTA->ISFR = 0xffffffff;
  //使用我们编写的宏定义清除发生中断的引脚
  //PORTA_FLAG_CLR(A1);
  
}


void PORTC_IRQHandler(void)    //PROTC中断执行函数,此处不要改动
{
  //清除中断标志第一种方法直接操作寄存器，每一位对应一个引脚
  //PORTC->ISFR = 0xffffffff;
  //使用我们编写的宏定义清除发生中断的引脚
  PORTC_FLAG_CLR(C6);
  VSYNC();
}


void DMA0_IRQHandler(void)    //DMA中断执行函数,此处不要改动
{
  DMA_IRQ_CLEAN(DMA_CH0);
  row_finished();
  
}


int16 flag_steer=0;        //舵机打角标志，此变量是沿用自上一版c信标车，h信标车不使用，不必理会或修改

uint16 Begin=0;
uint8 Reversing_flag=0;
uint8 time_count=0;
uint8 count=0;
void PIT0_IRQHandler(void)   //PIT0号定时器中断执行函数，此处的MotorSpeed2和MotorSpeed3中的局部变量值需要调试修改
{
  PIT_FlAG_CLR(pit0);
  flag_steer++;             //舵机打角标志
  if(flag_steer>=1)         //舵机打角标志大于等于1时，即舵机有正常的角度输出的情况下，才运行if所包含的条件语句（此语句是沿用自上一版c信标车，h信标车不使用，不必理会或修改）
  {
    flag_steer=0;
    if(mt9v032_finish_flag)           //读取到一场图像采集完成标志位
    {
      mt9v032_finish_flag = 0;        //将一场图像采集完成标志位置0
      find();                         //find()函数跳转到ImageDeal.c具体可见,用于识别摄像头采集到的图像中的信标灯光斑
      
      
   //以下这段条件语句，用于通过接触开关（红外光电传感器）实现壁障
      if(Infra_redL==0&&Infra_redR==0&&Infra_redM==0)//右侧或中间及左侧的接触开关检测到障碍
      {
        ftm_pwm_duty(ftm1,ftm_ch0,915);    //上一版C车程序需要舵机打角以规避障碍，h信标车不需此句程序，无需修改
        MotorSpeed4(260,290,260,290);   //自旋躲墙
      }
      else if(Infra_redL==0)               //左侧接触开关检测到障碍
      {
        ftm_pwm_duty(ftm1,ftm_ch0,740);    //上一版C车程序需要舵机打角以规避障碍，h信标车不需此句程序，无需修改
        MotorSpeed2(400,430,400,430);      //车子左侧有障碍，车子向右侧水平移动规避障碍500 530
        //MotorSpeed4(260,290,260,290);
      }
      else if(Infra_redR==0||Infra_redM==0)//右侧或中间的接触开关检测到障碍
      {
        ftm_pwm_duty(ftm1,ftm_ch0,915);    //上一版C车程序需要舵机打角以规避障碍，h信标车不需此句程序，无需修改
        MotorSpeed3(400,430,400,430);      //车子右侧或前方有障碍，车子向左侧水平移动规避障碍
        //MotorSpeed4(260,290,260,290);
      }
      /*
      else if(Infra_redL==0&&Infra_redR==0&&Infra_redM==0)//右侧或中间的接触开关检测到障碍
      {
        ftm_pwm_duty(ftm1,ftm_ch0,915);    //上一版C车程序需要舵机打角以规避障碍，h信标车不需此句程序，无需修改
        MotorSpeed4(260,290,260,290);
      }
      */
      else                                 //无障碍时
      {
        Steering_Engine_PWM();            //无障碍时，执行电机(及车辆运行方向)控制函数Steering_Engine_PWM()，Steering_Engine_PWM()跳转至ImageDeal.c可见
     } 
    }
  } 
}



//-------------------------------------------------------------------------------------------------------------------
//  @brief      UART3中断执行函数
//  @return     void   
//  @since      v1.0
//  Sample usage:               当UART3启用中断功能且发生中断的时候会自动执行该函数
//-------------------------------------------------------------------------------------------------------------------
void UART3_RX_TX_IRQHandler(void)
{
  if(UART3->S1 & UART_S1_RDRF_MASK)                                     //接收数据寄存器满
  {
    //用户需要处理接收数据
    mt9v032_cof_uart_interrupt();
  }
  if(UART3->S1 & UART_S1_TDRE_MASK )                                    //发送数据寄存器空
  {
    //用户需要处理发送数据
    
  }
}


/*
中断函数名称，用于设置对应功能的中断函数
Sample usage:当前启用了DMA0中断，然后就到下面去找哪个是DMA0的中断函数名称，找到后写一个该名称的函数即可
void DMA0_IRQHandler(void)
{
;
}
记得进入中断后清除标志位


DMA0_IRQHandler
DMA1_IRQHandler
DMA2_IRQHandler
DMA3_IRQHandler
DMA4_IRQHandler
DMA5_IRQHandler
DMA6_IRQHandler
DMA7_IRQHandler
DMA8_IRQHandler
DMA9_IRQHandler
DMA10_IRQHandler
DMA11_IRQHandler
DMA12_IRQHandler
DMA13_IRQHandler
DMA14_IRQHandler
DMA15_IRQHandler
DMA_Error_IRQHandler
MCM_IRQHandler
FTFE_IRQHandler
Read_Collision_IRQHandler
LVD_LVW_IRQHandler
LLW_IRQHandler
Watchdog_IRQHandler
RNG_IRQHandler
I2C0_IRQHandler
I2C1_IRQHandler
SPI0_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
CAN0_ORed_Message_buffer_IRQHandler
CAN0_Bus_Off_IRQHandler
CAN0_Error_IRQHandler
CAN0_Tx_Warning_IRQHandler
CAN0_Rx_Warning_IRQHandler
CAN0_Wake_Up_IRQHandler
I2S0_Tx_IRQHandler
I2S0_Rx_IRQHandler
CAN1_ORed_Message_buffer_IRQHandler
CAN1_Bus_Off_IRQHandler
CAN1_Error_IRQHandler
CAN1_Tx_Warning_IRQHandler
CAN1_Rx_Warning_IRQHandler
CAN1_Wake_Up_IRQHandler
Reserved59_IRQHandler
UART0_LON_IRQHandler
UART0_RX_TX_IRQHandler
UART0_ERR_IRQHandler
UART1_RX_TX_IRQHandler
UART1_ERR_IRQHandler
UART2_RX_TX_IRQHandler
UART2_ERR_IRQHandler
UART3_RX_TX_IRQHandler
UART3_ERR_IRQHandler
UART4_RX_TX_IRQHandler
UART4_ERR_IRQHandler
UART5_RX_TX_IRQHandler
UART5_ERR_IRQHandler
ADC0_IRQHandler
ADC1_IRQHandler
CMP0_IRQHandler
CMP1_IRQHandler
CMP2_IRQHandler
FTM0_IRQHandler
FTM1_IRQHandler
FTM2_IRQHandler
CMT_IRQHandler
RTC_IRQHandler
RTC_Seconds_IRQHandler
PIT0_IRQHandler
PIT1_IRQHandler
PIT2_IRQHandler
PIT3_IRQHandler
PDB0_IRQHandler
USB0_IRQHandler
USBDCD_IRQHandler
ENET_1588_Timer_IRQHandler
ENET_Transmit_IRQHandler
ENET_Receive_IRQHandler
ENET_Error_IRQHandler
Reserved95_IRQHandler
SDHC_IRQHandler
DAC0_IRQHandler
DAC1_IRQHandler
TSI0_IRQHandler
MCG_IRQHandler
LPTimer_IRQHandler
Reserved102_IRQHandler
PORTA_IRQHandler
PORTB_IRQHandler
PORTC_IRQHandler
PORTD_IRQHandler
PORTE_IRQHandler
PORTF_IRQHandler
Reserved109_IRQHandler
SWI_IRQHandler
NFC_IRQHandler
USBHS_IRQHandler
Reserved113_IRQHandler
CMP3_IRQHandler
Reserved115_IRQHandler
Reserved116_IRQHandler
FTM3_IRQHandler
ADC2_IRQHandler
ADC3_IRQHandler
I2S1_Tx_IRQHandler
I2S1_Rx_IRQHandler
DefaultISR
*/



