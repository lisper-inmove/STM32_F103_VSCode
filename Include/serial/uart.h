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
#define ENABLE_USART1_IDLE_IT 0
// 是否使用 DMA
#define ENABLE_USART1_DMA 1

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


#define UART1_BaudRate 921600
// 最大接收数据长度一定要不大于txbuffer/rxbuffer，否则会发生内存越界，导致破坏 DMA_HandleTypeDef hdma_usart1_tx;DMA_HandleTypeDef hdma_usart1_rx;
#define UART1_RX_SIZE 20  
extern UART_HandleTypeDef uart;
extern uint8_t txbuff[UART1_RX_SIZE];
extern uint8_t rxbuff[UART1_RX_SIZE];
extern uint8_t rxstate;

void UART_Init(uint32_t baudRate);
void Set_UART1_DMA(UART_HandleTypeDef *huart);

#endif