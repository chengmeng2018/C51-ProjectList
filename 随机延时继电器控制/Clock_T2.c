
#include <STC8.H>

#include "Clock_T2.h"

static tmTypedef tm= {1,10,15,0};
void ClockT2_Init()
{
    AUXR &= ~T2x12;		//��ʱ��ʱ��12Tģʽ
    T2L = T2Count;                                 //65536-11.0592M/12/1000
    T2H = T2Count>>8;
    AUXR |= T2R;                                //������ʱ��
    IE2 = ET2;                                  //ʹ�ܶ�ʱ���ж�
    EA = 1;
}
tmTypedef GetClock()
{
    return tm;
}
char subTime(tmTypedef* oldtm,tmTypedef* subVal)
{

    if(tm.tmVal-oldtm->tmVal>subVal->tmVal)
    {
        return 1;
    }
    else
    {
        return 0;
    }

}
//1000Hz��Ƶ��
void TM2_Isr() interrupt 12
{
    static int val=0;
    AUXINTIF &= ~T2IF;                          //���жϱ�־

    val++;
    if(val>999)
    {
        //100Hz�Ľ���Ƶ��
        tm.tmVal++;
        val=0;
    }
}