#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#define _SS_MAX_RX_BUFF 96

#define INT_TYPE 0
#define STRING_TYPE 1

void UartBegin(unsigned int bps,void (*uart_init)(unsigned int),void (*uart_send)(unsigned char));
void SerialPrint(void const *send_data,int type);
void SerialPrintln(void const *send_data,int type);
void SerialWrite(char send_data);
int SerialRead(void);
int SerialAvailable(void);
int SerialInt(unsigned char rec_data);
void ClearRxBuf(void);

#endif

