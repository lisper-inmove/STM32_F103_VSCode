#include "uart.h"
#include "main.h"

UART_HandleTypeDef uart1;

void UART1_Init(uint32_t baudRate) {
	uart1.Instance = USART1;
	uart1.Init.BaudRate = baudRate;
	uart1.Init.WordLength = UART_WORDLENGTH_8B;
	uart1.Init.StopBits = UART_STOPBITS_1;
	uart1.Init.Parity = UART_PARITY_NONE;
	uart1.Init.Mode = UART_MODE_TX_RX;
	uart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    HAL_UART_Init(&uart1);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitType;
    if (huart->Instance == USART1) {
        __HAL_RCC_GPIOA_CLK_ENABLE();
        __HAL_RCC_USART1_CLK_ENABLE();

        GPIO_InitType.Pin = GPIO_PIN_9;
        GPIO_InitType.Mode = GPIO_MODE_AF_PP;
        GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
        HAL_GPIO_Init(GPIOA, &GPIO_InitType);

        GPIO_InitType.Pin = GPIO_PIN_10;
        GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
        GPIO_InitType.Pull = GPIO_PULLUP;
        HAL_GPIO_Init(GPIOA, &GPIO_InitType);
    }
}