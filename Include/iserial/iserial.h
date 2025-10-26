#ifndef __SERIAL_H_
#define __SERIAL_H_

#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_uart.h"

#define SERIAL_1_Enable_IT 1
#define SERIAL_1_GPIO_Group GPIOA
#define SERIAL_1_GPIO_TX_Pin GPIO_PIN_9
#define SERIAL_1_GPIO_RX_Pin GPIO_PIN_10

#define SERIAL_2_Enable_TI 0
#define SERIAL_2_GPIO_Group GPIOA
#define SERIAL_2_GPIO_TX_Pin GPIO_PIN_2
#define SERIAL_2_GPIO_RX_Pin GPIO_PIN_3

#define SERIAL_3_Enable_TI 0
#define SERIAL_3_GPIO_Group GPIOB
#define SERIAL_3_GPIO_TX_Pin GPIO_PIN_10
#define SERIAL_3_GPIO_RX_Pin GPIO_PIN_11

#define USART_CLK_ENABLE_BY_PORT(usart)               \
  do {                                              \
    if ((usart) == USART1)      { __HAL_RCC_USART1_CLK_ENABLE(); } \
    else if ((usart) == USART2) { __HAL_RCC_USART2_CLK_ENABLE(); } \
    else if ((usart) == USART3) { __HAL_RCC_USART3_CLK_ENABLE(); } \
  } while (0)

#define DATA_BUF_SIZE 256
#define TX_RX_TIME_OUT 200
#define U1_BAUD_RATE 921600
#define U2_BAUD_RATE 921600
#define U3_BAUD_RATE 921600
  
void U1_Init(uint32_t bandrate);
void U2_Init(uint32_t bandrate);
void U3_Init(uint32_t bandrate);
void SerialInit(USART_TypeDef *usart, uint32_t baudRate, UART_HandleTypeDef *huart);
void Serial_Loop(UART_HandleTypeDef *huart);

extern UART_HandleTypeDef uart1;
extern UART_HandleTypeDef uart2;
extern UART_HandleTypeDef uart3;

#endif