#include "headfile.h"

uint16 X_sum = 0;                //光斑横坐标求和
uint16 Y_sum = 0;                //光斑纵坐标求和
uint16 threshold=14;            //定义图像二值化阈值，此处Threshold值须与ImageDeal.h中的Threshold值修改一致
uint8  ADdata1[ROW][COL];        //经过二值化处理后的图像数据储存于此数组中，处理后的二值化图像会显示在TFT显示屏上
uint16 MIDX = 0;                 //定义光斑中心位置横坐标并置0
uint16 MIDY = 0;                 //定义光斑中心位置纵坐标并置0
uint16 i;
uint16 j;
uint16 Dir_Mid;       //图像经过全部处理步骤后，将信标灯光斑中心横坐标MIDX赋给Dir_Mid 
uint16 Distance;      //图像经过全部处理步骤后，将信标灯光斑中心纵坐标MIDY赋给Distance
uint8 flag_beacon=0;  //检测到信标灯光斑标志，flag_beacon等于1标志没有发现信标灯，相反为0。初始化置0
uint16 count_beacon=0;//定义信标灯光斑出现次数计数标志位，通过计算信标灯光斑中心横坐标出现的次数（即计算光斑出现的次数），进而判断图像中是否存在信标灯
uint16 begin_beacon=0;//发车（电机启动）标志，初始化置0


void Binaryzation()  //此函数是具体的图像二值化处理函数,处理后的二值化图像显示在TFT显示屏上
{ 
  
  uint16 m,n;            //m和n两个变量分别用于图像的行计数和列计数
  for(m=0; m<ROW; m++)   //m的上限即图像的高度（行数）ROW，ROW的具体值请见ImageDeal.h
  { 
    for(n=0; n<COL; n++) //n的上限即图像的宽度（列数）COL，COl的具体值请见ImageDeal.h
    { 
      if(image[m][n]>Threshold) //对于某个像素点（该点横坐标位置为m，纵坐标位置为n），其灰度数字量image[m][n]，若大于二值化阈值Threshold则将image[m][n]置255，则判断其为灰度亮点（判断为白点），进而将ADdata1[m][n]经过二值化处理后的图像数据255储存于数组ADdata1[ROW][COL]中
        ADdata1[m][n] = 255; 
      else                     //对于某个像素点（该点横坐标位置为m，纵坐标位置为n），其灰度数字量image[m][n]，若小于二值化阈值Threshold则将image[m][n]置0，则判断其为灰度暗点（判断为黑点），进而将ADdata1[m][n]经过二值化处理后的图像数据0储存于数组ADdata1[ROW][COL]中
        ADdata1[m][n] = 0;     
      
    }
  }
}


void find()                  //定义find()函数，find()函数核心目的是将原始灰度图像中的白色光斑识别出来，认为该白斑就是信标灯光斑。 ——请逐行阅读程序及注释，并结合《算法说明：图像处理篇》来逐步理解
{
  uint16 n = 0;  
  uint8 data[1000][2];       //定义存放信标灯亮斑的横、纵坐标的数组data[1000][2]
  uint32 sumx = 0,sumy = 0;  
  n=0;
  for(i=0;i<ROW-35;i++)   //舍弃最近35行（即原始灰度图像的最下方35行舍弃），以提高图像处理的效率。因为无论信标灯距离车子远近，图像的最下方35行均作用不大或者没有用途，故忽略掉。
  {
    for(j=0;j<COL-1;j++)  //对于某一横行i，从最左列（j=0号列）为基准水平向右数三列，即连续数三个像素点。如果这三个像素点都是亮点，进而记录这三个点的纵坐标之和及横坐标之和。然后在该横行向右平移一列（j=1）为基准，再向右数三列进行相同的操作直至完成对该横行所有像素点的判断。这是对每3个亮点的横坐标求均值，进而求得整幅图像中所有亮斑的中心点横坐标 （无环境杂光干扰，则准确求得信标灯光斑中心横坐标；有环境杂光干扰，也可以把干扰光斑的影响降到最低，进而求出大致或较准确的信标灯光斑中心横坐标）。。。纵坐标也同理。
    {
      if((image[i][j-1]>Threshold)&&(image[i][j]>Threshold)&&(image[i][j+1]>Threshold))//连续三个点判断为信标
      {
        data[n][1]  = i; //记录三个亮点的纵坐标之和
        data[n++][0] = j;//记录三个亮点的横坐标之和
        if(n >=1000)     //将数组data[1000][2]的各个维的元素的数量限制为最高1000个，超过则清零。数组的容量应设定为有限个
          n = 0;
      }
    }
  }
  
  for(i=0;i<n;i++)
  {                               
    sumx += data[i][0];//sumx += data[i][0]是将每行所求得的若干组中值累加求和，然后除以其总组数，即MIDX = (uint16)sumx/n ，至此求出最终的信标灯光斑中心位置横坐标MIDX
    sumy += data[i][1];//纵坐标，道理同横坐标
  }
  
  MIDX = (uint16)sumx/n;        
  MIDY = (uint16)sumy/n;
       
  if(MIDX==0)          //如果光斑中心位置横坐标为0，则说明图像中没有信标灯光斑出现
  {
    {count_beacon++;}  //每当一幅图像处理完毕，若该幅图像中存在信标灯光斑，则信标灯光斑出现次数计数标志位加一
    if(count_beacon>=5)//如果连续5副图像没有信标灯光斑，才进该if条件语句，以避免信标闪烁造成的影响——原因具体解释请见《算法说明：图像处理篇》
    { 
      flag_beacon=1;   //flag_beacon等于1标志没有发现信标灯，此时电机（及车辆运行方向）控制会进入原地顺时针右旋找灯模式，（下文有详述）
      Dir_Mid = 0;     //没有信标灯光斑存在，Dir_Mid=0，通过光斑中心横坐标MIDX的纵向直线当然不存在
      Distance = 0;    //没有信标灯光斑存在，Distance=0，信标车距离灯的纵向距离当然不存在
    }
  }
  else                //如果光斑中心位置横坐标不为0，则说明图像中有信标灯光斑出现
  {
    begin_beacon=1;   //发车（电机启动）标志置1，由此指令，四个电机方可转动
    count_beacon=0;   //检测到信标灯光斑标志清零
    flag_beacon=0;    //flag_beacon等于0标志发现信标灯
    Dir_Mid = MIDX;   //图像经过全部处理步骤后，将信标灯光斑中心横坐标MIDX赋给Dir_Mid 
    Distance = MIDY;  //图像经过全部处理步骤后，将信标灯光斑中心纵坐标MIDY赋给Distance
  }
  
}


int16 NewPWMValue = 0; 
float PWM_P ;          
int Steering_Engine=915; //上一版C车程序舵机中值设定，h信标车不需此句程序，无需修改。
uint16 Steer_Mid=60;     //定义一个方向计算基准常量，其值设定为60，不要改动。
uint16 SeekSpeedL=100;   //定义左前轮电机转速基准常量，其值设定为100，不要改动。
uint16 SeekSpeedR=100;   //定义右前轮电机转速基准常量，其值设定为100，不要改动。
uint16 SeekSpeedLL=100;  //定义左后轮电机转速基准常量，其值设定为100，不要改动。
uint16 SeekSpeedRR=100;  //定义右后轮电机转速基准常量，其值设定为100，不要改动。

void Steering_Engine_PWM()  //电机(及车辆运动方向)控制函数， ——请逐行阅读程序及注释，并结合《算法说明：图像处理篇》和《算法说明：速度控制篇》来逐步理解
{
  
 if(begin_beacon==1)        //发车（电机启动）标志等于1时，由此指令，四个电机方可转动
 {
    
  if(flag_beacon==1)        //flag_beacon等于1标志没有发现信标灯        
  {
    ftm_pwm_duty(ftm1,ftm_ch0,915); //上一版C车程序需要舵机打角以使得车子向右转圈寻找信标灯，h信标车不需此句程序，无需修改。
    MotorSpeed4(260,290,260,290);  //h信标车原地顺时针（右转）转圈，以寻找下一个信标灯 
  }
  else                     //else即flag_beacon等于0标志发现信标灯
  {    
    PWM_P =1.1;            //在选取曲线路径右侧绕灯时，修正比例PWM_P应为1 ,在选取直线路径直接撞灯时，PWM_P选1.1即可。
    NewPWMValue=(int16)((Steer_Mid-Dir_Mid)*PWM_P);  //注意：本句程序沿用上一版C车求舵机打角偏差值的语句，求偏差算法和语句均不变，但区别在于下文中对已求出的偏差NewPWMValue的处理有所不同。
                                                     //Steer_Mid=60为一个方向计算基准常量，与光斑纵向直线坐标Dir_Mid（光斑中心横坐标MIDX）相减，其结果再乘以修正比例PWM_P，进而将方向偏差转化为四个电机的PWM占空比偏差值（转速偏差值）NewPWMValue。在 MotorSpeed1电机(及车辆运动方向)控制函数中，例如前左电机：电机转速基准常量SeekSpeedL加上NewPWMValue，即为该电机进过偏差计算后实际输出的PWM占空比
   
    ftm_pwm_duty(ftm1,ftm_ch0,830+NewPWMValue);     //210 上一版C车程序需要舵机打角以使得车子向右转圈寻找信标灯，用舵机中值与NewPWMValue相加，h信标车不需此句程序，无需修改。
    MotorSpeed1(SeekSpeedL-NewPWMValue,SeekSpeedR+NewPWMValue,SeekSpeedLL-NewPWMValue,SeekSpeedRR+NewPWMValue);  //当光斑位于图像右半部分时（光斑中心横坐标存在右偏差），求得NewPWMValue小于0，左侧两轮加速正向运动，右侧两轮减速向前运动，车车一边朝向信标灯运动，一边逐步缩小右偏差，直至方向朝向正中（偏差缩小为零）。当光斑位于图像左半部分时，同理。

  }
}
 else             //else即发车（电机启动）标志等于0时，由此指令，四个电机方不允许转动
  {
  MotorSpeed1(0,0,0,0);
  }
 
}


void MotorSpeed1(int speed1,int speed3,int speed11,int speed33)   //定义四个同类型的pwm占空比电机调速控制函数，四个函数用于控制车车的不同运动状态。此类函数中包含h信标车四个电机的运转参数，四个局部变量依次是前左、前右、后左、后右四个电机的占空比值，占空比值不能超过1000。这里是速度控制函数的定义，其具体占空比值由SystemInit()中的7个速度档所设定，对应2号至7号拨码开关。
{                                                                   //请结合具体《算法说明：速度控制篇》来理解h信标车麦克纳姆轮底盘的基本控制方法，以及与之对应的电机控制程序
  ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);          //左前正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,speed1);
  ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,speed3);     //右前正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,0);
  ftm_pwm_duty(MOTOR_FTM,MOTOR5_PWM,0);          //左后正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR6_PWM,speed11);
  ftm_pwm_duty(MOTOR_FTM,MOTOR7_PWM,speed33);    //右后正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR8_PWM,0);
}

void MotorSpeed2(int speed101,int speed301,int speed1101,int speed3301)
{
  ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,speed101);   //左前反转
  ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,0);  
  ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,speed301);   //右前正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,0);
  ftm_pwm_duty(MOTOR_FTM,MOTOR5_PWM,0);          //左后正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR6_PWM,speed1101);
  ftm_pwm_duty(MOTOR_FTM,MOTOR7_PWM,0);          //右后反转
  ftm_pwm_duty(MOTOR_FTM,MOTOR8_PWM,speed3301);
}

void MotorSpeed3(int speed102,int speed302,int speed1102,int speed3302)
{
  ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);           //左前正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,speed102);  
  ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);           //右前反转
  ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,speed302);
  ftm_pwm_duty(MOTOR_FTM,MOTOR5_PWM,speed1102);   //左后反转
  ftm_pwm_duty(MOTOR_FTM,MOTOR6_PWM,0);
  ftm_pwm_duty(MOTOR_FTM,MOTOR7_PWM,speed3302);   //右后正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR8_PWM,0);
}

void MotorSpeed4(int speed103,int speed303,int speed1103,int speed3303)
{
  ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);          //左前正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,speed103);
  ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);          //右前反转
  ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,speed303);
  ftm_pwm_duty(MOTOR_FTM,MOTOR5_PWM,0);          //左后正转
  ftm_pwm_duty(MOTOR_FTM,MOTOR6_PWM,speed1103);
  ftm_pwm_duty(MOTOR_FTM,MOTOR7_PWM,0);          //右后反转
  ftm_pwm_duty(MOTOR_FTM,MOTOR8_PWM,speed3303);
}


float P_SpeedL=20;        //本h信标车最终决定不使用编码器进行闭环控速，具体缘由请见《算法说明：速度控制篇》。以下这些闭环pid电机速度控制变量,均未使用
float P_SpeedR=20;
float I_SpeedL=3;
float I_SpeedR=3;
float D_SpeedL=0;
float D_SpeedR=0;

int GoalSpeedL=100;
int GoalSpeedR=100;
static int OlderrorL=0;
static int OlderrorR=0;
int NewL=0;
int NewR=0;
static int errorL=0;
static int errorR=0;
int SpeedL;
int SpeedR;
int Speed;

void motor_L()
{
  errorL=GoalSpeedL-SpeedL;
  NewL=NewL+(P_SpeedL*(errorL-OlderrorL)+ I_SpeedL*(errorL));
  OlderrorL=errorL;
  if(NewL>800)   NewL=800;
  if(NewL<=0)    NewL=0;    
  ftm_pwm_duty(MOTOR_FTM,MOTOR1_PWM,0);
  ftm_pwm_duty(MOTOR_FTM,MOTOR2_PWM,NewL);		   
}
void motor_R()
{
  errorR=GoalSpeedR-SpeedR;
  NewR=NewR+(P_SpeedR*(errorR-OlderrorR)+ I_SpeedR*(errorR));
  OlderrorR=errorR;
  if(NewR>800)   NewR=800;
  if(NewR<=0)    NewR=0;    
  ftm_pwm_duty(MOTOR_FTM,MOTOR3_PWM,0);
  ftm_pwm_duty(MOTOR_FTM,MOTOR4_PWM,NewR);
}


void motor()
{
  motor_L();
  motor_R();
}


