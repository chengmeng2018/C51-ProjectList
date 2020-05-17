//����Ƶ����ͳһ���� 12Mhz

#include <STC8G.H>

#include "motor.h"//�������
#include "DIG4201.h"//˫λ���������
#include "keyBoard.h"//��������
#include "IAP_EEPROM.h"//
#include "Clock_T2.h"//ʱ������
#include "DeTime.h"//��ʱ��������
#include "uartT1.h"
sbit GPIO_Ir=P5^4;

char Ir_Flag;//�����Ƿ����źű�־λ
char RunFlag;//�Ƿ����б�־λ
char workMode=0;
tmTypedef subtm;
tmTypedef SleepTm,SleepLastTm;
//�������ģ��
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
    ReadAllData();//��ȡ����ǰ������
    Lasttm=GetClock();
    Setdata(m_DeTimeData.CurVal);
    while(1)
    {
        DigDisplay();
        {
            //��������
            static KeyStruct keynum;
            static tmTypedef	KeyDownLasttm;
            static tmTypedef	Keysubtm= {3}; //��������µ��źŻ�ȡƵ��
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
            //��ʱ�ػ����
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
            //�������ģ��
            if(RunFlag==1)
            {
                switch (Ir_Flag)
                {
                case 0x00://��ȡ�����ź�
                    if(GPIO_Ir)
                    {//Ѱ�ҵ͵�ƽ�ź�
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
                case 0x01://�ȴ��ȹ���ǰ��ʱ
                    if(subTime(&Lasttm,&subtm))
                    {
                        if(m_DeTimeData.RngFlag)
                        {
                            //tmVal�ĵ�λ��1s ���Ա����Ŵ�
                            subtm.tmVal=	getRng();//���µ����Ӵ���CurVal
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
                        //��õ���ʱʱ��
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
            //tmVal�ĵ�λ��1s ���Ա����Ŵ�
            subtm.tmVal=	getRng();//���µ����Ӵ���CurVal
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
    //P5.4 ׼˫���
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
    case 0x00://�������޲���
        switch(val)
        {
        case 0x01://Up
            //����
            SetRunFlag(1);
            Setdata(workMode*10+1);
            break;
        case 0x02://Down
            //ֹͣ
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
    case 0x01://���õ��ѡ��
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
    case 0x02://���ù���ģʽ ����¼���̶�ʱ��
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
    case 0x03://�޸��趨ʱ��
        if( m_DeTimeData.RngFlag ==1)
        {
            //������ʱ��
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
    case 0x04://�޸�RngMinʱ��
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
    case 0x05://�趨�Զ�ͣ���Ĳ���
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