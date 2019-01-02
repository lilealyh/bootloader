#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

#define BUFFR_MAX 1446
#define FRAME_HEADER 'E'

#define PACKOVERTIME	300				//��λ10����
#define FLASHWRITE_MAX	3				//FLASHʧ��д������

typedef struct _UsartProtocol_str
{
	uint8_t type;//������ 1��͸������ 2��AT����
	uint16_t len;//����
	char data[BUFFR_MAX];//����
	uint16_t crc;	//У��
}UsartProtocol_str;


typedef struct _UsartStatus_str
{	
	uint16_t TxCount;
	uint8_t Status;
	uint8_t PacketDelay;
	uint8_t StartReceiveFlag;
	uint8_t StartReceiveFlag1;
	uint8_t ReceiveLenFlag;
	uint8_t ReceiveDataFlag;
	uint8_t ReceiveCrcFlag;
	uint8_t TimeOutCount;
	uint16_t ReceiveCount;
	uint8_t ReceiveFinishFlag;
	uint8_t *pSendBuff;
}UsartStatus_str;


#endif

