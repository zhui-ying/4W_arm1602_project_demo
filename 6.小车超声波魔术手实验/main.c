/********************************* 深圳市航太电子有限公司 *******************************
* 实 验 名 ：小车超声波魔术手实验
* 实验说明 ：小车通过超声波判断与前方障碍物的距离，并保持一定的距离
* 实验平台 ：航太ARM单片机开发板
* 连接方式 ：请参考interface.h文件
* 注    意 ：超声波读取偶尔会存在读到值为0或者很大的情况，要注意一下
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
	if((distance_cm < 15 && distance_cm > 10) || distance_cm > 25) 
	{
		ctrl_comm = COMM_STOP;//防震荡
		return;
	}
	
	if(distance_cm <= 10) ctrl_comm = COMM_DOWN;
	
	if(distance_cm >= 15) ctrl_comm = COMM_UP;
}

int main(void)
{
	delay_init();
	GPIOCLKInit();
	UserLEDInit();
	LCD1602Init();
	IRCtrolInit();
	TIM2_Init();
	MotorInit();
	UltraSoundInit();
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
				LCD1602WriteDistance(distance_cm);//更新距离
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
			if(ctrl_comm_last != ctrl_comm)//接收到红外信号
			{
				ctrl_comm_last = ctrl_comm;
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
			}
		}
		
 }
}

