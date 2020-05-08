
#include <STC8G.H>
#include "uartT1.h"

#define FOSC            12000000UL
#define BRT             (65536 - FOSC / 4800 / 4)

bit busy;
char wptr;
char rptr;
char buffer[16];

void UartIsr() interrupt 4
{
    if (TI)
    {
        TI = 0;
        busy = 0;
    }
    if (RI)
    {
        RI = 0;
        buffer[wptr++] = SBUF;
        wptr &= 0x0f;
    }
}

void UartInit()
{
    SCON = 0x50;
    TMOD =(TMOD&0x0F)| 0x00;
    TL1 = BRT;
    TH1 = BRT >> 8;
    TR1 = 1;
    AUXR = 0x40;
    wptr = 0x00;
    rptr = 0x00;
    busy = 0;
    ES = 1;
    EA = 1;
}

void UartSend(char dat)
{
    while (busy);
    busy = 1;
    SBUF = dat;
}

void UartSendStr(char *p)
{
#warning DEbug
    return ;
    while (*p)
    {
        UartSend(*p++);
    }
}


