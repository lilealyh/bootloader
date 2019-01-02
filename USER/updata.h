#ifndef __UPDATA_H
#define __UPDATA_H

#include "stm32f10x.h"

/**
										STM32F103VE ��FLASH������

			0X08000000--> |----------------------|
										|    BootLader(64K)		 |
			0x08010000--> |----------------------|
										|                      |
										|    APP0(222K)        |
										|                      |
			0x08047800-->	|----------------------|
										|                      |
										|    APP1(222K)        |
										|                      |	
			0x0807f000-->	|----------------------|
										|    SYSTEM(4K)		     |
										|----------------------|


*/
#define APP_BASE_ADR1		0x08010000	
#define APP_BASE_ADR2		0x08047800	
#define SYSTEM_BASE_ADR	    0x0807f800

#define FLASH_SIZE 		512  	//��λK��STM32F103VE FLASH Ϊ512K
#define PAGE_SIZE   	2048    //��λ�ֽڣ���С�ɲ�����С


#define FactoryReset_Pin()	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)		//�ָ�����APP

typedef struct __SystemParameter_str
{
	char h_version[32];		//Ӳ���汾��
	uint32_t app_addr;		//����APP��ַ
	uint32_t old_addr;		//��APP��ַ
	char ver[16];					//����APP�汾
	uint16_t crc;					//�ļ�У��ֵ
	uint16_t s_count;			//�ɹ����´���
	uint16_t e_count;			//����ʧ�ܴ���
	uint32_t size;				//�ļ���С	
	char sn[16];					//���к�
	uint16_t uflag;				//������־
}SystemParameter_str;

typedef struct _Protecl_str
{
	uint16_t cmd;
	char sn[16];
	char ver[16];	
	uint16_t packcount;
	uint16_t crc;
	uint32_t tn;
	uint32_t size;
	uint32_t master;
}Protecl_str;

////�ⲿ��������
extern void SystemConfigure(void);
//extern uint8_t FLASH_ReadData(uint32_t startAddress,uint8_t *buff,uint16_t len);
extern uint8_t FactoryResetDeal(void);
extern void SystemParameterInit(void);
#endif


