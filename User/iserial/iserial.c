#include "iserial.h"
#include "igpio.h"
#include "stm32f103xb.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "stm32f1xx_hal_rcc.h"
#include "stm32f1xx_hal_uart.h"
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

UART_HandleTypeDef uart1;

uint8_t rxbuf[DATA_BUF_SIZE];
uint8_t txbuf[DATA_BUF_SIZE];

void Led1_Init(void) {
    GPIO_InitTypeDef gpio;
    GPIO_CLK_ENABLE_BY_PORT(GPIOA);
    gpio.Pin = GPIO_PIN_5;
    gpio.Mode = GPIO_MODE_OUTPUT_OD;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &gpio);
}

void SerialInit(USART_TypeDef *usart, uint32_t baudRate, UART_HandleTypeDef *huart) {
    huart->Instance = usart;
    huart->Init.BaudRate = baudRate;
    huart->Init.WordLength = UART_WORDLENGTH_8B;
    huart->Init.Parity = UART_PARITY_NONE;
    huart->Init.StopBits = UART_STOPBITS_1;
    huart->Init.Mode = UART_MODE_TX_RX;
    huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    HAL_UART_Init(huart);

    __HAL_UART_ENABLE_IT(huart, UART_IT_IDLE);
    HAL_UART_Receive_IT(huart, rxbuf, DATA_BUF_SIZE);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef gpio;
    if (huart->Instance == USART1) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();

        gpio.Pin = GPIO_PIN_9;
        gpio.Mode = GPIO_MODE_AF_PP;
        gpio.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIOA, &gpio);

        gpio.Pin = GPIO_PIN_10;
        gpio.Mode = GPIO_MODE_AF_INPUT;
        gpio.Pull = GPIO_NOPULL;
        HAL_GPIO_Init(GPIOA, &gpio);

        HAL_NVIC_SetPriority(USART1_IRQn, 3, 0);
        HAL_NVIC_EnableIRQ(USART1_IRQn);

    }
}

void USART1_IRQHandler(void) {
    HAL_UART_IRQHandler(&uart1);

    if(__HAL_UART_GET_FLAG(&uart1, UART_FLAG_IDLE)){
		__HAL_UART_CLEAR_IDLEFLAG(&uart1);
		HAL_UART_AbortReceive_IT(&uart1);
	}
}

void HAL_UART_AbortReceiveCpltCallback(UART_HandleTypeDef *huart) {
    int32_t count = DATA_BUF_SIZE - __HAL_DMA_GET_COUNTER(uart1.hdmarx);
    if (count > 0) {
        memcpy(txbuf, rxbuf, count);
        HAL_UART_Transmit_IT(&uart1, txbuf, count);
    }
    HAL_UART_Receive_IT(&uart1, rxbuf, DATA_BUF_SIZE);
}

void u1_printf(char *fmt,...) {
	uint8_t tempbuff[256];
	uint16_t i;
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char *)tempbuff, fmt, ap);
	va_end(ap);
	
	for(i=0; i<strlen((char *)tempbuff); i++){
		while(!__HAL_UART_GET_FLAG(&uart1, UART_FLAG_TXE));
		uart1.Instance->DR = tempbuff[i];
	}
	while(!__HAL_UART_GET_FLAG(&uart1, UART_FLAG_TC));
}