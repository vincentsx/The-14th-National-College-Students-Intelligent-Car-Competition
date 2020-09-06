#ifndef _CONTROL_H_
#define _CONTROL_H_


/************SCI上位机显示波形*****************/
extern void OutPut_Data(void);
extern float OutData[4];

/************超声波控制************************/
extern void UltrasonicRead();
extern uint16 a[8];

#endif