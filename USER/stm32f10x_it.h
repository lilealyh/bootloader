/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"


#define CEL_RUN    1
#define CEL_STOP	 0
#define SET_NOSET_    0x00
#define SET_USART_ONE 0xfd
#define SET_USART_SEC 0xfe

#define SET_INTIT_       0
#define SET_MAXPOWER_    1
#define SET_BELOWPOWER_  2
#define SET_ECERGECLEAR_ 3
#define SET_BELOWTIME_   4
#define SET_VOICE_       5

//-----------------------------
extern  u8 MC74HC595num;
extern  u8 datafresh;
extern  volatile u8 read_run;
extern  volatile u8 exti_run;
extern  u8 open_rn8209;


extern  u32 setmode_num;
extern 	volatile u32 setmode_fresh;
extern  volatile u32 showmode_fresh;
volatile u32 shown_fresh;
extern  u32 global_maxpower;
extern  u32 global_belowpower;
extern  u32 global_voice;
extern  u32 global_energe;
extern  u32 global_shownum;
extern  u32 global_showdata;
extern  u32 percent_time;
extern  u32 add_percentime;

extern volatile int rxfial_flag;
extern volatile int rxfialcmd;
extern  volatile u8 EXTI2_flag;

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
