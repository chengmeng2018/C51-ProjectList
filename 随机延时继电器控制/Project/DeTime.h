#ifndef	__DETIME__H_
#define	__DETIME__H_

typedef struct
{
    char RngFlag:1;//是否随机获取数字
    char RngMin;//随机值最小值
    char RngMax;//随机值最大值
    char StaticVal;//当前固定数值
    char CurVal;//当前倒计时
    union
    {
        int seed;
        char seedData[2];
    } ss;

} DeTimeStruct;
extern DeTimeStruct m_DeTimeData;
char getRng();
void SaveDeTime();
void LoadDeTime();
void SetFlag(char enble);//是否为随机
void OffSetstVal(char offVal);//固定数值
void OffSetMXVal(char offVal);//最大随机值
void OffSetMNVal(char offVal);//最小随机值
#endif