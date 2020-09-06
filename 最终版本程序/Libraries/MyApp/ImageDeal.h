#ifndef _IMAGEDEAL_H_
#define _IMAGEDEAL_H_
#include "headfile.h"

#define Threshold 14        //摄像头二值化阈值 14 18
#define Black_Point 0       //摄像头二值化后，若像素点的灰度数字量小于阈值，判断为黑点，黑点阈值置0 （最暗）
#define White_Point 255     //摄像头二值化后，若像素点的灰度数字量大于阈值，判断为白点，白点阈值置255（最亮）
extern uint16 Dir_Mid;    //图像经过全部处理步骤后，将信标灯光斑中心横坐标MIDX赋给Dir_Mid 
extern uint16 Distance;   //图像经过全部处理步骤后，将信标灯光斑中心纵坐标MIDY赋给Distance
extern uint8 flag_beacon; //检测到信标灯光斑标志，flag_beacon等于1标志没有发现信标灯，相反为0
extern uint16 count_beacon;//信标灯光斑出现次数计数标志位，通过计算信标灯光斑中心横坐标出现的次数（即计算光斑出现的次数），进而判断图像中是否存在信标灯
extern uint16 begin_beacon;//发车（电机启动）标志
extern void find();  //定义find()函数，find()函数核心目的是将原始灰度图像中的白色光斑识别出来，认为该白斑就是信标灯光斑
#define COL     188//图像宽度（列数）    范围1-752     但K60采集不允许超过188，不要修改
#define ROW     120//图像高度（行数）	范围1-480    但K60采集不允许超过120，不要修改
extern void Binaryzation(); //定义具体的图像二值化处理函数Binaryzation()，具体见ImageDeal.c
extern uint8 ADdata1[ROW][COL]; //二值化处理后的数据数据存储于数组ADdata1[ROW][COL]
extern uint32 sumx,sumy; 
extern int16 NewPWMValue; //经过光斑与图像中线的横向偏差计算之后，得出的左侧和右侧电机占空比速度/运动方向修正占空比值
extern void Steering_Engine_PWM();//电机(及车辆运动方向)控制函数 
extern void MotorSpeed(int speedL,int speedR,int speedLL,int speedRR);          //定义多个同类型的pwm占空比电机调速控制函数，多个函数用于控制车车的不同运动状态。此类函数中包含h信标车四个电机的运转参数，四个局部变量依次是前左、前右、后左、后右四个电机的占空比值，占空比值不能超过1000
extern void MotorSpeed1(int speed1,int speed3,int speed11,int speed33);
extern void MotorSpeed2(int speed101,int speed301,int speed1101,int speed3301);
extern void MotorSpeed3(int speed102,int speed302,int speed1102,int speed3302);
extern void MotorSpeed4(int speed103,int speed303,int speed1103,int speed3303);
extern int16 flag_steer; //舵机打角标志，此变量是沿用自上一版c信标车，h信标车不使用，不必理会或修改

extern int SpeedL;       //本h信标车最终决定不使用编码器进行闭环控速，具体缘由请见《算法说明：速度控制篇》。以下这些闭环pid电机速度控制变量,均未使用
extern int SpeedR;
extern int Speed;
extern int NewL;
extern int NewR;
extern int GoalSpeedL;
extern int GoalSpeedR;
extern uint16 Steer_Mid;
extern uint16 SeekSpeedR;
extern uint16 SeekSpeedL;
extern uint16 SeekSpeedLL;
extern uint16 SeekSpeedRR;
extern void motor();

#endif