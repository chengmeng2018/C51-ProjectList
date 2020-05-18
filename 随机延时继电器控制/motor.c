
#include <STC8.H>
#include "motor.h"

#if 0
sbit IOA = P3^2;
sbit IOB = P3^3;
sbit IOC = P3^4;
sbit IOD = P3^5;
#else
sbit IO4 = P3^2;
sbit IO5 = P3^3;
sbit IO3 = P3^4;
sbit IO2 = P3^5;
#endif

//unsigned char TableA[] = { 0XF7,0XFB,0XF3};	 //A��Ȧϸ�ֱ�
//unsigned char TableB[] = { 0XeF,0XdF,0XcF};	 //B��Ȧϸ�ֱ�

static unsigned int m_StepTimeMs=20;//������� ÿ����ʱ
char  SW_MOTO;//ֱ������Ͳ��������ѡ�񿪹�
static char startFlag=0;
void InitIO();
void motorInit()
{
    TMOD = (TMOD&0xF0)|0x00;
    TL0 = CountT0;                                 //65536-11.0592M/12/1000
    TH0 = CountT0>>8;
    TR0 = 1;                                    //������ʱ��
    ET0 = 1;                                    //ʹ�ܶ�ʱ���ж�
    EA = 1;
    InitIO();
}
void InitIO()
{
    P3M1 &=~(0x3C);
    P3M0 |=(0x3C);
}
void run(char enble)
{
    startFlag=enble>0;

}
void SelectMotorDC(char enble)
{
    SW_MOTO=enble>0;
}
void motorStep()
{
    if(startFlag)
    {
			char code stepData[8]={0x01,0x03,0x02,0x06,0x04,0x0C,0x08,0x09};
        static char step;
        char IOData;
        step=(step+1)%8;
				IOData=~stepData[step];
        IOData=(IOData)&0x0f;
        IO5=(IOData&0x01)!=0;
        IO3=(IOData&0x02)!=0;
        IO4=(IOData&0x04)!=0;
        IO2=(IOData&0x08)!=0;
    }
    else
    {
			IO5=1;
			IO4=1;
			IO3=1;
			IO2=1;
//        IOA=1;
//        IOB=1;
//        IOC=1;
//        IOD=1;
    }

}
void MotorDCStep()
{
    if(startFlag)
    {
        IO4=1;
        IO5=0;
    }
    else
    {
        IO4=0;
        IO5=0;
    }
}
void TM0_Isr() interrupt 1
{
    static int count=0;
    count++;
    if(m_StepTimeMs==count)
    {
        count=0;
        if(SW_MOTO)
        {
            //ֱ�����
            MotorDCStep();
        }
        else
        {
            //�������
            motorStep();
        }
    }


}