/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
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

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/


#define USART1_DEBUG_

extern uint8_t TenMsFlag;


extern void Usart1ReceiveOpt(unsigned char mychar);

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1) {

#ifdef  DEF_DEBUG_
        USART1_SendByte(0xee);
#endif
//		break;
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1) {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/


/**
  * @brief  This function handles UASRT1 interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
		uint8_t uchar;
	 	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
    {
     		USART_ClearFlag(USART1, USART_FLAG_ORE);
    }

  	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    {
      	USART_ClearITPendingBit(USART1, USART_IT_RXNE);
      	uchar=USART_ReceiveData(USART1);
      	Usart1ReceiveOpt(uchar);
    }
  	if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
  	{
//  		Usart1_SendBuffData();  	
  	}

}
/**
  * @brief  This function handles TIM4 interrupts request.
  * @param  None
  * @retval None
  */
void TIM4_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM4,  TIM_IT_Update ) != RESET) 
    {

        TIM_ClearITPendingBit(TIM4,TIM_IT_Update );
        TenMsFlag=1;
    }
}


/**
  * @brief  This function handles TIM5 interrupts request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM5,  TIM_IT_Update ) != RESET) {
       

        TIM_ClearITPendingBit(TIM5,TIM_IT_Update );
    }
}




/**
  * @brief  This function handles TIM6 interrupts request.
  * @param  None
  * @retval None
  */
void TIM6_IRQHandler(void)
{
    if(TIM_GetITStatus(TIM6,  TIM_IT_Update ) != RESET) {

        TIM_ClearITPendingBit(TIM6,TIM_IT_Update );
    }
}

/**
  * @brief  This function handles TIM7 interrupts request.
  * @param  None?????????0~2^32?0~4294967296
  * @retval None
  */
void TIM7_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM7,  TIM_IT_Update ) != RESET) {
   
        TIM_ClearITPendingBit(TIM7,TIM_IT_Update );
    }
}



/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0) != RESET) {
        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}
/**
  * @brief  This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line1) != RESET) {
        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line1);
    }
}
/**
  * @brief  This function handles External line 2 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI2_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line2) != RESET) {
        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line2);
    }
}
/**
  * @brief  This function handles External line 3 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI3_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line3) != RESET) {
        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line3);
    }
}
/**
  * @brief  This function handles External line 4 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI4_IRQHandler(void)
{
//	u8 i=0;

    if(EXTI_GetITStatus(EXTI_Line4) != RESET) {
    
       

//		Delay_1ms(1000);
//		SetPrimask(0x01);
//		SystemReset();

        /* Clear the  EXTI line 0 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line4);
    }
}


/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
	 led3on;
	 led3off;
  */
void EXTI9_5_IRQHandler(void)
{

    if(EXTI_GetITStatus(EXTI_Line5) != RESET) { //dec

        /* Clear the  EXTI line 5 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line5);
    }
    if(EXTI_GetITStatus(EXTI_Line6) != RESET) { //add
       
        /* Clear the  EXTI line 6 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line6);
    }
    if(EXTI_GetITStatus(EXTI_Line7) != RESET) { //set
    
        /* Clear the  EXTI line 7 pending bit */
        EXTI_ClearITPendingBit(EXTI_Line7);
    }








}







/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
