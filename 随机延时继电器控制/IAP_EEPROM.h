//ʹ�õ�Ƭ���ڲ���Flash��ģ��EEPROMʹ��
//IAP15W4K58S4 ��Ƭ������ֱ�Ӳ������е�Flash ������ʹ��ʱ��ҪС�Ĳ��ܲ����Լ��ĳ�������
//ÿ512���ֽ���һ������������ĳ��ַʱ��ֱ�Ӳ��������ڵ�������������
//�Ƽ�ʹ�õ�Ƭ�������һ��Flash ����ַ�� D3B7-D8C9 D8C9-DDDB DDDB-E2ED E2ED - E7FF
#ifndef	__IAP_EEPROM__H_
#define	__IAP_EEPROM__H_

#include "intrins.h"

enum DataAddr
{
    Addr_Init_Flag=0x01,
    Addr_MotorDC=0x010,
    Addr_DelayTime=0x020,
    Addr_RngFlag,
    Addr_SubTime=0x030,
    Addr_Workmode=0x040,
    Addr_RunFlag,
		Addr_SleepHour,
};

//��д��Χ 0000H~0FFFH
char IapRead(int addr);//��ȡһ���ֽ�
void IapProgram(int addr, char dat);//д��һ���ֽ�
void IapErase(int addr);//����һ��512�ֽ� �;�λ��Ч

#endif