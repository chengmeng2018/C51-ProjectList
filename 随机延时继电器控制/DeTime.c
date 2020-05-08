
#include "DeTime.h"
#include <stdlib.h>
DeTimeStruct m_DeTimeData;
void SetFlag(char enble)//是否为随机
{
    m_DeTimeData.RngFlag =enble>0;
}
void OffSetstVal(char offVal)//固定数值
{
    m_DeTimeData.StaticVal+=offVal;
    if(m_DeTimeData.StaticVal>99)
    {
        m_DeTimeData.StaticVal=99;
    }
    if(m_DeTimeData.StaticVal<0)
    {
        m_DeTimeData.StaticVal=0;
    }
}
void OffSetMXVal(char offVal)//最大随机值
{
    m_DeTimeData.RngMax+=offVal;
    if(m_DeTimeData.RngMax>99)
    {
        m_DeTimeData.RngMax=99;
    }
    if(m_DeTimeData.RngMax<m_DeTimeData.RngMin)
    {
        m_DeTimeData.RngMax=m_DeTimeData.RngMin;
    }
}
void OffSetMNVal(char offVal)//最小随机值
{
    m_DeTimeData.RngMin+=offVal;
    if(m_DeTimeData.RngMin>m_DeTimeData.RngMax)
    {
        m_DeTimeData.RngMin=m_DeTimeData.RngMax;
    }
    if(m_DeTimeData.RngMin<0)
    {
        m_DeTimeData.RngMin=0;
    }
}
char getRng()
{
    char CurVal;
    float fVal;
    int t;
    t=rand();
    m_DeTimeData.ss.seed=t;
    CurVal=	t%m_DeTimeData.RngMax;
    fVal=	CurVal/1.0/m_DeTimeData.RngMax;
    CurVal=	m_DeTimeData.RngMin+fVal*(m_DeTimeData.RngMax-m_DeTimeData.RngMin);

    return CurVal;
}