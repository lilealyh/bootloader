#include "stm32f10x.h"
#include "stm32f10x_conf.h"
#include "stm32f10x_adc.h"

GPIO_InitTypeDef GPIO_InitStructure;
EXTI_InitTypeDef EXTI_InitStructure;


void GPIO_Config(void)
{
    //* ����GPIOAʱ��*/
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOE, ENABLE);
    /* Enable AFIO clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_SetBits(GPIOD,GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6);
}

void USART1_Configuration(u32 baud)
{
    USART_InitTypeDef USART_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);

    // Configure USART Tx as alternate function push-pull
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART Rx as input floating
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    // Configure USART
    USART_InitStructure.USART_BaudRate = baud;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl =USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    /* USART������ */
    USART_Cmd(USART1, ENABLE);

    /* ����USART1�����жϣ����жϻ����USART1�������ݼĴ�����Ϊ��*/
    USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);

}


void TIM4_Config(void)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//  NVIC_InitTypeDef NVIC_InitStructure;
    /* TIM4 ʱ��ʹ�� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    TIM_DeInit(TIM4);
    TIM_InternalClockConfig(TIM4);
    TIM_TimeBaseStructure.TIM_Prescaler = (72-1);
    TIM_TimeBaseStructure.TIM_Period = (10000 - 1);//10ms��ʱ��
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);

    //ʹ����װ��
    TIM_ARRPreloadConfig(TIM4,ENABLE);
    TIM_ClearITPendingBit(TIM4, TIM_IT_Update);

    TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}


void NVIC_Config(void)
{
//    NVIC_InitTypeDef NVIC_InitStructure;

    /* ����USART1�ж� */
//  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
//  NVIC_Init(&NVIC_InitStructure);

//  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//  NVIC_Init(&NVIC_InitStructure);

}

void SystemConfigure(void)
{
    GPIO_Config();
    USART1_Configuration(115200);
//	 	TIM4_Config();
//	 	NVIC_Config();
}



