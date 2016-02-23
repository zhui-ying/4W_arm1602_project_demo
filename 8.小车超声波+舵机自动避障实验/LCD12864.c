
#include "LCD12864.h"
#include "charcode.h"
#include "interface.h"
#include <stdio.h>

unsigned char const table1[]="Hantech MCU";
unsigned char const table2[]="Command:";

//延时n us
void delay_nus(unsigned long n)
{
	unsigned long j;
	while(n--)
	{ j=8;
		while(j--);
	}
}


//GPIO配置函数
void LCDGPIO_Configuration(void)
{		
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LCDCS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDCS_GPIO, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = LCDRST_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDRST_GPIO, &GPIO_InitStructure);  
	
	GPIO_InitStructure.GPIO_Pin = LCDRS_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDRS_GPIO, &GPIO_InitStructure);   

	GPIO_InitStructure.GPIO_Pin = LCDSCL_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDSCL_GPIO, &GPIO_InitStructure);   
	
	GPIO_InitStructure.GPIO_Pin = LCDSID_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 	
	GPIO_Init(LCDSID_GPIO, &GPIO_InitStructure);  
}

/*******************************************************************************
* 函 数 名         : LCD12864_WriteCmd
* 函数功能		   : 写入一个命令到12864
* 输    入         : cmd
* 输    出         : 无
*******************************************************************************/

void LcdWriteCmd(u8 cmd)
{
	char i; 
	LCDCS_RESET;
	LCDRS_RESET;
	for(i=0;i<8;i++){ 
		LCDSCL_RESET; 
		if(cmd&0x80) LCDSID_SET; //置数据线
		else       LCDSID_RESET; 
		LCDSCL_SET; //产生时钟上升沿
		cmd <<= 1; 
	} 
	LCDCS_SET; 
}

/*******************************************************************************
* 函 数 名         : LcdWriteData
* 函数功能		   : 写入一个数据到12864
* 输    入         : dat
* 输    出         : 无
*******************************************************************************/

void LcdWriteData(u8 dat)
{	
	char i; 
	LCDCS_RESET;
	LCDRS_SET;
	for(i=0;i<8;i++){ 
		LCDSCL_RESET; 
		if(dat&0x80) LCDSID_SET; //置数据线
		else       LCDSID_RESET; 
		
		LCDSCL_SET; //产生时钟上升沿
		dat <<= 1; 
	} 
	LCDCS_SET; 
}

/*******************************************************************************
* 函 数 名 ：LCD12864Init
* 函数功能 ：LCD1602初始化
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void LCD12864Init(void)
{
	const unsigned char con_d2 = 0x16; //对比度细调默认值,可设置范围 0x00～0x3f
	
	LCDGPIO_Configuration();
	
	LCDRST_RESET;     Delay_us(3000);//低电平复位
	LCDRST_SET;     Delay_us(50);  //复位完成
	
	LcdWriteCmd(0xe2); Delay_us(15);   //软复位 

	LcdWriteCmd(0x2c); Delay_us(15);  //升压步聚 Boost ON
	LcdWriteCmd(0x2e); Delay_us(15);  //升压步聚 Voltage Regular ON
	LcdWriteCmd(0x2f); Delay_us(15);  //升压步聚 Voltage Follower ON

	LcdWriteCmd(0x24);  //粗调对比度，可设置范围 0x20～0x27
	LcdWriteCmd(0x81);  //微调对比度,此行及下行需紧跟
	LcdWriteCmd(con_d2);//0x1a,微调对比度的值，可设置范围 0x00～0x3f

	LcdWriteCmd(0xa2);  //1/9 偏压比(bais set)
	LcdWriteCmd(0xc8);  //行扫描顺序：0xc8从上到下,0xc0从下到上
	LcdWriteCmd(0xa0);  //列扫描顺序：0xa0从左到右,0xa1从右到左

	LcdWriteCmd(0x40);  //起始行：第一行开始
	LcdWriteCmd(0xaf);  //开显示	

	LcdClearScreen();
	LcdWriteString(0, 0, "航太电子有限公司");
	ShowEnString(0,3,"Command:");
	ShowEnString(0,5,"Distance:    cm");
	LCD12864WriteCommand(' ');
}

/*******************************************************************************
* 函 数 名         : LcdClearScreen
* 函数功能		   : 清屏12864
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/

void LcdClearScreen(void)
{
	unsigned char i, j;

	for(i=0; i<8; i++)
	{
		LcdWriteCmd(0xB0+i); //y轴地址

		//--在写入数据之后X坐标的坐标是会自动加1的，我们初始化使用0xA0
		//x轴坐标从0x10,0x00开始
		LcdWriteCmd(0x10); 
		LcdWriteCmd(0x00);							   
		
		//X轴有128位，就一共刷128次，X坐标会自动加1，所以我们不用再设置坐标
		for(j=0; j<128; j++)
		{
			LcdWriteData(0x00);  
		}
	}
}

/*******************************************************************************
* 函 数 名         : LcdWriteString
* 函数功能		   : 在12864上面书写16X16的汉字
* 输    入         : x, y, cn
* 输    出         : 无
* 说    明		   : 该函数可以直接书写字符串
*******************************************************************************/
unsigned char LcdWriteString(unsigned char x, unsigned char y, unsigned char *cn)
{
	unsigned char j, x1, x2, wordNum;
	y = y*2;
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y > 7)
	{
		return 0;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return 0;
	}
	y += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	LcdWriteCmd(y);
	while ( *cn != '\0')	 //在C语言中字符串结束以‘\0’结尾
	{	
		
		//--设置Y坐标--//
		LcdWriteCmd(y);

		x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
		x2 = x & 0x0F;          //去低四位
		//--设置X坐标--//
		LcdWriteCmd(0x10 + x1);   //高4位
		LcdWriteCmd(x2);	//低4位

		for (wordNum=0; wordNum<50; wordNum++)
		{
			//--查询要写的字在字库中的位置--//
			if ((CN16CHAR[wordNum].Index[0] == *cn)
					&&(CN16CHAR[wordNum].Index[1] == *(cn+1)))
			{
				for (j=0; j<32; j++) //写一个字
				{		
					if (j == 16)	 //由于16X16用到两个Y坐标，当大于等于16时，切换坐标
					{
						//--设置Y坐标--//
						LcdWriteCmd(y + 1);
						
						//--设置X坐标--//
						LcdWriteCmd(0x10 + x1);   //高4位
						LcdWriteCmd(x2);	//低4位
					}
					LcdWriteData(CN16CHAR[wordNum].Msk[j]);
				}
				x += 16;
			}//if查到字结束		
		} //for查字结束	
		cn += 2;
	}	//while结束
	return 1;
}

//查找字符所在数组位置,未找到就返回空格
int SearchEnPosition(unsigned char value)
{
	int i=0;
	while(EN16CHAR[i].Index != ' ')
	{
		if(EN16CHAR[i].Index == value) return i;
		i++;
	}
	return i;
}

//显示一个英文字符
void ShowEnChar(unsigned char x, unsigned char y, unsigned char en)	
{  
	unsigned char j, x1, x2, wordNum,y1;
	
	y1 = y;
	//--Y的坐标只能从0到7，大于则直接返回--//
	if(y1 > 7)
	{
		return;
	}

	//--X的坐标只能从0到128，大于则直接返回--//
	if(x > 128)
	{
		return;
	}
	y1 += 0xB0;	   //求取Y坐标的值
	//--设置Y坐标--//
	LcdWriteCmd(y1);

	x1 = (x >> 4) & 0x0F;   //由于X坐标要两句命令，分高低4位，所以这里先取出高4位
	x2 = x & 0x0F;          //去低四位
	//--设置X坐标--//
	LcdWriteCmd(0x10 + x1);   //高4位
	LcdWriteCmd(x2);	//低4位
	//--查询要写的字在字库中的位置--//
	wordNum = SearchEnPosition(en);		    
	for (j=0; j<16; j++) //写一个字
	{		
		if (j == 8)	 //由于16X16用到两个Y坐标，当大于等于16时，切换坐标
		{
			//--设置Y坐标--//
			LcdWriteCmd(y1 + 1);
			
			//--设置X坐标--//
			LcdWriteCmd(0x10 + x1);   //高4位
			LcdWriteCmd(x2);	//低4位
		}
		LcdWriteData(EN16CHAR[wordNum].Msk[j]);
	}
}

//128*4
void ShowEnString(unsigned int x,unsigned int y,unsigned char *str)
{  
	unsigned int x1,y1;
	x1=x;
	y1=y;
	while(*str!='\0')
	{	
		ShowEnChar(x1,y1,*str);
		x1+=8;
		if(x1+8 > 128)//换行
		{
			x1 = x;
			y1 = y1+2;
		}
		str++;
	}	
}

/*******************************************************************************
* 函 数 名 ：LCD12864WriteCommand
* 函数功能 ：显示指令到屏幕 U D L R S 
* 输    入 ：comm 字符格式
* 输    出 ：无
*******************************************************************************/
void LCD12864WriteCommand(char comm)
{
	ShowEnString(9*8,3,(unsigned char *)&comm);
}

void LCD12864WriteDistance(unsigned int distance)
{
	char data_buf[4];
	
	sprintf(data_buf,"%03d",(int)distance);
	data_buf[3] = 0;
	ShowEnString(9*8,5,(unsigned char *)data_buf);
}
