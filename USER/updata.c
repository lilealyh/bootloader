#include <stdio.h>
#include <string.h>
#include "stm32f10x.h"
#include "usart.h"
#include "updata.h"

extern SystemParameter_str *pSystemParameter;
extern char SN[16];


extern void SystemConfigure(void);

//本地函数声明
int8_t FLASH_ReadData(uint32_t startAddress,uint8_t *buff,uint16_t len);
uint8_t FLASH_WriteData(uint32_t startAddress,uint8_t *buff,uint16_t len);

/**
功能：软件重启
描述：系统软件重启
参数：无
返回值：无
*/
void SystenReboot(void)
{
	  __set_FAULTMASK(1); // 关闭所有中断
	 	NVIC_SystemReset(); // 复位
}

/**
功能：向量地址重定向
参数：无
描述：BOOTLOADER完成跳转后，更新向量地址
返回值：无

*/
void VTOR_Redirect(void)
{
	uint32_t App_Base_Addr;
	
	//基址正确性检查  
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
//  	//启用默认地址:APP1地址
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
功能：从指定地址开始读取多个数据
参数：startAddress:开始地址 *buff：待写入数据缓存 len：写入长度
描述：len 不能被4整除时，计算时+1
返回值：0：成功 非0：失败
*/

int8_t FLASH_ReadData(uint32_t startAddress,uint8_t *buff,uint16_t len)
{
	uint32_t temp;
	uint32_t addr_temp;
	uint16_t count;
	uint16_t i;
	
	//检查地址是否合法
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
功能：从指定地址开始写入多个数据
参数：startAddress:开始地址 *buff：待写入数据缓存 len：写入长度
返回值：0：成功 非0：失败
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
    return(1);//非法地址
  }
  FLASH_Unlock();         //解锁写保护
  
  offsetAddress=startAddress-FLASH_BASE;               //计算去掉0X08000000后的实际偏移地址
  sectorPosition=offsetAddress/PAGE_SIZE;            //计算页地址，对于STM32F103VET6为0~255  
  
  sectorStartAddress=sectorPosition*PAGE_SIZE+FLASH_BASE;    //对应扇区的首地址
	if((offsetAddress%2048)==0)
	{
		//擦除这个扇区
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
  //写入数据
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
  
  FLASH_Lock();//上锁写保护
  
  return(0);
}
/**
功能：恢复出厂APP
描述：通过检测恢复出厂引脚状态，低电平时间大于3秒，恢复出厂APP，系统立即重启，该函数每10ms调用一次
参数：无
返回值：无
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
//				//重启
//		  	SystenReboot();
//		  }
		}
	}
    return 0;
}
/**
功能：上电状态上报
描述：上电后每3秒上报一次状态，收到服务器响应之后停止发送，该函数每10ms调用一次
参数：无
返回值：无
*/




/**
功能：将系统参数恢复初始值
描述：将计数值，软件版本恢复到初始值，该函数测试用
参数：无
返回值：无
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
