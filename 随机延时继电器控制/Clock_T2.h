#ifndef __CLOCK_T2_H_
#define __CLOCK_T2_H_


#ifndef OSC
#define OSC 12000000
#endif

#ifndef freqT2
#define freqT2 1000    //进入中断的频率
#endif

#ifndef T2Count
#define T2Count 65536-OSC/12/freqT2
#endif

//typedef struct
//{
//    char day;
//    char hour;
//    char min;
//    char sec;
//    char ms;//单位10ms
//} ;
typedef union
{
    char time[4];
    long tmVal;
} tmTypedef;

void ClockT2_Init();
tmTypedef GetClock();
//分析延时时间是否到达
char subTime(tmTypedef* oldtm,tmTypedef* subVal);
#endif