#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "updata.h"

extern SystemParameter_str *pSystemParameter;
extern char SN[16];


extern void SystemConfigure(void);

//���غ�������
int8_t FLASH_ReadData(uint32_t startAddress,uint8_t *buff,uint16_t len);
uint8_t FLASH_WriteData(uint32_t startAddress,uint8_t *buff,uint16_t len);

/**
���ܣ��������
������ϵͳ�������
��������
����ֵ����
*/
void SystenReboot(void)
{
	  __set_FAULTMASK(1); // �ر������ж�
	 	NVIC_SystemReset(); // ��λ
}

/**
���ܣ�������ַ�ض���
��������
������BOOTLOADER�����ת�󣬸���������ַ
����ֵ����

*/
void VTOR_Redirect(void)
{
	uint32_t App_Base_Addr;
	
	//��ַ��ȷ�Լ��  
  if((pSystemParameter->app_addr==APP_BASE_ADR1)||(pSystemParameter->app_addr==APP_BASE_ADR2))
	{
		App_Base_Addr=pSystemParameter->app_addr;
	}
	else
	{
		App_Base_Addr=APP_BASE_ADR1;
	}
	
	SCB->VTOR = App_Base_Addr;
	
//	FLASH_ReadData(SYSTEM_BASE_ADR,(uint8_t *)&temp.h_version[0],sizeof(SystemParameter_str));
//	
//	App_Base_Addr=temp.app_addr;
//	
//	if((App_Base_Addr!=APP_BASE_ADR1)&&(App_Base_Addr!=APP_BASE_ADR2))
//  {
//  	//����Ĭ�ϵ�ַ:APP1��ַ
//  	App_Base_Addr=APP_BASE_ADR1;
//  }
//  
//  if(App_Base_Addr==APP_BASE_ADR1)
//  {
//  	SCB->VTOR = FLASH_BASE | (APP_BASE_ADR1-FLASH_BASE);
//  }
//  else
//  {
//  	SCB->VTOR = FLASH_BASE | (APP_BASE_ADR2-FLASH_BASE);
//  }
}
/**
���ܣ���ָ����ַ��ʼ��ȡ�������
������startAddress:��ʼ��ַ *buff����д�����ݻ��� len��д�볤��
������len ���ܱ�4����ʱ������ʱ+1
����ֵ��0���ɹ� ��0��ʧ��
*/

int8_t FLASH_ReadData(uint32_t startAddress,uint8_t *buff,uint16_t len)
{
	uint32_t temp;
	uint32_t addr_temp;
	uint16_t count;
	uint16_t i;
	
	//����ַ�Ƿ�Ϸ�
	if((startAddress<FLASH_BASE)||(startAddress>FLASH_BASE+FLASH_SIZE*1024))
	{
		return(1);
	}
	
	if((len%4)==0)
	{
		count=len/4;
	}
	else
	{
		count=len/4 +1;
	}

  for(i=0;i<count;i++)
  {
  	addr_temp=startAddress+i*4;
  	temp=*(__IO uint32_t*)addr_temp;
  	
		buff[i*4+3]=temp>>24;
		buff[i*4+2]=temp>>16;
		buff[i*4+1]=temp>>8;
		buff[i*4]=temp;
  }
  return(0);
}

/**
���ܣ���ָ����ַ��ʼд��������
������startAddress:��ʼ��ַ *buff����д�����ݻ��� len��д�볤��
����ֵ��0���ɹ� ��0��ʧ��
*/
uint8_t FLASH_WriteData(uint32_t startAddress,uint8_t *buff,uint16_t len)
{
	uint32_t offsetAddress;
	uint32_t sectorPosition;
	uint32_t sectorStartAddress;
	uint16_t dataIndex;
	uint32_t temp;
	
  if(startAddress<FLASH_BASE||((startAddress+len)>=(FLASH_BASE+1024*FLASH_SIZE)))
  {
    return(1);//�Ƿ���ַ
  }
  FLASH_Unlock();         //����д����
  
  offsetAddress=startAddress-FLASH_BASE;               //����ȥ��0X08000000���ʵ��ƫ�Ƶ�ַ
  sectorPosition=offsetAddress/PAGE_SIZE;            //����ҳ��ַ������STM32F103VET6Ϊ0~255  
  
  sectorStartAddress=sectorPosition*PAGE_SIZE+FLASH_BASE;    //��Ӧ�������׵�ַ
	if((offsetAddress%2048)==0)
	{
		//�����������
  	FLASH_ErasePage(sectorStartAddress);
  }
  
  if((len%4)==0)
  {
  	len=len/4;
  }
  else
  {
  	len=len/4 +1;
  }
  //д������
  for(dataIndex=0;dataIndex<len;dataIndex++)
  {
  	temp=buff[dataIndex*4+3];
  	temp=temp<<8;
  	temp=temp | buff[dataIndex*4+2];
  	temp=temp<<8;
  	temp=temp | buff[dataIndex*4+1];
  	temp=temp<<8;
  	temp=temp | buff[dataIndex*4];

    FLASH_ProgramWord(startAddress+dataIndex*4,temp);
  }
  
  FLASH_Lock();//����д����
  
  return(0);
}
/**
���ܣ��ָ�����APP
������ͨ�����ָ���������״̬���͵�ƽʱ�����3�룬�ָ�����APP��ϵͳ�����������ú���ÿ10ms����һ��
��������
����ֵ����
*/
uint16_t FactoryResetCount;
uint8_t FactoryResetDeal(void) 
{
	if(FactoryReset_Pin()==Bit_RESET)
	{
		FactoryResetCount++;
		if(FactoryResetCount>300)
		{
			FactoryResetCount=0;
			
			if(pSystemParameter->app_addr==APP_BASE_ADR1)
			{
				pSystemParameter->app_addr=APP_BASE_ADR2;
			}
			else
			{
				pSystemParameter->app_addr=APP_BASE_ADR1;
			}
			FLASH_WriteData(SYSTEM_BASE_ADR,(uint8_t *)&pSystemParameter->h_version[0],sizeof(SystemParameter_str));
			return(0);
			
//			if(pSystemParameter->app_addr!=pSystemParameter->old_addr)
//			{
//				if((pSystemParameter->old_addr==APP_BASE_ADR1)||(pSystemParameter->old_addr==APP_BASE_ADR2))
//				{
//					pSystemParameter->app_addr=pSystemParameter->old_addr;
//				}
//				else
//				{
//					pSystemParameter->app_addr=APP_BASE_ADR1;
//				}
//				
//	  		FLASH_WriteData(SYSTEM_BASE_ADR,(uint8_t *)&pSystemParameter->h_version[0],sizeof(SystemParameter_str));
//				printf("Restore the factory value default APP1 \r\n");
//				
//				//����
//		  	SystenReboot();
//		  }
		}
	}
    return 0;
}
/**
���ܣ��ϵ�״̬�ϱ�
�������ϵ��ÿ3���ϱ�һ��״̬���յ���������Ӧ֮��ֹͣ���ͣ��ú���ÿ10ms����һ��
��������
����ֵ����
*/




/**
���ܣ���ϵͳ�����ָ���ʼֵ
������������ֵ������汾�ָ�����ʼֵ���ú���������
��������
����ֵ����
*/
void SystemParameterInit(void)
{
	pSystemParameter->app_addr=APP_BASE_ADR1;
	pSystemParameter->old_addr=APP_BASE_ADR1;
	sprintf(pSystemParameter->ver,"%s","V1.00");
	pSystemParameter->crc=0xffff;
	pSystemParameter->s_count=0;
	pSystemParameter->e_count=0;
	
	FLASH_WriteData(SYSTEM_BASE_ADR,(uint8_t *)&pSystemParameter->h_version[0],sizeof(SystemParameter_str));
}
