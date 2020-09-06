/********************************************
逐飞科技 总钻风-摄像头  历程
Designed by Fly Sir
软件版本:V1.1
最后更新:2016年5月3日
相关信息参考下列地址：
淘宝店：https://seekfree.taobao.com/
------------------------------------
软件版本： IAR 7.2 or MDK 5.17
目标核心： MK60DN512VLL10
============================================
MT9V032接线定义：
------------------------------------ 
模块管脚            单片机管脚
SDA(51的RX)         C17
SCL(51的TX)         C16
场中断              C6
行中断				未使用，因此不接
像素中断            C18            
数据口              C8-C15 

TFT1.8液晶                
SCL                 A15
SDA                 A16        
RES                 B16    
DC                  B17
CS					A14
============================================

分辨率是                188*120
摄像头参数设置可以到    SEEKFREE-->h_file-->SEEKFREE_MT9V032.h

总钻风-摄像头测试步骤：
1.下载程序到开发板
2.插上串口线或者USB转TTL
3.接好
uint8 status1=0; 
uint8 status2=0; MT9V032模块接线
4.通电在TFT液晶上即可观看    
*********************************************/  
#include "headfile.h" //在headfile.h中定义一系列头文件

uint8 status1=0;      //定义变量status1，status1用于储存拨码开关1号档（连接k60D11引脚）的电平值并将其最初置0
uint16 Infra_redL=1;  //定义变量Infra_redL用于存储左侧的接触开关（红外光电传感器）的输出电平值并将其最初置0
uint16 Infra_redR=1;  //同上，右侧
uint16 Infra_redM=1;  //同上，中侧
char ch;
int main(void)        //mian函数即整个程序的主函数，所有主要子函数都必须在mian函数下按顺序依次运行
{
  SystemInit();       //SystemInit()函数中主要包含了信标车所使用的主要变量和参数的定义及具体设定值，打开SystemInit()可详查
  for(;;)             
  { 
    Binaryzation();             // Binaryzation()为图像二值化库函数，只用静态阈值。此库函数我们仅使用，不要做任何修改。
    status1= gpio_get(D11);     //status1在此读取并储存拨码开关1号档（连接k60D11引脚）的电平值
    
    Infra_redL = gpio_get(E10); //Infra_redL在此读取并存储左侧的接触开关（红外光电传感器）的输出电平值
    Infra_redR = gpio_get(E8);  //同上，右侧
    Infra_redM = gpio_get(E9);  //同上，中侧
#if 0
    OutData[0]=GoalSpeedL;       //SCI示波器发送函数(使用上位机显示GoalSpeedL、SpeedL、GoalSpeedR、SpeedR变量的值)，仅研制样车辅助使用，新手不必研究。
    OutData[1]=SpeedL;         
    OutData[2]=GoalSpeedR;
    OutData[3]=SpeedR;
    OutPut_Data();
#endif
    if(flag_beacon==0)       //k60芯片上led小灯通过亮灭表明摄像头的图像经过处理后，通过标志变量flag_beacon的0或1值，表示是否发现信标灯光斑。
    {
      led (LED0,LED_ON);     //如果标志变量flag_beacon等于0，led小灯亮，表明发现信标灯
    }
    else if(flag_beacon==1)
    {
      led (LED0,LED_OFF);    //如果标志变量flag_beacon等于1，led小灯灭，表明未发现信标灯
    }
    if(mt9v032_finish_flag)  //读取到一场图像采集完成标志位 
    {
      mt9v032_finish_flag = 0;//将一场图像采集完成标志位置0
     if(status1==1)           //若拨码开关1号档电平值为1，则TFT显示屏显示摄像头采集到的原始灰度图像
      {
        displayimage032(image[0]); 
        status1=0;            //TFT显示原始灰度图像后，重新再次将status1置0
      }
      else if(status1==0)     //若拨码开关1号档电平值为0，则TFT显示屏显示摄像头采集到的原始灰度图像经过二值化处理后的二值化图像
      {
        displayimage032(ADdata1[0]);//处理后的二值化图像显示在TFT显示屏上
        status1=0;            //TFT显示图像后，重新再次将status1置0
      }
    }
  }
}

