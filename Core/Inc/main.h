/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_6
#define LED_GPIO_Port GPIOA
#define SW_Pin GPIO_PIN_0
#define SW_GPIO_Port GPIOA
#define Event_Out_GPIO_Port GPIOA
#define Event_Out_GPIO_Pin GPIO_PIN_3

// 串口通信USART1
#define USART_Port USART1
#define Serial_GPIO_Port GPIOA
#define Serial_GPIO_RX_Pin GPIO_PIN_10
#define Serial_GPIO_TX_Pin GPIO_PIN_9

// 串口通信USART2
// #define USART_Port USART2
// #define Serial_GPIO_Port GPIOA
// #define Serial_GPIO_RX_Pin GPIO_PIN_3
// #define Serial_GPIO_TX_Pin GPIO_PIN_2

// 串口通信USART3
// #define USART_Port USART3
// #define Serial_GPIO_Port GPIOB
// #define Serial_GPIO_RX_Pin GPIO_PIN_11
// #define Serial_GPIO_TX_Pin GPIO_PIN_10

#define UART1_BaudRate 921600
#define UART1_RX_SIZE 200

#define USART_CLK_ENABLE_BY_PORT(port)               \
  do {                                              \
    if ((port) == USART1)      { __HAL_RCC_USART1_CLK_ENABLE(); } \
    else if ((port) == USART2) { __HAL_RCC_USART2_CLK_ENABLE(); } \
    else if ((port) == USART3) { __HAL_RCC_USART3_CLK_ENABLE(); } \
  } while (0)
  
#define USART_CLK_DISABLE_BY_PORT(port)               \
  do {                                              \
    if ((port) == USART1)      { __HAL_RCC_USART1_CLK_DISABLE(); } \
    else if ((port) == USART2) { __HAL_RCC_USART2_CLK_DISABLE(); } \
    else if ((port) == USART3) { __HAL_RCC_USART3_CLK_DISABLE(); } \
  } while (0)

#define GPIO_CLK_ENABLE_BY_PORT(port)               \
  do {                                              \
    if ((port) == GPIOA)      { __HAL_RCC_GPIOA_CLK_ENABLE(); } \
    else if ((port) == GPIOB) { __HAL_RCC_GPIOB_CLK_ENABLE(); } \
    else if ((port) == GPIOC) { __HAL_RCC_GPIOC_CLK_ENABLE(); } \
  } while (0)

#define GPIO_CLK_DISABLE_BY_PORT(port)              \
  do {                                              \
    if ((port) == GPIOA)      { __HAL_RCC_GPIOA_CLK_DISABLE(); } \
    else if ((port) == GPIOB) { __HAL_RCC_GPIOB_CLK_DISABLE(); } \
    else if ((port) == GPIOC) { __HAL_RCC_GPIOC_CLK_DISABLE(); } \
  } while (0)

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
