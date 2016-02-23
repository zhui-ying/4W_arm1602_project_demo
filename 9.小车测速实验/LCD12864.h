#ifndef  __LCD1602_H_
#define  __LCD1602_H_
#include "stm32f10x.h"

void LCD12864Init(void);
unsigned char LcdWriteString(unsigned char x, unsigned char y, unsigned char *cn);
void ShowEnString(unsigned int x,unsigned int y,unsigned char *str);
void LcdClearScreen(void);
void LCD12864WriteCommand(char comm);
void LCD12864WriteSpeed(unsigned char fl,unsigned char fr);

#endif

