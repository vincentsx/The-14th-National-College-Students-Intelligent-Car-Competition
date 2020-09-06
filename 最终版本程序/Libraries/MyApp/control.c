#include "headfile.h"

uint16 a[8]; //超声波数据缓冲区,不使用超声波

void UltrasonicRead()   //超声波adc采集函数，本车不使用
{
  a[0] =  adc_ave(adc0,ADC0_SE8, ADC_12bit,100);
  a[1] =  adc_ave(adc0,ADC0_SE9, ADC_12bit,100);
  a[2] =  adc_ave(adc0,ADC0_SE12,ADC_12bit,100);
  a[3] =  adc_ave(adc0,ADC0_SE13,ADC_12bit,100);
  a[4] =  adc_ave(adc1,ADC1_SE10,ADC_12bit,100);
  a[5] =  adc_ave(adc1,ADC1_SE11,ADC_12bit,100);
  a[6] =  adc_ave(adc1,ADC1_SE12,ADC_12bit,100);
  a[7] =  adc_ave(adc1,ADC1_SE13,ADC_12bit,100);
}

/***********上位机显示波形**********************/
float OutData[4];


//**************************************************************************
/*
*  功能说明：SCI示波器CRC校验
内部调用函数
*  参数说明： 无
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//**************************************************************************
static unsigned short CRC_CHECK(unsigned char *Buf, unsigned char CRC_CNT)
{
    unsigned short CRC_Temp;
    unsigned char i,j;
    CRC_Temp = 0xffff;

    for (i=0;i<CRC_CNT; i++){
        CRC_Temp ^= Buf[i];
        for (j=0;j<8;j++) {
            if (CRC_Temp & 0x01)
                CRC_Temp = (CRC_Temp >>1 ) ^ 0xa001;
            else
                CRC_Temp = CRC_Temp >> 1;
        }
    }
    return(CRC_Temp);
}

//************************************************
//
/*
*  功能说明：SCI示波器发送函数

*  参数说明：
OutData[]  需要发送的数值赋予该数组
*  函数返回：无符号结果值
*  修改时间：2013-2-10
*/
//****************************************************
void OutPut_Data(void)
{
    int temp[4] = {0};
    unsigned int temp1[4] = {0};
    unsigned char databuf[10] = {0};
    unsigned char i;
    unsigned short CRC16 = 0;
    for(i=0;i<4;i++)
    {

        temp[i]  = (int)OutData[i];
        temp1[i] = (unsigned int)temp[i];

    }

    for(i=0;i<4;i++)
    {
        databuf[i*2]   = (unsigned char)(temp1[i]%256);
        databuf[i*2+1] = (unsigned char)(temp1[i]/256);
    }

    CRC16 = CRC_CHECK(databuf,8);
    databuf[8] = CRC16%256;
    databuf[9] = CRC16/256;

    for(i=0;i<10;i++)
    {
        uart_putchar (uart2,(char)databuf[i]);
    }
}

