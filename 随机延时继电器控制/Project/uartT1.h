
#ifndef __UARTT1_H__
#define __UARTT1_H__

#include <stdio.h>
#include "intrins.h"
void UartInit();
void UartSend(char dat);
void UartSendStr(char *p);
#endif