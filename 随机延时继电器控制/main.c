//晶振频率请统一保持 12Mhz

#include <STC8G.H>

#include "motor.h"//电机驱动
#include "DIG4201.h"//双位数码管驱动
#include "keyBoard.h"//键盘驱动
#include "IAP_EEPROM.h"//
#include "Clock_T2.h"//时钟驱动
#include "DeTime.h"//延时控制驱动
#include "uartT1.h"
sbit GPIO_Ir=P5^4;

char Ir_Flag;//红外是否有信号标志位
char RunFlag;//是否运行标志位
char workMode=0;
tmTypedef subtm;
tmTypedef SleepTm,SleepLastTm;
//红外组件模块
static tmTypedef	Lasttm;
char SleepHour=0,sleepOk=0;
void ReadAllData();
void WriteAllData();
void WorkSet(char val,KeyState keymode);
void InitIr();

void SetRunFlag(char enb);
void main()
{
    static tmTypedef	Lasttm;
    DigInit();//P1 P3.6 P3.7
    InitKeyboard();
    ClockT2_Init();//Use interrupt 12
		motorInit();
    InitIr();
    ReadAllData();//读取掉电前的数据
    Lasttm=GetClock();
    Setdata(m_DeTimeData.CurVal);
    while(1)
    {
        DigDisplay();
        {
            //按键操作
            static KeyStruct keynum;
            static tmTypedef	KeyDownLasttm;
            static tmTypedef	Keysubtm= {3}; //解决长按下的信号获取频率
            KeyState keyrec;
            keyrec=key_scan(&keynum);
            if(keyrec==KeyFallEdge )
            {
                WorkSet(keynum.Value,KeyFallEdge);
                KeyDownLasttm=	GetClock();
            }
            else if(keyrec ==KeyDown &&subTime(&KeyDownLasttm,&Keysubtm))
            {
                WorkSet(keynum.Value,keyrec);
            }
        }
        {
            //定时关机组件
            if(SleepHour!=0 &&RunFlag==1)
            {
                if(subTime(&SleepLastTm,&SleepTm))
                {
										SleepLastTm=GetClock();
                    sleepOk=1;
                    RunFlag=0;
                    run(0);
                    SetDataRaw(0xFF,0);
                    SetDataRaw(0xFF,1);
                }
            }
        }
        {
            //红外组件模块
            if(RunFlag==1)
            {
                switch (Ir_Flag)
                {
                case 0x00://获取红外信号
                    if(GPIO_Ir)
                    {//寻找低电平信号
                        Lasttm=	GetClock();
                        run(0);
                        Ir_Flag=0x01;
                    }
										else
										{
										SetDataRaw(0xFF&~(0x40),0);
                    SetDataRaw(0xFF&~(0x40),1);
										}
                    break;
                case 0x01://等待度过当前延时
                    if(subTime(&Lasttm,&subtm))
                    {
                        if(m_DeTimeData.RngFlag)
                        {
                            //tmVal的单位是1s 所以倍数放大
                            subtm.tmVal=	getRng();//将新的种子存入CurVal
                        }
                        else
                        {
                            subtm.tmVal=m_DeTimeData.StaticVal;
                        }
												Ir_Flag=0x02;
                        run(1);
                    }
                    else
                    {
                        //获得倒计时时间
                        long timetmp=GetClock().tmVal-Lasttm.tmVal;
                        m_DeTimeData.CurVal=(subtm.tmVal-timetmp);
                        Setdata(m_DeTimeData.CurVal);
                    }
                    break;
								case 0x02:
									if(!GPIO_Ir)
									{
										  Ir_Flag=0x00;
									}
									else
									{
										SetDataRaw(0xFF&~(0x01),0);
                    SetDataRaw(0xFF&~(0x01),1);								
									}
								break;
                }
            }
        }
    }

}
void ReadAllData()
{
    char InitFlag=IapRead(Addr_Init_Flag);
    if(InitFlag==0x01)
    {
        // LoadDeTime();
        char i;
        m_DeTimeData.StaticVal=IapRead(Addr_DelayTime);
        m_DeTimeData.RngFlag=IapRead(Addr_RngFlag+0);
        m_DeTimeData.RngMax=IapRead(Addr_RngFlag+1);
        m_DeTimeData.RngMin=IapRead(Addr_RngFlag+2);
        if(m_DeTimeData.RngMax>99 ||m_DeTimeData.RngMax<0)
        {
            m_DeTimeData.RngMax=99;
        }
        for(i=0; i<sizeof(m_DeTimeData.ss.seedData); i++)
        {
            m_DeTimeData.ss.seedData[i]=IapRead(Addr_RngFlag+3+i);
        }
				SW_MOTO=IapRead(Addr_MotorDC);
        RunFlag=IapRead(Addr_RunFlag);
        workMode=IapRead(	Addr_Workmode);
				SleepHour=IapRead(Addr_SleepHour);
				if(SleepHour!=0)
				{
					SleepTm.tmVal=SleepHour*60*60;
					SleepLastTm=GetClock();
				}
    }
    else
    {
        m_DeTimeData.StaticVal=10;
        m_DeTimeData.RngFlag=1;
        m_DeTimeData.RngMax=30;
        m_DeTimeData.RngMin=5;
        //m_DeTimeData.ss.seed
				SW_MOTO=1;
        Ir_Flag=0x00;
        RunFlag=0;
        workMode=0;
				SleepHour=0;	
    }
		  if(m_DeTimeData.RngFlag)
        {
            //tmVal的单位是1s 所以倍数放大
            subtm.tmVal=	getRng();//将新的种子存入CurVal
        }
        else
        {
            subtm.tmVal=m_DeTimeData.StaticVal;
        }

}
void WriteAllData()
{
    char i;
    IapErase(Addr_Init_Flag);
    IapProgram(Addr_Init_Flag,0x01);
    IapProgram(Addr_DelayTime,m_DeTimeData.StaticVal);
    IapProgram(Addr_RngFlag+0,m_DeTimeData.RngFlag);
    IapProgram(Addr_RngFlag+1,m_DeTimeData.RngMax);
    IapProgram(Addr_RngFlag+2,m_DeTimeData.RngMin);
    for(i=0; i<sizeof(m_DeTimeData.ss.seedData); i++)
    {
        IapProgram(Addr_RngFlag+3+i,m_DeTimeData.ss.seedData[i]);
    }
		IapProgram(Addr_MotorDC,SW_MOTO);
    IapProgram(Addr_RunFlag,RunFlag);
    IapProgram(Addr_Workmode,workMode);
		IapProgram(Addr_SleepHour,SleepHour);
}

void InitIr()
{
    //P5.4 准双向口
    P5M1 &=~(0x10);
    P5M0 &=~(0x10);
    GPIO_Ir=1;
}
void SetRunFlag(char enb)
{
    RunFlag=enb>0;
   //WriteAllData();
}
void WorkSet(char val,KeyState keymode)
{
#define workModeNum 6
    if(sleepOk!=0 && workMode==0x04 )
    {
        SetRunFlag(1);
        sleepOk=0;
        run(1);
        Setdata(m_DeTimeData.CurVal);
        Lasttm=	GetClock();
    }
    switch(workMode)
    {
    case 0x00://启动后无操作
        switch(val)
        {
        case 0x01://Up
            //启动
            SetRunFlag(1);
            Setdata(workMode*10+1);
            break;
        case 0x02://Down
            //停止
            SetRunFlag(0);
            run(0);
            Setdata(workMode*10);
            break;
        case 0x04://mode
            workMode=(workMode+1)%workModeNum;
						SetRunFlag(0);
            Setdata(workMode*10);
            SetDataRaw(0xFF,1);
            break;
        }
        break;
    case 0x01://设置电机选项
        switch(val)
        {
        case 0x01://Up
            SelectMotorDC(1);
            Setdata(workMode*10+1);
            break;
        case 0x02://Down
            SelectMotorDC(0);
            Setdata(workMode*10);
            break;
        case 0x04://mode
            workMode=(workMode+1)%workModeNum;
            Setdata(workMode*10);
            SetDataRaw(0xFF,1);
            break;
        }
        break;
    case 0x02://设置工作模式 随机事件或固定时间
        switch(val)
        {
        case 0x01://Up
            SetFlag(1);
            Setdata(workMode*10+1);
            break;
        case 0x02://Down
            SetFlag(0);
            Setdata(workMode*10);
            break;
        case 0x04://mode
            workMode=(workMode+1)%workModeNum;
            Setdata(workMode*10);
            SetDataRaw(0xFF,1);
            break;
        }
				break;
    case 0x03://修改设定时间
        if( m_DeTimeData.RngFlag ==1)
        {
            //如果随机时间
            switch(val)
            {
            case 0x01://Up
                OffSetMXVal(1);
                Setdata(m_DeTimeData.RngMax);
                break;
            case 0x02://Down
                OffSetMXVal(-1);
                Setdata(m_DeTimeData.RngMax);
                break;
            case 0x04://mode
                workMode=(workMode+1)%workModeNum;
                Setdata(workMode*10);
                SetDataRaw(0xFF,1);
                break;
            }

        }
        else
        {
            switch(val)
            {
            case 0x01://Up
                OffSetstVal(1);
                Setdata(m_DeTimeData.StaticVal);
                break;
            case 0x02://Down
                OffSetstVal(-1);
                Setdata(m_DeTimeData.StaticVal);
                break;
            case 0x04://mode
                workMode=0x05;
                Setdata(workMode*10);
                SetDataRaw(0xFF,1);
                break;
            }
        }
        break;
    case 0x04://修改RngMin时间
        switch(val)
        {
        case 0x01://Up
            OffSetMNVal(1);
            Setdata(m_DeTimeData.RngMin);
            break;
        case 0x02://Down
            OffSetMNVal(-1);
            Setdata(m_DeTimeData.RngMin);
            break;
        case 0x04://mode
           workMode=(workMode+1)%workModeNum;
            Setdata(workMode*10);
            SetDataRaw(0xFF,1);
            break;
        }
        break;
    case 0x05://设定自动停机的参数
        switch(val)
        {
        case 0x01://Up
            SleepHour=(SleepHour+1)>99?99:(SleepHour+1);
            Setdata(SleepHour);
            break;
        case 0x02://Down
            SleepHour=(SleepHour-1)<0?0:(SleepHour-1);
            Setdata(SleepHour);
            break;
        case 0x04://mode
            workMode=0;
            Setdata(workMode*10);
            SetDataRaw(0xFF,1);
            break;
        }
        SleepTm.tmVal=SleepHour*60*60;
        SleepLastTm=GetClock();
        break;
    }
    WriteAllData();
}