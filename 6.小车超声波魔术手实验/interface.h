#ifndef __INTERFACE_H_
#define __INTERFACE_H_

#include "stm32f10x.h"

//user LED PG15
#define LED_PIN           GPIO_Pin_15
#define LED_GPIO        GPIOG
#define LED_SET         GPIO_SetBits(LED_GPIO , LED_PIN)
#define LED_RESET       GPIO_ResetBits(LED_GPIO , LED_PIN)

//红外遥控 红外接收器数据线,外部中断 PD10
#define IRIN_PIN         GPIO_Pin_10
#define IRIN_GPIO        GPIOD
#define IRIN_PORTSOURCE  GPIO_PortSourceGPIOD
#define IRIN_PINSOURCE   GPIO_PinSource10
#define IRIN_EXITLINE    EXTI_Line10
#define IRIN_IRQCH       EXTI15_10_IRQn
#define IRIN             GPIO_ReadInputDataBit(IRIN_GPIO, IRIN_PIN)


//超声波控制 外部中断返回 PB14
#define Echo_PIN         GPIO_Pin_14
#define Echo_GPIO        GPIOB
#define Echo_PORTSOURCE  GPIO_PortSourceGPIOB
#define Echo_PINSOURCE   GPIO_PinSource14
#define Echo_EXITLINE    EXTI_Line14
#define Echo_IRQCH       EXTI15_10_IRQn
#define Echo             GPIO_ReadInputDataBit(Echo_GPIO, Echo_PIN)
//触发IO PD8
#define Trig_PIN         GPIO_Pin_8
#define Trig_GPIO        GPIOD
#define Trig_RESET       GPIO_SetBits(Trig_GPIO , Trig_PIN)
#define Trig_SET         GPIO_ResetBits(Trig_GPIO , Trig_PIN)//5转3.3做了一次反相 modfied by LC 2015.09.20 10:18


//舵机控制IO PD12
#define Servo_PIN         GPIO_Pin_12
#define Servo_GPIO        GPIOD
#define Servo_SET         GPIO_SetBits(Servo_GPIO , Servo_PIN)
#define Servo_RESET       GPIO_ResetBits(Servo_GPIO , Servo_PIN)

//LCD1602 IO
//LCDRS PC13   
#define LCDRS_PIN         GPIO_Pin_13
#define LCDRS_GPIO        GPIOC
#define LCDRS_SET         GPIO_SetBits(LCDRS_GPIO , LCDRS_PIN)
#define LCDRS_RESET       GPIO_ResetBits(LCDRS_GPIO , LCDRS_PIN)

//LCDWR PC14   
#define LCDWR_PIN         GPIO_Pin_14
#define LCDWR_GPIO        GPIOC
#define LCDWR_SET         GPIO_SetBits(LCDWR_GPIO , LCDWR_PIN)
#define LCDWR_RESET       GPIO_ResetBits(LCDWR_GPIO , LCDWR_PIN)

//LCDEN PC15  
#define LCDEN_PIN         GPIO_Pin_15
#define LCDEN_GPIO        GPIOC
#define LCDEN_SET         GPIO_SetBits(LCDEN_GPIO , LCDEN_PIN)
#define LCDEN_RESET       GPIO_ResetBits(LCDEN_GPIO , LCDEN_PIN)

//LCDPORT PF0-PF7
#define LCD_PORT        0x00ff
#define LCD_GPIO        GPIOF
#define LCDWRITE_DATA(data)   LCD_GPIO->BSRR = 0x00ff0000;LCD_GPIO->BSRR = (data)     

//电机驱动IO定义 
/* 
FRONT_LEFT_F_PIN	PG13	左前前进IO
FRONT_LEFT_B_PIN	PG11	左前后退IO

FRONT_RIGHT_F_PIN	PC11	右前前进IO
FRONT_RIGHT_B_PIN	PD0	    右前后退IO

BEHIND_LEFT_F_PIN	PD6	    左后前进IO
BEHIND_LEFT_B_PIN	PG9	    左后后退IO

BEHIND_RIGHT_F_PIN	PD4	    右后前进IO
BEHIND_RIGHT_B_PIN	PD2	    右后后退IO
 */
#define FRONT_LEFT_F_PIN         GPIO_Pin_13
#define FRONT_LEFT_F_GPIO        GPIOG
#define FRONT_LEFT_F_SET         GPIO_SetBits(FRONT_LEFT_F_GPIO , FRONT_LEFT_F_PIN)
#define FRONT_LEFT_F_RESET       GPIO_ResetBits(FRONT_LEFT_F_GPIO , FRONT_LEFT_F_PIN)

#define FRONT_LEFT_B_PIN         GPIO_Pin_11
#define FRONT_LEFT_B_GPIO        GPIOG
#define FRONT_LEFT_B_SET         GPIO_SetBits(FRONT_LEFT_B_GPIO , FRONT_LEFT_B_PIN)
#define FRONT_LEFT_B_RESET       GPIO_ResetBits(FRONT_LEFT_B_GPIO , FRONT_LEFT_B_PIN)

#define FRONT_RIGHT_F_PIN         GPIO_Pin_11
#define FRONT_RIGHT_F_GPIO        GPIOC
#define FRONT_RIGHT_F_SET         GPIO_SetBits(FRONT_RIGHT_F_GPIO , FRONT_RIGHT_F_PIN)
#define FRONT_RIGHT_F_RESET       GPIO_ResetBits(FRONT_RIGHT_F_GPIO , FRONT_RIGHT_F_PIN)

#define FRONT_RIGHT_B_PIN         GPIO_Pin_0
#define FRONT_RIGHT_B_GPIO        GPIOD
#define FRONT_RIGHT_B_SET         GPIO_SetBits(FRONT_RIGHT_B_GPIO , FRONT_RIGHT_B_PIN)
#define FRONT_RIGHT_B_RESET       GPIO_ResetBits(FRONT_RIGHT_B_GPIO , FRONT_RIGHT_B_PIN)

#define BEHIND_LEFT_F_PIN         GPIO_Pin_6
#define BEHIND_LEFT_F_GPIO        GPIOD
#define BEHIND_LEFT_F_SET         GPIO_SetBits(BEHIND_LEFT_F_GPIO , BEHIND_LEFT_F_PIN)
#define BEHIND_LEFT_F_RESET       GPIO_ResetBits(BEHIND_LEFT_F_GPIO , BEHIND_LEFT_F_PIN)

#define BEHIND_LEFT_B_PIN         GPIO_Pin_9
#define BEHIND_LEFT_B_GPIO        GPIOG
#define BEHIND_LEFT_B_SET         GPIO_SetBits(BEHIND_LEFT_B_GPIO , BEHIND_LEFT_B_PIN)
#define BEHIND_LEFT_B_RESET       GPIO_ResetBits(BEHIND_LEFT_B_GPIO , BEHIND_LEFT_B_PIN)

#define BEHIND_RIGHT_F_PIN         GPIO_Pin_4
#define BEHIND_RIGHT_F_GPIO        GPIOD
#define BEHIND_RIGHT_F_SET         GPIO_SetBits(BEHIND_RIGHT_F_GPIO , BEHIND_RIGHT_F_PIN)
#define BEHIND_RIGHT_F_RESET       GPIO_ResetBits(BEHIND_RIGHT_F_GPIO , BEHIND_RIGHT_F_PIN)

#define BEHIND_RIGHT_B_PIN         GPIO_Pin_2
#define BEHIND_RIGHT_B_GPIO        GPIOD
#define BEHIND_RIGHT_B_SET         GPIO_SetBits(BEHIND_RIGHT_B_GPIO , BEHIND_RIGHT_B_PIN)
#define BEHIND_RIGHT_B_RESET       GPIO_ResetBits(BEHIND_RIGHT_B_GPIO , BEHIND_RIGHT_B_PIN)

//循迹光电对管
/* 
中循迹	SEARCH_M_PIN	PG8
右循迹	SEARCH_R_PIN	PG6
左循迹	SEARCH_L_PIN	PG4
 */
#define SEARCH_M_PIN         GPIO_Pin_8
#define SEARCH_M_GPIO        GPIOG
#define SEARCH_M_IO          GPIO_ReadInputDataBit(SEARCH_M_GPIO, SEARCH_M_PIN)

#define SEARCH_R_PIN         GPIO_Pin_6
#define SEARCH_R_GPIO        GPIOG
#define SEARCH_R_IO          GPIO_ReadInputDataBit(SEARCH_R_GPIO, SEARCH_R_PIN)

#define SEARCH_L_PIN         GPIO_Pin_4
#define SEARCH_L_GPIO        GPIOG
#define SEARCH_L_IO          GPIO_ReadInputDataBit(SEARCH_L_GPIO, SEARCH_L_PIN)
#define BLACK_AREA 1
#define WHITE_AREA 0

//红外避障
/* 
右避障	VOID_R_PIN	PC7
左避障	VOID_L_PIN	PG2
 */
#define VOID_R_PIN         GPIO_Pin_7
#define VOID_R_GPIO        GPIOC
#define VOID_R_IO          GPIO_ReadInputDataBit(VOID_R_GPIO, VOID_R_PIN)

#define VOID_L_PIN         GPIO_Pin_2
#define VOID_L_GPIO        GPIOG
#define VOID_L_IO          GPIO_ReadInputDataBit(VOID_L_GPIO, VOID_L_PIN)
#define BARRIER_Y 0 //有障碍物
#define BARRIER_N 1  //无障碍物

//测速
/* 
速度码盘右	FRONT_RIGHT_S_PIN	PA11
速度码盘左	FRONT_LEFT_S_PIN	PA12
 */
#define FRONT_RIGHT_S_PIN         GPIO_Pin_11
#define FRONT_RIGHT_S_GPIO        GPIOA
#define FRONT_RIGHT_S_IO          GPIO_ReadInputDataBit(FRONT_RIGHT_S_GPIO, FRONT_RIGHT_S_PIN)

#define FRONT_LEFT_S_PIN         GPIO_Pin_12
#define FRONT_LEFT_S_GPIO        GPIOA
#define FRONT_LEFT_S_IO          GPIO_ReadInputDataBit(FRONT_LEFT_S_GPIO, FRONT_LEFT_S_PIN)

//左前
#define FRONT_LEFT_GO    FRONT_LEFT_F_SET; FRONT_LEFT_B_RESET//前进
#define FRONT_LEFT_BACK  FRONT_LEFT_F_RESET; FRONT_LEFT_B_SET//后退
#define FRONT_LEFT_STOP  FRONT_LEFT_F_RESET; FRONT_LEFT_B_RESET//停止

//右前
#define FRONT_RIGHT_GO     FRONT_RIGHT_F_SET;  FRONT_RIGHT_B_RESET
#define FRONT_RIGHT_BACK   FRONT_RIGHT_F_RESET;FRONT_RIGHT_B_SET
#define FRONT_RIGHT_STOP   FRONT_RIGHT_F_RESET;FRONT_RIGHT_B_RESET

//左后
#define BEHIND_LEFT_GO     BEHIND_LEFT_F_SET;BEHIND_LEFT_B_RESET
#define BEHIND_LEFT_BACK   BEHIND_LEFT_F_RESET;BEHIND_LEFT_B_SET
#define BEHIND_LEFT_STOP   BEHIND_LEFT_F_RESET;BEHIND_LEFT_B_RESET

//右后
#define BEHIND_RIGHT_GO    BEHIND_RIGHT_F_SET;BEHIND_RIGHT_B_RESET
#define BEHIND_RIGHT_BACK  BEHIND_RIGHT_F_RESET;BEHIND_RIGHT_B_SET
#define BEHIND_RIGHT_STOP  BEHIND_RIGHT_F_RESET;BEHIND_RIGHT_B_RESET

#define SPEED_DUTY 40//默认占空比 按1ms最小分辨率 周期50ms计算

//指令定义
#define COMM_STOP  'I'//停止
#define COMM_UP    'A'//前进
#define COMM_DOWN  'B'//后退
#define COMM_LEFT  'C'//左转
#define COMM_RIGHT 'D'//右转

extern unsigned char tick_5ms;//5ms计数器，作为主函数的基本周期
extern unsigned char tick_1ms;//1ms计数器，作为电机的基本计数器
extern unsigned int speed_count;//占空比计数器 50次一周期
extern unsigned char bt_rec_flag;//蓝牙控制标志位

void delay_init(void);
void Delayms(u32 Nms);
void Delay_us(u32 Nus);
void LEDToggle(uint16_t Led);

void IRIN_Configuration(void);
void TIM2_Init(void);
void RedRayInit(void);
void ServoInit(void);
void GPIOCLKInit(void);
void UserLEDInit(void);

#endif

