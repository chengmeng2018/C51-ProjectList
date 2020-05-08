//使用单片机内部的Flash，模拟EEPROM使用
//IAP15W4K58S4 这款单片机可以直接擦除所有的Flash ，所以使用时候要小心不能擦除自己的程序区域
//每512个字节算一个扇区，擦除某地址时会直接擦除其所在的整个扇区数据
//推荐使用单片机的最后一段Flash ，地址是 D3B7-D8C9 D8C9-DDDB DDDB-E2ED E2ED - E7FF
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

//读写范围 0000H~0FFFH
char IapRead(int addr);//读取一个字节
void IapProgram(int addr, char dat);//写入一个字节
void IapErase(int addr);//擦除一个512字节 低九位无效

#endif