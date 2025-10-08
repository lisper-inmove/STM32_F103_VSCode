#ifndef __U_UART_H
#define __U_UART_H

#include "stdint.h"
#include "string.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"
#include "stm32f1xx_hal_dma.h"

// USART1,USART2,USART3之间切换
#define TEST_USART_NUMBER 1
// 是否使用USART1的重映射
#define ENABLE_USART1_REMAAP 0
// 是否使用USART1的中断
#define ENABLE_USART1_IT 1
// 是否使用空闲中断
#define ENABLE_USART1_IDLE_IT 1
// 是否使用 DMA
#define ENABLE_USART1_DMA 1

#define UART1_BaudRate 921600
#define UART1_RX_SIZE 200

#if TEST_USART_NUMBER && (TEST_USART_NUMBER == 1)
  // 串口通信USART1
  #define USART_Port USART1
  #if ENABLE_USART1_REMAAP
    // 将USART RX, TX 重映射到 PB6, PB7
    #define Serial_GPIO_Port GPIOB
    #define Serial_GPIO_RX_Pin GPIO_PIN_7
    #define Serial_GPIO_TX_Pin GPIO_PIN_6
  #elif !ENABLE_USART1_REMAAP
    #define Serial_GPIO_Port GPIOA
    #define Serial_GPIO_RX_Pin GPIO_PIN_10
    #define Serial_GPIO_TX_Pin GPIO_PIN_9
  #endif
#elif TEST_USART_NUMBER && (TEST_USART_NUMBER == 2) 
  // 串口通信USART2
  #define USART_Port USART2
  #define Serial_GPIO_Port GPIOA
  #define Serial_GPIO_RX_Pin GPIO_PIN_3
  #define Serial_GPIO_TX_Pin GPIO_PIN_2
#elif TEST_USART_NUMBER && (TEST_USART_NUMBER == 3)
  // 串口通信USART3
  #define USART_Port USART3
  #define Serial_GPIO_Port GPIOB
  #define Serial_GPIO_RX_Pin GPIO_PIN_11
  #define Serial_GPIO_TX_Pin GPIO_PIN_10
#endif

extern UART_HandleTypeDef uart;
extern uint8_t txbuff[64];
extern uint8_t rxbuff[64];
extern uint8_t rxstate;

void UART_Init(uint32_t baudRate);
void Set_UART1_DMA(UART_HandleTypeDef *huart);

#endif