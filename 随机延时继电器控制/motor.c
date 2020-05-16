
#include <STC8.H>
#include "motor.h"

sbit IOA = P3^2;
sbit IOB = P3^3;
sbit IOC = P3^4;
sbit IOD = P3^5;

//unsigned char TableA[] = { 0XF7,0XFB,0XF3};	 //A��Ȧϸ�ֱ�
//unsigned char TableB[] = { 0XeF,0XdF,0XcF};	 //B��Ȧϸ�ֱ�

static unsigned int m_StepTimeMs=10;//������� ÿ����ʱ
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
        static char step;
        char IOData;
        step=(step+1)%4;
        IOData=(~(0x01<<step))&0x0f;
        IOA=(IOData&0x01)!=0;
        IOB=(IOData&0x02)!=0;
        IOC=(IOData&0x04)!=0;
        IOD=(IOData&0x08)!=0;
    }
    else
    {
        IOA=1;
        IOB=1;
        IOC=1;
        IOD=1;
    }

}
void MotorDCStep()
{
    if(startFlag)
    {
        IOA=1;
        IOB=0;
    }
    else
    {
        IOA=0;
        IOB=0;
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