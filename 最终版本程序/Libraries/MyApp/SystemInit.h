#ifndef  __SYSTEMINIT_H_
#define  __SYSTEMINIT_H_

#include "headfile.h"

//拨码开关接口定义
#define Key1 gpio_get(D11)
#define Key2 gpio_get(D12)
#define Key3 gpio_get(D13)
#define Key4 gpio_get(D14)
#define Key5 gpio_get(D15)
#define Key6 gpio_get(E0)
#define Key7 gpio_get(E1)
#define Key8 gpio_get(E2)
//电机接口
#define MOTOR_FTM   ftm0                //FTM0模块
#define MOTOR1_PWM  ftm_ch0		//PTC1     通道0
#define MOTOR2_PWM  ftm_ch1		//PTC2     通道1
#define MOTOR3_PWM  ftm_ch2		//PTC3     通道2
#define MOTOR4_PWM  ftm_ch3		//PTC4     通道3
#define MOTOR5_PWM  ftm_ch4             //PTD4     通道4
#define MOTOR6_PWM  ftm_ch5             //PTD5     通道5
#define MOTOR7_PWM  ftm_ch6             //PTD6     通道6
#define MOTOR8_PWM  ftm_ch7             //PTD7     通道7
#define MOTOR_HZ    (10*1000)           //电机频率（赫兹）

//函数声明及变量定义
extern void SystemInit(void);
extern void UltrasonicInit();
extern void Switch();
extern uint16 speed0;
extern uint16 speed1;
extern uint16 speed2;
extern uint16 speed3;
extern uint16 speed4;
extern uint16 speed5;
extern uint16 speed6;

extern uint16 Infra_redL;
extern uint16 Infra_redR;
extern uint16 Infra_redM;


#endif
