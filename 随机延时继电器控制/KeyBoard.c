

#include <STC8.H>

#include "keyBoard.h"

//#define Matrix

void delay2ms(void)   //延时函数
{
    unsigned char i,j;
    for(i=133; i>0; i--)
        for(j=6; j>0; j--);
}
//#define MatrixKeyboard P2
#ifdef Matrix
//sbit matkey1=P1^0;
//sbit matkey2=P1^1;
//sbit matkey3=P5^2;
//sbit matkey4=P0^4;
//sbit matkey5=P0^3;
//sbit matkey6=P0^2;
//sbit matkey7=P0^1;
//sbit matkey8=P1^7;
sbit matkey1=P1^0;
sbit matkey2=P1^1;
sbit matkey3=P1^2;
sbit matkey4=P1^3;
sbit matkey5=P1^4;
sbit matkey6=P1^5;
sbit matkey7=P1^6;
sbit matkey8=P1^7;

void SetMatKeyData(char ndata)
{
    matkey1=(ndata&0x01)!=0;
    matkey2=(ndata&0x02)!=0;
    matkey3=(ndata&0x04)!=0;
    matkey4=(ndata&0x08)!=0;
    matkey5=(ndata&0x10)!=0;
    matkey6=(ndata&0x20)!=0;
    matkey7=(ndata&0x40)!=0;
    matkey8=(ndata&0x80)!=0;
}
char getMatKeyData()
{
    char ndata=0;
    ndata|=matkey1!=0?0x01:0;
    ndata|=matkey2!=0?0x02:0;
    ndata|=matkey3!=0?0x04:0;
    ndata|=matkey4!=0?0x08:0;
    ndata|=matkey5!=0?0x10:0;
    ndata|=matkey6!=0?0x20:0;
    ndata|=matkey7!=0?0x40:0;
    ndata|=matkey8!=0?0x80:0;
    return ndata;
}
unsigned char 	GetMatrixKeyboard()
{
    unsigned char KeyValue;
//用来存放读取到的键值
    //MatrixKeyboard=0X0F;
    SetMatKeyData(0X0F);
    delay2ms();
    switch(getMatKeyData())
    {
    case(0X07):
        KeyValue=0;
        break;
    case(0X0b):
        KeyValue=4;
        break;
    case(0X0d):
        KeyValue=8;
        break;
    case(0X0e):
        KeyValue=12;
        break;
    default:
        KeyValue=17;	//检测出错回复17意思是把数码管全灭掉。
        return  KeyValue;
        break;
    }
    //测试行
    //MatrixKeyboard=0XF0;
    SetMatKeyData(0xF0);
    delay2ms();
    switch(getMatKeyData())
    {
    case(0X70):
        KeyValue=0+KeyValue;
        break;
    case(0Xb0):
        KeyValue=1+KeyValue;
        break;
    case(0Xd0):
        KeyValue=2+KeyValue;
        break;
    case(0Xe0):
        KeyValue=3+KeyValue;
        break;
        break;
    default:
        KeyValue=17;
        return  KeyValue;
        break;
    }
    return KeyValue;
}
#endif

sbit Key1=P5^5;
sbit Key2=P3^0;
sbit Key3=P3^1;
unsigned char IndependentKeyboard()
{
    unsigned char keyValue=0;
    keyValue=Key3==0;
    keyValue<<=1;
    keyValue|=Key2==0;
    keyValue<<=1;
    keyValue|=Key1==0;
    return keyValue;
}
void InitKeyboard()
{
    P5M1 &=~(0x20);
    P5M0 &=~(0x20);
    P3M1&=~(0x03);
    P3M0&=~(0x03);
    Key1=1;
    Key2=1;
    Key3=1;
}
//按键检测
unsigned char key_scan(KeyStruct* KeyNum)
{
    unsigned char keyValue=0;
    unsigned char State=KeyNum->State;
    unsigned char KeyTemp=KeyNum->Value;
    /*******矩阵键盘读取操作******/
    keyValue =	IndependentKeyboard();//独立按键检测
    //keyValue==0表示没有按键按下
    /*******矩阵键盘读取操作***/
    //keyValue=GetMatrixKeyboard();
    /*  keyValue==17表示没有按键按下
    如果变更键盘注意修改 CheckKey分支判断条件
    *************************/
    switch(State)
    {
    case CheckKey:
        if (keyValue == 0)
        {
            break;
        }
        KeyTemp = keyValue;
        State = DelayKey;
    case DelayKey:
        delay2ms();
        State= KeyTemp==keyValue ? KeyFallEdge:CheckKey;
        break;
    case KeyFallEdge:
        State=KeyDown;
        break;
    case KeyDown:
        if(KeyTemp != keyValue)
        {
            State=KeyRisEdge;
            KeyNum->lastValue= KeyTemp;
        }
        break;
    case KeyRisEdge:
        State=CheckKey;
        break;
    default:
        break;
    }
    KeyNum->State=State;
    KeyNum->Value=keyValue;
    return State;
}
//char keynumtoASCII(char keynum)
//{
//    char keyval=0;
//    char code keyASCII[17]="123E456O789L0.SQ!";
//    keyval=keyASCII[keynum];
//    /*switch(keynum)
//    {
//    case 00:keyval='1':break;
//    case 01:keyval='2':break;
//    case 02:keyval='3':break;
//    case 03:keyval='E':break;
//    case 04:keyval='4':break;
//    case 05:keyval='5':break;
//    case 06:keyval='6':break;
//    case 07:keyval='O':break;
//    case 08:keyval='7':break;
//    case 09:keyval='8':break;
//    case 10:keyval='9':break;
//    case 11:keyval='L':break;
//    case 12:keyval='0':break;
//    case 13:keyval='.':break;
//    case 14:keyval='S':break;
//    	case 15:keyval='Q':break;
//    	default:break;
//    }*/

//    return keyval;
//}
