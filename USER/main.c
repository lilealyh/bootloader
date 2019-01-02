#include "stm32f10x.h"
#include "flash.h"
#include "usart.h"
#include "updata.h"
#include <stdio.h>
#include <string.h>

#define Version	"V2018090801b"

uint8_t TenMsFlag;//10ms标志


uint32_t App_Base_Addr;

SystemParameter_str SystemParameter;
SystemParameter_str *pSystemParameter=&SystemParameter;



typedef  void (*pFunction)(void);
static pFunction Jump_To_Application;
static uint32_t JumpAddress;


extern uint8_t FLASH_ReadData(uint32_t startAddress,uint8_t *buff,uint16_t len);


/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
uint16_t count;
int main(void)
{
    SystemConfigure();
    FLASH_ReadData(SYSTEM_BASE_ADR,(uint8_t *)&pSystemParameter->h_version[0],sizeof(SystemParameter_str));
    if(pSystemParameter->app_addr==0xffffffff)
    {
        SystemParameterInit();
    }
    //版本回归
    for(count=0; count<512; count++)
    {
        if(FactoryResetDeal()==0)
        {
            break;
        }
    }
    //基址正确性检查
    if((pSystemParameter->app_addr==APP_BASE_ADR1)||(pSystemParameter->app_addr==APP_BASE_ADR2))
    {
        App_Base_Addr=pSystemParameter->app_addr;
    }
    else
    {
        App_Base_Addr=APP_BASE_ADR1;
    }
#if 0
    printf("Run At BootLoader\r\n");
    printf("addr:%08x\r\n",0X08000000);
    printf("ver:%s\r\n",Version);
    printf("SN:%s\r\n",pSystemParameter->sn);
    printf("Updaata Success Count:%d\r\n",pSystemParameter->s_count);
    printf("Updaata Fail Count:%d\r\n",pSystemParameter->e_count);
#endif


    /*启动应用*/
    if (((*(__IO uint32_t*)App_Base_Addr) & 0x2FFE0000 ) == 0x20000000)
    {
        count=USART1->DR;
        USART_ClearITPendingBit(USART1,USART_IT_ORE);
        USART_ClearITPendingBit(USART1,USART_IT_PE);
        USART_ClearITPendingBit(USART1,USART_IT_ERR);
        USART_Cmd(USART1, DISABLE);
        /* Jump to user application */
        JumpAddress = *(__IO uint32_t*) (App_Base_Addr + 4);
        Jump_To_Application = (pFunction) JumpAddress;
        /* Initialize user application's Stack Pointer */
        __set_MSP(*(__IO uint32_t*) App_Base_Addr);
        __asm("CPSID I");
        __asm("CPSID F");
        Jump_To_Application();
    }
}
