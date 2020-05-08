#include <STC8.H>
#include "DIG4201.h"

#if 0
sbit DIG_CS1 =P1^7;//5
sbit DIG_CS2 =P1^1;//10
sbit DIG_A =P1^4;//7
sbit DIG_B =P1^3;//6
sbit DIG_C =P1^0;//4
sbit DIG_D =P1^2;//1
sbit DIG_E =P3^7;//3
sbit DIG_F =P1^5;//8
sbit DIG_G =P1^6;//9
sbit DIG_dp =P3^6;//2
#else //由于电路设计问题 所以对调IO 
sbit DIG_CS1 =P1^1;//5
sbit DIG_CS2 =P1^7;//10
sbit DIG_A =P3^6;//7
sbit DIG_B =P1^2;//6
sbit DIG_C =P1^6;//4
sbit DIG_D =P1^3;//1
sbit DIG_E =P1^5;//3
sbit DIG_F =P3^7;//8
sbit DIG_G =P1^0;//9
sbit DIG_dp =P1^4;//2
#endif

//0、1、2、3、4、5、6、7、8、9、A、b、C、d、E、F的显示码
unsigned char code DIG_CODE[17] =
{
    0x3f,0x06,0x5b,0x4f,0x66,0x6d,0x7d,0x07,
    0x7f,0x6f,0x77,0x7c,0x39,0x5e,0x79,0x71,0x00
};

static  unsigned char m_val[2];

void SendIO(unsigned char m_val);
void delay();

void DigInit()
{
    //推挽输出 Dig的片选端
//    P1M1=(P1M1&(~0x82));
//    P1M0=P1M0|0x82;
    P1M1=0xff;
    P1M0=0xFF;
    P1M1=(P1M1&(~0x82));
    P1M0=P1M0|0x82;
    P3M1 |=0xC0;
    P3M0 |=0xC0;
}
void DigDisplay()
{
    DIG_CS1=0;
    DIG_CS2=0;

    //SendIO(17);//不显示任何
    /*第一位*/
    SendIO(m_val[0]);
    DIG_CS1=1;
    delay();
    DIG_CS1=0;

    /*显示第二位*/
    SendIO(m_val[1]);
    DIG_CS2=1;
    delay();
    DIG_CS2=0;

}
void SetDataRaw(char raw,char idx)
{
    idx=idx%sizeof(m_val);
    m_val[idx]=raw;
}
void Setdata( unsigned char val)
{
    m_val[0]=~DIG_CODE[val%100/10];
    m_val[1]=~DIG_CODE[val%10];
}

void TestABCD(char val)
{
    DIG_CS1=1;
    DIG_CS2=1;
    DIG_A =(val&0x01)!=0;
    DIG_B =(val&0x02)!=0;
    DIG_C =(val&0x04)!=0;
    DIG_D =(val&0x08)!=0;
    DIG_E =(val&0x10)!=0;
    DIG_F =(val&0x20)!=0;
    DIG_G =(val&0x40)!=0;
    DIG_dp=(val&0x80)!=0;

}
void SendIO(unsigned char val)
{
    DIG_A =(val&0x01)!=0;
    DIG_B =(val&0x02)!=0;
    DIG_C =(val&0x04)!=0;
    DIG_D =(val&0x08)!=0;
    DIG_E =(val&0x10)!=0;
    DIG_F =(val&0x20)!=0;
    DIG_G =(val&0x40)!=0;
    DIG_dp=(val&0x80)!=0;
}
void delay()
{
    int j = 100;						 //扫描间隔时间设定
    while(j--);
}