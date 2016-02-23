/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：超声波舵机自动避障实验
* 实验说明 ：将超声波模块放在舵机旋转轴上，通过转动舵机，来获取前方，左边以及右边的障碍物距离
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：请参考interface.h文件
* 注    意 ：舵机旋转角度建议不要使用0°和180°，可以稍微往中间一点，比如选取20°和160 °，这样舵机旋转更精确
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

unsigned char duoji_count=0;
unsigned char zhuanjiao = 11;

void DuojiMid()
{
	zhuanjiao = 11;
	Delayms(150);//延时1s
}

void DuojiRight()
{
	zhuanjiao = 5;
	Delayms(150);//延时1s
}

void DuojiLeft()
{
	zhuanjiao = 18;
	Delayms(150);//延时1s
}

///获取三个方向的距离,进来前舵机方向为向前
void GetAllDistance(unsigned int *dis_left,unsigned int *dis_right,unsigned int *dis_direct)
{
	CarStop();
	GetDistanceDelay();
	*dis_direct = distance_cm;
	
	DuojiRight();
	Delayms(100);
	GetDistanceDelay();//获取右边距离
	*dis_right = distance_cm;
	
	DuojiMid();
	DuojiLeft();
	Delayms(100);
	GetDistanceDelay();//获取左边距离
	*dis_left = distance_cm;
	
	DuojiMid();//归位
}

void BarrierProc()
{
		if(distance_cm < 10)//前方有障碍物
	{
		unsigned int dis_left;//左边距离
		unsigned int dis_right;//右边距离
		unsigned int dis_direct;//右边距离
		if(distance_cm < 8)
		{
			CarBack();
			Delayms(400);
		}
		
		while(1)
		{
			GetAllDistance(&dis_left,&dis_right,&dis_direct);
			if(dis_direct < 5)
			{
				CarBack();
				Delayms(300);
				continue;
			}
			else if((dis_left < 5) || (dis_right < 5))
			{
				CarBack();
				Delayms(300);
				continue;
			}
			else if(dis_direct >= dis_left && dis_direct >= dis_right)//前方距离最远
			{
				CarGo();
				Delayms(600);
				return;
			}
			else if(dis_left <= dis_right)//右转
			{
				CarRight();
				Delayms(500);
			}
			else if(dis_right < dis_left)
			{
				CarLeft();
				Delayms(500);
			}
		}
	}
	else
	{
		CarGo();
	}
}

int main(void)
{
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	//LCD1602Init();
	//IRCtrolInit();
	TIM2_Init();
	MotorInit();
	UltraSoundInit();
	//RedRayInit();
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
				//LCD1602WriteDistance(distance_cm);//更新距离
			}
//			continue_time--;//200ms 无接收指令就停车
//			if(continue_time == 0)
//			{
//				continue_time = 1;
//				CarStop();
//			}
			//do something
			Distance();//计算距离
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

