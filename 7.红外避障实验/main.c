/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：红外避障实验
* 实验说明 ：使用红外避障来控制小车移动
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：请参考interface.h文件
* 注    意 ：1.因地面和轮子的差异，左右转向时需要根据实际需要调节差速的差值
*            2.建议使用不反光的书本作为引导物
* 作    者 ：航太电子产品研发部    QQ ：1909197536
* 店    铺 ：http://shop120013844.taobao.com/
****************************************************************************************/

#include "stm32f10x.h"
#include "interface.h"
#include "LCD1602.h"
#include "IRCtrol.h"
#include "motor.h"
#include "UltrasonicCtrol.h"

//全局变量定义
unsigned int speed_count=0;//占空比计数器 50次一周期
char front_left_speed_duty=SPEED_DUTY;
char front_right_speed_duty=SPEED_DUTY;
char behind_left_speed_duty=SPEED_DUTY;
char behind_right_speed_duty=SPEED_DUTY;

unsigned char tick_5ms = 0;//5ms计数器，作为主函数的基本周期
unsigned char tick_1ms = 0;//1ms计数器，作为电机的基本计数器
unsigned char tick_200ms = 0;//刷新显示

char ctrl_comm = COMM_STOP;//控制指令
char ctrl_comm_last = COMM_STOP;//上一次的指令
unsigned char continue_time=0;

void BarrierProc(void)
{
	char void_l,void_r;
	void_l = VOID_L_IO;
	void_r = VOID_R_IO;
	
	if(void_r == BARRIER_Y || void_l == BARRIER_Y)
	{
		CarBack();
		Delayms(500);
		if(void_r == BARRIER_Y && void_l == BARRIER_Y)
		{
			CarBack();
			Delayms(300);			
			CarRight();
			Delayms(400);
			CarGo();
			Delayms(100);
			return;
		}
		
			CarRight();
			Delayms(600);		
			CarGo();
			Delayms(100);
			return;		
		
//		if(void_r == BARRIER_Y)
//		{
//			CarLeft();
//			Delayms(600);		
//			CarGo();
//			Delayms(100);
//			return;			
//		}
//		
//		if(void_l == BARRIER_Y)
//		{
//			CarRight();
//			Delayms(600);		
//			CarGo();
//			Delayms(100);
//			return;			
//		}
	}else
	{
		CarGo();
	}
}

int main(void)
{
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
//	LCD1602Init();
//	IRCtrolInit();
	TIM2_Init();
	MotorInit();
	//UltraSoundInit();
	RedRayInit();
	ServoInit();

 while(1)
 {	 
	 		if(tick_5ms >= 5)
		{
			tick_5ms = 0;
			tick_200ms++;
			if(tick_200ms >= 40)
			{
				tick_200ms = 0;
				LEDToggle(LED_PIN);
			}
//			continue_time--;//200ms 无接收指令就停车
//			if(continue_time == 0)
//			{
//				continue_time = 1;
//				CarStop();
//			}
			//do something
			BarrierProc();
//			if(ctrl_comm_last != ctrl_comm)//接收到红外信号
//			{
//				ctrl_comm_last = ctrl_comm;
//				switch(ctrl_comm)
//				{
//					case COMM_UP:    CarGo();break;
//					case COMM_DOWN:  CarBack();break;
//					case COMM_LEFT:  CarLeft();break;
//					case COMM_RIGHT: CarRight();break;
//					case COMM_STOP:  CarStop();break;
//					default : break;
//				}
//				LCD1602WriteCommand(ctrl_comm);
//			}
		}
		
 }
}

