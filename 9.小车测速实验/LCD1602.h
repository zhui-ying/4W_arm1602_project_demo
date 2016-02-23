#ifndef __LCD1602_H_
#define __LCD1602_H_


void LCD1602Init(void);
void LCD1602WriteCommand(char comm);
void LCD1602WriteSpeed(unsigned char fl,unsigned char fr);
#endif
