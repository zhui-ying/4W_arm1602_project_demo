//serialportAPI
#include<stdio.h>  
#include <stdlib.h>
#include "serialportAPI.h"

static char _receive_buffer[_SS_MAX_RX_BUFF]; 
static volatile unsigned char _receive_buffer_tail = 0;
static volatile unsigned char _receive_buffer_head = 0;
unsigned char _buffer_overflow = 0;//溢出标志位

static void (*UartInit)(unsigned int);
static void (*UartSend)(unsigned char);

void UartBegin(unsigned int bps,void (*uart_init)(unsigned int),void (*uart_send)(unsigned char))
{
	UartInit = uart_init;
	UartSend = uart_send;
	UartInit(bps);
}

void ClearRxBuf(void)
{
	_receive_buffer_tail = 0;
	_receive_buffer_head = 0;
	_buffer_overflow = 0;
}

//发送
//调用的时候如果是整数，传入的必须是int型，并加上&符
void SerialPrint(void const *send_data,int type)
{
	if(type == STRING_TYPE)
	{
		char *data_temp = (char *)send_data;//强制类型转换
		while(*data_temp != '\0')
		{
			UartSend(*data_temp++);
		}
	}
	else if(type == INT_TYPE)
	{	
		char str[7];
		char *index = str;
		int data_temp = (int)(*(int *)send_data);
		sprintf(index, "%d" , data_temp);
		while(*index != '\0')
		{
			UartSend(*index++);
		}		
	}
}

//带换行的发送
//调用的时候如果是整数，传入的必须是int型，并加上&符
void SerialPrintln(void const *send_data,int type)
{
	SerialPrint(send_data,type);
	UartSend('\r');
	UartSend('\n');
}

void SerialWrite(char send_data)
{
	UartSend(send_data);
}

int SerialRead(void)
{
	unsigned char d;
	  // Empty buffer?
  if (_receive_buffer_head == _receive_buffer_tail)
    return -1;

  // Read from "head"
  d = _receive_buffer[_receive_buffer_head]; // grab next byte
  _receive_buffer_head = (_receive_buffer_head + 1) % _SS_MAX_RX_BUFF;
  return (int)d;
}

int SerialAvailable(void)
{
	return (_receive_buffer_tail + _SS_MAX_RX_BUFF - _receive_buffer_head) % _SS_MAX_RX_BUFF;
}

//call by UART interrupt
int SerialInt(unsigned char rec_data)
{
    // if buffer full, set the overflow flag and return
    if ((_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF != _receive_buffer_head) 
    {
      // save new data in buffer: tail points to where byte goes
      _receive_buffer[_receive_buffer_tail] = rec_data; // save new byte
      _receive_buffer_tail = (_receive_buffer_tail + 1) % _SS_MAX_RX_BUFF;
    } 
    else 
    {
      _buffer_overflow = 1;
    }	
		return 1;
}
