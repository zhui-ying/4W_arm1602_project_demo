/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：小车蓝牙遥控实验
* 实验说明 ：使用手机连接蓝牙模块后，通过手机发送指令来控制小车
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：请参考interface.h文件
* 注    意 ：指令必须连续发送才会使小车动作，若停止发送指令，则小车会停止
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "LCD1602.h"
#include "IRCtrol.h"
#include "motor.h"
#include "uart.h"
#include "ESP8266.h"
#include "serialportAPI.h"
#include <stdlib.h>
#include <stdio.h>

//全局变量定义
unsigned int speed_count=0;//占空比计数器 50次一周期
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms计数器，作为主函数的基本周期
unsigned char tick_1ms = 0;//1ms计数器，作为电机的基本计数器
unsigned char tick_200ms = 0;//刷新显示
unsigned int tick_3s = 0;//隔3s断线检测

char ctrl_comm = COMM_STOP;//控制指令
unsigned char continue_time=0;
unsigned char wifi_rec_flag=0;//蓝牙控制标志位

#define HOST_NAME   "192.168.31.195" //主机IP
#define HOST_PORT   8080  //主机端口

#define SSID "Hantech"  //路由器名
#define PSD  "hantechinternet" //密码

#define RECVBUF_SIZE 16
unsigned char buffer[RECVBUF_SIZE] = {0};

int main(void)
{
	USART1Conf(9600);
	DBG("system begin");
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	LCD1602Init();
	TIM2_Init();
	MotorInit();
	ServoInit();
//	USART3Conf(9600);

		while(1)
	{
		DBG("chang baud");
		
		UartBegin(115200,&USART3Conf,&PutChar);//每次设置完波特率后需要做一点延时
		delay(500);
		SetBaud(19200);
		UartBegin(19200,&USART3Conf,&PutChar);//更改波特率到19200
		delay(500);
		if(0 != SetBaud(19200))//在新波特率下检查是否成功
		{
			break;//成功就退出
		}	
	}

	while(0 == WifiInit(SSID,PSD,HOST_NAME,HOST_PORT));
	
 while(1)
 {	
		if(SerialAvailable() > 5)//接收到至少5个字符
		{
			int len;
			len = recv(buffer, RECVBUF_SIZE, 100);
			if (len > 0)
			{
					char inChar;
					inChar = buffer[0];
	//			  send(buffer, len);
					if (ctrl_comm != inChar || continue_time == 1)
					{
						wifi_rec_flag = 1;
						ctrl_comm = inChar;
					}
					continue_time = 60;//更新持续时间
				  tick_3s = 0;//心跳包周期
			} 
		}
		
	 		if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_200ms++;
			if(tick_200ms >= 40)
			{
				tick_200ms = 0;
				tick_3s++;
				LEDToggle(LED_PIN);
				if(tick_3s >= 15)//15*200=3000
				{
					tick_3s = 0;
					//定时检查连接是否正常，如果不正常,wifi复位重新连接
					if (getSystemStatus() != STATUS_GETLINK)
					{
						DBG("TCP unlink");
						while(!WifiInit(SSID,PSD,HOST_NAME,HOST_PORT));
					}else
					{
						DBG("TCP link tick");
					}					
				}
			}
			//continue_time--;//200ms 无接收指令就停车
			if(continue_time == 0)
			{
				continue_time = 1;
				CarStop();
			}
			//do something
			if(wifi_rec_flag == 1)//接收到红外信号
			{
				wifi_rec_flag = 0;
				switch(ctrl_comm)
				{
					case COMM_UP:    CarGo();break;
					case COMM_DOWN:  CarBack();break;
					case COMM_LEFT:  CarLeft();break;
					case COMM_RIGHT: CarRight();break;
					case COMM_STOP:  CarStop();break;
					default : break;
				}
				LCD1602WriteCommand(ctrl_comm);
				DBG("recv COMM");
			}
		}
		
 }
}

