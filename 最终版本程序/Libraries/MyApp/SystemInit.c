#include "headfile.h"

void SystemInit()  
{
  get_clk();                   //上电后必须运行一次这个函数，获取各个模块的频率信息，便于后面各个模块的参数设置
  gpio_init(A25,GPO,0);        //A25接口定义但暂未使用
                              
  //拨码开关初始化
  gpio_init(D11, GPI,0);       //定义D11接口为IO接口，置0
  gpio_init(D12, GPI,0);       //同上
  gpio_init(D13, GPI,0);
  gpio_init(D14, GPI,0);             
  gpio_init(D15, GPI,0);
  gpio_init(E0, GPI,0);
  gpio_init(E1, GPI,0);
  gpio_init(E2, GPI,0);
  port_init_NoAlt(D11,PULLUP);//将D11接口拉高
  port_init_NoAlt(D12,PULLUP);//同上
  port_init_NoAlt(D13,PULLUP);
  port_init_NoAlt(D14,PULLUP);
  port_init_NoAlt(D15,PULLUP);
  port_init_NoAlt(E0,PULLUP);
  port_init_NoAlt(E1,PULLUP);
  port_init_NoAlt(E2,PULLUP);  
                          
  Switch();                   //通过拨码开关2号档至8号档选择7个速度档之一，以实现车子的速度控制和运动方向控制，Switch()具体见SystemDeal.c下方
  
  ftm_pwm_init(ftm1,ftm_ch0,50,830);//舵机初始化（上一版C车程序需要舵机打角以控制车子运动方向，h信标车不需此句程序，无需修改）
  
 //电机初始化，初始化电机频率为0
  ftm_pwm_init(MOTOR_FTM,MOTOR1_PWM,MOTOR_HZ,0);   //前左轮
  ftm_pwm_init(MOTOR_FTM,MOTOR2_PWM,MOTOR_HZ,0);
  ftm_pwm_init(MOTOR_FTM,MOTOR3_PWM,MOTOR_HZ,0);   //前右轮
  ftm_pwm_init(MOTOR_FTM,MOTOR4_PWM,MOTOR_HZ,0);
  
  ftm_pwm_init(MOTOR_FTM,MOTOR5_PWM,MOTOR_HZ,0);   //后左轮
  ftm_pwm_init(MOTOR_FTM,MOTOR6_PWM,MOTOR_HZ,0);
  ftm_pwm_init(MOTOR_FTM,MOTOR7_PWM,MOTOR_HZ,0);   //后右轮
  ftm_pwm_init(MOTOR_FTM,MOTOR8_PWM,MOTOR_HZ,0);
  
                                                      //编码器初始化（本h信标车最终决定不使用编码器进行闭环控速，具体解释请见《算法说明：速度控制篇》）
//  ftm_quad_init(ftm2);                              //编码器左
//  port_init_NoAlt(FTM2_QDPHA_PIN,PULLUP);
//  lptmr_pulse_init(lpt0_alt1,0xFFFF,LPT_Rising);    //编码器右
  
 
  uart_init (uart2, 115200);                          //初始换串口以与电脑通信（本车在调试时不需用串口通信，通过TFT显示屏的图像足以调试车辆参数）
  
  lcd_init();            //小液晶初始化                    
 
  camera_init();         //摄像头初始化
   
  UltrasonicInit();      //超声波初始化（本车不使用超声波传感器壁障，忽略此语句）
 
  led_init(LED_MAX);     //核心板的LED灯初始化  
  
  pit_init_ms(pit0,5);   //pit定时器（0号）初始化，程序运行周期设定为5ms ，此周期请勿修改
  
  enable_irq (PIT0_IRQn);//开0号定时器PIT0中断
  
  gpio_init(E10,GPI,0);  //红外对管初始化
  gpio_init(E8,GPI,0); 
  gpio_init(E9,GPI,0); 
  
  set_irq_priority(DMA0_IRQn,2);//摄像头图像DMA传输中断初始化设置
  set_irq_priority(PIT0_IRQn,3);//速度控制和图像处理程序中断初始化设置
 
  
}

void UltrasonicInit()      //超声波信号adc模块采集函数（本车不使用超声波传感器壁障，忽略此段语句）
{
  adc_init(ADC0_SE8);//B0
  adc_init(ADC0_SE9);//B1
  adc_init(ADC0_SE12);//B2
  adc_init(ADC0_SE13);//B3
  adc_init(ADC1_SE10);//B4
  adc_init(ADC1_SE11);//B5
  adc_init(ADC1_SE12);//B6
  adc_init(ADC1_SE13);//B7
}


uint8 status2=1;          //定义拨码开关2号档对应的变量status2并赋值为1
uint8 status3=1;          //同上
uint8 status4=1;
uint8 status5=1; 
uint8 status6=1;
uint8 status7=1; 
uint8 status8=1; 
uint16 speed0=0;          //定义全局速度变量speed0并赋值为0
uint16 speed1=0;          //同上
uint16 speed2=0;
uint16 speed3=0;
uint16 speed4=0;
uint16 speed5=0;
uint16 speed6=0;

void Switch()            //通过拨码开关2号档至8号档选择7个速度档之一，以实现车子的速度控制和运动方向控制
{
  status2= gpio_get(D12);//将D12接口读取到的电平值赋给拨码开关2号档对应的变量status2
  status3= gpio_get(D13);//同上
  status4= gpio_get(D14);
  status5= gpio_get(D15);
  status6= gpio_get(E0);
  status7= gpio_get(E1);
  status8= gpio_get(E2);
  
 if(status2==0)         //(2 312 500)
  {                     //(3 363 550)
    speed0=1;           //(4 365 550 过弯稳)
    speed1=0;           //(5 395 600)
    speed2=0;           //(6 396 600)
    speed3=0;           //(7 411 600)
    speed4=0;           //(8 314 400 同归于尽玩法)
    speed5=0;
    speed6=0;
    Steer_Mid=60;       //舵机中值设定（上一版C车程序需要舵机打角以控制车子运动方向，h信标车不需此句程序，无需修改）
    SeekSpeedL=312;     //在2号档中，前左轮速度值设定，最高值1000（超过1000会烧毁电机驱动芯片！）
    SeekSpeedR=500;     //在2号档中，前右轮速度值设定，最高值1000（超过1000会烧毁电机驱动芯片！）
    SeekSpeedLL=312;    //在2号档中，后左轮速度值设定，最高值1000（超过1000会烧毁电机驱动芯片！） 
    SeekSpeedRR=500;    //在2号档中，后右轮速度值设定，最高值1000（超过1000会烧毁电机驱动芯片！）
  }
   else if(status3==0) //同上
  { 
    speed0=0;
    speed1=1;
    speed2=0;
    speed3=0;
    speed4=0;
    speed5=0;
    speed6=0;
    Steer_Mid=60;
    SeekSpeedL=363;
    SeekSpeedR=550;
    SeekSpeedLL=363;
    SeekSpeedRR=550;
  }
  else if(status4==0) //同上
  { 
    speed0=0;
    speed1=0;
    speed2=1;
    speed3=0;
    speed4=0;
    speed5=0;
    speed6=0;
    Steer_Mid=60;
    SeekSpeedL=365;
    SeekSpeedR=550;
    SeekSpeedLL=365;
    SeekSpeedRR=550;
  }
  else if(status5==0) //同上
  { 
    speed0=0;
    speed1=0;
    speed2=0;
    speed3=1;
    speed4=0;
    speed5=0;
    speed6=0;
    Steer_Mid=60;
    SeekSpeedL=420;
    SeekSpeedR=600;
    SeekSpeedLL=420;
    SeekSpeedRR=600;
  }
  else if(status6==0) //同上
  {
    speed0=0;
    speed1=0;
    speed2=0;
    speed3=0;
    speed4=1;    
    speed5=0;
    speed6=0;
    Steer_Mid=60;
    SeekSpeedL=425;
    SeekSpeedR=600;
    SeekSpeedLL=425;
    SeekSpeedRR=600;
  }
  else if(status7==0) //同上
  {
    speed0=0;
    speed1=0;
    speed2=0;
    speed3=0;
    speed4=0;    
    speed5=1;
    speed6=0;
    Steer_Mid=60;
    SeekSpeedL=430;
    SeekSpeedR=600;
    SeekSpeedLL=430;
    SeekSpeedRR=600;
  }
  else if(status8==0) //同上
  {
    speed0=0;
    speed1=0;
    speed2=0;
    speed3=0;
    speed4=0;    
    speed5=0;
    speed6=1;
    Steer_Mid=60;
    SeekSpeedL=486;
    SeekSpeedR=650;
    SeekSpeedLL=486;
    SeekSpeedRR=650;
  }
}
  