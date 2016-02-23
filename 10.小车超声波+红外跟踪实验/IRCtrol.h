#ifndef __IRCTROL_H_
#define __IRCTROL_H_

#include "stm32f10x.h"

extern unsigned char ir_rec_flag;//接收数据标志位 1 有新数据 0 没有
extern char ctrl_comm;//控制指令
extern unsigned char continue_time;
void IRCtrolInit(void);
void IRIntIsr(void);

#endif
