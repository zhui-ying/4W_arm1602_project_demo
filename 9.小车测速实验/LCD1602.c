#include "LCD1602.h"
#include "interface.h"
#include "stm32f10x.h"
#include <stdio.h>

//全局变量定义
unsigned char const table1[]="FL:    cm/s    ";
unsigned char const table2[]="FR:    cm/s    ";

/*******************************************************************************
* 函 数 名 ：LcdBusy
* 函数功能 ：检查LCD忙状态,LcdBusy 为1时，忙，等待。lcd-busy为0时,闲，可写指令与数据
* 输    入 ：无
* 输    出 ：状态
*******************************************************************************/
//bit LcdBusy()
// {                          
//    bit result;
//    LCDRS_RESET;
//    LCDWR_SET;
//    LCDEN_SET;
//    Delay_us(1);
//    result = (bit)(P0&0x80);
//    LCDEN_RESET;
//    return(result); 
//}
 
/*******************************************************************************
* 函 数 名 ：write_com
* 函数功能 ：LCD1602 写指令
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void LcdWriteCom(unsigned char com)
{
	//while(LcdBusy());
	Delay_us(20);
	LCDWRITE_DATA(com);
	LCDRS_RESET;
	LCDWR_RESET;
	LCDEN_RESET;
	Delay_us(10);
	LCDEN_SET;
	Delay_us(10);
	LCDEN_RESET;
	Delay_us(10);
}

/*******************************************************************************
* 函 数 名 ：write_com
* 函数功能 ：LCD1602 写数据
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void LcdWriteDate(unsigned char date)
{
	//while(LcdBusy());
	Delay_us(20);
	LCDWRITE_DATA(date);
	LCDRS_SET;
	LCDWR_RESET;
	LCDEN_RESET;
	Delay_us(10);
	LCDEN_SET;
	Delay_us(10);
	LCDEN_RESET; 
	Delay_us(10);	
}

void LCD1602PortInit()
{
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = LCDRS_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(LCDRS_GPIO , &GPIO_InitStructure);	
	
	GPIO_InitStructure.GPIO_Pin = LCDWR_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(LCDWR_GPIO , &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = LCDEN_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(LCDEN_GPIO , &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Pin = LCD_PORT;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(LCD_GPIO , &GPIO_InitStructure);		
}

/*******************************************************************************
* 函 数 名 ：LCD1602Init
* 函数功能 ：LCD1602初始化
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void LCD1602Init()
{
	char index=0;
	LCD1602PortInit();
	Delayms(100);
	LcdWriteCom(0x38);  //设置16*2显示，8位数据接口
	LcdWriteCom(0x0c); //开显示，显示光标且闪烁
	LcdWriteCom(0x06);//写一个指针自动加一
	LcdWriteCom(0x01);//清屏  
	Delayms(100);//延时一段时间时间，等待LCD1602稳定	
	
	LcdWriteCom(0x80);//设置第一行 数据地址指针
	for(index=0;index<11;index++)
	{
		LcdWriteDate(table1[index]);  //写入数据             
	}
	
	LcdWriteCom(0xc0);//设置第二行 数据地址指针
	for(index=0;index<11;index++)
	{
		LcdWriteDate(table2[index]);  //写入数据             
	}
}

/*******************************************************************************
* 函 数 名 ：LCD1602WriteCommand
* 函数功能 ：显示指令到屏幕 U D L R S 
* 输    入 ：comm 字符格式
* 输    出 ：无
*******************************************************************************/
void LCD1602WriteCommand(char comm)
{
	LcdWriteCom(0xc0 + 14);
	LcdWriteDate(comm);  //写入数据   
}

void LCD1602WriteSpeed(unsigned char fl,unsigned char fr)
{
	char data_buf[4];
	int index=0;
	
	data_buf[3] = 0;
	sprintf(data_buf,"%03d",(int)fl);
	LcdWriteCom(0x80+3);//设置第一行 数据地址指针
	for(index=0;index<3;index++)
	{
		LcdWriteDate(data_buf[index]);  //写入数据             
	}
	
	sprintf(data_buf,"%03d",(int)fr);
	LcdWriteCom(0xc0+3);//设置第一行 数据地址指针
	for(index=0;index<3;index++)
	{
		LcdWriteDate(data_buf[index]);  //写入数据             
	}
}
