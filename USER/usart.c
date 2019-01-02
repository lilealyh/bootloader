#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "updata.h"

uint8_t Uart1ReceiveBuff[BUFFR_MAX];
uint8_t InstructionType;


uint8_t AckFlag;
uint16_t AckOverTimeCount;//���볬ʱ����

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

//��������
//�ⲿ��������
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
	
	if((c>='0')&&(c<':'))//����
	{
		temp=c-0x30;
	}
	else if((c>='A')&&(c<'G'))//��д��ĸ
	{
		temp=c-0x41+10;
	}
	else if((c>='a')&&(c<'g'))//��д��ĸ
	{
		temp=c-0x61+10;
	}
	else
	{
		temp=0;
	}
	return(temp);
}

uint16_t Usart1PackDataLen;//�����ݳ���

void Usart1ReceiveOpt(unsigned char mychar)
{	
	uint16_t i,temp;
	
	if((pUsart1Status->StartReceiveFlag1==0)&&((mychar=='e')||(mychar=='A')||(mychar=='R')||(mychar=='T')))
	{
		//���հ�ͷ
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
				//͸��ָ��
				pUsart1Status->StartReceiveFlag1=1;	
				pUsart1Status->ReceiveLenFlag=1;
			
				pUsart1Status->TimeOutCount=0;		
				pUsart1Status->ReceiveCount=2;//��������ֽ�
				
				Usart1PackDataLen=0;
				
				InstructionType=1;
								
			}
			else if((Uart1ReceiveBuff[0]=='A')&&(Uart1ReceiveBuff[1]=='T')||(Uart1ReceiveBuff[0]=='R')&&(Uart1ReceiveBuff[1]=='T'))
			{
				//ATָ�����
				pUsart1Status->StartReceiveFlag1=1;	
				pUsart1Status->ReceiveLenFlag=1;
				
				pUsart1Status->TimeOutCount=0;		
				pUsart1Status->ReceiveCount=2;//��������ֽ�
				
				Usart1PackDataLen=0;
				
				InstructionType=2;				
			}
		}
	}
	else if(InstructionType==1)
	{
		if((pUsart1Status->ReceiveLenFlag==1)&&(pUsart1Status->ReceiveCount<6))
		{
			//���հ���
			Uart1ReceiveBuff[pUsart1Status->ReceiveCount]=mychar;	
			
			pUsart1Status->ReceiveCount++;
			if(pUsart1Status->ReceiveCount==6)
			{
				temp=Char2Hex(Uart1ReceiveBuff[2]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[3]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[4]);
				temp=(temp<<4)+Char2Hex(Uart1ReceiveBuff[5]);
		
				Usart1PackDataLen=temp;//���ȱ�ʾ�����ֽڸ��������ַ���ʽ����*2
				pUsart1ReceiveData->len=temp;
				if(Usart1PackDataLen<=BUFFR_MAX)
				{
					pUsart1Status->ReceiveDataFlag=1;
					pUsart1Status->ReceiveLenFlag=0;
				}
				else
				{
					//��������λ
					pUsart1Status->StartReceiveFlag=0;	
					pUsart1Status->StartReceiveFlag1=0;	
					pUsart1Status->ReceiveLenFlag=0;
				}
				
				pUsart1Status->TimeOutCount=0;		
			}
			
		}
		else if(pUsart1Status->ReceiveDataFlag>0)
		{
			//��������
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
			//����CRC
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
				pUsart1ReceiveData->data[i]='\0';//����ַ���������
				
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
				//���һ�����ݽ���
				pUsart1ReceiveData->type=InstructionType;
				pUsart1Status->ReceiveFinishFlag=1;		
				InstructionType=0;
				
				Uart1ReceiveBuff[pUsart1Status->ReceiveCount-1]='\0';//����ַ���������
				
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
//	//��ʼ������ָ��
//	pUsart1Status->pSendBuff=Uart1SendBuff;
//	//�������ͻ�����ж�
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
���ܣ�����1�������ݴ���
�������Խ��յ�͸��Э�����ݽ��д���
������*pdata��Э�����ݽṹָ��
����ֵ:��
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

