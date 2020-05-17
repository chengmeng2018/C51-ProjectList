#ifndef __MOTOR_H_
#define __MOTOR_H_


#ifndef OSC
#define OSC 12000000
#endif
#ifndef freqT0
#define freqT0 1000    //进入中断的频率
#endif
#ifndef CountT0
#define CountT0 65536-OSC/12/freqT0
#endif

extern char  SW_MOTO;//直流电机和步进电机的选择开关

//初始化电机 使用定时器0
void motorInit();
void run(char enble);
void SelectMotorDC(char enble);
#endif