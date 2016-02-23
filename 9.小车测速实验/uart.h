#ifndef __UART_H_
#define __UART_H_

#include "stm32f10x.h"

void USART1Conf(u32 baudRate);
void PutChar(u8 Data);
void PutNChar(u8 *buf , u16 size);
void PutStr(char *str);
#endif
