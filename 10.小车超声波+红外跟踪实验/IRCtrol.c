#include "IRCtrol.h"
#include "interface.h"

unsigned char ir_rec_flag=0;//接收数据标志位 1 有新数据 0 没有
unsigned char IRCOM[4];

//use time3 realize delay systick已经在main函数中使用了，在中断中不能重复使用
void Time3Init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    TIM_TimeBaseStructure.TIM_Period = 1;
    TIM_TimeBaseStructure.TIM_Prescaler = (72 - 1);//72M / 72 = 1us
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Down;
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
}

//1us 延时
void DelayUs(vu32 nCount)
{
  u16 TIMCounter = nCount;
  TIM_Cmd(TIM3, ENABLE);
  TIM_SetCounter(TIM3, TIMCounter);
  while (TIMCounter>1)
  {
    TIMCounter = TIM_GetCounter(TIM3);
  }
  TIM_Cmd(TIM3, DISABLE);
}

//外部中断配置 红外遥控配置
void IRCtrolInit(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	EXTI_InitTypeDef 	EXTI_InitStructure;//定义一个外部中断相关的结构体
	NVIC_InitTypeDef NVIC_InitStructure; //定义一个中断的结构体
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO , ENABLE);
	GPIO_InitStructure.GPIO_Pin = IRIN_PIN;//配置使能GPIO管脚
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//配置GPIO模式,输入上拉
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//配置GPIO端口速度
	GPIO_Init(IRIN_GPIO , &GPIO_InitStructure);
		
	GPIO_EXTILineConfig(IRIN_PORTSOURCE , IRIN_PINSOURCE);
	EXTI_InitStructure.EXTI_Line = IRIN_EXITLINE;//将对应的GPIO口连接到中断线上
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//中断事件类型，下降沿
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//选择模式，中断型
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能该中断
	EXTI_Init(&EXTI_InitStructure);//将配置好的参数写入寄存器
			
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);	 //阶级为0，不可嵌套
	NVIC_InitStructure.NVIC_IRQChannel = 	IRIN_IRQCH;//打开PINA_8的外部中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//主优先级0，最高
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//子优先级，最低
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;	//使能该模块中断
	NVIC_Init(&NVIC_InitStructure);	//中断初始化，将结构体定义的数据执行
	
	Time3Init();
}

/*******************************************************************************
* 函 数 名 ：DelayIr
* 函数功能 ：0.14MS 延时
* 输    入 ：无
* 输    出 ：无
*******************************************************************************/
void DelayIr(unsigned char x)  
{
  while(x--)
 {
  DelayUs(140);
 }
}

void IRIntIsr(void)
{
  unsigned char j,k,N=0;
	 DelayIr(15);
	 if (IRIN==1) 
     { 
	   return;
	  } 
		 continue_time = 40;//连发信号，表示指令持续 40*5 = 200ms 无指令停车
                           //确认IR信号出现
  while (!IRIN)            //等IR变为高电平，跳过9ms的前导低电平信号。
    {DelayIr(1);}

 for (j=0;j<4;j++)         //收集四组数据
 { 
  for (k=0;k<8;k++)        //每组数据有8位
  {
   while (IRIN)            //等 IR 变为低电平，跳过4.5ms的前导高电平信号。
     {DelayIr(1);}
    while (!IRIN)          //等 IR 变为高电平
     {DelayIr(1);}
     while (IRIN)           //计算IR高电平时长
      {
    DelayIr(1);
    N++;           
    if (N>=30)
	 { 
	 return;}                  //0.14ms计数过长自动离开。
      }                        //高电平计数完毕                
     IRCOM[j]=IRCOM[j] >> 1;                  //数据最高位补“0”
     if (N>=8) {IRCOM[j] = IRCOM[j] | 0x80;}  //数据最高位补“1”
     N=0;
  }//end for k
 }//end for j
   
	k = ~IRCOM[3];
   if (IRCOM[2] != k)
   { 
     return; }
	 
		 //指令转换
		 
		 switch(IRCOM[2])
		 {
			 case 0x46: ctrl_comm = COMM_UP;break;
			 case 0x15: ctrl_comm = COMM_DOWN;break;
			 case 0x44: ctrl_comm = COMM_LEFT;break;
			 case 0x43: ctrl_comm = COMM_RIGHT;break;
			 case 0x40: ctrl_comm = COMM_STOP;break;
			 default :  return;
		 }
		 ir_rec_flag = 1;
	
}
