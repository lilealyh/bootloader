#ifndef __UPDATA_H
#define __UPDATA_H

#include "stm32f10x.h"

/**
										STM32F103VE 主FLASH分区表

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

#define FLASH_SIZE 		512  	//单位K，STM32F103VE FLASH 为512K
#define PAGE_SIZE   	2048    //单位字节，最小可擦除大小


#define FactoryReset_Pin()	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)		//恢复出厂APP

typedef struct __SystemParameter_str
{
	char h_version[32];		//硬件版本号
	uint32_t app_addr;		//最新APP地址
	uint32_t old_addr;		//旧APP地址
	char ver[16];					//最新APP版本
	uint16_t crc;					//文件校验值
	uint16_t s_count;			//成功更新次数
	uint16_t e_count;			//更新失败次数
	uint32_t size;				//文件大小	
	char sn[16];					//序列号
	uint16_t uflag;				//升级标志
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

////外部函数声明
extern void SystemConfigure(void);
//extern uint8_t FLASH_ReadData(uint32_t startAddress,uint8_t *buff,uint16_t len);
extern uint8_t FactoryResetDeal(void);
extern void SystemParameterInit(void);
#endif


