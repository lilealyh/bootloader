#ifndef __USART_H
#define __USART_H

#include "stm32f10x.h"

#define BUFFR_MAX 1446
#define FRAME_HEADER 'E'

#define PACKOVERTIME	300				//单位10毫秒
#define FLASHWRITE_MAX	3				//FLASH失败写最大次数

typedef struct _UsartProtocol_str
{
	uint8_t type;//包类型 1：透传数据 2：AT数据
	uint16_t len;//包长
	char data[BUFFR_MAX];//数据
	uint16_t crc;	//校验
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

