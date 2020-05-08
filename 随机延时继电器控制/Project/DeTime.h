#ifndef	__DETIME__H_
#define	__DETIME__H_

typedef struct
{
    char RngFlag:1;//�Ƿ������ȡ����
    char RngMin;//���ֵ��Сֵ
    char RngMax;//���ֵ���ֵ
    char StaticVal;//��ǰ�̶���ֵ
    char CurVal;//��ǰ����ʱ
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
void SetFlag(char enble);//�Ƿ�Ϊ���
void OffSetstVal(char offVal);//�̶���ֵ
void OffSetMXVal(char offVal);//������ֵ
void OffSetMNVal(char offVal);//��С���ֵ
#endif