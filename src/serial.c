/* Copyright (c) 2019 - 2020, Khang Hua, email: khanghua1505@gmail.com
 * All right reserved.
 *
 * This file is written and modified by Khang Hua.
 *
 * This model is free software; you can redistribute it and/or modify it under the terms
 * of the GNU Lesser General Public License; either version 2.1 of the License, or (at your option)
 * any later version. See the GNU Lesser General Public License for more details,
 *
 * This model is distributed in the hope that it will be useful.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stm32f10x.h>

void serial_init(void)
{
    /* Enable APB2 Clock for serial pin. */
    
    RCC_APB2PeriphClockCmd(
          RCC_APB2Periph_GPIOA | 
          RCC_APB2Periph_AFIO  | 
          RCC_APB2Periph_USART1, 
          ENABLE);
    
    /* GPIO Configuration. */
    
    GPIO_InitTypeDef RxPinConfig = 
    {
        .GPIO_Pin = GPIO_Pin_10,
        .GPIO_Mode = GPIO_Mode_IN_FLOATING,
    };
    GPIO_Init(GPIOA, &RxPinConfig);
    
    GPIO_InitTypeDef TxPinConfig = 
    {
        .GPIO_Pin = GPIO_Pin_9,
        .GPIO_Mode = GPIO_Mode_AF_PP,
        .GPIO_Speed = GPIO_Speed_50MHz,
    };
    GPIO_Init(GPIOA, &TxPinConfig);
    
    /* Serial Configuration. */
    
    USART_InitTypeDef UsartConfig = 
    {
        .USART_BaudRate = 115200,
        .USART_WordLength = USART_WordLength_8b,
        .USART_StopBits = USART_StopBits_1,
        .USART_Parity = USART_Parity_No,
        .USART_HardwareFlowControl = USART_HardwareFlowControl_None,
        .USART_Mode = USART_Mode_Rx | USART_Mode_Tx,
    };
    USART_Init(USART1, &UsartConfig);
    
    USART_Cmd(USART1, ENABLE);
}

void serial_enable(bool enabled)
{
    USART_Cmd(USART1, enabled ? ENABLE : DISABLE);
}

uint16_t kgetc(void)
{
    while(USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET)
    {
    }
    return USART_ReceiveData(USART1);
}

void kputc(uint8_t c)
{
    USART_SendData(USART1, c);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {
    }
}