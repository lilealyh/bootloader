#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "updata.h"

uint8_t Uart1ReceiveBuff[BUFFR_MAX];
uint8_t InstructionType;


uint8_t AckFlag;
uint16_t AckOverTimeCount;//回码超时计数

uint16_t PackNum;
uint16_t PackCount;

uint16_t crc_value;
Protecl_str UpdataTemp;
char *out;
uint8_t UpDataFlag;

UsartStatus_str Usart1Status;
UsartStatus_str *pUsart1Status=&Usart1Status;

UsartProtocol_str Usart1ReceiveData;
UsartProtocol_str *pUsart1ReceiveData=&Usart1ReceiveData;

//函数声明
//外部变量声明
extern uint16_t UpdataOverTimeCount;
extern SystemParameter_str *pSystemParameter;

extern uint8_t FLASH_WriteData(uint32_t startAddress,uint8_t *buff,uint16_t len);


void Usart1_SendBuffData(void)
{
	;
}

uint8_t Char2Hex(char c)
{
	uint8_t temp;
	
	if((c>='0')&&(c<':'))//数字
	{
		temp=c-0x30;
	}
	else if((c>='A')&&(c<'G'))//大写字母
	{
		temp=c-0x41+10;
	}
	else if((c>='a')&&(c<'g'))//大写字母
	{
		temp=c-0x61+10;
	}
	else
	{
		temp=0;
	}
	return(temp);
}

uint16_t Usart1PackDataLen;//包数据长度

void Usart1ReceiveOpt(unsigned char mychar)
{	
	uint16_t i,temp;
	
	if((pUsart1Status->StartReceiveFlag1==0)&&((mychar=='e')||(mychar=='A')||(mychar=='R')||(mychar=='T')))
	{
		//接收包头
		if(pUsart1Status->StartReceiveFlag==0)
		{
			pUsart1Status->StartReceiveFlag=1;	
			pUsart1Status->StartReceiveFlag1=0;
			
			Uart1ReceiveBuff[0]=mychar;			
		}
		else if((pUsart1Status->StartReceiveFlag==1)&&(pUsart1Status->StartReceiveFlag1==0))
		{
			Uart1ReceiveBuff[1]=mychar;	
			
			if((Uart1ReceiveBuff[0]=='e')&&(Uart1ReceiveBuff[1]=='e'))
			{
				//透传指令
				pUsart1Status->StartReceiveFlag1=1;	
				pUsart1Status->ReceiveLenFlag=1;
			
				pUsart1Status->TimeOutCount=0;		
				pUsart1Status->ReceiveCount=2;//接收完成字节
				
				Usart1PackDataLen=0;
				
				InstructionType=1;
								
			}
			else if((Uart1ReceiveBuff[0]=='A')&&(Uart1ReceiveBuff[1]=='T')||(Uart1ReceiveBuff[0]=='R')&&(Uart1ReceiveBuff[1]=='T'))
			{
				//AT指令回码
				pUsart1Status->StartReceiveFlag1=1;	
				pUsart1Status->ReceiveLenFlag=1;
				
				pUsart1Status->TimeOutCount=0;		
				pUsart1Status->ReceiveCount=2;//接收完成字节
				
				Usart1PackDataLen=0;
				
				InstructionType=2;				
			}
		}
	}
	else if(InstructionType==1)
	{
		if((pUsart1Status->ReceiveLenFlag==1)&&(pUsart1Status->ReceiveCount<6))
		{
			//接收包长
			Uart1ReceiveBuff[pUsart1Status->ReceiveCount]=mychar;	
			
			pUsart1Status->ReceiveCount++;
			if(pUsart1Status->ReceiveCount==6)
			{
				temp=Char2Hex(Uart1ReceiveBuff[2]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[3]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[4]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[5]);
		
				Usart1PackDataLen=temp;//长度表示的是字节个数，以字符方式发送*2
				pUsart1ReceiveData->len=temp;
				if(Usart1PackDataLen<=BUFFR_MAX)
				{
					pUsart1Status->ReceiveDataFlag=1;
					pUsart1Status->ReceiveLenFlag=0;
				}
				else
				{
					//包长错误复位
					pUsart1Status->StartReceiveFlag=0;	
					pUsart1Status->StartReceiveFlag1=0;	
					pUsart1Status->ReceiveLenFlag=0;
				}
				
				pUsart1Status->TimeOutCount=0;		
			}
			
		}
		else if(pUsart1Status->ReceiveDataFlag>0)
		{
			//接收数据
			pUsart1Status->TimeOutCount=0;		
			
			if(pUsart1Status->ReceiveCount<(Usart1PackDataLen+6))
			{			
				Uart1ReceiveBuff[pUsart1Status->ReceiveCount]=mychar;			
				pUsart1Status->ReceiveCount++;
			}
			
			if(pUsart1Status->ReceiveCount>=(Usart1PackDataLen+6))
			{			
				pUsart1Status->ReceiveDataFlag=0;
				pUsart1Status->ReceiveCrcFlag=1;
				
	
			}	
			
		}		
		else if(pUsart1Status->ReceiveCrcFlag>0)
		{
			//接收CRC
			if(pUsart1Status->ReceiveCount<(Usart1PackDataLen+6+4))
			{
				Uart1ReceiveBuff[pUsart1Status->ReceiveCount]=mychar;			
				pUsart1Status->ReceiveCount++;
			}
			if(pUsart1Status->ReceiveCount>=(Usart1PackDataLen+6+4))
			{			
				pUsart1Status->ReceiveFinishFlag=1;		
				
				pUsart1ReceiveData->type=InstructionType;
				
				
				for(i=0;i<Usart1PackDataLen;i++)
				{
					pUsart1ReceiveData->data[i]=Uart1ReceiveBuff[6+i];
				}
				pUsart1ReceiveData->data[i]='\0';//添加字符串结束符
				
				temp=Char2Hex(Uart1ReceiveBuff[6+Usart1PackDataLen]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[6+Usart1PackDataLen+1]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[6+Usart1PackDataLen+2]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[6+Usart1PackDataLen+3]);
							
				pUsart1ReceiveData->crc=temp;			
				
				pUsart1Status->StartReceiveFlag=0;	
				pUsart1Status->StartReceiveFlag1=0;	
				pUsart1Status->ReceiveLenFlag=0;
				pUsart1Status->ReceiveDataFlag=0;
				pUsart1Status->ReceiveCrcFlag=0;
				InstructionType=0;
				Usart1PackDataLen=0;
			}
		}
	}
	else if(InstructionType==2)
	{
		if(pUsart1Status->ReceiveCount<(BUFFR_MAX-1))
		{
			Uart1ReceiveBuff[pUsart1Status->ReceiveCount]=mychar;				
			pUsart1Status->ReceiveCount++;
			
			pUsart1Status->TimeOutCount=0;		
			
			if(mychar=='\r')
			{
				//完成一包数据接收
				pUsart1ReceiveData->type=InstructionType;
				pUsart1Status->ReceiveFinishFlag=1;		
				InstructionType=0;
				
				Uart1ReceiveBuff[pUsart1Status->ReceiveCount-1]='\0';//添加字符串结束符
				
				for(i=0;i<pUsart1Status->ReceiveCount+1;i++)
				{
					pUsart1ReceiveData->data[i]=Uart1ReceiveBuff[i];
				}
				
				pUsart1Status->StartReceiveFlag=0;	
				pUsart1Status->StartReceiveFlag1=0;	
				pUsart1Status->ReceiveLenFlag=0;
				pUsart1Status->ReceiveDataFlag=0;
				pUsart1Status->ReceiveCrcFlag=0;
				InstructionType=0;
				Usart1PackDataLen=0;
			}
		}
	}
}


void Usart1_StartSendData(uint8_t *pdata,uint8_t len)
{	
//	pUsart1Status->TxCount=len;
//	pUsart1Status->Status=1;
//	//初始化发送指针
//	pUsart1Status->pSendBuff=Uart1SendBuff;
//	//开启发送缓冲空中断
//	USART_ITConfig(USART1, USART_IT_TXE, ENABLE);
	;
}


void Usart1OverTime(void)
{
	if((pUsart1Status->StartReceiveFlag>0)||(pUsart1Status->StartReceiveFlag1>0))
	{
		pUsart1Status->TimeOutCount++;
		if(pUsart1Status->TimeOutCount>50)
		{
			pUsart1Status->StartReceiveFlag=0;	
			pUsart1Status->StartReceiveFlag1=0;	
			pUsart1Status->ReceiveLenFlag=0;
			pUsart1Status->ReceiveDataFlag=0;
			pUsart1Status->ReceiveCrcFlag=0;
			pUsart1Status->ReceiveFinishFlag=0;	
			
			Usart1PackDataLen=0;
			pUsart1Status->TimeOutCount=0;
			pUsart1Status->ReceiveCount=0;
			InstructionType=0;
		}
	}

}
void Usart1ATCommandDeal(char *str)
{
	uint16_t i;
	char buff[6][32];	
	char *result = NULL;
	
	result = strtok( str, "," );
	i=0;
	while(result != NULL) 
	{
		sprintf(buff[i],"%s",result);
		i++;
		
		if(i>=6)
		{
			break;
		}
		result = strtok( NULL, "," );
	}
	
	if(strcmp(buff[0],"AT+SN")==0)
	{
		sprintf(pSystemParameter->sn,"%s",buff[1]);
        FLASH_WriteData(SYSTEM_BASE_ADR,(uint8_t *)&pSystemParameter->h_version[0],sizeof(SystemParameter_str));
  	
  	printf("SN:%s OK\r\n",pSystemParameter->sn);
	}
}

/**
功能：串口1接收数据处理
描述：对接收的透传协议数据进行处理
参数：*pdata：协议数据结构指针
返回值:无
*/

void Usart1ReceiveDeal(UsartProtocol_str *pdata)
{
}


int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	return ch;
}

