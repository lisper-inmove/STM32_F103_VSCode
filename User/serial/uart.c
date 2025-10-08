#include "uart.h"
#include "main.h"

UART_HandleTypeDef uart;

void UART_Init(uint32_t baudRate) {
	uart.Instance = USART_Port;
	uart.Init.BaudRate = baudRate;
	uart.Init.WordLength = UART_WORDLENGTH_8B;
	uart.Init.StopBits = UART_STOPBITS_1;
	uart.Init.Parity = UART_PARITY_NONE;
	uart.Init.Mode = UART_MODE_TX_RX;
	uart.Init.HwFlowCtl = UART_HWCONTROL_NONE;

    HAL_UART_Init(&uart);
}

void HAL_UART_MspInit(UART_HandleTypeDef *huart) {
    GPIO_InitTypeDef GPIO_InitType;
    GPIO_CLK_ENABLE_BY_PORT(Serial_GPIO_Port);
    USART_CLK_ENABLE_BY_PORT(USART_Port);

    GPIO_InitType.Pin = Serial_GPIO_TX_Pin;
    GPIO_InitType.Mode = GPIO_MODE_AF_PP;
    GPIO_InitType.Speed = GPIO_SPEED_FREQ_MEDIUM;
    HAL_GPIO_Init(Serial_GPIO_Port, &GPIO_InitType);

    GPIO_InitType.Pin = Serial_GPIO_RX_Pin;
    GPIO_InitType.Mode = GPIO_MODE_AF_INPUT;
    GPIO_InitType.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(Serial_GPIO_Port, &GPIO_InitType);
}