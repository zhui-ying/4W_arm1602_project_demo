#ifndef __LCD1602_H_
#define __LCD1602_H_


void LCD1602Init(void);
void LCD1602WriteCommand(char comm);
void LCD1602WriteDistance(unsigned int distance);
#endif
