#ifndef __U_UART_H
#define __U_UART_H

#include "stdint.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"

extern UART_HandleTypeDef uart1;

void UART1_Init(uint32_t baudRate);

#endif